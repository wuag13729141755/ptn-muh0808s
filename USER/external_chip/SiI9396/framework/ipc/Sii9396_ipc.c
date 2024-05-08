/*
  2016 (C) Lattice Semiconductor Corporation

  No part of this work may be reproduced, modified, distributed, transmitted,
  transcribed, or translated into any language or computer format, in any form
  or by any means without written permission of Lattice Semiconductor, Inc.,
  111 SW 5th Ave, Suite 700, Portland, OR 97204.

*/

#include "Sii9396_osal.h"
#include "Sii9396_ipc.h"

#ifdef SII_IPC_HOST
#include "Sii9396_ipc_host.h"
#include "Sii9396_ipc_hal_host.h"
#endif /* SII_IPC_HOST */

#ifdef SII_IPC_SLAVE
#include "Sii9396_ipc_slave.h"
#include "Sii9396_ipc_hal_slave.h"
#include "Sii9396_si_debug_log.h"
#endif /* SII_IPC_SLAVE */


/*
   IPC Structure & Context
 */

#ifdef SII_IPC_SLAVE
extern struct sii_ipc_slave_context ipc_slave_context_data;
extern struct sii_ipc_slave_context *ipc_slave_context;
#endif /* SII_IPC_SLAVE */

/*
   IPC External API
 */

uint8_t calculate_checksum_2bit(uint8_t value1, uint8_t value2) {
	uint8_t checksum = 0x00;

	checksum = (value1 & 0x03)
		+ ((value1 >> 2) & 0x03)
		+ ((value1 >> 4) & 0x03)
		+ ((value1 >> 6) & 0x03)
			   + (value2 & 0x03)
		+ ((value2 >> 2) & 0x03)
		+ ((value2 >> 4) & 0x03)
		+ ((value2 >> 6) & 0x03);
	return (0 - checksum) & 0x03;
}

bool verify_checksum_2bit(uint8_t value1, uint8_t value2) {
	uint8_t checksum = 0x00;

	checksum = (value1 & 0x03)
		+ ((value1 >> 2) & 0x03)
		+ ((value1 >> 4) & 0x03)
		+ ((value1 >> 6) & 0x03)
			   + (value2 & 0x03)
		+ ((value2 >> 2) & 0x03)
		+ ((value2 >> 4) & 0x03)
		+ ((value2 >> 6) & 0x03);
	return (0 == (checksum & 0x03))? true : false;
}

uint8_t calculate_generic_checksum(
	uint8_t *bytes, uint8_t checksum, uint8_t length) {
	uint8_t i;

	for (i = 0; i < length; i++)
		checksum += bytes[i];

	checksum = 0x100 - checksum;

	return checksum;
}

uint8_t calculate_checksum_byte(uint8_t *bytes, uint8_t length) {
	return calculate_generic_checksum(bytes, 0, length);
}

bool verify_checksum_byte(uint8_t *bytes, uint8_t length) {
	return (0 == calculate_generic_checksum(bytes, 0, length))
		? true : false;
}

