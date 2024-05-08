/*
  2016 (C) Lattice Semiconductor Corporation

  No part of this work may be reproduced, modified, distributed, transmitted,
  transcribed, or translated into any language or computer format, in any form
  or by any means without written permission of Lattice Semiconductor, Inc.,
  111 SW 5th Ave, Suite 700, Portland, OR 97204.

*/

#include "Sii9396_osal.h"
#include "Sii9396_ipc_host.h"
#include "Sii9396_ipc_hal_host.h"

#define POLL_METHOD	1
#define HOST_IPC_POLL	1

/*
   IPC Internal Structure & Context for Host Side
 */

struct sii_ipc_host_context ipc_host_context[SII_INST_COUNT_MAX];


/*
   IPC Internal API for Host Side
 */

enum sii_ipc_status sii_ipc_host_enable_interrupt(sii_inst_t inst, bool enable) {
	enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;

	/* control slave interrupt */
	status = sii_ipc_hal_set_slave_interrupt_mask(inst, enable);
	return status;
}

enum sii_ipc_status sii_ipc_host_clear_interrupt(sii_inst_t inst) {
	enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;

	status = sii_ipc_hal_clear_slave_interrupt(inst);
	return status;
}

enum sii_ipc_status sii_ipc_host_clear_and_enable_interrupt(sii_inst_t inst) {
	enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;

	status = sii_ipc_host_clear_interrupt(inst);
	if (status != SII_IPC_STATUS_SUCCESS) {
		goto exit;
	}

	status = sii_ipc_host_enable_interrupt(inst, true);
	if (status != SII_IPC_STATUS_SUCCESS) {
		goto exit;
	}

exit:
	return status;
}

enum sii_ipc_status sii_ipc_host_acquire_mutex(sii_inst_t inst) {
	enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;
	enum sii_os_status osal_status = SII_OS_STATUS_SUCCESS;

	osal_status = sii_os_semaphore_take(
			ipc_host_context[inst].host_semaphore,
			SII_IPC_TIMEOUT_HOST_WAIT_MS);
	if (SII_OS_STATUS_SUCCESS != osal_status) {
		if (SII_OS_STATUS_TIMEOUT == osal_status) {
			status = SII_IPC_STATUS_ERROR_OSAL_SEMAPHORE_TIMEOUT;
			warn("Take Host semaphore timeout!");
		} else {
			status = SII_IPC_STATUS_ERROR_OSAL_SEMAPHORE;
			warn("Cannot take Host semaphore!");
		}
	}

	return status;
}

enum sii_ipc_status sii_ipc_host_release_mutex(sii_inst_t inst) {
	enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;
	enum sii_os_status osal_status = SII_OS_STATUS_SUCCESS;

	osal_status = sii_os_semaphore_give(
			ipc_host_context[inst].host_semaphore);
	if (SII_OS_STATUS_SUCCESS != osal_status) {
		warn("Cannot give Host semaphore!");
		status = SII_IPC_STATUS_ERROR_OSAL_SEMAPHORE;
	}

	return status;
}

enum sii_ipc_status sii_ipc_host_take_reply_semaphore(sii_inst_t inst) {
	enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;
	enum sii_os_status osal_status = SII_OS_STATUS_SUCCESS;

	osal_status = sii_os_semaphore_take(
			ipc_host_context[inst].host_reply_semaphore,
			SII_IPC_TIMEOUT_HOST_WAIT_REPLY_MS);
	if (SII_OS_STATUS_SUCCESS != osal_status) {
		if (SII_OS_STATUS_TIMEOUT == osal_status) {
			status = SII_IPC_STATUS_ERROR_OSAL_SEMAPHORE_TIMEOUT;
			warn("Take Host Reply semaphore timeout!");
		} else {
			status = SII_IPC_STATUS_ERROR_OSAL_SEMAPHORE;
			warn("Cannot take Host Reply semaphore!");
		}
	}

	return status;
}

enum sii_ipc_status sii_ipc_host_give_reply_semaphore(sii_inst_t inst) {
	enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;
	enum sii_os_status osal_status = SII_OS_STATUS_SUCCESS;

	osal_status = sii_os_semaphore_give(
			ipc_host_context[inst].host_reply_semaphore);
	if (SII_OS_STATUS_SUCCESS != osal_status) {
		warn("Cannot give Host Reply semaphore!");
		status = SII_IPC_STATUS_ERROR_OSAL_SEMAPHORE;
	}

	return status;
}

enum sii_ipc_status sii_ipc_host_take_ntf_queue_semaphore(sii_inst_t inst) {
	enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;
	enum sii_os_status osal_status = SII_OS_STATUS_SUCCESS;

	osal_status = sii_os_semaphore_take(
			ipc_host_context[inst].host_ntf_queue_semaphore,
			SII_IPC_TIMEOUT_HOST_WAIT_NTF_QUEUE_MS);
	if (SII_OS_STATUS_SUCCESS != osal_status) {
		if (SII_OS_STATUS_TIMEOUT == osal_status) {
			status = SII_IPC_STATUS_ERROR_OSAL_SEMAPHORE_TIMEOUT;
			warn("Take Host Notification Queue semaphore timeout!");
		} else {
			status = SII_IPC_STATUS_ERROR_OSAL_SEMAPHORE;
			warn("Cannot take Host Notification Queue semaphore!");
		}
	}

	return status;
}

enum sii_ipc_status sii_ipc_host_give_ntf_queue_semaphore(sii_inst_t inst) {
	enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;
	enum sii_os_status osal_status = SII_OS_STATUS_SUCCESS;

	osal_status = sii_os_semaphore_give(
			ipc_host_context[inst].host_ntf_queue_semaphore);
	if (SII_OS_STATUS_SUCCESS != osal_status) {
		warn("Cannot give Host Notification Queue semaphore!");
		status = SII_IPC_STATUS_ERROR_OSAL_SEMAPHORE;
	}

	return status;
}

enum sii_ipc_status sii_ipc_host_push_queue_notification(
		sii_inst_t inst, struct sii_ipc_notification notification) {
	enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;
	enum sii_ipc_status status2 = SII_IPC_STATUS_SUCCESS;

	status = sii_ipc_host_take_ntf_queue_semaphore(inst);
	if (status != SII_IPC_STATUS_SUCCESS) {
		goto exit_last;
	}

	if (ipc_host_context[inst].notification_queue_count
			>= SII_IPC_NOTIFICATION_QUEUE_LENGTH) {
		status = SII_IPC_STATUS_ERROR_NOTIFICATION_QUEUE_FULL;
		goto exit;
	}

	memcpy(&(ipc_host_context[inst].notification_queue
			[ipc_host_context[inst].notification_queue_tail]),
		&notification,
		sizeof(notification));

	ipc_host_context[inst].notification_queue_count++;

	ipc_host_context[inst].notification_queue_tail =
		(ipc_host_context[inst].notification_queue_tail + 1)
		% SII_IPC_NOTIFICATION_QUEUE_LENGTH;

exit:
	status2 = sii_ipc_host_give_ntf_queue_semaphore(inst);
	if (status2 != SII_IPC_STATUS_SUCCESS) {
		goto exit;
	}

exit_last:

	return status;
}

