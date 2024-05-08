#ifndef GSV2K11_TX2P2_MAP_FCT_H
#define GSV2K11_TX2P2_MAP_FCT_H
#define GSV2K11_TX2P2_get_TX_HDCP2P2_DESIRED(port, pval)                 AvHalI2cReadField8(GSV2K11_TX2P2_MAP_ADDR(port), 0x00, 0x1, 0x0, pval)
#define GSV2K11_TX2P2_set_TX_HDCP2P2_DESIRED(port, val)                  AvHalI2cWriteField8(GSV2K11_TX2P2_MAP_ADDR(port), 0x00, 0x1, 0x0, val)
#define GSV2K11_TX2P2_set_HDCP2P2_MASTER_CLK_DIV(port, val)              AvHalI2cWriteField32BE(GSV2K11_TX2P2_MAP_ADDR(port), 0x06, 0xFF, 0xFF, 0, 2, val)
#define GSV2K11_TX2P2_set_REVOCATION_CHECK_PASS(port, val)               AvHalI2cWriteField8(GSV2K11_TX2P2_MAP_ADDR(port), 0x08, 0x1, 0x0, val)
#define GSV2K11_TX2P2_set_TX_HDCP2P2_CLEAR_ERROR(port, val)              AvHalI2cWriteField8(GSV2K11_TX2P2_MAP_ADDR(port), 0x0B, 0x80, 0x7, val)
#define GSV2K11_TX2P2_set_TX_HDCP2P2_OP_WHEN_ERROR(port, val)            AvHalI2cWriteField8(GSV2K11_TX2P2_MAP_ADDR(port), 0x0B, 0x40, 0x6, val)
#define GSV2K11_TX2P2_set_TX_HDCP2P2_ENCRYPTION_ENABLE(port, val)        AvHalI2cWriteField8(GSV2K11_TX2P2_MAP_ADDR(port), 0x0B, 0x2, 0x1, val)
#define GSV2K11_TX2P2_set_TX_HDCP2P2_FSM_DISABLE(port, val)              AvHalI2cWriteField8(GSV2K11_TX2P2_MAP_ADDR(port), 0x28, 0x1, 0x0, val)
#define GSV2K11_TX2P2_set_TX_HDCP2P2_CLEAR_RXID_LIST_READY(port, val)    AvHalI2cWriteField8(GSV2K11_TX2P2_MAP_ADDR(port), 0x5D, 0x2, 0x1, val)
#define GSV2K11_TX2P2_set_TX_HDCP2P2_CLEAR_RXID_READY(port, val)         AvHalI2cWriteField8(GSV2K11_TX2P2_MAP_ADDR(port), 0x5D, 0x1, 0x0, val)
#endif
