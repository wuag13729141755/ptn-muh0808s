/*
  2016 (C) Lattice Semiconductor Corporation

  No part of this work may be reproduced, modified, distributed, transmitted,
  transcribed, or translated into any language or computer format, in any form
  or by any means without written permission of Lattice Semiconductor, Inc.,
  111 SW 5th Ave, Suite 700, Portland, OR 97204.

 */
#include "includes.h"

#include "Sii9396_ipc_hal.h"
#include "Sii9396_ipc_host.h"

#include "Sii9396_osal.h"
#include "Sii9396_hal.h"
#include "Sii9396_device.h"

struct sii_device sii_dev[SII_DEV_COUNT_MAX];


// Ò³µÄÊµÀý
static BYTE m_acI2CBus[]=
{
    _I2C_TABLE_INDEX_Sil9396,
};


#if defined(USE_SPI)
enum sii_spi_opcode {
	SII_SPI_OPCODE_READ_BLOCK = 0x23,
	SII_SPI_OPCODE_WRITE = 0x02
};

enum sii_spi_codes {
	SII_SPI_CODE_READ_DUMMY = 0x00
};
#endif

sii_hwid_t hwid_to_inst_map[SII_DEV_COUNT_MAX] = {0};
sii_inst_t inst_to_hwid_map[SII_INST_COUNT_MAX] = {0};
#ifdef __KERNEL__
extern wait_queue_head_t sii9396_interrupt_wait;

DECLARE_WAIT_QUEUE_HEAD(sii9396_interrupt_wait);

#endif
enum sii_status sii_device_init(sii_inst_t inst, sii_hwid_t hwid) {
#ifdef __KERNEL__
	init_waitqueue_head(&sii9396_interrupt_wait);
#endif
	if (SII_STATUS_SUCCESS != sii_hal_init(hwid)) {
		return SII_STATUS_ERR_FAILED;
	}
	inst_to_hwid_map[inst] = hwid;
	hwid_to_inst_map[hwid] = inst;
	return SII_STATUS_SUCCESS;
}

void sii_device_term(sii_inst_t inst) {
#ifdef __KERNEL__
	wake_up_interruptible_sync(&sii9396_interrupt_wait);
#endif
	sii_hwid_t hwid = inst_to_hwid_map[inst];
	sii_hal_term(hwid);
	hwid_to_inst_map[hwid] = SII_HWID_NULL;
	inst_to_hwid_map[inst] = SII_INST_NULL;
}

void sii_device_reset(sii_inst_t inst, uint32_t time_msec)
{
	uint32_t reset_time = time_msec > SII_RESET_TIME ?
									time_msec : SII_RESET_TIME;
	sii_hwid_t hwid = inst_to_hwid_map[inst];
	sii_hal_reset(hwid, true);
	sii_os_sleep_msec(reset_time);
	sii_hal_reset(hwid, false);
	//sii_os_sleep_msec(SII_BOOT_TIME);
}

enum sii_status sii_device_int_pin_status(sii_inst_t inst, bool *status)
{
	sii_hwid_t hwid = inst_to_hwid_map[inst];
	return sii_hal_int_pin_status(hwid, status);
}

enum sii_status sii_device_isr_enable(sii_inst_t inst, bool enable)
{
	sii_hwid_t hwid = inst_to_hwid_map[inst];
	return sii_hal_isr_enable(hwid, enable);
}

void sii_device_isr(sii_hwid_t hwid) {
	enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;
	sii_inst_t inst = hwid_to_inst_map[hwid];

	status = sii_ipc_host_handler(inst);
	if (status != SII_IPC_STATUS_SUCCESS) {
		//warn("ipc handler doesn't return success.\n");
	}
}

int sii_device_reg_read_byte(
		sii_inst_t inst,
		uint8_t page_num,
		uint8_t offset,
		uint8_t *data_byte) {
	return sii_device_reg_read_block(inst, page_num, offset, 1, data_byte);
}

int sii_device_reg_write_byte(
		sii_inst_t inst,
		uint8_t page_num,
		uint8_t offset,
		uint8_t data_byte) {
	return sii_device_reg_write_block(inst, page_num, offset, 1, &data_byte);
}

