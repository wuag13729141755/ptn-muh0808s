///*****************************************
//  Copyright (C) 2009-2020
//  ITE Tech. Inc. All Rights Reserved
//  Proprietary and Confidential
///*****************************************
//   @file   <it6635_opts.c>
//   @author Kenneth.Hung@ite.com.tw
//   @date   2020/12/30
//   @fileversion: IT66353_SAMPLE_1.06
//******************************************/
#include "IT6635/chip/IT6635_config.h"
#include "IT6635/chip/IT6635_platform.h"
#include "IT6635/chip/IT6635_debug.h"
#include "IT6635/chip/it6635_drv.h"
#include "IT6635/chip/it6635_EQ.h"
#include "IT6635/chip/it6635.h"

//
// TX_PN_SWAP
// 1: Enable TX side TMDS P/N swap
// 0: Disable TX side TMDS P/N swap
#define TX_PN_SWAP 0
#ifndef TX_PN_SWAP

    #pragma message("TX_PN_SWAP is defined to 0")
    #pragma message("IT6635 EVB should be TX_PN_SWAP==1")
	#error ("Please define TX_PN_SWAP by your PCB layout.")

#else

    #if TX_PN_SWAP
		#ifdef _SHOW_PRAGMA_MSG
//	    #pragma message("TX_PN_SWAP is pre-defined to 1")
		#endif
    #else
		#ifdef _SHOW_PRAGMA_MSG
//	    #pragma message("TX_PN_SWAP is pre-defined to 0")
		#endif
    #endif

#endif

//
// DEFAULT_RS_IDX
// :The default EQ when power on.
//
#define DEFAULT_RS_IDX 4

//
// DEFAULT_PORT
// :The default active port when power on.
#ifndef DEFAULT_PORT
#define DEFAULT_PORT   0
#endif

// constant definition
#define HPD_TOGGLE_TIMEOUT_400MS (27)
#define HPD_TOGGLE_TIMEOUT_1SEC (100)
#define HPD_TOGGLE_TIMEOUT_2SEC (20|(BIT7))
#define HPD_TOGGLE_TIMEOUT_3SEC (30|(BIT7))

#ifndef DEFAULT_EDID_FROM_SINK
#define DEFAULT_EDID_FROM_SINK          1
#endif

_CODE IT6635_RX_OPTIONS st_RxOptsNum[IT6635_I2C_DEV_NUM];
_CODE IT6635_TX_OPTIONS st_TxOptsNum[IT6635_I2C_DEV_NUM];
_CODE IT6635_DEV_OPTION st_DevOptsNum[IT6635_I2C_DEV_NUM];

_CODE IT6635_RX_OPTIONS s_RxOpts=
{
    0xC3, //u8 tag1;
    0,//u8 EnRxDDCBypass;
    0,//u8 EnRxPWR5VBypass;
    0,//u8 EnRxHPDBypass;
    1, //u8 TryFixedEQFirst;
    1,//u8 EnableAutoEQ;
    1,//u8 NonActivePortReplyHPD;
    0,//u8 DisableEdidRam;
    {DEFAULT_RS_IDX, DEFAULT_RS_IDX, DEFAULT_RS_IDX}, //u8 DefaultEQ[3];
    1,//u8 FixIncorrectHdmiEnc;
    0,//u8 HPDOutputInverse;
    HPD_TOGGLE_TIMEOUT_400MS,//u8 HPDTogglePeriod;
//    HPD_TOGGLE_TIMEOUT_1SEC,//u8 HPDTogglePeriod;
//    HPD_TOGGLE_TIMEOUT_2SEC,//u8 HPDTogglePeriod;
    1,//u8 TxOEAlignment;
    (u8)sizeof(IT6635_RX_OPTIONS),//u8 str_size;
};

_CODE IT6635_TX_OPTIONS s_TxOpts=
{
    0x3C,//u8 tag1;
    TX_PN_SWAP,//u8 EnTxPNSwap;
    TX_PN_SWAP,//u8 EnTxChSwap;
    0,//u8 EnTxVCLKInv;
    0,//u8 EnTxOutD1t;
    DEFAULT_EDID_FROM_SINK,//u8 CopyEDIDFromSink;
    1, //u8 ParsePhysicalAddr;
    1,//u8 TurnOffTx5VWhenSwitchPort;
    (u8)sizeof(IT6635_TX_OPTIONS),//u8 str_size;
};

