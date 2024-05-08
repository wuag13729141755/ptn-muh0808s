/*
  2016 (C) Lattice Semiconductor Corporation

  No part of this work may be reproduced, modified, distributed, transmitted,
  transcribed, or translated into any language or computer format, in any form
  or by any means without written permission of Lattice Semiconductor, Inc.,
  111 SW 5th Ave, Suite 700, Portland, OR 97204.


*/

#ifndef _IPC_API_H
#define _IPC_API_H




#ifdef SII_IPC_HOST

#include "Sii9396_ipc_host.h"
enum sii_ipc_api_status {
	SII_IPC_API_SUCCESS,
	SII_IPC_API_FAILURE,
};

enum sii_ipc_command_extend_opcode {
	/* 8 General Opcodes: 0x10 ~ 0x17*/
	SII_IPC_PHY_CMD_OPCODE_GET_CHIP_ID = 0x10,
	SII_IPC_PHY_CMD_OPCODE_GET_CHIP_REVISON,
	SII_IPC_PHY_CMD_OPCODE_GET_FW_VERSION,
	SII_IPC_PHY_CMD_OPCODE_GET_RELEASE_TIMESTAMP,
	SII_IPC_PHY_CMD_OPCODE_GET_BOOT_STATUS,
	SII_IPC_PHY_CMD_OPCODE_GET_REG,
	SII_IPC_PHY_CMD_OPCODE_SET_REG,
	SII_IPC_PHY_CMD_OPCODE_GET_PRODUCT_ID,
	/* 40 HDMI Common Opcodes: 0x18 ~ 0x3F*/
	SII_IPC_PHY_CMD_OPCODE_GET_RX_PLUS_5V = 0x18,
	SII_IPC_PHY_CMD_OPCODE_GET_RX_HPD,
	SII_IPC_PHY_CMD_OPCODE_SET_RX_HPD,
	SII_IPC_PHY_CMD_OPCODE_GET_RX_AV_MUTE,
	SII_IPC_PHY_CMD_OPCODE_GET_RX_LINK_TYPE,
	SII_IPC_PHY_CMD_OPCODE_GET_RX_EDID_CONTENT,
	SII_IPC_PHY_CMD_OPCODE_SET_RX_EDID_CONTENT,
	SII_IPC_PHY_CMD_OPCODE_GET_RX_INFOFRAME,
	SII_IPC_PHY_CMD_OPCODE_GET_RX_TMDS_LOCK_STATUS,
	SII_IPC_PHY_CMD_OPCODE_GET_RX_CHARACTER_ERR_CNT,
	SII_IPC_PHY_CMD_OPCODE_GET_RX_DEV_ID_STRING,
	SII_IPC_PHY_CMD_OPCODE_GET_RX_VIDEO_INFO,
	SII_IPC_PHY_CMD_OPCODE_SET_RX_TERM,
	SII_IPC_PHY_CMD_OPCODE_SET_RX_EQ,
	SII_IPC_PHY_CMD_OPCODE_GET_RX_AUDIO_INFO,
	SII_IPC_PHY_CMD_OPCODE_SET_RX_AUDIO_MCLK_RATIO,
	SII_IPC_PHY_CMD_OPCODE_SET_RX_AUDIO_EXTRACTION,
	SII_IPC_PHY_CMD_OPCODE_GET_RX_SCRAMBLE_FULL,