enum sii_ipc_status sii_ipc_host_pop_queue_notification(
		sii_inst_t inst, struct sii_ipc_notification *notification) {
	enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;
	enum sii_ipc_status status2 = SII_IPC_STATUS_SUCCESS;

	status = sii_ipc_host_take_ntf_queue_semaphore(inst);
	if (status != SII_IPC_STATUS_SUCCESS) {
		goto exit_last;
	}

	if (0 >= ipc_host_context[inst].notification_queue_count) {
		status = SII_IPC_STATUS_ERROR_NOTIFICATION_QUEUE_EMPTY;
		goto exit;
	}

	memcpy(notification,
		&(ipc_host_context[inst].notification_queue
			[ipc_host_context[inst].notification_queue_head]),
		sizeof(*notification));

	ipc_host_context[inst].notification_queue_count--;

	ipc_host_context[inst].notification_queue_head =
		(ipc_host_context[inst].notification_queue_head + 1)
		% SII_IPC_NOTIFICATION_QUEUE_LENGTH;

exit:
	status2 = sii_ipc_host_give_ntf_queue_semaphore(inst);
	if (status2 != SII_IPC_STATUS_SUCCESS) {
		goto exit;
	}

exit_last:

	return status;
}

enum sii_ipc_status sii_ipc_host_check_change(
		sii_inst_t inst, bool *new_reply, bool *new_notification) {
		enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;

	uint8_t status_register1 = 0x00;
	uint8_t status_register2 = 0x00;

	status = sii_ipc_hal_read_slave_status1(inst, &status_register1);
	if (status != SII_IPC_STATUS_SUCCESS) {
		goto exit;
	}

	status = sii_ipc_hal_read_slave_status2(inst, &status_register2);
	if (status != SII_IPC_STATUS_SUCCESS) {
		goto exit;
	}

	*new_reply = (SII_BIT_IPC_SLAVE_STATUS1_RPL_STATE_MASK
			& status_register1) ? true : false;

	*new_notification = (SII_BIT_IPC_SLAVE_STATUS2_NTF_STATE_MASK
			& status_register2) ? true : false;

		if (!(*new_reply) && !(*new_notification)) {
				//warn("Unknown interrupt source!");
				status = SII_IPC_STATUS_ERROR_PROTOCOL;
				goto exit;
		}

exit:
		return status;
}

enum sii_ipc_status sii_ipc_sync_with_slave_before_send_command(sii_inst_t inst) {
	enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;
	uint8_t status_register1 = 0;
	bool last_reply_active = true;
	uint8_t  retry_count=0;

	do {
		status = sii_ipc_hal_read_slave_status1(
				inst, &status_register1);
		if (status != SII_IPC_STATUS_SUCCESS)
		{
			goto exit;
		}

		last_reply_active = (SII_BIT_IPC_SLAVE_STATUS1_RPL_STATE_MASK
				& status_register1) ? true : false;
		if(true == last_reply_active)
			retry_count++;
	} while ((true == last_reply_active)&&(retry_count<10));

exit:
	return status;
}

enum sii_ipc_status sii_ipc_send_register_command(
	sii_inst_t inst, struct sii_ipc_register_command command) {
	enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;

	uint8_t status_register1 = 0x00;
	uint8_t status_register2 = 0x00;
	uint8_t checksum = 0x00;
	sii_os_sleep_msec(10);

	command.opcode = (command.opcode
			<< SII_BIT_IPC_HOST_STATUS1_CMD_OPCODE_SHIFT)
			& SII_BIT_IPC_HOST_STATUS1_CMD_OPCODE_MASK;
	command.parameter = (command.parameter
			<< SII_BIT_IPC_HOST_STATUS2_CMD_PARAMETER_SHIFT)
			& SII_BIT_IPC_HOST_STATUS2_CMD_PARAMETER_MASK;

	checksum = calculate_checksum_2bit(command.opcode, command.parameter);

	status_register1 = command.opcode;
	status_register2 = command.parameter | checksum;
#if 0
	status = sii_ipc_hal_write_host_status1(inst, status_register1);
	if (status != SII_IPC_STATUS_SUCCESS) {
		goto exit;
	}
#endif
	status = sii_ipc_hal_write_host_status2(inst, status_register2);
	if (status != SII_IPC_STATUS_SUCCESS) {
		goto exit;
	}

#if 1
	status = sii_ipc_hal_write_host_status1(inst, status_register1 | 0x80);
	if (status != SII_IPC_STATUS_SUCCESS) {
		goto exit;
	}
#endif
#if 0
	status = sii_ipc_hal_set_host_interrupt(inst, true);
	if (status != SII_IPC_STATUS_SUCCESS) {
		goto exit;
	}
#endif
exit:
	return status;
}

enum sii_ipc_status sii_ipc_send_buffer_command(sii_inst_t inst,
		struct sii_ipc_packet_command *packet_command,
		bool need_buffer_reply) {
	enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;

	uint8_t status_register1 = 0x00;
	uint8_t status_register2 = 0x00;
	uint8_t checksum = 0x00;
	sii_os_sleep_msec(10);
	packet_command->opcode = need_buffer_reply ?
		SII_IPC_PHY_CMD_OPCODE_BUF_CMD_BUF_RPL_INDICATOR :
		SII_IPC_PHY_CMD_OPCODE_BUFFER_COMMAND_INDICATOR;

	packet_command->opcode = (packet_command->opcode
			<< SII_BIT_IPC_HOST_STATUS1_CMD_OPCODE_SHIFT)
		& SII_BIT_IPC_HOST_STATUS1_CMD_OPCODE_MASK;

	packet_command->parameter = (packet_command->parameter
			<< SII_BIT_IPC_HOST_STATUS2_CMD_PARAMETER_SHIFT)
		& SII_BIT_IPC_HOST_STATUS2_CMD_PARAMETER_MASK;

	checksum = calculate_checksum_2bit(
			packet_command->opcode, packet_command->parameter);

	/*
	   Prepare Status Register 1 & 2
	 */
	status_register1 = packet_command->opcode;
	status_register2 = packet_command->parameter | checksum;

	/*
	   Update Status Register 1 & 2
	 */
#if 0
	status = sii_ipc_hal_write_host_status1(inst, status_register1);
	if (status != SII_IPC_STATUS_SUCCESS) {
		goto exit;
	}
#endif
	status = sii_ipc_hal_write_host_status2(inst, status_register2);
	if (status != SII_IPC_STATUS_SUCCESS) {
		goto exit;
	}

	/*
	   Send the Command Packet
	 */
	sii_os_sleep_msec(10);
	/* Send Buffer Out */
	status = sii_ipc_hal_write_host_buffer_command(
			inst,
			(uint8_t *)(packet_command->packet),
			packet_command->packet->packet_length);
	if (status != SII_IPC_STATUS_SUCCESS) {
		goto exit;
	}
#if 1
	status = sii_ipc_hal_write_host_status1(inst, status_register1 | 0x80);
	if (status != SII_IPC_STATUS_SUCCESS) {
		goto exit;
	}
#endif
#if 0
	/*
	   Trigger the Host Interrupt
	 */
	status = sii_ipc_hal_set_host_interrupt(inst, true);
	if (status != SII_IPC_STATUS_SUCCESS) {
		goto exit;
	}
#endif
exit:
	return status;
}

