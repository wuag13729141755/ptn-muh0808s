#include "includes.h"
#if ((IT663XX_VER != IT663XX_VER_1_07)&&(IT663XX_VER != IT663XX_NONE))
#if(_PRJ_TYPE_SWITCH_SPLITER==_PRJ_SWITCHER_AND_SPLITER)||(_PRJ_TYPE_SWITCH_SPLITER==_PRJ_SWITCHER_ONLY)


ite663xx_func_api_t ite663xx_stm32_api =
{
    #if (IS_IT663XX==IT66341)
    IT66341_SysKeyIn,
    #elif (IS_IT663XX==IT6634)
    IT6634_SysKeyIn,
    #elif (IS_IT663XX==IT66321)
    IT66321_SysKeyIn,
    #endif
    IT663XX_STM32_SysFuncSel,
    IT663XX_TxHdcpForceEffect,
    IT663XX_TxOutDisable,
    IT663XX_TxOutEnable,
    IT663XX_TxAdoDisable,
    IT663XX_TxAdoEnable,
    IT663XX_AudioProcess,
    IT663XX_CopyEdidFromTx,
    IT663XX_SetRxEdid,
};

pite663xx_func_api_t pITE663xx_STM32_api = &ite663xx_stm32_api;

#if (NUM_IT663XX_USE == 1)
tx_hdcp_force_t g_tIt663xxTxHdcpType[NUM_IT663XX_USE] = {_PRJ_TX_HDCP_MODE_ACTIVE};
tx_hdcp_force_t g_tPreIt663xxTxHdcpType[NUM_IT663XX_USE] = {tx_hdcp_force_max};
#elif (NUM_IT663XX_USE == 2)
tx_hdcp_force_t g_tIt663xxTxHdcpType[NUM_IT663XX_USE] = {_PRJ_TX_HDCP_MODE_ACTIVE, tx_hdcp_force_off};
tx_hdcp_force_t g_tPreIt663xxTxHdcpType[NUM_IT663XX_USE] = {tx_hdcp_force_max,  tx_hdcp_force_max};
#else
tx_hdcp_force_t g_tIt663xxTxHdcpType[NUM_IT663XX_USE] =
{
    #if (NUM_IT663XX_USE >= 1)
    tx_hdcp_force_off,
    #endif
    #if (NUM_IT663XX_USE >= 2)
    tx_hdcp_force_off,
    #endif
    #if (NUM_IT663XX_USE >= 3)
    tx_hdcp_force_off,
    #endif
    #if (NUM_IT663XX_USE >= 4)
    tx_hdcp_force_off,
    #endif
    #if (NUM_IT663XX_USE >= 5)
    tx_hdcp_force_off,
    #endif
    #if (NUM_IT663XX_USE >= 6)
    tx_hdcp_force_off,
    #endif
    #if (NUM_IT663XX_USE >= 7)
    tx_hdcp_force_off,
    #endif
    #if (NUM_IT663XX_USE >= 8)
    tx_hdcp_force_off,
    #endif
};
tx_hdcp_force_t g_tPreIt663xxTxHdcpType[NUM_IT663XX_USE] =
{
    #if (NUM_IT663XX_USE >= 1)
    tx_hdcp_force_max,
    #endif
    #if (NUM_IT663XX_USE >= 2)
        tx_hdcp_force_max,
    #endif
    #if (NUM_IT663XX_USE >= 3)
        tx_hdcp_force_max,
    #endif
    #if (NUM_IT663XX_USE >= 4)
        tx_hdcp_force_max,
    #endif
    #if (NUM_IT663XX_USE >= 5)
        tx_hdcp_force_max,
    #endif
    #if (NUM_IT663XX_USE >= 6)
        tx_hdcp_force_max,
    #endif
    #if (NUM_IT663XX_USE >= 7)
        tx_hdcp_force_max,
    #endif
    #if (NUM_IT663XX_USE >= 8)
        tx_hdcp_force_max,
    #endif
};
#endif

tx_out_status_t it66341_tx_out_status = tx_out_enable_stu, pre_it66341_tx_out_status = tx_out_enable_stu;
tx_out_status_t it66341_tx_ado_status = tx_out_enable_stu, pre_it66341_tx_ado_status = tx_out_enable_stu;

#if _PRJ_TYPE_SWITCH_SPLITER == _PRJ_SWITCHER_AND_SPLITER
tx_hdcp_force_t prj_suh2x_h2_tx_hdcp_type = _PRJ_TX_HDCP_MODE_ACTIVE, pre_suh2x_h2_tx_hdcp_type = tx_hdcp_force_max;
#endif

#if (HDCP2_HDCP1_DIVIDAULY == _MACRO_ENABLE)
extern bool Tx_HDCP_max_1_4;
#endif

#define IT66321_TX                  (OUT_TX1)

bool g_bHdmiAudMute = FALSE;
bool g_bInAudChgFlg[NUM_IT663XX_USE] = {FALSE};
bool g_bIt663xxSetRxEdidFlg[_HDMI_INPUT_PORT_USED_NUMBER] = {FALSE};
#if _ENABLE_SWITCH_PORT_TO_MUTE_I2S_AUDIO
bool g_bSwitchAudChgFlg[NUM_IT663XX_USE] = {FALSE};
#endif

iTE_u16 g_u16AudioProDelayTick[NUM_IT663XX_USE] = {0x00};// delay tick of audio process

extern iTE_u32  g_u32TxAdoEncConf[NUM_IT663XX_USE];

iTE_u8      g_u8It663xxRxHdcpState[NUM_IT663XX_USE];
iTE_u32         g_u32It663xxRxStaCheckCnt[NUM_IT663XX_USE] = {0};
iTE_u8      g_u8It663xxPreSigSta[NUM_IT663XX_USE] = {0};
iTE_u32         g_u32It663xxPreAudSta[NUM_IT663XX_USE] = {0};

bool g_bIt663xxChangeRxEdidFlag[_HDMI_INPUT_PORT_USED_NUMBER] = {FALSE};

#if _ENABLE_EDID_SWITCH_USE_NEW_MODE
bool g_bRxEdidChgFlg = TRUE;//FALSE;
#endif
#if _ENABLE_EDID_SWITCH_UNREBOOT
bool g_bUserEdidChange = TRUE;
#endif
#if _ENABLE_FAST_TO_SWITCH_PORT
iTE_u1 g_bSwUnplugHpdSta = TRUE;
#endif
#if _ENABLE_INPUT_PORT_HDCP_FOURCE_ON_14_OR_22
iTE_u1 g_bEnableSwHDCP22Flag = FALSE;
#endif

#if ENABLE_IT6634_SWITCH_EDID_LONG_TIME_HPD_LOW_TO_HIGH
extern iTE_u1 g_u1IsEdidSwitchMode[4];
#endif

void Project_STM32_SysFuncSel(iTE_u8 u8FuncKey);

void IT663XX_STM32_SysFuncSel(iTE_u8 u8FuncKey)
{
    Project_STM32_SysFuncSel(u8FuncKey);
}

iTE_u16 IT66341_TxHdcpForceSet(tx_hdcp_force_t forceType)
{
    iTE_u8 u8TxPort;
    iTE_u16 it66341_hdcp_sta = 0;

    switch(forceType)
    {
        case tx_hdcp_force_off:
            it66341_hdcp_sta |= TX_HDCP_FORCE_CLR;
            break;
        case tx_hdcp_force_1_4:
            it66341_hdcp_sta |= (TX_HDCP_FORCE_SET | TX_HDCP_ENG1);
            for(u8TxPort = OUT_TX0; u8TxPort < OUT_TXn; u8TxPort++)
            {
                if(HTx_Select(u8TxPort))
                {
                    STA_CHANGE(g_pstCurTxInfo->u8TxEdidSta, TXEDID_HDCP_SUP_MASK, TXEDID_HDCP_SUP_1);
                }
            }
            break;
        case tx_hdcp_force_2_2:
            it66341_hdcp_sta |= (TX_HDCP_FORCE_SET | TX_HDCP_ENG2);
            for(u8TxPort = OUT_TX0; u8TxPort < OUT_TXn; u8TxPort++)
            {
                if(HTx_Select(u8TxPort))
                {
                    STA_CHANGE(g_pstCurTxInfo->u8TxEdidSta, TXEDID_HDCP_SUP_MASK, TXEDID_HDCP_SUP_2);
                }
            }
            break;
        case tx_hdcp_force_bypass:
            it66341_hdcp_sta |= (TX_HDCP_FORCE_SET | TX_HDCP_ENG_NONE);
            break;
        default:
            it66341_hdcp_sta |= TX_HDCP_FORCE_CLR;
            break;
    }

    return it66341_hdcp_sta;
}

