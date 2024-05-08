/*
* SiI8630/SiI8632/SiI9630 Linux Driver
*
* 2016 © Lattice Semiconductor Corporation
*
* This program is free software; you can redistribute it and/or
* modify it only under the terms of  version 2 of the GNU General Public License
* as published by the Free Software Foundation, and not any later version.
*
* This program is distributed ìAS ISÅEWITHOUT ANY WARRANTY of any kind,
* whether express or implied, including without limitation the implied warranty
* of MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE or NON-INFRINGEMENT.
* See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software Foundation, Inc.,
* 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*
* You may contact Lattice Semiconductor Corporation at 111 SW 5th Ave,
* Suite 700, Portland, OR 97204
*/
#ifndef __SPIDERMAN_H__
#define __SPIDERMAN_H__

#include "Sii9630_si_drv_cp8630.h"

#if 1//(CONFIG_SOURCE_ONLY == 0)

#define SII_IPC_HW_LIMITATION

#define EDID_HOST_PRE_SET
#define SII_IPC_HOST

#define sii_hwid_t	uint8_t
#ifdef LINUX_BUILD
#define sii_inst_t	uint8_t
#else
#define sii_inst_t	uint8_t
#endif
#define SII_INST_NULL	(sii_inst_t)(-1)
#define SII_HWID_NULL	(sii_hwid_t)(-1)
#define SII_INST_COUNT_MAX	1
#define SII_DEV_COUNT_MAX	1

enum sii_status {
	SII_STATUS_SUCCESS = 0,
	SII_STATUS_ERR_INVALID_PARAM = -1,
	SII_STATUS_ERR_FAILED = -2,
};

enum sii_ipc_api_status {
	SII_IPC_API_SUCCESS,
	SII_IPC_API_FAILURE,
};

enum sii_ipc_page {
	SII_PAGE_IPC_HOST = 0x1F,
	SII_PAGE_IPC_SLAVE = 0x10
};

enum sii_ipc_reg_host_status {
	SII_REG_IPC_HOST_STATUS1 = 0x80,
#ifndef SII_IPC_HW_LIMITATION
	SII_REG_IPC_HOST_STATUS2 = 0x82,
#else
	SII_REG_IPC_HOST_STATUS2 = 0x3F,
#endif
};

enum sii_ipc_reg_slave_status {
	SII_REG_IPC_SLAVE_STATUS1 = 0x81,
#ifndef SII_IPC_HW_LIMITATION
	SII_REG_IPC_SLAVE_STATUS2 = 0x83,
#else
	SII_REG_IPC_SLAVE_STATUS2 = 0x7F,
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

enum sii_ipc_reg_slave_interrupt_mask_bits {
	SII_BIT_IPC_SLAVE_INTERRUPT = 0x80,
	SII_BIT_IPC_SLAVE_INTERRUPT_MASK = 0x80,

	SII_BIT_IPC_SLAVE_INTERRUPT_SHIFT = 0x07,
	SII_BIT_IPC_SLAVE_INTERRUPT_MASK_SHIFT = 0x07
};

enum sii_ipc_reg_slave_interrupt_mask {
	SII_REG_IPC_SLAVE_INTERRUPT = 0x81,
	SII_REG_IPC_SLAVE_INTERRUPT_MASK = 0x83
};

struct sii_device {
	sii_hwid_t hwid;
	bool isr_enable;
};

/*
   IPC Structure & Context
 */

enum sii_ipc_status {
	SII_IPC_STATUS_SUCCESS = 0,
	SII_IPC_STATUS_FAILURE = -1,
	SII_IPC_STATUS_TIMEOUT = -2,

	SII_IPC_STATUS_ERROR_INVALID_COMMAND = -10,
	SII_IPC_STATUS_ERROR_INVALID_COMMAND_OPCODE = -11,
	SII_IPC_STATUS_ERROR_INVALID_PARAMETER = -12,
	SII_IPC_STATUS_ERROR_INVALID_COMMAND_STORAGE_TYPE = -13,

	SII_IPC_STATUS_ERROR_INVALID_REPLY = -20,
	SII_IPC_STATUS_ERROR_REPLY_FAILURE = -21,
	SII_IPC_STATUS_ERROR_REPLY_NACK = -22,
	SII_IPC_STATUS_ERROR_REPLY_ACK = -23,
	SII_IPC_STATUS_ERROR_REPLY_ERROR_UNKNOWN = -29,

