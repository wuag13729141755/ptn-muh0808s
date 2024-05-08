/*
  2016 (C) Lattice Semiconductor Corporation

  No part of this work may be reproduced, modified, distributed, transmitted,
  transcribed, or translated into any language or computer format, in any form
  or by any means without written permission of Lattice Semiconductor, Inc.,
  111 SW 5th Ave, Suite 700, Portland, OR 97204.

 */

#include "Sii9396_osal.h"
#include "Sii9396_device.h"
#include "Sii9396_ipc_hal_host.h"
#include "Sii9396_ipc_host.h"

/*
   Device HAL API for Host Side
 */

static enum sii_ipc_status sii_ipc_hal_host_write_byte(
		sii_inst_t inst, uint8_t offset, uint8_t value) {

	enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;
	enum sii_status sii_st = SII_STATUS_SUCCESS;

	sii_st = sii_device_reg_write_byte(
			inst, SII_PAGE_IPC_HOST, offset, value);

	if (sii_st != SII_STATUS_SUCCESS) {
		err("HAL failed!");
		status = SII_IPC_STATUS_ERROR_HAL_DEVICE;
		goto exit;
	}

exit:
	return status;
}

static enum sii_ipc_status sii_ipc_hal_host_write_block(
		sii_inst_t inst, uint8_t offset,
		uint8_t *buffer, uint8_t length) {

	enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;
	enum sii_status sii_st = SII_STATUS_SUCCESS;

	sii_st = sii_device_reg_write_block(
			inst, SII_PAGE_IPC_HOST, offset, length, buffer);

	if (sii_st != SII_STATUS_SUCCESS) {
		err("HAL failed!");
		status = SII_IPC_STATUS_ERROR_HAL_DEVICE;
		goto exit;
	}

exit:
	return status;
}

static enum sii_ipc_status sii_ipc_hal_host_read_byte(
		sii_inst_t inst, uint8_t offset, uint8_t *value) {

	enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;
	enum sii_status sii_st = SII_STATUS_SUCCESS;

	sii_st = sii_device_reg_read_byte(
			inst, SII_PAGE_IPC_HOST, offset, value);

	if (sii_st != SII_STATUS_SUCCESS) {
		err("HAL failed!");
		status = SII_IPC_STATUS_ERROR_HAL_DEVICE;
		goto exit;
	}

exit:
	return status;
}

static enum sii_ipc_status sii_ipc_hal_host_read_block(
		sii_inst_t inst, uint8_t offset,
		uint8_t *buffer, uint8_t length) {

	enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;
	enum sii_status sii_st = SII_STATUS_SUCCESS;

	sii_st = sii_device_reg_read_block(
			inst, SII_PAGE_IPC_HOST, offset, length, buffer);

	if (sii_st != SII_STATUS_SUCCESS) {
		err("HAL failed!");
		status = SII_IPC_STATUS_ERROR_HAL_DEVICE;
		goto exit;
	}

exit:
	return status;
}

/*
   IPC HAL API for Host Side
 */

enum sii_ipc_status sii_ipc_hal_host_init(sii_inst_t inst) {
	enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;

	//dbg("");

	/*
	   Init host interupt/mask, status, and buffer
	 */

	/* clear interrupt if any */
	status = sii_ipc_hal_set_host_interrupt(inst, false);
	if (status != SII_IPC_STATUS_SUCCESS) {
		goto exit;
	}

	/* clear status 1 */
	status = sii_ipc_hal_write_host_status1(inst, 0x00);
	if (status != SII_IPC_STATUS_SUCCESS) {
		goto exit;
	}

	/* clear status 2 */
	status = sii_ipc_hal_write_host_status2(inst, 0x00);
	if (status != SII_IPC_STATUS_SUCCESS) {
		goto exit;
	}

	sii_os_sleep_msec(10);

	/* clear buffer */
	status = sii_ipc_hal_clear_host_buffer(inst);
	if (status != SII_IPC_STATUS_SUCCESS) {
		goto exit;
	}

exit:
	return status;
}

enum sii_ipc_status sii_ipc_hal_host_acquire_mutex(sii_inst_t inst) {
	enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;

	/* FD: TBI, on hold, may not be needed */
	return status;
}

enum sii_ipc_status sii_ipc_hal_host_release_mutex(sii_inst_t inst) {
	enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;

	/* FD: TBI, on hold, may not be needed */
	return status;
}

