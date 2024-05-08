/*
  2016 (C) Lattice Semiconductor Corporation

  No part of this work may be reproduced, modified, distributed, transmitted,
  transcribed, or translated into any language or computer format, in any form
  or by any means without written permission of Lattice Semiconductor, Inc.,
  111 SW 5th Ave, Suite 700, Portland, OR 97204.

*/

#include "Sii9396_osal.h"
#include "Sii9396_ipc_api.h"


#ifdef SII_IPC_HOST

static enum sii_ipc_status check_message(uint8_t protocol_version,
		uint8_t msg_type, uint16_t msg_id, struct sii_ipc_message *msg)
{
	enum sii_ipc_status ipc_status = SII_IPC_STATUS_SUCCESS;
	if (protocol_version != msg->protocol_version)
		ipc_status = SII_IPC_STATUS_FAILURE;
	else if (msg_type != msg->message_payload.message_type)
		ipc_status = SII_IPC_STATUS_FAILURE;
	else if (msg_id != ((msg->message_payload.message_id_l) |
			(((uint16_t) msg->message_payload.message_id_h) << 8)))
		ipc_status = SII_IPC_STATUS_FAILURE;
	return ipc_status;
}

static enum sii_ipc_status sii_check_message_validity(uint8_t protocol_version,
		uint8_t msg_type, uint16_t msg_id, struct sii_ipc_message *msg)
{
	uint16_t msg_length = msg->message_length_l |
			((uint16_t) msg->message_length_h) << 8;
	if (!verify_checksum_byte((uint8_t *) msg, (uint8_t) msg_length))
		return SII_IPC_STATUS_ERROR_MESSAGE_CHECKSUM;
	if (SII_IPC_STATUS_SUCCESS != check_message(
												protocol_version,
												msg_type, msg_id, msg))
		return SII_IPC_STATUS_ERROR_MESSAGE;
	return SII_IPC_STATUS_SUCCESS;
}

static void sii_compose_message(
		uint8_t protocol_version,
		uint8_t message_type,
		uint16_t message_id,
		uint16_t message_length,
		struct sii_ipc_message *message)
{

	message->message_payload.message_type = message_type;
	message->message_payload.message_id_h = message_id >> 8;
	message->message_payload.message_id_l = message_id & 0xFF;
	/* message_payload data is already filled by the function caller */

	message->protocol_version = protocol_version;
	message_length += SII_IPC_MESSAGE_HEADER_LENGTH;
	message->message_length_h = (uint8_t) (message_length >> 8);
	message->message_length_l = (uint8_t) (message_length & 0xFF);
	message->message_checksum = calculate_checksum_byte(
			(uint8_t *) message,
			(uint8_t) message_length);
}

static enum sii_ipc_api_status sii_api_reg_cmd_reg_reply_process(
		sii_inst_t inst,
		struct sii_ipc_register_command command,
		struct sii_ipc_register_reply *reply)
{
	enum sii_ipc_api_status api_status = SII_IPC_API_SUCCESS;
	enum sii_ipc_status ipc_status = SII_IPC_STATUS_SUCCESS;

	ipc_status = sii_execute_register_command_register_reply(inst,
																command,
																reply);
	if (ipc_status != SII_IPC_STATUS_SUCCESS) {
		api_status = SII_IPC_API_FAILURE;
		//last_err = SII_IPC_API_ERR__INTERNAL;
		goto exit;
	}

	if (reply->reply_status != SII_IPC_PHY_REPLY_STATUS_SUCCESS) {
		api_status = SII_IPC_API_FAILURE;
		/* Save api error code */
		//last_err = reply->reply_return;
	}
exit:
	sii_ipc_display_error(ipc_status);
	return api_status;
}

static enum sii_ipc_api_status sii_api_reg_cmd_msg_reply_process(
		sii_inst_t inst,
		struct sii_ipc_register_command command,
		struct sii_ipc_message_reply *reply)
{
	enum sii_ipc_api_status api_status = SII_IPC_API_SUCCESS;
	enum sii_ipc_status ipc_status = SII_IPC_STATUS_SUCCESS;

	ipc_status = sii_execute_register_command_message_reply(inst,
															command,
															reply);
	if (ipc_status != SII_IPC_STATUS_SUCCESS) {
		api_status = SII_IPC_API_FAILURE;
		//last_err = SII_IPC_API_ERR__INTERNAL;
		goto exit;
	}

	if (reply->reply_status != SII_IPC_PHY_REPLY_STATUS_SUCCESS) {
		api_status = SII_IPC_API_FAILURE;
		/* Save api error code */
		//last_err = reply->reply_return;
	} else {
		ipc_status = sii_check_message_validity(
		SII_IPC_MESSAGE_PROTOCOL_VERSION_1,
												SII_IPC_MESSAGE_TYPE_REPLY,
												command.opcode,
												reply->message);
		if (ipc_status == SII_IPC_STATUS_ERROR_MESSAGE_CHECKSUM) {
			api_status = SII_IPC_API_FAILURE;
			//last_err = SII_IPC_API_ERR__CHECKSUM;
		}
		else if (ipc_status == SII_IPC_STATUS_ERROR_MESSAGE) {
			api_status = SII_IPC_API_FAILURE;
			//last_err = SII_IPC_API_ERR__MSG_FORMAT;
		}
	}

exit:
	//sii_os_sleep_msec(10);
	sii_ipc_display_error(ipc_status);
	return api_status;
}

