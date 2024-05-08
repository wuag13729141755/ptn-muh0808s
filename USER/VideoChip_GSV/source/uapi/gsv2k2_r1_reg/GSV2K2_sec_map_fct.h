#ifndef GSV2K2_SEC_MAP_FCT_H
#define GSV2K2_SEC_MAP_FCT_H
#define GSV2K2_SEC_get_CP_CSC_BYPASS(port, pval)                      AvHalI2cReadField8(GSV2K2_SEC_MAP_ADDR(port), 0x01, 0x8, 0x3, pval)
#define GSV2K2_SEC_set_CP_CSC_BYPASS(port, val)                       AvHalI2cWriteField8(GSV2K2_SEC_MAP_ADDR(port), 0x01, 0x8, 0x3, val)
#define GSV2K2_SEC_get_CP_MODE(port, pval)                            AvHalI2cReadField8(GSV2K2_SEC_MAP_ADDR(port), 0x01, 0x7, 0, pval)
#define GSV2K2_SEC_set_CP_MODE(port, val)                             AvHalI2cWriteField8(GSV2K2_SEC_MAP_ADDR(port), 0x01, 0x7, 0, val)
#define GSV2K2_SEC_set_CP_SRC_SEL(port, val)                          AvHalI2cWriteField8(GSV2K2_SEC_MAP_ADDR(port), 0x04, 0x3, 0, val)
#define GSV2K2_SEC_set_TX2_SRC_SEL(port, val)                         AvHalI2cWriteField8(GSV2K2_SEC_MAP_ADDR(port), 0x05, 0x38, 3, val)
#define GSV2K2_SEC_set_TX1_SRC_SEL(port, val)                         AvHalI2cWriteField8(GSV2K2_SEC_MAP_ADDR(port), 0x05, 0x7, 0, val)
#define GSV2K2_SEC_set_RXB_FIFO_EN(port, val)                         AvHalI2cWriteField8(GSV2K2_SEC_MAP_ADDR(port), 0xCE, 0x20, 0x5, val)
#define GSV2K2_SEC_set_RXA_FIFO_EN(port, val)                         AvHalI2cWriteField8(GSV2K2_SEC_MAP_ADDR(port), 0xCE, 0x10, 0x4, val)
#define GSV2K2_SEC_set_RXB_FIFO_EN_OVERRIDE(port, val)                AvHalI2cWriteField8(GSV2K2_SEC_MAP_ADDR(port), 0xCE, 0x2, 0x1, val)
#define GSV2K2_SEC_set_RXA_FIFO_EN_OVERRIDE(port, val)                AvHalI2cWriteField8(GSV2K2_SEC_MAP_ADDR(port), 0xCE, 0x1, 0x0, val)
#define GSV2K2_SEC_get_RB_HDCP_RX_VERSION(port, pval)                 AvHalI2cReadField8(GSV2K2_SEC_MAP_ADDR(port), 0xEF, 0x3, 0, pval)
#endif