bool SetForceHdcpActive(tx_hdcp_force_t hdcp_type)
{
    bool ret = FALSE;
    iTE_u8 u8TxPort, u8RxPort, u8RdData;

    switch(hdcp_type)
    {
        case tx_hdcp_force_off:
            for(u8TxPort = OUT_TX0; u8TxPort < OUT_TXn; u8TxPort++)
            {
                if(HTx_Select(u8TxPort))
                {
                    u8RdData = HTx_R(0x07);
                    #if(IT663XX_VER == IT663XX_VER_1_07)
                    if((u8RdData&0x08)&&(u8RdData&0x10)&&((g_pstCurTxInfo->u8TxHdcpSta & TXHDCP_HDCP_MASK) != TXHDCP_HDCP_GOING))
                    #else
                    if((u8RdData&0x08)&&(u8RdData&0x10)&&((g_pstCurTxInfo->u16TxHdcpSta & TXHDCP_HDCP_MASK) != TXHDCP_HDCP_GOING))
                    #endif
                    {
                        u8RxPort = HSw_R(0x50 + u8TxPort) & 0x03;
                        HRx_Select(u8RxPort);
                        //if(!(HRx_R(0xCF) & 0x20))
                        {
                            HTx_Set(0x41, 0x01, 0x00);      // Disable CPDesired
                        }
                        #if(IT663XX_VER == IT663XX_VER_1_07)
                        STA_CHANGE(g_pstCurTxInfo->u8TxHdcpSta, TXHDCP_HDCP_MASK, TXHDCP_HDCP_FIRE);
                        #else
                        STA_CHANGE(g_pstCurTxInfo->u16TxHdcpSta, TXHDCP_HDCP_MASK, TXHDCP_HDCP_FIRE);
                        #endif
                        g_pstCurTxInfo->u16RxHdmiChkCnt = 0;
                        //g_pstCurTxInfo->u8TxHdcp2FalseCnt = TxHdcp2FalseTestDefaultCnt;
                        ret = TRUE;
                    }
                }
            }
            break;
        case tx_hdcp_force_1_4:
        case tx_hdcp_force_2_2:
            for(u8TxPort = OUT_TX0; u8TxPort < OUT_TXn; u8TxPort++)
            {
                if(HTx_Select(u8TxPort))
                {
                    u8RdData = HTx_R(0x07);
                    #if(IT663XX_VER == IT663XX_VER_1_07)
                    if((u8RdData&0x08)&&(u8RdData&0x10)&&((g_pstCurTxInfo->u8TxHdcpSta & TXHDCP_HDCP_MASK) != TXHDCP_HDCP_GOING))
                    #else
                    if((u8RdData&0x08)&&(u8RdData&0x10)&&((g_pstCurTxInfo->u16TxHdcpSta & TXHDCP_HDCP_MASK) != TXHDCP_HDCP_GOING))
                    #endif
                    //if(((HTx_R(0x07) & 0x18) == 0x18)&&((g_pstCurTxInfo->u16TxHdcpSta & TXHDCP_HDCP_MASK) != TXHDCP_HDCP_GOING))
                    {
                        #if(IT663XX_VER == IT663XX_VER_1_07)
                        STA_CHANGE(g_pstCurTxInfo->u8TxHdcpSta, TXHDCP_HDCP_MASK, TXHDCP_HDCP_FIRE);
                        #else
                        STA_CHANGE(g_pstCurTxInfo->u16TxHdcpSta, TXHDCP_HDCP_MASK, TXHDCP_HDCP_FIRE);
                        #endif
                        g_pstCurTxInfo->u16RxHdmiChkCnt = 0;
                        //g_pstCurTxInfo->u8TxHdcp2FalseCnt = TxHdcp2FalseTestDefaultCnt;
                        ret = TRUE;
                    }
                }
            }
            break;
        default:
            break;
    }

    return ret;
}

void TxFunVarSet(iTE_u16 setVal)
{
    iTE_u8 u8TxPort;

    for(u8TxPort = OUT_TX0; u8TxPort < OUT_TXn; u8TxPort++)
    {
        if(HTx_Select(u8TxPort))
        {
            /*g_u16TxFun[u8TxPort]*/g_pu16TxFunComm[u8TxPort] &= (~(TX_HDCP_ENG_MASK|TX_HDCP_FORCE_MASK));
            /*g_u16TxFun[u8TxPort]*/g_pu16TxFunComm[u8TxPort] |= setVal;
        }
    }
}

#if _PRJ_TYPE_SWITCH_SPLITER == _PRJ_SWITCHER_AND_SPLITER
#if(HDCP_HANDLE_FOR_AUDIO_BEFORE_VIDEO == _MACRO_DISABLE)
void prj_suh2x_h2_TxHdcpForce(iTE_u8 rx_state)
{
    switch(prj_suh2x_h2_tx_hdcp_type)
    {
        case tx_hdcp_force_off:
            if(prj_suh2x_h2_tx_hdcp_type != pre_suh2x_h2_tx_hdcp_type)
            {
                g_ptIt663xxTxHdcpTypeComm[0]/*g_tIt663xxTxHdcpType*/ = tx_hdcp_force_off;
                it6664_tx_hdcp_type = tx_hdcp_force_off;
                pre_suh2x_h2_tx_hdcp_type = prj_suh2x_h2_tx_hdcp_type;
            }
            break;
        case tx_hdcp_force_1_4:
            if(prj_suh2x_h2_tx_hdcp_type != pre_suh2x_h2_tx_hdcp_type)
            {
                g_ptIt663xxTxHdcpTypeComm[0]/*g_tIt663xxTxHdcpType*/ = tx_hdcp_force_off;
                it6664_tx_hdcp_type = tx_hdcp_force_1_4;
                pre_suh2x_h2_tx_hdcp_type = prj_suh2x_h2_tx_hdcp_type;
            }
            break;
        case tx_hdcp_force_2_2:
            if(prj_suh2x_h2_tx_hdcp_type != pre_suh2x_h2_tx_hdcp_type)
            {
                g_ptIt663xxTxHdcpTypeComm[0]/*g_tIt663xxTxHdcpType*/ = tx_hdcp_force_off;
                it6664_tx_hdcp_type = tx_hdcp_force_2_2;
                pre_suh2x_h2_tx_hdcp_type = prj_suh2x_h2_tx_hdcp_type;
            }
            break;
        case tx_hdcp_force_bypass:
            switch(rx_state)
            {
                case 1:
                    if(pre_suh2x_h2_tx_hdcp_type != tx_hdcp_force_off)
                    {
                        g_ptIt663xxTxHdcpTypeComm[0]/*g_tIt663xxTxHdcpType*/ = tx_hdcp_force_off;
                        it6664_tx_hdcp_type = tx_hdcp_force_off;
                        pre_suh2x_h2_tx_hdcp_type = tx_hdcp_force_off;
                    }
                    break;
                case 2:
                #if _PRJ_TX_HDCP_BYPASS_MAX_1_4 == _MACRO_ENABLE
                case 3:
                #endif
                    if(pre_suh2x_h2_tx_hdcp_type != tx_hdcp_force_1_4)
                    {
                        g_ptIt663xxTxHdcpTypeComm[0]/*g_tIt663xxTxHdcpType*/ = tx_hdcp_force_off;
                        it6664_tx_hdcp_type = tx_hdcp_force_1_4;
                        pre_suh2x_h2_tx_hdcp_type = tx_hdcp_force_1_4;
                    }
                    break;
                #if _PRJ_TX_HDCP_BYPASS_MAX_1_4 == _MACRO_DISABLE
                case 3:
                    if(pre_suh2x_h2_tx_hdcp_type != tx_hdcp_force_2_2)
                    {
                        g_ptIt663xxTxHdcpTypeComm[0]/*g_tIt663xxTxHdcpType*/ = tx_hdcp_force_off;
                        it6664_tx_hdcp_type = tx_hdcp_force_2_2;
                        pre_suh2x_h2_tx_hdcp_type = tx_hdcp_force_2_2;
                    }
                    break;
                #endif
                default:
                    break;
            }
            break;
        default:
            break;
    }
}
#else
#if (USING_1to8==_MACRO_DISABLE)
extern extern_variables ar_gext;
#else
extern extern_variables ar_gext[3];
#endif