enum sii_ipc_status sii_ipc_receive_register_reply(
	sii_inst_t inst, struct sii_ipc_register_reply *reply) {
	enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;

	uint8_t status_register1 = 0x00;
	uint8_t status_register2 = 0x00;
	bool verify_checksum = false;
	sii_os_sleep_msec(10);
	status = sii_ipc_hal_read_slave_status1(inst, &status_register1);
	if (status != SII_IPC_STATUS_SUCCESS) {
		goto exit;
	}

	status = sii_ipc_hal_read_slave_status2(inst, &status_register2);
	if (status != SII_IPC_STATUS_SUCCESS) {
		goto exit;
	}

	verify_checksum = verify_checksum_2bit(status_register1, status_register2);
	if (!verify_checksum) {
		status = SII_IPC_STATUS_ERROR_HAL_CHECKSUM;
		goto exit;
	}

	reply->reply_status = (status_register1
			& SII_BIT_IPC_SLAVE_STATUS1_REPLY_STATUS_MASK)
			>> SII_BIT_IPC_SLAVE_STATUS1_REPLY_STATUS_SHIFT;
	reply->reply_return = (status_register1
			& SII_BIT_IPC_SLAVE_STATUS1_REPLY_RETURN_MASK)
			>> SII_BIT_IPC_SLAVE_STATUS1_REPLY_RETURN_SHIFT;

exit:
	return status;
}

static enum sii_ipc_status sii_ipc_extract_packet_from_buffer_reply(
	sii_inst_t inst, struct sii_ipc_packet *reply_packet,
	struct sii_ipc_buffer_reply *reply) {
	enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;
	bool verify_checksum = false;

	if (SII_IPC_PACKET_PROTOCOL_VERSION_1 != reply->packet->protocol_version) {
		/* FD: TBR, support future protocol extension */
		status = SII_IPC_STATUS_ERROR_PROTOCOL_VERSION_UNSUPPORTED;
		goto exit;
	} else {
		reply_packet->protocol_version = reply->packet->protocol_version;

		/* Verify checksum */
		verify_checksum = verify_checksum_byte((uint8_t *)(reply->packet),
							reply->packet->packet_length);

		if (!verify_checksum) {
			status = SII_IPC_STATUS_ERROR_PACKET_CHECKSUM;
			goto exit;
		}

		/* Extract real Packet payload */
		reply_packet->sequence_number = reply->packet->sequence_number;
		reply_packet->fragment_count = reply->packet->fragment_count;
		reply_packet->packet_length = reply->packet->packet_length;
		reply_packet->packet_checksum = reply->packet->packet_checksum;

		memcpy(reply_packet->packet_payload,
				reply->packet->packet_payload,
				reply_packet->packet_length - SII_IPC_PACKET_HEADER_LENGTH);
	}

exit:
	return status;
}

enum sii_ipc_status sii_ipc_extract_notification_from_packet_ntf(
		sii_inst_t inst, struct sii_ipc_packet_ntf *packet_ntf) {
	enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;

	bool verify_checksum = false;
	uint8_t ntf_count = 0;
	uint8_t ntf_index = 0;
	struct sii_ipc_notification notification;

	/* Check Protocol Version */
	if (SII_IPC_PACKET_NTF_PROTOCOL_VERSION_1
			!= packet_ntf->protocol_version) {
		/* FD: TBR, support future protocol extension */
		status = SII_IPC_STATUS_ERROR_PROTOCOL_VERSION_UNSUPPORTED;
		goto exit;
	}

	/* Verify checksum */
	verify_checksum = verify_checksum_byte((uint8_t *)(packet_ntf),
			packet_ntf->packet_length);
	if (!verify_checksum) {
		status = SII_IPC_STATUS_ERROR_PACKET_CHECKSUM;
		goto exit;
	}

	/*
	   Extract Notifications, Push into Notification Queue
	 */
	ntf_count = packet_ntf->packet_length
		- SII_IPC_PACKET_NTF_HEADER_LENGTH;

	for (; ntf_index < ntf_count; ntf_index++) {

		/* Extract Payload of Notification Packet */
		notification.notification_type =
			packet_ntf->packet_payload[ntf_index];

		/* Push into Notification Queue */
		status = sii_ipc_host_push_queue_notification(inst, notification);
		if (status != SII_IPC_STATUS_SUCCESS) {
			goto exit;
		}
	}

exit:
	return status;
}

enum sii_ipc_status sii_ipc_receive_buffer_reply(
	sii_inst_t inst, struct sii_ipc_packet_reply *packet_reply,
	bool *more_packet) {
	enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;

	struct sii_ipc_buffer_reply buffer_reply;
	uint8_t status_register1 = 0x00;
	uint8_t status_register2 = 0x00;
	bool verify_checksum = false;
	uint8_t buffer_status = 0x00;

	/* Initial Return */
	*more_packet = false;

	sii_os_sleep_msec(10);
	status = sii_ipc_hal_read_slave_status1(inst, &status_register1);
	if (status != SII_IPC_STATUS_SUCCESS) {
		goto exit;
	}
	dbg("****staus1");

	status = sii_ipc_hal_read_slave_status2(inst, &status_register2);
	if (status != SII_IPC_STATUS_SUCCESS) {
		goto exit;
	}
	dbg("****staus2");

	verify_checksum = verify_checksum_2bit(
			status_register1, status_register2);
	if (!verify_checksum) {
		status = SII_IPC_STATUS_ERROR_HAL_CHECKSUM;
		goto exit;
	}
	dbg("****staus3");

	packet_reply->reply_status = (status_register1
			& SII_BIT_IPC_SLAVE_STATUS1_REPLY_STATUS_MASK)
			>> SII_BIT_IPC_SLAVE_STATUS1_REPLY_STATUS_SHIFT;

	packet_reply->reply_return = (status_register1
			& SII_BIT_IPC_SLAVE_STATUS1_REPLY_RETURN_MASK)
			>> SII_BIT_IPC_SLAVE_STATUS1_REPLY_RETURN_SHIFT;

	if (packet_reply->reply_status != SII_IPC_PHY_REPLY_STATUS_SUCCESS) {
		/* NACK / ACK / FAILURE : no need to check the Buffer */
		goto exit;
	}

	buffer_status = (status_register1
			& SII_BIT_IPC_SLAVE_STATUS1_BUFFER_STATUS_MASK)
			>> SII_BIT_IPC_SLAVE_STATUS1_BUFFER_STATUS_SHIFT;
	if (buffer_status != SII_IPC_PHY_BUFFER_STATUS_REPLY) {
		status = SII_IPC_STATUS_ERROR_HAL_NO_REPLY_IN_BUFFER;
		goto exit;
	}

	dbg("****staus4");
	buffer_reply.packet = &(ipc_host_context[inst].host_packet);
	memset(buffer_reply.packet, 0, SII_IPC_PACKET_LENGTH);

	status = sii_ipc_hal_read_slave_buffer_reply(
			inst,
			(void *)(buffer_reply.packet), SII_IPC_PACKET_LENGTH);
	if (status != SII_IPC_STATUS_SUCCESS) {
		goto exit;
	}

	dbg("****staus5");
	/* FD: TBR, double check this to save one copy & process */
	status = sii_ipc_extract_packet_from_buffer_reply(inst,
			packet_reply->packet, &buffer_reply);
	if (status != SII_IPC_STATUS_SUCCESS) {
		goto exit;
	}

	dbg("****staus6");

	/* FD: TBC, may need more careful check in case out-of-order delivery */
	*more_packet = packet_reply->packet->sequence_number + 1
					< packet_reply->packet->fragment_count
					? true : false;

	#if 1
	info("seq num: %d\n", packet_reply->packet->sequence_number);
	info("fgt cnt: %d\n", packet_reply->packet->fragment_count);

#endif
exit:
	return status;
}

