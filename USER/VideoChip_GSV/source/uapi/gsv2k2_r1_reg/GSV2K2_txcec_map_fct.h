#ifndef GSV2K2_TXCEC_MAP_FCT_H
#define GSV2K2_TXCEC_MAP_FCT_H
#define GSV2K2_TXCEC_set_CEC_TX_FRAME_LENGTH(port, val)                 AvHalI2cWriteField8(GSV2K2_TXCEC_MAP_ADDR(port), 0x10, 0x1F, 0, val)
#define GSV2K2_TXCEC_get_CEC_TX_ENABLE(port, pval)                      AvHalI2cReadField8(GSV2K2_TXCEC_MAP_ADDR(port), 0x11, 0x1, 0x0, pval)
#define GSV2K2_TXCEC_set_CEC_TX_ENABLE(port, val)                       AvHalI2cWriteField8(GSV2K2_TXCEC_MAP_ADDR(port), 0x11, 0x1, 0x0, val)
#define GSV2K2_TXCEC_set_CEC_TX_RETRY(port, val)                        AvHalI2cWriteField8(GSV2K2_TXCEC_MAP_ADDR(port), 0x12, 0x70, 4, val)
#define GSV2K2_TXCEC_get_TX_LOWDRIVE_COUNTER(port, pval)                AvHalI2cReadField8(GSV2K2_TXCEC_MAP_ADDR(port), 0x14, 0xF0, 4, pval)
#define GSV2K2_TXCEC_get_CEC_TX_NACK_COUNTER(port, pval)                AvHalI2cReadField8(GSV2K2_TXCEC_MAP_ADDR(port), 0x14, 0xF, 0, pval)
#define GSV2K2_TXCEC_get_CEC_RX_BUF0_FRAME_LENGTH(port, pval)           AvHalI2cReadField8(GSV2K2_TXCEC_MAP_ADDR(port), 0x25, 0x1F, 0, pval)
#define GSV2K2_TXCEC_get_CEC_RX_BUF2_TIMESTAMP(port, pval)              AvHalI2cReadField8(GSV2K2_TXCEC_MAP_ADDR(port), 0x26, 0x30, 4, pval)
#define GSV2K2_TXCEC_get_CEC_RX_BUF1_TIMESTAMP(port, pval)              AvHalI2cReadField8(GSV2K2_TXCEC_MAP_ADDR(port), 0x26, 0xC, 2, pval)
#define GSV2K2_TXCEC_get_CEC_RX_BUF0_TIMESTAMP(port, pval)              AvHalI2cReadField8(GSV2K2_TXCEC_MAP_ADDR(port), 0x26, 0x3, 0, pval)
#define GSV2K2_TXCEC_get_CEC_RX_BUF1_FRAME_LENGTH(port, pval)           AvHalI2cReadField8(GSV2K2_TXCEC_MAP_ADDR(port), 0x37, 0x1F, 0, pval)
#define GSV2K2_TXCEC_get_CEC_RX_BUF2_FRAME_LENGTH(port, pval)           AvHalI2cReadField8(GSV2K2_TXCEC_MAP_ADDR(port), 0x48, 0x1F, 0, pval)
#define GSV2K2_TXCEC_get_CEC_RX_BUF2_READY(port, pval)                  AvHalI2cReadField8(GSV2K2_TXCEC_MAP_ADDR(port), 0x49, 0x4, 0x2, pval)
#define GSV2K2_TXCEC_get_CEC_RX_BUF1_READY(port, pval)                  AvHalI2cReadField8(GSV2K2_TXCEC_MAP_ADDR(port), 0x49, 0x2, 0x1, pval)
#define GSV2K2_TXCEC_get_CEC_RX_BUF0_READY(port, pval)                  AvHalI2cReadField8(GSV2K2_TXCEC_MAP_ADDR(port), 0x49, 0x1, 0x0, pval)
#define GSV2K2_TXCEC_set_CEC_RX_3BUFFERS_ENABLE(port, val)              AvHalI2cWriteField8(GSV2K2_TXCEC_MAP_ADDR(port), 0x4A, 0x8, 0x3, val)
#define GSV2K2_TXCEC_set_CEC_RX_CLEAR_BUF2(port, val)                   AvHalI2cWriteField8(GSV2K2_TXCEC_MAP_ADDR(port), 0x4A, 0x4, 0x2, val)
#define GSV2K2_TXCEC_set_CEC_RX_CLEAR_BUF1(port, val)                   AvHalI2cWriteField8(GSV2K2_TXCEC_MAP_ADDR(port), 0x4A, 0x2, 0x1, val)
#define GSV2K2_TXCEC_set_CEC_RX_CLEAR_BUF0(port, val)                   AvHalI2cWriteField8(GSV2K2_TXCEC_MAP_ADDR(port), 0x4A, 0x1, 0x0, val)
#define GSV2K2_TXCEC_get_CEC_LOGIC_ADDR_SELECTION(port, pval)           AvHalI2cReadField8(GSV2K2_TXCEC_MAP_ADDR(port), 0x4B, 0x70, 4, pval)
#define GSV2K2_TXCEC_set_CEC_LOGIC_ADDR_SELECTION(port, val)            AvHalI2cWriteField8(GSV2K2_TXCEC_MAP_ADDR(port), 0x4B, 0x70, 4, val)
#define GSV2K2_TXCEC_set_CEC_LOGIC_ADDR1(port, val)                     AvHalI2cWriteField8(GSV2K2_TXCEC_MAP_ADDR(port), 0x4C, 0xF0, 4, val)
#define GSV2K2_TXCEC_set_CEC_LOGIC_ADDR0(port, val)                     AvHalI2cWriteField8(GSV2K2_TXCEC_MAP_ADDR(port), 0x4C, 0xF, 0, val)
#define GSV2K2_TXCEC_set_CEC_LOGIC_ADDR2(port, val)                     AvHalI2cWriteField8(GSV2K2_TXCEC_MAP_ADDR(port), 0x4D, 0xF, 0, val)
#define GSV2K2_TXCEC_set_CEC_POWER_MODE(port, val)                      AvHalI2cWriteField8(GSV2K2_TXCEC_MAP_ADDR(port), 0x4E, 0x3, 0, val)
#define GSV2K2_TXCEC_set_CEC_PHY_ADDR(port, val)                        AvHalI2cWriteField32BE(GSV2K2_TXCEC_MAP_ADDR(port), 0x80, 0xFF, 0xFF, 0, 2, val)
#endif