	SII_IPC_PHY_CMD_OPCODE_GET_TX_AV_MUTE = 0x2B,
	SII_IPC_PHY_CMD_OPCODE_SET_TX_AV_MUTE,
	SII_IPC_PHY_CMD_OPCODE_GET_TX_TMDS_STATUS,
	SII_IPC_PHY_CMD_OPCODE_SET_TX_TMDS_STATUS,
	SII_IPC_PHY_CMD_OPCODE_GET_TX_TMDS_SWING_LEVEL,
	SII_IPC_PHY_CMD_OPCODE_SET_TX_TMDS_SWING_LEVEL,
	SII_IPC_PHY_CMD_OPCODE_GET_TX_TMDS_PIN_SWAP,
	SII_IPC_PHY_CMD_OPCODE_SET_TX_TMDS_PIN_SWAP,
	SII_IPC_PHY_CMD_OPCODE_GET_TX_HPD,
	SII_IPC_PHY_CMD_OPCODE_GET_TX_RXSENSE,
	SII_IPC_PHY_CMD_OPCODE_GET_TX_DS_EDID_STATUS,
	SII_IPC_PHY_CMD_OPCODE_GET_TX_DS_EDID_CONTENT,
	SII_IPC_PHY_CMD_OPCODE_SET_TX_SCRAMBLE,
	SII_IPC_PHY_CMD_OPCODE_GET_TX_CHARACTER_ERR_CNT,
	SII_IPC_PHY_CMD_OPCODE_GET_TX_DEV_ID_STRING,
	SII_IPC_PHY_CMD_OPCODE_SET_TX_COLOR_SPACE,
	SII_IPC_PHY_CMD_OPCODE_SET_TX_AUDIO_INSERTION,
	SII_IPC_PHY_CMD_OPCODE_SET_TX_SCRAMBLE_FULL,
	/* 24 Rx HDCP Opcodes: 0x40 ~ 0x57*/
	SII_IPC_PHY_CMD_OPCODE_GET_RX_HDCP_DDC = 0x40,
	SII_IPC_PHY_CMD_OPCODE_GET_RX_HDCP_STATUS,
	SII_IPC_PHY_CMD_OPCODE_GET_RX_HDCP_US_VERSION,
	SII_IPC_PHY_CMD_OPCODE_GET_RX_HDCP_BKSV,
	SII_IPC_PHY_CMD_OPCODE_GET_RX_HDCP_RXID,
	SII_IPC_PHY_CMD_OPCODE_SET_RX_HDCP_DDC,
	SII_IPC_PHY_CMD_OPCODE_GET_RX_HDCP_REPEATER_MODE,
	SII_IPC_PHY_CMD_OPCODE_SET_RX_HDCP_REPEATER_MODE,
	SII_IPC_PHY_CMD_OPCODE_TRIGGER_RX_HDCP_RXID_LIST_TO_SEND,
	SII_IPC_PHY_CMD_OPCODE_SET_RX_HDCP_RXID_LIST,
	SII_IPC_PHY_CMD_OPCODE_SET_RX_HDCP_TOPOLOGY,
	SII_IPC_PHY_CMD_OPCODE_GET_RX_HDCP_STREAM_MANAGE_INFO,
	/* 24 Tx HDCP Opcodes: 0x58 ~ 0x6F*/
	SII_IPC_PHY_CMD_OPCODE_SET_TX_HDCP_PROTECTION = 0x58,
	SII_IPC_PHY_CMD_OPCODE_SET_TX_HDCP_CONTENT_TYPE,
	SII_IPC_PHY_CMD_OPCODE_GET_TX_HDCP_DS_VERSION,
	SII_IPC_PHY_CMD_OPCODE_GET_TX_HDCP_STATUS,
	SII_IPC_PHY_CMD_OPCODE_GET_TX_HDCP_RCVID,
	SII_IPC_PHY_CMD_OPCODE_GET_TX_HDCP_DS_RXID_LIST,
	SII_IPC_PHY_CMD_OPCODE_GET_TX_HDCP_DS_REPEATER_BIT,
	SII_IPC_PHY_CMD_OPCODE_GET_TX_HDCP_DS_TOPOLOGY,
	SII_IPC_PHY_CMD_OPCODE_SET_TX_HDCP_STREAM_MANAGE_INFO,
	/* 14 Opcodes reserved for Command Extension: 0b111_0000 ~ 0b111_1101 */
	SII_IPC_PHY_CMD_OPCODE_GET_HOST_MODE = 0x70,
	SII_IPC_PHY_CMD_OPCODE_SET_HOST_MODE,
};

enum sii_ipc_api_status sii_host_get_rx_plus5v(
	sii_inst_t inst,
	struct sii_ipc_register_reply *reply);
enum sii_ipc_api_status sii_host_get_rx_hpd(
	sii_inst_t inst,
	struct sii_ipc_register_reply *reply);
enum sii_ipc_api_status sii_host_get_rx_av_mute(
	sii_inst_t inst,
	struct sii_ipc_register_reply *reply);
enum sii_ipc_api_status sii_host_get_rx_link_type(
	sii_inst_t inst,
	struct sii_ipc_register_reply *reply);
enum sii_ipc_api_status sii_host_get_rx_edid_content(
	sii_inst_t inst,
	uint8_t blocknum, struct sii_ipc_message *msg);
enum sii_ipc_api_status sii_host_get_rx_info_frame(
	sii_inst_t inst,
	uint8_t type, struct sii_ipc_message *msg);
