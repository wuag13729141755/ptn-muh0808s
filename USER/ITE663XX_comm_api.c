#include "includes.h"
#if ((IT663XX_VER != IT663XX_VER_1_07)&&(IT663XX_VER != IT663XX_NONE))
#if(_PRJ_TYPE_SWITCH_SPLITER==_PRJ_SWITCHER_AND_SPLITER)||(_PRJ_TYPE_SWITCH_SPLITER==_PRJ_SWITCHER_ONLY)

iTE_u8 					gCurSysStatus = 0;
iTE_u8  				u8IntEvent =0;
iTE_u16 				g_u16Ite66341TxHdcpStatus[NUM_IT663XX_USE] = {0}; //assign the hdcp status of ite66341 tx port
#if Project_SCU61E_66341_AUTO_SWITCH
volatile iTE_u32 		g_vu32AutoDectTimeCnt = 1;//_TIME_AUTO_SWTICH_RES_CHNGED;//
#endif

bool 					g_bAutoSwPowerOnStatus = TRUE;//do not need to switch when (1)switch MANU to AUTO (2)REMEMBER STATUS BEFORE POWER OFF
bool					g_bIt663xxSpdifEnable = FALSE, g_bIt663xxIISEnable = FALSE;

FuncAutoSwitchHook		FuncAutoSwHook = NULL;//hook function after auto switched

#if (_EN_CEC_POWER_ON_TV_DETECTED_SIGNAL==_MACRO_ENABLE)
bool g_bCecOnTVDetectedNewSignal = FALSE;
#endif

sTxInfo** g_ppstTxInfoComm;
sRxInfo** g_ppstRxInfoComm;
iTE_u16*  g_pu16TxPortStaComm;
iTE_u16*  g_pu16TxVdoStaComm;
iTE_u8*	  g_pu8RxEdidComm;
iTE_u16*  g_pu16TxFunComm;
iTE_u8*	  g_pu8AudCodecComm;
iTE_u32*  g_pu32RxAdoDecInfoComm;
iTE_u32*  g_pu32TxAdoEncConfComm;
iTE_u8*   g_pu8TxAdoEncSettingComm;
iTE_u8*   g_pu8RxInfoCAComm;
iTE_u8*   g_pu8TxInfoCAComm;
iTE_u8*	  g_pu8RxSelComm;
iTE_u8*	  g_pu8TxSelComm;
iTE_u8*   g_pu8RxHdcp2StaComm;
bool*	  g_pbInAudChgFlgComm;
iTE_u16*  g_pu16CurTxFunComm;
#if (EN_MHL == iTE_TRUE)
sMhlInfo* g_pstMRxComm;
iTE_u8*	  g_pu83DInforComm;
#endif
iTE_u8*	  g_pu8RxReg44Comm;
iTE_u32*  g_pu32RCLKComm;
iTE_u8*   g_pu8Hdcp2ChkCntComm;
iTE_u8*   g_pu8CurRxAdrComm;
iTE_u8*   g_pu8CurActiveRxComm;
iTE_u8*   g_pu8CurRxPortComm;
iTE_u8*   g_pu8ConvSplComm;
iTE_u8*	  g_pu8TxHdcp2StaComm;
iTE_u8*	  g_pu8FsChkCntComm;
iTE_u8*	  g_pu8SysModeCntComm;
iTE_u8*	  g_pu8CurEdidAdrComm;
iTE_u8*	  g_pu8CurTxPortComm;
iTE_u8*   g_pu8RxHdmiModeComm;
iTE_u8*   g_pu8CurRxFunCom;
iTE_u8*   g_pu8RxFunComm;

iTE_u8*   g_pu8CurTxAdrCom;
#if _ENABLE_SWITCH_PORT_TO_MUTE_I2S_AUDIO
bool*	  g_pbSwitchAudChgFlgComm;
#endif
#if ENABLE_IT66341_DVI_EDID_TO_CHANGE_HDCP14
iTE_u8 g_u8EdidDviMode[NUM_IT663XX_USE][IN_RXn];
iTE_u8*  g_pu8EdidDviModeComm;
#endif

tx_hdcp_force_t*  g_ptIt663xxTxHdcpTypeComm;
tx_hdcp_force_t*  g_ptPreIt663xxTxHdcpTypeComm;
iTE_u16* 		  g_pu16Ite663xxTxHdcpStatusComm;

iTE_u8	  g_it663xx_device;
bool		g_bIt663xxAutoSwRunFlag = TRUE;

iTE_u8	g_u8It663xxDumpReg = 0;

#if	1//((IS_IT663XX == IT66341))
extern iTE_u8	g_u8TxHdcp2Sta[NUM_IT663XX_USE];
extern iTE_u8	g_u8AudCodec[NUM_IT663XX_USE];
extern iTE_u16 g_u16TxPortSta[NUM_IT663XX_USE][3];
extern iTE_u16	g_u16TxVdoSta[NUM_IT663XX_USE][3];
extern iTE_u8	g_u8RxEdid[NUM_IT663XX_USE][4];
extern iTE_u8	g_u8SysModeCnt[NUM_IT663XX_USE];
extern iTE_u8	g_u8TxEdidChg;
extern iTE_u8	g_u8SupAdoChg;
extern iTE_u32 g_u32RxAdoDecInfo[NUM_IT663XX_USE];
extern iTE_u8	g_u8RxInfoCA[NUM_IT663XX_USE];
extern iTE_u8	g_u8CurActiveRx[NUM_IT663XX_USE];
extern iTE_u8	g_u8TxShift;
extern iTE_u32 g_u32TxAdoEncConf[NUM_IT663XX_USE];
extern iTE_u32 g_u32RCLK[NUM_IT663XX_USE];
extern iTE_u8	g_u8TxInfoCA[NUM_IT663XX_USE];
extern iTE_u8	g_u8Hdcp2ChkCnt[NUM_IT663XX_USE];
extern iTE_u8	g_u8RxReg44[NUM_IT663XX_USE][4];
extern iTE_u8	g_u8ConvSpl[NUM_IT663XX_USE];
extern iTE_u8	g_u8FsChkCnt[NUM_IT663XX_USE];
extern EdidInfo 	g_stTxEdidInfoAry[NUM_IT663XX_USE][1], g_stRxEdidInfoAry[NUM_IT663XX_USE][4];
extern sTxInfo stTxInfo[NUM_IT663XX_USE][1];
extern sRxInfo	RxInfo[NUM_IT663XX_USE][4];
extern iTE_u8	g_u8RxHdcp2Sta[NUM_IT663XX_USE];
#if (EN_MHL == iTE_TRUE)
extern sMhlInfo	g_stMRx[NUM_IT663XX_USE];
extern iTE_u8	u83DInfor[NUM_IT663XX_USE][EDID_3D_SUPP_MAX];
extern MSC_WB_INFO stMhlMscWb;
#endif
extern iTE_u8	g_u8It663xxDumpReg;
extern iTE_u8 	g_u8It663xxRxHdcpState[NUM_IT663XX_USE];
extern iTE_u32 g_u32It663xxRxStaCheckCnt[NUM_IT663XX_USE];
extern iTE_u8 	g_u8It663xxPreSigSta[NUM_IT663XX_USE];
extern iTE_u32 g_u32It663xxPreAudSta[NUM_IT663XX_USE];
extern iTE_u16 g_u16AudioProDelayTick[NUM_IT663XX_USE];
#endif
extern iTE_u8 g_u8CurRxPort[NUM_IT663XX_USE];
extern iTE_u8 g_u8CurTxPort[NUM_IT663XX_USE];
extern iTE_u8	g_u8RxFun[NUM_IT663XX_USE][4];

const iTE_u8 hdmi_input_logic_index_table[] =
{
	_HDMI_INPUT_1_LOGIC_INDEX,
	_HDMI_INPUT_2_LOGIC_INDEX,
	_HDMI_INPUT_3_LOGIC_INDEX,
	_HDMI_INPUT_4_LOGIC_INDEX,
	_HDMI_INPUT_5_LOGIC_INDEX,
	_HDMI_INPUT_6_LOGIC_INDEX,
	_HDMI_INPUT_7_LOGIC_INDEX,
};

const iTE_u8 hdmi_input_node_rx_table[] =
{
	_HDMI_INPUT_NODE_RX_ITE_DEV_0,
	_HDMI_INPUT_NODE_RX_ITE_DEV_1,
	_HDMI_INPUT_NODE_RX_ITE_DEV_2,

};

#if _ENABLE_IT663XX_MULTI_CHIP_IN_USE
const stInputRxIndex_t RxPortMap[] =
{
    // u8DevNum ,  u8PortIndex
    {_HDMI_INPUT_1_DEV, _HDMI_INPUT_1_RX_PORT},
    {_HDMI_INPUT_2_DEV, _HDMI_INPUT_2_RX_PORT},
    {_HDMI_INPUT_3_DEV, _HDMI_INPUT_3_RX_PORT},
    {_HDMI_INPUT_4_DEV, _HDMI_INPUT_4_RX_PORT},
    {_HDMI_INPUT_5_DEV, _HDMI_INPUT_5_RX_PORT},
    {_HDMI_INPUT_6_DEV, _HDMI_INPUT_6_RX_PORT},
    {_HDMI_INPUT_7_DEV, _HDMI_INPUT_7_RX_PORT},
    {_HDMI_INPUT_8_DEV, _HDMI_INPUT_8_RX_PORT},
};
#endif

#if (_PRJ_TYPE_SWITCH_SPLITER==_PRJ_SWITCHER_ONLY)
	#if	(IS_IT663XX == IT66321)
	iTE_u8  gIte66341CurEdidMode = (RX_EDID_SPECIFIC_PA_CLR | RX_EDID_MODE_COPY_TX1);
	#elif (IS_IT663XX == IT66341)
	iTE_u8  gIte66341CurEdidMode = (RX_EDID_SPECIFIC_PA_CLR | RX_EDID_MODE_COPY_TX0);
	#elif (IS_IT663XX == IT6634)
	//add your code
	#endif
#else
iTE_u8  gIte66341CurEdidMode = (RX_EDID_SPECIFIC_PA_DEFAULT | RX_EDID_MODE_DEFAULT_0);
#endif

#if _ENABLE_DEBUG_LOAD_ITE_CHIP_REGISTER
bool g_bLoadIteChipRegisterFlag = FALSE;
#endif

#if ENABLE_IT6634_SWITCH_EDID_LONG_TIME_HPD_LOW_TO_HIGH
iTE_u1 g_u1IsEdidSwitchMode[4] = {FALSE,FALSE,FALSE,FALSE};
#endif

extern iTE_u8   g_u8AdoOut;

extern iTE_u8 const ExtendEdidTable[18];
extern stProjectGlobalAllVar_t g_stPrjGloVar;
extern const stRxHpdDetDeclare_t stProjectRxHpdDetTable[];
extern const stIT663xxRstDeclare_t stProjectIT663xxRstTable[];
SemaphoreHandle_t   xSemaphore_It663xxIrqSnd;
SemaphoreHandle_t   xSemaphore_It663xxSysInitSnd;

void It663xxIrq_xSemaphoreInit(void)
{
#if _SYSTEM_SUPPORT_RTOS
    xSemaphore_It663xxIrqSnd = xSemaphoreCreateMutex();
#endif
}

void It663xxIrq_xSemaphoreTask(void)
{
#if _SYSTEM_SUPPORT_RTOS
    xSemaphoreTake(xSemaphore_It663xxIrqSnd, portMAX_DELAY);
#endif
}

void It663xxIrq_xSemaphoreGive(void)
{
#if _SYSTEM_SUPPORT_RTOS
    xSemaphoreGive(xSemaphore_It663xxIrqSnd);
#endif
}

void It663xxSysInit_xSemaphoreInit(void)
{
#if _SYSTEM_SUPPORT_RTOS
    xSemaphore_It663xxSysInitSnd = xSemaphoreCreateMutex();
#endif
}

void It663xxSysInit_xSemaphoreTask(void)
{
#if _SYSTEM_SUPPORT_RTOS
    xSemaphoreTake(xSemaphore_It663xxSysInitSnd, portMAX_DELAY);
#endif
}

void It663xxSysInit_xSemaphoreGive(void)
{
#if _SYSTEM_SUPPORT_RTOS
    xSemaphoreGive(xSemaphore_It663xxSysInitSnd);
#endif
}

void DumpAllBankReg(iTE_u8 u8Adr)
{
	iTE_u8	i, u8RegData[0x10];
	for(i=0; i < 0x10; i++)
	{
		i2c_read_byte( u8Adr, i << 4, 0x10, u8RegData, u8I2CBus);
		iTE_MsgReg(("0x%02X: %02X %02X %02X %02X   "\
			"%02X %02X %02X %02X   %02X %02X %02X %02X   "\
			"%02X %02X %02X %02X\r\n",\
			i<<4, u8RegData[0], u8RegData[1], u8RegData[2],\
			u8RegData[3], u8RegData[4], u8RegData[5], u8RegData[6],\
			u8RegData[7], u8RegData[8], u8RegData[9], u8RegData[10],\
			u8RegData[11], u8RegData[12], u8RegData[13], u8RegData[14],\
			u8RegData[15]));
	}
}

