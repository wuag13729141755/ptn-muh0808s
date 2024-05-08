#ifndef _GSV2K7_CFG_H
#define _GSV2K7_CFG_H

#include "gsv2k7.h"
#include "av_config.h"

/* Map Address is fixed, used for single chip platform */
/* if multiple chips are implemented,
   Device->DeviceAddress | MapAddress to generate the 32-bit address */
#define  gsv2k7Dev_PrimMapAddress            AvGenerateDeviceAddress(0x00,0x01,0xB0,0x00)
#define  gsv2k7Dev_SecMapAddress             AvGenerateDeviceAddress(0x00,0x01,0xB0,0x01)
#define  gsv2k7Dev_PllMapAddress             AvGenerateDeviceAddress(0x00,0x01,0xB0,0x02)
#define  gsv2k7Dev_Pll2MapAddress            AvGenerateDeviceAddress(0x00,0x01,0xB0,0x04)
#define  gsv2k7Dev_APllMapAddress            AvGenerateDeviceAddress(0x00,0x01,0xB0,0x05)
#define  gsv2k7Dev_IntMapAddress             AvGenerateDeviceAddress(0x00,0x01,0xB0,0x03)
#define  gsv2k7Dev_VspMapAddress             AvGenerateDeviceAddress(0x00,0x01,0xB0,0x12)
#define  gsv2k7Dev_Rx1HdmiMapAddress         AvGenerateDeviceAddress(0x00,0x01,0xB0,0x20)
#define  gsv2k7Dev_Rx1AudMapAddress          AvGenerateDeviceAddress(0x00,0x01,0xB0,0x22)
#define  gsv2k7Dev_Rx1Hdcp2p2MapAddress      AvGenerateDeviceAddress(0x00,0x01,0xB0,0x21)
#define  gsv2k7Dev_Rx1EdidMapAddress         AvGenerateDeviceAddress(0x00,0x01,0xB0,0x10)
#define  gsv2k7Dev_Rx1ScdcMapAddress         AvGenerateDeviceAddress(0x00,0x01,0xB0,0x23)
#define  gsv2k7Dev_Rx1InfoFrameMapAddress    AvGenerateDeviceAddress(0x00,0x01,0xB0,0x24)
#define  gsv2k7Dev_Rx1InfoFrame2MapAddress   AvGenerateDeviceAddress(0x00,0x01,0xB0,0x25)
#define  gsv2k7Dev_Rx1RepMapAddress          AvGenerateDeviceAddress(0x00,0x01,0xB0,0x26)
#define  gsv2k7Dev_RxALane0MapAddress        AvGenerateDeviceAddress(0x00,0x01,0xB0,0x27)
#define  gsv2k7Dev_RxALane1MapAddress        AvGenerateDeviceAddress(0x00,0x01,0xB0,0x28)
#define  gsv2k7Dev_RxALane2MapAddress        AvGenerateDeviceAddress(0x00,0x01,0xB0,0x29)
#define  gsv2k7Dev_Rx2HdmiMapAddress         AvGenerateDeviceAddress(0x00,0x01,0xB0,0x30)
#define  gsv2k7Dev_Rx2AudMapAddress          AvGenerateDeviceAddress(0x00,0x01,0xB0,0x32)
#define  gsv2k7Dev_Rx2Hdcp2p2MapAddress      AvGenerateDeviceAddress(0x00,0x01,0xB0,0x31)
#define  gsv2k7Dev_Rx2EdidMapAddress         AvGenerateDeviceAddress(0x00,0x01,0xB0,0x10)
#define  gsv2k7Dev_Rx2ScdcMapAddress         AvGenerateDeviceAddress(0x00,0x01,0xB0,0x33)
#define  gsv2k7Dev_Rx2InfoFrameMapAddress    AvGenerateDeviceAddress(0x00,0x01,0xB0,0x34)
#define  gsv2k7Dev_Rx2InfoFrame2MapAddress   AvGenerateDeviceAddress(0x00,0x01,0xB0,0x35)
#define  gsv2k7Dev_Rx2RepMapAddress          AvGenerateDeviceAddress(0x00,0x01,0xB0,0x36)
#define  gsv2k7Dev_RxBLane0MapAddress        AvGenerateDeviceAddress(0x00,0x01,0xB0,0x37)
#define  gsv2k7Dev_RxBLane1MapAddress        AvGenerateDeviceAddress(0x00,0x01,0xB0,0x38)
#define  gsv2k7Dev_RxBLane2MapAddress        AvGenerateDeviceAddress(0x00,0x01,0xB0,0x39)
#define  gsv2k7Dev_RxCLane0MapAddress        AvGenerateDeviceAddress(0x00,0x01,0xB0,0x3A)
#define  gsv2k7Dev_RxCLane1MapAddress        AvGenerateDeviceAddress(0x00,0x01,0xB0,0x3B)
#define  gsv2k7Dev_RxCLane2MapAddress        AvGenerateDeviceAddress(0x00,0x01,0xB0,0x3C)
#define  gsv2k7Dev_RxDLane0MapAddress        AvGenerateDeviceAddress(0x00,0x01,0xB0,0x3D)
#define  gsv2k7Dev_RxDLane1MapAddress        AvGenerateDeviceAddress(0x00,0x01,0xB0,0x3E)
#define  gsv2k7Dev_RxDLane2MapAddress        AvGenerateDeviceAddress(0x00,0x01,0xB0,0x3F)
#define  gsv2k7Dev_Tx1MainMapAddress         AvGenerateDeviceAddress(0x00,0x01,0xB0,0x40)
#define  gsv2k7Dev_Tx1UdpMapAddress          AvGenerateDeviceAddress(0x00,0x01,0xB0,0x42)
#define  gsv2k7Dev_Tx1CecMapAddress          AvGenerateDeviceAddress(0x00,0x01,0xB0,0x41)
#define  gsv2k7Dev_TxAPhyMapAddress          AvGenerateDeviceAddress(0x00,0x01,0xB0,0x60)
#define  gsv2k7Dev_TxAEdidMapAddress         AvGenerateDeviceAddress(0x00,0x01,0xB0,0x61)
#define  gsv2k7Dev_TxAHdcp2p2MapAddress      AvGenerateDeviceAddress(0x00,0x01,0xB0,0x62)
#define  gsv2k7Dev_Tx2MainMapAddress         AvGenerateDeviceAddress(0x00,0x01,0xB0,0x50)
#define  gsv2k7Dev_Tx2UdpMapAddress          AvGenerateDeviceAddress(0x00,0x01,0xB0,0x52)
#define  gsv2k7Dev_Tx2CecMapAddress          AvGenerateDeviceAddress(0x00,0x01,0xB0,0x51)
#define  gsv2k7Dev_TxBPhyMapAddress          AvGenerateDeviceAddress(0x00,0x01,0xB0,0x70)
#define  gsv2k7Dev_TxBEdidMapAddress         AvGenerateDeviceAddress(0x00,0x01,0xB0,0x71)
#define  gsv2k7Dev_TxBHdcp2p2MapAddress      AvGenerateDeviceAddress(0x00,0x01,0xB0,0x72)

