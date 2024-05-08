/*
  2016 (C) Lattice Semiconductor Corporation

  No part of this work may be reproduced, modified, distributed, transmitted,
  transcribed, or translated into any language or computer format, in any form
  or by any means without written permission of Lattice Semiconductor, Inc.,
  111 SW 5th Ave, Suite 700, Portland, OR 97204.

*/

#ifndef _HAL_H
#define _HAL_H

#include "Sii9396_sii_common.h"

#define SII_RESET_TIME	0

#if (MACHINETYPE==MUH44A_H2)
#define SII_BOOT_TIME	200
#else
#define SII_BOOT_TIME	200
#endif
/*
   HAL API for Host Side
 */

enum sii_status sii_hal_platform_init(uint8_t *dev_num,
	sii_hwid_t* hwid, uint32_t *dev_id);
void sii_hal_platform_term(void);
enum sii_status sii_hal_init(sii_hwid_t hwid);
void sii_hal_term(sii_hwid_t hwid);
enum sii_status sii_hal_reset(sii_hwid_t hwid, bool reset);

enum sii_status sii_hal_int_pin_status(sii_hwid_t hwid, bool *status);
enum sii_status sii_hal_isr_enable(sii_hwid_t hwid, bool enable);
void sii_hal_isr(sii_hwid_t hwid);
void sii_hal_isr_done(sii_hwid_t hwid);

enum sii_status sii_hal_op(
		sii_hwid_t hwid,
		uint8_t *pTxData,
		uint16_t Txsize,
		uint8_t *pRxData,
		uint16_t Rxsize);
#endif /* !_HAL_H */