void prj_suh2x_h2_TxHdcpForce(iTE_u8 rx_state)
{
    switch(prj_suh2x_h2_tx_hdcp_type)
    {
        case tx_hdcp_force_off:
            if(prj_suh2x_h2_tx_hdcp_type != pre_suh2x_h2_tx_hdcp_type)
            {
                g_ptIt663xxTxHdcpTypeComm[0]/*g_tIt663xxTxHdcpType*/ = tx_hdcp_force_off;
                #if (USING_1to8==_MACRO_DISABLE)
                memset(ar_gext.TXHDCPModeCur, tx_hdcp_force_off, 4);
                #else
                memset(ar_gext[0].TXHDCPModeCur, tx_hdcp_force_off, 4);
                memset(ar_gext[1].TXHDCPModeCur, tx_hdcp_force_off, 4);
                memset(ar_gext[2].TXHDCPModeCur, tx_hdcp_force_off, 4);
                #endif
                pre_suh2x_h2_tx_hdcp_type = prj_suh2x_h2_tx_hdcp_type;
            }
            break;
        case tx_hdcp_force_1_4:
            if(prj_suh2x_h2_tx_hdcp_type != pre_suh2x_h2_tx_hdcp_type)
            {
                g_ptIt663xxTxHdcpTypeComm[0]/*g_tIt663xxTxHdcpType*/ = tx_hdcp_force_off;
                #if (USING_1to8==_MACRO_DISABLE)
                memset(ar_gext.TXHDCPModeCur, tx_hdcp_force_1_4, 4);
                #else
                memset(ar_gext[0].TXHDCPModeCur, tx_hdcp_force_1_4, 4);
                memset(ar_gext[1].TXHDCPModeCur, tx_hdcp_force_1_4, 4);
                memset(ar_gext[2].TXHDCPModeCur, tx_hdcp_force_1_4, 4);
                #endif
                pre_suh2x_h2_tx_hdcp_type = prj_suh2x_h2_tx_hdcp_type;
            }
            break;
        case tx_hdcp_force_2_2:
            if(prj_suh2x_h2_tx_hdcp_type != pre_suh2x_h2_tx_hdcp_type)
            {
                g_ptIt663xxTxHdcpTypeComm[0]/*g_tIt663xxTxHdcpType*/ = tx_hdcp_force_off;
                #if (USING_1to8==_MACRO_DISABLE)
                memset(ar_gext.TXHDCPModeCur, tx_hdcp_force_2_2, 4);
                #else
                memset(ar_gext[0].TXHDCPModeCur, tx_hdcp_force_2_2, 4);
                memset(ar_gext[1].TXHDCPModeCur, tx_hdcp_force_2_2, 4);
                memset(ar_gext[2].TXHDCPModeCur, tx_hdcp_force_2_2, 4);
                #endif
                pre_suh2x_h2_tx_hdcp_type = prj_suh2x_h2_tx_hdcp_type;
            }
            break;
        case tx_hdcp_force_bypass:
            switch(rx_state)
            {
                case 1:
                    if(pre_suh2x_h2_tx_hdcp_type != tx_hdcp_force_off)
                    {
                        g_ptIt663xxTxHdcpTypeComm[0]/*g_tIt663xxTxHdcpType*/ = tx_hdcp_force_off;
                        #if (USING_1to8==_MACRO_DISABLE)
                        memset(ar_gext.TXHDCPModeCur, tx_hdcp_force_off, 4);
                        #else
                        memset(ar_gext[0].TXHDCPModeCur, tx_hdcp_force_off, 4);
                        memset(ar_gext[1].TXHDCPModeCur, tx_hdcp_force_off, 4);
                        memset(ar_gext[2].TXHDCPModeCur, tx_hdcp_force_off, 4);
                        #endif
                        pre_suh2x_h2_tx_hdcp_type = tx_hdcp_force_off;
                    }
                    break;
                case 2:
                #if _PRJ_TX_HDCP_BYPASS_MAX_1_4 == _MACRO_ENABLE
                case 3:
                #endif
                    if(pre_suh2x_h2_tx_hdcp_type != tx_hdcp_force_1_4)
                    {
                        g_ptIt663xxTxHdcpTypeComm[0]/*g_tIt663xxTxHdcpType*/ = tx_hdcp_force_off;
                        #if (USING_1to8==_MACRO_DISABLE)
                        memset(ar_gext.TXHDCPModeCur, tx_hdcp_force_1_4, 4);
                        #else
                        memset(ar_gext[0].TXHDCPModeCur, tx_hdcp_force_1_4, 4);
                        memset(ar_gext[1].TXHDCPModeCur, tx_hdcp_force_1_4, 4);
                        memset(ar_gext[2].TXHDCPModeCur, tx_hdcp_force_1_4, 4);
                        #endif
                        pre_suh2x_h2_tx_hdcp_type = tx_hdcp_force_1_4;
                    }
                    break;
                #if _PRJ_TX_HDCP_BYPASS_MAX_1_4 == _MACRO_DISABLE
                case 3:
                #if (HDCP2_HDCP1_DIVIDAULY == _MACRO_ENABLE)

                    if( Tx_HDCP_max_1_4)
                    {
                        if(pre_suh2x_h2_tx_hdcp_type != tx_hdcp_force_1_4)
                        {
                            g_ptIt663xxTxHdcpTypeComm[0]/*g_tIt663xxTxHdcpType*/ = tx_hdcp_force_off;
                            #if (USING_1to8==_MACRO_DISABLE)
                            memset(ar_gext.TXHDCPModeCur, tx_hdcp_force_1_4, 4);
                            #else
                            memset(ar_gext[0].TXHDCPModeCur, tx_hdcp_force_1_4, 4);
                            memset(ar_gext[1].TXHDCPModeCur, tx_hdcp_force_1_4, 4);
                            memset(ar_gext[2].TXHDCPModeCur, tx_hdcp_force_1_4, 4);
                            #endif
                            pre_suh2x_h2_tx_hdcp_type = tx_hdcp_force_1_4;
                            //RS232_printf("HDCP2.2->HDCP1.4\r\n");
                        }
                    }
                    else
                    {
                        if(pre_suh2x_h2_tx_hdcp_type != tx_hdcp_force_2_2)
                        {
                            g_ptIt663xxTxHdcpTypeComm[0]/*g_tIt663xxTxHdcpType*/ = tx_hdcp_force_off;
                            #if (USING_1to8==_MACRO_DISABLE)
                            memset(ar_gext.TXHDCPModeCur, tx_hdcp_force_2_2, 4);
                            #else
                            memset(ar_gext[0].TXHDCPModeCur, tx_hdcp_force_2_2, 4);
                            memset(ar_gext[1].TXHDCPModeCur, tx_hdcp_force_2_2, 4);
                            memset(ar_gext[2].TXHDCPModeCur, tx_hdcp_force_2_2, 4);
                            #endif
                            pre_suh2x_h2_tx_hdcp_type = tx_hdcp_force_2_2;
                            //RS232_printf("HDCP2.2->HDCP2.2\r\n");
                        }
                    }
                    break;
                #else
                    if(pre_suh2x_h2_tx_hdcp_type != tx_hdcp_force_2_2)
                    {
                        g_ptIt663xxTxHdcpTypeComm[0]/*g_tIt663xxTxHdcpType*/ = tx_hdcp_force_off;
                        #if (USING_1to8==_MACRO_DISABLE)
                        memset(ar_gext.TXHDCPModeCur, tx_hdcp_force_2_2, 4);
                        #else
                        memset(ar_gext[0].TXHDCPModeCur, tx_hdcp_force_2_2, 4);
                        memset(ar_gext[1].TXHDCPModeCur, tx_hdcp_force_2_2, 4);
                        memset(ar_gext[2].TXHDCPModeCur, tx_hdcp_force_2_2, 4);
                        #endif
                        pre_suh2x_h2_tx_hdcp_type = tx_hdcp_force_2_2;
                    }
                    break;
                #endif

                #endif
                default:
                    break;
            }
            break;
        default:
            break;
    }
}
#endif
#endif

void IT663XX_TxHdcpForceEffect(void)
{
    #if (_ENABLE_MULTI_VIEW_TO_UNUSED_HDCP_BYPASS == _MACRO_DISABLE)
    iTE_u8              i, j=0;

    for(i=0; i<NUM_IT663XX_USE; i++)
    {
        if(g_it663xx_device == IT663XX_Device[i])
        {
            break;
        }
    }

    #if _ENABLE_IT663XX_MULTI_CHIP_IN_USE
    j = j;
    //if(1)
    #else
    if(g_pu8RxSelComm[0] == hdmi_input_node_rx_table[i])
    {
        for(j=i+1; j<NUM_IT663XX_USE; j++)
        {
            if(g_u8RxSel[j] != hdmi_input_node_rx_table[j])
            {
                g_u8It663xxRxHdcpState[i] = g_u8It663xxRxHdcpState[j];
                break;
            }
        }
    }
    else
    #endif
    {
        g_u32It663xxRxStaCheckCnt[i] ++;
        if(g_u32It663xxRxStaCheckCnt[i] >= TIME_SRC_HDCP_TYPE_DET)
        {
            g_u32It663xxRxStaCheckCnt[i] = 0;
            #if (IS_IT663XX==IT66341)
            g_u8It663xxRxHdcpState[i] = IT66341_GetRxSta(g_pu8RxSelComm[0]/*g_u8RxSel*/);
            #elif (IS_IT663XX==IT6634)
            g_u8It663xxRxHdcpState[i] = IT6634_GetRxSta(g_pu8RxSelComm[0]/*g_u8RxSel*/);
            #elif (IS_IT663XX==IT66321)
            g_u8It663xxRxHdcpState[i] = IT66321_GetRxSta(g_pu8RxSelComm[0]/*g_u8RxSel*/);
            #endif

            #if _PRJ_TYPE_SWITCH_SPLITER == _PRJ_SWITCHER_AND_SPLITER
            //prj_suh2x_h2_TxHdcpForce(g_u8It663xxRxHdcpState[i]);
            #endif
        }
    }

    if(g_ptIt663xxTxHdcpTypeComm[0]/*g_tIt663xxTxHdcpType*/ != tx_hdcp_force_bypass)
    {
        if(g_ptPreIt663xxTxHdcpTypeComm[0]/*pre_it66341_tx_hdcp_type*/ != g_ptIt663xxTxHdcpTypeComm[0]/*g_tIt663xxTxHdcpType*/)
        {
            g_pu16Ite663xxTxHdcpStatusComm[0]/*u16Ite66341TxHdcpType*/ = IT66341_TxHdcpForceSet(g_ptIt663xxTxHdcpTypeComm[0]/*g_tIt663xxTxHdcpType*/);
            TxFunVarSet(g_pu16Ite663xxTxHdcpStatusComm[0]/*u16Ite66341TxHdcpType*/);
            if(SetForceHdcpActive(g_ptIt663xxTxHdcpTypeComm[0]/*g_tIt663xxTxHdcpType*/))
            {
                g_ptPreIt663xxTxHdcpTypeComm[0]/*pre_it66341_tx_hdcp_type*/ = g_ptIt663xxTxHdcpTypeComm[0]/*g_tIt663xxTxHdcpType*/;
            }
        }
    }
    else
    {
        switch(g_u8It663xxRxHdcpState[i])
        {
            case 1:
                if(g_ptPreIt663xxTxHdcpTypeComm[0]/*pre_it66341_tx_hdcp_type*/ != tx_hdcp_force_off)
                {
                    g_pu16Ite663xxTxHdcpStatusComm[0]/*u16Ite66341TxHdcpType*/ = IT66341_TxHdcpForceSet(tx_hdcp_force_off);
                    TxFunVarSet(g_pu16Ite663xxTxHdcpStatusComm[0]/*u16Ite66341TxHdcpType*/);
                    if(SetForceHdcpActive(tx_hdcp_force_off))
                    {
                        g_ptPreIt663xxTxHdcpTypeComm[0]/*pre_it66341_tx_hdcp_type*/ = tx_hdcp_force_off;
                    }
                }
                break;
            case 2:
            #if _PRJ_TX_HDCP_BYPASS_MAX_1_4 == _MACRO_ENABLE
            case 3:
            #endif
                if(g_ptPreIt663xxTxHdcpTypeComm[0]/*pre_it66341_tx_hdcp_type*/ != tx_hdcp_force_1_4)
                {
                    g_pu16Ite663xxTxHdcpStatusComm[0]/*u16Ite66341TxHdcpType*/ = IT66341_TxHdcpForceSet(tx_hdcp_force_1_4);
                    TxFunVarSet(g_pu16Ite663xxTxHdcpStatusComm[0]/*u16Ite66341TxHdcpType*/);
                    if(SetForceHdcpActive(tx_hdcp_force_1_4))
                    {
                        g_ptPreIt663xxTxHdcpTypeComm[0]/*pre_it66341_tx_hdcp_type*/ = tx_hdcp_force_1_4;
                    }
                }
                break;
            #if _PRJ_TX_HDCP_BYPASS_MAX_1_4 == _MACRO_DISABLE
            case 3:
                if(g_ptPreIt663xxTxHdcpTypeComm[0]/*pre_it66341_tx_hdcp_type*/ != tx_hdcp_force_2_2)
                {
                    g_pu16Ite663xxTxHdcpStatusComm[0]/*u16Ite66341TxHdcpType*/ = IT66341_TxHdcpForceSet(tx_hdcp_force_2_2);
                    TxFunVarSet(g_pu16Ite663xxTxHdcpStatusComm[0]/*u16Ite66341TxHdcpType*/);
                    if(SetForceHdcpActive(tx_hdcp_force_2_2))
                    {
                        g_ptPreIt663xxTxHdcpTypeComm[0]/*pre_it66341_tx_hdcp_type*/ = tx_hdcp_force_2_2;
                    }
                }
                break;
            #endif
            default:
                break;
        }
    }
    #endif
}

