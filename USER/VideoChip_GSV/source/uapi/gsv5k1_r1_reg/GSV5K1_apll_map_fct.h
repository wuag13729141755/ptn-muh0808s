#ifndef GSV5K1_APLL_MAP_FCT_H
#define GSV5K1_APLL_MAP_FCT_H
#define GSV5K1_APLL_set_TXA_RPLL_FASTLOCK_CTL(port, val)               AvHalI2cWriteField8(GSV5K1_APLL_MAP_ADDR(port), 0x52, 0x40, 0x6, val)
#define GSV5K1_APLL_set_RXA_RPLL_FASTLOCK_CTL(port, val)               AvHalI2cWriteField8(GSV5K1_APLL_MAP_ADDR(port), 0x62, 0x40, 0x6, val)
#define GSV5K1_APLL_set_RXA_CKIO_EN_Z0(port, val)                      AvHalI2cWriteField8(GSV5K1_APLL_MAP_ADDR(port), 0x70, 0x2, 0x1, val)
#define GSV5K1_APLL_set_RXB_CKIO_EN_Z0(port, val)                      AvHalI2cWriteField8(GSV5K1_APLL_MAP_ADDR(port), 0x73, 0x2, 0x1, val)
#define GSV5K1_APLL_set_TXB_RPLL_FASTLOCK_CTL(port, val)               AvHalI2cWriteField8(GSV5K1_APLL_MAP_ADDR(port), 0x82, 0x40, 0x6, val)
#define GSV5K1_APLL_set_RXB_RPLL_FASTLOCK_CTL(port, val)               AvHalI2cWriteField8(GSV5K1_APLL_MAP_ADDR(port), 0x92, 0x40, 0x6, val)
#endif
