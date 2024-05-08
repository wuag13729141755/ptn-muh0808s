#ifndef GSV5K1_RX2P2_MAP_FCT_H
#define GSV5K1_RX2P2_MAP_FCT_H
#define GSV5K1_RX2P2_set_RX_HDCP2P2_REPEATER_ENABLE(port, val)          AvHalI2cWriteField8(GSV5K1_RX2P2_MAP_ADDR(port), 0x00, 0x1, 0x0, val)
#define GSV5K1_RX2P2_get_RX_DISABLE_ECC_CHECK(port, pval)               AvHalI2cReadField8(GSV5K1_RX2P2_MAP_ADDR(port), 0x07, 0x1, 0x0, pval)
#define GSV5K1_RX2P2_set_RX_DISABLE_ECC_CHECK(port, val)                AvHalI2cWriteField8(GSV5K1_RX2P2_MAP_ADDR(port), 0x07, 0x1, 0x0, val)
#define GSV5K1_RX2P2_set_RX_HDCP2P2_REAUTH_REQUEST(port, val)           AvHalI2cWriteField8(GSV5K1_RX2P2_MAP_ADDR(port), 0x15, 0x1, 0x0, val)
#define GSV5K1_RX2P2_set_RX_HDCP2P2_FSM_DISABLE(port, val)              AvHalI2cWriteField8(GSV5K1_RX2P2_MAP_ADDR(port), 0x1C, 0x1, 0x0, val)
#endif
