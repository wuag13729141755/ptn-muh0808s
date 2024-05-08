 ///*****************************************
//  Copyright (C) 2009-2014
//  ITE Tech. Inc. All Rights Reserved
//  Proprietary and Confidential
///*****************************************
//   @file   <Main.c>
//   @author Jau-Chih.Tseng@ite.com.tw
//   @date   2014/01/07
//   @fileversion: ITE_HDMITX_SAMPLE_3.17
//******************************************/

// ver 1.0.0: release demo code,Mingchih Lung
// 2007/11/19: Partition Spliter.c to Cat6023.c,Cat6613.c,EDID.c,HDCP.c,IO.c,Utility.c and Spliter.c
// 2007/11/20: Modify default EDID to match CEA specification
// 2008/01/29: add HDCP_ROM_SEL for two type of 6613 PKG
// 2008/02/13: Increase a DDC abort command     for BENQ monitor
// 2008/02/14: fix A2 version HDCP bugs. add RX singal port define
// 2008/02/29: remove HDCP_ROM_SEL,new 6613 ROM detect.Fix audio muilty channel bug

#include "IT66121/io/it66121_drv.h"

 IT66121_I2C_CONFIG const cg_stIT66121_I2CConfig[IT66121_DEV_NUM]=
{
    {
        #if IT66121_PIN_ADDR_LOW0
    	HDMI_TX_I2C_SLAVE_ADDR1,     // PCADR pin LOW
        #else
        HDMI_TX_I2C_SLAVE_ADDR2,     // PCADR pin HIGH
        #endif
		_I2C_TABLE_INDEX_IT66121_0,
    },
#if (IT66121_DEV_NUM>=2)
    {
        #if IT66121_PIN_ADDR_LOW1
    	HDMI_TX_I2C_SLAVE_ADDR1,     // PCADR pin LOW
        #else
        HDMI_TX_I2C_SLAVE_ADDR2,     // PCADR pin HIGH
        #endif
		_I2C_TABLE_INDEX_IT66121_1,
    },
#endif
#if (IT66121_DEV_NUM>=3)
    {
        #if IT66121_PIN_ADDR_LOW2
    	HDMI_TX_I2C_SLAVE_ADDR1,     // PCADR pin LOW
        #else
        HDMI_TX_I2C_SLAVE_ADDR2,     // PCADR pin HIGH
        #endif
		_I2C_TABLE_INDEX_IT66121_2,
    },
#endif
#if (IT66121_DEV_NUM>=4)
    {
        #if IT66121_PIN_ADDR_LOW3
    	HDMI_TX_I2C_SLAVE_ADDR1,     // PCADR pin LOW
        #else
        HDMI_TX_I2C_SLAVE_ADDR2,     // PCADR pin HIGH
        #endif
		_I2C_TABLE_INDEX_IT66121_3,
    },
#endif
};

u8 g_u8It66121DevAddr = HDMI_TX_I2C_SLAVE_ADDR1;
u8 g_u8It66121BusID = 0;

void IT66121_DeviceSel(u8 u8DevID)
{
    g_u8It66121DevAddr = cg_stIT66121_I2CConfig[u8DevID].u8DevAddr;
    g_u8It66121BusID   = cg_stIT66121_I2CConfig[u8DevID].u8BusId;
}

void IT66121_init(void)
{
    u8 l_u8Index;

    for(l_u8Index=0;l_u8Index<IT66121_DEV_NUM;l_u8Index++)
    {
        IT66121_DeviceSel(l_u8Index);
        InitHDMITX_Variable(l_u8Index);
        InitHDMITX(l_u8Index);

        HDMITX_ChangeDisplayOption(l_u8Index,HDMI_1080p60,HdmiTx_GetOutColorMode(bOutputColorMode[l_u8Index])) ;
    }
}

//BYTE count[HDMITX_MAX_DEV_COUNT]=0;

void IT66121_Process(void)
{
    u8 l_u8Index;

    for(l_u8Index=0;l_u8Index<IT66121_DEV_NUM;l_u8Index++)
    {
        IT66121_DeviceSel(l_u8Index);
        HDMITX_DevLoopProc(l_u8Index);
    }
}

void it66121_delay_ms(unsigned int ms)
{
    delay_ms(ms);
}