#define GSV2K7_PRIM_MAP_ADDR(port)    (((port->device->index)<<24) | gsv2k7Dev_PrimMapAddress)
#define GSV2K7_SEC_MAP_ADDR(port)     (((port->device->index)<<24) | gsv2k7Dev_SecMapAddress)
#define GSV2K7_HDMI_EDID_ADDR(port)   (((port->device->index)<<24) | gsv2k7Dev_Rx1EdidMapAddress)
#define GSV2K7_RXEDID_MAP_ADDR(port)  (((port->device->index)<<24) | gsv2k7Dev_Rx1RepMapAddress)
#define GSV2K7_PLL_MAP_ADDR(port)     (((port->device->index)<<24) | gsv2k7Dev_PllMapAddress)
#define GSV2K7_PLL2_MAP_ADDR(port)    (((port->device->index)<<24) | gsv2k7Dev_Pll2MapAddress)
#define GSV2K7_APLL_MAP_ADDR(port)    (((port->device->index)<<24) | gsv2k7Dev_APllMapAddress)
#define GSV2K7_INT_MAP_ADDR(port)     (((port->device->index)<<24) | gsv2k7Dev_IntMapAddress)
#define GSV2K7_VSP_MAP_ADDR(port)     (((port->device->index)<<24) | gsv2k7Dev_VspMapAddress)
#define GSV2K7_TXCEC_MAP_ADDR(port)   (((port->device->index)<<24) | gsv2k7Dev_Tx2CecMapAddress)
#define GSV2K7_TXDIG_MAP_ADDR(port)   (((port->device->index)<<24) | gsv2k7Dev_Tx2MainMapAddress)
#define GSV2K7_RXDIG_MAP_ADDR(port)   (((port->device->index)<<24) | gsv2k7Dev_Rx2HdmiMapAddress)
#define GSV2K7_TXEDID_MAP_ADDR(port)  (((port->device->index)<<24) | gsv2k7Dev_TxBEdidMapAddress)
#define GSV2K7_TXPKT_MAP_ADDR(port)   (((port->device->index)<<24) | gsv2k7Dev_Tx2UdpMapAddress)
#define GSV2K7_RXRPT_MAP_ADDR(port)   (((port->device->index)<<24) | gsv2k7Dev_Rx2RepMapAddress)
#define GSV2K7_RXINFO_MAP_ADDR(port)  (((port->device->index)<<24) | gsv2k7Dev_Rx2InfoFrameMapAddress)
#define GSV2K7_RXINFO2_MAP_ADDR(port) (((port->device->index)<<24) | gsv2k7Dev_Rx2InfoFrame2MapAddress)
#define GSV2K7_TX2P2_MAP_ADDR(port)   (((port->device->index)<<24) | gsv2k7Dev_TxBHdcp2p2MapAddress)
#define GSV2K7_TXPHY_MAP_ADDR(port)   (((port->device->index)<<24) | gsv2k7Dev_TxBPhyMapAddress)
#define GSV2K7_RX2P2_MAP_ADDR(port)   (((port->device->index)<<24) | gsv2k7Dev_Rx2Hdcp2p2MapAddress)
#define GSV2K7_RXAUD_MAP_ADDR(port)   (((port->device->index)<<24) | gsv2k7Dev_Rx2AudMapAddress)
#define GSV2K7_RXSCDC_MAP_ADDR(port)  (((port->device->index)<<24) | gsv2k7Dev_Rx2ScdcMapAddress)