enum sii_ipc_status sii_ipc_receive_register_notification(
	sii_inst_t inst, struct sii_ipc_register_notification *notification) {
	enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;

	uint8_t status_register1 = 0x00;
	uint8_t status_register2 = 0x00;
	bool verify_checksum = false;

	status = sii_ipc_hal_read_slave_status1(inst, &status_register1);
	if (status != SII_IPC_STATUS_SUCCESS) {
		goto exit;
	}

	status = sii_ipc_hal_read_slave_status2(inst, &status_register2);
	if (status != SII_IPC_STATUS_SUCCESS) {
		goto exit;
	}

	verify_checksum = verify_checksum_2bit(status_register1, status_register2);
	if (!verify_checksum) {
		status = SII_IPC_STATUS_ERROR_HAL_CHECKSUM;
		goto exit;
	}

	notification->notification_type = (status_register2
			& SII_BIT_IPC_SLAVE_STATUS2_NOTIFICATION_TYPE_MASK)
			>> SII_BIT_IPC_SLAVE_STATUS2_NOTIFICATION_TYPE_SHIFT;

exit:
	return status;
}

enum sii_ipc_status sii_ipc_receive_buffer_notification(
		sii_inst_t inst, struct sii_ipc_packet_notification *packet_notification) {
	enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;

	uint8_t status_register1 = 0x00;
	uint8_t status_register2 = 0x00;
	bool verify_checksum = false;
	uint8_t buffer_status = 0x00;

	struct sii_ipc_packet_ntf *packet_ntf =
		packet_notification->packet;

	status = sii_ipc_hal_read_slave_status1(inst, &status_register1);
	if (status != SII_IPC_STATUS_SUCCESS) {
		goto exit;
	}

	status = sii_ipc_hal_read_slave_status2(inst, &status_register2);
	if (status != SII_IPC_STATUS_SUCCESS) {
		goto exit;
	}

	verify_checksum = verify_checksum_2bit(
			status_register1, status_register2);
	if (!verify_checksum) {
		status = SII_IPC_STATUS_ERROR_HAL_CHECKSUM;
		goto exit;
	}

	packet_notification->type = (status_register2
			& SII_BIT_IPC_SLAVE_STATUS2_NOTIFICATION_TYPE_MASK)
		>> SII_BIT_IPC_SLAVE_STATUS2_NOTIFICATION_TYPE_SHIFT;

	if (packet_notification->type !=
			SII_IPC_PHY_NOTIFICATION_TYPE_BUFFER_NOTIFICATION) {
		warn("Unsupported Notification Type");

		/* FD: TBU, extension for other Notification Types */
		/*
		switch(packet_notification->type) {
			case :
				break;
			default :
				break;
		}
		*/
		goto exit;
	}

	buffer_status = (status_register1
			& SII_BIT_IPC_SLAVE_STATUS1_BUFFER_STATUS_MASK)
		>> SII_BIT_IPC_SLAVE_STATUS1_BUFFER_STATUS_SHIFT;
	if (buffer_status != SII_IPC_PHY_BUFFER_STATUS_NOTIFICATION) {
		status = SII_IPC_STATUS_ERROR_HAL_NO_NOTIFICATION_IN_BUFFER;
		goto exit;
	}

	memset(packet_ntf, 0, SII_IPC_PACKET_NTF_LENGTH);

	status = sii_ipc_hal_read_slave_buffer_notification(
			inst,
			(void *)(packet_ntf), SII_IPC_PACKET_NTF_LENGTH);
	if (status != SII_IPC_STATUS_SUCCESS) {
		goto exit;
	}

exit:
	return status;
}

/*
   IPC External API for Host Side
 */

enum sii_ipc_status sii_execute_register_command_no_reply(
	sii_inst_t inst, struct sii_ipc_register_command command) {

	enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;
	enum sii_ipc_status status2 = SII_IPC_STATUS_SUCCESS;
	bool release_mutex = false;

	#ifdef HOST_IPC_POLL
	//give one chance for extracting notification from internal MCU
	sii_ipc_host_handler(inst);
	#endif

	status = sii_ipc_sync_with_slave_before_send_command(inst);
	if (status != SII_IPC_STATUS_SUCCESS) {
		goto exit;
	}

	/* Acquire Host Execute Mutex */
	status = sii_ipc_host_acquire_mutex(inst);
	if (status != SII_IPC_STATUS_SUCCESS) {
		goto exit;
	}

	release_mutex = true;

	/* Send the Command (Register Command) */
	status = sii_ipc_send_register_command(inst, command);
	if (status != SII_IPC_STATUS_SUCCESS) {
		goto exit;
	}

exit:
	ipc_host_context[inst].host_sm = SII_IPC_HOST_SM_IDLE;

	if (true == release_mutex) {
		/* Release Host Mutex */
		status2 = sii_ipc_host_release_mutex(inst);
		if (status2 != SII_IPC_STATUS_SUCCESS) {
			status = status2;
		}
	}

	return status;
}

enum sii_ipc_status sii_execute_register_command_register_reply(
	sii_inst_t inst, struct sii_ipc_register_command command,
	struct sii_ipc_register_reply *reply) {

	enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;
	enum sii_ipc_status status2 = SII_IPC_STATUS_SUCCESS;
	bool release_mutex = false;
	bool clear_interrupt = false;

	#ifdef POLL_METHOD
	uint8_t timeout = 0;
	#ifdef HOST_IPC_POLL
	//give one chance for extracting notification from internal MCU
	sii_ipc_host_handler(inst);
	#endif
	#endif

	status = sii_ipc_sync_with_slave_before_send_command(inst);
	if (status != SII_IPC_STATUS_SUCCESS) {
		goto exit;
	}

	/* Acquire Host Execute Mutex */
	status = sii_ipc_host_acquire_mutex(inst);
	if (status != SII_IPC_STATUS_SUCCESS) {
		goto exit;
	}

	release_mutex = true;

	/* Send the Command (Register Command) */
	status = sii_ipc_send_register_command(inst, command);
	if (status != SII_IPC_STATUS_SUCCESS) {
		goto exit;
	}

	ipc_host_context[inst].host_sm = SII_IPC_HOST_SM_WAIT_REPLY;

	#if POLL_METHOD
	status = sii_ipc_host_handler(inst);
	while (status != SII_IPC_STATUS_SUCCESS && timeout < 5)
	{
		sii_os_sleep_msec(10);
		halUICtrlEventHandler();
		status = sii_ipc_host_handler(inst);
		timeout ++;
	}
	#endif

	/* FD: TBI, Start Send Command Timer */

	status = sii_ipc_host_take_reply_semaphore(inst);
	if (status != SII_IPC_STATUS_SUCCESS) {
		if (status == SII_IPC_STATUS_ERROR_OSAL_SEMAPHORE_TIMEOUT) {
		}
		goto exit;
	}

	clear_interrupt = true;
	//sii_os_sleep_msec(10);
	/* Receive the Reply (Register Reply) */
	status = sii_ipc_receive_register_reply(inst, reply);
	if (status != SII_IPC_STATUS_SUCCESS) {
		goto exit;
	}

	/* FD: TBI, Stop Send Command Timer */

exit:
	ipc_host_context[inst].host_sm = SII_IPC_HOST_SM_IDLE;

	if (true == clear_interrupt) {
		status2 = sii_ipc_host_clear_and_enable_interrupt(inst);
		if (status2 != SII_IPC_STATUS_SUCCESS) {
			status = status2;
		}
	}

	if (true == release_mutex) {
		/* Release Host Mutex */
		status2 = sii_ipc_host_release_mutex(inst);
		if (status2 != SII_IPC_STATUS_SUCCESS) {
			status = status2;
		}
	}

	/* FD: TBI, Stop Send Command Timer if there */

	return status;
}