void ChangeBank(iTE_u8 u8Adr, iTE_u8 u8Mask, iTE_u8 u8Bank)
{
	iTE_u8	u8RegData;
	i2c_read_byte(u8Adr, 0x0F, 1, &u8RegData, u8I2CBus);
	u8RegData &= u8Mask;
	u8RegData |= u8Bank;
	i2c_write_byte(u8Adr, 0x0F, 1, &u8RegData, u8I2CBus);
}

iTE_u1 HOLD_STATUS(void)
{

#if _ENABLE_DEBUG_LOAD_ITE_CHIP_REGISTER
    if(g_bLoadIteChipRegisterFlag == TRUE)
        return 1;
#endif
	return 0;
}

void ExtInt_Enable(iTE_u1 bEnable)
{
	bEnable = bEnable;
}

iTE_u1 HoldSystem(void)
{
	if(HOLD_STATUS())
	{
		if(g_u8It663xxDumpReg)
		{
			iTE_u8	u8Cnt1;
            iTE_u8  l_u8Index;
            for(l_u8Index=0; l_u8Index<NUM_IT663XX_USE; l_u8Index++)
            {
                iT663XX_DevSel(IT663XX_Device[l_u8Index]);

    			for(u8Cnt1 = 0; u8Cnt1 < 4; u8Cnt1++)
    			{
    				iTE_MsgReg(("HDMI_Sw Bank%d:\r\n", u8Cnt1));
    				ChangeBank(SW_ADR+u8I2CAdr, 0xFC, u8Cnt1);
    				DumpAllBankReg(SW_ADR+u8I2CAdr);
    			}
    			ChangeBank(SW_ADR+u8I2CAdr, 0xFC, 0x00);

    			for(u8Cnt1 = 0; u8Cnt1 < 4; u8Cnt1++)
    			{
    				iTE_MsgReg(("HDMI_Rx0 Bank%d:\r\n", u8Cnt1));
    				ChangeBank(RX0_ADR+u8I2CAdr, 0xF8, u8Cnt1);
    				DumpAllBankReg(RX0_ADR+u8I2CAdr);
    			}
    			ChangeBank(RX0_ADR+u8I2CAdr, 0xF8, 0x00);

                #if ((IS_IT663XX == IT6634)||(IS_IT663XX == IT66341))
    			for(u8Cnt1 = 0; u8Cnt1 < 4; u8Cnt1++)
    			{
    				iTE_MsgReg(("HDMI_Rx1  Bank%d:\r\n", u8Cnt1));
    				ChangeBank(RX1_ADR+u8I2CAdr, 0xF8, u8Cnt1);
    				DumpAllBankReg(RX1_ADR+u8I2CAdr);
    			}
    			ChangeBank(RX1_ADR, 0xF8, 0x00);
                #endif

    			for(u8Cnt1 = 0; u8Cnt1 < 4; u8Cnt1++)
    			{
    				iTE_MsgReg(("HDMI_Rx2  Bank%d:\r\n", u8Cnt1));
    				ChangeBank(RX2_ADR+u8I2CAdr, 0xF8, u8Cnt1);
    				DumpAllBankReg(RX2_ADR+u8I2CAdr);
    			}
    			ChangeBank(RX2_ADR+u8I2CAdr, 0xF8, 0x00);

                #if ((IS_IT663XX == IT6634)||(IS_IT663XX == IT66341))
    			for(u8Cnt1 = 0; u8Cnt1 < 4; u8Cnt1++)
    			{
    				iTE_MsgReg(("HDMI_Rx3  Bank%d:\r\n", u8Cnt1));
    				ChangeBank(RX3_ADR+u8I2CAdr, 0xF8, u8Cnt1);
    				DumpAllBankReg(RX3_ADR+u8I2CAdr);
    			}
    			ChangeBank(RX3_ADR+u8I2CAdr, 0xF8, 0x00);
                #endif

                #if ((IS_IT663XX == IT6634)||(IS_IT663XX == IT66341))
    			iTE_MsgReg(("HDMI_Tx0 :\r\n"));
				DumpAllBankReg(TX0_ADR+u8I2CAdr);
                #endif

                #if ((IS_IT663XX == IT6634)||(IS_IT663XX == IT66321))
    			iTE_MsgReg(("HDMI_Tx1 :\r\n"));
				DumpAllBankReg(TX1_ADR+u8I2CAdr);
                #endif

                #if (IS_IT663XX == IT6634)
    			iTE_MsgReg(("HDMI_Tx2 :\r\n"));
				DumpAllBankReg(TX2_ADR+u8I2CAdr);
                #endif
            }

			g_u8It663xxDumpReg = 0;

            iTE_MsgReg(("Hold\\\r"));
            iTE_MsgReg(("Hold-\r"));
            iTE_MsgReg(("Hold/\r"));
            iTE_MsgReg(("Hold|\r"));
		}

		return iTE_TRUE;
	}
	else
	{
		g_u8It663xxDumpReg = 1;
		return iTE_FALSE;
	}
}

void IT663xx_GloVarInit(void)
{
	#if	((IS_IT663XX == IT66341))
	memset(g_stTxEdidInfoAry, 0, (sizeof(EdidInfo)*NUM_IT663XX_USE*1));
	memset(g_stRxEdidInfoAry, 0, (sizeof(EdidInfo)*NUM_IT663XX_USE*4));
	memset(stTxInfo, 0, (sizeof(sTxInfo)*NUM_IT663XX_USE*1));
	memset(RxInfo, 0, (sizeof(sRxInfo)*NUM_IT663XX_USE*4));
	memset(g_u16TxFun, 0, (sizeof(iTE_u16)*NUM_IT663XX_USE*3));
	memset(g_u8AudCodec, 0, NUM_IT663XX_USE);
	memset(g_u16TxPortSta, 0, (sizeof(iTE_u16)*NUM_IT663XX_USE*3));
	memset(g_u16TxVdoSta, 0, (sizeof(iTE_u16)*NUM_IT663XX_USE*3));
	memset(g_u8RxEdid, 0, (NUM_IT663XX_USE*4));
	//memset(g_u8RxSel, IN_RX0, NUM_IT663XX_USE);
	memset(g_u8SysModeCnt, 0, NUM_IT663XX_USE);
	memset(g_u8RxHdcp2Sta, 0, NUM_IT663XX_USE);
	memset(&g_u8TxEdidChg, 0, 1);
	memset(&g_u8SupAdoChg, 0, 1);
	memset(g_u8CurEdidAdr, 0, NUM_IT663XX_USE);
	memset(g_u32RxAdoDecInfo, 0, (sizeof(iTE_u32)*NUM_IT663XX_USE));
	memset(g_u8RxInfoCA, 0, NUM_IT663XX_USE);
	memset(g_u8CurRxAdr, 0, NUM_IT663XX_USE);
	memset(g_u8CurActiveRx, 0, NUM_IT663XX_USE);
	memset(g_u8CurRxPort, 0, NUM_IT663XX_USE);
	#if HdmiTxVideoModeBypassFromRx
	memset(g_u8RxHdmiMode, HDMI, (sizeof(iTE_u16)*NUM_IT663XX_USE*IN_RXn));
	#endif
	memset(&gCurTxAdr, 0, NUM_IT663XX_USE);
	memset(g_u8CurTxPort, 0, NUM_IT663XX_USE);
	memset(&g_u8TxShift, 0, 1);
	memset(g_u8TxHdcp2Sta, 0, NUM_IT663XX_USE);
	memset(g_u16CurTxFun, 0, (sizeof(iTE_u16)*NUM_IT663XX_USE));
	memset(g_u32TxAdoEncConf, 0, (sizeof(iTE_u32)*NUM_IT663XX_USE));
	memset(g_u32RCLK, 0, (sizeof(iTE_u32)*NUM_IT663XX_USE));
	memset(g_u8TxInfoCA, 0, NUM_IT663XX_USE);
	memset(g_u8Hdcp2ChkCnt, 0, NUM_IT663XX_USE);
	memset(g_u8RxReg44, 0, NUM_IT663XX_USE);
	memset(g_u8ConvSpl, 0, NUM_IT663XX_USE);
	memset(g_u8FsChkCnt, 0, NUM_IT663XX_USE);
	#if (EN_MHL == iTE_TRUE)
	memset(g_stMRx, 0, (sizeof(sMhlInfo)*NUM_IT663XX_USE));
	memset(u83DInfor, 0, (NUM_IT663XX_USE*EDID_3D_SUPP_MAX));
	memset(&stMhlMscWb, 0, (sizeof(MSC_WB_INFO)));
	#endif

	memset(&g_u8It663xxDumpReg, 0, 1);
	memset(g_u8It663xxRxHdcpState, 0, NUM_IT663XX_USE);
	memset(g_u32It663xxRxStaCheckCnt, 0, (sizeof(iTE_u32)*NUM_IT663XX_USE));
	memset(g_u8It663xxPreSigSta, 0, NUM_IT663XX_USE);
	memset(g_u32It663xxPreAudSta, 0, (sizeof(iTE_u32)*NUM_IT663XX_USE));
	memset(g_tIt663xxTxHdcpType, tx_hdcp_force_off, (sizeof(tx_hdcp_force_t)*NUM_IT663XX_USE));
	g_tIt663xxTxHdcpType[0] = _PRJ_TX_HDCP_MODE_ACTIVE;
	memset(g_tPreIt663xxTxHdcpType, tx_hdcp_force_max, (sizeof(tx_hdcp_force_t)*NUM_IT663XX_USE));
	memset(g_u16AudioProDelayTick, 0, (sizeof(iTE_u16)*NUM_IT663XX_USE));
	memset(g_u16Ite66341TxHdcpStatus, 0, (sizeof(iTE_u16)*NUM_IT663XX_USE));

	memset(g_u8RxFun, 0, (sizeof(iTE_u16)*NUM_IT663XX_USE*4));

    #elif (IS_IT663XX == IT66321)
	memset(g_stTxEdidInfoAry, 0, (sizeof(EdidInfo)*NUM_IT663XX_USE*1));
	memset(g_stRxEdidInfoAry, 0, (sizeof(EdidInfo)*NUM_IT663XX_USE*2));
	memset(stTxInfo, 0, (sizeof(sTxInfo)*NUM_IT663XX_USE*1));
	memset(RxInfo, 0, (sizeof(sRxInfo)*NUM_IT663XX_USE*2));
	memset(g_u16TxFun, 0, (sizeof(iTE_u16)*NUM_IT663XX_USE*3));
	memset(g_u8AudCodec, 0, NUM_IT663XX_USE);
	memset(g_u16TxPortSta, 0, (sizeof(iTE_u16)*NUM_IT663XX_USE*3));
	memset(g_u16TxVdoSta, 0, (sizeof(iTE_u16)*NUM_IT663XX_USE*3));
	memset(g_u8RxEdid, 0, (NUM_IT663XX_USE*4));
	//memset(g_u8RxSel, IN_RX0, NUM_IT663XX_USE);
	memset(g_u8SysModeCnt, 0, NUM_IT663XX_USE);
	memset(g_u8RxHdcp2Sta, 0, NUM_IT663XX_USE);
	memset(&g_u8TxEdidChg, 0, 1);
	memset(&g_u8SupAdoChg, 0, 1);
	memset(g_u8CurEdidAdr, 0, NUM_IT663XX_USE);
	memset(g_u32RxAdoDecInfo, 0, (sizeof(iTE_u32)*NUM_IT663XX_USE));
	memset(g_u8RxInfoCA, 0, NUM_IT663XX_USE);
	memset(g_u8CurRxAdr, 0, NUM_IT663XX_USE);
	memset(g_u8CurActiveRx, 0, NUM_IT663XX_USE);
	memset(g_u8CurRxPort, 0, NUM_IT663XX_USE);
	#if HdmiTxVideoModeBypassFromRx
	memset(g_u8RxHdmiMode, HDMI, (sizeof(iTE_u16)*NUM_IT663XX_USE*IN_RXn));
	#endif
	memset(&gCurTxAdr, 0, NUM_IT663XX_USE);
	memset(g_u8CurTxPort, 0, NUM_IT663XX_USE);
	memset(&g_u8TxShift, 0, 1);
	memset(g_u8TxHdcp2Sta, 0, NUM_IT663XX_USE);
	memset(g_u16CurTxFun, 0, (sizeof(iTE_u16)*NUM_IT663XX_USE));
	memset(g_u32TxAdoEncConf, 0, (sizeof(iTE_u32)*NUM_IT663XX_USE));
	memset(g_u32RCLK, 0, (sizeof(iTE_u32)*NUM_IT663XX_USE));
	memset(g_u8TxInfoCA, 0, NUM_IT663XX_USE);
	memset(g_u8Hdcp2ChkCnt, 0, NUM_IT663XX_USE);
	memset(g_u8RxReg44, 0, NUM_IT663XX_USE);
	memset(g_u8ConvSpl, 0, NUM_IT663XX_USE);
	memset(g_u8FsChkCnt, 0, NUM_IT663XX_USE);
	#if (EN_MHL == iTE_TRUE)
	memset(g_stMRx, 0, (sizeof(sMhlInfo)*NUM_IT663XX_USE));
	memset(u83DInfor, 0, (NUM_IT663XX_USE*EDID_3D_SUPP_MAX));
	memset(&stMhlMscWb, 0, (sizeof(MSC_WB_INFO)));
	#endif

	memset(&g_u8It663xxDumpReg, 0, 1);
	memset(g_u8It663xxRxHdcpState, 0, NUM_IT663XX_USE);
	memset(g_u32It663xxRxStaCheckCnt, 0, (sizeof(iTE_u32)*NUM_IT663XX_USE));
	memset(g_u8It663xxPreSigSta, 0, NUM_IT663XX_USE);
	memset(g_u32It663xxPreAudSta, 0, (sizeof(iTE_u32)*NUM_IT663XX_USE));
	memset(g_tIt663xxTxHdcpType, tx_hdcp_force_off, (sizeof(tx_hdcp_force_t)*NUM_IT663XX_USE));
	g_tIt663xxTxHdcpType[0] = _PRJ_TX_HDCP_MODE_ACTIVE;
	memset(g_tPreIt663xxTxHdcpType, tx_hdcp_force_max, (sizeof(tx_hdcp_force_t)*NUM_IT663XX_USE));
	memset(g_u16AudioProDelayTick, 0, (sizeof(iTE_u16)*NUM_IT663XX_USE));
	memset(g_u16Ite66341TxHdcpStatus, 0, (sizeof(iTE_u16)*NUM_IT663XX_USE));

	memset(g_u8RxFun, 0, (sizeof(iTE_u16)*NUM_IT663XX_USE*4));
	#endif


    #if ENABLE_IT66341_DVI_EDID_TO_CHANGE_HDCP14
	memset(g_u8EdidDviMode, 0, (sizeof(iTE_u16)*NUM_IT663XX_USE*IN_RXn));
    #endif

    #if ENABLE_IT6634_SWITCH_EDID_LONG_TIME_HPD_LOW_TO_HIGH
    memset(g_u1IsEdidSwitchMode, FALSE, 4);
    #endif

	{
        u8 i;
        for(i=0;i<_HDMI_INPUT_PORT_USED_NUMBER;i++)
        {
            if((1<<i)&PortResetForEdid)
                g_bIt663xxChangeRxEdidFlag[i] = TRUE;
            else
                g_bIt663xxChangeRxEdidFlag[i] = FALSE;
        }
	}

}

