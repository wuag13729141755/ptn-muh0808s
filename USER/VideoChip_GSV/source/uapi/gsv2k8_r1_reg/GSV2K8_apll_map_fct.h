#ifndef GSV2K8_APLL_MAP_FCT_H
#define GSV2K8_APLL_MAP_FCT_H

#define GSV2K8_APLL_set_TXA_RPLL_FASTLOCK_CTL(port, val)               AvHalI2cWriteField8(GSV2K8_APLL_MAP_ADDR(port), 0x52, 0x40, 0x6, val)
#define GSV2K8_APLL_set_RXA_RPLL_FASTLOCK_CTL(port, val)               AvHalI2cWriteField8(GSV2K8_APLL_MAP_ADDR(port), 0x62, 0x40, 0x6, val)
#define GSV2K8_APLL_set_TXB_RPLL_FASTLOCK_CTL(port, val)               AvHalI2cWriteField8(GSV2K8_APLL_MAP_ADDR(port), 0x82, 0x40, 0x6, val)
#define GSV2K8_APLL_set_RXB_RPLL_FASTLOCK_CTL(port, val)               AvHalI2cWriteField8(GSV2K8_APLL_MAP_ADDR(port), 0x92, 0x40, 0x6, val)
#define GSV2K8_APLL_set_RXC_RPLL_FASTLOCK_CTL(port, val)               AvHalI2cWriteField8(GSV2K8_APLL_MAP_ADDR(port), 0xB2, 0x40, 0x6, val)
#define GSV2K8_APLL_set_RXD_RPLL_FASTLOCK_CTL(port, val)               AvHalI2cWriteField8(GSV2K8_APLL_MAP_ADDR(port), 0xE2, 0x40, 0x6, val)

#endif