static enum sii_ipc_status sii_execute_register_command_buffer_reply(
	sii_inst_t inst, struct sii_ipc_register_command command,
	struct sii_ipc_packet_reply *packet_reply,
	bool *more_packet) {

	enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;
	enum sii_ipc_status status2 = SII_IPC_STATUS_SUCCESS;
	bool release_mutex = false;
	bool clear_interrupt = false;

	#if POLL_METHOD
	uint8_t timeout = 0;
	#endif

	/* Initial Return */
	*more_packet = false;

	#ifdef HOST_IPC_POLL
	//give one chance for extracting
	sii_ipc_host_handler(inst);
	#endif
	status = sii_ipc_sync_with_slave_before_send_command(inst);
	if (status != SII_IPC_STATUS_SUCCESS) {
		goto exit;
	}

	/* Acquire Host Execute Mutex */
	status = sii_ipc_host_acquire_mutex(inst);
	if (status != SII_IPC_STATUS_SUCCESS) {
		goto exit;
	}

	release_mutex = true;

	/* Send the Command (Register Command) */
	status = sii_ipc_send_register_command(inst, command);
	if (status != SII_IPC_STATUS_SUCCESS) {
		dbg("sii_ipc_send_register_command  fail....\n");
		goto exit;
	}

	dbg("\nsii_ipc_send_register_command success...\n");
	ipc_host_context[inst].host_sm = SII_IPC_HOST_SM_WAIT_REPLY;

	/* FD: TBI, Start Send Command Timer */

	#if POLL_METHOD
	status = sii_ipc_host_handler(inst);
	while (status != SII_IPC_STATUS_SUCCESS && timeout < 5)
	{
		sii_os_sleep_msec(10);
		halUICtrlEventHandler();
		status = sii_ipc_host_handler(inst);
		timeout ++;
	}
	#endif

	status = sii_ipc_host_take_reply_semaphore(inst);
	if (status != SII_IPC_STATUS_SUCCESS) {
		if (status == SII_IPC_STATUS_ERROR_OSAL_SEMAPHORE_TIMEOUT) {
		}
		goto exit;
	}

	clear_interrupt = true;
	//sii_os_sleep_msec(10);
	/* Receive the Reply (Buffer Reply) */
	status = sii_ipc_receive_buffer_reply(inst, packet_reply, more_packet);
	if (status != SII_IPC_STATUS_SUCCESS) {
		dbg("sii_ipc_receive_buffer_reply  fail...\n");
		goto exit;
	}
	dbg("sii_ipc_receive_buffer_reply  success...\n");

	/* FD: TBI, Stop Send Command Timer */

exit:
	ipc_host_context[inst].host_sm = SII_IPC_HOST_SM_IDLE;

	if (true == clear_interrupt) {
		status2 = sii_ipc_host_clear_and_enable_interrupt(inst);
		if (status2 != SII_IPC_STATUS_SUCCESS) {
			status = status2;
		}
	}

	if (true == release_mutex) {
		/* Release Host Mutex */
		status2 = sii_ipc_host_release_mutex(inst);
		if (status2 != SII_IPC_STATUS_SUCCESS) {
			status = status2;
		}
	}

	/* FD: TBI, Stop Send Command Timer if there */

	return status;
}

static enum sii_ipc_status sii_execute_buffer_command_register_reply(
	sii_inst_t inst, struct sii_ipc_packet_command *command,
	struct sii_ipc_register_reply *reply) {

	enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;
	enum sii_ipc_status status2 = SII_IPC_STATUS_SUCCESS;
	bool release_mutex = false;
	bool clear_interrupt = false;

	#if POLL_METHOD
	uint8_t timeout = 0;
	#ifdef HOST_IPC_POLL
	//give one chance for extracting
	sii_ipc_host_handler(inst);
	#endif
	#endif

	status = sii_ipc_sync_with_slave_before_send_command(inst);
	if (status != SII_IPC_STATUS_SUCCESS) {
		goto exit;
	}

	/* Acquire Host Execute Mutex */
	status = sii_ipc_host_acquire_mutex(inst);
	if (status != SII_IPC_STATUS_SUCCESS) {
		goto exit;
	}

	release_mutex = true;

	/* Send the Command (Buffer Command)
	   (expect Register Reply)
	 */
	status = sii_ipc_send_buffer_command(inst, command, false);
	if (status != SII_IPC_STATUS_SUCCESS) {
		goto exit;
	}

	ipc_host_context[inst].host_sm = SII_IPC_HOST_SM_WAIT_REPLY;

	/* FD: TBI, Start Send Command Timer */


	#if POLL_METHOD
	status = sii_ipc_host_handler(inst);
	while (status != SII_IPC_STATUS_SUCCESS && timeout < 5)
	{
		sii_os_sleep_msec(10);
		halUICtrlEventHandler();
		status = sii_ipc_host_handler(inst);
		timeout ++;
	}
	#endif

	status = sii_ipc_host_take_reply_semaphore(inst);
	if (status != SII_IPC_STATUS_SUCCESS) {
		if (status == SII_IPC_STATUS_ERROR_OSAL_SEMAPHORE_TIMEOUT) {
		}
		goto exit;
	}

	clear_interrupt = true;
	//sii_os_sleep_msec(10);
	/* Receive the Reply (Register Reply) */
	status = sii_ipc_receive_register_reply(inst, reply);
	if (status != SII_IPC_STATUS_SUCCESS) {
		goto exit;
	}

	/* FD: TBI, Stop Send Command Timer */

exit:
	ipc_host_context[inst].host_sm = SII_IPC_HOST_SM_IDLE;

	if (true == clear_interrupt) {
		status2 = sii_ipc_host_clear_and_enable_interrupt(inst);
		if (status2 != SII_IPC_STATUS_SUCCESS) {
			status = status2;
		}
	}

	if (true == release_mutex) {
		/* Release Host Mutex */
		status2 = sii_ipc_host_release_mutex(inst);
		if (status2 != SII_IPC_STATUS_SUCCESS) {
			status = status2;
		}
	}

	/* FD: TBI, Stop Send Command Timer if there */

	return status;
}

