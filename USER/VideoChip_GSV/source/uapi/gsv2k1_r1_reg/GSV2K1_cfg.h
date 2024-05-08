#ifndef _GSV2K1_CFG_H
#define _GSV2K1_CFG_H

#include "gsv2k1.h"
#include "av_config.h"

/* Map Address is fixed, used for single chip platform */
/* if multiple chips are implemented,
   Device->DeviceAddress | MapAddress to generate the 32-bit address */
#define  gsv2k1Dev_PrimMapAddress            AvGenerateDeviceAddress(0x00,0x01,0xB0,0x00)
#define  gsv2k1Dev_SecMapAddress             AvGenerateDeviceAddress(0x00,0x01,0xB0,0x01)
#define  gsv2k1Dev_PllMapAddress             AvGenerateDeviceAddress(0x00,0x01,0xB0,0x02)
#define  gsv2k1Dev_APllMapAddress            AvGenerateDeviceAddress(0x00,0x01,0xB0,0x05)
#define  gsv2k1Dev_PPllMapAddress            AvGenerateDeviceAddress(0x00,0x01,0xB0,0x06)
#define  gsv2k1Dev_IntMapAddress             AvGenerateDeviceAddress(0x00,0x01,0xB0,0x03)
#define  gsv2k1Dev_VspMapAddress             AvGenerateDeviceAddress(0x00,0x01,0xB0,0x12)
#define  gsv2k1Dev_ParMapAddress             AvGenerateDeviceAddress(0x00,0x01,0xB0,0x14)
#define  gsv2k1Dev_AgMapAddress              AvGenerateDeviceAddress(0x00,0x01,0xB0,0x15)
#define  gsv2k1Dev_Rx1HdmiMapAddress         AvGenerateDeviceAddress(0x00,0x01,0xB0,0x20)
#define  gsv2k1Dev_Rx1AudMapAddress          AvGenerateDeviceAddress(0x00,0x01,0xB0,0x22)
#define  gsv2k1Dev_Rx1Hdcp2p2MapAddress      AvGenerateDeviceAddress(0x00,0x01,0xB0,0x21)
#define  gsv2k1Dev_Rx1EdidMapAddress         AvGenerateDeviceAddress(0x00,0x01,0xB0,0x10)
#define  gsv2k1Dev_Rx1ScdcMapAddress         AvGenerateDeviceAddress(0x00,0x01,0xB0,0x23)
#define  gsv2k1Dev_Rx1InfoFrameMapAddress    AvGenerateDeviceAddress(0x00,0x01,0xB0,0x24)
#define  gsv2k1Dev_Rx1InfoFrame2MapAddress   AvGenerateDeviceAddress(0x00,0x01,0xB0,0x25)
#define  gsv2k1Dev_Rx1RepMapAddress          AvGenerateDeviceAddress(0x00,0x01,0xB0,0x26)
#define  gsv2k1Dev_RxALane0MapAddress        AvGenerateDeviceAddress(0x00,0x01,0xB0,0x27)
#define  gsv2k1Dev_RxALane1MapAddress        AvGenerateDeviceAddress(0x00,0x01,0xB0,0x28)
#define  gsv2k1Dev_RxALane2MapAddress        AvGenerateDeviceAddress(0x00,0x01,0xB0,0x29)
#define  gsv2k1Dev_Tx1MainMapAddress         AvGenerateDeviceAddress(0x00,0x01,0xB0,0x40)
#define  gsv2k1Dev_Tx1UdpMapAddress          AvGenerateDeviceAddress(0x00,0x01,0xB0,0x42)
#define  gsv2k1Dev_Tx1CecMapAddress          AvGenerateDeviceAddress(0x00,0x01,0xB0,0x41)
#define  gsv2k1Dev_TxAPhyMapAddress          AvGenerateDeviceAddress(0x00,0x01,0xB0,0x60)
#define  gsv2k1Dev_TxAEdidMapAddress         AvGenerateDeviceAddress(0x00,0x01,0xB0,0x61)
#define  gsv2k1Dev_TxAHdcp2p2MapAddress      AvGenerateDeviceAddress(0x00,0x01,0xB0,0x62)
#define  gsv2k1Dev_TxBPhyMapAddress          AvGenerateDeviceAddress(0x00,0x01,0xB0,0x70)
#define  gsv2k1Dev_TxBEdidMapAddress         AvGenerateDeviceAddress(0x00,0x01,0xB0,0x71)
#define  gsv2k1Dev_TxBHdcp2p2MapAddress      AvGenerateDeviceAddress(0x00,0x01,0xB0,0x72)

