/*
  2016 (C) Lattice Semiconductor Corporation

  No part of this work may be reproduced, modified, distributed, transmitted,
  transcribed, or translated into any language or computer format, in any form
  or by any means without written permission of Lattice Semiconductor, Inc.,
  111 SW 5th Ave, Suite 700, Portland, OR 97204.

*/

#ifndef _OSAL_H
#define _OSAL_H

#include "Sii9396_sii_common.h"

#ifdef SII_IPC_HOST
#if 0 //def __KERNEL__
#include "osal_linux_kernel.h"
#define SII_OS_OPERATION_API
#elif 0//defined(WIN32)
#include "osal_win32.h"
#define SII_OS_OPERATION_API
#else
#include "Sii9396_osal_win32.h"
#endif
#endif /* SII_IPC_HOST */

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

#define INT16_T_MAX	(int16_t)((1UL << ((sizeof(int16_t) << 3) - 1)) - 1)
#define INT32_T_MAX	(int32_t)((1UL << ((sizeof(int32_t) << 3) - 1)) - 1)
#define INT64_T_MAX	(int64_t)((1ULL << ((sizeof(int64_t) << 3) - 1)) - 1)

#define SII_OS_INFINITE_WAIT	-1		/* INFINITE in Win32 */
#define SII_OS_NO_WAIT		0
#define SII_OS_STACK_SIZE_AUTO	-1
#define SII_OS_STATUS_NULL	NULL

struct SiiOsDebugChannel {
	uint32_t channel;
	char *channelName;
};

#define SII_OS_DEBUG_FORMAT_SIMPLE	0x0000u
#define SII_OS_DEBUG_FORMAT_FILEINFO	0x0001u
#define SII_OS_DEBUG_FORMAT_CHANNEL	0x0002u
#define SII_OS_DEBUG_FORMAT_TIMESTAMP	0x0004u

enum sii_os_status sii_os_init(uint32_t maxChannels);
enum sii_os_status sii_os_term(void);

void *sii_os_alloc(const char *pName, uint32_t size, uint32_t flags);
void *sii_os_calloc(const char *pName, uint32_t size, uint32_t flags);
void *sii_os_realloc(const char *pName, void *pAddr, uint32_t size,
			uint32_t flags);
void sii_os_free(void *pAddr);
void sii_os_sleep_msec(uint32_t timeMsec);
void sii_os_sleep_usec(uint32_t timeUsec);

//#ifdef SII_OS_OPERATION_API
enum sii_os_status sii_os_semaphore_create(const char *pName, uint32_t maxCount,
					uint32_t initialValue,
					struct SiiOsSemaphore **pRetSemId);
enum sii_os_status sii_os_semaphore_delete(struct SiiOsSemaphore *semId);
enum sii_os_status sii_os_semaphore_give(struct SiiOsSemaphore *semId);
enum sii_os_status sii_os_semaphore_take(struct SiiOsSemaphore *semId,
					int32_t timeMsec);
uint32_t sii_os_get_current_time_second(void);
bool sii_os_delay_second(uint32_t base, uint32_t delay);
int sii_os_file_open(const char* path, int flags, int rights);
int sii_os_file_close(int fd);
int sii_os_file_read(int fd, long offset,
	unsigned int size, unsigned char* data);
int sii_os_get_file_length(int fd);
//#endif /* SII_OS_OPERATION_API */
#endif /* !_OSAL_H */
