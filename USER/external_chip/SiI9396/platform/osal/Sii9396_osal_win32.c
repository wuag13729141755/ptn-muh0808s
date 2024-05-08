/*
  2016 (C) Lattice Semiconductor Corporation

  No part of this work may be reproduced, modified, distributed, transmitted,
  transcribed, or translated into any language or computer format, in any form
  or by any means without written permission of Lattice Semiconductor, Inc.,
  111 SW 5th Ave, Suite 700, Portland, OR 97204.

*/

#include "Sii9396_osal.h"
#include <stdlib.h>
//#include <time.h>

int debug_level = DEBUG_LEVEL_DEFAULT;

#ifdef SII_OS_OPERATION_API

CRITICAL_SECTION sii_section;
/*
 * Initialize the OSAL and allocate required resources.
 *
 * Parameters:
 *  maxChannels: Maximum number of debug channels to support.
 *
 * Return value:
 *  SII_OS_STATUS_SUCCESS on success
 *  SII_OS_STATUS_ERR_INVALID_PARAM if maxChannels == 0
 */
enum sii_os_status sii_os_init(uint32_t maxChannels)
{
	enum sii_os_status retval = SII_OS_STATUS_SUCCESS;

	if (0 == maxChannels) {
		err("Invalid parameter\n");
		retval = SII_OS_STATUS_ERR_INVALID_PARAM;
		goto done;
	}
	InitializeCriticalSection(&sii_section);
done:
	return retval;
}

/*
 * Terminate the OSAL and deallocate all associated resources.
 *
 * Return value:
 *  SII_OS_STATUS_SUCCESS on success
 *  SII_OS_STATUS_ERR_FAILED if any error occurs
 */
enum sii_os_status sii_os_term(void)
{
	enum sii_os_status retval = SII_OS_STATUS_SUCCESS;

	return retval;
}

/*
 * Create a binary or counting semaphore.
 *
 * Parameters:
 *  pName: Semaphore identifier (16 chars max).
 *  maxCount: Max semaphore count (1 for binary semaphore).
 *  initialValue: Initial count on semaphore (0 for semaphore unavailable).
 *  pRetSemId: Returned semaphore object identifier or NULL on failure.
 *
 * Return value:
 *  SII_OS_STATUS_SUCCESS on success
 *  SII_OS_STATUS_ERR_INVALID_PARAM if any parameter is invalid
 *  SII_OS_STATUS_ERR_NOT_AVAIL if insufficient memory
 *  SII_OS_STATUS_ERR_FAILED for all other errors
 */
enum sii_os_status sii_os_semaphore_create(const char *pName, uint32_t maxCount,
					uint32_t initialValue,
					struct SiiOsSemaphore **pRetSemId)
{
	enum sii_os_status retval = SII_OS_STATUS_SUCCESS;
	struct SiiOsSemaphore *semaphoreData = NULL;

	dbg("%s\n", (NULL != pName) ? pName : "");

	if (NULL == pRetSemId) {
		err("Invalid parameter\n");
		retval = SII_OS_STATUS_ERR_INVALID_PARAM;
		goto done;
	}
	*pRetSemId = NULL;

	if (maxCount < initialValue) {
		err("Invalid parameter\n");
		retval = SII_OS_STATUS_ERR_INVALID_PARAM;
		goto done;
	}

	semaphoreData = sii_os_calloc(pName, sizeof(struct SiiOsSemaphore), 0);
	if (NULL == semaphoreData) {
		retval = SII_OS_STATUS_ERR_NOT_AVAIL;
		goto done;
	}

	semaphoreData->semaphore = CreateSemaphore(
		NULL,		   // default security attributes
		initialValue,   // initial count
		maxCount,	   // maximum count
		pName);		 // semaphore name

	if (NULL == semaphoreData->semaphore) {
		retval = SII_OS_STATUS_ERR_NOT_AVAIL;
		goto done;
	}

	*pRetSemId = semaphoreData;

done:
	if (SII_OS_STATUS_SUCCESS != retval)
		sii_os_free(semaphoreData);
	return retval;
}