enum sii_ipc_status sii_ipc_hal_set_host_interrupt(
		sii_inst_t inst, bool interrupt) {

	enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;
	uint8_t interrupt_byte = 0;

	//dbg("");

	status = sii_ipc_hal_host_read_byte(
			inst, SII_REG_IPC_HOST_INTERRUPT, &interrupt_byte);

	if (status != SII_IPC_STATUS_SUCCESS) {
		goto exit;
	}

	interrupt_byte = (interrupt ?
			interrupt_byte | SII_BIT_IPC_HOST_INTERRUPT :
			interrupt_byte & ~SII_BIT_IPC_HOST_INTERRUPT);

	status = sii_ipc_hal_host_write_byte(
			inst, SII_REG_IPC_HOST_INTERRUPT, interrupt_byte);

	if (status != SII_IPC_STATUS_SUCCESS) {
		goto exit;
	}

exit:
	return status;
}

enum sii_ipc_status sii_ipc_hal_get_host_interrupt(
		sii_inst_t inst, bool *interrupt) {

	enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;
	uint8_t interrupt_byte = 0;

	//dbg("");

	status = sii_ipc_hal_host_read_byte(
			inst, SII_REG_IPC_HOST_INTERRUPT, &interrupt_byte);
	if (status != SII_IPC_STATUS_SUCCESS) {
		goto exit;
	}

	*interrupt = (interrupt_byte & SII_BIT_IPC_HOST_INTERRUPT)
		? true : false;

exit:
	return status;
}

/* FD: TBU, not supported by HW */
/*
enum sii_ipc_status sii_ipc_hal_set_host_interrupt_mask(
		sii_inst_t inst, bool interrupt_mask) {

	enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;
	uint8_t interrupt_mask_byte = 0;

	status = sii_ipc_hal_host_read_byte(
			inst, SII_REG_IPC_HOST_INTERRUPT_MASK,
			&interrupt_mask_byte);

	if (status != SII_IPC_STATUS_SUCCESS) {
		goto exit;
	}

	interrupt_mask_byte = (interrupt_mask ?
			interrupt_mask_byte | SII_BIT_IPC_HOST_INTERRUPT_MASK :
			interrupt_mask_byte & ~SII_BIT_IPC_HOST_INTERRUPT_MASK);

	status = sii_ipc_hal_host_write_byte(
			inst, SII_REG_IPC_HOST_INTERRUPT, interrupt_mask_byte);

	if (status != SII_IPC_STATUS_SUCCESS) {
		goto exit;
	}

exit:
	return status;
}
*/

enum sii_ipc_status sii_ipc_hal_write_host_status1(
		sii_inst_t inst, uint8_t status1) {

	enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;
	uint8_t status1_reg = 0;

	status = sii_ipc_hal_host_read_byte(
			inst, SII_REG_IPC_HOST_STATUS1, &status1_reg);

	if (status != SII_IPC_STATUS_SUCCESS) {
		goto exit;
	}

	status1_reg = status1 | (status1_reg & SII_BIT_IPC_HOST_INTERRUPT);

	status = sii_ipc_hal_host_write_byte(
			inst, SII_REG_IPC_HOST_STATUS1, status1_reg);

	if (status != SII_IPC_STATUS_SUCCESS) {
		goto exit;
	}

exit:
	return status;
}

enum sii_ipc_status sii_ipc_hal_write_host_status2(
		sii_inst_t inst, uint8_t status2) {

	enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;
	uint8_t status2_reg = 0;

	//dbg("");

	status = sii_ipc_hal_host_read_byte(
			inst, SII_REG_IPC_HOST_STATUS2, &status2_reg);

	if (status != SII_IPC_STATUS_SUCCESS) {
		goto exit;
	}

	status2_reg = status2 | (status2_reg & SII_BIT_IPC_HOST_INTERRUPT_MASK);

	status = sii_ipc_hal_host_write_byte(
			inst, SII_REG_IPC_HOST_STATUS2, status2_reg);

	if (status != SII_IPC_STATUS_SUCCESS) {
		goto exit;
	}

exit:
	return status;
}

enum sii_ipc_status sii_ipc_hal_write_host_buffer(
		sii_inst_t inst, uint8_t *buffer, uint8_t length) {

	enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;

	//dbg("");

	if (length > SII_IPC_HOST_BUFFER_LENGTH || 0 == length) {
		status = SII_IPC_STATUS_ERROR_HAL_EXCEED_BUFFER_LIMIT;
		goto exit;
	}

	status = sii_ipc_hal_host_write_block(
			inst, SII_REG_IPC_HOST_BUFFER_START, buffer, length);

	if (status != SII_IPC_STATUS_SUCCESS) {
		goto exit;
	}

exit:
	return status;
}

