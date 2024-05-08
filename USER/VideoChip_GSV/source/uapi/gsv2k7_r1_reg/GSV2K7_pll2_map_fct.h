#ifndef GSV2K7_PLL2_MAP_FCT_H
#define GSV2K7_PLL2_MAP_FCT_H
#define GSV2K7_PLL2_get_RB_RXC_PLL_SER_DIV(port, pval)                 AvHalI2cReadField8(GSV2K7_PLL2_MAP_ADDR(port), 0x2F, 0x7, 0, pval)
#define GSV2K7_PLL2_get_RB_RXC_PLL_REF_PRE_DIV_FREQ(port, pval)        AvHalI2cReadField32BE(GSV2K7_PLL2_MAP_ADDR(port), 0x33, 0x3, 0xFF, 0, 3, pval)
#define GSV2K7_PLL2_get_RB_RXC_PLL_VCO_POST_DIV_FREQ(port, pval)       AvHalI2cReadField32BE(GSV2K7_PLL2_MAP_ADDR(port), 0x36, 0x3, 0xFF, 0, 3, pval)
#define GSV2K7_PLL2_get_RB_RXC_PLL_VCO_FREQ(port, pval)                AvHalI2cReadField32BE(GSV2K7_PLL2_MAP_ADDR(port), 0x39, 0xF, 0xFF, 0, 2, pval)
#define GSV2K7_PLL2_get_RB_RXD_PLL_SER_DIV(port, pval)                 AvHalI2cReadField8(GSV2K7_PLL2_MAP_ADDR(port), 0x5F, 0x7, 0, pval)
#define GSV2K7_PLL2_get_RB_RXD_PLL_REF_PRE_DIV_FREQ(port, pval)        AvHalI2cReadField32BE(GSV2K7_PLL2_MAP_ADDR(port), 0x63, 0x3, 0xFF, 0, 3, pval)
#define GSV2K7_PLL2_get_RB_RXD_PLL_VCO_POST_DIV_FREQ(port, pval)       AvHalI2cReadField32BE(GSV2K7_PLL2_MAP_ADDR(port), 0x66, 0x3, 0xFF, 0, 3, pval)
#define GSV2K7_PLL2_get_RB_RXD_PLL_VCO_FREQ(port, pval)                AvHalI2cReadField32BE(GSV2K7_PLL2_MAP_ADDR(port), 0x69, 0xF, 0xFF, 0, 2, pval)
#endif
