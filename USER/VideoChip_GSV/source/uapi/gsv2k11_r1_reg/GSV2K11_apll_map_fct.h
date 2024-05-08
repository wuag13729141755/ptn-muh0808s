#ifndef GSV2K11_APLL_MAP_FCT_H
#define GSV2K11_APLL_MAP_FCT_H
#define GSV2K11_APLL_set_TXA_RPLL_FASTLOCK_CTL(port, val)               AvHalI2cWriteField8(GSV2K11_APLL_MAP_ADDR(port), 0x52, 0x40, 0x6, val)
#define GSV2K11_APLL_set_RXA_RPLL_FASTLOCK_CTL(port, val)               AvHalI2cWriteField8(GSV2K11_APLL_MAP_ADDR(port), 0x62, 0x40, 0x6, val)
#endif