void MCU_Init(void)
{
	pstIT663xxRstDeclare_t pstIteRst, pstIteRstOrg = (pstIT663xxRstDeclare_t)&stProjectIT663xxRstTable[0];
    iTE_Msg(("Start to init it663xx!!\r\n"));

	IT663xx_GloVarInit();
    if(_HDMI_INPUT_TYPE_C_PORT != _HDMI_INPUT_NONE_BIT_INDEX)
    {
        SetTypeCResetPin();
    }

	pstIteRst = pstIteRstOrg;
	while(pstIteRst->pstRstLocate != NULL)
	{
		if(pstIteRst->eRstLevel == ePinLevel_Low)
		{
			GPIO_ResetBits(STM32_GPIO_PORT[pstIteRst->pstRstLocate->u8PortType-PORT_GPIOA], pstIteRst->pstRstLocate->u16GpioPin);
		}
		else
		{
			GPIO_SetBits(STM32_GPIO_PORT[pstIteRst->pstRstLocate->u8PortType-PORT_GPIOA], pstIteRst->pstRstLocate->u16GpioPin);
		}

		pstIteRst ++;
	}

	iTE_Sleep_ms(50);

	pstIteRst = pstIteRstOrg;
	while(pstIteRst->pstRstLocate != NULL)
	{
		if(pstIteRst->eRstLevel == ePinLevel_High)
		{
			GPIO_ResetBits(STM32_GPIO_PORT[pstIteRst->pstRstLocate->u8PortType-PORT_GPIOA], pstIteRst->pstRstLocate->u16GpioPin);
		}
		else
		{
			GPIO_SetBits(STM32_GPIO_PORT[pstIteRst->pstRstLocate->u8PortType-PORT_GPIOA], pstIteRst->pstRstLocate->u16GpioPin);
		}

		pstIteRst ++;
	}

	iTE_Sleep_ms(500);

	#if Project_SCU61E_66341_AUTO_SWITCH
		#if (_EN_EEPROM_FUN_SAVE_HDMI_SEL==_MACRO_ENABLE)
		SetAutoSwDetectTime(_TIME_AUTO_SWTICH_RES_CHNGED);
		#endif
	#endif

    #if _ENABLE_HDMITX_HDCP_FUNCTION
    {
        pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
        pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
        g_tIt663xxTxHdcpType[0] = (tx_hdcp_force_t)pstPrjSaveVar->stHdcpStatusOp.u8HdcpSta[0];
	}
    #endif

    #if (_DETECT_TYPE_AUTO_SWITCH == _AUTO_SWITCH_DETECT_TYPE_HPD_AND_TMDS)
    mapp_DetectModeInit();
    #endif
    #if _ENABLE_IT663XX_EDID_CONTROL
    {   // add by wf8421 20180119 Audio config to saved variable
        u8 l_u8DevID;
        #if _ENABLE_EDID_SWITCH_A_SINGLE_CHANNEL
        u8 l_u8RxPort;
        #endif
        #if (_DISABLE_EDID_BYPASS == DISABLE)
        pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
        pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
        #endif

        for(l_u8DevID=0; l_u8DevID<NUM_IT663XX_USE; l_u8DevID++)
        {
            #if _ENABLE_EDID_SWITCH_A_SINGLE_CHANNEL
            for(l_u8RxPort=0;l_u8RxPort<IN_RXn;l_u8RxPort++)
            {
                #if (_DISABLE_EDID_BYPASS == DISABLE)
                if(pstPrjSaveVar->stEdidSelIndex.u8EdidFlashIndex == EDID_MODE_BYPASS)
                {
                    SetIT663xxEdidSource(l_u8DevID,l_u8RxPort, eIT663xxGetEdidFrom_Tx);
                }
                else
                #endif
                {
                    SetIT663xxEdidSource(l_u8DevID,l_u8RxPort, eIT663xxGetEdidFrom_Buffer);
                }
            }
            #else
            #if (_DISABLE_EDID_BYPASS == DISABLE)
            if(pstPrjSaveVar->stEdidSelIndex.u8EdidFlashIndex == EDID_MODE_BYPASS)
            {
                SetIT663xxEdidSource(l_u8DevID, eIT663xxGetEdidFrom_Tx);
            }
            else
            #endif
            {
                SetIT663xxEdidSource(l_u8DevID, eIT663xxGetEdidFrom_Buffer);
            }
            #endif
        }
        #if _ENABLE_SAVE_CURRENT_EDID
        //memcpy(g_pu8It663xxEdidBuffer,pstPrjSaveVar->stEdidSelIndex.u8CurEdidbuf,256);
        mapp_StarupEdidInit();
        #endif
    }
    #else
    {
        u8 l_u8DevID;
        for(l_u8DevID=0; l_u8DevID<NUM_IT663XX_USE; l_u8DevID++)
        {
            SetIT663xxEdidSource(l_u8DevID, eIT663xxGetEdidFrom_Tx);
        }
    }
    #endif
}

#if (SUH4AU_H2_EDID_HANDLE == _MACRO_ENABLE)
void IT663XX_Reset_for_Edid(eIT663xxGetEdidSrc_t eCopySrc)
{
	uint8_t i;
    #if _ENABLE_EDID_SWITCH_A_SINGLE_CHANNEL
    u8 l_u8RxPort;
    #endif

	for(i=0; i<NUM_IT663XX_USE; i++)
	{
        #if _ENABLE_EDID_SWITCH_A_SINGLE_CHANNEL
        for(l_u8RxPort=0;l_u8RxPort<IN_RXn;l_u8RxPort++)
        {
		    SetIT663xxEdidSource(i,l_u8RxPort, eCopySrc);
        }
        #else
		SetIT663xxEdidSource(i, eCopySrc);
        #endif
	}

    #if Project_SCU61E_66341_AUTO_SWITCH
	SetAutoSwDetectTime(_TIME_AUTO_SWTICH_RES_CHNGED);
    #endif
	g_bIt663xxSetRxEdidFlg[0] = TRUE;
}
#elif _ENABLE_EDID_SWITCH_USE_NEW_MODE
void IT663XX_Reset_for_Edid(eIT663xxGetEdidSrc_t eCopySrc, uint32_t u32BitIndRx)
{
	uint8_t i;

    #if Project_SCU61E_66341_AUTO_SWITCH
	SetAutoSwDetectTime(_TIME_AUTO_SWTICH_RES_CHNGED);
    #endif
    mapp_EdidChange();
	for(i=0; i<_HDMI_INPUT_PORT_USED_NUMBER; i++)
	{
		if(u32BitIndRx & (1<<i))
		{
			g_bIt663xxChangeRxEdidFlag[i] = TRUE;
		}
	}

}
#elif _ENABLE_EDID_SWITCH_UNREBOOT
void IT663XX_Reset_for_Edid(eIT663xxGetEdidSrc_t eCopySrc, uint32_t u32BitIndRx)
{
    uint8_t l_u8DevID,l_u8RxPort;
    uint8_t l_u8CopyToTx;

    #if (IS_IT663XX == IT66341)
    l_u8CopyToTx = RX_EDID_MODE_COPY_TX0;
    #elif (IS_IT663XX == IT66321)
    l_u8CopyToTx = RX_EDID_MODE_COPY_TX1;
    #else
    l_u8CopyToTx = RX_EDID_MODE_COPY_TX0;
    #endif

    for(l_u8DevID=0; l_u8DevID<NUM_IT663XX_USE; l_u8DevID++)
    {
        #if _ENABLE_EDID_SWITCH_A_SINGLE_CHANNEL
        for(l_u8RxPort=0;l_u8RxPort<IN_RXn;l_u8RxPort++)
        {
            SetIT663xxEdidSource(l_u8DevID,l_u8RxPort, eCopySrc);
            if(GetIT663xxEdidSource(l_u8DevID,l_u8RxPort) == eIT663xxGetEdidFrom_Tx)
            {
                if(g_bIt663xxChangeRxEdidFlag[mapp_GetCurrentRxPort(l_u8DevID,l_u8RxPort)]==FALSE)
                {
                    g_u8RxEdid[l_u8DevID][l_u8RxPort] = (RX_EDID_SPECIFIC_PA_CLR | RX_EDID_MODE_DEFAULT_1);
                }
                else
                {
                    g_u8RxEdid[l_u8DevID][l_u8RxPort] = (RX_EDID_SPECIFIC_PA_CLR | l_u8CopyToTx);
                }
            }
            else if(GetIT663xxEdidSource(l_u8DevID,l_u8RxPort) == eIT663xxGetEdidFrom_Buffer)
            {
                if(g_bIt663xxChangeRxEdidFlag[mapp_GetCurrentRxPort(l_u8DevID,l_u8RxPort)]==FALSE)
                {
                    g_u8RxEdid[l_u8DevID][l_u8RxPort] = (RX_EDID_SPECIFIC_PA_CLR | RX_EDID_MODE_DEFAULT_1);
                }
                else
                {
                    g_u8RxEdid[l_u8DevID][l_u8RxPort] = (RX_EDID_SPECIFIC_PA_CLR | RX_EDID_MODE_DEFAULT_0);
                }
            }

            #if _ENABLE_TYPE_C_EDID_BY_PASS_ONLY
            if(isIt663xxDeviceRxTypecPort(g_it663xx_device, l_u8RxPort))
            {
                g_u8RxEdid[l_u8DevID][l_u8RxPort] = (RX_EDID_SPECIFIC_PA_CLR | l_u8CopyToTx);
            }
            #endif
        }
        #else
        SetIT663xxEdidSource(l_u8DevID, eCopySrc);
        for(l_u8RxPort=0;l_u8RxPort<IN_RXn;l_u8RxPort++)
        {
    		if(GetIT663xxEdidSource(l_u8DevID) == eIT663xxGetEdidFrom_Tx)
    		{
                if(g_bIt663xxChangeRxEdidFlag[mapp_GetCurrentRxPort(l_u8DevID,l_u8RxPort)]==FALSE)
                {
                    g_u8RxEdid[l_u8DevID][l_u8RxPort] = (RX_EDID_SPECIFIC_PA_CLR | RX_EDID_MODE_DEFAULT_1);
                }
                else
                {
                    g_u8RxEdid[l_u8DevID][l_u8RxPort] = (RX_EDID_SPECIFIC_PA_CLR | l_u8CopyToTx);
                }
    		}
    		else if(GetIT663xxEdidSource(l_u8DevID) == eIT663xxGetEdidFrom_Buffer)
    		{
                if(g_bIt663xxChangeRxEdidFlag[mapp_GetCurrentRxPort(l_u8DevID,l_u8RxPort)]==FALSE)
                {
                    g_u8RxEdid[l_u8DevID][l_u8RxPort] = (RX_EDID_SPECIFIC_PA_CLR | RX_EDID_MODE_DEFAULT_1);
                }
                else
                {
                    g_u8RxEdid[l_u8DevID][l_u8RxPort] = (RX_EDID_SPECIFIC_PA_CLR | RX_EDID_MODE_DEFAULT_0);
                }
    		}
            #if _ENABLE_TYPE_C_EDID_BY_PASS_ONLY
            if(isIt663xxDeviceRxTypecPort(g_it663xx_device, l_u8RxPort))
            {
                g_u8RxEdid[l_u8DevID][l_u8RxPort] = (RX_EDID_SPECIFIC_PA_CLR | l_u8CopyToTx);
            }
            #endif
        }
        #endif
    }

    #if Project_SCU61E_66341_AUTO_SWITCH
    SetAutoSwDetectTime(_TIME_AUTO_SWTICH_RES_CHNGED);
    #endif
    g_bUserEdidChange = TRUE;
    #if 1
    for(l_u8RxPort=0; l_u8RxPort<_HDMI_INPUT_PORT_USED_NUMBER; l_u8RxPort++)
    {
        if(u32BitIndRx & (1<<l_u8RxPort))
        {
            g_bIt663xxSetRxEdidFlg[l_u8RxPort] = TRUE;
        }
    }
    #endif
}