static enum sii_ipc_api_status sii_api_msg_cmd_reg_reply_process(
		sii_inst_t inst,
		struct sii_ipc_message_command command,
		struct sii_ipc_register_reply *reply)
{
	enum sii_ipc_api_status api_status = SII_IPC_API_SUCCESS;
	enum sii_ipc_status ipc_status = SII_IPC_STATUS_SUCCESS;
	uint16_t raw_msg_length = (command.message->message_length_l) |
			(((uint16_t) (command.message->message_length_h)) << 8);
	sii_compose_message(
	SII_IPC_MESSAGE_PROTOCOL_VERSION_1,
						SII_IPC_MESSAGE_TYPE_COMMAND,
						command.opcode,
						raw_msg_length,
						command.message);
	ipc_status = sii_execute_message_command_register_reply(inst,
															&command,
															reply);
	if (ipc_status != SII_IPC_STATUS_SUCCESS) {
		api_status = SII_IPC_API_FAILURE;
		//last_err = SII_IPC_API_ERR__INTERNAL;
		goto exit;
	}

	if (reply->reply_status != SII_IPC_PHY_REPLY_STATUS_SUCCESS) {
		api_status = SII_IPC_API_FAILURE;
		/* Save api error code */
		//last_err = reply->reply_return;
	}
exit:
	//sii_os_sleep_msec(10);
	sii_ipc_display_error(ipc_status);
	return api_status;
}

static enum sii_ipc_api_status sii_api_msg_cmd_msg_reply_process(
		sii_inst_t inst,
		struct sii_ipc_message_command command,
		struct sii_ipc_message_reply *reply)
{
	enum sii_ipc_api_status api_status = SII_IPC_API_SUCCESS;
	enum sii_ipc_status ipc_status = SII_IPC_STATUS_SUCCESS;
	uint16_t raw_msg_length = (command.message->message_length_l) |
			(((uint16_t) (command.message->message_length_h)) << 8);
	sii_compose_message(
	SII_IPC_MESSAGE_PROTOCOL_VERSION_1,
						SII_IPC_MESSAGE_TYPE_COMMAND,
						command.opcode,
						raw_msg_length,
						command.message);
	ipc_status = sii_execute_message_command_message_reply(inst,
															&command,
															reply);
	if (ipc_status != SII_IPC_STATUS_SUCCESS) {
		api_status = SII_IPC_API_FAILURE;
		//last_err = SII_IPC_API_ERR__INTERNAL;
		goto exit;
	}

	if (reply->reply_status != SII_IPC_PHY_REPLY_STATUS_SUCCESS) {
		api_status = SII_IPC_API_FAILURE;
		/* Save api error code */
		//last_err = reply->reply_return;
	} else {
		ipc_status = sii_check_message_validity(
		SII_IPC_MESSAGE_PROTOCOL_VERSION_1,
												SII_IPC_MESSAGE_TYPE_REPLY,
												command.opcode,
												reply->message);
		if (ipc_status == SII_IPC_STATUS_ERROR_MESSAGE_CHECKSUM) {
			api_status = SII_IPC_API_FAILURE;
			//last_err = SII_IPC_API_ERR__CHECKSUM;
		}
		else if (ipc_status == SII_IPC_STATUS_ERROR_MESSAGE) {
			api_status = SII_IPC_API_FAILURE;
			//last_err = SII_IPC_API_ERR__MSG_FORMAT;
		}
	}

exit:
	//sii_os_sleep_msec(10);
	sii_ipc_display_error(ipc_status);
	return api_status;
}

enum sii_ipc_api_status sii_host_get_rx_plus5v(
		sii_inst_t inst,
		struct sii_ipc_register_reply *reply)
{
	struct sii_ipc_register_command command;

	command.opcode = SII_IPC_PHY_CMD_OPCODE_GET_RX_PLUS_5V;
	command.parameter = 0x00;

	return sii_api_reg_cmd_reg_reply_process(inst, command, reply);
}

enum sii_ipc_api_status sii_host_get_rx_hpd(
		sii_inst_t inst,
		struct sii_ipc_register_reply *reply)
{
	struct sii_ipc_register_command command;

	command.opcode = SII_IPC_PHY_CMD_OPCODE_GET_RX_HPD;
	command.parameter = 0x00;

	return sii_api_reg_cmd_reg_reply_process(inst, command, reply);
}

enum sii_ipc_api_status sii_host_get_rx_av_mute(
		sii_inst_t inst,
		struct sii_ipc_register_reply *reply)
{
	struct sii_ipc_register_command command;