/*
 * Delete a semaphore.
 *
 * Parameters:
 *  semId: Semaphore object identifier.
 *
 * Return value:
 *  SII_OS_STATUS_SUCCESS on success
 *  SII_OS_STATUS_ERR_INVALID_PARAM if any parameter is invalid
 */
enum sii_os_status sii_os_semaphore_delete(struct SiiOsSemaphore *semId)
{
	enum sii_os_status retval = SII_OS_STATUS_SUCCESS;

	if (NULL == semId) {
		err("Invalid parameter\n");
		retval = SII_OS_STATUS_ERR_INVALID_PARAM;
		goto done;
	}

	CloseHandle(semId->semaphore);

	sii_os_free(semId);

done:
	return retval;
}

/*
 * Release a semaphore.
 *
 * Parameters:
 *  semId: Semaphore object identifier.
 *
 * Return value:
 *  SII_OS_STATUS_SUCCESS on success
 *  SII_OS_STATUS_ERR_INVALID_PARAM if any parameter is invalid
 *  SII_OS_STATUS_ERR_SEM_COUNT_EXCEEEDED if too many releases occurred
 *  SII_OS_STATUS_ERR_FAILED for all other errors
 */
enum sii_os_status sii_os_semaphore_give(struct SiiOsSemaphore *semId)
{
	enum sii_os_status retval = SII_OS_STATUS_SUCCESS;

	if (NULL == semId) {
		err("Invalid parameter\n");
		retval = SII_OS_STATUS_ERR_INVALID_PARAM;
		goto done;
	}

	if (!ReleaseSemaphore(semId->semaphore, // handle to semaphore
						  1,				// increase count by one
						  NULL			  // not interested in previous count
						  )) {
		retval = SII_OS_STATUS_ERR_FAILED;
	}

done:
	return retval;
}

/*
 * Take a semaphore with or without a timeout.
 *
 * Parameters:
 *  semId: Semaphore object identifier.
 *  timeMsec: Timeout period to wait for semaphore in milliseconds.
 *	Possible values:
 *	  SII_OS_INFINITE_WAIT to block waiting for the semaphore.
 *	  SII_OS_NO_WAIT to return immediately if semaphore is not available.
 *	  1 -> 2147483647 (0x7fffffff) for a finite wait for the semaphore.
 *
 * Return value:
 *  SII_OS_STATUS_SUCCESS on success
 *  SII_OS_STATUS_ERR_INVALID_PARAM if any parameter is invalid
 *  SII_OS_STATUS_TIMEOUT on a timeout
 *  SII_OS_STATUS_ERR_FAILED for all other errors
 */
enum sii_os_status sii_os_semaphore_take(struct SiiOsSemaphore *semId,
					int32_t timeMsec)
{
	enum sii_os_status retval = SII_OS_STATUS_SUCCESS;
	int down_retval;

	if (NULL == semId) {
		err("Invalid parameter\n");
		retval = SII_OS_STATUS_ERR_INVALID_PARAM;
		goto done;
	}

	down_retval = WaitForSingleObject(
									semId->semaphore,   // handle to semaphore
									timeMsec);		  // time-out interval

	if (down_retval) {
		if (WAIT_TIMEOUT == down_retval)
			retval = SII_OS_STATUS_TIMEOUT;
		else
			retval = SII_OS_STATUS_ERR_FAILED;
	}

done:
	return retval;
}

/*
 * Allocate OS memory.
 *
 * Parameters:
 *  pName: name of the block.
 *  size: size in bytes.
 *  flags: OS supported flags (default=0). Currently, flags is ignored.
 *
 * Return value:
 *  Pointer to allocated memory or SII_OS_STATUS_NULL
 */
void *sii_os_alloc(const char *pName, uint32_t size, uint32_t flags)
{
	void *ptr;
	EnterCriticalSection(&sii_section);
	ptr = malloc((size_t)size);
	LeaveCriticalSection(&sii_section);
	return ptr;
}