void IT663XX_TxOutDisable(void)
{
    #if (IS_IT663XX==IT66341)
    if(HTx_Select(OUT_TX0))
    {
        HTx_DisOut(OUT_TX0);
        #if(IT663XX_VER == IT663XX_VER_1_07)
    g_pstCurTxInfo->u8TxHdcpSta = 0x00;
        #else
        g_pstCurTxInfo->u16TxHdcpSta = 0x00;
        #endif
    }
    #elif (IS_IT663XX==IT66321)
    if(HTx_Select(IT66321_TX))
    {
        HTx_DisOut(IT66321_TX);
                #if(IT663XX_VER == IT663XX_VER_1_07)
        g_pstCurTxInfo->u8TxHdcpSta = 0x00;
                #else
                g_pstCurTxInfo->u16TxHdcpSta = 0x00;
                #endif
    }

    #elif (IS_IT663XX==IT6634)

    #endif
}

void IT663XX_TxOutEnable(void)
{
    #if (IS_IT663XX==IT66341)
	if(HTx_Select(IT66341_TX))
    {
        HTx_EnOut(OUT_TX0);
    }
    #elif (IS_IT663XX==IT66321)
    if(HTx_Select(IT66321_TX))
    {
        HTx_EnOut(IT66321_TX);
    }
    #elif (IS_IT663XX==IT6634)

    #endif
}

void IT663XX_TxAdoDisable(void)
{
    #if (IS_IT663XX==IT66341)
	if(HTx_Select(IT66341_TX))
    {
        g_bHdmiAudMute = TRUE;

        HSw_Bank(0);
        HSw_Set(0x7C, 0xF6, 0xF6);
        HSw_Set(0x7D, 0xFB, 0xFB);
        HSw_Set(0x7E, 0x50, 0x50);
    }
    #elif (IS_IT663XX==IT66321)
    if(HTx_Select(IT66321_TX))
    {
        iTE_u8 l_u8Offset = 0;
        g_bHdmiAudMute = TRUE;
        l_u8Offset = (IT66321_TX<<4);

        HSw_Bank(0);
        HSw_Set(0x7C+l_u8Offset, 0xF6, 0xF6);
        HSw_Set(0x7D+l_u8Offset, 0xFB, 0xFB);
        HSw_Set(0x7E+l_u8Offset, 0x50, 0x50);
    }
    //ITE663XX_I2sDisable();
    //ITE663XX_SpdifDisable();
    #elif (IS_IT663XX==IT6634)

    #endif
}

void IT663XX_TxAdoEnable(void)
{
    #if (IS_IT663XX==IT66341)
	if(HTx_Select(IT66341_TX))
    {
        g_bHdmiAudMute = FALSE;

        HSw_Bank(0);
        HSw_Set(0x7C, 0xF6, 0x00);
        HSw_Set(0x7D, 0xFB, 0x00);
        HSw_Set(0x7E, 0x50, 0x00);
    }
    #elif (IS_IT663XX==IT66321)
    if(HTx_Select(IT66321_TX))
    {
        iTE_u8 l_u8Offset = 0;
        g_bHdmiAudMute = FALSE;
        l_u8Offset = (IT66321_TX<<4);

        HSw_Bank(0);
        HSw_Set(0x7C+l_u8Offset, 0xF6, 0x00);
        HSw_Set(0x7D+l_u8Offset, 0xFB, 0x00);
        HSw_Set(0x7E+l_u8Offset, 0x50, 0x00);
    }
    //ITE663XX_I2sEnable();
    //ITE663XX_SpdifEnable();
    #elif (IS_IT663XX==IT6634)

    #endif
}

void SetAudioProDlyTick(iTE_u8 u8DevIdx, iTE_u16 u16Tick)
{
    g_u16AudioProDelayTick[u8DevIdx] = u16Tick;
}

iTE_u16 PollAudioProDlyTick(iTE_u8 u8DevIdx)
{
    return g_u16AudioProDelayTick[u8DevIdx];
}

void AudioProDlyTickLoop(void)
{
    iTE_u8 u8i;

    for(u8i=0; u8i<NUM_IT663XX_USE; u8i++)
    {
        if(g_u16AudioProDelayTick[u8i] > 1)
        {
            g_u16AudioProDelayTick[u8i] --;
        }
    }
}

void IT663XX_AudioProcess(void)
{
    iTE_u8          cur_sig_sta = 0, u8Index;

    #if _ENABLE_SWITCH_PORT_TO_MUTE_I2S_AUDIO
    static bool sl_bSwitchPortAudioChange = FALSE;
    #endif

    u8Index = IT663XX_GetDeviceIndexByID(g_it663xx_device);

    #if (IS_IT663XX==IT66341)
    cur_sig_sta = IT66341_GetTxSta();
    #elif (IS_IT663XX==IT66321)
    cur_sig_sta = IT66321_GetTxSta();
    #elif (IS_IT663XX==IT6634)
    //add you to do
    #endif

    if((g_u8It663xxPreSigSta[u8Index] != cur_sig_sta)
        || (g_u32It663xxPreAudSta[u8Index] != /*g_u32TxAdoEncConf*/g_pu32TxAdoEncConfComm[0])
        || (/*g_bInAudChgFlg*/g_pbInAudChgFlgComm[0])
        #if _ENABLE_SWITCH_PORT_TO_MUTE_I2S_AUDIO
        || (g_pbSwitchAudChgFlgComm[0] == TRUE)
        #endif
        )
    {
        if(g_u8It663xxPreSigSta[u8Index] != cur_sig_sta)
        {
            g_u8It663xxPreSigSta[u8Index] = cur_sig_sta;
        }

        if(g_u32It663xxPreAudSta[u8Index] != /*g_u32TxAdoEncConf*/g_pu32TxAdoEncConfComm[0])
        {
            g_u32It663xxPreAudSta[u8Index] = /*g_u32TxAdoEncConf*/g_pu32TxAdoEncConfComm[0];
        }

        if(/*g_bInAudChgFlg*/g_pbInAudChgFlgComm[0])
        {
            /*g_bInAudChgFlg*/g_pbInAudChgFlgComm[0] = FALSE;
        }
        #if _ENABLE_SWITCH_PORT_TO_MUTE_I2S_AUDIO
        if(g_pbSwitchAudChgFlgComm[0] == TRUE)
        {
            g_pbSwitchAudChgFlgComm[0] = FALSE;
            sl_bSwitchPortAudioChange = TRUE;
        }
        #endif
        SetAudioProDlyTick(u8Index, 500);// 500ms
    }

    if(PollAudioProDlyTick(u8Index)==1)
    {
        SetAudioProDlyTick(u8Index, 0);

        if(g_u8It663xxPreSigSta[u8Index] == 1)
        {
            if((/*g_u32TxAdoEncConf*/g_pu32TxAdoEncConfComm[0] & ADO_CONF_TYPE_MASK) == ADO_CONF_TYPE_LPCM)
            {
                if(g_bIt663xxSpdifEnable) ITE663XX_SpdifEnable();
                if(g_bIt663xxIISEnable) ITE663XX_I2sEnable();
            }
            else
            {
                if(g_bIt663xxSpdifEnable) ITE663XX_SpdifEnable();
                ITE663XX_I2sDisable();
            }
        }

        #if _ENABLE_SWITCH_PORT_TO_MUTE_I2S_AUDIO
        if(sl_bSwitchPortAudioChange == TRUE)
        {
            sl_bSwitchPortAudioChange = FALSE;
            if(g_bIt663xxSpdifEnable||g_bIt663xxIISEnable)
            {
                ITE663XX_AudioDecEnable();  //add by wf8421 20190102 to switch port enable audio decode
            }

            if((/*g_u32TxAdoEncConf*/g_pu32TxAdoEncConfComm[0] & ADO_CONF_TYPE_MASK) == ADO_CONF_TYPE_LPCM)
            {
                if(g_bIt663xxSpdifEnable)
                    ITE663XX_SpdifEnable();
                else
                    ITE663XX_SpdifDisable();

                if(g_bIt663xxIISEnable)
                    ITE663XX_I2sEnable();
                else
                    ITE663XX_I2sDisable();
            }
            else
            {
                if(g_bIt663xxSpdifEnable)
                    ITE663XX_SpdifEnable();
                else
                    ITE663XX_SpdifDisable();
                ITE663XX_I2sDisable();
            }
        }
        #endif
    }

    if(cur_sig_sta == 0)
    {
        ITE663XX_SpdifDisable();
        ITE663XX_I2sDisable();
    }

    if((/*g_u32TxAdoEncConf*/g_pu32TxAdoEncConfComm[0] & ADO_CONF_TYPE_MASK) != ADO_CONF_TYPE_LPCM)
    {
        if(g_bIt663xxSpdifEnable) ITE663XX_SpdifEnable();
        ITE663XX_I2sDisable();
    }
}