	command.opcode = SII_IPC_PHY_CMD_OPCODE_GET_RX_AV_MUTE;
	command.parameter = 0x00;

	return sii_api_reg_cmd_reg_reply_process(inst, command, reply);
}

enum sii_ipc_api_status sii_host_get_rx_link_type(
		sii_inst_t inst,
		struct sii_ipc_register_reply *reply)
{
	struct sii_ipc_register_command command;

	command.opcode = SII_IPC_PHY_CMD_OPCODE_GET_RX_LINK_TYPE;
	command.parameter = 0x00;

	return sii_api_reg_cmd_reg_reply_process(inst, command, reply);
}

enum sii_ipc_api_status sii_host_get_rx_edid_content(
		sii_inst_t inst,
		uint8_t blocknum, struct sii_ipc_message *msg)
{
	struct sii_ipc_register_command command;
	struct sii_ipc_message_reply reply;

	command.opcode = SII_IPC_PHY_CMD_OPCODE_GET_RX_EDID_CONTENT;
	command.parameter = blocknum;
	reply.message = msg;

	return sii_api_reg_cmd_msg_reply_process(inst, command, &reply);
}

enum sii_ipc_api_status sii_host_get_rx_info_frame(
		sii_inst_t inst,
		uint8_t type, struct sii_ipc_message *msg)
{
	struct sii_ipc_register_command command;
	struct sii_ipc_message_reply reply;

	command.opcode = SII_IPC_PHY_CMD_OPCODE_GET_RX_INFOFRAME;
	command.parameter = type;
	reply.message = msg;

	return sii_api_reg_cmd_msg_reply_process(inst, command, &reply);
}

enum sii_ipc_api_status sii_host_get_rx_char_error_count(
		sii_inst_t inst,
		struct sii_ipc_message *msg)
{
	struct sii_ipc_register_command command;
	struct sii_ipc_message_reply reply;

	command.opcode = SII_IPC_PHY_CMD_OPCODE_GET_RX_CHARACTER_ERR_CNT;
	command.parameter = 0x00;
	reply.message = msg;

	return sii_api_reg_cmd_msg_reply_process(inst, command, &reply);
}

enum sii_ipc_api_status sii_host_get_rx_tmds_lock_status(
		sii_inst_t inst,
		struct sii_ipc_message *msg)
{
	struct sii_ipc_register_command command;
	struct sii_ipc_message_reply reply;

	command.opcode = SII_IPC_PHY_CMD_OPCODE_GET_RX_TMDS_LOCK_STATUS;
	command.parameter = 0x00;
	reply.message = msg;

	return sii_api_reg_cmd_msg_reply_process(inst, command, &reply);
}

enum sii_ipc_api_status sii_host_get_rx_device_id_string(
		sii_inst_t inst,
		struct sii_ipc_message *msg)
{
	struct sii_ipc_register_command command;
	struct sii_ipc_message_reply reply;

	command.opcode = SII_IPC_PHY_CMD_OPCODE_GET_RX_DEV_ID_STRING;
	command.parameter = 0x00;
	reply.message = msg;

	return sii_api_reg_cmd_msg_reply_process(inst, command, &reply);
}

enum sii_ipc_api_status sii_host_get_rx_video_info(
		sii_inst_t inst,
		struct sii_ipc_message *msg)
{
	struct sii_ipc_register_command command;
	struct sii_ipc_message_reply reply;

	command.opcode = SII_IPC_PHY_CMD_OPCODE_GET_RX_VIDEO_INFO;
	command.parameter = 0x00;
	reply.message = msg;

	return sii_api_reg_cmd_msg_reply_process(inst, command, &reply);
}

enum sii_ipc_api_status sii_host_get_rx_audio_info(
		sii_inst_t inst,
		struct sii_ipc_message *msg)
{
	struct sii_ipc_register_command command;
	struct sii_ipc_message_reply reply;

	command.opcode = SII_IPC_PHY_CMD_OPCODE_GET_RX_AUDIO_INFO;
	command.parameter = 0x00;
	reply.message = msg;

	return sii_api_reg_cmd_msg_reply_process(inst, command, &reply);
}
enum sii_ipc_api_status sii_host_get_rx_reg(
		sii_inst_t inst, struct sii_ipc_message *msg)
{
	struct sii_ipc_message_command msg_command;
	struct sii_ipc_message_reply msg_reply;

	msg_command.opcode = SII_IPC_PHY_CMD_OPCODE_GET_REG;
	msg_command.parameter = 0;
	msg_command.message = msg;
	msg_reply.message = msg;

	return sii_api_msg_cmd_msg_reply_process(inst, msg_command, &msg_reply);
}

enum sii_ipc_api_status sii_host_set_rx_hpd(
		sii_inst_t inst, uint8_t value)
{
	struct sii_ipc_register_command command;
	struct sii_ipc_register_reply reply;

	command.opcode = SII_IPC_PHY_CMD_OPCODE_SET_RX_HPD;
	command.parameter = value;

	return sii_api_reg_cmd_reg_reply_process(inst, command, &reply);
}

