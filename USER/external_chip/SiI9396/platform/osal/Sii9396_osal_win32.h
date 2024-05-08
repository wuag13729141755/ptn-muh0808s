/*  
  2016 (C) Lattice Semiconductor Corporation
  
  No part of this work may be reproduced, modified, distributed, transmitted,
  transcribed, or translated into any language or computer format, in any form
  or by any means without written permission of Lattice Semiconductor, Inc.,
  111 SW 5th Ave, Suite 700, Portland, OR 97204.

*/

#ifndef _OSAL_WIN32_H
#define _OSAL_WIN32_H

#include <stdio.h>
//#include <windows.h>

extern int debug_level;

#define MAX_NAME_LEN 32

#define DEBUG_LEVEL_NONE		-1
#define DEBUG_LEVEL_CRITICAL		0
#define DEBUG_LEVEL_WARNINGS		1
#define DEBUG_LEVEL_VERBOSE	 2
//#define DEBUG_LEVEL_DEFAULT	   DEBUG_LEVEL_CRITICAL
#define DEBUG_LEVEL_DEFAULT	 DEBUG_LEVEL_VERBOSE

#define SII_FILE_FLAG_RDONLY		0

#define SII_FILE_RIGHT_IRUSR		0

struct SiiOsSemaphore {
	unsigned char semaphore;
};

void dbg(char *fmt, ...);
void err(char *fmt, ...);
void warn(char *fmt, ...);
void info(char *fmt, ...);
void edid_info(char *fmt, ...);

#if 0
#define dbg(format, ...) 
#if 0
	do { \
		if ((DEBUG_LEVEL_NONE != debug_level) && \
			(DEBUG_LEVEL_CRITICAL <= debug_level)) { \
			printf("SiI9396 dbg: %s: " format, \
				__FUNCTION__,  __VA_ARGS__); \
		} \
	} while (0)
#endif

#define err(format, ...) \
	do { \
		if ((DEBUG_LEVEL_NONE != debug_level) && \
			(DEBUG_LEVEL_CRITICAL <= debug_level)) { \
			printf("SiI9396 err: %s: " format, \
				__FUNCTION__,  __VA_ARGS__); \
		} \
	} while (0)

#define warn(format, ...) \
	do { \
		if ((DEBUG_LEVEL_NONE != debug_level) && \
			(DEBUG_LEVEL_WARNINGS <= debug_level)) { \
			printf("SiI9396 warn: %s: " format, \
				__FUNCTION__,  __VA_ARGS__); \
		} \
	} while (0)


#define info(format, ...) \
	do { \
		if ((DEBUG_LEVEL_NONE != debug_level) && \
			(DEBUG_LEVEL_CRITICAL <= debug_level)) { \
			printf(format,  __VA_ARGS__); \
		} \
	} while (0)
#endif

#endif /* !_OSAL_WIN32_H */