extern EdidInfo**   g_ppstTxEdidInfo;
extern stProjectGlobalAllVar_t g_stPrjGloVar;

EDID_STAT IT663XX_CopyEdidFromTx(iTE_u8 u8RxPort, iTE_u8 u8TxPort, EdidInfo *pstEdidInfo, bool bJustRead)
{
    bool        bEDIDInvaild = FALSE;
    #if (_EN_USE_VALENS_CHIP_LIMITED_MAX_300M == _MACRO_ENABLE)
    uint8_t     i, offset;
    #endif

    #if (_PRJ_TYPE_SWITCH_SPLITER == _PRJ_SWITCHER_ONLY)
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalRunVar_t    pstPrjRunVar = &pstPrjAllVar->stPrjRunVar;
    #endif

    if(HTx_Select(u8TxPort)){
        EDID_STAT   eState;
        iTE_u8  u8Edid[0x80];

        if(pstEdidInfo == NULL){
            iTE_MsgE(("pstEdidInfo = NULL\n"));
        }

        eState = Edid_BlkRead(0, u8Edid);
        #if ENABLE_IT66341_DVI_EDID_TO_CHANGE_HDCP14
        Edid_CheckDviMode(u8Edid);
        pstEdidInfo->u8ExtBlkCnt = u8Edid[0x7E];
        #endif
        if(eState == EDID_NO_ERR){
            //if EDID is correct
            if(!is_edid_valid(u8Edid))
            {
                memcpy(u8Edid, System_Default_Edid_table, 128);

                bEDIDInvaild = TRUE;
            }
            #if (_EN_USE_VALENS_CHIP_LIMITED_MAX_300M == _MACRO_ENABLE)
            else
            {
                offset = 72;
                //monitor range limits
                for(i=0; i<3; i++)
                {
                    if((u8Edid[offset]==0x00)&&(u8Edid[offset+1]==0x00)&&\
                        (u8Edid[offset+2]==0x00)&&(u8Edid[offset+3]==0xFD)&&(u8Edid[offset+4]==0x00))
                    {
                        if(u8Edid[offset+9] > 30)
                        {
                            u8Edid[offset+9] = 30;
                        }
                        break;
                    }
                    else
                    {
                        offset += 18;
                    }
                }
            }
            #endif
        }
        else
        {
            memcpy(u8Edid, System_Default_Edid_table, 128);
            bEDIDInvaild = TRUE;
        }

        if(!bJustRead) memcpy(&g_pu8It663xxEdidBuffer[0], u8Edid, 128);
        #if (_PRJ_TYPE_SWITCH_SPLITER == _PRJ_SWITCHER_ONLY)
        memcpy(pstPrjRunVar->u8OutEdid[0], u8Edid, 128);
        #endif
        if(!bJustRead)
        {
            PortEdidMax4K30(u8Edid, g_it663xx_device, u8RxPort);
            Edid_BlkWrite(0, u8Edid);
        }

        if(bEDIDInvaild)
        {
            if(!bJustRead) memcpy(&g_pu8It663xxEdidBuffer[128], System_Default_Edid_table+128, 128);
            #if (_PRJ_TYPE_SWITCH_SPLITER == _PRJ_SWITCHER_ONLY)
            memcpy(&pstPrjRunVar->u8OutEdid[0][128], System_Default_Edid_table+128, 128);
            #endif
            if(!bJustRead)
            {
                memcpy(u8Edid, System_Default_Edid_table+128, 128);
                Edid_BlkWrite(1, u8Edid);
            }
            return EDID_NO_ERR;
        }

        if(g_ppstTxEdidInfo[u8TxPort]->u8CeaBank){          // HDMI sink
            eState = Edid_BlkRead(g_ppstTxEdidInfo[u8TxPort]->u8CeaBank, u8Edid);
            if(eState == EDID_NO_ERR){
                if(!bJustRead)
                {
                    iTE_u8  u8Temp;
                    Edid_CopyEdidInfo((iTE_pu8)g_ppstTxEdidInfo[u8TxPort], (iTE_pu8)pstEdidInfo);
                    u8Temp = pstEdidInfo->pstVsdbInfo.u8HFVsdb[1];
                    if(u8Temp & (~HFVSDB_B6_MASK)){
                        u8Temp &= HFVSDB_B6_MASK;
                        u8Edid[pstEdidInfo->pstVsdbInfo.u8HFVsdbAdr + 5] = u8Temp;
                        pstEdidInfo->pstVsdbInfo.u8HFVsdb[1] = u8Temp;
                    }

                    u8Temp = pstEdidInfo->pstVsdbInfo.u8HFVsdb[2];
                    if(u8Temp & (~HFVSDB_B7_MASK)){
                        u8Temp &= HFVSDB_B7_MASK;
                        u8Edid[pstEdidInfo->pstVsdbInfo.u8HFVsdbAdr + 6] = u8Temp;
                        pstEdidInfo->pstVsdbInfo.u8HFVsdb[2] = u8Temp;
                    }

                    u8Temp = pstEdidInfo->pstVsdbInfo.u8B6;
                    if(u8Temp & (~VSDB_B6_MASK)){
                        u8Temp &= VSDB_B6_MASK;
                        u8Edid[pstEdidInfo->pstVsdbInfo.u8PaAdr + 2] = u8Temp;
                        pstEdidInfo->pstVsdbInfo.u8B6 = u8Temp;
                    }
                }

                #if (_EN_USE_VALENS_CHIP_LIMITED_MAX_300M == _MACRO_ENABLE)
                LimitedVSDBMaxPixel(u8Edid, 0x3C);//MAX PIXEL CLOCK is 300M
                #endif

                if(!bJustRead) memcpy(&g_pu8It663xxEdidBuffer[128], u8Edid, 128);
                #if (_PRJ_TYPE_SWITCH_SPLITER == _PRJ_SWITCHER_ONLY)
                memcpy(&pstPrjRunVar->u8OutEdid[0][128], u8Edid, 128);
                #endif
                if(!bJustRead) Edid_BlkWrite(1, u8Edid);
            }
        }else{
            if(!bJustRead) memcpy(&g_pu8It663xxEdidBuffer[128], System_Default_Edid_table+128, 128);
            #if (_PRJ_TYPE_SWITCH_SPLITER == _PRJ_SWITCHER_ONLY)
            memcpy(&pstPrjRunVar->u8OutEdid[0][128], System_Default_Edid_table+128, 128);
            #endif
            if(!bJustRead)
            {
                memcpy(u8Edid, System_Default_Edid_table+128, 128);
                Edid_BlkWrite(1, u8Edid);
            }
            return EDID_CEA_ERR;
        }

        if(eState == EDID_READ_ERR){
            if((g_pstCurTxInfo->u16TxPortSta & TXPORT_POWER_ON_MASK) == TXPORT_POWER_ON_CLR){
                return EDID_HDMI_ERR;
            }
        }
        return eState;
    }

    return EDID_HDMI_ERR;
}
#if (_ENABLE_EDID_SWITCH_AUDIO == _MACRO_ENABLE)
bool g_bisSwitchEdid = TRUE;
#endif
void IT663XX_SetRxEdid(void)
{
#if _ENABLE_EDID_SWITCH_USE_NEW_MODE
    mapp_RxPortEdidSwitch();
#elif _ENABLE_EDID_SWITCH_UNREBOOT
    #if _ENABLE_EDID_SWITCH_A_SINGLE_CHANNEL
    IT6634_SysEdidChangeBySighalChannel();
    #else
    IT6634_SysEdidChange();
    #endif
#else
    #if(_ENABLE_DEBUG_FIRST_POWER_ON_NO_EDID_ == _MACRO_DISABLE)
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    #endif
    uint8_t                     i, j, u8RxLogicAddr, u8RxPort;

    for(i=0; i<_HDMI_INPUT_PORT_USED_NUMBER; i++)
    {
        #if(_ENABLE_DEBUG_FIRST_POWER_ON_NO_EDID_ == _MACRO_ENABLE)
        if((g_bIt663xxSetRxEdidFlg[i])/*&&(pstPrjSaveVar->stHdmiSel.u8SwChannel==i)*/)
        #else
        if((g_bIt663xxSetRxEdidFlg[i])&&(pstPrjSaveVar->stHdmiSel.u8SwChannel==i))
        #endif
        {
            #if _ENABLE_IT663XX_MULTI_CHIP_IN_USE
            u8RxPort = RxPortMap[i].u8PortIndex;
            j = RxPortMap[i].u8DevNum;
            if(g_it663xx_device == IT663XX_Device[j])
            {
                IT663XX_EdidWrite(u8RxPort, g_pu8It663xxEdidBuffer);
                g_bIt663xxSetRxEdidFlg[i] = FALSE;
            }
            #else
            u8RxLogicAddr = hdmi_input_logic_index_table[i];

            for(j=0; j<NUM_IT663XX_USE; j++)
            {
                u8RxPort = ((u8RxLogicAddr>>(_HDMI_INPUT_PHYSIC_SHIFT*j))&_HDMI_INPUT_PHYSIC_MASK);
                if(u8RxPort != hdmi_input_node_rx_table[j])
                {
                    break;
                }
            }
            if(g_it663xx_device == IT663XX_Device[j])
            {
                #if (_ENABLE_EDID_SWITCH_AUDIO == _MACRO_ENABLE)
                g_bisSwitchEdid = TRUE;
                g_bIt663xxIISEnable = FALSE;
                ITE663XX_I2sDisable();
                g_bIt663xxSpdifEnable = FALSE;
                ITE663XX_SpdifDisable();
                #endif
                IT663XX_EdidWrite(u8RxPort, g_pu8It663xxEdidBuffer);
                g_bIt663xxSetRxEdidFlg[i] = FALSE;
            }
            #endif
        }
    }
#endif
}