	SII_IPC_STATUS_ERROR_INVALID_NOTIFICATION = -30,
	SII_IPC_STATUS_ERROR_NOTIFICATION_QUEUE_FULL = -31,
	SII_IPC_STATUS_ERROR_NOTIFICATION_QUEUE_EMPTY = -32,

	SII_IPC_STATUS_ERROR_PROTOCOL = -40,
	SII_IPC_STATUS_ERROR_PROTOCOL_VERSION_UNSUPPORTED = -41,
	SII_IPC_STATUS_ERROR_PROTOCOL_NACK = -42,
	SII_IPC_STATUS_ERROR_PROTOCOL_CANNOT_NACK = -43,

	SII_IPC_STATUS_ERROR_MESSAGE = -50,
	SII_IPC_STATUS_ERROR_MESSAGE_CHECKSUM = -51,

	SII_IPC_STATUS_ERROR_PACKET = -60,
	SII_IPC_STATUS_ERROR_PACKET_CHECKSUM = -61,

	SII_IPC_STATUS_ERROR_OSAL = -70,
	SII_IPC_STATUS_ERROR_OSAL_OUT_OF_MEMORY = -71,
	SII_IPC_STATUS_ERROR_OSAL_SEMAPHORE = -72,
	SII_IPC_STATUS_ERROR_OSAL_SEMAPHORE_TIMEOUT = -73,

	SII_IPC_STATUS_ERROR_HAL = -80,
	SII_IPC_STATUS_ERROR_HAL_CHECKSUM = -81,
	SII_IPC_STATUS_ERROR_HAL_EXCEED_BUFFER_LIMIT = -82,
	SII_IPC_STATUS_ERROR_HAL_NO_REPLY_IN_BUFFER = -83,
	SII_IPC_STATUS_ERROR_HAL_NO_NOTIFICATION_IN_BUFFER = -84,
	SII_IPC_STATUS_ERROR_HAL_DEVICE = -89,

	SII_IPC_STATUS_ERROR_UNKNOWN = -99
};



enum sii_os_status {
	SII_OS_STATUS_SUCCESS = 0,
	SII_OS_STATUS_ERR_INVALID_PARAM = -1,
	SII_OS_STATUS_ERR_FAILED = -2,
	SII_OS_STATUS_ERR_NOT_AVAIL = -3,
	SII_OS_STATUS_ERR_SEM_COUNT_EXCEEDED = -4,
	SII_OS_STATUS_QUEUE_FULL = -5,
	SII_OS_STATUS_QUEUE_EMPTY = -6,
	SII_OS_STATUS_TIMEOUT = -7,
	SII_OS_STATUS_ERR_INVALID_OP = -8,
	SII_OS_STATUS_ERR_INVALID_HANDLE = -9,
	SII_OS_STATUS_ERR_IN_USE = -10,
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
	SII_IPC_PHY_CMD_OPCODE_GET_TX_HDCP_DS_FAILURE_REASON,
	/* 14 Opcodes reserved for Command Extension: 0b111_0000 ~ 0b111_1101 */
	SII_IPC_PHY_CMD_OPCODE_GET_HOST_MODE = 0x70,
	SII_IPC_PHY_CMD_OPCODE_SET_HOST_MODE,
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
   IPC Protocol - Hardware, 'HW' for short
 */


#ifndef SII_IPC_HW_LIMITATION
#define SII_IPC_HW_BUFFER_LENGTH		64
#else
#define SII_IPC_HW_BUFFER_LENGTH		63
#endif

/* Define a Reply Buffer with size enough to conatin
   a Reply Payload of 32 bytes */
/* 32 + SII_IPC_MESSAGE_HEADER_LENGTH + SII_IPC_PACKET_HEADER_LENGTH */
#define SII_IPC_HW_BUF_RPL_LENGTH		(32 + 7 + 5)

#define SII_IPC_HW_BUF_NTF_LENGTH		\
	(SII_IPC_HW_BUFFER_LENGTH - SII_IPC_HW_BUF_RPL_LENGTH)

#define SII_IPC_HW_BUF_CMD_LENGTH		SII_IPC_HW_BUF_RPL_LENGTH
#define SII_IPC_HW_BUF_PKT_LENGTH		SII_IPC_HW_BUF_RPL_LENGTH

enum sii_ipc_reg_slave_buf_rpl {
	/* this is length of Slave buffer, not a register */
	SII_IPC_SLAVE_BUF_RPL_LENGTH = SII_IPC_HW_BUF_RPL_LENGTH,