/*
 * Allocate OS memory and initialize the memory to 0.
 *
 * Parameters:
 *  pName: name of the block.
 *  size: size in bytes.
 *  flags: OS supported flags (default=0). Currently, flags is ignored.
 *
 * Return value:
 *  Pointer to allocated memory or SII_OS_STATUS_NULL
 */
void *sii_os_calloc(const char *pName, uint32_t size, uint32_t flags)
{
	void *ptr;
	EnterCriticalSection(&sii_section);
	ptr = calloc((size_t)size, sizeof(uint8_t));
	LeaveCriticalSection(&sii_section);
	return ptr;
}

/*
 * Reallocate OS memory. The contents of the block are preserved up to the
 * lesser of the new and old sizes. If new size is smaller than old size,
 * an existing string will be truncated and not be NULL-terminated.
 *
 * Parameters:
 *  pName: name of the block.
 *  pAddr: address of previously allocated block.
 *  size: new size in bytes.
 *  flags: OS supported flags (default=0). Currently, flags is ignored.
 *
 * Return value:
 *  Pointer to allocated memory or SII_OS_STATUS_NULL
 */
void *
sii_os_realloc(const char *pName, void *pAddr, uint32_t size, uint32_t flags)
{
	void *ptr;
	EnterCriticalSection(&sii_section);
	ptr = realloc(pAddr, (size_t)size);
	LeaveCriticalSection(&sii_section);
	return ptr;
}

/*
 * Free OS memory.
 *
 * Parameters:
 *  pAddr: address of memory allocated with SiiOsAlloc or SiiOsCalloc.
 *	  If pAddr==NULL, then nothing happens.
 */
void sii_os_free(void *pAddr)
{
	if (NULL != pAddr) {
		EnterCriticalSection(&sii_section);
		free(pAddr);
		LeaveCriticalSection(&sii_section);
	}
}

void sii_os_sleep_msec(uint32_t timeMsec)
{
	Sleep(timeMsec);
}

void sii_os_sleep_usec(uint32_t timeUsec)
{
	static LARGE_INTEGER frequence = {0};
	LARGE_INTEGER interval, privious, current;
	if (frequence.QuadPart == 0)
		QueryPerformanceFrequency(&frequence);
	interval.QuadPart = frequence.QuadPart * timeUsec /1000000;
	QueryPerformanceCounter(&privious);
	current = privious;
	while(current.QuadPart - privious.QuadPart < interval.QuadPart)
		QueryPerformanceCounter(&current);
}

uint32_t sii_os_get_current_time_second(void)
{
	return (uint32_t)time(NULL);
}

bool sii_os_delay_second(uint32_t base, uint32_t delay)
{
	uint32_t time_diff = sii_os_get_current_time_second() - base;
	if (time_diff < delay) {
		return false;
	}
	return true;
}

int sii_os_file_open(const char* path, int flags, int rights)
{
	const char *file_flag_tab[] = {"rb","wb"};
	FILE* filp;
	filp = fopen(path, file_flag_tab[flags]);
	if (NULL == filp)
		return -1;
	return (int)filp;
}

int sii_os_file_close(int fd)
{
	return fclose((FILE*)fd);
}

int sii_os_file_read(int fd, long offset,
	unsigned int size, unsigned char* data)
{
	if (fseek((FILE*)fd, offset, SEEK_SET))
		return -1;
	return fread(data, sizeof(unsigned char), size, (FILE*)fd);
}

int sii_os_get_file_length(int fd)
{
	fpos_t pos;
	if (fseek((FILE*)fd, 0, SEEK_END))
		return -1;
	if (fgetpos((FILE*)fd, &pos))
		return -1;
	return (int)pos;
}

#else

/*
 * Initialize the OSAL and allocate required resources.
 *
 * Parameters:
 *  maxChannels: Maximum number of debug channels to support.
 *
 * Return value:
 *  SII_OS_STATUS_SUCCESS on success
 *  SII_OS_STATUS_ERR_INVALID_PARAM if maxChannels == 0
 */
enum sii_os_status sii_os_init(uint32_t maxChannels)
{
	enum sii_os_status retval = SII_OS_STATUS_SUCCESS;