#if _ENABLE_EDID_SWITCH_A_SINGLE_CHANNEL
void IT663XX_EdidSwitchForSingleChannel(eIT663xxGetEdidSrc_t eCopySrc, u8 u8InputPort)
{
    uint8_t l_u8DevID,l_u8RxPort;
    uint8_t l_u8CopyToTx;

    #if (IS_IT663XX == IT66341)
    l_u8CopyToTx = RX_EDID_MODE_COPY_TX0;
    #elif (IS_IT663XX == IT66321)
    l_u8CopyToTx = RX_EDID_MODE_COPY_TX1;
    #else
    l_u8CopyToTx = RX_EDID_MODE_COPY_TX0;
    #endif

    #if _ENABLE_IT663XX_MULTI_CHIP_IN_USE
    l_u8DevID = RxPortMap[u8InputPort].u8DevNum;
    l_u8RxPort = RxPortMap[u8InputPort].u8PortIndex;
    #else
    {
        u8 j;
        u8 l_u8RxLogicAddr;
        l_u8RxLogicAddr = hdmi_input_logic_index_table[u8InputPort];

    	for(j=0; j<NUM_IT663XX_USE; j++)
    	{
    		l_u8RxPort = ((l_u8RxLogicAddr>>(_HDMI_INPUT_PHYSIC_SHIFT*j))&_HDMI_INPUT_PHYSIC_MASK);
    		if(l_u8RxPort != hdmi_input_node_rx_table[j])
    		{
    			break;
    		}
    	}
        l_u8DevID = j;
    }
    #endif
    SetIT663xxEdidSource(l_u8DevID,l_u8RxPort, eCopySrc);
    if(GetIT663xxEdidSource(l_u8DevID,l_u8RxPort) == eIT663xxGetEdidFrom_Tx)
    {
        if(g_bIt663xxChangeRxEdidFlag[mapp_GetCurrentRxPort(l_u8DevID,l_u8RxPort)]==FALSE)
        {
            g_u8RxEdid[l_u8DevID][l_u8RxPort] = (RX_EDID_SPECIFIC_PA_CLR | RX_EDID_MODE_DEFAULT_1);
        }
        else
        {
            g_u8RxEdid[l_u8DevID][l_u8RxPort] = (RX_EDID_SPECIFIC_PA_CLR | l_u8CopyToTx);
        }
    }
    else if(GetIT663xxEdidSource(l_u8DevID,l_u8RxPort) == eIT663xxGetEdidFrom_Buffer)
    {
        if(g_bIt663xxChangeRxEdidFlag[mapp_GetCurrentRxPort(l_u8DevID,l_u8RxPort)]==FALSE)
        {
            g_u8RxEdid[l_u8DevID][l_u8RxPort] = (RX_EDID_SPECIFIC_PA_CLR | RX_EDID_MODE_DEFAULT_1);
        }
        else
        {
            g_u8RxEdid[l_u8DevID][l_u8RxPort] = (RX_EDID_SPECIFIC_PA_CLR | RX_EDID_MODE_DEFAULT_0);
        }
    }
#if _ENABLE_TYPE_C_EDID_BY_PASS_ONLY
    if(isIt663xxDeviceRxTypecPort(l_u8DevID, l_u8RxPort))
    {
        g_u8RxEdid[l_u8DevID][l_u8RxPort] = (RX_EDID_SPECIFIC_PA_CLR | l_u8CopyToTx);
    }
#endif

#if Project_SCU61E_66341_AUTO_SWITCH
    SetAutoSwDetectTime(_TIME_AUTO_SWTICH_RES_CHNGED);
#endif
    g_bUserEdidChange = TRUE;
    g_bIt663xxSetRxEdidFlg[u8InputPort] = TRUE;
}

#endif

#else
void IT663XX_Reset_for_Edid(eIT663xxGetEdidSrc_t eCopySrc, uint32_t u32BitIndRx)
{
	uint8_t i;
    #if _ENABLE_EDID_SWITCH_A_SINGLE_CHANNEL
    u8 l_u8RxPort;
    #endif

    for(i=0; i<NUM_IT663XX_USE; i++)
    {
    #if _ENABLE_EDID_SWITCH_A_SINGLE_CHANNEL
        for(l_u8RxPort=0;l_u8RxPort<IN_RXn;l_u8RxPort++)
        {
            SetIT663xxEdidSource(i,l_u8RxPort, eCopySrc);
        }
    #else
        SetIT663xxEdidSource(i, eCopySrc);
    #endif
    }

    #if Project_SCU61E_66341_AUTO_SWITCH
	SetAutoSwDetectTime(_TIME_AUTO_SWTICH_RES_CHNGED);
    #endif

	#if (Project_663xx_RST_METHOD_FOR_EDID_USE==_IT663XX_RST_METHOD_FOR_EDID_WIRTE)
	for(i=0; i<_HDMI_INPUT_PORT_USED_NUMBER; i++)
	{
		if(u32BitIndRx & (1<<i))
		{
			g_bIt663xxSetRxEdidFlg[i] = TRUE;
		}
	}
	#elif (Project_663xx_RST_METHOD_FOR_EDID_USE==_IT663XX_RST_METHOD_FOR_EDID_REBOOT)
		gCurSysStatus = 0;
	#endif
}
#endif

#if ((_EN_CEC_POWER_ON_TV_DETECTED_SIGNAL==_MACRO_ENABLE)&&(_PRJ_TYPE_SWITCH_SPLITER==_PRJ_SWITCHER_ONLY))
void IT663XX_PwrOnTvDtdSngl(void)
{
	//add code for power on tv by cec when detected signal

}
#endif
#if(_ENABLE_SWITCH_TO_USBC_EDID == _MACRO_ENABLE)
extern bool SwitchToUSBC;
#endif
void IT663XX_SwitchChannelByIndex(iTE_u8 u8LogicRxSel)
{
	iTE_u8 i, u8It663xxDevSave;
	pstProjectGlobalAllVar_t 	pstPrjAllVar = &g_stPrjGloVar;
	pstProjectGlobalSaveVar_t	pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
	pstHdmiSwitchParam_t		pstHdmiSW = &pstPrjSaveVar->stHdmiSel;
    #if(_ENABLE_SWITCH_TO_USBC_EDID == _MACRO_ENABLE)
    if(/*(g_bSwUnplugHpdSta == FALSE)&&*/(u8LogicRxSel == 2))
        SwitchToUSBC = TRUE;
    #endif
	if(pstHdmiSW->u8SwChannel == u8LogicRxSel)
	{
		return;
	}

	IT663XX_SetRxSelByLogic(u8LogicRxSel);
    #if _ENABLE_USB_SWITCH_BY_INPUT_PORT
    mapp_UsbSwitchFunction(u8LogicRxSel);
    #endif

	u8It663xxDevSave = g_it663xx_device;
	for(i=0; i<NUM_IT663XX_USE; i++)
	{
		iT663XX_DevSel(IT663XX_Device[i]);
		#if (IS_IT663XX==IT66341)
			IT66341_SwitchPort(g_pu8RxSelComm[0]);
		#elif (IS_IT663XX==IT6634)
			IT6634_SwitchPort(g_pu8TxSelComm[0], g_pu8RxSelComm[0]);
		#elif (IS_IT663XX==IT66321)
			IT66321_SwitchPort(g_pu8RxSelComm[0]);
		#endif
	}
	iT663XX_DevSel(u8It663xxDevSave);
}

