/*
  2016 (C) Lattice Semiconductor Corporation

  No part of this work may be reproduced, modified, distributed, transmitted,
  transcribed, or translated into any language or computer format, in any form
  or by any means without written permission of Lattice Semiconductor, Inc.,
  111 SW 5th Ave, Suite 700, Portland, OR 97204.

 */

#ifndef _DEVICE_H
#define _DEVICE_H

#include "Sii9396_sii_common.h"

//#define USE_SPI
#define USE_I2C

enum sii_device_page {
	SII_PAGE_AP = 0x1F,
	SII_PAGE_EMSC = 0xFF
};

enum sii_device_ap_reg {
	SII_REG_AP_AP_BUF_00 = 0x00,
	SII_REG_AP_FW_BUF_00 = 0x40,
	SII_REG_AP_AP_CTRL0 = 0x80,
	SII_REG_AP_FW_CTRL0 = 0x81,
	SII_REG_AP_AP_CTRL1 = 0x82,
	SII_REG_AP_FW_CTRL1 = 0x83,

	SII_REG_AP_ISP_OPCODE = 0x90,
#define BIT_MASK_ISP_OPCODE__WRITE_ENABLE   0x06
#define BIT_MASK_ISP_OPCODE__WRITE_BLOCK	0x02
#define BIT_MASK_ISP_OPCODE__READ_BLOCK	 0x03
#define BIT_MASK_ISP_OPCODE__ERASE_FLASH	0x60
	SII_REG_AP_ISP_RX_DATA = 0x94,
	SII_REG_AP_ISP_TX_DATA = 0x95,
	SII_REG_AP_ISP_CTRL = 0xA0,
#define BIT_MASK_ISP_CTRL__ISP_ENABLE	   0x01
	SII_REG_AP_MAXRANGE = 0xAD		/* guard value */
};

struct sii_device {
	sii_hwid_t hwid;
	bool in_use;
	bool in_reset;
	bool isr_enable;
	BYTE pin_reset;		//CS
	BYTE pin_int;			//MOSI
	BYTE  slave_addr;
	//Aardvark i2cdev;
	struct SiiOsSemaphore *sem;
	//struct thread_info isr;
};


#ifdef SII_IPC_HOST
enum sii_status sii_device_init(sii_inst_t inst, sii_hwid_t hwid);
void sii_device_term(sii_inst_t inst);
void sii_device_reset(sii_inst_t inst, uint32_t time_msec);
enum sii_status sii_device_isr_enable(sii_inst_t inst, bool enable);
void sii_device_isr(sii_hwid_t hwid);

enum sii_status sii_device_int_pin_status(sii_inst_t inst, bool *status);
#endif

#ifdef SII_IPC_SLAVE
enum sii_status sii_device_init(void);
void sii_device_term(void);
#endif

int sii_device_reg_read_byte(
#ifdef SII_IPC_HOST
		sii_inst_t inst,
#endif
		uint8_t page_num,
		uint8_t offset,
		uint8_t *data_byte);

int sii_device_reg_write_byte(
#ifdef SII_IPC_HOST
		sii_inst_t inst,
#endif
		uint8_t page_num,
		uint8_t offset,
		uint8_t data_byte);

int sii_device_reg_read_block(
#ifdef SII_IPC_HOST
		sii_inst_t inst,
#endif
		uint8_t page_num,
		uint8_t offset,
		uint16_t len,
		uint8_t *data_block);

int sii_device_reg_write_block(
#ifdef SII_IPC_HOST
		sii_inst_t inst,
#endif
		uint8_t page_num,
		uint8_t offset,
		uint16_t len,
		uint8_t *data_block);
#endif /* !_DEVICE_H */
