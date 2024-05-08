#ifndef GSV2K11_PPLL_MAP_FCT_H
#define GSV2K11_PPLL_MAP_FCT_H
#define GSV2K11_PPLL_set_PAR_RPLL_FASTLOCK_CTL(port, val)               AvHalI2cWriteField8(GSV2K11_PPLL_MAP_ADDR(port), 0x52, 0x40, 0x6, val)
#define GSV2K11_PPLL_set_PAR_PLL_LOCK_CLEAR(port, val)                  AvHalI2cWriteField8(GSV2K11_PPLL_MAP_ADDR(port), 0x86, 0x1, 0x0, val)
#define GSV2K11_PPLL_get_RB_PAR_PLL_LOCK(port, pval)                    AvHalI2cReadField8(GSV2K11_PPLL_MAP_ADDR(port), 0x8C, 0x80, 0x7, pval)
#define GSV2K11_PPLL_get_RB_PAR_PLL_REF_CLK_FREQ(port, pval)            AvHalI2cReadField32BE(GSV2K11_PPLL_MAP_ADDR(port), 0x90, 0x3, 0xFF, 0, 3, pval)
#define GSV2K11_PPLL_get_RB_PAR_PLL_REF_PRE_DIV_FREQ(port, pval)        AvHalI2cReadField32BE(GSV2K11_PPLL_MAP_ADDR(port), 0x93, 0x3, 0xFF, 0, 3, pval)
#define GSV2K11_PPLL_get_RB_PAR_PLL_VCO_POST_DIV_FREQ(port, pval)       AvHalI2cReadField32BE(GSV2K11_PPLL_MAP_ADDR(port), 0x96, 0x3, 0xFF, 0, 3, pval)
#define GSV2K11_PPLL_get_RB_PAR_PLL_LOCK_CAPTURED(port, pval)           AvHalI2cReadField8(GSV2K11_PPLL_MAP_ADDR(port), 0x9B, 0x8, 0x3, pval)
#endif