static enum sii_ipc_status sii_execute_buffer_command_buffer_reply(
	sii_inst_t inst, struct sii_ipc_packet_command *command,
	struct sii_ipc_packet_reply *packet_reply,
	bool *more_packet) {

	enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;
	enum sii_ipc_status status2 = SII_IPC_STATUS_SUCCESS;
	bool release_mutex = false;
	bool clear_interrupt = false;

	#ifdef POLL_METHOD
	uint8_t timeout = 0;
	#endif


	/* Initial Return */
	*more_packet = false;

	#ifdef HOST_IPC_POLL
	sii_ipc_host_handler(inst);
	#endif

	status = sii_ipc_sync_with_slave_before_send_command(inst);
	if (status != SII_IPC_STATUS_SUCCESS) {
		goto exit;
	}

	/* Acquire Host Execute Mutex */
	status = sii_ipc_host_acquire_mutex(inst);
	if (status != SII_IPC_STATUS_SUCCESS) {
		goto exit;
	}

	release_mutex = true;

	/* Send the Command (Buffer Command)
	   (expect Buffer Reply)
	 */
	status = sii_ipc_send_buffer_command(inst, command, true);
	if (status != SII_IPC_STATUS_SUCCESS) {
		goto exit;
	}

	ipc_host_context[inst].host_sm = SII_IPC_HOST_SM_WAIT_REPLY;


	#ifdef POLL_METHOD
	status = sii_ipc_host_handler(inst);
	while (status != SII_IPC_STATUS_SUCCESS && timeout < 5)
	{
		sii_os_sleep_msec(10);
		halUICtrlEventHandler();
		status = sii_ipc_host_handler(inst);
		timeout ++;
	}
	#endif

	/* FD: TBI, Start Send Command Timer */

	status = sii_ipc_host_take_reply_semaphore(inst);
	if (status != SII_IPC_STATUS_SUCCESS) {
		if (status == SII_IPC_STATUS_ERROR_OSAL_SEMAPHORE_TIMEOUT) {
		}
		goto exit;
	}

	clear_interrupt = true;
	//sii_os_sleep_msec(10);
	/* Receive the Reply (Buffer Reply) */
	status = sii_ipc_receive_buffer_reply(inst, packet_reply, more_packet);
	if (status != SII_IPC_STATUS_SUCCESS) {
		goto exit;
	}

	/* FD: TBI, Stop Send Command Timer */

exit:
	ipc_host_context[inst].host_sm = SII_IPC_HOST_SM_IDLE;

	if (true == clear_interrupt) {
		status2 = sii_ipc_host_clear_and_enable_interrupt(inst);
		if (status2 != SII_IPC_STATUS_SUCCESS) {
			status = status2;
		}
	}

	if (true == release_mutex) {
		/* Release Host Mutex */
		status2 = sii_ipc_host_release_mutex(inst);
		if (status2 != SII_IPC_STATUS_SUCCESS) {
			status = status2;
		}
	}

	/* FD: TBI, Stop Send Command Timer if there */

	return status;
}

static enum sii_ipc_status sii_receive_next_piece_of_buffer_reply(
	sii_inst_t inst, struct sii_ipc_packet_reply *packet_reply,
	bool *more_packet) {

	struct sii_ipc_register_command command;
	enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;

	/* Initial Return */
	*more_packet = false;

	/* Prepare the Command */
	command.opcode = SII_IPC_PHY_CMD_OPCODE_ACK_SLAVE_BUFFER_REPLY;
	command.parameter = 0x00;

	status = sii_execute_register_command_buffer_reply(inst,
				command, packet_reply, more_packet);

	return status;
}

static enum sii_ipc_status sii_refuse_next_piece_of_buffer_reply(sii_inst_t inst) {
	struct sii_ipc_register_command command;
	enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;

	/* Prepare the Command */
	command.opcode = SII_IPC_PHY_CMD_OPCODE_NACK_SLAVE_BUFFER_REPLY;
	command.parameter = 0x00;  // reason of NACK if necessary

	status = sii_execute_register_command_no_reply(inst, command);

	return status;
}

static enum sii_ipc_status sii_accept_last_piece_of_buffer_reply(sii_inst_t inst) {
	struct sii_ipc_register_command command;
	enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;

	/* Prepare the Command */
	command.opcode = SII_IPC_PHY_CMD_OPCODE_ACK_SLAVE_BUFFER_REPLY;
	command.parameter = 0x00;

	status = sii_execute_register_command_no_reply(inst, command);

	return status;
}

static void sii_compose_message_reply_from_packet_reply(
	sii_inst_t inst, struct sii_ipc_message_reply *message_reply,
	struct sii_ipc_packet_reply *packet_reply) {

	struct sii_ipc_raw_message *raw_message =
				(struct sii_ipc_raw_message*)(message_reply->message);
	struct sii_ipc_packet *reply_packet = packet_reply->packet;

	memcpy(raw_message->raw_data + reply_packet->sequence_number
				* SII_IPC_PACKET_PAYLOAD_LENGTH_MAX,
			reply_packet->packet_payload,
			reply_packet->packet_length - SII_IPC_PACKET_HEADER_LENGTH);

	/* Save Reply Status & Reply Return */
	message_reply->reply_status = packet_reply->reply_status;
	message_reply->reply_return = packet_reply->reply_return;
}

enum sii_ipc_status sii_execute_register_command_message_reply(
		sii_inst_t inst, struct sii_ipc_register_command command,
		struct sii_ipc_message_reply *message_reply) {

	enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;
	enum sii_ipc_status status2 = SII_IPC_STATUS_SUCCESS;
	struct sii_ipc_packet_reply packet_reply;
	struct sii_ipc_packet reply_packet;
	bool more_packet = false;
	bool ack_last_piece = false;

	packet_reply.packet = &reply_packet;

	status = sii_execute_register_command_buffer_reply(inst,
			command, &packet_reply, &more_packet);
	if (status != SII_IPC_STATUS_SUCCESS) {
		goto exit;
	}

	dbg("Successfully send register command & receive buffer reply\n");

	if (packet_reply.reply_status != SII_IPC_PHY_REPLY_STATUS_SUCCESS) {
		/* Successful IPC Pair (Command & Reply),
		   Unsuccessful Execution */
		goto exit;
	}

	/* Compose Message Reply from Packet Reply */
	sii_compose_message_reply_from_packet_reply(inst,
			message_reply, &packet_reply);

	while (more_packet) {
		status = sii_receive_next_piece_of_buffer_reply(inst,
				&packet_reply, &more_packet);

		/* Refuse next piece of Reply
		   if there is any error during parsing */
		if (status != SII_IPC_STATUS_SUCCESS) {
			status2 = sii_refuse_next_piece_of_buffer_reply(inst);
			if (status2 != SII_IPC_STATUS_SUCCESS) {
				status =
					SII_IPC_STATUS_ERROR_PROTOCOL_CANNOT_NACK;
			} else {
				/* do nothing, use original error code */
			}

			ack_last_piece = false;
			break;  // no need to get next piece
		} else {
			ack_last_piece = true;
		}

		/* Compose Message Reply from Packet Reply */
		sii_compose_message_reply_from_packet_reply(inst,
				message_reply, &packet_reply);
	}

	/* Accept Last Piece of Buffer Reply */
	if (true == ack_last_piece) {
		sii_accept_last_piece_of_buffer_reply(inst);
		if (status != SII_IPC_STATUS_SUCCESS) {
			goto exit;
		}
	}

exit:
	return status;
}

static void sii_compose_packet_command_from_message_command(
		sii_inst_t inst, struct sii_ipc_packet_command *packet_command,
		struct sii_ipc_message_command *message_command,
		uint8_t packet_sequence) {

	struct sii_ipc_packet *command_packet = packet_command->packet;
	struct sii_ipc_raw_message *raw_message = NULL;
	uint16_t message_length = 0;

	raw_message = (struct sii_ipc_raw_message*)(message_command->message);

	memset(command_packet, 0, SII_IPC_PACKET_LENGTH);

	command_packet->protocol_version = SII_IPC_PACKET_PROTOCOL_VERSION_1;
	command_packet->sequence_number = packet_sequence;

	message_length = message_command->message->message_length_l +
		((uint16_t)(message_command->message->message_length_h) << 8);

	command_packet->fragment_count =
		(message_length + SII_IPC_PACKET_PAYLOAD_LENGTH_MAX - 1)
		/ SII_IPC_PACKET_PAYLOAD_LENGTH_MAX;

	if (command_packet->sequence_number < command_packet->fragment_count - 1) {
		command_packet->packet_length = SII_IPC_PACKET_LENGTH;
	} else {
		command_packet->packet_length = message_length -
			(SII_IPC_PACKET_PAYLOAD_LENGTH_MAX *
			(uint16_t)(command_packet->fragment_count - 1))
			+ SII_IPC_PACKET_HEADER_LENGTH;
	}

	/* Update packet_fragment */
	ipc_host_context[inst].packet_fragment = command_packet->fragment_count;

	memcpy(command_packet->packet_payload,
		raw_message->raw_data +
			(command_packet->sequence_number
			 * SII_IPC_PACKET_PAYLOAD_LENGTH_MAX),
		command_packet->packet_length
			- SII_IPC_PACKET_HEADER_LENGTH);

	command_packet->packet_checksum = calculate_checksum_byte(
			(uint8_t *)command_packet,
			command_packet->packet_length);

	/* Save Command Opcode & Command Parameter */
	packet_command->opcode = message_command->opcode;
	packet_command->parameter = message_command->parameter;
}