enum sii_ipc_api_status sii_host_get_rx_char_error_count(
	sii_inst_t inst,
	struct sii_ipc_message *msg);
enum sii_ipc_api_status sii_host_get_rx_tmds_lock_status(
	sii_inst_t inst,
	struct sii_ipc_message *msg);
enum sii_ipc_api_status sii_host_get_rx_device_id_string(
	sii_inst_t inst,
	struct sii_ipc_message *msg);
enum sii_ipc_api_status sii_host_get_rx_video_info(
	sii_inst_t inst,
	struct sii_ipc_message *msg);
enum sii_ipc_api_status sii_host_get_rx_audio_info(
	sii_inst_t inst,
	struct sii_ipc_message *msg);
enum sii_ipc_api_status sii_host_get_rx_reg(
	sii_inst_t inst, struct sii_ipc_message *msg);
enum sii_ipc_api_status sii_host_set_rx_hpd(
	sii_inst_t inst, uint8_t value);
enum sii_ipc_api_status sii_host_set_rx_term(
	sii_inst_t inst, uint8_t value);
enum sii_ipc_api_status sii_host_set_rx_edid_content(
	sii_inst_t inst,
	uint8_t blocknum, struct sii_ipc_message *msg);
enum sii_ipc_api_status sii_host_set_rx_eq(
	sii_inst_t inst, struct sii_ipc_message *msg);
enum sii_ipc_api_status sii_host_set_rx_audio_mclk_ratio(
	sii_inst_t inst, uint8_t value);
enum sii_ipc_api_status sii_host_set_rx_audio_extraction(
		sii_inst_t inst, uint8_t value);
enum sii_ipc_api_status sii_host_get_rx_scramble_full(
	sii_inst_t inst, struct sii_ipc_register_reply *reply);
enum sii_ipc_api_status sii_host_set_rx_reg(
	sii_inst_t inst, struct sii_ipc_message *msg);
enum sii_ipc_api_status sii_host_get_tx_tmds_status(
	sii_inst_t inst,
	struct sii_ipc_register_reply *reply);
enum sii_ipc_api_status sii_host_get_tx_av_mute(
	sii_inst_t inst,
	struct sii_ipc_register_reply *reply);
enum sii_ipc_api_status sii_host_get_tx_tmds_pin_swap(
	sii_inst_t inst,
	struct sii_ipc_register_reply *reply);
enum sii_ipc_api_status sii_host_get_tx_hpd(
	sii_inst_t inst,
	struct sii_ipc_register_reply *reply);
enum sii_ipc_api_status sii_host_get_tx_rxsense(
	sii_inst_t inst,
	struct sii_ipc_register_reply *reply);
enum sii_ipc_api_status sii_host_get_tx_ds_edid_status(
	sii_inst_t inst,
	struct sii_ipc_register_reply *reply);
enum sii_ipc_api_status sii_host_get_tx_ds_edid_content(
	sii_inst_t inst,
	uint8_t blocknum, struct sii_ipc_message *msg);
enum sii_ipc_api_status sii_host_get_tx_tmds_swing_level(
	sii_inst_t inst,
	struct sii_ipc_message *msg);
enum sii_ipc_api_status sii_host_set_tx_tmds_status(
	sii_inst_t inst, uint8_t value);
enum sii_ipc_api_status sii_host_set_tx_av_mute(
	sii_inst_t inst, uint8_t value);
enum sii_ipc_api_status sii_host_set_tx_tmds_swing_level(
	sii_inst_t inst,
	struct sii_ipc_message *msg);
enum sii_ipc_api_status sii_host_set_tx_tmds_pin_swap(
	sii_inst_t inst, uint8_t value);
enum sii_ipc_api_status sii_host_get_rx_hdcp_ddc(
	sii_inst_t inst,
	struct sii_ipc_register_reply *reply);
enum sii_ipc_api_status sii_host_get_rx_hdcp_us_version(
	sii_inst_t inst,
	struct sii_ipc_register_reply *reply);
enum sii_ipc_api_status sii_host_get_rx_hdcp_status(
	sii_inst_t inst,
		struct sii_ipc_message *msg);
enum sii_ipc_api_status sii_host_get_rx_hdcp_rxid(
	sii_inst_t inst,
	struct sii_ipc_message *msg);
enum sii_ipc_api_status sii_host_get_rx_hdcp_bksv(
	sii_inst_t inst,
	struct sii_ipc_message *msg);