#define GSV2K1_PRIM_MAP_ADDR(port)    (((port->device->index)<<24)|gsv2k1Dev_PrimMapAddress)
#define GSV2K1_SEC_MAP_ADDR(port)     (((port->device->index)<<24)|gsv2k1Dev_SecMapAddress)
#define GSV2K1_HDMI_EDID_ADDR(port)   (((port->device->index)<<24)|gsv2k1Dev_Rx1EdidMapAddress)
#define GSV2K1_RXEDID_MAP_ADDR(port)  (((port->device->index)<<24)|gsv2k1Dev_Rx1RepMapAddress)
#define GSV2K1_PLL_MAP_ADDR(port)     (((port->device->index)<<24)|gsv2k1Dev_PllMapAddress)
#define GSV2K1_APLL_MAP_ADDR(port)    (((port->device->index)<<24)|gsv2k1Dev_APllMapAddress)
#define GSV2K1_PPLL_MAP_ADDR(port)    (((port->device->index)<<24)|gsv2k1Dev_PPllMapAddress)
#define GSV2K1_PAR_MAP_ADDR(port)     (((port->device->index)<<24)|gsv2k1Dev_ParMapAddress)
#define GSV2K1_AG_MAP_ADDR(port)      (((port->device->index)<<24)|gsv2k1Dev_AgMapAddress)
#define GSV2K1_INT_MAP_ADDR(port)     (((port->device->index)<<24)|gsv2k1Dev_IntMapAddress)
#define GSV2K1_VSP_MAP_ADDR(port)     (((port->device->index)<<24)|gsv2k1Dev_VspMapAddress)
#define GSV2K1_RXDIG_MAP_ADDR(port)   (((port->device->index)<<24)|gsv2k1Dev_Rx1HdmiMapAddress)
#define GSV2K1_RXRPT_MAP_ADDR(port)   (((port->device->index)<<24)|gsv2k1Dev_Rx1RepMapAddress)
#define GSV2K1_RXINFO_MAP_ADDR(port)  (((port->device->index)<<24)|gsv2k1Dev_Rx1InfoFrameMapAddress)
#define GSV2K1_RXINFO2_MAP_ADDR(port) (((port->device->index)<<24)|gsv2k1Dev_Rx1InfoFrame2MapAddress)
#define GSV2K1_RX2P2_MAP_ADDR(port)   (((port->device->index)<<24)|gsv2k1Dev_Rx1Hdcp2p2MapAddress)
#define GSV2K1_RXAUD_MAP_ADDR(port)   (((port->device->index)<<24)|gsv2k1Dev_Rx1AudMapAddress)
#define GSV2K1_RXSCDC_MAP_ADDR(port)  (((port->device->index)<<24)|gsv2k1Dev_Rx1ScdcMapAddress)
#define GSV2K1_RXLN_MAP_ADDR(port)    (((port->device->index)<<24)|gsv2k1Dev_RxALane0MapAddress)
#define GSV2K1_RXLN0_MAP_ADDR(port)   (((port->device->index)<<24)|gsv2k1Dev_RxALane0MapAddress)
#define GSV2K1_RXLN1_MAP_ADDR(port)   (((port->device->index)<<24)|gsv2k1Dev_RxALane1MapAddress)
#define GSV2K1_RXLN2_MAP_ADDR(port)   (((port->device->index)<<24)|gsv2k1Dev_RxALane2MapAddress)
#define GSV2K1_TXPKT_MAP_ADDR(port)   (((port->device->index)<<24)|gsv2k1Dev_Tx1UdpMapAddress)
#define GSV2K1_TXCEC_MAP_ADDR(port)   (((port->device->index)<<24)|gsv2k1Dev_Tx1CecMapAddress)
#define GSV2K1_TXDIG_MAP_ADDR(port)   (((port->device->index)<<24)|gsv2k1Dev_Tx1MainMapAddress)

uint32 GSV2K1_TXEDID_MAP_ADDR(AvPort *port)
{
    if (port->index == 4)
        return (((port->device->index)<<24)|gsv2k1Dev_TxAEdidMapAddress);
    else if(port->index == 5)
        return (((port->device->index)<<24)|gsv2k1Dev_TxBEdidMapAddress);
   else
        return 0x00;
}

uint32 GSV2K1_TX2P2_MAP_ADDR(AvPort *port)
{
    if (port->index == 4)
        return (((port->device->index)<<24)|gsv2k1Dev_TxAHdcp2p2MapAddress);
    else if(port->index == 5)
        return (((port->device->index)<<24)|gsv2k1Dev_TxBHdcp2p2MapAddress);
   else
        return 0x00;
}

uint32 GSV2K1_TXPHY_MAP_ADDR(AvPort *port)
{
    if (port->index == 4)
        return (((port->device->index)<<24)|gsv2k1Dev_TxAPhyMapAddress);
    else if(port->index == 5)
        return (((port->device->index)<<24)|gsv2k1Dev_TxBPhyMapAddress);
    else
        return 0x00;
}

#endif
