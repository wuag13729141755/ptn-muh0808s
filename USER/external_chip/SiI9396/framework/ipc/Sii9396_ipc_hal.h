/*
  2016 (C) Lattice Semiconductor Corporation

  No part of this work may be reproduced, modified, distributed, transmitted,
  transcribed, or translated into any language or computer format, in any form
  or by any means without written permission of Lattice Semiconductor, Inc.,
  111 SW 5th Ave, Suite 700, Portland, OR 97204.

*/

#ifndef _IPC_HAL_H
#define _IPC_HAL_H

#include "Sii9396_ipc.h"


/*
   IPC Pages
 */
enum sii_ipc_page {
	SII_PAGE_IPC_HOST = 0x1F,
	SII_PAGE_IPC_SLAVE = 0x10
};

/*
   IPC Host Interrupt and Mask
 */
enum sii_ipc_reg_host_interrupt_mask {
	SII_REG_IPC_HOST_INTERRUPT = 0x80,
	SII_REG_IPC_HOST_INTERRUPT_MASK = 0x82
};

enum sii_ipc_reg_host_interrupt_mask_bits {
	SII_BIT_IPC_HOST_INTERRUPT = 0x80,
	SII_BIT_IPC_HOST_INTERRUPT_MASK = 0x80
};

/*
   IPC Slave Interrupt and Mask
 */
enum sii_ipc_reg_slave_interrupt_mask {
	SII_REG_IPC_SLAVE_INTERRUPT = 0x81,
	SII_REG_IPC_SLAVE_INTERRUPT_MASK = 0x83
};

enum sii_ipc_reg_slave_interrupt_mask_bits {
	SII_BIT_IPC_SLAVE_INTERRUPT = 0x80,
	SII_BIT_IPC_SLAVE_INTERRUPT_MASK = 0x80,

	SII_BIT_IPC_SLAVE_INTERRUPT_SHIFT = 0x07,
	SII_BIT_IPC_SLAVE_INTERRUPT_MASK_SHIFT = 0x07
};

/*
   IPC Host Status Registers
 */
enum sii_ipc_reg_host_status {
	SII_REG_IPC_HOST_STATUS1 = 0x80,
#ifndef SII_IPC_HW_LIMITATION
	SII_REG_IPC_HOST_STATUS2 = 0x82
#else
	SII_REG_IPC_HOST_STATUS2 = 0x3F
#endif
};

enum sii_ipc_reg_host_status_bits {
	SII_BIT_IPC_HOST_STATUS1_MASK = 0x7F,
	SII_BIT_IPC_HOST_STATUS2_MASK = 0x7F,

	SII_BIT_IPC_HOST_STATUS1_CMD_OPCODE_MASK = 0x7F,
	SII_BIT_IPC_HOST_STATUS2_CMD_PARAMETER_MASK = 0x7C,
	SII_BIT_IPC_HOST_STATUS2_CHECKSUM_MASK = 0x03,

	SII_BIT_IPC_HOST_STATUS1_CMD_OPCODE_SHIFT = 0x00,
	SII_BIT_IPC_HOST_STATUS2_CMD_PARAMETER_SHIFT = 0x02,
	SII_BIT_IPC_HOST_STATUS2_CHECKSUM_SHIFT = 0x00
};

/*
   IPC Slave Status Registers
 */
enum sii_ipc_reg_slave_status {
	SII_REG_IPC_SLAVE_STATUS1 = 0x81,
#ifndef SII_IPC_HW_LIMITATION
	SII_REG_IPC_SLAVE_STATUS2 = 0x83
#else
	SII_REG_IPC_SLAVE_STATUS2 = 0x7F
#endif
};

enum sii_ipc_reg_slave_status_bits {
	SII_BIT_IPC_SLAVE_STATUS1_MASK = 0x7F,
	SII_BIT_IPC_SLAVE_STATUS2_MASK = 0x7F,

