#ifndef _GSV2K11_CFG_H
#define _GSV2K11_CFG_H

#include "gsv2k11.h"
#include "av_config.h"

/* Map Address is fixed, used for single chip platform */
/* if multiple chips are implemented,
   Device->DeviceAddress | MapAddress to generate the 32-bit address */
#define  gsv2k11Dev_PrimMapAddress            AvGenerateDeviceAddress(0x00,0x01,0xB0,0x00)
#define  gsv2k11Dev_SecMapAddress             AvGenerateDeviceAddress(0x00,0x01,0xB0,0x01)
#define  gsv2k11Dev_PllMapAddress             AvGenerateDeviceAddress(0x00,0x01,0xB0,0x02)
#define  gsv2k11Dev_APllMapAddress            AvGenerateDeviceAddress(0x00,0x01,0xB0,0x05)
#define  gsv2k11Dev_PPllMapAddress            AvGenerateDeviceAddress(0x00,0x01,0xB0,0x06)
#define  gsv2k11Dev_IntMapAddress             AvGenerateDeviceAddress(0x00,0x01,0xB0,0x03)
#define  gsv2k11Dev_VspMapAddress             AvGenerateDeviceAddress(0x00,0x01,0xB0,0x12)
#define  gsv2k11Dev_ParMapAddress             AvGenerateDeviceAddress(0x00,0x01,0xB0,0x14)
#define  gsv2k11Dev_AgMapAddress              AvGenerateDeviceAddress(0x00,0x01,0xB0,0x15)
#define  gsv2k11Dev_Rx1HdmiMapAddress         AvGenerateDeviceAddress(0x00,0x01,0xB0,0x20)
#define  gsv2k11Dev_Rx1AudMapAddress          AvGenerateDeviceAddress(0x00,0x01,0xB0,0x22)
#define  gsv2k11Dev_Rx1Hdcp2p2MapAddress      AvGenerateDeviceAddress(0x00,0x01,0xB0,0x21)
#define  gsv2k11Dev_Rx1EdidMapAddress         AvGenerateDeviceAddress(0x00,0x01,0xB0,0x10)
#define  gsv2k11Dev_Rx1ScdcMapAddress         AvGenerateDeviceAddress(0x00,0x01,0xB0,0x23)
#define  gsv2k11Dev_Rx1InfoFrameMapAddress    AvGenerateDeviceAddress(0x00,0x01,0xB0,0x24)
#define  gsv2k11Dev_Rx1InfoFrame2MapAddress   AvGenerateDeviceAddress(0x00,0x01,0xB0,0x25)
#define  gsv2k11Dev_Rx1RepMapAddress          AvGenerateDeviceAddress(0x00,0x01,0xB0,0x26)
#define  gsv2k11Dev_RxALane0MapAddress        AvGenerateDeviceAddress(0x00,0x01,0xB0,0x27)
#define  gsv2k11Dev_RxALane1MapAddress        AvGenerateDeviceAddress(0x00,0x01,0xB0,0x28)
#define  gsv2k11Dev_RxALane2MapAddress        AvGenerateDeviceAddress(0x00,0x01,0xB0,0x29)
#define  gsv2k11Dev_Tx1MainMapAddress         AvGenerateDeviceAddress(0x00,0x01,0xB0,0x40)
#define  gsv2k11Dev_Tx1UdpMapAddress          AvGenerateDeviceAddress(0x00,0x01,0xB0,0x42)
#define  gsv2k11Dev_Tx1CecMapAddress          AvGenerateDeviceAddress(0x00,0x01,0xB0,0x41)
#define  gsv2k11Dev_TxBPhyMapAddress          AvGenerateDeviceAddress(0x00,0x01,0xB0,0x70)
#define  gsv2k11Dev_TxBEdidMapAddress         AvGenerateDeviceAddress(0x00,0x01,0xB0,0x71)
#define  gsv2k11Dev_TxBHdcp2p2MapAddress      AvGenerateDeviceAddress(0x00,0x01,0xB0,0x72)

