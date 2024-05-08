#ifndef GSV2K11_PLL_MAP_FCT_H
#define GSV2K11_PLL_MAP_FCT_H
#define GSV2K11_PLL_get_RB_RXA_PLL_SER_DIV(port, pval)                 AvHalI2cReadField8(GSV2K11_PLL_MAP_ADDR(port), 0x2F, 0x7, 0, pval)
#define GSV2K11_PLL_get_RB_RXA_PLL_REF_PRE_DIV_FREQ(port, pval)        AvHalI2cReadField32BE(GSV2K11_PLL_MAP_ADDR(port), 0x33, 0x3, 0xFF, 0, 3, pval)
#define GSV2K11_PLL_get_RB_RXA_PLL_VCO_POST_DIV_FREQ(port, pval)       AvHalI2cReadField32BE(GSV2K11_PLL_MAP_ADDR(port), 0x36, 0x3, 0xFF, 0, 3, pval)
#define GSV2K11_PLL_get_RB_RXA_PLL_VCO_FREQ(port, pval)                AvHalI2cReadField32BE(GSV2K11_PLL_MAP_ADDR(port), 0x39, 0xF, 0xFF, 0, 2, pval)
#define GSV2K11_PLL_set_TXA_PLL_LOCK_CLEAR(port, val)                  AvHalI2cWriteField8(GSV2K11_PLL_MAP_ADDR(port), 0x86, 0x1, 0x0, val)
#define GSV2K11_PLL_get_RB_TXA_PLL_LOCK(port, pval)                    AvHalI2cReadField8(GSV2K11_PLL_MAP_ADDR(port), 0x8C, 0x80, 0x7, pval)
#define GSV2K11_PLL_get_RB_TXA_PLL_PRE_DIV(port, pval)                 AvHalI2cReadField8(GSV2K11_PLL_MAP_ADDR(port), 0x8C, 0x1F, 0, pval)
#define GSV2K11_PLL_get_RB_TXA_PLL_POST_DIV(port, pval)                AvHalI2cReadField32BE(GSV2K11_PLL_MAP_ADDR(port), 0x8D, 0x7, 0xFF, 0, 2, pval)
#define GSV2K11_PLL_get_RB_TXA_PLL_REF_CLK_FREQ(port, pval)            AvHalI2cReadField32BE(GSV2K11_PLL_MAP_ADDR(port), 0x90, 0x3, 0xFF, 0, 3, pval)
#define GSV2K11_PLL_get_RB_TXA_PLL_REF_PRE_DIV_FREQ(port, pval)        AvHalI2cReadField32BE(GSV2K11_PLL_MAP_ADDR(port), 0x93, 0x3, 0xFF, 0, 3, pval)
#define GSV2K11_PLL_get_RB_TXA_PLL_VCO_POST_DIV_FREQ(port, pval)       AvHalI2cReadField32BE(GSV2K11_PLL_MAP_ADDR(port), 0x96, 0x3, 0xFF, 0, 3, pval)
#define GSV2K11_PLL_get_RB_TXA_PLL_LOCK_CAPTURED(port, pval)           AvHalI2cReadField8(GSV2K11_PLL_MAP_ADDR(port), 0x9B, 0x8, 0x3, pval)
#define GSV2K11_PLL_get_RB_TXB_PLL_SER_DIV(port, pval)                 AvHalI2cReadField8(GSV2K11_PLL_MAP_ADDR(port), 0xEC, 0x7, 0, pval)
#define GSV2K11_PLL_set_VG_CLK_SRC_SEL(port, val)                      AvHalI2cWriteField8(GSV2K11_PLL_MAP_ADDR(port), 0xFA, 0x7, 0, val)
#endif
