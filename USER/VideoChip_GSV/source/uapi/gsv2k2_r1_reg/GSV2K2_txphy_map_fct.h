#ifndef GSV2K2_TXPHY_MAP_FCT_H
#define GSV2K2_TXPHY_MAP_FCT_H
#define GSV2K2_TXPHY_set_TX_RR_CAPABLE(port, val)                       AvHalI2cWriteField8(GSV2K2_TXPHY_MAP_ADDR(port), 0x01, 0x2, 0x1, val)
#define GSV2K2_TXPHY_get_TX_SCDC_EN(port, pval)                         AvHalI2cReadField8(GSV2K2_TXPHY_MAP_ADDR(port), 0x01, 0x1, 0x0, pval)
#define GSV2K2_TXPHY_set_TX_SCDC_EN(port, val)                          AvHalI2cWriteField8(GSV2K2_TXPHY_MAP_ADDR(port), 0x01, 0x1, 0x0, val)
#define GSV2K2_TXPHY_set_RX_RR_CAPABLE(port, val)                       AvHalI2cWriteField8(GSV2K2_TXPHY_MAP_ADDR(port), 0x02, 0x2, 0x1, val)
#define GSV2K2_TXPHY_set_RX_SCDC_PRESENT(port, val)                     AvHalI2cWriteField8(GSV2K2_TXPHY_MAP_ADDR(port), 0x02, 0x1, 0x0, val)
#define GSV2K2_TXPHY_get_EDID_ANALYZE_DONE(port, pval)                  AvHalI2cReadField8(GSV2K2_TXPHY_MAP_ADDR(port), 0x03, 0x1, 0x0, pval)
#define GSV2K2_TXPHY_set_EDID_ANALYZE_DONE(port, val)                   AvHalI2cWriteField8(GSV2K2_TXPHY_MAP_ADDR(port), 0x03, 0x1, 0x0, val)
#define GSV2K2_TXPHY_set_TX_CLR_UPDATE_FLAG_EN(port, val)               AvHalI2cWriteField8(GSV2K2_TXPHY_MAP_ADDR(port), 0x07, 0x80, 0x7, val)
#define GSV2K2_TXPHY_set_TX_CLR_UPDATE_FLAG(port, val)                  AvHalI2cWriteField8(GSV2K2_TXPHY_MAP_ADDR(port), 0x08, 0xFF, 0, val)
#define GSV2K2_TXPHY_get_RX_SCDC_UPDATE_0(port, pval)                   AvHalI2cReadField8(GSV2K2_TXPHY_MAP_ADDR(port), 0x11, 0xFF, 0, pval)
#define GSV2K2_TXPHY_get_RX_DDC_UDP_DONE(port, pval)                    AvHalI2cReadField8(GSV2K2_TXPHY_MAP_ADDR(port), 0x1C, 0x1, 0x0, pval)
#define GSV2K2_TXPHY_get_RX_DDC_UDP_READ_DATA(port, pval)               AvHalI2cReadField8(GSV2K2_TXPHY_MAP_ADDR(port), 0x1D, 0xFF, 0, pval)
#define GSV2K2_TXPHY_set_RX_DDC_UDP_DEV_ADDR(port, val)                 AvHalI2cWriteField8(GSV2K2_TXPHY_MAP_ADDR(port), 0x1E, 0xFF, 0, val)
#define GSV2K2_TXPHY_set_RX_DDC_UDP_SUB_ADDR(port, val)                 AvHalI2cWriteField8(GSV2K2_TXPHY_MAP_ADDR(port), 0x20, 0xFF, 0, val)
#define GSV2K2_TXPHY_set_RX_DDC_UDP_WRITE_DATA(port, val)               AvHalI2cWriteField8(GSV2K2_TXPHY_MAP_ADDR(port), 0x21, 0xFF, 0, val)
#define GSV2K2_TXPHY_set_RX_DDC_UDP_ACCESS_REQ(port, val)               AvHalI2cWriteField8(GSV2K2_TXPHY_MAP_ADDR(port), 0x22, 0x1, 0x0, val)
#define GSV2K2_TXPHY_set_TX_HDCP1P4_CLR_ERR(port, val)                  AvHalI2cWriteField8(GSV2K2_TXPHY_MAP_ADDR(port), 0x97, 0x80, 0x7, val)
#define GSV2K2_TXPHY_set_TX_HDCP1P4_CLR_BKSV_FLAG(port, val)            AvHalI2cWriteField8(GSV2K2_TXPHY_MAP_ADDR(port), 0x97, 0x40, 0x6, val)
#define GSV2K2_TXPHY_set_RD_CPU_PC_TRIGGER(port, val)                   AvHalI2cWriteField8(GSV2K2_TXPHY_MAP_ADDR(port), 0xAA, 0x1, 0x0, val)
#define GSV2K2_TXPHY_get_RB_CPU_PC_CAPTURED(port, pval)                 AvHalI2cReadField32BE(GSV2K2_TXPHY_MAP_ADDR(port), 0xAB, 0x7, 0xFF, 0, 2, pval)
#define GSV2K2_TXPHY_get_TX_HDCP1P4_DESIRED(port, pval)                 AvHalI2cReadField8(GSV2K2_TXPHY_MAP_ADDR(port), 0xB0, 0x80, 0x7, pval)
#define GSV2K2_TXPHY_set_TX_HDCP1P4_DESIRED(port, val)                  AvHalI2cWriteField8(GSV2K2_TXPHY_MAP_ADDR(port), 0xB0, 0x80, 0x7, val)
#define GSV2K2_TXPHY_set_TX_HDCP1P4_ENC_EN(port, val)                   AvHalI2cWriteField8(GSV2K2_TXPHY_MAP_ADDR(port), 0xB0, 0x10, 0x4, val)
#define GSV2K2_TXPHY_get_TX_HDCP1P4_BKSV0(port, pval)                   AvHalI2cReadField8(GSV2K2_TXPHY_MAP_ADDR(port), 0xBF, 0xFF, 0, pval)
#define GSV2K2_TXPHY_get_TX_HDCP1P4_BKSV1(port, pval)                   AvHalI2cReadField8(GSV2K2_TXPHY_MAP_ADDR(port), 0xC0, 0xFF, 0, pval)
#define GSV2K2_TXPHY_get_TX_HDCP1P4_BKSV2(port, pval)                   AvHalI2cReadField8(GSV2K2_TXPHY_MAP_ADDR(port), 0xC1, 0xFF, 0, pval)
#define GSV2K2_TXPHY_get_TX_HDCP1P4_BKSV3(port, pval)                   AvHalI2cReadField8(GSV2K2_TXPHY_MAP_ADDR(port), 0xC2, 0xFF, 0, pval)
#define GSV2K2_TXPHY_get_TX_HDCP1P4_BKSV4(port, pval)                   AvHalI2cReadField8(GSV2K2_TXPHY_MAP_ADDR(port), 0xC3, 0xFF, 0, pval)
#define GSV2K2_TXPHY_set_TX_DDC_READ_EDID_RAM_PART(port, val)           AvHalI2cWriteField8(GSV2K2_TXPHY_MAP_ADDR(port), 0xC4, 0xFF, 0, val)
#define GSV2K2_TXPHY_get_TX_HDCP1P4_ERROR_FLAG(port, pval)              AvHalI2cReadField8(GSV2K2_TXPHY_MAP_ADDR(port), 0xC5, 0x80, 0x7, pval)
#define GSV2K2_TXPHY_get_TX_HDCP1P4_RUNNING_RB(port, pval)              AvHalI2cReadField8(GSV2K2_TXPHY_MAP_ADDR(port), 0xC5, 0x20, 0x5, pval)
#define GSV2K2_TXPHY_get_TX_EDID_READY_RB(port, pval)                   AvHalI2cReadField8(GSV2K2_TXPHY_MAP_ADDR(port), 0xC5, 0x10, 0x4, pval)
#define GSV2K2_TXPHY_get_TX_EDID_DDC_ERROR_RB(port, pval)               AvHalI2cReadField8(GSV2K2_TXPHY_MAP_ADDR(port), 0xC5, 0x8, 0x3, pval)
#define GSV2K2_TXPHY_get_TX_HDCP1P4_BKSV_READ_FLAG(port, pval)          AvHalI2cReadField8(GSV2K2_TXPHY_MAP_ADDR(port), 0xC7, 0x80, 0x7, pval)
#define GSV2K2_TXPHY_get_TX_HDCP1P4_BKSV_COUNT(port, pval)              AvHalI2cReadField8(GSV2K2_TXPHY_MAP_ADDR(port), 0xC7, 0x7F, 0, pval)
#define GSV2K2_TXPHY_get_TX_HDCP1P4_STATE_RB(port, pval)                AvHalI2cReadField8(GSV2K2_TXPHY_MAP_ADDR(port), 0xC8, 0xF, 0, pval)
#define GSV2K2_TXPHY_set_TX_EDID_RE_READ(port, val)                     AvHalI2cWriteField8(GSV2K2_TXPHY_MAP_ADDR(port), 0xC9, 0x10, 0x4, val)
#define GSV2K2_TXPHY_get_TX_EDID_TRY_TIMES(port, pval)                  AvHalI2cReadField8(GSV2K2_TXPHY_MAP_ADDR(port), 0xC9, 0xF, 0, pval)
#define GSV2K2_TXPHY_set_TX_EDID_TRY_TIMES(port, val)                   AvHalI2cWriteField8(GSV2K2_TXPHY_MAP_ADDR(port), 0xC9, 0xF, 0, val)
#define GSV2K2_TXPHY_get_TX_HDCP2P2_CAPABILITY_RB(port, pval)           AvHalI2cReadField8(GSV2K2_TXPHY_MAP_ADDR(port), 0xCD, 0x2, 0x1, pval)
#define GSV2K2_TXPHY_get_TX_HDCP2P2_CHECKED(port, pval)                 AvHalI2cReadField8(GSV2K2_TXPHY_MAP_ADDR(port), 0xCD, 0x1, 0x0, pval)
#define GSV2K2_TXPHY_set_TX_EN_STERM_LANE2(port, val)                   AvHalI2cWriteField8(GSV2K2_TXPHY_MAP_ADDR(port), 0xD1, 0x80, 0x7, val)
#define GSV2K2_TXPHY_set_TX_EN_STERM_LANE1(port, val)                   AvHalI2cWriteField8(GSV2K2_TXPHY_MAP_ADDR(port), 0xD1, 0x40, 0x6, val)
#define GSV2K2_TXPHY_set_TX_EN_STERM_LANE0(port, val)                   AvHalI2cWriteField8(GSV2K2_TXPHY_MAP_ADDR(port), 0xD1, 0x20, 0x5, val)
#define GSV2K2_TXPHY_set_TX_EN_STERM_CLOCK(port, val)                   AvHalI2cWriteField8(GSV2K2_TXPHY_MAP_ADDR(port), 0xD1, 0x10, 0x4, val)
#define GSV2K2_TXPHY_set_TX_HPD_MAN_VALUE(port, val)                    AvHalI2cWriteField8(GSV2K2_TXPHY_MAP_ADDR(port), 0xEF, 0x80, 0x7, val)
#define GSV2K2_TXPHY_get_TX_HPD_SRC_SEL(port, pval)                     AvHalI2cReadField8(GSV2K2_TXPHY_MAP_ADDR(port), 0xEF, 0x3, 0, pval)
#define GSV2K2_TXPHY_set_TX_HPD_SRC_SEL(port, val)                      AvHalI2cWriteField8(GSV2K2_TXPHY_MAP_ADDR(port), 0xEF, 0x3, 0, val)
#define GSV2K2_TXPHY_get_TX_SCR_EN_MAN(port, pval)                      AvHalI2cReadField8(GSV2K2_TXPHY_MAP_ADDR(port), 0xF1, 0x80, 0x7, pval)
#define GSV2K2_TXPHY_set_TX_SCR_EN_MAN(port, val)                       AvHalI2cWriteField8(GSV2K2_TXPHY_MAP_ADDR(port), 0xF1, 0x80, 0x7, val)
#define GSV2K2_TXPHY_get_TX_SCR_EN_MAN_EN(port, pval)                   AvHalI2cReadField8(GSV2K2_TXPHY_MAP_ADDR(port), 0xF1, 0x40, 0x6, pval)
#define GSV2K2_TXPHY_set_TX_SCR_EN_MAN_EN(port, val)                    AvHalI2cWriteField8(GSV2K2_TXPHY_MAP_ADDR(port), 0xF1, 0x40, 0x6, val)
#define GSV2K2_TXPHY_get_TX_HDMI2_MODE_MAN(port, pval)                  AvHalI2cReadField8(GSV2K2_TXPHY_MAP_ADDR(port), 0xF1, 0x20, 0x5, pval)
#define GSV2K2_TXPHY_set_TX_HDMI2_MODE_MAN(port, val)                   AvHalI2cWriteField8(GSV2K2_TXPHY_MAP_ADDR(port), 0xF1, 0x20, 0x5, val)
#define GSV2K2_TXPHY_get_TX_HDMI2_MODE_MAN_EN(port, pval)               AvHalI2cReadField8(GSV2K2_TXPHY_MAP_ADDR(port), 0xF1, 0x10, 0x4, pval)
#define GSV2K2_TXPHY_set_TX_HDMI2_MODE_MAN_EN(port, val)                AvHalI2cWriteField8(GSV2K2_TXPHY_MAP_ADDR(port), 0xF1, 0x10, 0x4, val)
#define GSV2K2_TXPHY_get_HPD_RAW_STATE(port, pval)                      AvHalI2cReadField8(GSV2K2_TXPHY_MAP_ADDR(port), 0xF1, 0x1, 0x0, pval)
#define GSV2K2_TXPHY_get_TX_TMDS_CLK_DRIVER_ENABLE(port, pval)          AvHalI2cReadField8(GSV2K2_TXPHY_MAP_ADDR(port), 0xF2, 0x80, 0x7, pval)
#define GSV2K2_TXPHY_set_TMDS_SRST_MAN_EN(port, val)                    AvHalI2cWriteField8(GSV2K2_TXPHY_MAP_ADDR(port), 0xF4, 0x20, 0x5, val)
#define GSV2K2_TXPHY_set_TMDS_SRST_MAN(port, val)                       AvHalI2cWriteField8(GSV2K2_TXPHY_MAP_ADDR(port), 0xF4, 0x10, 0x4, val)
#endif