	maxChannels=maxChannels;
	#if 0
	if (0 == maxChannels) {
		err("Invalid parameter\n");
		retval = SII_OS_STATUS_ERR_INVALID_PARAM;
		goto done;
	}
	InitializeCriticalSection(&sii_section);
done:
	#endif
	return retval;
}

/*
 * Terminate the OSAL and deallocate all associated resources.
 *
 * Return value:
 *  SII_OS_STATUS_SUCCESS on success
 *  SII_OS_STATUS_ERR_FAILED if any error occurs
 */
enum sii_os_status sii_os_term(void)
{
	enum sii_os_status retval = SII_OS_STATUS_SUCCESS;

	return retval;
}

/*
 * Create a binary or counting semaphore.
 *
 * Parameters:
 *  pName: Semaphore identifier (16 chars max).
 *  maxCount: Max semaphore count (1 for binary semaphore).
 *  initialValue: Initial count on semaphore (0 for semaphore unavailable).
 *  pRetSemId: Returned semaphore object identifier or NULL on failure.
 *
 * Return value:
 *  SII_OS_STATUS_SUCCESS on success
 *  SII_OS_STATUS_ERR_INVALID_PARAM if any parameter is invalid
 *  SII_OS_STATUS_ERR_NOT_AVAIL if insufficient memory
 *  SII_OS_STATUS_ERR_FAILED for all other errors
 */
enum sii_os_status sii_os_semaphore_create(const char *pName, uint32_t maxCount,
					uint32_t initialValue,
					struct SiiOsSemaphore **pRetSemId)
{
	enum sii_os_status retval = SII_OS_STATUS_SUCCESS;

	#if 0
	struct SiiOsSemaphore *semaphoreData = NULL;

	dbg("%s\n", (NULL != pName) ? pName : "");

	if (NULL == pRetSemId) {
		err("Invalid parameter\n");
		retval = SII_OS_STATUS_ERR_INVALID_PARAM;
		goto done;
	}
	*pRetSemId = NULL;

	if (maxCount < initialValue) {
		err("Invalid parameter\n");
		retval = SII_OS_STATUS_ERR_INVALID_PARAM;
		goto done;
	}

	semaphoreData = sii_os_calloc(pName, sizeof(struct SiiOsSemaphore), 0);
	if (NULL == semaphoreData) {
		retval = SII_OS_STATUS_ERR_NOT_AVAIL;
		goto done;
	}

	semaphoreData->semaphore = CreateSemaphore(
		NULL,		   // default security attributes
		initialValue,   // initial count
		maxCount,	   // maximum count
		pName);		 // semaphore name

	if (NULL == semaphoreData->semaphore) {
		retval = SII_OS_STATUS_ERR_NOT_AVAIL;
		goto done;
	}

	*pRetSemId = semaphoreData;

done:
	if (SII_OS_STATUS_SUCCESS != retval)
		sii_os_free(semaphoreData);
	#endif

	return retval;
}

/*
 * Delete a semaphore.
 *
 * Parameters:
 *  semId: Semaphore object identifier.
 *
 * Return value:
 *  SII_OS_STATUS_SUCCESS on success
 *  SII_OS_STATUS_ERR_INVALID_PARAM if any parameter is invalid
 */
enum sii_os_status sii_os_semaphore_delete(struct SiiOsSemaphore *semId)
{
	enum sii_os_status retval = SII_OS_STATUS_SUCCESS;

	#if 0
	if (NULL == semId) {
		err("Invalid parameter\n");
		retval = SII_OS_STATUS_ERR_INVALID_PARAM;
		goto done;
	}

	CloseHandle(semId->semaphore);

	sii_os_free(semId);

done:
	#endif
	return retval;
}

/*
 * Release a semaphore.
 *
 * Parameters:
 *  semId: Semaphore object identifier.
 *
 * Return value:
 *  SII_OS_STATUS_SUCCESS on success
 *  SII_OS_STATUS_ERR_INVALID_PARAM if any parameter is invalid
 *  SII_OS_STATUS_ERR_SEM_COUNT_EXCEEEDED if too many releases occurred
 *  SII_OS_STATUS_ERR_FAILED for all other errors
 */
