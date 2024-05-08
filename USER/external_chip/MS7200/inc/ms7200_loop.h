
#ifndef _MS7200_LOOP_H_
#define _MS7200_LOOP_H_
#include "MS7200/inc/ms7200.h"

#ifndef DEV_NUM_MS7200
#define DEV_NUM_MS7200      1
#endif

#ifndef MS7200_DEV_ADDR
#define MS7200_DEV_ADDR     0X56//0XB2
#endif

#ifndef MS7200_CS_MODE
#define MS7200_CS_MODE      DVOUT_CS_MODE_YUV444//refer to DVOUT_CS_MODE_E
#endif
#ifndef MS7200_BW_MODE
#define MS7200_BW_MODE      DVOUT_BW_MODE_16_24BIT//refer to DVOUT_BW_MODE_E
#endif
#ifndef MS7200_DR_MODE
#define MS7200_DR_MODE      DVOUT_DR_MODE_SDR//refer to DVOUT_DR_MODE_E
#endif
#ifndef MS7200_SY_MODE
#define MS7200_SY_MODE      DVOUT_SY_MODE_HSVSDE//refer to DVOUT_SY_MODE_E
#endif

#ifndef MS7200_DEV_ADDR_1
#define MS7200_DEV_ADDR_1   0X56//0XB2
#endif

#ifndef MS7200_CS_MODE_1
#define MS7200_CS_MODE_1    DVOUT_CS_MODE_YUV444//refer to DVOUT_CS_MODE_E
#endif
#ifndef MS7200_BW_MODE_1
#define MS7200_BW_MODE_1    DVOUT_BW_MODE_16_24BIT//refer to DVOUT_BW_MODE_E
#endif
#ifndef MS7200_DR_MODE_1
#define MS7200_DR_MODE_1    DVOUT_DR_MODE_SDR//refer to DVOUT_DR_MODE_E
#endif
#ifndef MS7200_SY_MODE_1
#define MS7200_SY_MODE_1    DVOUT_SY_MODE_HSVSDE//refer to DVOUT_SY_MODE_E
#endif

#ifndef MS7200_DEV_ADDR_2
#define MS7200_DEV_ADDR_2   0X56//0XB2
#endif
#ifndef MS7200_CS_MODE_2
#define MS7200_CS_MODE_2    DVOUT_CS_MODE_YUV444//refer to DVOUT_CS_MODE_E
#endif
#ifndef MS7200_BW_MODE_2
#define MS7200_BW_MODE_2    DVOUT_BW_MODE_16_24BIT//refer to DVOUT_BW_MODE_E
#endif
#ifndef MS7200_DR_MODE_2
#define MS7200_DR_MODE_2    DVOUT_DR_MODE_SDR//refer to DVOUT_DR_MODE_E
#endif
#ifndef MS7200_SY_MODE_2
#define MS7200_SY_MODE_2    DVOUT_SY_MODE_HSVSDE//refer to DVOUT_SY_MODE_E
#endif

#ifndef MS7200_DEV_ADDR_3
#define MS7200_DEV_ADDR_3   0X56//0XB2
#endif
#ifndef MS7200_CS_MODE_3
#define MS7200_CS_MODE_3    DVOUT_CS_MODE_YUV444//refer to DVOUT_CS_MODE_E
#endif
#ifndef MS7200_BW_MODE_3
#define MS7200_BW_MODE_3    DVOUT_BW_MODE_16_24BIT//refer to DVOUT_BW_MODE_E
#endif
#ifndef MS7200_DR_MODE_3
#define MS7200_DR_MODE_3    DVOUT_DR_MODE_SDR//refer to DVOUT_DR_MODE_E
#endif
#ifndef MS7200_SY_MODE_3
#define MS7200_SY_MODE_3    DVOUT_SY_MODE_HSVSDE//refer to DVOUT_SY_MODE_E
#endif

#ifndef MS7200_DEV_ADDR_4
#define MS7200_DEV_ADDR_4   0X56//0XB2
#endif
#ifndef MS7200_CS_MODE_4
#define MS7200_CS_MODE_4    DVOUT_CS_MODE_YUV444//refer to DVOUT_CS_MODE_E
#endif
#ifndef MS7200_BW_MODE_4
#define MS7200_BW_MODE_4    DVOUT_BW_MODE_16_24BIT//refer to DVOUT_BW_MODE_E
#endif
#ifndef MS7200_DR_MODE_4
#define MS7200_DR_MODE_4    DVOUT_DR_MODE_SDR//refer to DVOUT_DR_MODE_E
#endif
#ifndef MS7200_SY_MODE_4
#define MS7200_SY_MODE_4    DVOUT_SY_MODE_HSVSDE//refer to DVOUT_SY_MODE_E
#endif

#ifndef MS7200_CLK_INV
#define MS7200_CLK_INV          MS_FALSE
#endif
#ifndef MS7200_PHASE_DELAY
#define MS7200_PHASE_DELAY      0
#endif

#ifndef MS7200_CLK_DRIVER
#define MS7200_CLK_DRIVER       0
#endif

#ifndef MS7200_AUDIO_OUT_SPDIF
#define MS7200_AUDIO_OUT_SPDIF      MS_FALSE
#endif

typedef struct _stMS7200Config_T_
{
    u8 u8I2cIndex;
    u8 u8DevAddr;
    u8 u8CSMode;
    u8 u8BWMode;
    u8 u8DRMode;
    u8 u8SYMode;
}stMS7200Config_T;

void ms7200_init(void);
void ms7200_loop(void);

#endif