enum sii_ipc_api_status sii_host_set_rx_term(
		sii_inst_t inst, uint8_t value)
{
	struct sii_ipc_register_command command;
	struct sii_ipc_register_reply reply;

	command.opcode = SII_IPC_PHY_CMD_OPCODE_SET_RX_TERM;
	command.parameter = value;

	return sii_api_reg_cmd_reg_reply_process(inst, command, &reply);
}

enum sii_ipc_api_status sii_host_set_rx_edid_content(
		sii_inst_t inst,
		uint8_t blocknum, struct sii_ipc_message *msg)
{
	struct sii_ipc_message_command command;
	struct sii_ipc_register_reply reply;

	command.opcode = SII_IPC_PHY_CMD_OPCODE_SET_RX_EDID_CONTENT;
	command.parameter = blocknum;
	command.message = msg;

	return sii_api_msg_cmd_reg_reply_process(inst, command, &reply);
}

enum sii_ipc_api_status sii_host_set_rx_eq(
		sii_inst_t inst, struct sii_ipc_message *msg)
{
	struct sii_ipc_message_command command;
	struct sii_ipc_register_reply reply;

	command.opcode = SII_IPC_PHY_CMD_OPCODE_SET_RX_EQ;
	command.parameter = 0;
	command.message = msg;

	return sii_api_msg_cmd_reg_reply_process(inst, command, &reply);
}

enum sii_ipc_api_status sii_host_set_rx_reg(
		sii_inst_t inst, struct sii_ipc_message *msg)
{
	struct sii_ipc_message_command command;
	struct sii_ipc_register_reply reply;

	command.opcode = SII_IPC_PHY_CMD_OPCODE_SET_REG;
	command.parameter = 1;		//Set offset & value
	command.message = msg;

	return sii_api_msg_cmd_reg_reply_process(inst, command, &reply);
}

enum sii_ipc_api_status sii_host_set_rx_audio_mclk_ratio(
		sii_inst_t inst, uint8_t value)
{
	struct sii_ipc_register_command command;
	struct sii_ipc_register_reply reply;

	command.opcode = SII_IPC_PHY_CMD_OPCODE_SET_RX_AUDIO_MCLK_RATIO;
	command.parameter = value;

	return sii_api_reg_cmd_reg_reply_process(inst, command, &reply);
}

enum sii_ipc_api_status sii_host_set_rx_audio_extraction(
		sii_inst_t inst, uint8_t value)
{
	struct sii_ipc_register_command command;
	struct sii_ipc_register_reply reply;

	command.opcode = SII_IPC_PHY_CMD_OPCODE_SET_RX_AUDIO_EXTRACTION;
	command.parameter = value;

	return sii_api_reg_cmd_reg_reply_process(inst, command, &reply);
}

enum sii_ipc_api_status sii_host_get_rx_scramble_full(
	sii_inst_t inst, struct sii_ipc_register_reply *reply)
{
	struct sii_ipc_register_command command;

	command.opcode = SII_IPC_PHY_CMD_OPCODE_GET_RX_SCRAMBLE_FULL;
	command.parameter = 0x00;

	return sii_api_reg_cmd_reg_reply_process(inst, command, reply);
}

enum sii_ipc_api_status sii_host_get_tx_tmds_status(
		sii_inst_t inst,
		struct sii_ipc_register_reply *reply)
{
	struct sii_ipc_register_command command;

	command.opcode = SII_IPC_PHY_CMD_OPCODE_GET_TX_TMDS_STATUS;
	command.parameter = 0x00;

	return sii_api_reg_cmd_reg_reply_process(inst, command, reply);
}

enum sii_ipc_api_status sii_host_get_tx_av_mute(
		sii_inst_t inst,
		struct sii_ipc_register_reply *reply)
{
	struct sii_ipc_register_command command;

	command.opcode = SII_IPC_PHY_CMD_OPCODE_GET_TX_AV_MUTE;
	command.parameter = 0x00;

	return sii_api_reg_cmd_reg_reply_process(inst, command, reply);
}

enum sii_ipc_api_status sii_host_get_tx_tmds_pin_swap(
		sii_inst_t inst,
		struct sii_ipc_register_reply *reply)
{
	struct sii_ipc_register_command command;

	command.opcode = SII_IPC_PHY_CMD_OPCODE_GET_TX_TMDS_PIN_SWAP;
	command.parameter = 0x00;

	return sii_api_reg_cmd_reg_reply_process(inst, command, reply);
}

enum sii_ipc_api_status sii_host_get_tx_hpd(
		sii_inst_t inst,
		struct sii_ipc_register_reply *reply)
{
	struct sii_ipc_register_command command;

	command.opcode = SII_IPC_PHY_CMD_OPCODE_GET_TX_HPD;
	command.parameter = 0x00;

	return sii_api_reg_cmd_reg_reply_process(inst, command, reply);
}

enum sii_ipc_api_status sii_host_get_tx_rxsense(
		sii_inst_t inst,
		struct sii_ipc_register_reply *reply)
{
	struct sii_ipc_register_command command;