#define SIGNAL_EXIST_DETECT_TIMES       20
#define POWER_ON_DETECT_TIMES           5
#if _ENABLE_IT663XX_MULTI_CHIP_IN_USE
void IT663XX_SourceAutoSwitch(void)
{
	iTE_u8 				i, j, rx_signal_sta = 0, cur_temp = 0, u8IteDevSave;
	iTE_u8 				u8Reg13[NUM_IT663XX_USE][IN_RXn], u8RxTmp;
	iTE_u8 				rx_hotplug_sta = 0;
	static iTE_u8 		pre_rx_signal_sta = 0;
	static bool 		pre_isAutoSwitchMode = TRUE;

	#if ((_DETECT_TYPE_AUTO_SWITCH == _AUTO_SWITCH_DETECT_TYPE_TMDS)||(_DETECT_TYPE_AUTO_SWITCH == _AUTO_SWITCH_DETECT_TYPE_HPD_AND_TMDS))
	static iTE_u8 		pre_rx_hotplug_sta = 0;
	static iTE_u8 		signal_exist_cnt_3[SIGNAL_EXIST_DETECT_TIMES] = {0}, exist_cnt_x=0;
	static iTE_u8	    u8PowerOnDetectNum = 0;
	#endif

	pstProjectGlobalAllVar_t 	pstPrjAllVar = &g_stPrjGloVar;
	pstProjectGlobalRunVar_t 	pstPrjRunVar = &pstPrjAllVar->stPrjRunVar;
	pstProjectGlobalSaveVar_t	pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;

    #if Project_SCU61E_66341_AUTO_SWITCH
	if(!PollAutoSwDetectStatus()) return;//auto switch detect time
    #endif

	u8IteDevSave = g_it663xx_device;

	//get the value of reg 0x13
	for(i=0; i<NUM_IT663XX_USE; i++)
	{
		iT663XX_DevSel(IT663XX_Device[i]);
		for(j=IN_RX0; j<IN_RXn; j++)
		{
			if(HRx_Select(j))
			{
				u8Reg13[i][j] = HRx_R(0x13);
			}
		}
	}

	iT663XX_DevSel(u8IteDevSave);

	for(i=0; i<_HDMI_INPUT_PORT_USED_NUMBER; i++)
	{
		//for(j=0; j<NUM_IT663XX_USE; j++)
		{
			//u8shift = (_HDMI_INPUT_PHYSIC_SHIFT*j);
			u8RxTmp = RxPortMap[i].u8PortIndex;//(hdmi_input_logic_index_table[i]>>u8shift)&_HDMI_INPUT_PHYSIC_MASK;
            j = RxPortMap[i].u8DevNum;
			//if(hdmi_input_node_rx_table[j] != u8RxTmp)
			{
				if(isRxHpdDetNull(i))
				{
					if((u8Reg13[j][u8RxTmp]&0x01) ||(u8Reg13[j][u8RxTmp]&0x40))
					{
						rx_hotplug_sta |= (1<<i);
					}
				}
				else
				{
					if((isGetHpdDetConnect(i)) ||(u8Reg13[j][u8RxTmp]&0x40))
					{
						rx_hotplug_sta |= (1<<i);
					}
				}

				#if (_DETECT_TYPE_AUTO_SWITCH == _AUTO_SWITCH_DETECT_TYPE_TMDS)
				#if (Project_SCU61E_PS176_HDCP_BUG_PRO == _MACRO_ENABLE)
				//if current rx is MHL signal, detect HPD only
				if(((u8RxTmp==IN_RX0)&&((u8Reg13[j][u8RxTmp]&0x40)!=0x00))||((1<<i)==_HDMI_INPUT_TYPE_C_PORT)||((1<<i)==_HDMI_INPUT_DP_PORT))
				#else
				//if current rx is MHL signal, detect HPD only
				if((u8RxTmp==IN_RX0)&&((u8Reg13[j][u8RxTmp]&0x40)!=0x00))
				#endif
				{
					if((rx_hotplug_sta&(1<<i))!=0)
					{
						rx_signal_sta |= (1<<i);
					}
				}
				//HDMI signal
				else if((u8Reg13[j][u8RxTmp]&0x04)&&(u8Reg13[j][u8RxTmp]&0x08)&&\
					(u8Reg13[j][u8RxTmp]&0x10)&&(rx_hotplug_sta&(1<<i)))
				{
					rx_signal_sta |= (1<<i);
				}
				#elif (_DETECT_TYPE_AUTO_SWITCH == _AUTO_SWITCH_DETECT_TYPE_HPD)
				if(rx_hotplug_sta&(1<<i))
				{
					rx_signal_sta |= (1<<i);
				}
                #elif (_DETECT_TYPE_AUTO_SWITCH == _AUTO_SWITCH_DETECT_TYPE_HPD_AND_TMDS)
                if(g_emDetectMode == emHPD)
                {
                    if(rx_hotplug_sta&(1<<i))
    				{
    					rx_signal_sta |= (1<<i);
    				}
                }
                else
                {
                    #if (Project_SCU61E_PS176_HDCP_BUG_PRO == _MACRO_ENABLE)
                    //if current rx is MHL signal, detect HPD only
                    if(((u8RxTmp==IN_RX0)&&((u8Reg13[j][u8RxTmp]&0x40)!=0x00))||((1<<i)==_HDMI_INPUT_TYPE_C_PORT)||((1<<i)==_HDMI_INPUT_DP_PORT))
                    #else
                    //if current rx is MHL signal, detect HPD only
                    if((u8RxTmp==IN_RX0)&&((u8Reg13[j][u8RxTmp]&0x40)!=0x00))
                    #endif
                    {
                        if((rx_hotplug_sta&(1<<i))!=0)
                        {
                            rx_signal_sta |= (1<<i);
                        }
                    }
                    //HDMI signal
                    else if((u8Reg13[j][u8RxTmp]&0x04)&&(u8Reg13[j][u8RxTmp]&0x08)&&\
                        (u8Reg13[j][u8RxTmp]&0x10)&&(rx_hotplug_sta&(1<<i)))
                    {
                        rx_signal_sta |= (1<<i);
                    }
                }
				#endif

				break;
			}
		}
	}
	#if ((_DETECT_TYPE_AUTO_SWITCH == _AUTO_SWITCH_DETECT_TYPE_TMDS)\
        ||(_DETECT_TYPE_AUTO_SWITCH == _AUTO_SWITCH_DETECT_TYPE_HPD_AND_TMDS))
	cur_temp = pre_rx_hotplug_sta^rx_hotplug_sta;
	for(i=0; i<_HDMI_INPUT_PORT_USED_NUMBER; i++)
	{
		if(((cur_temp&(1<<i))>0)&&((rx_hotplug_sta&(1<<i))==0))//from 1 to 0
		{
			for(j=0; j<SIGNAL_EXIST_DETECT_TIMES; j++)
			{
				signal_exist_cnt_3[j] &= (~(1<<i));
			}
			rx_signal_sta &= (~(1<<i));
		}
	}

	pre_rx_hotplug_sta = rx_hotplug_sta;

	signal_exist_cnt_3[exist_cnt_x++] = rx_signal_sta;

	if(exist_cnt_x >= SIGNAL_EXIST_DETECT_TIMES)
	{
		exist_cnt_x = 0;
	}

	for(i=0; i<_HDMI_INPUT_PORT_USED_NUMBER; i++)
	{
		for(j=0; j<SIGNAL_EXIST_DETECT_TIMES; j++)
		{
			if(signal_exist_cnt_3[j] & (1<<i))
			{
				rx_signal_sta |= 1<<i;
			}
		}
	}

	if(u8PowerOnDetectNum < POWER_ON_DETECT_TIMES)
	{
		u8PowerOnDetectNum ++;
		rx_signal_sta = 0;
	}
	#endif

	pstPrjRunVar->u16RxSignalSta = rx_signal_sta;
	//iTE_MsgA(("\r\n///////////////////////////////\r\n"));
	//iTE_MsgA(("Rx port signal status is %d\r\n", rx_signal_sta));
	//iTE_MsgA(("///////////////////////////////\r\n"));

	u8RxTmp = pstPrjSaveVar->stHdmiSel.u8SwChannel;//the current select

	#if (_EN_CEC_POWER_ON_TV_DETECTED_SIGNAL==_MACRO_ENABLE)
	if(((pre_rx_signal_sta^rx_signal_sta)&rx_signal_sta)>0)
	{
		//if manual switch mode, and it isn't the active rx
		if(!pstPrjSaveVar->stHdmiSel.bAutoSwitch)
		{
			if(((pre_rx_signal_sta^rx_signal_sta)&rx_signal_sta)&(1<<u8RxTmp))
			{
				g_bCecOnTVDetectedNewSignal = TRUE;
			}
		}
		else
		{
			g_bCecOnTVDetectedNewSignal = TRUE;
		}
	}
	#endif

	if(!pstPrjSaveVar->stHdmiSel.bAutoSwitch) // isn't the auto switch mode
	{
		pre_isAutoSwitchMode = pstPrjSaveVar->stHdmiSel.bAutoSwitch;
		pre_rx_signal_sta = rx_signal_sta;
		return;
	}

	if((!pre_isAutoSwitchMode)&&(pstPrjSaveVar->stHdmiSel.bAutoSwitch))
	{
		pre_isAutoSwitchMode = pstPrjSaveVar->stHdmiSel.bAutoSwitch;
		pre_rx_signal_sta = 0;
	}

	if(pre_rx_signal_sta != rx_signal_sta)
	{
		if((g_bAutoSwPowerOnStatus)&&(rx_signal_sta&(1<<u8RxTmp)))
		{
			g_bAutoSwPowerOnStatus = FALSE;
		}
		else
		{
			g_bAutoSwPowerOnStatus = FALSE;
			cur_temp = pre_rx_signal_sta ^ rx_signal_sta;
			iTE_MsgA(("rx signal change, rx_signal_sta = 0x%2x, cur_temp = 0x%2x\r\n", rx_signal_sta, cur_temp));
			for(i=0; i<_HDMI_INPUT_PORT_USED_NUMBER; i++)
			{
				if(cur_temp & (1 << i))
				{
					if(rx_signal_sta & (1<<i))
					{
						iTE_MsgA(("plug in: auto switch to port %d\r\n", i));

						//Switch hdmi channel
						IT663XX_SwitchChannelByIndex(i);

						//save switch channel
						pstPrjSaveVar->stHdmiSel.u8SwChannel = i;
						SetUserData_HdmiSel();

						//hook function
						if(FuncAutoSwHook != NULL) {FuncAutoSwHook(i);}

						pre_rx_signal_sta = rx_signal_sta;
						return;
					}
					else
					{
						if(i == u8RxTmp)
						{
							iTE_MsgA(("signal missed, switch to next:"));
							for(j=0; j<_HDMI_INPUT_PORT_USED_NUMBER; j++)
							{
								if(rx_signal_sta & (1<<j))
								{
									iTE_MsgA(("plug out: auto switch to port %d\r\n", j));

									//Switch hdmi channel
									IT663XX_SwitchChannelByIndex(j);

									//save switch channel
									pstPrjSaveVar->stHdmiSel.u8SwChannel = j;
						            SetUserData_HdmiSel();

									//hook function
									if(FuncAutoSwHook != NULL) {FuncAutoSwHook(i);}

									pre_rx_signal_sta = rx_signal_sta;
									return;
								}
							}
						}
					}
				}
			}
		}
		pre_rx_signal_sta = rx_signal_sta;
	}
}

