#ifndef GSV5K1_CCA_MAP_FCT_H
#define GSV5K1_CCA_MAP_FCT_H
#define GSV5K1_CCA_set_SRC_DDC_ENABLE(port, val)                      AvHalI2cWriteField8(GSV5K1_CCA_MAP_ADDR(port), 0x01, 0x1, 0x0, val)
#define GSV5K1_CCA_get_SRC_START_CALIBRATE_SWING(port, pval)          AvHalI2cReadField8(GSV5K1_CCA_MAP_ADDR(port), 0x07, 0x80, 0x7, pval)
#define GSV5K1_CCA_set_SRC_START_CALIBRATE_SWING(port, val)           AvHalI2cWriteField8(GSV5K1_CCA_MAP_ADDR(port), 0x07, 0x80, 0x7, val)
#define GSV5K1_CCA_set_SRC_FSM_RESET(port, val)                       AvHalI2cWriteField8(GSV5K1_CCA_MAP_ADDR(port), 0x42, 0x4, 0x2, val)
#define GSV5K1_CCA_get_RB_SRC_PRBS_PHASE_LOCKED(port, pval)           AvHalI2cReadField8(GSV5K1_CCA_MAP_ADDR(port), 0x50, 0x8, 0x3, pval)
#define GSV5K1_CCA_get_RB_SRC_END_CALIBRATE_SWING(port, pval)         AvHalI2cReadField8(GSV5K1_CCA_MAP_ADDR(port), 0x55, 0x80, 0x7, pval)
#define GSV5K1_CCA_get_RB_SRC_PRBS_ERROR_CNT(port, pval)              AvHalI2cReadField32LE(GSV5K1_CCA_MAP_ADDR(port), 0x56, 0xFF, 0xFF, 0, 3, pval)
#define GSV5K1_CCA_get_RB_SRC_CCA_TX_PLL_LOCK(port, pval)             AvHalI2cReadField8(GSV5K1_CCA_MAP_ADDR(port), 0x6D, 0x8, 0x3, pval)
#define GSV5K1_CCA_set_SNK_FSM_RESET(port, val)                       AvHalI2cWriteField8(GSV5K1_CCA_MAP_ADDR(port), 0xB9, 0x40, 0x6, val)
#define GSV5K1_CCA_set_SNK_MAN_PD_EDRIVER_EN(port, val)               AvHalI2cWriteField8(GSV5K1_CCA_MAP_ADDR(port), 0xB9, 0x4, 0x2, val)
#define GSV5K1_CCA_set_SNK_MAN_PD_MDRIVER_EN(port, val)               AvHalI2cWriteField8(GSV5K1_CCA_MAP_ADDR(port), 0xB9, 0x2, 0x1, val)
#define GSV5K1_CCA_get_RB_SNK_PRBS_PHASE_LOCKED(port, pval)           AvHalI2cReadField8(GSV5K1_CCA_MAP_ADDR(port), 0xD0, 0x8, 0x3, pval)
#define GSV5K1_CCA_get_RB_SNK_PRBS_ERROR_CNT(port, pval)              AvHalI2cReadField32LE(GSV5K1_CCA_MAP_ADDR(port), 0xD6, 0xFF, 0xFF, 0, 3, pval)
#define GSV5K1_CCA_get_RB_SNK_END_CALIBRATE_SWING(port, pval)         AvHalI2cReadField8(GSV5K1_CCA_MAP_ADDR(port), 0xDC, 0x10, 0x4, pval)
#define GSV5K1_CCA_get_RB_SNK_CCA_CDR_LOCK(port, pval)                AvHalI2cReadField8(GSV5K1_CCA_MAP_ADDR(port), 0xDC, 0x1, 0x0, pval)
#endif