	command.opcode = SII_IPC_PHY_CMD_OPCODE_GET_TX_RXSENSE;
	command.parameter = 0x00;

	return sii_api_reg_cmd_reg_reply_process(inst, command, reply);
}

enum sii_ipc_api_status sii_host_get_tx_ds_edid_status(
		sii_inst_t inst,
		struct sii_ipc_register_reply *reply)
{
	struct sii_ipc_register_command command;

	command.opcode = SII_IPC_PHY_CMD_OPCODE_GET_TX_DS_EDID_STATUS;
	command.parameter = 0x00;

	return sii_api_reg_cmd_reg_reply_process(inst, command, reply);
}

enum sii_ipc_api_status sii_host_get_tx_ds_edid_content(
		sii_inst_t inst,
		uint8_t blocknum, struct sii_ipc_message *msg)
{
	struct sii_ipc_register_command command;
	struct sii_ipc_message_reply reply;

	command.opcode = SII_IPC_PHY_CMD_OPCODE_GET_TX_DS_EDID_CONTENT;
	command.parameter = blocknum;
	reply.message = msg;

	return sii_api_reg_cmd_msg_reply_process(inst, command, &reply);
}

enum sii_ipc_api_status sii_host_get_tx_tmds_swing_level(
		sii_inst_t inst,
		struct sii_ipc_message *msg)
{
	struct sii_ipc_register_command command;
	struct sii_ipc_message_reply reply;

	command.opcode = SII_IPC_PHY_CMD_OPCODE_GET_TX_TMDS_SWING_LEVEL;
	command.parameter = 0x00;
	reply.message = msg;

	return sii_api_reg_cmd_msg_reply_process(inst, command, &reply);
}

enum sii_ipc_api_status sii_host_set_tx_tmds_status(
		sii_inst_t inst, uint8_t value)
{
	struct sii_ipc_register_command command;
	struct sii_ipc_register_reply reply;

	command.opcode = SII_IPC_PHY_CMD_OPCODE_SET_TX_TMDS_STATUS;
	command.parameter = value;

	return sii_api_reg_cmd_reg_reply_process(inst, command, &reply);
}

enum sii_ipc_api_status sii_host_set_tx_av_mute(
		sii_inst_t inst, uint8_t value)
{
	struct sii_ipc_register_command command;
	struct sii_ipc_register_reply reply;

	command.opcode = SII_IPC_PHY_CMD_OPCODE_SET_TX_AV_MUTE;
	command.parameter = value;

	return sii_api_reg_cmd_reg_reply_process(inst, command, &reply);
}

enum sii_ipc_api_status sii_host_set_tx_tmds_swing_level(
		sii_inst_t inst,
		struct sii_ipc_message *msg)
{
	struct sii_ipc_message_command command;
	struct sii_ipc_register_reply reply;

	command.opcode = SII_IPC_PHY_CMD_OPCODE_SET_TX_TMDS_SWING_LEVEL;
	command.parameter = 0x00;
	command.message = msg;

	return sii_api_msg_cmd_reg_reply_process(inst, command, &reply);
}

enum sii_ipc_api_status sii_host_set_tx_tmds_pin_swap(
		sii_inst_t inst, uint8_t value)
{
	struct sii_ipc_register_command command;
	struct sii_ipc_register_reply reply;

	command.opcode = SII_IPC_PHY_CMD_OPCODE_SET_TX_TMDS_PIN_SWAP;
	command.parameter = value;

	return sii_api_reg_cmd_reg_reply_process(inst, command, &reply);
}

enum sii_ipc_api_status sii_host_get_rx_hdcp_ddc(
		sii_inst_t inst,
		struct sii_ipc_register_reply *reply)
{
	struct sii_ipc_register_command command;

	command.opcode = SII_IPC_PHY_CMD_OPCODE_GET_RX_HDCP_DDC;
	command.parameter = 0x00;

	return sii_api_reg_cmd_reg_reply_process(inst, command, reply);
}

enum sii_ipc_api_status sii_host_get_rx_hdcp_us_version(
		sii_inst_t inst,
		struct sii_ipc_register_reply *reply)
{
	struct sii_ipc_register_command command;

	command.opcode = SII_IPC_PHY_CMD_OPCODE_GET_RX_HDCP_US_VERSION;
	command.parameter = 0x00;

	return sii_api_reg_cmd_reg_reply_process(inst, command, reply);
}

enum sii_ipc_api_status sii_host_get_rx_hdcp_status(
		sii_inst_t inst,
		struct sii_ipc_message *msg)
{
	struct sii_ipc_register_command command;
	struct sii_ipc_message_reply reply;

	command.opcode = SII_IPC_PHY_CMD_OPCODE_GET_RX_HDCP_STATUS;
	command.parameter = 0x00;
	reply.message = msg;

	return sii_api_reg_cmd_msg_reply_process(inst, command, &reply);
}