	SII_REG_IPC_SLAVE_BUF_RPL_START = 0x40,
	SII_REG_IPC_SLAVE_BUF_RPL_END = (SII_REG_IPC_SLAVE_BUF_RPL_START \
			+ SII_IPC_SLAVE_BUF_RPL_LENGTH - 1)
};
/*
   IPC Protocol - Abstract, 'ABS' for short
 */

/*
   IPC Protocol - Physical, 'PHY' for short
 */

enum sii_ipc_command_opcode {
	/*
	   5 Opcodes for Protocol Procedures:
	   0b000_0000 ~ 0b000_0100
	 */
	SII_IPC_PHY_CMD_OPCODE_NACK_SLAVE_BUFFER_REPLY = 0x00,
	SII_IPC_PHY_CMD_OPCODE_ACK_SLAVE_BUFFER_REPLY = 0x01,
	SII_IPC_PHY_CMD_OPCODE_NACK_SLAVE_BUFFER_NOTIFICATION = 0x02,
	SII_IPC_PHY_CMD_OPCODE_ACK_SLAVE_BUFFER_NOTIFICATION = 0x03,

	SII_IPC_PHY_CMD_OPCODE_NOTIFICATION_RETRIEVE = 0x04,

	/*
	   11 Opcodes reserved for Protocol Procedures:
	   0b000_0101 ~ 0b000_1111
	 */

	/* 110 Opcodes for Commands: 0b001_0000 ~ 0b111_1110 */

	/*
	   2 Opcodes for Protocol Procedures:
	   0b111_1110 ~ 0b111_1111
	 */
	SII_IPC_PHY_CMD_OPCODE_BUF_CMD_BUF_RPL_INDICATOR = 0x7E,
	SII_IPC_PHY_CMD_OPCODE_BUFFER_COMMAND_INDICATOR = 0x7F
};

enum sii_ipc_phy_reply_status {
	SII_IPC_PHY_REPLY_STATUS_NACK = 0x00,
	SII_IPC_PHY_REPLY_STATUS_ACK = 0x01,
	SII_IPC_PHY_REPLY_STATUS_FAILURE = 0x02,
	SII_IPC_PHY_REPLY_STATUS_SUCCESS = 0x03
};

enum sii_ipc_phy_reply_return_nack {
	SII_IPC_PHY_REPLY_RETURN_UNSUPPORTED = 0x00,
	SII_IPC_PHY_REPLY_RETURN_INVALID_PARAM = 0x01,
	SII_IPC_PHY_REPLY_RETURN_BUSY = 0x02,
	SII_IPC_PHY_REPLY_RETURN_UNKNOWN = 0x03
};

enum sii_ipc_phy_reply_return_ack {
	SII_IPC_PHY_REPLY_RETURN_DEFAULT = 0x00,
};

enum sii_ipc_phy_buffer_status {
	SII_IPC_PHY_BUFFER_STATUS_EMPTY = 0x00,
	SII_IPC_PHY_BUFFER_STATUS_REPLY = 0x01,
	SII_IPC_PHY_BUFFER_STATUS_NOTIFICATION = 0x02,
	SII_IPC_PHY_BUFFER_STATUS_RESERVE = 0x03
};

enum sii_ipc_phy_notification_type {
	SII_IPC_PHY_NOTIFICATION_TYPE_TCMD_DONE = 0x00,

	/* 1 Opcode reserved for Notification Extension: 0b0001 */

	SII_IPC_PHY_NOTIFICATION_TYPE_BUFFER_NOTIFICATION = 0x02,

	SII_IPC_PHY_NOTIFICATION_TYPE_DEBUG = 0x03

	/* 8 Opcodes for Notification: 0b0100 ~ 0b1011 */

	/* 4 Opcodes reserved for Notification Extension: 0b1100 ~ 0b1111 */
};

/*
   IPC Message, 'MSG' for short
 */

#define SII_IPC_MESSAGE_DATA_LENGTH_MAX		172//1024
#define SII_IPC_MESSAGE_HEADER_LENGTH		7
#define SII_IPC_MESSAGE_LENGTH	\
	(SII_IPC_MESSAGE_DATA_LENGTH_MAX + SII_IPC_MESSAGE_HEADER_LENGTH)

#define SII_IPC_MESSAGE_PROTOCOL_VERSION_1	1

enum sii_ipc_message_type {
	SII_IPC_MESSAGE_TYPE_COMMAND = 0x00,
	SII_IPC_MESSAGE_TYPE_REPLY = 0x01,
	SII_IPC_MESSAGE_TYPE_NOTIFICATION = 0x02