enum sii_ipc_api_status sii_host_set_rx_hdcp_ddc(
	sii_inst_t inst, uint8_t value);
enum sii_ipc_api_status sii_host_get_rx_hdcp_repeater_mode(
	sii_inst_t inst,
	struct sii_ipc_register_reply *reply);
enum sii_ipc_api_status sii_host_set_rx_hdcp_repeater_mode(
	sii_inst_t inst, uint8_t value);
enum sii_ipc_api_status sii_host_trigger_rx_hdcp_rxid_list_to_send(
	sii_inst_t inst);
enum sii_ipc_api_status sii_host_set_rx_hdcp_rxid_list(
	sii_inst_t inst,
	struct sii_ipc_message *msg);
enum sii_ipc_api_status sii_host_set_rx_hdcp_topology(
	sii_inst_t inst,
	struct sii_ipc_message *msg);
enum sii_ipc_api_status sii_host_get_rx_hdcp_stream_manage_info(
	sii_inst_t inst,
	struct sii_ipc_message *msg);
enum sii_ipc_api_status sii_host_get_chip_id(
	sii_inst_t inst, struct sii_ipc_message *msg);
enum sii_ipc_api_status sii_host_get_chip_revision(
	sii_inst_t inst,
	struct sii_ipc_register_reply *reply);
enum sii_ipc_api_status sii_host_get_product_id(
		sii_inst_t inst,
		struct sii_ipc_register_reply *reply);
enum sii_ipc_api_status sii_host_get_firmware_version(
	sii_inst_t inst,
	struct sii_ipc_message *msg);
enum sii_ipc_api_status sii_host_get_release_timestamp(
	sii_inst_t inst,
	struct sii_ipc_message *msg);
enum sii_ipc_api_status sii_host_get_boot_status(
	sii_inst_t inst,
	struct sii_ipc_register_reply *reply);
enum sii_ipc_api_status sii_host_get_host_mode(
	sii_inst_t inst,
	struct sii_ipc_register_reply *reply);
enum sii_ipc_api_status sii_host_set_host_mode(
	sii_inst_t inst, uint8_t value);
enum sii_ipc_api_status sii_host_set_tx_scramble(
	sii_inst_t inst, uint8_t value);
enum sii_ipc_api_status sii_host_set_tx_scramble_full(
	sii_inst_t inst, uint8_t value);
enum sii_ipc_api_status sii_host_get_tx_char_error_count(
	sii_inst_t inst,
	struct sii_ipc_message *msg);
enum sii_ipc_api_status sii_host_get_tx_device_id_string(
	sii_inst_t inst,
	struct sii_ipc_message *msg);
enum sii_ipc_api_status sii_host_set_tx_color_space(
	sii_inst_t inst, uint8_t value);
enum sii_ipc_api_status sii_host_set_tx_hdcp_protection(
	sii_inst_t inst, uint8_t value);
enum sii_ipc_api_status sii_host_set_tx_hdcp_content_type(
	sii_inst_t inst, uint8_t value);
enum sii_ipc_api_status sii_host_get_tx_hdcp_ds_version(
	sii_inst_t inst,
	struct sii_ipc_register_reply *reply);
enum sii_ipc_api_status sii_host_get_tx_hdcp_status(
	sii_inst_t inst,
		struct sii_ipc_message *msg);
enum sii_ipc_api_status sii_host_set_tx_audio_insertion(
	sii_inst_t inst,
	struct sii_ipc_message *msg);
enum sii_ipc_api_status sii_host_get_tx_hdcp_rcvid(
	sii_inst_t inst,
	struct sii_ipc_message *msg);
enum sii_ipc_api_status sii_host_get_tx_hdcp_ds_rxid_list(
	sii_inst_t inst,
	struct sii_ipc_message *msg);
enum sii_ipc_api_status sii_host_get_tx_hdcp_ds_repeater_bit(
	sii_inst_t inst,
	struct sii_ipc_register_reply *reply);
enum sii_ipc_api_status sii_host_get_tx_hdcp_ds_topology(
	sii_inst_t inst,
	struct sii_ipc_message *msg);
enum sii_ipc_api_status sii_host_set_tx_hdcp_stream_manage_info(
	sii_inst_t inst,
	struct sii_ipc_message *msg);
#endif /* SII_IPC_HOST */

#endif /* !_IPC_API_H */