enum sii_ipc_api_status sii_host_get_rx_hdcp_rxid(
		sii_inst_t inst,
		struct sii_ipc_message *msg)
{
	struct sii_ipc_register_command command;
	struct sii_ipc_message_reply reply;

	command.opcode = SII_IPC_PHY_CMD_OPCODE_GET_RX_HDCP_RXID;
	command.parameter = 0x00;
	reply.message = msg;

	return sii_api_reg_cmd_msg_reply_process(inst, command, &reply);
}

enum sii_ipc_api_status sii_host_get_rx_hdcp_bksv(
		sii_inst_t inst,
		struct sii_ipc_message *msg)
{
	struct sii_ipc_register_command command;
	struct sii_ipc_message_reply reply;

	command.opcode = SII_IPC_PHY_CMD_OPCODE_GET_RX_HDCP_BKSV;
	command.parameter = 0x00;
	reply.message = msg;

	return sii_api_reg_cmd_msg_reply_process(inst, command, &reply);
}

enum sii_ipc_api_status sii_host_set_rx_hdcp_ddc(
		sii_inst_t inst, uint8_t value)
{
	struct sii_ipc_register_command command;
	struct sii_ipc_register_reply reply;

	command.opcode = SII_IPC_PHY_CMD_OPCODE_SET_RX_HDCP_DDC;
	command.parameter = value;

	return sii_api_reg_cmd_reg_reply_process(inst, command, &reply);
}

enum sii_ipc_api_status sii_host_get_rx_hdcp_repeater_mode(
		sii_inst_t inst,
		struct sii_ipc_register_reply *reply)
{
	struct sii_ipc_register_command command;

	command.opcode = SII_IPC_PHY_CMD_OPCODE_GET_RX_HDCP_REPEATER_MODE;
	command.parameter = 0x00;

	return sii_api_reg_cmd_reg_reply_process(inst, command, reply);
}

enum sii_ipc_api_status sii_host_set_rx_hdcp_repeater_mode(
		sii_inst_t inst, uint8_t value)
{
	struct sii_ipc_register_command command;
	struct sii_ipc_register_reply reply;

	command.opcode = SII_IPC_PHY_CMD_OPCODE_SET_RX_HDCP_REPEATER_MODE;
	command.parameter = value;

	return sii_api_reg_cmd_reg_reply_process(inst, command, &reply);
}

enum sii_ipc_api_status sii_host_trigger_rx_hdcp_rxid_list_to_send(
		sii_inst_t inst)
{
	struct sii_ipc_register_command command;
	struct sii_ipc_register_reply reply;

	command.opcode = SII_IPC_PHY_CMD_OPCODE_TRIGGER_RX_HDCP_RXID_LIST_TO_SEND;
	command.parameter = 0x00;

	return sii_api_reg_cmd_reg_reply_process(inst, command, &reply);
}

enum sii_ipc_api_status sii_host_set_rx_hdcp_rxid_list(
		sii_inst_t inst,
		struct sii_ipc_message *msg)
{
	struct sii_ipc_message_command command;
	struct sii_ipc_register_reply reply;

	command.opcode = SII_IPC_PHY_CMD_OPCODE_SET_RX_HDCP_RXID_LIST;
	command.parameter = 0x00;
	command.message = msg;

	return sii_api_msg_cmd_reg_reply_process(inst, command, &reply);
}

enum sii_ipc_api_status sii_host_set_rx_hdcp_topology(
		sii_inst_t inst,
		struct sii_ipc_message *msg)
{
	struct sii_ipc_message_command command;
	struct sii_ipc_register_reply reply;

	command.opcode = SII_IPC_PHY_CMD_OPCODE_SET_RX_HDCP_TOPOLOGY;
	command.parameter = 0x00;
	command.message = msg;

	return sii_api_msg_cmd_reg_reply_process(inst, command, &reply);
}

enum sii_ipc_api_status sii_host_get_rx_hdcp_stream_manage_info(
		sii_inst_t inst,
		struct sii_ipc_message *msg)
{
	struct sii_ipc_register_command command;
	struct sii_ipc_message_reply reply;

	command.opcode = SII_IPC_PHY_CMD_OPCODE_GET_RX_HDCP_STREAM_MANAGE_INFO;
	command.parameter = 0x00;
	reply.message = msg;

	return sii_api_reg_cmd_msg_reply_process(inst, command, &reply);
}

enum sii_ipc_api_status sii_host_get_chip_id(
		sii_inst_t inst, struct sii_ipc_message *msg)
{
	struct sii_ipc_register_command command;
	struct sii_ipc_message_reply reply;

	command.opcode = SII_IPC_PHY_CMD_OPCODE_GET_CHIP_ID;
	command.parameter = 0x00;
	reply.message = msg;

	return sii_api_reg_cmd_msg_reply_process(inst, command, &reply);
}

enum sii_ipc_api_status sii_host_get_chip_revision(
		sii_inst_t inst,
		struct sii_ipc_register_reply *reply)
{
	struct sii_ipc_register_command command;

