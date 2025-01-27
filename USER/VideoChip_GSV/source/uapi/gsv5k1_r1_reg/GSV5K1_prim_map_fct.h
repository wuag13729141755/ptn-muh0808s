#ifndef GSV5K1_PRIM_MAP_FCT_H
#define GSV5K1_PRIM_MAP_FCT_H
#define GSV5K1_PRIM_set_RX1_CORE_SEL(port, val)                        AvHalI2cWriteField8(GSV5K1_PRIM_MAP_ADDR(port), 0x01, 0x70, 4, val)
#define GSV5K1_PRIM_set_RX2_CORE_EN(port, val)                         AvHalI2cWriteField8(GSV5K1_PRIM_MAP_ADDR(port), 0x01, 0x8, 0x3, val)
#define GSV5K1_PRIM_set_RX2_CORE_SEL(port, val)                        AvHalI2cWriteField8(GSV5K1_PRIM_MAP_ADDR(port), 0x01, 0x7, 0, val)
#define GSV5K1_PRIM_set_TXB_PWR_DN(port, val)                          AvHalI2cWriteField8(GSV5K1_PRIM_MAP_ADDR(port), 0x02, 0x20, 0x5, val)
#define GSV5K1_PRIM_set_TXA_PWR_DN(port, val)                          AvHalI2cWriteField8(GSV5K1_PRIM_MAP_ADDR(port), 0x02, 0x10, 0x4, val)
#define GSV5K1_PRIM_set_RX1_TMDS_BYPASS_EN(port, val)                  AvHalI2cWriteField8(GSV5K1_PRIM_MAP_ADDR(port), 0x03, 0x80, 0x7, val)
#define GSV5K1_PRIM_set_TX1_TMDS_BYPASS_EN(port, val)                  AvHalI2cWriteField8(GSV5K1_PRIM_MAP_ADDR(port), 0x03, 0x20, 0x5, val)
#define GSV5K1_PRIM_set_TX2_TMDS_BYPASS_EN(port, val)                  AvHalI2cWriteField8(GSV5K1_PRIM_MAP_ADDR(port), 0x03, 0x10, 0x4, val)
#define GSV5K1_PRIM_set_TX1_AUD_SRC_SEL(port, val)                     AvHalI2cWriteField8(GSV5K1_PRIM_MAP_ADDR(port), 0x08, 0xF0, 4, val)
#define GSV5K1_PRIM_set_TX2_AUD_SRC_SEL(port, val)                     AvHalI2cWriteField8(GSV5K1_PRIM_MAP_ADDR(port), 0x08, 0xF, 0, val)
#define GSV5K1_PRIM_set_AP1_OUT_SRC_SEL(port, val)                     AvHalI2cWriteField8(GSV5K1_PRIM_MAP_ADDR(port), 0x0A, 0xF0, 4, val)
#define GSV5K1_PRIM_get_ONLY_USE_RX1_EN(port, pval)                    AvHalI2cReadField8(GSV5K1_PRIM_MAP_ADDR(port), 0x13, 0x1, 0x0, pval)
#define GSV5K1_PRIM_set_ONLY_USE_RX1_EN(port, val)                     AvHalI2cWriteField8(GSV5K1_PRIM_MAP_ADDR(port), 0x13, 0x1, 0x0, val)
#define GSV5K1_PRIM_set_TXB_ARC_PWR_DN(port, val)                      AvHalI2cWriteField8(GSV5K1_PRIM_MAP_ADDR(port), 0x8F, 0x2, 0x1, val)
#define GSV5K1_PRIM_set_TXA_ARC_PWR_DN(port, val)                      AvHalI2cWriteField8(GSV5K1_PRIM_MAP_ADDR(port), 0x8F, 0x1, 0x0, val)
#define GSV5K1_PRIM_set_TXB_TERM_DET_PWR_DN(port, val)                 AvHalI2cWriteField8(GSV5K1_PRIM_MAP_ADDR(port), 0x90, 0x2, 0x1, val)
#define GSV5K1_PRIM_set_TXA_TERM_DET_PWR_DN(port, val)                 AvHalI2cWriteField8(GSV5K1_PRIM_MAP_ADDR(port), 0x90, 0x1, 0x0, val)
#define GSV5K1_PRIM_set_TXB_HDCP_EN(port, val)                         AvHalI2cWriteField8(GSV5K1_PRIM_MAP_ADDR(port), 0x93, 0x2, 0x1, val)
#define GSV5K1_PRIM_set_TXA_HDCP_EN(port, val)                         AvHalI2cWriteField8(GSV5K1_PRIM_MAP_ADDR(port), 0x93, 0x1, 0x0, val)
#define GSV5K1_PRIM_get_TXB_HDCP_CFG_MAN_EN(port, pval)                AvHalI2cReadField8(GSV5K1_PRIM_MAP_ADDR(port), 0x94, 0x2, 0x1, pval)
#define GSV5K1_PRIM_set_TXB_HDCP_CFG_MAN_EN(port, val)                 AvHalI2cWriteField8(GSV5K1_PRIM_MAP_ADDR(port), 0x94, 0x2, 0x1, val)
#define GSV5K1_PRIM_get_TXA_HDCP_CFG_MAN_EN(port, pval)                AvHalI2cReadField8(GSV5K1_PRIM_MAP_ADDR(port), 0x94, 0x1, 0x0, pval)
#define GSV5K1_PRIM_set_TXA_HDCP_CFG_MAN_EN(port, val)                 AvHalI2cWriteField8(GSV5K1_PRIM_MAP_ADDR(port), 0x94, 0x1, 0x0, val)
#define GSV5K1_PRIM_get_TXB_CFG_TO_HDCP2P2(port, pval)                 AvHalI2cReadField8(GSV5K1_PRIM_MAP_ADDR(port), 0x95, 0x2, 0x1, pval)
#define GSV5K1_PRIM_set_TXB_CFG_TO_HDCP2P2(port, val)                  AvHalI2cWriteField8(GSV5K1_PRIM_MAP_ADDR(port), 0x95, 0x2, 0x1, val)
#define GSV5K1_PRIM_get_TXA_CFG_TO_HDCP2P2(port, pval)                 AvHalI2cReadField8(GSV5K1_PRIM_MAP_ADDR(port), 0x95, 0x1, 0x0, pval)
#define GSV5K1_PRIM_set_TXA_CFG_TO_HDCP2P2(port, val)                  AvHalI2cWriteField8(GSV5K1_PRIM_MAP_ADDR(port), 0x95, 0x1, 0x0, val)
#define GSV5K1_PRIM_get_AUD1_OEN(port, pval)                           AvHalI2cReadField8(GSV5K1_PRIM_MAP_ADDR(port), 0x9D, 0xFF, 0, pval)
#define GSV5K1_PRIM_set_AUD1_OEN(port, val)                            AvHalI2cWriteField8(GSV5K1_PRIM_MAP_ADDR(port), 0x9D, 0xFF, 0, val)
#define GSV5K1_PRIM_set_RX_5V_DET_MAN_EN(port, val)                    AvHalI2cWriteField8(GSV5K1_PRIM_MAP_ADDR(port), 0xAF, 0x30, 4, val)
#define GSV5K1_PRIM_set_RX_HPD_MAN_VALUE(port, val)                    AvHalI2cWriteField8(GSV5K1_PRIM_MAP_ADDR(port), 0xB2, 0xF, 0, val)
#define GSV5K1_PRIM_set_RX_TMDSPLL_EN_OVERRIDE(port, val)              AvHalI2cWriteField8(GSV5K1_PRIM_MAP_ADDR(port), 0xB5, 0xF, 0, val)
#define GSV5K1_PRIM_set_RX_CHANNEL_EN_OVERRIDE(port, val)              AvHalI2cWriteField8(GSV5K1_PRIM_MAP_ADDR(port), 0xB6, 0xF, 0, val)
#define GSV5K1_PRIM_get_RX_RTERM_EN(port, pval)                        AvHalI2cReadField8(GSV5K1_PRIM_MAP_ADDR(port), 0xBB, 0xF, 0, pval)
#define GSV5K1_PRIM_set_RX_RTERM_EN(port, val)                         AvHalI2cWriteField8(GSV5K1_PRIM_MAP_ADDR(port), 0xBB, 0xF, 0, val)
#define GSV5K1_PRIM_get_RX_HDCP_CFG_MAN_EN(port, pval)                 AvHalI2cReadField8(GSV5K1_PRIM_MAP_ADDR(port), 0xBD, 0xC0, 6, pval)
#define GSV5K1_PRIM_set_RX_HDCP_CFG_MAN_EN(port, val)                  AvHalI2cWriteField8(GSV5K1_PRIM_MAP_ADDR(port), 0xBD, 0xC0, 6, val)
#define GSV5K1_PRIM_get_RX_CFG_TO_HDCP2P2(port, pval)                  AvHalI2cReadField8(GSV5K1_PRIM_MAP_ADDR(port), 0xBD, 0xC, 2, pval)
#define GSV5K1_PRIM_set_RX_CFG_TO_HDCP2P2(port, val)                   AvHalI2cWriteField8(GSV5K1_PRIM_MAP_ADDR(port), 0xBD, 0xC, 2, val)
#define GSV5K1_PRIM_set_RX1_HDCP_EN(port, val)                         AvHalI2cWriteField8(GSV5K1_PRIM_MAP_ADDR(port), 0xBE, 0x1, 0x0, val)
#define GSV5K1_PRIM_set_MAIN_RST(port, val)                            AvHalI2cWriteField8(GSV5K1_PRIM_MAP_ADDR(port), 0xFF, 0x80, 0x7, val)
#define GSV5K1_PRIM_set_VSP_RST(port, val)                             AvHalI2cWriteField8(GSV5K1_PRIM_MAP_ADDR(port), 0xFF, 0x1, 0x0, val)
#endif
