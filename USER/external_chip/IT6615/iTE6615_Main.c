///*****************************************
//  Copyright (C) 2009-2019
//  ITE Tech. Inc. All Rights Reserved
//  Proprietary and Confidential
///*****************************************
//   @file   <iTE6615_Main.c>
//   @author Kuro.Chung@ite.com.tw
//   @date   2019/06/05
//   @fileversion: iTE6615_MCUSRC_1.03
//******************************************/
#include "iTE6615_Global.h"
#include "iTE6615_Main.h"

#include "iTE6615_version.h"
bool g_bEnableHdcpSwitch = FALSE;
u8   g_u8OutputHdcpSta = 0;
static bool sg_bEnableIt6615PowerChangeFlag = FALSE;
static bool sg_bEnableIt6615PowerOffFlag = FALSE;
u8  g_pu8It6615TxEdidBuffer[256];
static bool sg_bEnableTxEdidFlag = FALSE;
static bool sg_bSignalInputChangeFlag = FALSE;
static bool sg_bIt6615AvMuteStatus = FALSE;
static bool sg_bIt6615AvMuteChangeFlag = FALSE;

static u16 sg_u16It6615Vic = 0;
#if _ENABLE_SKIP_NO_AUDIO_INTERRUPT
bool g_bEnableIt6615PowerOffFlag = TRUE;
#endif

void mapp_Ite6615SetOutputVic(u16 u16Vic)
{
    sg_u16It6615Vic = u16Vic;
}

u16 mapp_Ite6615GetOutputVic(void)
{
    return sg_u16It6615Vic;
}

void mapp_Ite6615SetSignalInputChange(bool bChange)
{
    sg_bSignalInputChangeFlag = bChange;
}

bool mapp_Ite6615GetSignalInputChange(void)
{
    return sg_bSignalInputChangeFlag;
}

void mapp_Ite6615SetPowerOffState(bool bSta)
{
    sg_bEnableIt6615PowerOffFlag = bSta;
    sg_bEnableIt6615PowerChangeFlag = TRUE;
}

bool mapp_Ite6615GetPowerState(void)
{
    return sg_bEnableIt6615PowerOffFlag;
}

void mapp_Ite6615SetAvMute(bool bSta)
{
    sg_bIt6615AvMuteStatus = bSta;
    sg_bIt6615AvMuteChangeFlag = TRUE;
}

void mapp_Ite6615AvMuteProcess(void)
{
    if(sg_bIt6615AvMuteChangeFlag)
    {
        sg_bIt6615AvMuteChangeFlag = FALSE;

        if(sg_bIt6615AvMuteStatus)
            iTE6615_SetTX_AVMute(AVMUTE_ON);
        else
            iTE6615_SetTX_AVMute(AVMUTE_OFF);
    }
}

void mapp_Ite6615HdcpProcess(void)
{
#if _ENABLE_IT6615_STARTUP_TODO_HDCP_TIMER
    if(!PlatformTimerExpired(etUser_It6615StartupTimer))
    {
        return;
    }
#endif

    if(g_bEnableHdcpSwitch)
    {
        if(iTE6615_CheckTX_HPD())
        {
            g_bEnableHdcpSwitch = FALSE;
            iTE6615_OutputHdcpSwitch(g_u8OutputHdcpSta);
        }
    }
}

void ITE6615_UserFunctionProcess(void)
{
    mapp_Ite6615HdcpProcess();
    mapp_Ite6615AvMuteProcess();
}

void ITE6615_SetReadTxEdidFlag(bool bSta)
{
    sg_bEnableTxEdidFlag = bSta;
}

bool ITE6615_GetReadTxEdidFlag(void)
{
    return sg_bEnableTxEdidFlag;
}

