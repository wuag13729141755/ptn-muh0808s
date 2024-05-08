/*
  2016 (C) Lattice Semiconductor Corporation

  No part of this work may be reproduced, modified, distributed, transmitted,
  transcribed, or translated into any language or computer format, in any form
  or by any means without written permission of Lattice Semiconductor, Inc.,
  111 SW 5th Ave, Suite 700, Portland, OR 97204.


*/

#ifndef _FLASH_H
#define _FLASH_H

#include "Sii9396_sii_common.h"

#define SII_ISP_PAGE_SIZE   256
#define MAX_FLASH_RETRY_TIMES	3

enum flash_status {
	FLASH_UPDATE_SUCCESS,
	FLASH_IN_PROGRESS,
	FLASH_VERIFY_ERR,
	FLASH_NOT_READY,
	FLASH_READY,
};

enum sii_status flash_init_status(sii_inst_t inst);
enum sii_status flash_init(sii_inst_t inst);
enum flash_status flash_get_status(sii_inst_t inst);
enum sii_status flash_update(sii_inst_t inst, const uint8_t* file_name);

#endif /* !_FLASH_H */