	/* 0x03 ~ 0xFF are reserved for extension */
};

struct sii_ipc_message_payload {
	uint8_t message_type;
	uint8_t message_id_h;	/* MSB 8 bits are reserved */
	uint8_t message_id_l;	/* LSB 8 bits support max 128 Commands */
	uint8_t message_data[SII_IPC_MESSAGE_DATA_LENGTH_MAX];
};

struct sii_ipc_message {
	uint8_t protocol_version;
	uint8_t message_length_h;	/* High byte */
	uint8_t message_length_l;	/* Low byte */
	uint8_t message_checksum;
	struct sii_ipc_message_payload message_payload;
};

struct sii_ipc_raw_message {
	uint8_t raw_data[SII_IPC_MESSAGE_LENGTH];
};

/*
   IPC Packet, 'PKT' for short
 */

#define SII_IPC_PACKET_LENGTH			SII_IPC_HW_BUF_PKT_LENGTH
#define SII_IPC_PACKET_HEADER_LENGTH		5
#define SII_IPC_PACKET_PAYLOAD_LENGTH_MAX	 \
	(SII_IPC_PACKET_LENGTH - SII_IPC_PACKET_HEADER_LENGTH)
#define SII_IPC_PACKET_FRAGMENT_COUNT_MAX	\
	((SII_IPC_MESSAGE_DATA_LENGTH_MAX + SII_IPC_PACKET_PAYLOAD_LENGTH_MAX - 1) \
	 / SII_IPC_PACKET_PAYLOAD_LENGTH_MAX)
#define SII_IPC_PACKET_SEQUENCE_NUMBER_MAX	\
	(SII_IPC_PACKET_FRAGMENT_COUNT_MAX - 1)

#define SII_IPC_PACKET_PROTOCOL_VERSION_1	1
#define SII_IPC_PACKET_PAYLOAD_START		5
#define SII_IPC_PACKET_LENGTH_INDEX		3
#define SII_IPC_PACKET_CHECKSUM_INDEX		4

struct sii_ipc_packet {
	uint8_t protocol_version;
	uint8_t sequence_number;
	uint8_t fragment_count;
	uint8_t packet_length;
	uint8_t packet_checksum;
	uint8_t packet_payload[SII_IPC_PACKET_PAYLOAD_LENGTH_MAX];
};

/*
   IPC Packet for Notification, 'NTF' for short
 */

#define SII_IPC_PACKET_NTF_LENGTH		SII_IPC_HW_BUF_NTF_LENGTH
#define SII_IPC_PACKET_NTF_HEADER_LENGTH	3
#define SII_IPC_PACKET_NTF_PAYLOAD_LENGTH_MAX	 \
	(SII_IPC_PACKET_NTF_LENGTH - SII_IPC_PACKET_NTF_HEADER_LENGTH)

#define SII_IPC_PACKET_NTF_PROTOCOL_VERSION_1	1
#define SII_IPC_PACKET_NTF_PAYLOAD_START	3
#define SII_IPC_PACKET_NTF_LENGTH_INDEX		1
#define SII_IPC_PACKET_NTF_CHECKSUM_INDEX	2

struct sii_ipc_packet_ntf {
	uint8_t protocol_version;
	uint8_t packet_length;
	uint8_t packet_checksum;
	uint8_t packet_payload[SII_IPC_PACKET_NTF_PAYLOAD_LENGTH_MAX];
};


/* FD: TBU, check & update these values */
#define SII_IPC_TIMEOUT_HOST_WAIT_MS		(2000)
#define SII_IPC_TIMEOUT_HOST_WAIT_REPLY_MS	(200)
#define SII_IPC_TIMEOUT_HOST_WAIT_NTF_QUEUE_MS	(2000)

#define SII_IPC_NOTIFICATION_QUEUE_LENGTH	16

#define SII_IPC_PACKET_SEQUENCE_NONE		1//-1
#define SII_IPC_PACKET_FRAGMENT_NONE		1//-1

struct sii_ipc_register_command {
	uint8_t opcode;
	uint8_t parameter;
};

struct sii_ipc_packet_command {
	uint8_t opcode;
	uint8_t parameter;
	struct sii_ipc_packet *packet;
};

struct sii_ipc_message_command {
	uint8_t opcode;
	uint8_t parameter;
	struct sii_ipc_message *message;
};

struct sii_ipc_register_reply {
	uint8_t reply_status;
	uint8_t reply_return;
};

struct sii_ipc_buffer_reply {
	uint8_t reply_status;
	uint8_t reply_length;
	struct sii_ipc_packet *packet;
};

struct sii_ipc_packet_reply {
	uint8_t reply_status;
	uint8_t reply_return;
	struct sii_ipc_packet *packet;
};

struct sii_ipc_message_reply {
	uint8_t reply_status;
	uint8_t reply_return;
	struct sii_ipc_message *message;
};

struct sii_ipc_notification {
	uint8_t notification_type;
};

struct sii_ipc_register_notification {
	uint8_t notification_type;
};

struct sii_ipc_packet_notification {
	uint8_t type;
	struct sii_ipc_packet_ntf *packet;
};


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