	SII_BIT_IPC_SLAVE_STATUS1_RPL_STATE_MASK = 0x40,
	SII_BIT_IPC_SLAVE_STATUS1_REPLY_STATUS_MASK = 0x30,
	SII_BIT_IPC_SLAVE_STATUS1_BUFFER_STATUS_MASK = 0x0C,
	SII_BIT_IPC_SLAVE_STATUS1_REPLY_RETURN_MASK = 0x03,
	SII_BIT_IPC_SLAVE_STATUS2_NTF_STATE_MASK = 0x40,
	SII_BIT_IPC_SLAVE_STATUS2_NOTIFICATION_TYPE_MASK = 0x3C,
	SII_BIT_IPC_SLAVE_STATUS2_CHECKSUM_MASK = 0x03,

	SII_BIT_IPC_SLAVE_STATUS1_RPL_STATE_SHIFT = 0x06,
	SII_BIT_IPC_SLAVE_STATUS1_REPLY_STATUS_SHIFT = 0x04,
	SII_BIT_IPC_SLAVE_STATUS1_BUFFER_STATUS_SHIFT = 0x02,
	SII_BIT_IPC_SLAVE_STATUS1_REPLY_RETURN_SHIFT = 0x00,
	SII_BIT_IPC_SLAVE_STATUS2_NTF_STATE_SHIFT = 0x06,
	SII_BIT_IPC_SLAVE_STATUS2_NOTIFICATION_TYPE_SHIFT = 0x02,
	SII_BIT_IPC_SLAVE_STATUS2_CHECKSUM_SHIFT = 0x00
};

/*
   IPC Host Buffer
 */
enum sii_ipc_reg_host_buffer {
	/* this is length of Host buffer, not a register */
	SII_IPC_HOST_BUFFER_LENGTH = SII_IPC_HW_BUFFER_LENGTH,

	SII_REG_IPC_HOST_BUFFER_START = 0x00,
	SII_REG_IPC_HOST_BUFFER_END = (SII_REG_IPC_HOST_BUFFER_START \
			+ SII_IPC_HOST_BUFFER_LENGTH - 1)
};

enum sii_ipc_reg_host_buf_cmd {
	/* this is length of Host buffer, not a register */
	SII_IPC_HOST_BUF_CMD_LENGTH = SII_IPC_HW_BUF_CMD_LENGTH,

	SII_REG_IPC_HOST_BUF_CMD_START = 0x00,
	SII_REG_IPC_HOST_BUF_CMD_END = (SII_REG_IPC_HOST_BUF_CMD_START \
			+ SII_IPC_HOST_BUF_CMD_LENGTH - 1)
};

/*
   IPC Slave Buffer
 */
enum sii_ipc_reg_slave_buffer {
	/* this is length of Slave buffer, not a register */
	SII_IPC_SLAVE_BUFFER_LENGTH = SII_IPC_HW_BUFFER_LENGTH,

	SII_REG_IPC_SLAVE_BUFFER_START = 0x40,
	SII_REG_IPC_SLAVE_BUFFER_END = (SII_REG_IPC_SLAVE_BUFFER_START \
			+ SII_IPC_SLAVE_BUFFER_LENGTH - 1)
};

enum sii_ipc_reg_slave_buf_rpl {
	/* this is length of Slave buffer, not a register */
	SII_IPC_SLAVE_BUF_RPL_LENGTH = SII_IPC_HW_BUF_RPL_LENGTH,

	SII_REG_IPC_SLAVE_BUF_RPL_START = 0x40,
	SII_REG_IPC_SLAVE_BUF_RPL_END = (SII_REG_IPC_SLAVE_BUF_RPL_START \
			+ SII_IPC_SLAVE_BUF_RPL_LENGTH - 1)
};

enum sii_ipc_reg_slave_buf_ntf {
	/* this is length of Slave buffer, not a register */
	SII_IPC_SLAVE_BUF_NTF_LENGTH = SII_IPC_HW_BUF_NTF_LENGTH,

	SII_REG_IPC_SLAVE_BUF_NTF_START = SII_REG_IPC_SLAVE_BUF_RPL_START \
			+ SII_IPC_SLAVE_BUF_RPL_LENGTH,
	SII_REG_IPC_SLAVE_BUF_NTF_END = (SII_REG_IPC_SLAVE_BUF_NTF_START \
			+ SII_IPC_SLAVE_BUF_NTF_LENGTH - 1)
};

#endif /* !_IPC_HAL_H */