	command.opcode = SII_IPC_PHY_CMD_OPCODE_GET_CHIP_REVISON;
	command.parameter = 0x00;

	return sii_api_reg_cmd_reg_reply_process(inst, command, reply);
}

enum sii_ipc_api_status sii_host_get_product_id(
		sii_inst_t inst,
		struct sii_ipc_register_reply *reply)
{
	struct sii_ipc_register_command command;

	command.opcode = SII_IPC_PHY_CMD_OPCODE_GET_PRODUCT_ID;
	command.parameter = 0x00;

	return sii_api_reg_cmd_reg_reply_process(inst, command, reply);
}

enum sii_ipc_api_status sii_host_get_firmware_version(
		sii_inst_t inst,
		struct sii_ipc_message *msg)
{
	struct sii_ipc_register_command command;
	struct sii_ipc_message_reply reply;

	command.opcode = SII_IPC_PHY_CMD_OPCODE_GET_FW_VERSION;
	command.parameter = 0x00;
	reply.message = msg;

	return sii_api_reg_cmd_msg_reply_process(inst, command, &reply);
}

enum sii_ipc_api_status sii_host_get_release_timestamp(
		sii_inst_t inst,
		struct sii_ipc_message *msg)
{
	struct sii_ipc_register_command command;
	struct sii_ipc_message_reply reply;

	command.opcode = SII_IPC_PHY_CMD_OPCODE_GET_RELEASE_TIMESTAMP;
	command.parameter = 0x00;
	reply.message = msg;

	return sii_api_reg_cmd_msg_reply_process(inst, command, &reply);
}

enum sii_ipc_api_status sii_host_get_boot_status(
		sii_inst_t inst,
		struct sii_ipc_register_reply *reply)
{
	struct sii_ipc_register_command command;

	command.opcode = SII_IPC_PHY_CMD_OPCODE_GET_BOOT_STATUS;
	command.parameter = 0x00;

	return sii_api_reg_cmd_reg_reply_process(inst, command, reply);
}

enum sii_ipc_api_status sii_host_get_host_mode(
		sii_inst_t inst,
		struct sii_ipc_register_reply *reply)
{
	struct sii_ipc_register_command command;

	command.opcode = SII_IPC_PHY_CMD_OPCODE_GET_HOST_MODE;
	command.parameter = 0x00;

	return sii_api_reg_cmd_reg_reply_process(inst, command, reply);
}

enum sii_ipc_api_status sii_host_set_host_mode(
		sii_inst_t inst, uint8_t value)
{
	struct sii_ipc_register_command command;
	struct sii_ipc_register_reply reply;

	command.opcode = SII_IPC_PHY_CMD_OPCODE_SET_HOST_MODE;
	command.parameter = value;

	return sii_api_reg_cmd_reg_reply_process(inst, command, &reply);
}

enum sii_ipc_api_status sii_host_set_tx_scramble(
		sii_inst_t inst, uint8_t value)
{
	struct sii_ipc_register_command command;
	struct sii_ipc_register_reply reply;

	command.opcode = SII_IPC_PHY_CMD_OPCODE_SET_TX_SCRAMBLE;
	command.parameter = value;

	return sii_api_reg_cmd_reg_reply_process(inst, command, &reply);
}

enum sii_ipc_api_status sii_host_set_tx_scramble_full(
		sii_inst_t inst, uint8_t value)
{
	struct sii_ipc_register_command command;
	struct sii_ipc_register_reply reply;

	command.opcode = SII_IPC_PHY_CMD_OPCODE_SET_TX_SCRAMBLE_FULL;
	command.parameter = value;

	return sii_api_reg_cmd_reg_reply_process(inst, command, &reply);
}

enum sii_ipc_api_status sii_host_get_tx_char_error_count(
		sii_inst_t inst,
		struct sii_ipc_message *msg)
{
	struct sii_ipc_register_command command;
	struct sii_ipc_message_reply reply;

	command.opcode = SII_IPC_PHY_CMD_OPCODE_GET_TX_CHARACTER_ERR_CNT;
	command.parameter = 0x00;
	reply.message = msg;

	return sii_api_reg_cmd_msg_reply_process(inst, command, &reply);
}

enum sii_ipc_api_status sii_host_get_tx_device_id_string(
		sii_inst_t inst,
		struct sii_ipc_message *msg)
{
	struct sii_ipc_register_command command;
	struct sii_ipc_message_reply reply;

	command.opcode = SII_IPC_PHY_CMD_OPCODE_GET_TX_DEV_ID_STRING;
	command.parameter = 0x00;
	reply.message = msg;

	return sii_api_reg_cmd_msg_reply_process(inst, command, &reply);
}

enum sii_ipc_api_status sii_host_set_tx_color_space(
		sii_inst_t inst, uint8_t value)
{
	struct sii_ipc_register_command command;
	struct sii_ipc_register_reply reply;

	command.opcode = SII_IPC_PHY_CMD_OPCODE_SET_TX_COLOR_SPACE;
	command.parameter = value;

	return sii_api_reg_cmd_reg_reply_process(inst, command, &reply);
}