	struct sii_ipc_notification
		notification_queue[SII_IPC_NOTIFICATION_QUEUE_LENGTH];
	uint8_t notification_queue_head;
	uint8_t notification_queue_tail;
	uint8_t notification_queue_count;

	enum sii_ipc_host_state_machine host_sm;

	uint8_t slave_status1_reg;
	uint8_t slave_status2_reg;
	bool slave_status1_cache;
	bool slave_status2_cache;
};


enum sii_api_status {
	SII_API_SUCCESS = 0,
	SII_API_FAILURE = 1
};

enum sii_boot_status
{
	SII_BOOT_STAT__SUCCESS,
	SII_BOOT_STAT__IN_PROGRESS,
	SII_BOOT_STAT__FAILURE,
};

enum sii_av_link_type
{
	SII_AV_LINK__NONE,
	SII_AV_LINK__HDMI1,
	SII_AV_LINK__HDMI2,
	SII_AV_LINK__DVI,
};
#if 0
enum sii_tmds_pin_swap
{
	SII_TMDS_PIN_SWAP__NONE,
	SII_TMDS_PIN_SWAP__DATA,
	SII_TMDS_PIN_SWAP__FULL,
};
#endif

enum sii_hdcp_status
{
	SII_HDCP_STATUS__OFF,
	SII_HDCP_STATUS__SUCCESS_1X,
	SII_HDCP_STATUS__SUCCESS_22,
	SII_HDCP_STATUS__AUTHENTICATING,
	SII_HDCP_STATUS__FAILED,
};

enum sii_hdcp_version
{
	SII_HDCP_VERSION__NONE,
	SII_HDCP_VERSION__1X,
	SII_HDCP_VERSION__20,
	SII_HDCP_VERSION__22,
	SII_HDCP_VERSION__NOT_SUPPORTED
};

struct sii_hdcp_topology {
	uint32_t seq_num_v;
	bool hdcp1_device_ds;
	bool hdcp2_device_ds;
	bool max_cascade_exceeded;
	bool max_device_exceeded;
	uint8_t device_count;
	uint8_t depth;
};

struct sii_hdcp_stream_manage_info {
	uint32_t seq_num_m;
	uint16_t k;
	uint16_t stream_id_type[1];
};
#if 0
enum sii_flash_status {
	SII_FLASH_STATUS__SUCCESS,
	SII_FLASH_STATUS__IN_PROGRESS,
	SII_FLASH_STATUS__FAILURE,
};
#endif

enum sii_event {
	SII_EVENT__BOOT_DONE,
	SII_EVENT__RX_AV_LINK_CHNG,
	SII_EVENT__AV_MUTE_CHNG,
	SII_EVENT__SCDT_CHNG,
	SII_EVENT__DS_CONNECT_STATUS_CHNG,
	SII_EVENT__DS_EDID_STATUS_CHNG,
	SII_EVENT__RX_HDCP_STATUS_CHNG,
	SII_EVENT__HDR_REC,
	SII_EVENT__VSIF_REC,
	SII_EVENT__RX_VIDEO_RESOLUTION_CHNG,
	SII_EVENT__RX_TMDS_CLOCK_CHNG,
	SII_EVENT__TX_RXSENSE_STATUS_CHNG,
	SII_EVENT__TX_HDCP_STATUS_CHNG,
	SII_EVENT__RX_HDCP_AUTH_START,
	SII_EVENT__RX_HDCP_AUTH_DONE,
	SII_EVENT__RX_HDCP_RCVID_DONE,
	SII_EVENT__RX_HDCP_SMNG_CHNG,
	SII_EVENT__RX_HDCP_AUTH_FAIL,
	SII_EVENT__TX_HDCP_AUTH_DONE,
	SII_EVENT__TX_HDCP_RCVID_CHNG,
	SII_EVENT__TX_HDCP_SMNG_DONE,
	SII_EVENT__TX_HDCP_AUTH_FAIL,
	SII_EVENT__AVI_REC,
	SII_EVENT__AIF_REC,
	SII_EVENT__RX_SCRAMBLE_STATUS_CHNG,
	SII_EVENT__BASE = 32
};

enum sii_infoframe_type {
	SII_INFOFRAME__VSIF,
	SII_INFOFRAME__HDR,
	SII_INFOFRAME__AVI,
	SII_INFOFRAME__AIF
};

enum sii_reg_page {
	SII_CEC_PAGE,
	SII_SCDC_PAGE
};

enum sii_color_space {
	SII_COLOR_SPACE__UNKNOWN,
	SII_COLOR_SPACE__RGB,
	SII_COLOR_SPACE__YCBCR444,
	SII_COLOR_SPACE__YCBCR422,
	SII_COLOR_SPACE__YCBCR420,
};
#if 0

enum sii_hdcp_content_type {
	SII_HDCP_CONTENT_TYPE__TYPE0,
	SII_HDCP_CONTENT_TYPE__TYPE1
};

enum sii_hdcp_fail_reason {
	SII_HDCP_FAIL_REASON__NONE = 0x00,
	SII_HDCP_FAIL_REASON__INTERNAL = 0x01,
	SII_HDCP_FAIL_REASON__DS_EXCEED = 0x02,
	SII_HDCP_FAIL_REASON__DEV_LIMIT = 0x04,
};


enum sii_audio_mode {
	SII_AUDIO__PASSTHROUGH,
	SII_AUDIO__I2S,
	SII_AUDIO__TDM,
	SII_AUDIO__SPDIF,
	SII_AUDIO__NONE
};

struct sii_audio_format {
    uint8_t aifData[10]; //!< Audio Info-Frame data. Refer to CEA861 specification for details.
    uint8_t csData[7];   //!< Channel Status data. Refer to IEC 60958-3 specification for details.
    uint8_t audioMode;
};

enum sii_audio_mclk_ratio {
	SII_AUDIO_MCLK_RATIO__128X,
	SII_AUDIO_MCLK_RATIO__256X,
	SII_AUDIO_MCLK_RATIO__384X,
	SII_AUDIO_MCLK_RATIO__512X
};

enum sii_scramble_state {
	SII_SCRAMBLE_OFF,
	SII_SCRAMBLE_ON_ABOVE_340M,
	SII_SCRAMBLE_ON_BELOW_340M
};

struct sii_tmds_lock_status {
	bool clock_detected;
	bool ch0_locked;
	bool ch1_locked;
	bool ch2_locked;
};

struct sii_char_error_count {
	bool	ch0_valid;
	bool	ch1_valid;
	bool	ch2_valid;
	uint8_t	checksum;
	uint16_t ch0_char_error_count;
	uint16_t ch1_char_error_count;
	uint16_t ch2_char_error_count;
};

struct sii_device_id_string {
	uint8_t str[8];
};

struct sii_audio_info {
	uint32_t n;
	uint32_t cts;
	bool layout1;
	bool dsd;
	bool hbra;
	uint8_t ch_stat[5];
};

enum sii_tmds_clock_ratio {
	SII_TMDS_CLK_RATIO__1_10,
	SII_TMDS_CLK_RATIO__1_40
};
#endif
struct sii_video_info {
	uint16_t h_res_total;
	uint16_t v_res_total;
	uint16_t h_res_active;
	uint16_t v_res_active;
	uint16_t tmds_clock;
	bool interlace;
	uint8_t color_depth;
	uint8_t color_space;
	uint8_t ratio;
};
#if 0
struct sii_tmds_swing_level {
	uint8_t clk_level;
	uint8_t data0_level;
	uint8_t data1_level;
	uint8_t data2_level;
};
#endif

enum sii_product_id {
	SII_PRODUCT_ID__NONE,
	SII_PRODUCT_ID__RX,
	SII_PRODUCT_ID__TX,
	SII_PRODUCT_ID__RPT_SINGLE,
};
#if 0

enum sii_eq_source {
	SII_EQ_SRC__HDMI1,
	SII_EQ_SRC__HDMI2,
};

struct sii_eq_setting {
	uint8_t source;
	uint8_t main_eq[8];
	uint8_t extend_eq[6];
};
#endif
#define SII_EDID_BLOCK_SIZE					128
#define SII_RX_INFOFRAME_MAX_LENGTH			32

enum sii_ipc_reg_host_buffer {
	/* this is length of Host buffer, not a register */
	SII_IPC_HOST_BUFFER_LENGTH = SII_IPC_HW_BUFFER_LENGTH,

