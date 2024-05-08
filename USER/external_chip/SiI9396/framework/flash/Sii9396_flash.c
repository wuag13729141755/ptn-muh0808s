/*
  2016 (C) Lattice Semiconductor Corporation

  No part of this work may be reproduced, modified, distributed, transmitted,
  transcribed, or translated into any language or computer format, in any form
  or by any means without written permission of Lattice Semiconductor, Inc.,
  111 SW 5th Ave, Suite 700, Portland, OR 97204.

*/

#include "Sii9396_hal.h"
#include "Sii9396_device.h"
#include "Sii9396_osal.h"
#include "Sii9396_flash.h"

struct sii_flash_context {
	enum flash_status internal_status;
};

struct sii_flash_context flash_context[SII_INST_COUNT_MAX];

static void flash_set_status(sii_inst_t inst, enum flash_status status)
{
	uint8_t index = (uint8_t)inst;
	flash_context[index].internal_status = status;
}

enum flash_status flash_get_status(sii_inst_t inst)
{
	uint8_t index = (uint8_t)inst;
	return flash_context[index].internal_status;
}

enum sii_status flash_init_status(sii_inst_t inst)
{
	uint8_t index = (uint8_t)inst;
	flash_context[index].internal_status = FLASH_NOT_READY;
	return SII_STATUS_SUCCESS;
}

enum sii_status flash_init(sii_inst_t inst)
{
	enum sii_status ret = SII_STATUS_SUCCESS;
	if(SII_STATUS_SUCCESS != sii_device_reg_write_byte(inst, SII_PAGE_AP,
			SII_REG_AP_ISP_CTRL, BIT_MASK_ISP_CTRL__ISP_ENABLE)) {
		flash_set_status(inst, FLASH_NOT_READY);
		ret = SII_STATUS_ERR_FAILED;
	} else
		flash_set_status(inst, FLASH_READY);
	return ret;
}

static enum sii_status flash_erase(sii_inst_t inst)
{
	enum sii_status ret = SII_STATUS_SUCCESS;
	ret = sii_device_reg_write_byte(inst, SII_PAGE_AP, SII_REG_AP_ISP_OPCODE,
									BIT_MASK_ISP_OPCODE__WRITE_ENABLE);
	if (ret != SII_STATUS_SUCCESS)
		goto erase_exit;
	ret = sii_device_reg_write_byte(inst, SII_PAGE_AP, SII_REG_AP_ISP_OPCODE,
									BIT_MASK_ISP_OPCODE__ERASE_FLASH);
	if (ret != SII_STATUS_SUCCESS)
		goto erase_exit;
	/* Physical erase of flash need at least 2 seconds */
	sii_os_sleep_msec(2000);
erase_exit:
	return ret;
}

static enum sii_status flash_write_block(sii_inst_t inst, uint32_t address,
	uint16_t len, uint8_t *block)
{
	enum sii_status ret = SII_STATUS_SUCCESS;
	uint8_t addrees_buffer[4];
	addrees_buffer[0] = BIT_MASK_ISP_OPCODE__WRITE_BLOCK;
	addrees_buffer[1] = (address >> 16) & 0xff;
	addrees_buffer[2] = (address >> 8) & 0xff;
	addrees_buffer[3] = address & 0xff;

	ret = sii_device_reg_write_byte(inst, SII_PAGE_AP, SII_REG_AP_ISP_OPCODE,
									BIT_MASK_ISP_OPCODE__WRITE_ENABLE);
	if (ret != SII_STATUS_SUCCESS)
		goto write_exit;
	ret = sii_device_reg_write_block(inst, SII_PAGE_AP, SII_REG_AP_ISP_OPCODE,
			sizeof(addrees_buffer), addrees_buffer);
	if (ret != SII_STATUS_SUCCESS)
		goto write_exit;
	ret = sii_device_reg_write_block(inst, SII_PAGE_AP, SII_REG_AP_ISP_RX_DATA,
			len, block);
	if (ret != SII_STATUS_SUCCESS)
		goto write_exit;
write_exit:
	return ret;
}