enum sii_ipc_status sii_ipc_hal_write_host_buffer_command(
		sii_inst_t inst, uint8_t *buffer, uint8_t length) {

	enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;

	//dbg("");

	if (length > SII_IPC_HOST_BUF_CMD_LENGTH || 0 == length) {
		status = SII_IPC_STATUS_ERROR_HAL_EXCEED_BUFFER_LIMIT;
		goto exit;
	}

	status = sii_ipc_hal_host_write_block(
			inst, SII_REG_IPC_HOST_BUF_CMD_START, buffer, length);

	if (status != SII_IPC_STATUS_SUCCESS) {
		goto exit;
	}

exit:
	return status;
}

enum sii_ipc_status sii_ipc_hal_clear_host_buffer(sii_inst_t inst) {
	enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;
	uint8_t buffer_init[SII_IPC_HOST_BUFFER_LENGTH] = {0};

	//dbg("");

	status = sii_ipc_hal_host_write_block(
			inst, SII_REG_IPC_HOST_BUFFER_START,
			buffer_init, SII_IPC_HOST_BUFFER_LENGTH);

	if (status != SII_IPC_STATUS_SUCCESS) {
		goto exit;
	}

exit:
	return status;
}

enum sii_ipc_status sii_ipc_hal_read_host_status1(
		sii_inst_t inst, uint8_t *status1) {

	enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;

	//dbg("");

	status = sii_ipc_hal_host_read_byte(
			inst, SII_REG_IPC_HOST_STATUS1, status1);

	if (status != SII_IPC_STATUS_SUCCESS) {
		goto exit;
	} else {
		*status1 &= SII_BIT_IPC_HOST_STATUS1_MASK;
	}

exit:
	return status;
}

enum sii_ipc_status sii_ipc_hal_read_host_status2(
		sii_inst_t inst, uint8_t *status2) {

	enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;

	//dbg("");

	status = sii_ipc_hal_host_read_byte(
			inst, SII_REG_IPC_HOST_STATUS2, status2);

	if (status != SII_IPC_STATUS_SUCCESS) {
		goto exit;
	} else {
		*status2 &= SII_BIT_IPC_HOST_STATUS2_MASK;
	}

exit:
	return status;
}

enum sii_ipc_status sii_ipc_hal_read_host_status1_reg(
		sii_inst_t inst, uint8_t *status1) {

	enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;

	//dbg("");

	status = sii_ipc_hal_host_read_byte(
			inst, SII_REG_IPC_HOST_STATUS1, status1);

	if (status != SII_IPC_STATUS_SUCCESS) {
		goto exit;
	}

exit:
	return status;
}

enum sii_ipc_status sii_ipc_hal_read_host_status2_reg(
		sii_inst_t inst, uint8_t *status2) {

	enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;

	//dbg("");

	status = sii_ipc_hal_host_read_byte(
			inst, SII_REG_IPC_HOST_STATUS2, status2);

	if (status != SII_IPC_STATUS_SUCCESS) {
		goto exit;
	}

exit:
	return status;
}

enum sii_ipc_status sii_ipc_hal_read_host_buffer(
		sii_inst_t inst, uint8_t *buffer, uint8_t length) {

	enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;

	//dbg("");

	if (length > SII_IPC_HOST_BUFFER_LENGTH || 0 == length) {
		status = SII_IPC_STATUS_ERROR_HAL_EXCEED_BUFFER_LIMIT;
		goto exit;
	}

	status = sii_ipc_hal_host_read_block(
			inst, SII_REG_IPC_HOST_BUFFER_START, buffer, length);

	if (status != SII_IPC_STATUS_SUCCESS) {
		goto exit;
	}

exit:
	return status;
}

enum sii_ipc_status sii_ipc_hal_read_slave_status1(
		sii_inst_t inst, uint8_t *status1) {

	enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;

	//dbg("");

	status = sii_ipc_hal_host_read_byte(
			inst, SII_REG_IPC_SLAVE_STATUS1, status1);

	if (status != SII_IPC_STATUS_SUCCESS) {
		goto exit;
	} else {
		*status1 &= SII_BIT_IPC_SLAVE_STATUS1_MASK;
	}

exit:
	return status;
}

enum sii_ipc_status sii_ipc_hal_read_slave_status2(
		sii_inst_t inst, uint8_t *status2) {

	enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;

	//dbg("");

	status = sii_ipc_hal_host_read_byte(
			inst, SII_REG_IPC_SLAVE_STATUS2, status2);

	if (status != SII_IPC_STATUS_SUCCESS) {
		goto exit;
	} else {
		*status2 &= SII_BIT_IPC_SLAVE_STATUS2_MASK;
	}

exit:
	return status;
}