#else
void IT663XX_SourceAutoSwitch(void)
{
	iTE_u8 				i, j, rx_signal_sta = 0, cur_temp = 0, u8IteDevSave;
	iTE_u8 				u8Reg13[NUM_IT663XX_USE][IN_RXn], u8shift, u8RxTmp;
	iTE_u8 				rx_hotplug_sta = 0;
	static iTE_u8 		pre_rx_signal_sta = 0;
	static bool 		pre_isAutoSwitchMode = TRUE;

	#if ((_DETECT_TYPE_AUTO_SWITCH == _AUTO_SWITCH_DETECT_TYPE_TMDS)\
        ||(_DETECT_TYPE_AUTO_SWITCH == _AUTO_SWITCH_DETECT_TYPE_HPD_AND_TMDS))
	static iTE_u8 		pre_rx_hotplug_sta = 0;
	static iTE_u8 		signal_exist_cnt_3[SIGNAL_EXIST_DETECT_TIMES] = {0}, exist_cnt_x=0;
	static iTE_u8	    u8PowerOnDetectNum = 0;
	#endif

	pstProjectGlobalAllVar_t 	pstPrjAllVar = &g_stPrjGloVar;
	pstProjectGlobalRunVar_t 	pstPrjRunVar = &pstPrjAllVar->stPrjRunVar;
	pstProjectGlobalSaveVar_t	pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;

    #if Project_SCU61E_66341_AUTO_SWITCH
	if(!PollAutoSwDetectStatus()) return;//auto switch detect time
    #endif

	u8IteDevSave = g_it663xx_device;

	//get the value of reg 0x13
	for(i=0; i<NUM_IT663XX_USE; i++)
	{
		iT663XX_DevSel(IT663XX_Device[i]);
		for(j=IN_RX0; j<IN_RXn; j++)
		{
			if(HRx_Select(j))
			{
				u8Reg13[i][j] = HRx_R(0x13);
			}
		}
	}

	iT663XX_DevSel(u8IteDevSave);

	for(i=0; i<_HDMI_INPUT_PORT_USED_NUMBER; i++)
	{
		for(j=0; j<NUM_IT663XX_USE; j++)
		{
			u8shift = (_HDMI_INPUT_PHYSIC_SHIFT*j);
			u8RxTmp = (hdmi_input_logic_index_table[i]>>u8shift)&_HDMI_INPUT_PHYSIC_MASK;
			if(hdmi_input_node_rx_table[j] != u8RxTmp)
			{
				if(isRxHpdDetNull(i))
				{
					if((u8Reg13[j][u8RxTmp]&0x01) ||(u8Reg13[j][u8RxTmp]&0x40))
					{
						rx_hotplug_sta |= (1<<i);
					}
				}
				else
				{
					if((isGetHpdDetConnect(i)) ||(u8Reg13[j][u8RxTmp]&0x40))
					{
						rx_hotplug_sta |= (1<<i);
					}
				}

				#if (_DETECT_TYPE_AUTO_SWITCH == _AUTO_SWITCH_DETECT_TYPE_TMDS)
				#if (Project_SCU61E_PS176_HDCP_BUG_PRO == _MACRO_ENABLE)
				//if current rx is MHL signal, detect HPD only
				if(((u8RxTmp==IN_RX0)&&((u8Reg13[j][u8RxTmp]&0x40)!=0x00))||((1<<i)==_HDMI_INPUT_TYPE_C_PORT)||((1<<i)==_HDMI_INPUT_DP_PORT))
				#else
				//if current rx is MHL signal, detect HPD only
				if((u8RxTmp==IN_RX0)&&((u8Reg13[j][u8RxTmp]&0x40)!=0x00))
				#endif
				{
					if((rx_hotplug_sta&(1<<i))!=0)
					{
						rx_signal_sta |= (1<<i);
					}
				}
				//HDMI signal
				else if((u8Reg13[j][u8RxTmp]&0x04)&&(u8Reg13[j][u8RxTmp]&0x08)&&\
					(u8Reg13[j][u8RxTmp]&0x10)&&(rx_hotplug_sta&(1<<i)))
				{
					rx_signal_sta |= (1<<i);
				}
				#elif (_DETECT_TYPE_AUTO_SWITCH == _AUTO_SWITCH_DETECT_TYPE_HPD)
				if(rx_hotplug_sta&(1<<i))
				{
					rx_signal_sta |= (1<<i);
				}
                #elif (_DETECT_TYPE_AUTO_SWITCH == _AUTO_SWITCH_DETECT_TYPE_HPD_AND_TMDS)
                if(g_emDetectMode == emHPD)
                {
                    if(rx_hotplug_sta&(1<<i))
    				{
    					rx_signal_sta |= (1<<i);
    				}
                }
                else
                {
                    #if (Project_SCU61E_PS176_HDCP_BUG_PRO == _MACRO_ENABLE)
                    //if current rx is MHL signal, detect HPD only
                    if(((u8RxTmp==IN_RX0)&&((u8Reg13[j][u8RxTmp]&0x40)!=0x00))||((1<<i)==_HDMI_INPUT_TYPE_C_PORT)||((1<<i)==_HDMI_INPUT_DP_PORT))
                    #else
                    //if current rx is MHL signal, detect HPD only
                    if((u8RxTmp==IN_RX0)&&((u8Reg13[j][u8RxTmp]&0x40)!=0x00))
                    #endif
                    {
                        if((rx_hotplug_sta&(1<<i))!=0)
                        {
                            rx_signal_sta |= (1<<i);
                        }
                    }
                    //HDMI signal
                    else if((u8Reg13[j][u8RxTmp]&0x04)&&(u8Reg13[j][u8RxTmp]&0x08)&&\
                        (u8Reg13[j][u8RxTmp]&0x10)&&(rx_hotplug_sta&(1<<i)))
                    {
                        rx_signal_sta |= (1<<i);
                    }
                }
				#endif

				break;
			}
		}
	}
	#if ((_DETECT_TYPE_AUTO_SWITCH == _AUTO_SWITCH_DETECT_TYPE_TMDS)\
        ||(_DETECT_TYPE_AUTO_SWITCH == _AUTO_SWITCH_DETECT_TYPE_HPD_AND_TMDS))
	cur_temp = pre_rx_hotplug_sta^rx_hotplug_sta;
	for(i=0; i<_HDMI_INPUT_PORT_USED_NUMBER; i++)
	{
		if(((cur_temp&(1<<i))>0)&&((rx_hotplug_sta&(1<<i))==0))//from 1 to 0
		{
			for(j=0; j<SIGNAL_EXIST_DETECT_TIMES; j++)
			{
				signal_exist_cnt_3[j] &= (~(1<<i));
			}
			rx_signal_sta &= (~(1<<i));
		}
	}

	pre_rx_hotplug_sta = rx_hotplug_sta;

	signal_exist_cnt_3[exist_cnt_x++] = rx_signal_sta;

	if(exist_cnt_x >= SIGNAL_EXIST_DETECT_TIMES)
	{
		exist_cnt_x = 0;
	}

	for(i=0; i<_HDMI_INPUT_PORT_USED_NUMBER; i++)
	{
		for(j=0; j<SIGNAL_EXIST_DETECT_TIMES; j++)
		{
			if(signal_exist_cnt_3[j] & (1<<i))
			{
				rx_signal_sta |= 1<<i;
			}
		}
	}

	if(u8PowerOnDetectNum < POWER_ON_DETECT_TIMES)
	{
		u8PowerOnDetectNum ++;
		rx_signal_sta = 0;
	}
	#endif

	pstPrjRunVar->u16RxSignalSta = rx_signal_sta;
	//iTE_MsgA(("Rx port signal status is %d\r\n", rx_signal_sta));

	u8RxTmp = pstPrjSaveVar->stHdmiSel.u8SwChannel;//the current select

	#if (_EN_CEC_POWER_ON_TV_DETECTED_SIGNAL==_MACRO_ENABLE)
	if(((pre_rx_signal_sta^rx_signal_sta)&rx_signal_sta)>0)
	{
		//if manual switch mode, and it isn't the active rx
		if(!pstPrjSaveVar->stHdmiSel.bAutoSwitch)
		{
			if(((pre_rx_signal_sta^rx_signal_sta)&rx_signal_sta)&(1<<u8RxTmp))
			{
				g_bCecOnTVDetectedNewSignal = TRUE;
			}
		}
		else
		{
			g_bCecOnTVDetectedNewSignal = TRUE;
		}
	}
	#endif

	if(!pstPrjSaveVar->stHdmiSel.bAutoSwitch) // isn't the auto switch mode
	{
		pre_isAutoSwitchMode = pstPrjSaveVar->stHdmiSel.bAutoSwitch;
		pre_rx_signal_sta = rx_signal_sta;
		return;
	}

	if((!pre_isAutoSwitchMode)&&(pstPrjSaveVar->stHdmiSel.bAutoSwitch))
	{
		pre_isAutoSwitchMode = pstPrjSaveVar->stHdmiSel.bAutoSwitch;
		pre_rx_signal_sta = 0;
	}

	if(pre_rx_signal_sta != rx_signal_sta)
	{
		if((g_bAutoSwPowerOnStatus)&&(rx_signal_sta&(1<<u8RxTmp)))
		{
			g_bAutoSwPowerOnStatus = FALSE;
		}
		else
		{
			g_bAutoSwPowerOnStatus = FALSE;
			cur_temp = pre_rx_signal_sta ^ rx_signal_sta;
			iTE_MsgA(("rx signal change, rx_signal_sta = 0x%2x, cur_temp = 0x%2x\r\n", rx_signal_sta, cur_temp));
			for(i=0; i<_HDMI_INPUT_PORT_USED_NUMBER; i++)
			{
				if(cur_temp & (1 << i))
				{
					if(rx_signal_sta & (1<<i))
					{
						iTE_MsgA(("plug in: auto switch to port %d\r\n", i));

						//Switch hdmi channel
						IT663XX_SwitchChannelByIndex(i);

						//save switch channel
                        pstPrjSaveVar->stHdmiSel.u8SwChannel = i;
						SetUserData_HdmiSel();
						//hook function
						if(FuncAutoSwHook != NULL) {FuncAutoSwHook(i);}

						pre_rx_signal_sta = rx_signal_sta;
						return;
					}
					else
					{
						if(i == u8RxTmp)
						{
							iTE_MsgA(("signal missed, switch to next:"));
							for(j=0; j<_HDMI_INPUT_PORT_USED_NUMBER; j++)
							{
								if(rx_signal_sta & (1<<j))
								{
									iTE_MsgA(("plug out: auto switch to port %d\r\n", j));

									//Switch hdmi channel
									IT663XX_SwitchChannelByIndex(j);

									//save switch channel
									pstPrjSaveVar->stHdmiSel.u8SwChannel = j;
            						SetUserData_HdmiSel();

									//hook function
									#if(_ENABLE_SWITCH_TO_USBC_EDID == _MACRO_ENABLE)
									if(FuncAutoSwHook != NULL) {FuncAutoSwHook(j);}
									#else
									if(FuncAutoSwHook != NULL) {FuncAutoSwHook(i);}
									#endif

									pre_rx_signal_sta = rx_signal_sta;
									return;
								}
							}

                            #if _ENABLE_NO_SIGNAL_TO_SW_INPUT1
                            if(j>=_HDMI_INPUT_PORT_USED_NUMBER)
                            {
                                //Switch hdmi channel
                                IT663XX_SwitchChannelByIndex(0);
                                //save switch channel
                                pstPrjSaveVar->stHdmiSel.u8SwChannel = 0;
        						SetUserData_HdmiSel();

                                //hook function
                                if(FuncAutoSwHook != NULL) {FuncAutoSwHook(i);}
                            }
                            #endif
						}
					}
				}
			}
		}
		pre_rx_signal_sta = rx_signal_sta;
	}
}
#endif

#if(_ENABLE_POWER_OFF_HPD_TMDS_CHECK == _MACRO_ENABLE)
void PowerOffHpdOrTmdsChack()
{
	iTE_u8 				i, j, rx_signal_sta = 0, cur_temp = 0, u8IteDevSave;
	iTE_u8 				u8Reg13[NUM_IT663XX_USE][IN_RXn], u8shift, u8RxTmp;
	iTE_u8 				rx_hotplug_sta = 0;
	static iTE_u8 		pre_rx_signal_sta = 0;
	static bool 		pre_isAutoSwitchMode = TRUE;

	#if ((_DETECT_TYPE_AUTO_SWITCH == _AUTO_SWITCH_DETECT_TYPE_TMDS)\
        ||(_DETECT_TYPE_AUTO_SWITCH == _AUTO_SWITCH_DETECT_TYPE_HPD_AND_TMDS))
	static iTE_u8 		pre_rx_hotplug_sta = 0;
	static iTE_u8 		signal_exist_cnt_3[SIGNAL_EXIST_DETECT_TIMES] = {0}, exist_cnt_x=0;
	static iTE_u8	    u8PowerOnDetectNum = 0;
	#endif

	pstProjectGlobalAllVar_t 	pstPrjAllVar = &g_stPrjGloVar;
	pstProjectGlobalRunVar_t 	pstPrjRunVar = &pstPrjAllVar->stPrjRunVar;
	pstProjectGlobalSaveVar_t	pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;

    #if Project_SCU61E_66341_AUTO_SWITCH
	if(!PollAutoSwDetectStatus()) return;//auto switch detect time
    #endif

	u8IteDevSave = g_it663xx_device;

	//get the value of reg 0x13
	for(i=0; i<NUM_IT663XX_USE; i++)
	{
		iT663XX_DevSel(IT663XX_Device[i]);
		for(j=IN_RX0; j<IN_RXn; j++)
		{
			if(HRx_Select(j))
			{
				u8Reg13[i][j] = HRx_R(0x13);
			}
		}
	}

	iT663XX_DevSel(u8IteDevSave);

	for(i=0; i<_HDMI_INPUT_PORT_USED_NUMBER; i++)
	{
		for(j=0; j<NUM_IT663XX_USE; j++)
		{
			u8shift = (_HDMI_INPUT_PHYSIC_SHIFT*j);
			u8RxTmp = (hdmi_input_logic_index_table[i]>>u8shift)&_HDMI_INPUT_PHYSIC_MASK;
			if(hdmi_input_node_rx_table[j] != u8RxTmp)
			{
				if(isRxHpdDetNull(i))
				{
					//if((u8Reg13[j][u8RxTmp]&0x01) ||(u8Reg13[j][u8RxTmp]&0x40))
					{
						rx_hotplug_sta |= (1<<i);
					}
				}
				else
				{
					if((isGetHpdDetConnect(i)) /*||(u8Reg13[j][u8RxTmp]&0x40)*/)
					{
						rx_hotplug_sta |= (1<<i);
					}
				}

				#if (_DETECT_TYPE_AUTO_SWITCH == _AUTO_SWITCH_DETECT_TYPE_TMDS)
				#if (Project_SCU61E_PS176_HDCP_BUG_PRO == _MACRO_ENABLE)
				//if current rx is MHL signal, detect HPD only
				if(((u8RxTmp==IN_RX0)&&((u8Reg13[j][u8RxTmp]&0x40)!=0x00))||((1<<i)==_HDMI_INPUT_TYPE_C_PORT)||((1<<i)==_HDMI_INPUT_DP_PORT))
				#else
				//if current rx is MHL signal, detect HPD only
				if((u8RxTmp==IN_RX0)&&((u8Reg13[j][u8RxTmp]&0x40)!=0x00))
				#endif
				{
					if((rx_hotplug_sta&(1<<i))!=0)
					{
						rx_signal_sta |= (1<<i);
					}
				}
				//HDMI signal
				else if((u8Reg13[j][u8RxTmp]&0x04)&&(u8Reg13[j][u8RxTmp]&0x08)&&\
					(u8Reg13[j][u8RxTmp]&0x10)&&(rx_hotplug_sta&(1<<i)))
				{
					rx_signal_sta |= (1<<i);
				}
				#elif (_DETECT_TYPE_AUTO_SWITCH == _AUTO_SWITCH_DETECT_TYPE_HPD)
				if(rx_hotplug_sta&(1<<i))
				{
					rx_signal_sta |= (1<<i);
				}
                #elif (_DETECT_TYPE_AUTO_SWITCH == _AUTO_SWITCH_DETECT_TYPE_HPD_AND_TMDS)
                if(g_emDetectMode == emHPD)
                {
                    if(rx_hotplug_sta&(1<<i))
    				{
    					rx_signal_sta |= (1<<i);
    				}
                }
                else
                {
                    #if (Project_SCU61E_PS176_HDCP_BUG_PRO == _MACRO_ENABLE)
                    //if current rx is MHL signal, detect HPD only
                    if(((u8RxTmp==IN_RX0)&&((u8Reg13[j][u8RxTmp]&0x40)!=0x00))||((1<<i)==_HDMI_INPUT_TYPE_C_PORT)||((1<<i)==_HDMI_INPUT_DP_PORT))
                    #else
                    //if current rx is MHL signal, detect HPD only
                    if((u8RxTmp==IN_RX0)&&((u8Reg13[j][u8RxTmp]&0x40)!=0x00))
                    #endif
                    {
                        if((rx_hotplug_sta&(1<<i))!=0)
                        {
                            rx_signal_sta |= (1<<i);
                        }
                    }
                    //HDMI signal
                    else if((u8Reg13[j][u8RxTmp]&0x04)&&(u8Reg13[j][u8RxTmp]&0x08)&&\
                        (u8Reg13[j][u8RxTmp]&0x10)&&(rx_hotplug_sta&(1<<i)))
                    {
                        rx_signal_sta |= (1<<i);
                    }
                }
				#endif

				break;
			}
		}
	}
	#if ((_DETECT_TYPE_AUTO_SWITCH == _AUTO_SWITCH_DETECT_TYPE_TMDS)\
        ||(_DETECT_TYPE_AUTO_SWITCH == _AUTO_SWITCH_DETECT_TYPE_HPD_AND_TMDS))
	cur_temp = pre_rx_hotplug_sta^rx_hotplug_sta;
	for(i=0; i<_HDMI_INPUT_PORT_USED_NUMBER; i++)
	{
		if(((cur_temp&(1<<i))>0)&&((rx_hotplug_sta&(1<<i))==0))//from 1 to 0
		{
			for(j=0; j<SIGNAL_EXIST_DETECT_TIMES; j++)
			{
				signal_exist_cnt_3[j] &= (~(1<<i));
			}
			rx_signal_sta &= (~(1<<i));
		}
	}

	pre_rx_hotplug_sta = rx_hotplug_sta;

	signal_exist_cnt_3[exist_cnt_x++] = rx_signal_sta;

	if(exist_cnt_x >= SIGNAL_EXIST_DETECT_TIMES)
	{
		exist_cnt_x = 0;
	}

	for(i=0; i<_HDMI_INPUT_PORT_USED_NUMBER; i++)
	{
		for(j=0; j<SIGNAL_EXIST_DETECT_TIMES; j++)
		{
			if(signal_exist_cnt_3[j] & (1<<i))
			{
				rx_signal_sta |= 1<<i;
			}
		}
	}

	if(u8PowerOnDetectNum < POWER_ON_DETECT_TIMES)
	{
		u8PowerOnDetectNum ++;
		rx_signal_sta = 0;
	}
	#endif
    rx_signal_sta = rx_hotplug_sta;
	pstPrjRunVar->u16RxSignalSta = rx_signal_sta;
	//iTE_MsgA(("Rx port signal status is %d\r\n", rx_signal_sta));

	//u8RxTmp = pstPrjSaveVar->stHdmiSel.u8SwChannel;//the current select

	#if (_EN_CEC_POWER_ON_TV_DETECTED_SIGNAL==_MACRO_ENABLE)
	if(((pre_rx_signal_sta^rx_signal_sta)&rx_signal_sta)>0)
	{
		//if manual switch mode, and it isn't the active rx
		if(!pstPrjSaveVar->stHdmiSel.bAutoSwitch)
		{
			if(((pre_rx_signal_sta^rx_signal_sta)&rx_signal_sta)&(1<<u8RxTmp))
			{
				g_bCecOnTVDetectedNewSignal = TRUE;
			}
		}
		else
		{
			g_bCecOnTVDetectedNewSignal = TRUE;
		}
	}
	#endif

	if(!pstPrjSaveVar->stHdmiSel.bAutoSwitch) // isn't the auto switch mode
	{
		pre_isAutoSwitchMode = pstPrjSaveVar->stHdmiSel.bAutoSwitch;
		pre_rx_signal_sta = rx_signal_sta;
		return;
	}

	if((!pre_isAutoSwitchMode)&&(pstPrjSaveVar->stHdmiSel.bAutoSwitch))
	{
		pre_isAutoSwitchMode = pstPrjSaveVar->stHdmiSel.bAutoSwitch;
		pre_rx_signal_sta = 0;
	}

	if(pre_rx_signal_sta != rx_signal_sta)
	{
		if((g_bAutoSwPowerOnStatus)&&(rx_signal_sta&(1<<u8RxTmp)))
		{
			g_bAutoSwPowerOnStatus = FALSE;
		}
		else
		{
			//g_bAutoSwPowerOnStatus = FALSE;
			cur_temp = pre_rx_signal_sta ^ rx_signal_sta;
			iTE_MsgA(("rx signal change, rx_signal_sta = 0x%2x, cur_temp = 0x%2x\r\n", rx_signal_sta, cur_temp));
			for(i=0; i<_HDMI_INPUT_PORT_USED_NUMBER; i++)
			{
				if(cur_temp & (1 << i))
				{
					if(rx_signal_sta & (1<<i))
					{
						iTE_MsgA(("plug in: auto switch to port %d\r\n", i));

						//hook function
						//if(FuncAutoSwHook != NULL) {FuncAutoSwHook(i);}
//                        if(FuncPowerOffHpdTmdsCheckHook != NULL)
//							FuncPowerOffHpdTmdsCheckHook(i);

						pre_rx_signal_sta = rx_signal_sta;
						return;
					}

				}
			}
		}
		pre_rx_signal_sta = rx_signal_sta;
	}
}
#endif