enum sii_os_status sii_os_semaphore_give(struct SiiOsSemaphore *semId)
{
	enum sii_os_status retval = SII_OS_STATUS_SUCCESS;

	#if 0
	if (NULL == semId) {
		err("Invalid parameter\n");
		retval = SII_OS_STATUS_ERR_INVALID_PARAM;
		goto done;
	}

	if (!ReleaseSemaphore(semId->semaphore, // handle to semaphore
						  1,				// increase count by one
						  NULL			  // not interested in previous count
						  )) {
		retval = SII_OS_STATUS_ERR_FAILED;
	}

done:
	#endif
	return retval;
}

/*
 * Take a semaphore with or without a timeout.
 *
 * Parameters:
 *  semId: Semaphore object identifier.
 *  timeMsec: Timeout period to wait for semaphore in milliseconds.
 *	Possible values:
 *	  SII_OS_INFINITE_WAIT to block waiting for the semaphore.
 *	  SII_OS_NO_WAIT to return immediately if semaphore is not available.
 *	  1 -> 2147483647 (0x7fffffff) for a finite wait for the semaphore.
 *
 * Return value:
 *  SII_OS_STATUS_SUCCESS on success
 *  SII_OS_STATUS_ERR_INVALID_PARAM if any parameter is invalid
 *  SII_OS_STATUS_TIMEOUT on a timeout
 *  SII_OS_STATUS_ERR_FAILED for all other errors
 */
enum sii_os_status sii_os_semaphore_take(struct SiiOsSemaphore *semId,
					int32_t timeMsec)
{
	enum sii_os_status retval = SII_OS_STATUS_SUCCESS;

	#if 0
	int down_retval;

	if (NULL == semId) {
		err("Invalid parameter\n");
		retval = SII_OS_STATUS_ERR_INVALID_PARAM;
		goto done;
	}

	down_retval = WaitForSingleObject(
									semId->semaphore,   // handle to semaphore
									timeMsec);		  // time-out interval

	if (down_retval) {
		if (WAIT_TIMEOUT == down_retval)
			retval = SII_OS_STATUS_TIMEOUT;
		else
			retval = SII_OS_STATUS_ERR_FAILED;
	}

done:
	#endif

	return retval;
}

/*
 * Allocate OS memory.
 *
 * Parameters:
 *  pName: name of the block.
 *  size: size in bytes.
 *  flags: OS supported flags (default=0). Currently, flags is ignored.
 *
 * Return value:
 *  Pointer to allocated memory or SII_OS_STATUS_NULL
 */
void *sii_os_alloc(const char *pName, uint32_t size, uint32_t flags)
{
	void *ptr;
	#if 0
	EnterCriticalSection(&sii_section);
	ptr = malloc((size_t)size);
	LeaveCriticalSection(&sii_section);
	#endif
	return ptr;
}

/*
 * Allocate OS memory and initialize the memory to 0.
 *
 * Parameters:
 *  pName: name of the block.
 *  size: size in bytes.
 *  flags: OS supported flags (default=0). Currently, flags is ignored.
 *
 * Return value:
 *  Pointer to allocated memory or SII_OS_STATUS_NULL
 */
void *sii_os_calloc(const char *pName, uint32_t size, uint32_t flags)
{
	void *ptr;
	#if 0
	EnterCriticalSection(&sii_section);
	ptr = calloc((size_t)size, sizeof(uint8_t));
	LeaveCriticalSection(&sii_section);
	#endif
	return ptr;
}

/*
 * Reallocate OS memory. The contents of the block are preserved up to the
 * lesser of the new and old sizes. If new size is smaller than old size,
 * an existing string will be truncated and not be NULL-terminated.
 *
 * Parameters:
 *  pName: name of the block.
 *  pAddr: address of previously allocated block.
 *  size: new size in bytes.
 *  flags: OS supported flags (default=0). Currently, flags is ignored.
 *
 * Return value:
 *  Pointer to allocated memory or SII_OS_STATUS_NULL
 */