enum sii_ipc_api_status sii_host_set_tx_hdcp_protection(
		sii_inst_t inst, uint8_t value)
{
	struct sii_ipc_register_command command;
	struct sii_ipc_register_reply reply;

	command.opcode = SII_IPC_PHY_CMD_OPCODE_SET_TX_HDCP_PROTECTION;
	command.parameter = value;

	return sii_api_reg_cmd_reg_reply_process(inst, command, &reply);
}

enum sii_ipc_api_status sii_host_set_tx_hdcp_content_type(
		sii_inst_t inst, uint8_t value)
{
	struct sii_ipc_register_command command;
	struct sii_ipc_register_reply reply;

	command.opcode = SII_IPC_PHY_CMD_OPCODE_SET_TX_HDCP_CONTENT_TYPE;
	command.parameter = value;

	return sii_api_reg_cmd_reg_reply_process(inst, command, &reply);
}

enum sii_ipc_api_status sii_host_get_tx_hdcp_ds_version(
		sii_inst_t inst,
		struct sii_ipc_register_reply *reply)
{
	struct sii_ipc_register_command command;

	command.opcode = SII_IPC_PHY_CMD_OPCODE_GET_TX_HDCP_DS_VERSION;
	command.parameter = 0x00;

	return sii_api_reg_cmd_reg_reply_process(inst, command, reply);
}

enum sii_ipc_api_status sii_host_get_tx_hdcp_status(
		sii_inst_t inst,
		struct sii_ipc_message *msg)
{
	struct sii_ipc_register_command command;
	struct sii_ipc_message_reply reply;

	command.opcode = SII_IPC_PHY_CMD_OPCODE_GET_TX_HDCP_STATUS;
	command.parameter = 0x00;
	reply.message = msg;

	return sii_api_reg_cmd_msg_reply_process(inst, command, &reply);
}

enum sii_ipc_api_status sii_host_set_tx_audio_insertion(
		sii_inst_t inst,
		struct sii_ipc_message *msg)
{
	struct sii_ipc_message_command command;
	struct sii_ipc_register_reply reply;

	command.opcode = SII_IPC_PHY_CMD_OPCODE_SET_TX_AUDIO_INSERTION;
	command.parameter = 0;
	command.message = msg;

	return sii_api_msg_cmd_reg_reply_process(inst, command, &reply);
}

enum sii_ipc_api_status sii_host_get_tx_hdcp_rcvid(
		sii_inst_t inst,
		struct sii_ipc_message *msg)
{
	struct sii_ipc_register_command command;
	struct sii_ipc_message_reply reply;

	command.opcode = SII_IPC_PHY_CMD_OPCODE_GET_TX_HDCP_RCVID;
	command.parameter = 0x00;
	reply.message = msg;

	return sii_api_reg_cmd_msg_reply_process(inst, command, &reply);
}

enum sii_ipc_api_status sii_host_get_tx_hdcp_ds_rxid_list(
		sii_inst_t inst,
		struct sii_ipc_message *msg)
{
	struct sii_ipc_message_command command;
	struct sii_ipc_message_reply reply;

	command.opcode = SII_IPC_PHY_CMD_OPCODE_GET_TX_HDCP_DS_RXID_LIST;
	command.parameter = 0x00;
	command.message = msg;
	reply.message = msg;

	return sii_api_msg_cmd_msg_reply_process(inst, command, &reply);
}

enum sii_ipc_api_status sii_host_get_tx_hdcp_ds_repeater_bit(
		sii_inst_t inst,
		struct sii_ipc_register_reply *reply)
{
	struct sii_ipc_register_command command;

	command.opcode = SII_IPC_PHY_CMD_OPCODE_GET_TX_HDCP_DS_REPEATER_BIT;
	command.parameter = 0x00;

	return sii_api_reg_cmd_reg_reply_process(inst, command, reply);
}

enum sii_ipc_api_status sii_host_get_tx_hdcp_ds_topology(
		sii_inst_t inst,
		struct sii_ipc_message *msg)
{
	struct sii_ipc_register_command command;
	struct sii_ipc_message_reply reply;

	command.opcode = SII_IPC_PHY_CMD_OPCODE_GET_TX_HDCP_DS_TOPOLOGY;
	command.parameter = 0x00;
	reply.message = msg;

	return sii_api_reg_cmd_msg_reply_process(inst, command, &reply);
}

enum sii_ipc_api_status sii_host_set_tx_hdcp_stream_manage_info(
		sii_inst_t inst,
		struct sii_ipc_message *msg)
{
	struct sii_ipc_message_command command;
	struct sii_ipc_register_reply reply;

	command.opcode = SII_IPC_PHY_CMD_OPCODE_SET_TX_HDCP_STREAM_MANAGE_INFO;
	command.parameter = 0;
	command.message = msg;

	return sii_api_msg_cmd_reg_reply_process(inst, command, &reply);
}

#endif /* SII_IPC_HOST */
