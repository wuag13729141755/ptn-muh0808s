/*
  2016 (C) Lattice Semiconductor Corporation

  No part of this work may be reproduced, modified, distributed, transmitted,
  transcribed, or translated into any language or computer format, in any form
  or by any means without written permission of Lattice Semiconductor, Inc.,
  111 SW 5th Ave, Suite 700, Portland, OR 97204.

*/

#ifndef _IPC_HAL_HOST_H
#define _IPC_HAL_HOST_H

#include "Sii9396_ipc_hal.h"


/*
   IPC HAL API for Host Side
 */

enum sii_ipc_status sii_ipc_hal_host_init(sii_inst_t inst);

enum sii_ipc_status sii_ipc_hal_host_acquire_mutex(sii_inst_t inst);
enum sii_ipc_status sii_ipc_hal_host_release_mutex(sii_inst_t inst);

enum sii_ipc_status sii_ipc_hal_set_host_interrupt(
		sii_inst_t inst, bool interrupt);
enum sii_ipc_status sii_ipc_hal_get_host_interrupt(
		sii_inst_t inst, bool *interrupt);

/* FD: TBU, not supported by HW */
/* enum sii_ipc_status sii_ipc_hal_set_host_interrupt_mask(
		sii_inst_t inst, bool interrupt_mask); */

enum sii_ipc_status sii_ipc_hal_write_host_status1(
		sii_inst_t inst, uint8_t status1);
enum sii_ipc_status sii_ipc_hal_write_host_status2(
		sii_inst_t inst, uint8_t status2);
enum sii_ipc_status sii_ipc_hal_write_host_buffer(
		sii_inst_t inst, uint8_t *buffer, uint8_t length);
enum sii_ipc_status sii_ipc_hal_write_host_buffer_command(
		sii_inst_t inst, uint8_t *buffer, uint8_t length);
enum sii_ipc_status sii_ipc_hal_clear_host_buffer(sii_inst_t inst);

enum sii_ipc_status sii_ipc_hal_read_host_status1(
		sii_inst_t inst, uint8_t *status1);
enum sii_ipc_status sii_ipc_hal_read_host_status2(
		sii_inst_t inst, uint8_t *status2);
enum sii_ipc_status sii_ipc_hal_read_host_status1_reg(
		sii_inst_t inst, uint8_t *status1_reg);
enum sii_ipc_status sii_ipc_hal_read_host_status2_reg(
		sii_inst_t inst, uint8_t *status2_reg);
enum sii_ipc_status sii_ipc_hal_read_host_buffer(
		sii_inst_t inst, uint8_t *buffer, uint8_t length);

enum sii_ipc_status sii_ipc_hal_read_slave_status1(
		sii_inst_t inst, uint8_t *status1);
enum sii_ipc_status sii_ipc_hal_read_slave_status2(
		sii_inst_t inst, uint8_t *status2);
enum sii_ipc_status sii_ipc_hal_read_slave_status1_reg(
		sii_inst_t inst, uint8_t *status1_reg);
enum sii_ipc_status sii_ipc_hal_read_slave_status2_reg(
		sii_inst_t inst, uint8_t *status2_reg);
enum sii_ipc_status sii_ipc_hal_read_slave_buffer(
		sii_inst_t inst, uint8_t *buffer, uint8_t length);
enum sii_ipc_status sii_ipc_hal_read_slave_buffer_reply(
		sii_inst_t inst, uint8_t *buffer, uint8_t length);
enum sii_ipc_status sii_ipc_hal_read_slave_buffer_notification(
		sii_inst_t inst, uint8_t *buffer, uint8_t length);

enum sii_ipc_status sii_ipc_hal_clear_slave_interrupt(sii_inst_t inst);
enum sii_ipc_status sii_ipc_hal_set_slave_interrupt_mask(
		sii_inst_t inst, bool interrupt_mask);

#endif /* !_IPC_HAL_HOST_H */