void IT6634_Main(void)
{
	iTE_u8 l_u8Dev;

	if(gCurSysStatus == 0)
	{
		MCU_Init();
		gCurSysStatus = 1;
	}
	else
	{
		if(!HoldSystem())
		{
			switch(gCurSysStatus)
			{
				case 0x01:
                    It663xxSysInit_xSemaphoreTask();
					for(l_u8Dev=0; l_u8Dev<NUM_IT663XX_USE; l_u8Dev++)
					{
						iT663XX_DevSel(IT663XX_Device[l_u8Dev]);
						IT6634_SysInit();
					}
                    It663xxSysInit_xSemaphoreGive();
					if(gCurSysStatus < 2)
					{
						gCurSysStatus = 2;
					}
					break;
				case 0x02:
					if(1 || u8IntEvent)
					{
                        It663xxIrq_xSemaphoreTask();
						u8IntEvent = 0;
						if(g_bIt663xxAutoSwRunFlag) IT663XX_SourceAutoSwitch();
						for(l_u8Dev=0; l_u8Dev<NUM_IT663XX_USE; l_u8Dev++)
						{
							iT663XX_DevSel(IT663XX_Device[l_u8Dev]);
							IT6634_SysIrq();
						}
                        It663xxIrq_xSemaphoreGive();
					}
					break;
				case 0x03:
					gCurSysStatus = 1;
					break;
				case 0x04://disable tx out
					if(pre_it66341_tx_out_status == tx_out_enable_stu)
					{
						pre_it66341_tx_out_status = tx_out_disable_stu;
						pITE663xx_STM32_api->TxOutDisable();
					}
					break;
				default:
					gCurSysStatus = 0;
					break;
			}
		}
	}
}

#if ENABLE_IT66341_DVI_EDID_TO_CHANGE_HDCP14
extern void mapp_SwitchDviEdidModeToChangeHdcpVer(u8 u8RxPort);
#endif

void IT663XX_EdidWrite(iTE_u8 u8RxPort, iTE_u8 *u8Edid)
{
	iTE_u8	u8Cnt;
	iTE_u8	u8Rx20[4], u8Rx34[4];

	HRx_Select(u8RxPort);
	u8Rx34[u8RxPort] = HRx_R(0x34);
	HRx_W(0x34, u8Rx34[u8RxPort] & 0xEF);
	HRx_W(0x4B, (g_pu8CurEdidAdrComm[0]/*g_u8CurEdidAdr*/+u8I2CAdr) | 0x01);

	u8Rx34[u8RxPort] = HRx_R(0x34);
	HRx_W(0x34, u8Rx34[u8RxPort] & 0xEF);
	HRx_Set(0xC5, 0x11, 0x00); //  Reg_P0_DisableShadow=0
	HRx_Set(0x34, 0x01, 0x00);	// Disable auto control  RCLK, if RxPwr5V = 0 then Rx auto power down.
	HRx_W(0x4B, (g_pu8CurEdidAdrComm[0]/*g_u8CurEdidAdr*/+u8I2CAdr) | 0x01);
	u8Rx20[u8RxPort] = HRx_R(0x20);
	HRx_W(0x20, u8Rx20[u8RxPort] & 0xFB);

	for(u8Cnt = 0; u8Cnt < 2; u8Cnt++)
	{
		Edid_Wb(u8Cnt*0x80, 0x80, &u8Edid[u8Cnt*0x80]);
	}

    #if ENABLE_IT66341_DVI_EDID_TO_CHANGE_HDCP14
    Edid_CheckDviMode(u8Edid);
    #endif

	HRx_W(0x4B, (g_pu8CurEdidAdrComm[0]/*g_u8CurEdidAdr*/+u8I2CAdr));
	HRx_Set(0x34, 0x01, 0x01);	// Enable auto control RCLK
	HRx_W(0x20, u8Rx20[u8RxPort]);
	HRx_W(0x34, u8Rx34[u8RxPort]);

	HRx_W(0x4B, (g_pu8CurEdidAdrComm[0]/*g_u8CurEdidAdr*/+u8I2CAdr));

	HRx_W(0x34, u8Rx34[u8RxPort]);

    #if ENABLE_IT66341_DVI_EDID_TO_CHANGE_HDCP14
    mapp_SwitchDviEdidModeToChangeHdcpVer(u8RxPort);
    #endif

	HRx_HpdSet(u8RxPort, HPD_L2H);
}

#if (Project_SCU61E_PS176_HDCP_BUG_PRO == _MACRO_ENABLE)
#if _ENABLE_IT663XX_MULTI_CHIP_IN_USE
iTE_u8 PS176PortInputState(iTE_u8 u8ChipInRx)
{
    //pstProjectGlobalAllVar_t  pstPrjAllVar = &g_stPrjGloVar;
    //pstProjectGlobalSaveVar_t pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    //pstHdmiSwitchParam_t      pstHdmiSw = &pstPrjSaveVar->stHdmiSel;
    iTE_u8                      u8Shift, u8Shift2, u8Tmp, u8DevIndex, i, j, k=0, u8Ps176In[_HDMI_I_PORT_NUM];
    iTE_u8                      u8PortState = 0;// '0 for u8ChipInRx isn't ps176', '1 for u8ChipInRx is ps176, but current active isn't ps176', '2 for u8ChipInRx is ps176, and current active is ps176'
    bool                        bValidPort = TRUE;

    u8DevIndex = IT663XX_GetDeviceIndexByID(g_it663xx_device);

    for(i=0; i<_HDMI_I_PORT_NUM; i++)
    {
        if(((1<<i)==_HDMI_INPUT_TYPE_C_PORT)||((1<<i)==_HDMI_INPUT_DP_PORT))
        {
            u8Ps176In[k] = i;
            k++;
        }
    }
    if(k>0)
    {
        for(i=0; i<k; i++)
		{
			bValidPort = TRUE;
			u8Tmp = RxPortMap[u8Ps176In[i]];
			if(u8Tmp == u8ChipInRx)
			{
				//if(hdmi_input_node_rx_table[u8DevIndex] != u8ChipInRx)
				{
					if(u8DevIndex > 0)
					{
						for(j=0; j<u8DevIndex; j++)
						{
							if((RxPortMap[u8Ps176In[i]]) != j)
							{
								bValidPort = FALSE;
								break;
							}
						}

						if(!bValidPort) continue;
					}

					u8PortState = 1;
					if(RxPortMap[u8Ps176In[i]]==g_pu8RxSelComm[0])
					{
						u8PortState = 2;
						break;
					}
				}
			}
		}
    }

    return u8PortState;
}

#else
iTE_u8 PS176PortInputState(iTE_u8 u8ChipInRx)
{
	//pstProjectGlobalAllVar_t 	pstPrjAllVar = &g_stPrjGloVar;
	//pstProjectGlobalSaveVar_t	pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
	//pstHdmiSwitchParam_t		pstHdmiSw = &pstPrjSaveVar->stHdmiSel;
	iTE_u8					    u8Shift, u8Shift2, u8Tmp, u8DevIndex, i, j, k=0, u8Ps176In[_HDMI_I_PORT_NUM];
	iTE_u8 					    u8PortState = 0;// '0 for u8ChipInRx isn't ps176', '1 for u8ChipInRx is ps176, but current active isn't ps176', '2 for u8ChipInRx is ps176, and current active is ps176'
	bool						bValidPort = TRUE;

	u8DevIndex = IT663XX_GetDeviceIndexByID(g_it663xx_device);

	for(i=0; i<_HDMI_I_PORT_NUM; i++)
	{
		if(((1<<i)==_HDMI_INPUT_TYPE_C_PORT)||((1<<i)==_HDMI_INPUT_DP_PORT))
		{
			u8Ps176In[k] = i;
			k++;
		}
	}

	if(k != 0)
	{
		for(i=0; i<k; i++)
		{
			bValidPort = TRUE;
			u8Tmp = hdmi_input_logic_index_table[u8Ps176In[i]];
			u8Shift = (_HDMI_INPUT_PHYSIC_SHIFT*u8DevIndex);
			if(((u8Tmp>>u8Shift)&_HDMI_INPUT_PHYSIC_MASK)== u8ChipInRx)
			{
				if(hdmi_input_node_rx_table[u8DevIndex] != u8ChipInRx)
				{
					if(u8DevIndex > 0)
					{
						for(j=0; j<u8DevIndex; j++)
						{
							u8Shift2 = (_HDMI_INPUT_PHYSIC_SHIFT*j);
							if(((u8Tmp>>u8Shift2)&_HDMI_INPUT_PHYSIC_MASK) != hdmi_input_node_rx_table[j])
							{
								bValidPort = FALSE;
								break;
							}
						}

						if(!bValidPort) continue;
					}

					u8PortState = 1;
					//if(u8Ps176In[i]==pstHdmiSw->u8SwChannel)
					if(u8Ps176In[i]==g_pu8RxSelComm[0])
					{
						u8PortState = 2;
						break;
					}
				}
			}
		}
	}

	return u8PortState;
}
#endif
#endif

