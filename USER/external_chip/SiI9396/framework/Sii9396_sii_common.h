/*
  2016 (C) Lattice Semiconductor Corporation

  No part of this work may be reproduced, modified, distributed, transmitted,
  transcribed, or translated into any language or computer format, in any form
  or by any means without written permission of Lattice Semiconductor, Inc.,
  111 SW 5th Ave, Suite 700, Portland, OR 97204.

 */

#ifndef _SII_COMMON_H
#define _SII_COMMON_H

#ifdef __KERNEL__
	#include <linux/types.h> /* to get 'bool' */
	#define SII_IPC_HOST
#endif
#ifdef WIN32
	#define _CRT_SECURE_NO_WARNINGS
	#include <string.h>
	#include <stdint.h>
	//#include <stdbool.h>
	#if !defined(__BOOL_DEFINED)
	  typedef char	bool;
	  #define false 0
	  #define true (!false)
	#endif
	#define SII_IPC_HOST
#else
	#include <string.h>
	#include <stdint.h>
	#include "Sii9396_si_datatypes.h"
	//#include <stdbool.h>
	//typedef char	bool;
	//#define false 0
	//#define true (!false)
	#define SII_IPC_HOST
#endif

//#define TRUE  true
//#define FALSE false

#define CHIP_ID			0x9396
#define HOST_APP_NAME	"SiI9396 Rx"
#define HOST_SW_VER		"00.90.02"
#define RX_FW_VER		"00.90.02"

#ifdef SII_IPC_HOST
#define sii_hwid_t	uint8_t
#define sii_inst_t	uint8_t
#define SII_INST_NULL	(sii_inst_t)(-1)
#define SII_HWID_NULL	(sii_hwid_t)(-1)

#define SII_INST_COUNT_MAX	SII9396_DevNum
#define SII_DEV_COUNT_MAX	SII9396_DevNum
#endif /* SII_IPC_HOST */

#ifdef SII_IPC_SLAVE
#include "si_c99support.h"

#define bool		bool_t
#define int64_t		int32_t
#define uint64_t	uint32_t
#endif /* SII_IPC_SLAVE */

enum sii_status {
	SII_STATUS_SUCCESS = 0,
	SII_STATUS_ERR_INVALID_PARAM = -1,
	SII_STATUS_ERR_FAILED = -2,
};


#endif /* !_SII_COMMON_H */