	SII_REG_IPC_HOST_BUFFER_START = 0x00,
	SII_REG_IPC_HOST_BUFFER_END = (SII_REG_IPC_HOST_BUFFER_START \
			+ SII_IPC_HOST_BUFFER_LENGTH - 1)
};

enum sii_ipc_reg_host_interrupt_mask {
	SII_REG_IPC_HOST_INTERRUPT = 0x80,
	SII_REG_IPC_HOST_INTERRUPT_MASK = 0x82
};

enum sii_ipc_reg_host_interrupt_mask_bits {
	SII_BIT_IPC_HOST_INTERRUPT = 0x80,
	SII_BIT_IPC_HOST_INTERRUPT_MASK = 0x80
};

enum sii_ipc_reg_slave_buf_ntf {
	/* this is length of Slave buffer, not a register */
	SII_IPC_SLAVE_BUF_NTF_LENGTH = SII_IPC_HW_BUF_NTF_LENGTH,

	SII_REG_IPC_SLAVE_BUF_NTF_START = SII_REG_IPC_SLAVE_BUF_RPL_START \
			+ SII_IPC_SLAVE_BUF_RPL_LENGTH,
	SII_REG_IPC_SLAVE_BUF_NTF_END = (SII_REG_IPC_SLAVE_BUF_NTF_START \
			+ SII_IPC_SLAVE_BUF_NTF_LENGTH - 1)
};

enum sii_ipc_reg_host_buf_cmd {
	/* this is length of Host buffer, not a register */
	SII_IPC_HOST_BUF_CMD_LENGTH = SII_IPC_HW_BUF_CMD_LENGTH,