void ITE6615_ReadTxEdidProcess(void)
{
    static u8 sl_u8PreHpdSta = 0xFF;
    static u8 sl_u8DelayCnt = 0;
    u8 l_u8CurHpdSta;

    if(sl_u8DelayCnt<10)    // unit = 20ms // To read hpd state once per second
    {
        sl_u8DelayCnt++;
        return;
    }
    sl_u8DelayCnt = 0;

    #if _ENABLE_FOURCE_ON_IT6615_STATE
    l_u8CurHpdSta = (u8)mapp_GetOutHPDState();
    #else
    l_u8CurHpdSta = iTE6615_CheckTX_HPD();
    #endif

    if(sl_u8PreHpdSta!=l_u8CurHpdSta)
    {
        sl_u8PreHpdSta=l_u8CurHpdSta;

        IT6615_MSG(("===========TX HPD STA = %d\r\n",sl_u8PreHpdSta));
        //dbg_printf("===========TX HPD STA = %d\r\n",sl_u8PreHpdSta);
        if(sl_u8PreHpdSta)
        {
            u8 l_pu8EdidBuf[256];
            iTE6615_GetTxEdid(l_pu8EdidBuf);
            if(!is_edid_compare_same(g_pu8It6615TxEdidBuffer,l_pu8EdidBuf))
            {
                memcpy(g_pu8It6615TxEdidBuffer,l_pu8EdidBuf,256);
                ITE6615_SetReadTxEdidFlag(TRUE);
            }
            #if _ENABLE_FOURCE_ON_IT6615_STATE
            mapp_Ite6615SetSignalInputChange(TRUE);
            #endif

            #if _ENABLE_SKIP_NO_AUDIO_INTERRUPT
            g_bEnableIt6615PowerOffFlag = TRUE;
            #endif
        }
    }
}

void ITE6615_SrcChangeProcess(void)
{
    #if 0
    static u8 sl_u8DelayCnt = 0;

    if(sl_u8DelayCnt<20)    // unit = 20ms // To read hpd state once per second
    {
        sl_u8DelayCnt++;

        if(mapp_Ite6615GetSignalInputChange())
        {
            if (iTE6615_CheckTX_HPD())
                iTE6615_CurVTiming.VIC = mapp_Ite6615GetOutputVic();
        }
        return;
    }
    sl_u8DelayCnt = 0;
    #endif

    if(mapp_Ite6615GetSignalInputChange())
    {
        mapp_Ite6615SetSignalInputChange(FALSE);

        IT6615_MSG(("===========Source change ============== \r\n"));
        iTE6615_CurVTiming.VIC = mapp_Ite6615GetOutputVic();
        if (iTE6615_CheckTX_HPD())
        {
            #if _ENABLE_SKIP_NO_AUDIO_INTERRUPT
            g_bEnableIt6615PowerOffFlag = TRUE;
            #endif
            iTE6615_DATA.Flag_EDID_Parser_Ready = 0;
            iTE6615_sys_chg(iTE6615_STATES_Unplug);
            //iTE6615_SetTX_Power(POWER_OFF);
            It6615_delay1ms(100);
            iTE6615_sys_chg(iTE6615_STATES_HPDOn);
            //iTE6615_INIT_Chip();
        }
    }
}

void ITE6615_process(void)
{
    if(mapp_Ite6615GetPowerState())
    {
        if(sg_bEnableIt6615PowerChangeFlag)
        {
            sg_bEnableIt6615PowerChangeFlag = FALSE;
            iTE6615_SetTX_Power(POWER_OFF);
        }
        return;
    }
    else
    {
        if(sg_bEnableIt6615PowerChangeFlag)
        {
            sg_bEnableIt6615PowerChangeFlag = FALSE;
            //iTE6615_SetTX_Power(POWER_ON);
            iTE6615_sys_chg(iTE6615_STATES_Unplug);
            It6615_delay1ms(100);
            iTE6615_sys_chg(iTE6615_STATES_HPDOn);
        }
    }

    iTE6615_fsm();

    ITE6615_UserFunctionProcess();
    ITE6615_ReadTxEdidProcess();
    ITE6615_SrcChangeProcess();
}

void ITE6615_Init(void)
{
    #if ENABLE_IT6615_DEBUG
    IT6615_MSG(("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\r\n"));
    IT6615_MSG(("                   %s\r\n",ITE6615_VERSION_STRING));
    IT6615_MSG(("                   %s\r\n",ITE6615_BUILD_DATE_STR));
    IT6615_MSG(("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\r\n\r\n"));
    #endif
    u8IT6615_BusID = _I2C_TABLE_INDEX_IT6615_0;
    iTE6615_INIT_Chip();

    g_bEnableHdcpSwitch = TRUE;
    #if _ENABLE_IT6615_STARTUP_TODO_HDCP_TIMER
    PlatformTimerSet(etUser_It6615StartupTimer,5000);
    #endif
    memset(g_pu8It6615TxEdidBuffer,0,256);
}