#define GSV2K11_PRIM_MAP_ADDR(port)    (((port->device->index)<<24) | gsv2k11Dev_PrimMapAddress)
#define GSV2K11_SEC_MAP_ADDR(port)     (((port->device->index)<<24) | gsv2k11Dev_SecMapAddress)
#define GSV2K11_HDMI_EDID_ADDR(port)   (((port->device->index)<<24) | gsv2k11Dev_Rx1EdidMapAddress)
#define GSV2K11_RXEDID_MAP_ADDR(port)  (((port->device->index)<<24) | gsv2k11Dev_Rx1RepMapAddress)
#define GSV2K11_PLL_MAP_ADDR(port)     (((port->device->index)<<24) | gsv2k11Dev_PllMapAddress)
#define GSV2K11_APLL_MAP_ADDR(port)    (((port->device->index)<<24) | gsv2k11Dev_APllMapAddress)
#define GSV2K11_PPLL_MAP_ADDR(port)    (((port->device->index)<<24) | gsv2k11Dev_PPllMapAddress)
#define GSV2K11_PAR_MAP_ADDR(port)     (((port->device->index)<<24) | gsv2k11Dev_ParMapAddress)
#define GSV2K11_AG_MAP_ADDR(port)      (((port->device->index)<<24) | gsv2k11Dev_AgMapAddress)
#define GSV2K11_INT_MAP_ADDR(port)     (((port->device->index)<<24) | gsv2k11Dev_IntMapAddress)
#define GSV2K11_VSP_MAP_ADDR(port)     (((port->device->index)<<24) | gsv2k11Dev_VspMapAddress)
#define GSV2K11_RXDIG_MAP_ADDR(port)   (((port->device->index)<<24) | gsv2k11Dev_Rx1HdmiMapAddress)
#define GSV2K11_RXRPT_MAP_ADDR(port)   (((port->device->index)<<24) | gsv2k11Dev_Rx1RepMapAddress)
#define GSV2K11_RXINFO_MAP_ADDR(port)  (((port->device->index)<<24) | gsv2k11Dev_Rx1InfoFrameMapAddress)
#define GSV2K11_RXINFO2_MAP_ADDR(port) (((port->device->index)<<24) | gsv2k11Dev_Rx1InfoFrame2MapAddress)
#define GSV2K11_RX2P2_MAP_ADDR(port)   (((port->device->index)<<24) | gsv2k11Dev_Rx1Hdcp2p2MapAddress)
#define GSV2K11_RXAUD_MAP_ADDR(port)   (((port->device->index)<<24) | gsv2k11Dev_Rx1AudMapAddress)
#define GSV2K11_RXSCDC_MAP_ADDR(port)  (((port->device->index)<<24) | gsv2k11Dev_Rx1ScdcMapAddress)
#define GSV2K11_RXLN_MAP_ADDR(port)    (((port->device->index)<<24) | gsv2k11Dev_RxALane0MapAddress)
#define GSV2K11_RXLN0_MAP_ADDR(port)   (((port->device->index)<<24) | gsv2k11Dev_RxALane0MapAddress)
#define GSV2K11_RXLN1_MAP_ADDR(port)   (((port->device->index)<<24) | gsv2k11Dev_RxALane1MapAddress)
#define GSV2K11_RXLN2_MAP_ADDR(port)   (((port->device->index)<<24) | gsv2k11Dev_RxALane2MapAddress)
#define GSV2K11_TXCEC_MAP_ADDR(port)   (((port->device->index)<<24) | gsv2k11Dev_Tx1CecMapAddress)
#define GSV2K11_TXDIG_MAP_ADDR(port)   (((port->device->index)<<24) | gsv2k11Dev_Tx1MainMapAddress)
#define GSV2K11_TXEDID_MAP_ADDR(port)  (((port->device->index)<<24) | gsv2k11Dev_TxBEdidMapAddress)
#define GSV2K11_TXPKT_MAP_ADDR(port)   (((port->device->index)<<24) | gsv2k11Dev_Tx1UdpMapAddress)
#define GSV2K11_TX2P2_MAP_ADDR(port)   (((port->device->index)<<24) | gsv2k11Dev_TxBHdcp2p2MapAddress)
#define GSV2K11_TXPHY_MAP_ADDR(port)   (((port->device->index)<<24) | gsv2k11Dev_TxBPhyMapAddress)

#endif
