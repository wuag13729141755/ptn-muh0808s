/*
  2016 (C) Lattice Semiconductor Corporation

  No part of this work may be reproduced, modified, distributed, transmitted,
  transcribed, or translated into any language or computer format, in any form
  or by any means without written permission of Lattice Semiconductor, Inc.,
  111 SW 5th Ave, Suite 700, Portland, OR 97204.

*/

#ifndef _IPC_H
#define _IPC_H

#include "Sii9396_sii_common.h"


/*
   IPC Protocol - Hardware, 'HW' for short
 */

#define SII_IPC_HW_LIMITATION

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

#define SII_IPC_MESSAGE_DATA_LENGTH_MAX		1024
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

/* FD: TBU, check & update these values */
#define SII_IPC_TIMEOUT_HOST_WAIT_MS		(2000)
#define SII_IPC_TIMEOUT_HOST_WAIT_REPLY_MS	(2000)
#define SII_IPC_TIMEOUT_HOST_WAIT_NTF_QUEUE_MS	(2000)

#define SII_IPC_NOTIFICATION_QUEUE_LENGTH	16

#define SII_IPC_PACKET_SEQUENCE_NONE		-1
#define SII_IPC_PACKET_FRAGMENT_NONE		-1

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
   IPC External API
 */

uint8_t calculate_checksum_2bit(uint8_t value1, uint8_t value2);
bool verify_checksum_2bit(uint8_t value1, uint8_t value2);
uint8_t calculate_checksum_byte(uint8_t *bytes, uint8_t length);
bool verify_checksum_byte(uint8_t *bytes, uint8_t length);

void sii_ipc_display_error(enum sii_ipc_status status);

#endif /* !_IPC_H */
