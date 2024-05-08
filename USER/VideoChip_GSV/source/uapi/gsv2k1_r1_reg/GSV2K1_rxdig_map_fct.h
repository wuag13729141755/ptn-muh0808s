#ifndef GSV2K1_RXDIG_MAP_FCT_H
#define GSV2K1_RXDIG_MAP_FCT_H
#define GSV2K1_RXDIG_get_RB_RX_TMDS_PLL_LOCKED(port, pval)              AvHalI2cReadField8(GSV2K1_RXDIG_MAP_ADDR(port), 0x00, 0x80, 0x7, pval)
#define GSV2K1_RXDIG_get_RB_RX_HDMI_MODE(port, pval)                    AvHalI2cReadField8(GSV2K1_RXDIG_MAP_ADDR(port), 0x00, 0x20, 0x5, pval)
#define GSV2K1_RXDIG_get_RB_RX_VS_LOCKED(port, pval)                    AvHalI2cReadField8(GSV2K1_RXDIG_MAP_ADDR(port), 0x00, 0x10, 0x4, pval)
#define GSV2K1_RXDIG_get_RB_RX_HS_LOCKED(port, pval)                    AvHalI2cReadField8(GSV2K1_RXDIG_MAP_ADDR(port), 0x00, 0x8, 0x3, pval)
#define GSV2K1_RXDIG_get_RB_RX_AUDIO_LAYOUT(port, pval)                 AvHalI2cReadField8(GSV2K1_RXDIG_MAP_ADDR(port), 0x00, 0x2, 0x1, pval)
#define GSV2K1_RXDIG_get_RB_RX_AVMUTE_STATE(port, pval)                 AvHalI2cReadField8(GSV2K1_RXDIG_MAP_ADDR(port), 0x00, 0x1, 0x0, pval)
#define GSV2K1_RXDIG_get_RB_RX_DEEP_COLOR_MODE(port, pval)              AvHalI2cReadField8(GSV2K1_RXDIG_MAP_ADDR(port), 0x03, 0x30, 4, pval)
#define GSV2K1_RXDIG_get_RB_RX_PIXEL_REPETITION(port, pval)             AvHalI2cReadField8(GSV2K1_RXDIG_MAP_ADDR(port), 0x03, 0xF, 0, pval)
#define GSV2K1_RXDIG_get_RB_RX_AUDIO_N(port, pval)                      AvHalI2cReadField32BE(GSV2K1_RXDIG_MAP_ADDR(port), 0x09, 0xF, 0xFF, 0, 3, pval)
#define GSV2K1_RXDIG_get_RB_RX_HS_POL(port, pval)                       AvHalI2cReadField8(GSV2K1_RXDIG_MAP_ADDR(port), 0x0C, 0x80, 0x7, pval)
#define GSV2K1_RXDIG_get_RB_RX_VS_POL(port, pval)                       AvHalI2cReadField8(GSV2K1_RXDIG_MAP_ADDR(port), 0x0C, 0x40, 0x6, pval)
#define GSV2K1_RXDIG_get_RB_RX_INTERLACED(port, pval)                   AvHalI2cReadField8(GSV2K1_RXDIG_MAP_ADDR(port), 0x0C, 0x20, 0x5, pval)
#define GSV2K1_RXDIG_get_RB_RX_H_ACTIVE_WIDTH(port, pval)               AvHalI2cReadField32BE(GSV2K1_RXDIG_MAP_ADDR(port), 0x0C, 0x1F, 0xFF, 0, 2, pval)
#define GSV2K1_RXDIG_get_RB_RX_V_ACTIVE_HEIGHT_0(port, pval)            AvHalI2cReadField32BE(GSV2K1_RXDIG_MAP_ADDR(port), 0x0E, 0x1F, 0xFF, 0, 2, pval)
#define GSV2K1_RXDIG_get_RX_REPETITION_MAN_EN(port, pval)               AvHalI2cReadField8(GSV2K1_RXDIG_MAP_ADDR(port), 0x12, 0x10, 0x4, pval)
#define GSV2K1_RXDIG_get_RX_REPETITION_MAN(port, pval)                  AvHalI2cReadField8(GSV2K1_RXDIG_MAP_ADDR(port), 0x12, 0xF, 0, pval)
#define GSV2K1_RXDIG_get_RX_AUDIO_MCLK_X_N(port, pval)                  AvHalI2cReadField8(GSV2K1_RXDIG_MAP_ADDR(port), 0x14, 0x7, 0, pval)
#define GSV2K1_RXDIG_set_RX_AUDIO_MCLK_X_N(port, val)                   AvHalI2cWriteField8(GSV2K1_RXDIG_MAP_ADDR(port), 0x14, 0x7, 0, val)
#define GSV2K1_RXDIG_set_DCFIFO_RECENTER(port, val)                     AvHalI2cWriteField8(GSV2K1_RXDIG_MAP_ADDR(port), 0x22, 0x4, 0x2, val)
#define GSV2K1_RXDIG_get_RB_RX_H_TOTAL_WIDTH(port, pval)                AvHalI2cReadField32BE(GSV2K1_RXDIG_MAP_ADDR(port), 0x26, 0x3F, 0xFF, 0, 2, pval)
#define GSV2K1_RXDIG_get_RB_RX_H_FRONT_PORCH(port, pval)                AvHalI2cReadField32BE(GSV2K1_RXDIG_MAP_ADDR(port), 0x28, 0x1F, 0xFF, 0, 2, pval)
#define GSV2K1_RXDIG_get_RB_RX_H_SYNC_WIDTH(port, pval)                 AvHalI2cReadField32BE(GSV2K1_RXDIG_MAP_ADDR(port), 0x2A, 0x1F, 0xFF, 0, 2, pval)
#define GSV2K1_RXDIG_get_RB_RX_H_BACK_PORCH(port, pval)                 AvHalI2cReadField32BE(GSV2K1_RXDIG_MAP_ADDR(port), 0x2C, 0x1F, 0xFF, 0, 2, pval)
#define GSV2K1_RXDIG_get_RB_RX_V_TOTAL_0(port, pval)                    AvHalI2cReadField32BE(GSV2K1_RXDIG_MAP_ADDR(port), 0x2E, 0x3F, 0xFF, 0, 2, pval)
#define GSV2K1_RXDIG_get_RB_RX_V_FRONT_PORCH_0(port, pval)              AvHalI2cReadField32BE(GSV2K1_RXDIG_MAP_ADDR(port), 0x32, 0x3F, 0xFF, 0, 2, pval)
#define GSV2K1_RXDIG_get_RB_RX_V_SYNC_WIDTH_0(port, pval)               AvHalI2cReadField32BE(GSV2K1_RXDIG_MAP_ADDR(port), 0x36, 0x3F, 0xFF, 0, 2, pval)
#define GSV2K1_RXDIG_get_RB_RX_V_BACK_PORCH_0(port, pval)               AvHalI2cReadField32BE(GSV2K1_RXDIG_MAP_ADDR(port), 0x3A, 0x3F, 0xFF, 0, 2, pval)
#define GSV2K1_RXDIG_get_VPLL_MANUAL_MODE(port, pval)                   AvHalI2cReadField8(GSV2K1_RXDIG_MAP_ADDR(port), 0x5D, 0x40, 0x6, pval)
#define GSV2K1_RXDIG_set_VPLL_MANUAL_MODE(port, val)                    AvHalI2cWriteField8(GSV2K1_RXDIG_MAP_ADDR(port), 0x5D, 0x40, 0x6, val)
#define GSV2K1_RXDIG_set_N_CTS_STABLE_DET_START(port, val)              AvHalI2cWriteField8(GSV2K1_RXDIG_MAP_ADDR(port), 0x7A, 0x2, 0x1, val)
#define GSV2K1_RXDIG_get_RX_APLL_MAN_START_CALC(port, pval)             AvHalI2cReadField8(GSV2K1_RXDIG_MAP_ADDR(port), 0x7A, 0x1, 0x0, pval)
#define GSV2K1_RXDIG_set_RX_APLL_MAN_START_CALC(port, val)              AvHalI2cWriteField8(GSV2K1_RXDIG_MAP_ADDR(port), 0x7A, 0x1, 0x0, val)
#define GSV2K1_RXDIG_set_ECC_AVMUTE_MAN_EN(port, val)                   AvHalI2cWriteField8(GSV2K1_RXDIG_MAP_ADDR(port), 0xBE, 0x10, 0x4, val)
#endif
