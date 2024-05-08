/*
  2016 (C) Lattice Semiconductor Corporation

  No part of this work may be reproduced, modified, distributed, transmitted,
  transcribed, or translated into any language or computer format, in any form
  or by any means without written permission of Lattice Semiconductor, Inc.,
  111 SW 5th Ave, Suite 700, Portland, OR 97204.

 */

#ifndef _IPC_HOST_H
#define _IPC_HOST_H

#include "Sii9396_ipc.h"


/*
   IPC Internal Structure & Context for Host Side
 */

enum sii_ipc_host_state_machine {
	SII_IPC_HOST_SM_IDLE = 0,
	SII_IPC_HOST_SM_SEND_COMMAND,
	SII_IPC_HOST_SM_WAIT_REPLY,
	SII_IPC_HOST_SM_BUFFER_REPLY_IN_PROGRESS
};

struct sii_ipc_host_context {
	struct sii_ipc_message host_message;
	struct sii_ipc_packet host_packet;

	struct sii_ipc_packet_ntf host_packet_ntf;
	struct sii_ipc_packet_notification host_packet_notification;

	uint8_t packet_sequence;
	uint8_t packet_fragment;

	struct SiiOsSemaphore *host_ntf_queue_semaphore;
	struct sii_ipc_notification
		notification_queue[SII_IPC_NOTIFICATION_QUEUE_LENGTH];
	uint8_t notification_queue_head;
	uint8_t notification_queue_tail;
	uint8_t notification_queue_count;

	enum sii_ipc_host_state_machine host_sm;
	struct SiiOsSemaphore *host_semaphore;
	struct SiiOsSemaphore *host_reply_semaphore;
};

extern struct sii_ipc_host_context ipc_host_context[SII_INST_COUNT_MAX];

/*
   IPC Internal API for Host Side
 */

enum sii_ipc_status sii_ipc_host_enable_interrupt(sii_inst_t inst, bool enable);
enum sii_ipc_status sii_ipc_host_clear_interrupt(sii_inst_t inst);
enum sii_ipc_status sii_ipc_host_clear_and_enable_interrupt(sii_inst_t inst);

enum sii_ipc_status sii_ipc_host_acquire_mutex(sii_inst_t inst);
enum sii_ipc_status sii_ipc_host_release_mutex(sii_inst_t inst);

enum sii_ipc_status sii_ipc_host_take_reply_semaphore(sii_inst_t inst);
enum sii_ipc_status sii_ipc_host_give_reply_semaphore(sii_inst_t inst);

enum sii_ipc_status sii_ipc_host_push_queue_notification(sii_inst_t inst,
		struct sii_ipc_notification notification);

enum sii_ipc_status sii_ipc_host_pop_queue_notification(sii_inst_t inst,
		struct sii_ipc_notification *notification);

enum sii_ipc_status sii_ipc_host_check_change(
		sii_inst_t inst,
		bool *new_reply,
		bool *new_notification);

enum sii_ipc_status sii_ipc_sync_with_slave_before_send_command(
		sii_inst_t inst);

enum sii_ipc_status sii_ipc_send_register_command(
		sii_inst_t inst,
		struct sii_ipc_register_command command);

enum sii_ipc_status sii_ipc_send_buffer_command(
		sii_inst_t inst,
		struct sii_ipc_packet_command *packet_command,
		bool need_buffer_reply);

enum sii_ipc_status sii_ipc_receive_register_reply(
		sii_inst_t inst,
		struct sii_ipc_register_reply *reply);

enum sii_ipc_status sii_ipc_receive_buffer_reply(
		sii_inst_t inst,
		struct sii_ipc_packet_reply *packet_reply,
		bool *more_packet);

enum sii_ipc_status sii_ipc_receive_register_notification(
		sii_inst_t inst,
		struct sii_ipc_register_notification *notification);

enum sii_ipc_status sii_ipc_receive_buffer_notification(
		sii_inst_t inst,
		struct sii_ipc_packet_notification *packet_notification);

enum sii_ipc_status sii_ipc_extract_notification_from_packet_ntf(
		sii_inst_t inst,
		struct sii_ipc_packet_ntf *packet_ntf);

/*
   IPC External API for Host Side
 */

enum sii_ipc_status sii_ipc_init(sii_inst_t inst);
enum sii_ipc_status sii_ipc_exit(sii_inst_t inst);

enum sii_ipc_status sii_ipc_host_handler(sii_inst_t inst);

enum sii_ipc_status sii_execute_register_command_no_reply(
		sii_inst_t inst,
		struct sii_ipc_register_command command);

enum sii_ipc_status sii_execute_register_command_register_reply(
		sii_inst_t inst,
		struct sii_ipc_register_command command,
		struct sii_ipc_register_reply *reply);

enum sii_ipc_status sii_execute_register_command_message_reply(
		sii_inst_t inst,
		struct sii_ipc_register_command command,
		struct sii_ipc_message_reply *message_reply);

enum sii_ipc_status sii_execute_message_command_register_reply(
		sii_inst_t inst,
		struct sii_ipc_message_command *message_command,
		struct sii_ipc_register_reply *reply);

enum sii_ipc_status sii_execute_message_command_message_reply(
		sii_inst_t inst,
		struct sii_ipc_message_command *message_command,
		struct sii_ipc_message_reply *message_reply);

#endif /* !_IPC_HOST_H */
