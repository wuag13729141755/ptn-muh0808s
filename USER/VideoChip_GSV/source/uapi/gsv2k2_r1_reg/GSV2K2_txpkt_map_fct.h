#ifndef GSV2K2_TXPKT_MAP_FCT_H
#define GSV2K2_TXPKT_MAP_FCT_H
#define GSV2K2_TXPKT_set_AVI_IF_TYPE(port, val)                         AvHalI2cWriteField8(GSV2K2_TXPKT_MAP_ADDR(port), 0x1F, 0xFF, 0, val)
#define GSV2K2_TXPKT_set_AVI_IF_VER(port, val)                          AvHalI2cWriteField8(GSV2K2_TXPKT_MAP_ADDR(port), 0x20, 0xFF, 0, val)
#define GSV2K2_TXPKT_set_AVI_IF_LEN(port, val)                          AvHalI2cWriteField8(GSV2K2_TXPKT_MAP_ADDR(port), 0x21, 0xFF, 0, val)
#define GSV2K2_TXPKT_get_AUD_IF_CA7_0(port, pval)                       AvHalI2cReadField8(GSV2K2_TXPKT_MAP_ADDR(port), 0x64, 0xFF, 0, pval)
#define GSV2K2_TXPKT_set_AUD_IF_CA7_0(port, val)                        AvHalI2cWriteField8(GSV2K2_TXPKT_MAP_ADDR(port), 0x64, 0xFF, 0, val)
#define GSV2K2_TXPKT_get_TX_PKT_UPDATE(port, pval)                      AvHalI2cReadField32BE(GSV2K2_TXPKT_MAP_ADDR(port), 0xFB, 0x7F, 0xFF, 0, 2, pval)
#define GSV2K2_TXPKT_set_TX_PKT_UPDATE(port, val)                       AvHalI2cWriteField32BE(GSV2K2_TXPKT_MAP_ADDR(port), 0xFB, 0x7F, 0xFF, 0, 2, val)
#endif