enum sii_ipc_status sii_execute_message_command_register_reply(
		sii_inst_t inst, struct sii_ipc_message_command *message_command,
		struct sii_ipc_register_reply *reply) {

	enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;
	struct sii_ipc_packet_command packet_command;

	packet_command.packet = &(ipc_host_context[inst].host_packet);

	/*
	   Prepare the Packet Command for Sending
	 */
	ipc_host_context[inst].packet_sequence = 0;
	ipc_host_context[inst].packet_fragment = 0;

	/* Send Packet Command One by One if Necessary */
	do {
		/* Compose Packet Command from Message Command */
		sii_compose_packet_command_from_message_command(inst,
				&packet_command, message_command,
				ipc_host_context[inst].packet_sequence);

		status = sii_execute_buffer_command_register_reply(inst,
				&packet_command, reply);
		if (status != SII_IPC_STATUS_SUCCESS) {
			goto exit;
		}

		if (reply->reply_status != SII_IPC_PHY_REPLY_STATUS_ACK) {
			/* Successful Single-Buffer Command, or
			   NACK/FAILURE to Multi-Buffer Command */
			goto exit;
		}

		/* Prepare the Next Packet Command for Sending */
		(ipc_host_context[inst].packet_sequence)++;

	} while (ipc_host_context[inst].packet_sequence <
			ipc_host_context[inst].packet_fragment);

exit:
	return status;
}

enum sii_ipc_status sii_execute_message_command_message_reply(
		sii_inst_t inst, struct sii_ipc_message_command *message_command,
		struct sii_ipc_message_reply *message_reply) {

	enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;
	enum sii_ipc_status status2 = SII_IPC_STATUS_SUCCESS;

	struct sii_ipc_packet_command packet_command;
	struct sii_ipc_packet_reply packet_reply;
	struct sii_ipc_register_reply register_reply;

	struct sii_ipc_packet reply_packet;
	bool more_packet = false;
	bool ack_last_piece = false;

	packet_command.packet = &(ipc_host_context[inst].host_packet);
	packet_reply.packet = &reply_packet; /* FD: TBR, re-use host_packet */

	/*
	   Send Single-Packet Command or Multi-Packet Command
	   (receive the first Packet Reply)
	 */
	ipc_host_context[inst].packet_sequence = 0;
	ipc_host_context[inst].packet_fragment = 0;

	/* Compose First Packet Command from Message Command */
	sii_compose_packet_command_from_message_command(inst,
			&packet_command, message_command,
			ipc_host_context[inst].packet_sequence);

	if (1 == ipc_host_context[inst].packet_fragment) {

		/* Single-Packet Command */

		/* Need to Send the Only Packet Command */
	} else {

		/* Multi-Packet Command */

		/* Send Packet Command One by One */
		/* (expect to receive ACK) */
		do {
			status = sii_execute_buffer_command_register_reply(inst,
					&packet_command, &register_reply);
			if (status != SII_IPC_STATUS_SUCCESS) {
				goto exit;
			}

			if (register_reply.reply_status
					!= SII_IPC_PHY_REPLY_STATUS_ACK) {
				/* NACK/FAILURE to Multi-Buffer Command */
				goto exit;
			}

			/* Prepare the Next Packet Command for Sending */
			(ipc_host_context[inst].packet_sequence)++;

			/* Compose Packet Command from Message Command */
			sii_compose_packet_command_from_message_command(inst,
					&packet_command, message_command,
					ipc_host_context[inst].packet_sequence);

		} while (ipc_host_context[inst].packet_sequence + 1 <
				ipc_host_context[inst].packet_fragment);

		/* Need to Send Last Packet of Multi-Packet Command */
	}

	/*
	   Send the Only Packet Command of Single-Packet Command, or
	   Last Packet of Multi-Packet Command
	   (expect to receive the first Packet Reply)
	 */
	status = sii_execute_buffer_command_buffer_reply(inst,
			&packet_command,
			&packet_reply,
			&more_packet);
	if (status != SII_IPC_STATUS_SUCCESS) {
		goto exit;
	}

	if (packet_reply.reply_status != SII_IPC_PHY_REPLY_STATUS_SUCCESS) {
		/* Successful IPC Pair (Command & Reply),
		   Unsuccessful Execution */
		goto exit;
	}

	/* Compose Message Reply from Packet Reply */
	sii_compose_message_reply_from_packet_reply(inst,
			message_reply, &packet_reply);

	/*
	   Receive Packet Reply(s)
	   (except for the first Packet Reply)
	 */
	while (more_packet) {
		status = sii_receive_next_piece_of_buffer_reply(inst,
				&packet_reply, &more_packet);

		/* Refuse next piece of Reply
		   if there is any error during parsing */
		if (status != SII_IPC_STATUS_SUCCESS) {
			status2 = sii_refuse_next_piece_of_buffer_reply(inst);
			if (status2 != SII_IPC_STATUS_SUCCESS) {
				status =
					SII_IPC_STATUS_ERROR_PROTOCOL_CANNOT_NACK;
			} else {
				/* do nothing, use original error code */
			}

			ack_last_piece = false;
			break;  // no need to get next piece
		} else {
			ack_last_piece = true;
		}

		/* Compose Message Reply from Packet Reply */
		sii_compose_message_reply_from_packet_reply(inst,
				message_reply, &packet_reply);
	}

	/* Accept Last Piece of Buffer Reply */
	if (true == ack_last_piece) {
		sii_accept_last_piece_of_buffer_reply(inst);
		if (status != SII_IPC_STATUS_SUCCESS) {
			goto exit;
		}
	}

exit:
	return status;
}

enum sii_ipc_status sii_ipc_init(sii_inst_t inst) {
	enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;
	enum sii_os_status osal_status = SII_OS_STATUS_SUCCESS;

	memset(&ipc_host_context[inst], 0, sizeof(ipc_host_context[inst]));

	#if 0
	uint8_t *ptemp=(uint8_t *)(&ipc_host_context[inst].host_message);

	for(i=0; i<sizeof(ipc_host_context[inst]); i++)
	{
		*ptemp=0;
		ptemp++;
	}
	#endif


	warn("ipc_host_context = 0 ....\n");

	/* Explicit init */
	ipc_host_context[inst].notification_queue_count = 0;
	ipc_host_context[inst].notification_queue_head = 0;
	ipc_host_context[inst].notification_queue_tail = 0;
	ipc_host_context[inst].host_sm = SII_IPC_HOST_SM_IDLE;

