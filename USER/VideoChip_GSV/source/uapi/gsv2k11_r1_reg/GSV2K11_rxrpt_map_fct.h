#ifndef GSV2K11_RXRPT_MAP_FCT_H
#define GSV2K11_RXRPT_MAP_FCT_H
#define GSV2K11_RXRPT_set_RX_HDCP1P4_BCAPS(port, val)                    AvHalI2cWriteField8(GSV2K11_RXRPT_MAP_ADDR(port), 0x40, 0xFF, 0, val)
#define GSV2K11_RXRPT_set_RX_HDCP1P4_BSTATUS(port, val)                  AvHalI2cWriteField32BE(GSV2K11_RXRPT_MAP_ADDR(port), 0x41, 0xFF, 0xFF, 0, 2, val)
#define GSV2K11_RXRPT_set_RX_KSV_LIST_READY(port, val)                   AvHalI2cWriteField8(GSV2K11_RXRPT_MAP_ADDR(port), 0x69, 0x1, 0x0, val)
#define GSV2K11_RXRPT_set_RX_KSV_LIST_NUM(port, val)                     AvHalI2cWriteField8(GSV2K11_RXRPT_MAP_ADDR(port), 0x6A, 0x7F, 0, val)
#define GSV2K11_RXRPT_set_RX_EDID1_SPA_LOC_LSB(port, val)                AvHalI2cWriteField8(GSV2K11_RXRPT_MAP_ADDR(port), 0x71, 0xFF, 0, val)
#define GSV2K11_RXRPT_set_RXA_EDID_EN(port, val)                         AvHalI2cWriteField8(GSV2K11_RXRPT_MAP_ADDR(port), 0x74, 0x1, 0x0, val)
#define GSV2K11_RXRPT_set_RX_EDID_CHECKSUM_RECALC(port, val)             AvHalI2cWriteField8(GSV2K11_RXRPT_MAP_ADDR(port), 0x77, 0x4, 0x2, val)
#define GSV2K11_RXRPT_set_RX_KSV_LIST_READY_CLR(port, val)               AvHalI2cWriteField8(GSV2K11_RXRPT_MAP_ADDR(port), 0x78, 0x1, 0x0, val)
#define GSV2K11_RXRPT_set_HDCP_MAP_SEL(port, val)                        AvHalI2cWriteField8(GSV2K11_RXRPT_MAP_ADDR(port), 0x79, 0x7, 0, val)
#define GSV2K11_RXRPT_set_RX_EDID_RAM_PAGE_SEL(port, val)                AvHalI2cWriteField8(GSV2K11_RXRPT_MAP_ADDR(port), 0x7A, 0x70, 4, val)
#define GSV2K11_RXRPT_set_RX_EDID_RAM_SEL(port, val)                     AvHalI2cWriteField8(GSV2K11_RXRPT_MAP_ADDR(port), 0x7A, 0x3, 0, val)
#define GSV2K11_RXRPT_set_RXA_EDID_RAM_SEL(port, val)                    AvHalI2cWriteField8(GSV2K11_RXRPT_MAP_ADDR(port), 0x7D, 0x1, 0x0, val)
#endif