#if _ENABLE_IT663XX_MULTI_CHIP_IN_USE
iTE_u8 mapp_GetCurrentRxPort(iTE_u8 u8DevID, iTE_u8 u8CurPort)
{
    uint8_t l_u8PortIndex;

    for(l_u8PortIndex=0;l_u8PortIndex<_HDMI_INPUT_PORT_USED_NUMBER;l_u8PortIndex++)
    {
        if((RxPortMap[l_u8PortIndex].u8PortIndex == u8CurPort)&&(RxPortMap[l_u8PortIndex].u8DevNum==u8DevID))
            break;
    }

    return l_u8PortIndex;
}

#else
iTE_u8 mapp_GetCurrentRxPort(iTE_u8 u8DevID, iTE_u8 u8CurPort)
{
    uint8_t l_u8PortIndex,l_u8RxLogicAddr,L_u8RxPort;

    for(l_u8PortIndex=0;l_u8PortIndex<_HDMI_INPUT_PORT_USED_NUMBER;l_u8PortIndex++)
    {
        l_u8RxLogicAddr = hdmi_input_logic_index_table[l_u8PortIndex];

        if(u8DevID>0)
            L_u8RxPort = (u8CurPort<<(_HDMI_INPUT_PHYSIC_SHIFT*u8DevID));
        else
            L_u8RxPort = u8CurPort;

        if((l_u8RxLogicAddr == L_u8RxPort)&&(L_u8RxPort!=hdmi_input_node_rx_table[u8DevID]))
            break;
    }

    return l_u8PortIndex;
}
#endif

#if ENABLE_IT66341_DVI_EDID_TO_CHANGE_HDCP14
#if (IT66321==IS_IT663XX)
extern iTE_u8 IT66321_SysChkRxHdcpSup(void);
#else
extern iTE_u8 IT66341_SysChkRxHdcpSup(void);
#endif

void mapp_SwitchDviEdidModeToChangeHdcpVer(u8 u8RxPort)
{
    if(g_pu8EdidDviModeComm[u8RxPort] == 1)  // DVI mode
    {
        iTE_Msg(("****** DVI EDID to set 1.4 for Rx[%d] HDCP ******\n",u8RxPort));
        STA_CHANGE(g_pu8CurRxFunCom[0], RX_FUN_HDCP_MASK, RX_FUN_HDCP_1);
        g_pu8RxFunComm[u8RxPort] = g_pu8CurRxFunCom[0];
        HRx_HdcpEngSet(u8RxPort, RXHDCP_SUP_NONE);
        HRx_HdcpEngSet(u8RxPort, RXHDCP_SUP_1);
    }
    else
    {
        iTE_Msg(("****** HDMI EDID to set 2.2 for Rx[%d] HDCP ******\n",u8RxPort));
    #if _ENABLE_INPUT_PORT_HDCP_FOURCE_ON_14_OR_22
        if(g_bEnableSwHDCP22Flag == FALSE)
        {
            if(u8RxPort==IN_RX0)
            {
                STA_CHANGE(g_pu8CurRxFunCom[0], RX_FUN_HDCP_MASK, Def_RxFunc_Rx0);
            }
            else if(u8RxPort==IN_RX1)
            {
                STA_CHANGE(g_pu8CurRxFunCom[0], RX_FUN_HDCP_MASK, Def_RxFunc_Rx1);
            }
            else if(u8RxPort==IN_RX2)
            {
                STA_CHANGE(g_pu8CurRxFunCom[0], RX_FUN_HDCP_MASK, Def_RxFunc_Rx2);
            }
            else if(u8RxPort==IN_RX3)
            {
                STA_CHANGE(g_pu8CurRxFunCom[0], RX_FUN_HDCP_MASK, Def_RxFunc_Rx3);
            }
            g_pu8RxFunComm[u8RxPort] = g_pu8CurRxFunCom[0];
            HRx_HdcpEngSet(u8RxPort, RXHDCP_SUP_NONE);
            if((g_pu8CurRxFunCom[0]&RX_FUN_HDCP_MASK) == RX_FUN_HDCP_1)
                HRx_HdcpEngSet(u8RxPort, RXHDCP_SUP_1);
            else if((g_pu8CurRxFunCom[0]&RX_FUN_HDCP_MASK) == RX_FUN_HDCP_2)
                HRx_HdcpEngSet(u8RxPort, RXHDCP_SUP_2);
        }
        else
    #endif
        {
            STA_CHANGE(g_pu8CurRxFunCom[0], RX_FUN_HDCP_MASK, RX_FUN_HDCP_AUTO);
            g_pu8RxFunComm[u8RxPort] = g_pu8CurRxFunCom[0];
            #if (IT66321==IS_IT663XX)
            IT66321_SysChkRxHdcpSup();
            #else
            IT66341_SysChkRxHdcpSup();
            #endif
        }
    }
}
#endif

#if _ENABLE_EDID_SWITCH_USE_NEW_MODE
void mapp_RxPortEdidSwitch(void)
{
    #if (_DISABLE_EDID_BYPASS == DISABLE)
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    #endif
    u8 u8IteDevSave = g_it663xx_device;
    #if (IT66321==IS_IT663XX)
    u8 l_u8TxPort = IT66321_TX;
    #else
    u8 l_u8TxPort = IT66341_TX;
    #endif

    if(g_bRxEdidChgFlg&&HTx_Select(l_u8TxPort))
    {
        u8 l_u8Index,l_u8Port;
        #if (_DISABLE_EDID_BYPASS == DISABLE)
        if(pstPrjSaveVar->stEdidSelIndex.u8EdidFlashIndex==EDID_MODE_BYPASS)
        {
            Edid_GetFromDisplay(l_u8TxPort, g_pu8It663xxEdidBuffer);
        }
        #endif

        for(l_u8Index=0;l_u8Index<_HDMI_INPUT_PORT_USED_NUMBER;l_u8Index++)
        {
            if(l_u8Index >= _HDMI_INPUT_PORT_USED_NUMBER) break;
            #if _ENABLE_IT663XX_MULTI_CHIP_IN_USE
            iT663XX_DevSel(RxPortMap[l_u8Index].u8DevNum);
            l_u8Port = RxPortMap[l_u8Index].u8PortIndex;
            #else
            iT663XX_DevSel(IT663XX_Device[l_u8Index/IN_RXn]);
            l_u8Port = hdmi_input_logic_index_table[l_u8Index];
            #endif
            if(HRx_Select(l_u8Port))
            {
                #if 0// _ENABLE_TYPE_C_EDID_LIMITED
                if(l_u8Port == _USB_C_PORT_INDEX1)
                {
                    u8 l_u8EdidBuffer[256];
                    IT66341_TypeCPortToLimitEdid(g_pu8It663xxEdidBuffer,l_u8EdidBuffer);
                    mapp_EdidWriteToRxPort(l_u8Port,l_u8EdidBuffer);
                    mapp_Ps176Reset();
                }
                else
                #elif 0//_EN_PORT_USB_C_EDID_LIMITED_4K30
                if(l_u8Port == _USB_C_PORT_INDEX1)
                {
                    u8 l_u8EdidBuffer[256];
                    IT66341_TypeCPortToLimitEdid(UART_METHOD1_DEFAULT_720P_PCM2CH_EDID,l_u8EdidBuffer);
                    mapp_EdidWriteToRxPort(l_u8Port,l_u8EdidBuffer);
                    mapp_Ps176Reset();
                }
                else
                #endif
                if(g_bIt663xxChangeRxEdidFlag[l_u8Index] == FALSE)
                {
                    mapp_EdidWriteToRxPort(l_u8Port,System_Default_Edid_table);
                }
                else
                    mapp_EdidWriteToRxPort(l_u8Port,g_pu8It663xxEdidBuffer);
                #if ENABLE_IT66341_DVI_EDID_TO_CHANGE_HDCP14
                mapp_SwitchDviEdidModeToChangeHdcpVer(l_u8Port);
                #endif

                #if 0//_ENABLE_TYPE_C_SWITCH_REPLUG_HPD
                if((l_u8Port == _HDMI_INPUT_TYPE_C_PORT)&&(g_pu8RxSelComm[0]!=_HDMI_INPUT_TYPE_C_PORT))
                {
                    HRx_HpdSet(l_u8Port, HPD_L);
                }
                else
                #endif
                    HRx_HpdSet(l_u8Port,HPD_L2H);
            }
        }

        HSw_Timer0En(iTE_TRUE);
        g_bRxEdidChgFlg = FALSE;

        iT663XX_DevSel(u8IteDevSave);
    }
}
/*************************************************************/
void mapp_EdidChange(void)
{
    g_bRxEdidChgFlg = TRUE;
}

void mapp_TxPortReConnectToEdidChange(void)
{
    //puart_method1_edid_res_t pEdidRes = &g_UartMethod1Edid;
    //if(pEdidRes->edid_index.edid_flash_index==EDID_MODE_BYPASS)
    {
        mapp_EdidChange();
    }
}
#endif

#if _ENABLE_EDID_SWITCH_UNREBOOT
#if (IT66321==IS_IT663XX)
extern EDID_STAT IT66321_SysEdidGet(iTE_u8 u8RxPort, iTE_u8 u8EdidMode);
#else
extern EDID_STAT IT66341_SysEdidGet(iTE_u8 u8RxPort, iTE_u8 u8EdidMode);
#endif
extern iTE_u8   g_u8AdoOut;