_CODE IT6635_DEV_OPTION s_DevOpts =
{
    0x5A,//u8 tag1;
    SWAddr,//u8 SwAddr;
    RXAddr,//u8 RxAddr;
    CECAddr,//u8 CecAddr;
    RXEDIDAddr, //u8 EdidAddr;
    //u8 EnCEC;
    0, //u8 ForceRxOn;
    1, //u8 RxAutoPowerDown;
    1, //u8 DoTxPowerDown;
    0, //u8 TxPowerDownWhileWaitingClock;
    (u8)sizeof(IT6635_DEV_OPTION),// u8 str_size;

};


_CODE u8 s_default_edid_port0[]=
{
    0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x55, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x32, 0x1C, 0x01, 0x03, 0x80, 0x50, 0x2D, 0x78, 0x0A, 0xEE, 0x91, 0xA3, 0x54, 0x4C, 0x99, 0x26,
    0x0F, 0x50, 0x54, 0x01, 0x08, 0x00, 0xD1, 0xC0, 0xD1, 0x00, 0x81, 0xC0, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x08, 0xE8, 0x00, 0x30, 0xF2, 0x70, 0x5A, 0x80, 0xB0, 0x58,
    0x8A, 0x00, 0x20, 0xC2, 0x31, 0x00, 0x00, 0x1E, 0x04, 0x74, 0x00, 0x30, 0xF2, 0x70, 0x5A, 0x80,
    0xB0, 0x58, 0x8A, 0x00, 0x20, 0x52, 0x31, 0x00, 0x00, 0x1E, 0x00, 0x00, 0x00, 0xFD, 0x00, 0x18,
    0x4B, 0x1A, 0x51, 0x3C, 0x00, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0xFC,
    0x00, 0x48, 0x44, 0x20, 0x56, 0x69, 0x64, 0x65, 0x6F, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x01, 0xA1,
    0x02, 0x03, 0x3D, 0x70, 0x53, 0x61, 0x60, 0x5F, 0x5E, 0x5D, 0x66, 0x65, 0x64, 0x63, 0x62, 0x10,
    0x1F, 0x22, 0x21, 0x20, 0x05, 0x14, 0x04, 0x13, 0x23, 0x09, 0x1F, 0x07, 0x83, 0x01, 0x00, 0x00,
    0x67, 0x03, 0x0C, 0x00, 0x10, 0x00, 0xB8, 0x3C, 0x67, 0xD8, 0x5D, 0xC4, 0x01, 0x78, 0x80, 0x03,
    0xE2, 0x00, 0x0F, 0xE3, 0x05, 0xFF, 0x01, 0xE5, 0x0E, 0x61, 0x60, 0x66, 0x65, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x66,
};

void it6635_options_init(void)
{
    memcpy(&st_RxOptsNum[g_u8ITEDevice],&s_RxOpts,sizeof(IT6635_RX_OPTIONS));
    memcpy(&st_TxOptsNum[g_u8ITEDevice],&s_TxOpts,sizeof(IT6635_RX_OPTIONS));
    memcpy(&st_DevOptsNum[g_u8ITEDevice],&s_DevOpts,sizeof(IT6635_RX_OPTIONS));

    g_pstDev->opts.rx_opt[0] = &st_RxOptsNum[g_u8ITEDevice];
    g_pstDev->opts.rx_opt[1] = &st_RxOptsNum[g_u8ITEDevice];
    g_pstDev->opts.rx_opt[2] = &st_RxOptsNum[g_u8ITEDevice];
    g_pstDev->opts.rx_opt[3] = &st_RxOptsNum[g_u8ITEDevice];
    g_pstDev->opts.active_rx_opt = g_pstDev->opts.rx_opt[DEFAULT_PORT];
    g_pstDev->opts.tx_opt = &st_TxOptsNum[g_u8ITEDevice];
    g_pstDev->opts.dev_opt = &st_DevOptsNum[g_u8ITEDevice];
    if(g_pstDevAddr != NULL)
    {
        if(g_pstDevAddr->u8SWAddr != 0)
            g_pstDev->opts.dev_opt->SwAddr = g_pstDevAddr->u8SWAddr;
        if(g_pstDevAddr->u8RXAddr != 0)
            g_pstDev->opts.dev_opt->RxAddr = g_pstDevAddr->u8RXAddr;
        if(g_pstDevAddr->u8CECAddr != 0)
            g_pstDev->opts.dev_opt->CecAddr = g_pstDevAddr->u8CECAddr;
        if(g_pstDevAddr->u8EDIDAddr != 0)
            g_pstDev->opts.dev_opt->EdidAddr = g_pstDevAddr->u8EDIDAddr;
    }
    g_pstDev->vars.Rx_active_port = DEFAULT_PORT;

    it6635_vars_init();
}