#if (Project_663xx_MANY_CHIPS_SIGNAL_REBOOT==_MACRO_ENABLE)
void It663xxRepairSnowInSwitch(iTE_u8 u8TxPort,iTE_u8 u8DevIndex, iTE_u8 u8RxPort)
{
	if(hdmi_input_node_rx_table[u8DevIndex] == u8RxPort)
	{
		#if (IS_IT663XX == IT66341)
		if(HTx_Select(u8TxPort))
		{
			HTx_DisOut(u8TxPort);
			g_pstCurTxInfo->u16TxHdcpSta = 0x00;
		}
		#endif
	}
	else
	{
		#if (IS_IT663XX == IT66341)
		if(HTx_Select(u8TxPort))
		{
			HTx_DisOut(u8TxPort);
			g_pstCurTxInfo->u16TxHdcpSta = 0x00;

			if(HRx_Select(u8RxPort))
			{
				HRx_HpdSet(u8RxPort, HPD_L2H);
			}
		}
		#endif
	}
}
#endif

#if _ENABLE_IT663XX_MULTI_CHIP_IN_USE
bool isIt663xxDeviceRxTypecPort(iTE_u8 u8Dev, iTE_u8 u8Rx)
{
    iTE_u8 u8ii, u8jj, u8RxTmp;

    u8jj = IT663XX_GetDeviceIndexByID(u8Dev);

    for(u8ii=0; u8ii<_HDMI_INPUT_PORT_USED_NUMBER; u8ii++)
    {
        if(_HDMI_INPUT_TYPE_C_PORT != 0xFF)
        {
            if(_HDMI_INPUT_TYPE_C_PORT & (1<<u8ii))
            {
                u8RxTmp = RxPortMap[u8ii].u8PortIndex;
                if((u8RxTmp==u8Rx)&&(RxPortMap[u8ii].u8DevNum == u8jj))
                {
                    return TRUE;
                }
            }
        }
    }

    return FALSE;
}

#else
bool isIt663xxDeviceRxTypecPort(iTE_u8 u8Dev, iTE_u8 u8Rx)
{
	iTE_u8 u8ii, u8jj, u8shift, u8RxTmp;

	u8jj = IT663XX_GetDeviceIndexByID(u8Dev);
	u8shift = (_HDMI_INPUT_PHYSIC_SHIFT*u8jj);

	for(u8ii=0; u8ii<_HDMI_INPUT_PORT_USED_NUMBER; u8ii++)
	{
        if(_HDMI_INPUT_TYPE_C_PORT != 0xFF)
        {
    		if(_HDMI_INPUT_TYPE_C_PORT & (1<<u8ii))
    		{
    			u8RxTmp = (hdmi_input_logic_index_table[u8ii]>>u8shift)&_HDMI_INPUT_PHYSIC_MASK;
    			if((u8RxTmp==u8Rx)&&(u8RxTmp!=hdmi_input_node_rx_table[u8jj]))
    			{
    				return TRUE;
    			}
    		}
        }
	}

	return FALSE;
}
#endif
void PortEdidMax4K30(iTE_u8 *pBuffer, iTE_u8 u8Dev, iTE_u8 u8RxPort)
{
	bool						bOver300M = FALSE;

	#if (_EN_USE_VALENS_CHIP_LIMITED_MAX_300M == _MACRO_ENABLE)
	#else
	if(isIt663xxDeviceRxTypecPort(u8Dev, u8RxPort))
	#endif
	{
		//if Descriptor1 > 300M
		if(pBuffer[55]>0x75)
		{
			bOver300M = TRUE;
		}
		else if(pBuffer[55]==0x75)
		{
			if(pBuffer[54]>0x30)
			{
				bOver300M = TRUE;
			}
		}

		if(bOver300M)
		{
			memcpy(&pBuffer[54], ExtendEdidTable, 18);
			pBuffer[0x7F] = CalEdidCheckSum(pBuffer, 0);
		}
	}
}

iTE_u8 IT663XX_GetPhyAdrByLogic(iTE_u8 logic)
{
	return hdmi_input_logic_index_table[logic];
}

#if _ENABLE_IT663XX_MULTI_CHIP_IN_USE
void IT663XX_SetRxSelByLogic(iTE_u8 logicAdr)
{
    if(logicAdr >= _HDMI_INPUT_PORT_USED_NUMBER)
        return;

	g_u8RxSel[RxPortMap[logicAdr].u8DevNum] = RxPortMap[logicAdr].u8PortIndex;
}

#else
void IT663XX_SetRxSelByLogic(iTE_u8 logicAdr)
{
	uint8_t i, u8RxSelPhyAdr, shift;

	u8RxSelPhyAdr = IT663XX_GetPhyAdrByLogic(logicAdr);

	for(i=0; i<NUM_IT663XX_USE; i++)
	{
		shift = (_HDMI_INPUT_PHYSIC_SHIFT*i);
		g_u8RxSel[i] = (u8RxSelPhyAdr>>shift)&_HDMI_INPUT_PHYSIC_MASK;
	}
}
#endif

u8 IT663xx_GetInputPortIndex(u8 u8DevId, u8 u8RxPort)
{
    u8 i;
    u8 l_u8InPort = 0;
    for(i=0;i<_HDMI_INPUT_PORT_USED_NUMBER;i++)
    {
        #if _ENABLE_IT663XX_MULTI_CHIP_IN_USE
        if((u8DevId==RxPortMap[i].u8DevNum)&&(u8RxPort==RxPortMap[i].u8PortIndex))
        {
            l_u8InPort = i;
            break;
        }
        #else
        {
            u8 l_u8RealPort;
            u8 l_u8TempPort;
            l_u8RealPort = hdmi_input_logic_index_table[i];
            if(u8DevId>0)
                l_u8TempPort = ((u8RxPort<<(_HDMI_INPUT_PHYSIC_SHIFT*u8DevId))|hdmi_input_node_rx_table[u8DevId]);
            else
                l_u8TempPort = u8RxPort;
            if(l_u8RealPort == l_u8TempPort)
            {
                l_u8InPort = i;
                break;
            }
        }
        #endif
    }
    return l_u8InPort;
}

iTE_u8 IT663XX_GetDeviceIndexByID(iTE_u8 u8Dev)
{
	iTE_u8 i;

	for(i=0; i<NUM_IT663XX_USE; i++)
	{
		if(u8Dev == IT663XX_Device[i])
		{
			break;
		}
	}

	return i;

}

void ITE663XX_I2sDisable(void)
{
	HSw_Bank(2);
	if((HSw_R(0x1A)&0x01)==0x01)
	{
		HSw_Set(0x1A, 0x01, 0x00);
	}
	HSw_Bank(0);
}

void ITE663XX_I2sEnable(void)
{
	HSw_Bank(2);
	if((HSw_R(0x1A)&0x01)==0x00)
	{
		HSw_Set(0x1A, 0x01, 0x01);
	}
	HSw_Bank(0);
}

void ITE663XX_SpdifDisable(void)
{
	HSw_Bank(2);
	if((HSw_R(0x1A)&0x20)==0x20)
	{
		HSw_Set(0x1A, 0x20, 0x00);
	}
	HSw_Bank(0);
}

void ITE663XX_SpdifEnable(void)
{
	HSw_Bank(2);
   	if((HSw_R(0x1A)&0x20)==0x00)
   	{
		HSw_Set(0x1A, 0x20, 0x20);
   	}
   	HSw_Bank(0);
}

void ITE663XX_AudioDecEnable(void)
{
    if((HSw_R(0x56)&0x10)==0)
    {
        HRx_Select(g_pu8RxSelComm[0]);
        if((g_pu8AudCodecComm[0] & ADO_DEC_MASK) == (ADO_DEC_EN | g_pu8CurRxPortComm[0] << ADO_DEC_SHIFT))
        {
			#if(IT663XX_VER == IT663XX_VER_1_07)
			HSw_AudioDecEn(iTE_TRUE);
			#else
            HSw_AudioDecEn(iTE_TRUE, g_u8AdoOut);
			#endif
        }
    }
}

void NumIT663XXSpdifOnOff(bool bEnable)
{
	iTE_u8 ii;

	if(bEnable)
	{
		if(!g_bIt663xxSpdifEnable)
		{
			g_bIt663xxSpdifEnable = TRUE;
			if(PollAudioProDlyTick(0) == 0)
			{
			    #if (_ENABLE_EDID_SWITCH_AUDIO == _MACRO_ENABLE)
				SetAudioProDlyTick(0, 500);// 1ms
				#else
                SetAudioProDlyTick(0, 1);// 1ms
                #endif
			}
		}
	}
	else
	{
		if(g_bIt663xxSpdifEnable)
		{
			for(ii=0; ii<NUM_IT663XX_USE; ii++)
			{
				iT663XX_DevSel(IT663XX_Device[ii]);
				ITE663XX_SpdifDisable();
			}
			g_bIt663xxSpdifEnable = FALSE;
		}
	}
}

void NumIT663XXIISOnOff(bool bEnable)
{
	iTE_u8 ii;

	if(bEnable)
	{
		if(!g_bIt663xxIISEnable)
		{
			g_bIt663xxIISEnable = TRUE;
			if(PollAudioProDlyTick(0) == 0)
			{
			    #if (_ENABLE_EDID_SWITCH_AUDIO == _MACRO_ENABLE)
				SetAudioProDlyTick(0, 500);// 1ms
		        #else
                SetAudioProDlyTick(0, 1);// 1ms
                #endif
			}
		}
	}
	else
	{
		if(g_bIt663xxIISEnable)
		{
			for(ii=0; ii<NUM_IT663XX_USE; ii++)
			{
				iT663XX_DevSel(IT663XX_Device[ii]);
				ITE663XX_I2sDisable();
			}
			g_bIt663xxIISEnable = FALSE;
		}
	}
}


#if _ENABLE_DEBUG_LOAD_ITE_CHIP_REGISTER
void mapp_Ite66341RegWriteByte(iTE_u8 u8DevID, iTE_u8 u8address, iTE_u16 u16offset, iTE_u8 u8data)
{
    iTE_u8 l_u8BankMask = 0;
    iTE_u8 l_u8Bank;
    iTE_u8 l_u8RealOffset;
    iTE_u8 l_u8PreDevice;

    l_u8Bank = (u16offset>>8)&0x0F;
    l_u8RealOffset = (iTE_u8)((u16offset)&0xFF);
    l_u8PreDevice = g_it663xx_device;

    iT663XX_DevSel(IT663XX_Device[u8DevID]);
    switch(u8address)
    {
        case SW_ADR:
            l_u8BankMask = 0xFC;
            break;
        case RX0_ADR:
            l_u8BankMask = 0xF8;
            break;
        case RX1_ADR:
            l_u8BankMask = 0xF8;
            break;
        case RX2_ADR:
            l_u8BankMask = 0xF8;
            break;
        case RX3_ADR:
            l_u8BankMask = 0xF8;
            break;
        default:
            break;
    }
    if(l_u8BankMask!=0)
        ChangeBank(u8address,l_u8BankMask,l_u8Bank);
	if(i2c_write_byte( u8address, l_u8RealOffset, 1, &u8data, u8I2CBus ) == 0){
		iTE_MsgReg(("mapp_Ite66341RegWriteByte write faile\n"));
	}
    if(l_u8BankMask!=0)
        ChangeBank(u8address,l_u8BankMask,0x00);

    iT663XX_DevSel(l_u8PreDevice);
}

void mapp_Ite66341RegReadByte(iTE_u8 u8DevID, iTE_u8 u8address, iTE_u16 u16offset, iTE_u8 *pu8data)
{
    iTE_u8 l_u8BankMask = 0;
    iTE_u8 l_u8Bank;
    iTE_u8 l_u8RealOffset;
    iTE_u8 l_u8PreDevice;

    l_u8Bank = (u16offset>>8)&0x0F;
    l_u8RealOffset = (iTE_u8)((u16offset)&0xFF);

    l_u8PreDevice = g_it663xx_device;

    iT663XX_DevSel(IT663XX_Device[u8DevID]);
    switch(u8address)
    {
        case SW_ADR:
            l_u8BankMask = 0xFC;
            break;
        case RX0_ADR:
            l_u8BankMask = 0xF8;
            break;
        case RX1_ADR:
            l_u8BankMask = 0xF8;
            break;
        case RX2_ADR:
            l_u8BankMask = 0xF8;
            break;
        case RX3_ADR:
            l_u8BankMask = 0xF8;
            break;
        default:
            break;
    }
    if(l_u8BankMask!=0)
        ChangeBank(u8address,l_u8BankMask,l_u8Bank);
	if(i2c_read_byte( u8address, l_u8RealOffset, 1, pu8data, u8I2CBus ) == 0){
		iTE_MsgReg(("mapp_Ite66341RegReadByte Read faile\n"));
	}
    if(l_u8BankMask!=0)
        ChangeBank(u8address,l_u8BankMask,0x00);

    iT663XX_DevSel(l_u8PreDevice);
}
#endif  //#if _ENABLE_DEBUG_LOAD_ITE_CHIP_REGISTER

#endif //#if(_PRJ_TYPE_SWITCH_SPLITER==_PRJ_SWITCHER_AND_SPLITER)||(_PRJ_TYPE_SWITCH_SPLITER==_PRJ_SWITCHER_ONLY)


#elif (IT663XX_VER == IT663XX_VER_1_07)
#include "../USER/video_chip/IT663xx_1.07/ITE663XX_comm_api.c"
#endif