static enum sii_status flash_read_block(sii_inst_t inst, uint32_t address,
	uint16_t len, uint8_t *block)
{
	enum sii_status ret = SII_STATUS_SUCCESS;
	uint8_t addrees_buffer[4];
	addrees_buffer[0] = BIT_MASK_ISP_OPCODE__READ_BLOCK;
	addrees_buffer[1] = (address >> 16) & 0xff;
	addrees_buffer[2] = (address >> 8) & 0xff;
	addrees_buffer[3] = address & 0xff;

	ret = sii_device_reg_write_block(inst, SII_PAGE_AP, SII_REG_AP_ISP_OPCODE,
			sizeof(addrees_buffer), addrees_buffer);
	if (ret != SII_STATUS_SUCCESS)
		goto read_exit;
	ret = sii_device_reg_read_block(inst, SII_PAGE_AP, SII_REG_AP_ISP_TX_DATA,
			len, block);
	if (ret != SII_STATUS_SUCCESS)
		goto read_exit;
read_exit:
	return ret;
}

enum sii_status flash_update(sii_inst_t inst, const uint8_t* file_name)
{
	enum sii_status ret = SII_STATUS_SUCCESS;
	uint8_t write_buffer[SII_ISP_PAGE_SIZE];
	uint8_t read_buffer[SII_ISP_PAGE_SIZE];
	uint8_t retry_times = 0;
	uint16_t op_size;
	uint32_t offset = 0;
	uint32_t file_size;
	int fd;
	if (flash_get_status(inst) != FLASH_READY) {
		dbg("flash status err!\n");
		ret = SII_STATUS_ERR_FAILED;
		goto init_error;
	}
	fd = sii_os_file_open(file_name, SII_FILE_FLAG_RDONLY,SII_FILE_RIGHT_IRUSR);
	if (fd <= 0) {
		dbg("open file err:%s!\n", file_name);
		ret = SII_STATUS_ERR_INVALID_PARAM;
	} else {
		flash_set_status(inst, FLASH_IN_PROGRESS);

		file_size = sii_os_get_file_length(fd);			//获取升级程序文件的大小
		dbg("file size: %d\n", file_size);

		/* First step: erase flash */
			flash_erase(inst);

		/* Second step: program flash by writing pages */
			while (offset < file_size) {
				if ((offset + SII_ISP_PAGE_SIZE) <= file_size)
					op_size = SII_ISP_PAGE_SIZE;
				else
					op_size = file_size - offset;
				dbg("program offset: %d\n", offset);

				//sii_os_file_read(fd, offset, op_size, write_buffer);
				//memcpy(write_buffer, gEdidTable, op_size);

				flash_write_block(inst, offset, op_size, write_buffer);
				offset += op_size;
			}
			dbg("flash program success!\n");

		/* Third step: verify flash by reading pages */
			offset = 0;
			while (offset < file_size) {
				if ((offset + SII_ISP_PAGE_SIZE) <= file_size)
					op_size = SII_ISP_PAGE_SIZE;
				else
					op_size = file_size - offset;
				dbg("verify offset: %d\n", offset);
				//sii_os_file_read(fd, offset, op_size, write_buffer);
retry:			flash_read_block(inst, offset, op_size, read_buffer);
				if (memcmp(write_buffer, read_buffer, op_size)) {
				if(retry_times < MAX_FLASH_RETRY_TIMES) {
						flash_write_block(inst, offset, op_size, write_buffer);
						goto retry;
					}
					dbg("verify err!\n");
					ret = SII_STATUS_ERR_FAILED;
					flash_set_status(inst, FLASH_VERIFY_ERR);
					goto verify_error;
				} else
					retry_times = 0;
				offset += op_size;
			}

		sii_os_file_close(fd);
		flash_set_status(inst, FLASH_UPDATE_SUCCESS);
		dbg("flash update success!\n");
	}

	return ret;
verify_error:
	sii_os_file_close(fd);
init_error:
	return ret;
}