void sii_ipc_display_error(enum sii_ipc_status status) {

	if (status <= SII_IPC_STATUS_ERROR_UNKNOWN) {
/*
	SII_IPC_STATUS_ERROR_UNKNOWN = -99
*/
#ifdef SII_IPC_HOST
		err("UNKNOWN error found on Host side\n");
#endif /* SII_IPC_HOST */
#ifdef SII_IPC_SLAVE
	DBG_LOG0(DBG_ERR, 0, "UNKNOWN error found on Slave side\n");
#endif /* SII_IPC_SLAVE */
	} else if (status <= SII_IPC_STATUS_ERROR_HAL) {
/*
	SII_IPC_STATUS_ERROR_HAL = -80,
	SII_IPC_STATUS_ERROR_HAL_CHECKSUM = -81,
	SII_IPC_STATUS_ERROR_HAL_EXCEED_BUFFER_LIMIT = -82,
	SII_IPC_STATUS_ERROR_HAL_NO_REPLY_IN_BUFFER = -83,
	SII_IPC_STATUS_ERROR_HAL_DEVICE = -89,
*/
#ifdef SII_IPC_HOST
		err("HAL error found on Host side\n");
#endif /* SII_IPC_HOST */
#ifdef SII_IPC_SLAVE
	DBG_LOG0(DBG_ERR, 0, "HAL error found on Slave side\n");
#endif /* SII_IPC_SLAVE */
	} else if (status <= SII_IPC_STATUS_ERROR_OSAL) {
/*
	SII_IPC_STATUS_ERROR_OSAL = -70,
	SII_IPC_STATUS_ERROR_OSAL_OUT_OF_MEMORY = -71,
	SII_IPC_STATUS_ERROR_OSAL_SEMAPHORE = -72,
	SII_IPC_STATUS_ERROR_OSAL_SEMAPHORE_TIMEOUT = -73,
*/
#ifdef SII_IPC_HOST
		err("OSAL error found on Host side\n");
#endif /* SII_IPC_HOST */
#ifdef SII_IPC_SLAVE
	DBG_LOG0(DBG_ERR, 0, "OSAL error found on Slave side\n");
#endif /* SII_IPC_SLAVE */
	} else if (status <= SII_IPC_STATUS_ERROR_PACKET) {
/*
	SII_IPC_STATUS_ERROR_PACKET = -60,
	SII_IPC_STATUS_ERROR_PACKET_CHECKSUM = -61,
*/
#ifdef SII_IPC_HOST
		err("PACKET error found on Host side\n");
#endif /* SII_IPC_HOST */
#ifdef SII_IPC_SLAVE
	DBG_LOG0(DBG_ERR, 0, "PACKET error found on Slave side\n");
#endif /* SII_IPC_SLAVE */
	} else if (status <= SII_IPC_STATUS_ERROR_MESSAGE) {
/*
	SII_IPC_STATUS_ERROR_MESSAGE = -50,
	SII_IPC_STATUS_ERROR_MESSAGE_CHECKSUM = -51,
*/
#ifdef SII_IPC_HOST
		err("MESSAGE error found on Host side\n");
#endif /* SII_IPC_HOST */
#ifdef SII_IPC_SLAVE
	DBG_LOG0(DBG_ERR, 0, "MESSAGE error found on Slave side\n");
#endif /* SII_IPC_SLAVE */
	} else if (status <= SII_IPC_STATUS_ERROR_PROTOCOL) {
/*
	SII_IPC_STATUS_ERROR_PROTOCOL = -40,
*/
#ifdef SII_IPC_HOST
		//err("PROTOCOL error found on Host side\n");
#endif /* SII_IPC_HOST */

#ifdef SII_IPC_SLAVE
	DBG_LOG0(DBG_ERR, 0, "PROTOCOL error found on Slave side\n");
#endif /* SII_IPC_SLAVE */

	} else if (status <= SII_IPC_STATUS_ERROR_INVALID_NOTIFICATION) {
/*
	SII_IPC_STATUS_ERROR_INVALID_NOTIFICATION = -30,
	SII_IPC_STATUS_ERROR_NOTIFICATION_QUEUE_FULL = -31,
	SII_IPC_STATUS_ERROR_NOTIFICATION_QUEUE_EMPTY = -32,
*/
#ifdef SII_IPC_HOST
		err("NOTIFICATION error found on Host side\n");
#endif /* SII_IPC_HOST */
#ifdef SII_IPC_SLAVE
	DBG_LOG0(DBG_ERR, 0, "NOTIFICATION error found on Slave side\n");
#endif /* SII_IPC_SLAVE */
	} else if (status <= SII_IPC_STATUS_ERROR_INVALID_REPLY) {
/*
	SII_IPC_STATUS_ERROR_INVALID_REPLY = -20,
*/
#ifdef SII_IPC_HOST
		err("REPLY error found on Host side\n");
#endif /* SII_IPC_HOST */
#ifdef SII_IPC_SLAVE
	DBG_LOG0(DBG_ERR, 0, "REPLY error found on Slave side\n");
#endif /* SII_IPC_SLAVE */
	} else if (status <= SII_IPC_STATUS_ERROR_INVALID_COMMAND) {
/*
	SII_IPC_STATUS_ERROR_INVALID_COMMAND = -10,
	SII_IPC_STATUS_ERROR_INVALID_PARAMETER = -11,
*/
#ifdef SII_IPC_HOST
		err("COMMAND error found on Host side\n");
#endif /* SII_IPC_HOST */
#ifdef SII_IPC_SLAVE
	DBG_LOG0(DBG_ERR, 0, "COMMAND error found on Slave side\n");
#endif /* SII_IPC_SLAVE */
	} else if (status < SII_IPC_STATUS_SUCCESS) {
/*
	SII_IPC_STATUS_SUCCESS = 0,
	SII_IPC_STATUS_FAILURE = -1,
	SII_IPC_STATUS_TIMEOUT = -2,
*/
#ifdef SII_IPC_HOST
		err("TOP LEVEL error found on Host side\n");
#endif /* SII_IPC_HOST */
#ifdef SII_IPC_SLAVE
	DBG_LOG0(DBG_ERR, 0, "TOP LEVEL error found on Slave side\n");
#endif /* SII_IPC_SLAVE */
	}
}