enum sii_ipc_status sii_ipc_hal_read_slave_status1_reg(
		sii_inst_t inst, uint8_t *status1) {

	enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;

	//dbg("");

	status = sii_ipc_hal_host_read_byte(
			inst, SII_REG_IPC_SLAVE_STATUS1, status1);

	if (status != SII_IPC_STATUS_SUCCESS) {
		goto exit;
	}

exit:
	return status;
}

enum sii_ipc_status sii_ipc_hal_read_slave_status2_reg(
		sii_inst_t inst, uint8_t *status2) {

	enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;

	//dbg("");

	status = sii_ipc_hal_host_read_byte(
			inst, SII_REG_IPC_SLAVE_STATUS2, status2);

	if (status != SII_IPC_STATUS_SUCCESS) {
		goto exit;
	}

exit:
	return status;
}

enum sii_ipc_status sii_ipc_hal_read_slave_buffer(
		sii_inst_t inst, uint8_t *buffer, uint8_t length) {

	enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;

	//dbg("");

	if (length > SII_IPC_SLAVE_BUFFER_LENGTH || 0 == length) {
		status = SII_IPC_STATUS_ERROR_HAL_EXCEED_BUFFER_LIMIT;
		goto exit;
	}

	status = sii_ipc_hal_host_read_block(
			inst, SII_REG_IPC_SLAVE_BUFFER_START, buffer, length);

	if (status != SII_IPC_STATUS_SUCCESS) {
		goto exit;
	}

exit:
	return status;
}

enum sii_ipc_status sii_ipc_hal_read_slave_buffer_reply(
		sii_inst_t inst, uint8_t *buffer, uint8_t length) {

	enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;

	//dbg("");

	if (length > SII_IPC_SLAVE_BUF_RPL_LENGTH || 0 == length) {
		status = SII_IPC_STATUS_ERROR_HAL_EXCEED_BUFFER_LIMIT;
		goto exit;
	}

	status = sii_ipc_hal_host_read_block(
			inst, SII_REG_IPC_SLAVE_BUF_RPL_START, buffer, length);

	if (status != SII_IPC_STATUS_SUCCESS) {
		goto exit;
	}

exit:
	return status;
}

enum sii_ipc_status sii_ipc_hal_read_slave_buffer_notification(
		sii_inst_t inst, uint8_t *buffer, uint8_t length) {

	enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;

	//dbg("");

	if (length > SII_IPC_SLAVE_BUF_NTF_LENGTH || 0 == length) {
		status = SII_IPC_STATUS_ERROR_HAL_EXCEED_BUFFER_LIMIT;
		goto exit;
	}

	status = sii_ipc_hal_host_read_block(
			inst, SII_REG_IPC_SLAVE_BUF_NTF_START, buffer, length);

	if (status != SII_IPC_STATUS_SUCCESS) {
		goto exit;
	}

exit:
	return status;
}

enum sii_ipc_status sii_ipc_hal_clear_slave_interrupt(sii_inst_t inst) {
	enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;
	uint8_t interrupt_byte = 0;

	//dbg("");

	status = sii_ipc_hal_host_read_byte(
			inst, SII_REG_IPC_SLAVE_INTERRUPT, &interrupt_byte);

	if (status != SII_IPC_STATUS_SUCCESS) {
		goto exit;
	}

	interrupt_byte = interrupt_byte | SII_BIT_IPC_SLAVE_INTERRUPT;

	status = sii_ipc_hal_host_write_byte(
			inst, SII_REG_IPC_SLAVE_INTERRUPT, interrupt_byte);

	if (status != SII_IPC_STATUS_SUCCESS) {
		goto exit;
	}

exit:
	return status;
}

enum sii_ipc_status sii_ipc_hal_set_slave_interrupt_mask(
		sii_inst_t inst, bool interrupt_mask) {

	enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;
	uint8_t interrupt_mask_byte = 0;

	//dbg("");

	status = sii_ipc_hal_host_read_byte(
			inst, SII_REG_IPC_SLAVE_INTERRUPT_MASK,
			&interrupt_mask_byte);

	if (status != SII_IPC_STATUS_SUCCESS) {
		goto exit;
	}

	interrupt_mask_byte = (interrupt_mask ?
			interrupt_mask_byte | SII_BIT_IPC_SLAVE_INTERRUPT_MASK :
			interrupt_mask_byte & ~SII_BIT_IPC_SLAVE_INTERRUPT_MASK);

	status = sii_ipc_hal_host_write_byte(
			inst, SII_REG_IPC_SLAVE_INTERRUPT_MASK, interrupt_mask_byte);

	if (status != SII_IPC_STATUS_SUCCESS) {
		goto exit;
	}

exit:
	return status;
}
