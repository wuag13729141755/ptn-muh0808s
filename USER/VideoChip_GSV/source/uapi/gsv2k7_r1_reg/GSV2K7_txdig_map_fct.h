#ifndef GSV2K7_TXDIG_MAP_FCT_H
#define GSV2K7_TXDIG_MAP_FCT_H
#define GSV2K7_TXDIG_set_TX_AUDIO_MODE(port, val)                       AvHalI2cWriteField8(GSV2K7_TXDIG_MAP_ADDR(port), 0x00, 0xE0, 5, val)
#define GSV2K7_TXDIG_set_TX_AUDIO_ENCODE_STYLE(port, val)               AvHalI2cWriteField8(GSV2K7_TXDIG_MAP_ADDR(port), 0x00, 0x18, 3, val)
#define GSV2K7_TXDIG_set_TX_AUDIO_MCLK_FS_RATIO(port, val)              AvHalI2cWriteField8(GSV2K7_TXDIG_MAP_ADDR(port), 0x00, 0x6, 1, val)
#define GSV2K7_TXDIG_set_TX_AUDIO_SPDIF_ENABLE(port, val)               AvHalI2cWriteField8(GSV2K7_TXDIG_MAP_ADDR(port), 0x01, 0x80, 0x7, val)
#define GSV2K7_TXDIG_set_TX_AUDIO_EXT_MCLK_ENABLE(port, val)            AvHalI2cWriteField8(GSV2K7_TXDIG_MAP_ADDR(port), 0x01, 0x40, 0x6, val)
#define GSV2K7_TXDIG_set_TX_AUDIO_CS_OVERRIDE(port, val)                AvHalI2cWriteField8(GSV2K7_TXDIG_MAP_ADDR(port), 0x02, 0x40, 0x6, val)
#define GSV2K7_TXDIG_set_TX_AUDIO_I2S_BLOCK_ENABLE(port, val)           AvHalI2cWriteField8(GSV2K7_TXDIG_MAP_ADDR(port), 0x02, 0x3C, 2, val)
#define GSV2K7_TXDIG_set_TX_AUDIO_I2S_FORMAT(port, val)                 AvHalI2cWriteField8(GSV2K7_TXDIG_MAP_ADDR(port), 0x02, 0x3, 0, val)
#define GSV2K7_TXDIG_get_TX_AUDIO_SPDIF_SF_RB(port, pval)               AvHalI2cReadField8(GSV2K7_TXDIG_MAP_ADDR(port), 0x04, 0xF, 0, pval)
#define GSV2K7_TXDIG_set_TX_AUDIO_PAPB_SYNC(port, val)                  AvHalI2cWriteField8(GSV2K7_TXDIG_MAP_ADDR(port), 0x10, 0x40, 0x6, val)
#define GSV2K7_TXDIG_set_TX_AUDIO_CS_FORMAT_EXTRA(port, val)            AvHalI2cWriteField8(GSV2K7_TXDIG_MAP_ADDR(port), 0x15, 0x38, 3, val)
#define GSV2K7_TXDIG_set_TX_AUDIO_CS_COPYRIGHT(port, val)               AvHalI2cWriteField8(GSV2K7_TXDIG_MAP_ADDR(port), 0x15, 0x4, 0x2, val)
#define GSV2K7_TXDIG_set_TX_AUDIO_CS_FORMAT(port, val)                  AvHalI2cWriteField8(GSV2K7_TXDIG_MAP_ADDR(port), 0x15, 0x3, 0, val)
#define GSV2K7_TXDIG_set_TX_AUDIO_CS_CATEGORY_CODE(port, val)           AvHalI2cWriteField8(GSV2K7_TXDIG_MAP_ADDR(port), 0x16, 0xFF, 0, val)
#define GSV2K7_TXDIG_set_TX_AUDIO_CS_SOURCE_NUMBER(port, val)           AvHalI2cWriteField8(GSV2K7_TXDIG_MAP_ADDR(port), 0x17, 0xF, 0, val)
#define GSV2K7_TXDIG_set_TX_AUDIO_CS_MODE(port, val)                    AvHalI2cWriteField8(GSV2K7_TXDIG_MAP_ADDR(port), 0x18, 0x30, 4, val)
#define GSV2K7_TXDIG_set_TX_AUDIO_I2S_MAN_SF(port, val)                 AvHalI2cWriteField8(GSV2K7_TXDIG_MAP_ADDR(port), 0x18, 0xF, 0, val)
#define GSV2K7_TXDIG_set_TX_AUDIO_CS_WORD_LENGTH(port, val)             AvHalI2cWriteField8(GSV2K7_TXDIG_MAP_ADDR(port), 0x19, 0xF, 0, val)
#define GSV2K7_TXDIG_set_TX_AUDIO_LAYOUT_MAN_VALUE(port, val)           AvHalI2cWriteField8(GSV2K7_TXDIG_MAP_ADDR(port), 0x1A, 0x40, 0x6, val)
#define GSV2K7_TXDIG_get_TX_AUDIO_MUTE(port, pval)                      AvHalI2cReadField8(GSV2K7_TXDIG_MAP_ADDR(port), 0x1A, 0x20, 0x5, pval)
#define GSV2K7_TXDIG_set_TX_AUDIO_MUTE(port, val)                       AvHalI2cWriteField8(GSV2K7_TXDIG_MAP_ADDR(port), 0x1A, 0x20, 0x5, val)
#define GSV2K7_TXDIG_set_TX_MUX_MODE_AVMUTE_MAN(port, val)              AvHalI2cWriteField8(GSV2K7_TXDIG_MAP_ADDR(port), 0x1E, 0x80, 0x7, val)
#define GSV2K7_TXDIG_set_TX_MUX_MODE_AVMUTE_MAN_EN(port, val)           AvHalI2cWriteField8(GSV2K7_TXDIG_MAP_ADDR(port), 0x1E, 0x40, 0x6, val)
#define GSV2K7_TXDIG_set_MAN_POL_HS(port, val)                          AvHalI2cWriteField8(GSV2K7_TXDIG_MAP_ADDR(port), 0x1F, 0x2, 0x1, val)
#define GSV2K7_TXDIG_set_MAN_POL_VS(port, val)                          AvHalI2cWriteField8(GSV2K7_TXDIG_MAP_ADDR(port), 0x1F, 0x1, 0x0, val)
#define GSV2K7_TXDIG_set_TX_VIDEO_INPUT_FORMAT(port, val)               AvHalI2cWriteField8(GSV2K7_TXDIG_MAP_ADDR(port), 0x20, 0xF0, 4, val)
#define GSV2K7_TXDIG_get_TX_VIDEO_INPUT_CS(port, pval)                  AvHalI2cReadField8(GSV2K7_TXDIG_MAP_ADDR(port), 0x20, 0x8, 0x3, pval)
#define GSV2K7_TXDIG_set_TX_VIDEO_INPUT_CS(port, val)                   AvHalI2cWriteField8(GSV2K7_TXDIG_MAP_ADDR(port), 0x20, 0x8, 0x3, val)
#define GSV2K7_TXDIG_get_TX_VIDEO_OUTPUT_FORMAT(port, pval)             AvHalI2cReadField8(GSV2K7_TXDIG_MAP_ADDR(port), 0x21, 0xC0, 6, pval)
#define GSV2K7_TXDIG_set_TX_VIDEO_OUTPUT_FORMAT(port, val)              AvHalI2cWriteField8(GSV2K7_TXDIG_MAP_ADDR(port), 0x21, 0xC0, 6, val)
#define GSV2K7_TXDIG_set_TX_VIDEO_ASPECT_RATIO(port, val)               AvHalI2cWriteField8(GSV2K7_TXDIG_MAP_ADDR(port), 0x22, 0x2, 0x1, val)
#define GSV2K7_TXDIG_set_TX_VIDEO_PR_MODE(port, val)                    AvHalI2cWriteField8(GSV2K7_TXDIG_MAP_ADDR(port), 0x23, 0xC0, 6, val)
#define GSV2K7_TXDIG_set_TX_VIDEO_TMDS_CLK_RATIO_MAN(port, val)         AvHalI2cWriteField8(GSV2K7_TXDIG_MAP_ADDR(port), 0x23, 0x30, 4, val)
#define GSV2K7_TXDIG_set_TX_VIDEO_PR_TO_RX_MAN(port, val)               AvHalI2cWriteField8(GSV2K7_TXDIG_MAP_ADDR(port), 0x23, 0xC, 2, val)
#define GSV2K7_TXDIG_get_TX_VIDEO_CSC_ENABLE(port, pval)                AvHalI2cReadField8(GSV2K7_TXDIG_MAP_ADDR(port), 0x2B, 0x80, 0x7, pval)
#define GSV2K7_TXDIG_set_TX_VIDEO_CSC_ENABLE(port, val)                 AvHalI2cWriteField8(GSV2K7_TXDIG_MAP_ADDR(port), 0x2B, 0x80, 0x7, val)
#define GSV2K7_TXDIG_set_TX_VIDEO_CSC_SCALING_FACTOR(port, val)         AvHalI2cWriteField8(GSV2K7_TXDIG_MAP_ADDR(port), 0x2B, 0x60, 5, val)
#define GSV2K7_TXDIG_get_TX_BLACK_VIDEO_EN(port, pval)                  AvHalI2cReadField8(GSV2K7_TXDIG_MAP_ADDR(port), 0x4E, 0x8, 0x3, pval)
#define GSV2K7_TXDIG_set_TX_BLACK_VIDEO_EN(port, val)                   AvHalI2cWriteField8(GSV2K7_TXDIG_MAP_ADDR(port), 0x4E, 0x8, 0x3, val)
#define GSV2K7_TXDIG_set_TX_AUDIOFIFO_RESET(port, val)                  AvHalI2cWriteField8(GSV2K7_TXDIG_MAP_ADDR(port), 0x4F, 0x1, 0x0, val)
#define GSV2K7_TXDIG_set_TX_PKT_MPEG_ENABLE(port, val)                  AvHalI2cWriteField8(GSV2K7_TXDIG_MAP_ADDR(port), 0x50, 0x10, 0x4, val)
#define GSV2K7_TXDIG_set_TX_PKT_AUDIO_INFO_ENABLE(port, val)            AvHalI2cWriteField8(GSV2K7_TXDIG_MAP_ADDR(port), 0x50, 0x8, 0x3, val)
#define GSV2K7_TXDIG_set_TX_PKT_SPDIF_ENABLE(port, val)                 AvHalI2cWriteField8(GSV2K7_TXDIG_MAP_ADDR(port), 0x50, 0x4, 0x2, val)
#define GSV2K7_TXDIG_set_TX_PKT_AVIIF_PKT_ENABLE(port, val)             AvHalI2cWriteField8(GSV2K7_TXDIG_MAP_ADDR(port), 0x50, 0x2, 0x1, val)
#define GSV2K7_TXDIG_set_TX_PKT_VSIF_ENABLE(port, val)                  AvHalI2cWriteField8(GSV2K7_TXDIG_MAP_ADDR(port), 0x50, 0x1, 0x0, val)
#define GSV2K7_TXDIG_set_TX_PKT_USER1_ENABLE(port, val)                 AvHalI2cWriteField8(GSV2K7_TXDIG_MAP_ADDR(port), 0x51, 0x80, 0x7, val)
#define GSV2K7_TXDIG_set_TX_PKT_USER0_ENABLE(port, val)                 AvHalI2cWriteField8(GSV2K7_TXDIG_MAP_ADDR(port), 0x51, 0x40, 0x6, val)
#define GSV2K7_TXDIG_set_TX_PKT_GM_ENABLE(port, val)                    AvHalI2cWriteField8(GSV2K7_TXDIG_MAP_ADDR(port), 0x51, 0x20, 0x5, val)
#define GSV2K7_TXDIG_set_TX_PKT_ISRC_ENABLE(port, val)                  AvHalI2cWriteField8(GSV2K7_TXDIG_MAP_ADDR(port), 0x51, 0x10, 0x4, val)
#define GSV2K7_TXDIG_set_TX_PKT_ACP_ENABLE(port, val)                   AvHalI2cWriteField8(GSV2K7_TXDIG_MAP_ADDR(port), 0x51, 0x8, 0x3, val)
#define GSV2K7_TXDIG_set_TX_PKT_GC_ENABLE(port, val)                    AvHalI2cWriteField8(GSV2K7_TXDIG_MAP_ADDR(port), 0x51, 0x4, 0x2, val)
#define GSV2K7_TXDIG_get_TX_PKT_AUDIO_SAMPLE_ENABLE(port, pval)         AvHalI2cReadField8(GSV2K7_TXDIG_MAP_ADDR(port), 0x51, 0x2, 0x1, pval)
#define GSV2K7_TXDIG_set_TX_PKT_AUDIO_SAMPLE_ENABLE(port, val)          AvHalI2cWriteField8(GSV2K7_TXDIG_MAP_ADDR(port), 0x51, 0x2, 0x1, val)
#define GSV2K7_TXDIG_set_TX_PKT_N_CTS_ENABLE(port, val)                 AvHalI2cWriteField8(GSV2K7_TXDIG_MAP_ADDR(port), 0x51, 0x1, 0x0, val)
#define GSV2K7_TXDIG_set_TX_GC_CLEAR_AVMUTE(port, val)                  AvHalI2cWriteField8(GSV2K7_TXDIG_MAP_ADDR(port), 0x52, 0x80, 0x7, val)
#define GSV2K7_TXDIG_set_TX_GC_SET_AVMUTE(port, val)                    AvHalI2cWriteField8(GSV2K7_TXDIG_MAP_ADDR(port), 0x52, 0x40, 0x6, val)
#define GSV2K7_TXDIG_get_TX_GC_CD(port, pval)                           AvHalI2cReadField8(GSV2K7_TXDIG_MAP_ADDR(port), 0x53, 0xF, 0, pval)
#define GSV2K7_TXDIG_set_TX_GC_CD(port, val)                            AvHalI2cWriteField8(GSV2K7_TXDIG_MAP_ADDR(port), 0x53, 0xF, 0, val)
#define GSV2K7_TXDIG_set_TX_HDMI_MODE_OVERRIDE(port, val)               AvHalI2cWriteField8(GSV2K7_TXDIG_MAP_ADDR(port), 0xA5, 0x80, 0x7, val)
#define GSV2K7_TXDIG_set_TX_HDMI_MODE_MAN(port, val)                    AvHalI2cWriteField8(GSV2K7_TXDIG_MAP_ADDR(port), 0xA5, 0x40, 0x6, val)
#define GSV2K7_TXDIG_set_DCFIFO_RESET(port, val)                        AvHalI2cWriteField8(GSV2K7_TXDIG_MAP_ADDR(port), 0xA6, 0x20, 0x5, val)
#define GSV2K7_TXDIG_set_DCFIFO_RESET_ON_LOCK(port, val)                AvHalI2cWriteField8(GSV2K7_TXDIG_MAP_ADDR(port), 0xA6, 0x10, 0x4, val)
#define GSV2K7_TXDIG_set_TDM_DESER_NUM_PINS(port, val)                  AvHalI2cWriteField8(GSV2K7_TXDIG_MAP_ADDR(port), 0xB0, 0xC, 2, val)
#define GSV2K7_TXDIG_set_TDM_DESER_NUM_SLOT(port, val)                  AvHalI2cWriteField8(GSV2K7_TXDIG_MAP_ADDR(port), 0xB3, 0x3F, 0, val)
#define GSV2K7_TXDIG_set_TDM_DESER_NUM_CH(port, val)                    AvHalI2cWriteField8(GSV2K7_TXDIG_MAP_ADDR(port), 0xB4, 0x3F, 0, val)
#define GSV2K7_TXDIG_set_DSD_EN(port, val)                              AvHalI2cWriteField32BE(GSV2K7_TXDIG_MAP_ADDR(port), 0xDA, 0xFF, 0xFF, 0, 4, val)
#endif