//****************************************************************************
void IT6634_SysEdidChange(void)
{
    u8 l_u8RxPort;
    u8 l_u8RxRealPort;
    u8 u8IteDevSave = g_it663xx_device;
    #if !_ENABLE_IT663XX_MULTI_CHIP_IN_USE
    u8 j,u8shift;
    #endif

    if(g_bUserEdidChange == TRUE)
    {
        iTE_Msg(("+IT6634_SysEdidChange\n"));
        g_bUserEdidChange = FALSE;
        //u8DevIndex = IT663XX_GetDeviceIndexByID(g_it663xx_device);
        for(l_u8RxPort=0; l_u8RxPort<_HDMI_INPUT_PORT_USED_NUMBER; l_u8RxPort++)
        {
            #if _ENABLE_IT663XX_MULTI_CHIP_IN_USE
            iT663XX_DevSel(IT663XX_Device[RxPortMap[l_u8RxPort].u8DevNum]);
            l_u8RxRealPort = RxPortMap[l_u8RxPort].u8PortIndex;//mapp_GetCurrentRxPort(RxPortMap[l_u8RxPort].u8DevNum,l_u8RxPort);
            #else
            iT663XX_DevSel(IT663XX_Device[l_u8RxPort/IN_RXn]);
            //l_u8RxRealPort=hdmi_input_logic_index_table[l_u8RxPort];
            //l_u8RxRealPort = mapp_GetCurrentRxPort(l_u8RxPort/IN_RXn,l_u8RxPort);
            for(j=0; j<NUM_IT663XX_USE; j++)
            {
                u8shift = (_HDMI_INPUT_PHYSIC_SHIFT*j);
                l_u8RxRealPort = (hdmi_input_logic_index_table[l_u8RxPort]>>u8shift)&_HDMI_INPUT_PHYSIC_MASK;
                if(hdmi_input_node_rx_table[j] != l_u8RxRealPort)
                {
                    break;
                }
            }
            #endif
        #if 0//_ENABLE_SWITCH_EDID_SKIP_TYPE_C_PORT
            if(l_u8RxRealPort == _HDMI_INPUT_TYPE_C_PORT)
            {
                continue;
            }
        #endif

            if(g_bIt663xxChangeRxEdidFlag[l_u8RxPort]==FALSE)
            {
                continue;
            }

            if(HRx_Select(l_u8RxRealPort))
            {
                if(g_pu8RxSelComm[0]==l_u8RxRealPort)
                {
                    //HSw_AudioDecRst();
                    ITE663XX_I2sDisable();
                    ITE663XX_SpdifDisable();
                    //STA_CHANGE(g_pu8AudCodecComm[0], ADO_DEC_PORT_MASK, g_au8RxPortSel[IT6634_GetTxSrc(IT66341_TX)]);
                    //STA_CHANGE(g_pu8AudCodecComm[0], ADO_DEC_PORT_MASK, g_pu8RxSelComm[0]);
                }

                //if((g_u8RxEdid[l_u8RxPort] & RX_EDID_MODE_MASK) != RX_EDID_MODE_COPY_TX0)
                {
                    #if (IT66321==IS_IT663XX)
                    IT66321_SysEdidGet(l_u8RxRealPort, g_pu8RxEdidComm[l_u8RxRealPort]);
                    #else
                    IT66341_SysEdidGet(l_u8RxRealPort, g_pu8RxEdidComm[l_u8RxRealPort]);
                    #endif
                    if(g_pu8CurRxFunCom[0] & RX_FUN_FORCE_ON_MASK){
                        if((g_pu8CurRxFunCom[0] & RX_FUN_FORCE_ON_MASK) == RX_FUN_FORCE_ON_FULL){
                            #if (IT663XX_VER == IT663XX_VER_1_30)
                            HRx_PwrOn(l_u8RxRealPort);
                            #else
                            HRx_PwrOn(l_u8RxRealPort, iTE_TRUE);
                            #endif
                        }else{
                            #if (IT663XX_VER == IT663XX_VER_1_30)
                            HRx_PwrOn(l_u8RxRealPort);
                            #else
                            HRx_PwrOn(l_u8RxRealPort, iTE_FALSE);
                            #endif
                        }
                    }
                #if ENABLE_IT66341_DVI_EDID_TO_CHANGE_HDCP14
                    mapp_SwitchDviEdidModeToChangeHdcpVer(l_u8RxRealPort);
                #endif

                #if 0//ENABLE_IT66341_CHANGE_HDCP14_TO_NO_HDMI20_EDID
                    if(g_bEdidHDMI20Mode[l_u8RxRealPort] == 0)
                    {
                        iTE_Msg(("****** HDMI 1.4 EDID to set 1.4 for Rx[%d] HDCP ******\n",l_u8RxRealPort));
                        STA_CHANGE(g_pu8CurRxFunCom[0], RX_FUN_HDCP_MASK, RX_FUN_HDCP_1);
                        g_pu8RxFunComm[l_u8RxRealPort] = g_pu8CurRxFunCom[0];
                        HRx_HdcpEngSet(l_u8RxRealPort, RXHDCP_SUP_NONE);
                        HRx_HdcpEngSet(l_u8RxRealPort, RXHDCP_SUP_1);
                    }
                    else
                    {
                        iTE_Msg(("****** HDMI 2.0 EDID to set 2.2 for Rx[%d] HDCP ******\n",l_u8RxRealPort));
                        STA_CHANGE(g_pu8CurRxFunCom[0], RX_FUN_HDCP_MASK, RX_FUN_HDCP_AUTO);
                        g_pu8RxFunComm[l_u8RxRealPort] = g_pu8CurRxFunCom[0];
                        IT66341_SysChkRxHdcpSup();
                    }
                #endif

                #if Project_SCU61E_PS176_HDCP_BUG_PRO
                    //if((l_u8RxRealPort == _HDMI_INPUT_TYPE_C_PORT)&&(g_pu8RxSelComm[0]!=_HDMI_INPUT_TYPE_C_PORT))
                    if((PS176PortInputState(l_u8RxRealPort)!=0)&&(PS176PortInputState(l_u8RxRealPort)!=2))
                    {
                        HRx_HpdSet(l_u8RxRealPort, HPD_L);
                    }
                    else
                #endif
                    {
                        HRx_HpdSet(l_u8RxRealPort, HPD_L2H);
                    }

                    if(g_pu8RxSelComm[0]==l_u8RxRealPort)
                    {
                        if(g_bIt663xxSpdifEnable)
                            ITE663XX_SpdifEnable();
                        if(g_bIt663xxIISEnable)
                            ITE663XX_I2sEnable();
                        //HSw_AudioDecEn(iTE_TRUE, g_u8AdoOut);
                    }

                }
            }
        }
        iT663XX_DevSel(u8IteDevSave);
        iTE_Msg(("-IT6634_SysEdidChange\n"));
    }
}

