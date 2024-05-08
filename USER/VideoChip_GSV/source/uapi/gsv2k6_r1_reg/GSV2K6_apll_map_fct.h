#ifndef GSV2K6_APLL_MAP_FCT_H
#define GSV2K6_APLL_MAP_FCT_H
#define GSV2K6_APLL_set_TXA_RPLL_FASTLOCK_CTL(port, val)               AvHalI2cWriteField8(GSV2K6_APLL_MAP_ADDR(port), 0x52, 0x40, 0x6, val)
#define GSV2K6_APLL_set_RXA_RPLL_FASTLOCK_CTL(port, val)               AvHalI2cWriteField8(GSV2K6_APLL_MAP_ADDR(port), 0x62, 0x40, 0x6, val)
#define GSV2K6_APLL_set_TXB_RPLL_FASTLOCK_CTL(port, val)               AvHalI2cWriteField8(GSV2K6_APLL_MAP_ADDR(port), 0x82, 0x40, 0x6, val)
#define GSV2K6_APLL_set_RXB_RPLL_FASTLOCK_CTL(port, val)               AvHalI2cWriteField8(GSV2K6_APLL_MAP_ADDR(port), 0x92, 0x40, 0x6, val)
#define GSV2K6_APLL_set_TXC_RPLL_FASTLOCK_CTL(port, val)               AvHalI2cWriteField8(GSV2K6_APLL_MAP_ADDR(port), 0xA2, 0x40, 0x6, val)
#define GSV2K6_APLL_set_TXD_RPLL_FASTLOCK_CTL(port, val)               AvHalI2cWriteField8(GSV2K6_APLL_MAP_ADDR(port), 0xD2, 0x40, 0x6, val)
#endif
