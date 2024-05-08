#ifndef GSV5K1_DSC_MAP_FCT_H
#define GSV5K1_DSC_MAP_FCT_H
#define GSV5K1_DSC_get_DSC_VIC(port, pval)                            AvHalI2cReadField8(GSV5K1_DSC_MAP_ADDR(port), 0x40, 0xFF, 0, pval)
#define GSV5K1_DSC_set_DSC_VIC(port, val)                             AvHalI2cWriteField8(GSV5K1_DSC_MAP_ADDR(port), 0x40, 0xFF, 0, val)
#define GSV5K1_DSC_get_DSC_VIC_MANUAL_EN(port, pval)                  AvHalI2cReadField8(GSV5K1_DSC_MAP_ADDR(port), 0x41, 0x8, 0x3, pval)
#define GSV5K1_DSC_set_DSC_VIC_MANUAL_EN(port, val)                   AvHalI2cWriteField8(GSV5K1_DSC_MAP_ADDR(port), 0x41, 0x8, 0x3, val)
#define GSV5K1_DSC_set_DSC_SLICE_MULTIPLEX_DELAY_EN(port, val)        AvHalI2cWriteField8(GSV5K1_DSC_MAP_ADDR(port), 0x41, 0x2, 0x1, val)
#define GSV5K1_DSC_get_DSC_H_WAVELET_HALF_EN(port, pval)              AvHalI2cReadField8(GSV5K1_DSC_MAP_ADDR(port), 0x5B, 0x1, 0x0, pval)
#define GSV5K1_DSC_set_DSC_H_WAVELET_HALF_EN(port, val)               AvHalI2cWriteField8(GSV5K1_DSC_MAP_ADDR(port), 0x5B, 0x1, 0x0, val)
#define GSV5K1_DSC_set_DSC_INIT_OUT_DELAY_EN(port, val)               AvHalI2cWriteField8(GSV5K1_DSC_MAP_ADDR(port), 0x63, 0x4, 0x2, val)
#define GSV5K1_DSC_set_DSC_QP_CLEAR_EN(port, val)                     AvHalI2cWriteField8(GSV5K1_DSC_MAP_ADDR(port), 0x64, 0x40, 0x6, val)
#define GSV5K1_DSC_set_DSC_SLICE_CTL_MAN_EN(port, val)                AvHalI2cWriteField8(GSV5K1_DSC_MAP_ADDR(port), 0x65, 0x80, 0x7, val)
#define GSV5K1_DSC_set_DSC_DECODER_LAST_LINE_REP_EN(port, val)        AvHalI2cWriteField8(GSV5K1_DSC_MAP_ADDR(port), 0x68, 0x80, 0x7, val)
#define GSV5K1_DSC_set_DSC_SEQ_OUTPUT_MAN_EN(port, val)               AvHalI2cWriteField8(GSV5K1_DSC_MAP_ADDR(port), 0x69, 0x80, 0x7, val)
#define GSV5K1_DSC_set_DSC_RC_CTL_MAN_EN(port, val)                   AvHalI2cWriteField8(GSV5K1_DSC_MAP_ADDR(port), 0x70, 0x80, 0x7, val)
#define GSV5K1_DSC_set_DSC_ENCODER_DELAY_EN(port, val)                AvHalI2cWriteField8(GSV5K1_DSC_MAP_ADDR(port), 0x83, 0x1, 0x0, val)
#define GSV5K1_DSC_get_DSC_MEM_EMAB(port, pval)                       AvHalI2cReadField8(GSV5K1_DSC_MAP_ADDR(port), 0xF5, 0x38, 3, pval)
#define GSV5K1_DSC_set_DSC_MEM_EMAB(port, val)                        AvHalI2cWriteField8(GSV5K1_DSC_MAP_ADDR(port), 0xF5, 0x38, 3, val)
#endif