	warn("ipc_host_context init ok ....\n");

	ipc_host_context[inst].host_packet_notification.packet =
		&(ipc_host_context[inst].host_packet_ntf);

	ipc_host_context[inst].packet_sequence = SII_IPC_PACKET_SEQUENCE_NONE;
	ipc_host_context[inst].packet_fragment = SII_IPC_PACKET_FRAGMENT_NONE;

	osal_status = sii_os_semaphore_create("host_semaphore", 1, 1,
			&(ipc_host_context[inst].host_semaphore));
	if (SII_OS_STATUS_SUCCESS != osal_status) {
		err("Cannot create semaphore!");
		status = SII_IPC_STATUS_ERROR_OSAL_SEMAPHORE;
		goto exit;
	}

	osal_status = sii_os_semaphore_create("host_reply_semaphore", 1, 0,
			&(ipc_host_context[inst].host_reply_semaphore));
	if (SII_OS_STATUS_SUCCESS != osal_status) {
		err("Cannot create semaphore!");
		status = SII_IPC_STATUS_ERROR_OSAL_SEMAPHORE;
		goto exit;
	}

	osal_status = sii_os_semaphore_create("host_ntf_queue_semaphore", 1, 1,
			&(ipc_host_context[inst].host_ntf_queue_semaphore));
	if (SII_OS_STATUS_SUCCESS != osal_status) {
		err("Cannot create semaphore!");
		status = SII_IPC_STATUS_ERROR_OSAL_SEMAPHORE;
		goto exit;
	}
#if 0
	status = sii_ipc_hal_host_init(inst);
	if (status != SII_IPC_STATUS_SUCCESS) {
		goto exit;
	}

	/* enable slave interrupt */
	status = sii_ipc_hal_set_slave_interrupt_mask(inst, true);
	if (status != SII_IPC_STATUS_SUCCESS) {
		goto exit;
	}
#endif
exit:
	return status;
}

enum sii_ipc_status sii_ipc_exit(sii_inst_t inst) {
	enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;
	enum sii_os_status osal_status = SII_OS_STATUS_SUCCESS;

	if (NULL != ipc_host_context[inst].host_semaphore) {
		osal_status = sii_os_semaphore_delete(
				ipc_host_context[inst].host_semaphore);
		if (SII_OS_STATUS_SUCCESS != osal_status) {
			err("Cannot delete semaphore!");
		}
	}

	if (NULL != ipc_host_context[inst].host_reply_semaphore) {
		osal_status = sii_os_semaphore_delete(
				ipc_host_context[inst].host_reply_semaphore);
		if (SII_OS_STATUS_SUCCESS != osal_status) {
			err("Cannot delete semaphore!");
		}
	}

	if (NULL != ipc_host_context[inst].host_ntf_queue_semaphore) {
		osal_status = sii_os_semaphore_delete(
				ipc_host_context[inst].host_ntf_queue_semaphore);
		if (SII_OS_STATUS_SUCCESS != osal_status) {
			err("Cannot delete semaphore!");
		}
	}

	return status;
}

enum sii_ipc_status sii_ipc_host_handler(sii_inst_t inst) {
	enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;

	bool source_reply = false;
	bool source_notification = false;
	bool process_reply = false;
	bool process_notification = false;

	bool need_to_handle_ntf = false;
	bool need_to_clear_interrupt = false;
	bool need_to_give_semaphore = false;

	status = sii_ipc_host_enable_interrupt(inst, false);
	if (status != SII_IPC_STATUS_SUCCESS) {
		goto exit;
	}

	/* Check Interrupt Source */
	status = sii_ipc_host_check_change(inst,
			&source_reply, &source_notification);
	if (status != SII_IPC_STATUS_SUCCESS) {
		goto exit;
	}

	/* Check State Machine Status */
	switch(ipc_host_context[inst].host_sm) {
		case SII_IPC_HOST_SM_IDLE:
			/* Must be a Notification, discard Reply if any */
			process_reply = false;

			/* Need to Receive Notification */
			process_notification = true;
			break;

		case SII_IPC_HOST_SM_SEND_COMMAND:
			/* Must be a Notification, discard Reply if any */
			process_reply = false;

			/* Need to Receive Notification */
			process_notification = true;
			break;

		case SII_IPC_HOST_SM_WAIT_REPLY:
			/* May have a Reply */
			process_reply = true;

			/* Need to Receive Notification */
			process_notification = true;
			break;

		case SII_IPC_HOST_SM_BUFFER_REPLY_IN_PROGRESS:
			/* May have a Reply */
			process_reply = true;

			/* Need to Receive Notification */
			process_notification = true;
			break;

		default:
			break;
	}

	/* Receive Reply */
	if (process_reply) {
		if (source_reply) {
			/* There is a wantted Reply */
			/* Register Reply or One-Buffer Reply
			   or Multi-Buffer Reply will be processed
			   where the semaphonre is consumed */
			need_to_give_semaphore = true;
			/* State Machine Status will be updated
			   after the semaphore is consumed */
		}
	} else if (source_reply) {
		warn("Unwanted Reply.\n");
		/* Discard unwanted Reply, simply
		   clear the interrupt per Protocol */
		need_to_clear_interrupt = true;
	}

	/* Receive Notification */
	if (process_notification && source_notification) {

		/* FD, TBC, double check this */

		if (process_reply && source_reply) {
			/* FD: TBU, no support for this case yet */
			/* Host should handle Reply first,
			   add Notification in queue */

			need_to_clear_interrupt = false;
		} else if (process_reply) {
			/* Host should wait for Reply back,
			   add Notification in queue */
			need_to_handle_ntf = true;

			need_to_clear_interrupt = true;
		} else {
			/* Process Notification right now */
			need_to_handle_ntf = true;

			need_to_clear_interrupt = true;
		}
	} else if (source_notification) {
		warn("Unwanted Notification.\n");
		/* Discard unwanted Notification, simply
		   clear the interrupt per Protocol */
		need_to_clear_interrupt = true;
	} else {
		/* No Notification now, nothing to do */
	}

	if (need_to_handle_ntf) {
		/* Retrieve the Packet Notification */
		status = sii_ipc_receive_buffer_notification(inst,
			&(ipc_host_context[inst].host_packet_notification));
		if (status != SII_IPC_STATUS_SUCCESS) {
			goto exit_clear_interrupt;
		}

		/* Check Notification Type */
		if (SII_IPC_PHY_NOTIFICATION_TYPE_BUFFER_NOTIFICATION
			== ipc_host_context[inst].host_packet_notification.type) {
			/* Extract Notification per Notification Packet*/
			status =
				sii_ipc_extract_notification_from_packet_ntf(inst,
				ipc_host_context[inst].host_packet_notification.packet);
			if (status != SII_IPC_STATUS_SUCCESS) {
				goto exit_clear_interrupt;
			}
		}
		/* FD: TBU, support of other Notification Types */
	}

exit_clear_interrupt:
	if (need_to_clear_interrupt) {
		status = sii_ipc_host_clear_and_enable_interrupt(inst);
		if (status != SII_IPC_STATUS_SUCCESS) {
			goto exit_give_semaphore;
		}
	}

exit_give_semaphore:
	if (need_to_give_semaphore) {
		status = sii_ipc_host_give_reply_semaphore(inst);
		if (status != SII_IPC_STATUS_SUCCESS) {
			goto exit;
		}
	}

exit:
	sii_ipc_display_error(status);
	return status;
}