int sii_device_reg_read_block(
		sii_inst_t inst,
		uint8_t page_num,
		uint8_t offset,
		uint16_t len,
		uint8_t *data_block) {

	enum sii_status retVal = SII_STATUS_SUCCESS;
	uint8_t  eResult = 0;
	sii_hwid_t hwid = inst_to_hwid_map[inst];
#if defined(USE_SPI)
	uint8_t *spi_tx_bytes = NULL;
	uint8_t *spi_rx_bytes = NULL;

	/* FD: TBR, use fixed buffer to improve performance, same in write_block */
	if (SII_HWID_NULL == hwid) {
		err("hwid is invalid\n");
		retVal = SII_STATUS_ERR_FAILED;
		goto exit;
	}
	/* Allocate the Transfer Buffer */
	spi_tx_bytes = (uint8_t *)
		sii_os_calloc("SPI TX Buffer", (len + 5), 0);
	if (NULL == spi_tx_bytes) {
		err("Out of memory\n");
		retVal = SII_STATUS_ERR_FAILED;
		goto exit;
	}

	/* Allocate the Receive Buffer */
	spi_rx_bytes = (uint8_t *)
		sii_os_calloc("SPI RX Buffer", (len + 5), 0);
	if (NULL == spi_rx_bytes) {
		err("Out of memory\n");
		retVal = SII_STATUS_ERR_FAILED;
		goto free_tx_bytes;
	}

	spi_tx_bytes[0] = SII_SPI_OPCODE_READ_BLOCK;
	spi_tx_bytes[1] = page_num;
	spi_tx_bytes[2] = offset;
	spi_tx_bytes[3] = SII_SPI_CODE_READ_DUMMY;
	spi_tx_bytes[4] = SII_SPI_CODE_READ_DUMMY;

	retVal = sii_hal_op(hwid, spi_tx_bytes, len + 5,
							spi_rx_bytes, len + 5);
	if (SII_STATUS_SUCCESS != retVal) {
		err("SPI read block failed!");
		goto free_rx_bytes;
	}

	memcpy(data_block, &(spi_rx_bytes[5]), len);

free_rx_bytes:
	sii_os_free(spi_rx_bytes);

free_tx_bytes:
	sii_os_free(spi_tx_bytes);

#elif defined(USE_I2C)

	//uint8_t i2c_tx_bytes[2];
	WORD realaddr=0;
	//i2c_tx_bytes[0] = page_num;
	//i2c_tx_bytes[1] = offset;
	realaddr = (page_num<<8)|offset;
	//device_slect_9396(inst);
    	eResult = i2c_read_bytes(  m_acI2CBus[inst],sii_dev[inst].slave_addr, realaddr, data_block,len, TRUE);
    	retVal = (eResult==0) ? SII_STATUS_SUCCESS : SII_STATUS_ERR_FAILED;

#endif
exit:
	return (int)retVal;
}

int sii_device_reg_write_block(
		sii_inst_t inst,
		uint8_t page_num,
		uint8_t offset,
		uint16_t len,
		uint8_t *data_block) {

	enum sii_status retVal = SII_STATUS_SUCCESS;
	uint8_t  eResult = 0;
	sii_hwid_t hwid = inst_to_hwid_map[inst];
	//uint8_t *tx_bytes = NULL;
	//uint8_t tx_bytes[512]={0};
	WORD realaddr=0;
#if defined(USE_SPI)

	/* Allocate the Transfer Buffer */
	tx_bytes = (uint8_t *)
		sii_os_calloc("SPI TX Buffer", (len + 3), 0);
	if (NULL == tx_bytes) {
		err("Out of memory\n");
		retVal = SII_STATUS_ERR_FAILED;
		goto exit;
	}
	tx_bytes[0] = SII_SPI_OPCODE_WRITE;
	tx_bytes[1] = page_num;
	tx_bytes[2] = offset;
	memcpy(&(tx_bytes[3]), data_block, len);

	retVal = sii_hal_op(hwid, tx_bytes, len + 3, NULL, 0);
	if (SII_STATUS_SUCCESS != retVal) {
		err("SPI write block failed!");
		goto free_tx_bytes;
	}
#elif defined(USE_I2C)
    //tx_bytes[0] = page_num;
	//tx_bytes[1] = offset;
	//memcpy(&(tx_bytes[2]), data_block, len);
	realaddr = (page_num<<8)|offset;
    //device_slect_9396(inst);
	eResult = i2c_write_bytes( m_acI2CBus[inst],sii_dev[inst].slave_addr, realaddr, data_block , len, TRUE );
	retVal = (eResult==0) ? SII_STATUS_SUCCESS : SII_STATUS_ERR_FAILED;
#endif
free_tx_bytes:
	//sii_os_free(tx_bytes);

exit:
	return (int)retVal;
}