	SII_REG_IPC_HOST_BUF_CMD_START = 0x00,
	SII_REG_IPC_HOST_BUF_CMD_END = (SII_REG_IPC_HOST_BUF_CMD_START \
			+ SII_IPC_HOST_BUF_CMD_LENGTH - 1)
};

#define IEEE_REG_ID_HDMI1_4		0x000C03
#define IEEE_REG_ID_HDMI2_0		0xC45DD8
#define IEEE_REG_ID_INDEX		4

#define HDCP_RXID_LENGTH          5
#define MAX_NUM_PER_LOOP          2

#define HDCP1_MAX_DEPTH           7
#define HDCP1_MAX_DEVICE_COUNT    32
#define HDCP1_RXID_LIST_MAX       (HDCP1_MAX_DEVICE_COUNT + 1)

#define HDCP2_MAX_DEPTH           4
#define HDCP2_MAX_DEVICE_COUNT    31
#define HDCP2_RXID_LIST_MAX       (HDCP2_MAX_DEVICE_COUNT + 1)

#define EDID_HOST_PRE_SET

/**
* @brief HDCP Downstream Repeater RxInfo
*/
typedef struct
{
    bool  bHdcp1DeviceDs;         //!< HDCP 1.X compliant device in the topology if set to true, HDCP 2.X use only
    bool  bHdcp20DeviceDs;        //!< HDCP 2.0 compliant device in the topology if set to true, HDCP 2.X use only
    bool  bMaxCascadeExceeded;    //!< More than seven level for HDCP 1.X or four levels for HDCP 2.X of repeaters cascaded together if set to true
    bool  bMaxDevsExceeded;       //!< More than 31 devices (for HDCP1.X) or 127 devices (for HDCP 2.X) as attached if set to true
    uint8_t deviceCount;            //!< Total number of attached downstream devices
    uint8_t depth;                  //!< Repeater cascade depth
    uint32_t seqNumV;              //!< seq_num_V value, HDCP 2.X use only
} SiiDrvAdaptHdcpTopology_t;

#if 0
typedef struct
{
    uint8_t id[HDCP_RXID_LENGTH];
} HdcpRxIdList_t;

#endif

struct sii_hdcp_ksv
{
	uint8_t key[5];
};

#if 0
/* KSV data object */
struct sii_ksv_list {
	uint16_t length;
	uint8_t *p_list;
	uint8_t *p_list_start;
	uint8_t last_byte;
};
#endif

struct sii_rx_hdcp1_data
{
 	bool spiderman_rdy;
	bool auth_req_rcvd_frm_spiderman;
	bool hdcp_data_rdy;
	bool hdcp_data_sent;
	struct sii_hdcp_ksv *ksv_list;
	//struct hdcp_obj *p_hdcp_obj;
	struct sii_hdcp_topology topology_us;
	uint32_t ds_hdcp_start_time;
	enum sii_hdcp_version us_hdcp_ver;
	bool rx_hdcp_stream_mang_rcvd;
};

struct sii_rx_hdcp2_data
{
 	struct sii_hdcp_topology topology_us;
	struct sii_hdcp_stream_manage_info info_us;
};

struct sii_tx_hdcp2_data
{
    uint8_t bIsDsRepeater;
    uint8_t bHdcp2_CertRx_done;
    uint8_t bForceStreamManage;
    SiiDrvAdaptHdcpTopology_t RxTopologyDs;
    struct sii_drv_tx_strmng_2x_msg StreamManageInfoDs;
};

struct sii9396_platform_data {
	//struct i2c_adapter *i2c_bus;
	//struct i2c_client *i2c_dev_client;
	uint8_t i2c_device_id;
    uint8_t rx_inst;
	//uint32_t i2c_adapter_num;

};

extern struct sii9396_platform_data *ex_sp_pdata;

int spiderman_init(void);

enum sii_ipc_status sii_ipc_host_handler(sii_inst_t inst) ;

uint8_t sii_handle(sii_inst_t inst, void *p_obj) ;

uint32_t sii_tx_hdcp_event_handler(uint32_t event_flags);

void sii_drv_spiderman_init(pstdriver_obj_t drv_obj);
enum sii_ipc_status sii_ipc_check_notify(sii_inst_t inst, uint8_t *register2);

void set_need_to_send_strm_mng_msg_flag(void);


void sii_ipc_init(sii_inst_t inst);

enum sii_ipc_api_status sii_host_get_firmware_version(
        sii_inst_t inst,
        struct sii_ipc_message *msg);

enum sii_ipc_api_status sii_host_set_tx_audio_insertion(
	sii_inst_t inst,
	struct sii_ipc_message *msg);

enum sii_ipc_api_status sii_host_set_rx_audio_mclk_ratio(
	sii_inst_t inst, uint8_t value);

enum sii_ipc_api_status sii_host_set_rx_audio_extraction(
		sii_inst_t inst, uint8_t value);

enum sii_api_status sii_get_cec_reg(
	sii_inst_t inst, uint8_t offset, uint8_t length, uint8_t *value);

enum sii_api_status sii_set_cec_reg(
	sii_inst_t inst, uint8_t offset, uint8_t length, uint8_t *value);

enum sii_api_status sii_get_rx_hdcp_stream_manage_info(
        sii_inst_t inst,
        struct sii_hdcp_stream_manage_info *info);

enum sii_ipc_status sii_ipc_hal_host_init(sii_inst_t inst);

enum sii_ipc_status sii_ipc_hal_set_slave_interrupt_mask(
        sii_inst_t inst, bool interrupt_mask);

enum sii_ipc_api_status sii_get_boot_status(
	sii_inst_t inst, enum sii_boot_status *status);

enum sii_ipc_api_status sii_host_set_host_mode(
	sii_inst_t inst, uint8_t value);

enum sii_ipc_api_status sii_host_set_rx_hdcp_repeater_mode(
	sii_inst_t inst, uint8_t value);

enum sii_ipc_api_status sii_get_rx_hdcp_us_version(
	sii_inst_t inst,
	enum sii_hdcp_version *version);

enum sii_ipc_api_status sii_host_set_rx_hdcp_ddc(
	sii_inst_t inst, uint8_t value);

enum sii_api_status sii_get_tx_hdcp_ds_version(
        sii_inst_t inst,
        enum sii_hdcp_version *version);

bool Sii9396RxHDCPGet(sii_inst_t inst);

#endif // (CONFIG_SOURCE_ONLY == 0)

#endif // __SPIDERMAN_H__