uint32 GSV2K7_RXLN_MAP_ADDR(AvPort *port)
{
    if (port->index == 0)
        return (((port->device->index)<<24) | gsv2k7Dev_RxALane0MapAddress);
    else if(port->index == 1)
        return (((port->device->index)<<24) | gsv2k7Dev_RxBLane0MapAddress);
    else if(port->index == 2)
        return (((port->device->index)<<24) | gsv2k7Dev_RxCLane0MapAddress);
    else if(port->index == 3)
        return (((port->device->index)<<24) | gsv2k7Dev_RxDLane0MapAddress);
    else
        return 0x00;
}

uint32 GSV2K7_RXLN0_MAP_ADDR(AvPort *port)
{
    if (port->index == 0)
        return (((port->device->index)<<24) | gsv2k7Dev_RxALane0MapAddress);
    else if(port->index == 1)
        return (((port->device->index)<<24) | gsv2k7Dev_RxBLane0MapAddress);
    else if(port->index == 2)
        return (((port->device->index)<<24) | gsv2k7Dev_RxCLane0MapAddress);
    else if(port->index == 3)
        return (((port->device->index)<<24) | gsv2k7Dev_RxDLane0MapAddress);
    else
        return 0x00;
}

uint32 GSV2K7_RXLN1_MAP_ADDR(AvPort *port)
{
    if (port->index == 0)
        return (((port->device->index)<<24) | gsv2k7Dev_RxALane1MapAddress);
    else if(port->index == 1)
        return (((port->device->index)<<24) | gsv2k7Dev_RxBLane1MapAddress);
    else if(port->index == 2)
        return (((port->device->index)<<24) | gsv2k7Dev_RxCLane1MapAddress);
    else if(port->index == 3)
        return (((port->device->index)<<24) | gsv2k7Dev_RxDLane1MapAddress);
    else
        return 0x00;
}

uint32 GSV2K7_RXLN2_MAP_ADDR(AvPort *port)
{
    if (port->index == 0)
        return (((port->device->index)<<24) | gsv2k7Dev_RxALane2MapAddress);
    else if(port->index == 1)
        return (((port->device->index)<<24) | gsv2k7Dev_RxBLane2MapAddress);
    else if(port->index == 2)
        return (((port->device->index)<<24) | gsv2k7Dev_RxCLane2MapAddress);
    else if(port->index == 3)
        return (((port->device->index)<<24) | gsv2k7Dev_RxDLane2MapAddress);
    else
        return 0x00;
}


#endif