void *
sii_os_realloc(const char *pName, void *pAddr, uint32_t size, uint32_t flags)
{
	void *ptr;
	#if 0
	EnterCriticalSection(&sii_section);
	ptr = realloc(pAddr, (size_t)size);
	LeaveCriticalSection(&sii_section);
	#endif
	return ptr;
}

/*
 * Free OS memory.
 *
 * Parameters:
 *  pAddr: address of memory allocated with SiiOsAlloc or SiiOsCalloc.
 *	  If pAddr==NULL, then nothing happens.
 */
void sii_os_free(void *pAddr)
{
	#if 0
	if (NULL != pAddr) {
		EnterCriticalSection(&sii_section);
		free(pAddr);
		LeaveCriticalSection(&sii_section);
	}
	#endif
	pAddr =pAddr;
}

void sii_os_sleep_msec(uint32_t timeMsec)
{
	//Sleep(timeMsec);
	vTaskDelay((uint16_t)timeMsec);
}

void sii_os_sleep_usec(uint32_t timeUsec)
{
	#if 0
	static LARGE_INTEGER frequence = {0};
	LARGE_INTEGER interval, privious, current;
	if (frequence.QuadPart == 0)
		QueryPerformanceFrequency(&frequence);
	interval.QuadPart = frequence.QuadPart * timeUsec /1000000;
	QueryPerformanceCounter(&privious);
	current = privious;
	while(current.QuadPart - privious.QuadPart < interval.QuadPart)
		QueryPerformanceCounter(&current);
	#endif
	vTaskDelay((uint16_t)timeUsec/1000);
}

uint32_t sii_os_get_current_time_second(void)
{
	//return (uint32_t)time(NULL);
	return 0;
}

bool sii_os_delay_second(uint32_t base, uint32_t delay)
{
	uint32_t time_diff = sii_os_get_current_time_second() - base;
	if (time_diff < delay) {
		return false;
	}
	return true;
}

int sii_os_file_open(const char* path, int flags, int rights)
{
	#if 0
	const char *file_flag_tab[] = {"rb","wb"};
	FILE* filp;
	filp = fopen(path, file_flag_tab[flags]);
	if (NULL == filp)
		return -1;
	return (int)filp;
	#endif
	return 1;
}

int sii_os_file_close(int fd)
{
	//return fclose((FILE*)fd);
	fd = fd;
	return 1;
}

int sii_os_file_read(int fd, long offset,
	unsigned int size, unsigned char* data)
{
	#if 0
	if (fseek((FILE*)fd, offset, SEEK_SET))
		return -1;
	return fread(data, sizeof(unsigned char), size, (FILE*)fd);
	#endif
	return 1;
}

int sii_os_get_file_length(int fd)
{
	#if 0
	fpos_t pos;
	if (fseek((FILE*)fd, 0, SEEK_END))
		return -1;
	if (fgetpos((FILE*)fd, &pos))
		return -1;
	return (int)pos;
	#endif
	return 256;
}

void sii_printf(const char *p_fmt, ...)
{
	va_list arg;
	uint8_t p_msg[MAX_DBG_MSG_LEN];
	int len = 0;
	int max_msg_len = MAX_DBG_MSG_LEN;
#if (_DEBUG_PORT_SEL==_UART_PORT_1)
        USART_TypeDef *pstUart = USART1;
#elif (_DEBUG_PORT_SEL==_UART_PORT_2)
        USART_TypeDef *pstUart = USART2;
#elif (_DEBUG_PORT_SEL==_UART_PORT_3)
        USART_TypeDef *pstUart = USART3;
#else
        USART_TypeDef *pstUart = USART2;
#endif

	va_start(arg, p_fmt);
	len = Uart_vsnprintf((char *)p_msg, max_msg_len, p_fmt, arg);
	va_end(arg);

	USARTx_DirectSend_bytes(pstUart, p_msg, len);
}

#endif