#if _ENABLE_EDID_SWITCH_A_SINGLE_CHANNEL
void IT6634_SysEdidChangeBySighalChannel(void)
{
    u8 l_u8RxPort;
    u8 l_u8RxRealPort;
    u8 u8IteDevSave = g_it663xx_device;
    #if !_ENABLE_IT663XX_MULTI_CHIP_IN_USE
    u8 j,u8shift;
    #endif

    if(g_bUserEdidChange == TRUE)
    {
        g_bUserEdidChange = FALSE;
        for(l_u8RxPort=0; l_u8RxPort<_HDMI_INPUT_PORT_USED_NUMBER; l_u8RxPort++)
        {
            if(g_bIt663xxSetRxEdidFlg[l_u8RxPort] == TRUE)
            {
                iTE_Msg(("+IT6634_SysEdidChangeBySighalChannel  InputPort[%d]\n",l_u8RxPort));
                g_bIt663xxSetRxEdidFlg[l_u8RxPort] = FALSE;
            #if _ENABLE_IT663XX_MULTI_CHIP_IN_USE
                iT663XX_DevSel(IT663XX_Device[RxPortMap[l_u8RxPort].u8DevNum]);
                l_u8RxRealPort = RxPortMap[l_u8RxPort].u8PortIndex;//mapp_GetCurrentRxPort(RxPortMap[l_u8RxPort].u8DevNum,l_u8RxPort);
            #else
                iT663XX_DevSel(IT663XX_Device[l_u8RxPort/IN_RXn]);

                //l_u8RxRealPort=hdmi_input_logic_index_table[l_u8RxPort];
                //l_u8RxRealPort = mapp_GetCurrentRxPort(l_u8RxPort/IN_RXn,l_u8RxPort);
                for(j=0; j<NUM_IT663XX_USE; j++)
                {
                    u8shift = (_HDMI_INPUT_PHYSIC_SHIFT*j);
                    l_u8RxRealPort = (hdmi_input_logic_index_table[l_u8RxPort]>>u8shift)&_HDMI_INPUT_PHYSIC_MASK;
                    if(hdmi_input_node_rx_table[j] != l_u8RxRealPort)
                    {
                        break;
                    }
                }
            #endif

                if(g_bIt663xxChangeRxEdidFlag[l_u8RxPort]==FALSE)
                {
                    continue;
                }

                if(HRx_Select(l_u8RxRealPort))
                {
                    if(g_pu8RxSelComm[0]==l_u8RxRealPort)
                    {
                        //HSw_AudioDecRst();
                        ITE663XX_I2sDisable();
                        ITE663XX_SpdifDisable();
                        //STA_CHANGE(g_pu8AudCodecComm[0], ADO_DEC_PORT_MASK, g_au8RxPortSel[IT6634_GetTxSrc(IT66341_TX)]);
                        //STA_CHANGE(g_pu8AudCodecComm[0], ADO_DEC_PORT_MASK, g_pu8RxSelComm[0]);
                    }

                    #if 0
                    #if (IT66321==IS_IT663XX)
                    IT66321_SysEdidGet(l_u8RxRealPort, g_pu8RxEdidComm[l_u8RxRealPort]);
                    #else
                    IT66341_SysEdidGet(l_u8RxRealPort, g_pu8RxEdidComm[l_u8RxRealPort]);
                    #endif


                    if(g_pu8CurRxFunCom[0] & RX_FUN_FORCE_ON_MASK){
                        if((g_pu8CurRxFunCom[0] & RX_FUN_FORCE_ON_MASK) == RX_FUN_FORCE_ON_FULL){
                            #if (IT663XX_VER == IT663XX_VER_1_30)
                            HRx_PwrOn(l_u8RxRealPort);
                            #else
                            HRx_PwrOn(l_u8RxRealPort, iTE_TRUE);
                            #endif
                        }else{
                            #if (IT663XX_VER == IT663XX_VER_1_30)
                            HRx_PwrOn(l_u8RxRealPort);
                            #else
                            HRx_PwrOn(l_u8RxRealPort, iTE_FALSE);
                            #endif
                        }
                    }
                    #else
                    GetIT663xxEdidDataForSingleChannel(g_pu8It663xxEdidBuffer,l_u8RxPort);
                    if(g_bIt663xxChangeRxEdidFlag[l_u8RxPort] == FALSE)
                    {
                        mapp_EdidWriteToRxPort(l_u8RxRealPort,System_Default_Edid_table);
                    }
                    else
                    {
                        switch(g_pu8RxEdidComm[l_u8RxRealPort] & RX_EDID_MODE_MASK)
                        {
                            case    RX_EDID_MODE_COPY_TX0:
                            case    RX_EDID_MODE_COPY_TX1:
                            case    RX_EDID_MODE_COPY_TX2:
                                {
                                    iTE_u8 l_u8TxPort;

                                    #if (IS_IT663XX == IT66341)
                                    l_u8TxPort = IT66341_TX;
                                    #elif (IS_IT663XX == IT66321)
                                    l_u8TxPort = IT66321_TX;
                                    #else
                                    l_u8TxPort = OUT_TX0;
                                    #endif
                                    #if _ENABLE_TYPE_C_EDID_BY_PASS_ONLY
                                    //if(isIt663xxDeviceRxTypecPort(g_it663xx_device, l_u8RxRealPort))
                                    if((1<<l_u8RxPort) == _HDMI_INPUT_TYPE_C_PORT)
                                    {
                                        u8 l_u8EdidBuffer[256];
                                        Edid_GetFromDisplay(l_u8TxPort, l_u8EdidBuffer);
                                        IT66341_TypeCPortToLimitEdid(l_u8EdidBuffer,g_u8SingleChannelEdidBuffer[l_u8RxPort]);
                                        mapp_EdidWriteToRxPort(l_u8RxRealPort,g_u8SingleChannelEdidBuffer[l_u8RxPort]);
                                    }
                                    else
                                    #endif
                                    {
                                        Edid_GetFromDisplay(l_u8TxPort, g_u8SingleChannelEdidBuffer[l_u8RxPort]);
                                        mapp_EdidWriteToRxPort(l_u8RxRealPort,g_u8SingleChannelEdidBuffer[l_u8RxPort]);
                                    }
                                }
                                break;
                            default:
                                {
                                    mapp_EdidWriteToRxPort(l_u8RxRealPort,g_u8SingleChannelEdidBuffer[l_u8RxPort]);
                                }
                                break;
                        }
                    }
                    #endif
                    #if ENABLE_IT66341_DVI_EDID_TO_CHANGE_HDCP14
                    mapp_SwitchDviEdidModeToChangeHdcpVer(l_u8RxRealPort);
                    #endif

                    #if 0//ENABLE_IT66341_CHANGE_HDCP14_TO_NO_HDMI20_EDID
                    if(g_bEdidHDMI20Mode[l_u8RxRealPort] == 0)
                    {
                        iTE_Msg(("****** HDMI 1.4 EDID to set 1.4 for Rx[%d] HDCP ******\n",l_u8RxRealPort));
                        STA_CHANGE(g_pu8CurRxFunCom[0], RX_FUN_HDCP_MASK, RX_FUN_HDCP_1);
                        g_pu8RxFunComm[l_u8RxRealPort] = g_pu8CurRxFunCom[0];
                        HRx_HdcpEngSet(l_u8RxRealPort, RXHDCP_SUP_NONE);
                        HRx_HdcpEngSet(l_u8RxRealPort, RXHDCP_SUP_1);
                    }
                    else
                    {
                        iTE_Msg(("****** HDMI 2.0 EDID to set 2.2 for Rx[%d] HDCP ******\n",l_u8RxRealPort));
                        STA_CHANGE(g_pu8CurRxFunCom[0], RX_FUN_HDCP_MASK, RX_FUN_HDCP_AUTO);
                        g_pu8RxFunComm[l_u8RxRealPort] = g_pu8CurRxFunCom[0];
                        IT66341_SysChkRxHdcpSup();
                    }
                    #endif

                    #if Project_SCU61E_PS176_HDCP_BUG_PRO
                    //if((l_u8RxRealPort == _HDMI_INPUT_TYPE_C_PORT)&&(g_pu8RxSelComm[0]!=_HDMI_INPUT_TYPE_C_PORT))
                    if((PS176PortInputState(l_u8RxRealPort)!=0)&&(PS176PortInputState(l_u8RxRealPort)!=2))
                    {
                        HRx_HpdSet(l_u8RxRealPort, HPD_L);
                    }
                    else
                    #endif
                    {
                        #if ENABLE_IT6634_SWITCH_EDID_LONG_TIME_HPD_LOW_TO_HIGH
                        g_u1IsEdidSwitchMode[l_u8RxRealPort] = TRUE;
                        #endif
                        HRx_HpdSet(l_u8RxRealPort, HPD_L2H);
                    }

                    if(g_pu8RxSelComm[0]==l_u8RxRealPort)
                    {
                        if(g_bIt663xxSpdifEnable)
                            ITE663XX_SpdifEnable();
                        if(g_bIt663xxIISEnable)
                            ITE663XX_I2sEnable();
                        //HSw_AudioDecEn(iTE_TRUE, g_u8AdoOut);
                    }

                }

                iTE_Msg(("-IT6634_SysEdidChangeBySighalChannel  InputPort[%d]\n",l_u8RxPort));
            }
        }
        iT663XX_DevSel(u8IteDevSave);
    }
}

#endif

#endif

iTE_u8 IT663XX_GetTxTiming(iTE_u8 u8DevID, iTE_pu8 pu8Timing)
{
    iTE_u8 l_u8TxPort;
    u8 u8IteDevSave = g_it663xx_device;
    iTE_u8 l_u8Vic = 0;

    #if _ENABLE_IT663XX_MULTI_CHIP_IN_USE
    iT663XX_DevSel(IT663XX_Device[RxPortMap[u8DevID].u8DevNum]);
    #else
    iT663XX_DevSel(IT663XX_Device[u8DevID]);
    #endif
    #if (IS_IT663XX == IT66321)
    l_u8TxPort =  IT66321_TX;
    #elif (IS_IT663XX == IT66341)
    l_u8TxPort =  IT66341_TX;
    #elif (IS_IT663XX == IT6634)
    l_u8TxPort =  g_pu8TxSelComm[0];
    #endif

    if(HTx_Select(l_u8TxPort))
    {
        #if(IT663XX_VER == IT663XX_VER_1_07)
        l_u8Vic = HTx_ShowVidInfo(l_u8TxPort);
        #else
        l_u8Vic = HTx_ShowVidInfo(l_u8TxPort,pu8Timing);
        #endif
    }

    iT663XX_DevSel(u8IteDevSave);

    return l_u8Vic;
}

iTE_u8 IT663XX_GetRxTiming(iTE_u8 u8RxPort,iTE_pu8 pu8Timing)
{
    iTE_u8 l_u8Vic = 0;
    u8 u8IteDevSave = g_it663xx_device;
    u8 l_u8RxRealPort;

    #if _ENABLE_IT663XX_MULTI_CHIP_IN_USE
    iT663XX_DevSel(IT663XX_Device[RxPortMap[u8RxPort].u8DevNum]);
    l_u8RxRealPort = RxPortMap[u8RxPort].u8PortIndex;//mapp_GetCurrentRxPort(RxPortMap[u8RxPort].u8DevNum,RxPortMap[u8RxPort].u8PortIndex);
    #else
    iT663XX_DevSel(IT663XX_Device[u8RxPort/IN_RXn]);
    l_u8RxRealPort = mapp_GetCurrentRxPort(u8RxPort/IN_RXn,u8RxPort);
    #endif

    if(HRx_Select(l_u8RxRealPort))
    {
        #if(IT663XX_VER == IT663XX_VER_1_07)
        l_u8Vic = HRx_ShowVidInfo();
        #else
        l_u8Vic = HRx_ShowVidInfo(pu8Timing);
        #endif
        #if (IS_IT663XX==IT66341)
        pu8Timing[28] = IT66341_GetRxSta(l_u8RxRealPort);
        #elif (IS_IT663XX==IT6634)
        pu8Timing[28] = IT6634_GetRxSta(l_u8RxRealPort);
        #elif (IS_IT663XX==IT66321)
        pu8Timing[28] = IT66321_GetRxSta(l_u8RxRealPort);
        #endif
    }

    iT663XX_DevSel(u8IteDevSave);

    return l_u8Vic;
}

#if _ENABLE_INPUT_PORT_HDCP_FOURCE_ON_14_OR_22
void mapp_IT663xxSoftReboot(void)
{
    #if Project_SCU61E_66341_AUTO_SWITCH
    SetAutoSwDetectTime(_TIME_AUTO_SWTICH_RES_CHNGED);
    #endif
    gCurSysStatus = 0;
}
#endif

#endif //#if(_PRJ_TYPE_SWITCH_SPLITER==_PRJ_SWITCHER_AND_SPLITER)||(_PRJ_TYPE_SWITCH_SPLITER==_PRJ_SWITCHER_ONLY)

#elif (IT663XX_VER == IT663XX_VER_1_07)
#include "../USER/video_chip/IT663xx_1.07/ITE663XX_func_api.c"
#endif

