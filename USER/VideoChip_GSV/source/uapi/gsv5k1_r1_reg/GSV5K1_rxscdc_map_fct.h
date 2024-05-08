#ifndef GSV5K1_RXSCDC_MAP_FCT_H
#define GSV5K1_RXSCDC_MAP_FCT_H
#define GSV5K1_RXSCDC_set_SCDC_MFS0(port, val)                           AvHalI2cWriteField8(GSV5K1_RXSCDC_MAP_ADDR(port), 0xDE, 0xFF, 0, val)
#define GSV5K1_RXSCDC_set_SCDC_MFS1(port, val)                           AvHalI2cWriteField8(GSV5K1_RXSCDC_MAP_ADDR(port), 0xDF, 0xFF, 0, val)
#define GSV5K1_RXSCDC_set_SCDC_MFS10(port, val)                          AvHalI2cWriteField8(GSV5K1_RXSCDC_MAP_ADDR(port), 0xE8, 0xFF, 0, val)
#define GSV5K1_RXSCDC_set_SCDC_MFS11(port, val)                          AvHalI2cWriteField8(GSV5K1_RXSCDC_MAP_ADDR(port), 0xE9, 0xFF, 0, val)
#define GSV5K1_RXSCDC_set_SCDC_MFS12(port, val)                          AvHalI2cWriteField8(GSV5K1_RXSCDC_MAP_ADDR(port), 0xEA, 0xFF, 0, val)
#define GSV5K1_RXSCDC_set_SCDC_MFS13(port, val)                          AvHalI2cWriteField8(GSV5K1_RXSCDC_MAP_ADDR(port), 0xEB, 0xFF, 0, val)
#define GSV5K1_RXSCDC_set_SCDC_MFS14(port, val)                          AvHalI2cWriteField8(GSV5K1_RXSCDC_MAP_ADDR(port), 0xEC, 0xFF, 0, val)
#define GSV5K1_RXSCDC_set_SCDC_MFS15(port, val)                          AvHalI2cWriteField8(GSV5K1_RXSCDC_MAP_ADDR(port), 0xED, 0xFF, 0, val)
#define GSV5K1_RXSCDC_set_SCDC_MFS16(port, val)                          AvHalI2cWriteField8(GSV5K1_RXSCDC_MAP_ADDR(port), 0xEE, 0xFF, 0, val)
#define GSV5K1_RXSCDC_set_SCDC_MFS17(port, val)                          AvHalI2cWriteField8(GSV5K1_RXSCDC_MAP_ADDR(port), 0xEF, 0xFF, 0, val)
#define GSV5K1_RXSCDC_set_SCDC_MFS18(port, val)                          AvHalI2cWriteField8(GSV5K1_RXSCDC_MAP_ADDR(port), 0xF0, 0xFF, 0, val)
#define GSV5K1_RXSCDC_set_SCDC_MFS19(port, val)                          AvHalI2cWriteField8(GSV5K1_RXSCDC_MAP_ADDR(port), 0xF1, 0xFF, 0, val)
#endif
