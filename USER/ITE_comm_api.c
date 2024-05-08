#include "includes.h"

stProjectGlobalAllVar_t g_stPrjGloVar;
#if (IT663XX_VER != IT663XX_NONE)
#if _ENABLE_EDID_SWITCH_A_SINGLE_CHANNEL
eIT663xxGetEdidSrc_t g_eIt663xxGetEdidFrom[_SWITCH_CHIP_NUM][IN_RXn] =
{
    #if (SUH4AU_H2_EDID_POWER_ON_NO_EDID == _MACRO_ENABLE)
    {   eIT663xxGetEdidFrom_Buffer,
        eIT663xxGetEdidFrom_Buffer,
        eIT663xxGetEdidFrom_Buffer,
        eIT663xxGetEdidFrom_Buffer},
    #else
    {   eIT663xxGetEdidFrom_Tx,
        eIT663xxGetEdidFrom_Tx,
        eIT663xxGetEdidFrom_Tx,
        eIT663xxGetEdidFrom_Tx},
    #endif
    #if (NUM_IT663XX_USE>=2)
    {   eIT663xxGetEdidFrom_Tx,
        eIT663xxGetEdidFrom_Tx,
        eIT663xxGetEdidFrom_Tx,
        eIT663xxGetEdidFrom_Tx},
    #endif
    #if (NUM_IT663XX_USE>=3)
    {   eIT663xxGetEdidFrom_Tx,
        eIT663xxGetEdidFrom_Tx,
        eIT663xxGetEdidFrom_Tx,
        eIT663xxGetEdidFrom_Tx},
    #endif
    #if (NUM_IT663XX_USE>=4)
    {   eIT663xxGetEdidFrom_Tx,
        eIT663xxGetEdidFrom_Tx,
        eIT663xxGetEdidFrom_Tx,
        eIT663xxGetEdidFrom_Tx},
    #endif
};
#else
eIT663xxGetEdidSrc_t    g_eIt663xxGetEdidFrom[_SWITCH_CHIP_NUM] =
{
#if (SUH4AU_H2_EDID_POWER_ON_NO_EDID == _MACRO_ENABLE)
    eIT663xxGetEdidFrom_Buffer,
#else
    eIT663xxGetEdidFrom_Tx,
#endif
};
#endif
#endif //add by wf8421
iTE_u8  g_pu8It663xxEdidBuffer[256];
iTE_u8  *System_Default_Edid_table = NULL;
#if _ENABLE_EDID_SWITCH_A_SINGLE_CHANNEL
iTE_u8  g_u8SingleChannelEdidBuffer[_HDMI_INPUT_PORT_USED_NUMBER][256];
#endif
const unsigned char DataStr[]=__DATE__;
const unsigned char TimeStr[]=__TIME__;
stVersionType g_stTestVersion;

uint8_t *g_edid_tabel_user_def = (uint8_t *)((volatile uint32_t*)(EDID_START_ADDR_SAME_WITH_BOOT));

u8 const AllPrjDefEdidTabl[256] =
{
    0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x55, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x32, 0x1C, 0x01, 0x03, 0x80, 0x50, 0x2D, 0x78, 0x0A, 0xEE, 0x91, 0xA3, 0x54, 0x4C, 0x99, 0x26,
    0x0F, 0x50, 0x54, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x3A, 0x80, 0x18, 0x71, 0x38, 0x2D, 0x40, 0x58, 0x2C,
    0x45, 0x00, 0x20, 0xC2, 0x31, 0x00, 0x00, 0x1E, 0x00, 0x00, 0x00, 0x11, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC,
    0x00, 0x48, 0x44, 0x20, 0x56, 0x69, 0x64, 0x65, 0x6F, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x01, 0x68,
    0x02, 0x03, 0x23, 0x72, 0x4B, 0x90, 0x9F, 0x22, 0x21, 0x20, 0x05, 0x14, 0x04, 0x13, 0x12, 0x03,
    0x23, 0x09, 0x1F, 0x07, 0x83, 0x01, 0x00, 0x00, 0x67, 0x03, 0x0C, 0x00, 0x10, 0x00, 0x00, 0x20,
    0xE2, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xD7,
};

iTE_u8 const ExtendEdidTable[18] = //4k@30, 297M
{
    0x04,0x74,0x00,0x30,0xF2,0x70,0x5A,0x80,0xB0,
    0x58,0x8A,0x00,0xC4,0x8E,0x21,0x00,0x00,0x1E,
};

static volatile u32 gTimerCounters[etINVALID];

extern const stRxHpdDetDeclare_t stProjectRxHpdDetTable[];

uint8_t CRC8Calculate(void *pBuf ,unsigned pBufSize)
{
    uint8_t     retCRCValue = 0x00, *pData, i=0, pDataBuf=0;

    pData=(unsigned char *)pBuf;
    while(pBufSize--)
    {
        pDataBuf=*pData++;
        for(i=0;i<8;i++)
        {
            if((retCRCValue^(pDataBuf))&0x01)
            {
                retCRCValue^=0x18;
                retCRCValue>>=1;
                retCRCValue|=0x80;
            }
            else
            {
                retCRCValue>>=1;
            }
            pDataBuf>>=1;
        }
    }
    return retCRCValue;
}

iTE_u8 CalEdidCheckSum(iTE_u8 edid[],iTE_u8 block)
{
    iTE_u16 i,sum,Checksum;
    sum=0;
    Checksum=0;
    if(block)
    {
        for(i=0x80;i<0xFE;i++)  sum=sum+edid[i];
    }

    else
    {
        for(i=0x00;i<0x7F;i++)  sum=sum+edid[i];
    }
    Checksum= 0x100- (sum & 0xFF);
    return Checksum;
}

bool is_edid_valid(iTE_u8 *edid_table)
{
    if((edid_table[0]==0x00)&&(edid_table[1]==0xFF)&&(edid_table[2]==0xFF)&&\
        (edid_table[3]==0xFF)&&(edid_table[4]==0xFF)&&(edid_table[5]==0xFF)&&\
        (edid_table[6]==0xFF)&&(edid_table[7]==0x00))
    {
        return TRUE;
    }

    return FALSE;
}

#if (FUNCTION_MACRO_EDID_MODIFY_TO_STANDARD == _MACRO_ENABLE)
const uint8_t u8Const4k60StandardEdid[18] = {
0x08, 0xE8, 0x00, 0x30, 0xF2, 0x70, 0x5A, 0x80, 0xB0, 0x58, 0x8A, 0x00, 0xC4, 0x8E, 0x21, 0x00, 0x00, 0x1E};

const uint8_t u8Const4k30StandardEdid[18] = {
0x04, 0x74, 0x00, 0x30, 0xF2, 0x70, 0x5A, 0x80, 0xB0, 0x58, 0x8A, 0x00, 0x40, 0x84, 0x63, 0x00, 0x00, 0x1E};

void ModifyStandard4kEdid(unsigned char *edid_table)
{
    unsigned short u16PixelClk, u16Hactive, u16Htotal, u16Vactive, u16Vtotal;

    u16PixelClk = edid_table[55];
    u16PixelClk <<= 8;
    u16PixelClk += edid_table[54];

    u16Hactive = edid_table[58];
    u16Hactive <<= 4;
    u16Hactive &= 0xF00;
    u16Hactive += edid_table[56];

    u16Htotal = edid_table[58];
    u16Htotal <<= 8;
    u16Htotal &= 0xF00;
    u16Htotal += edid_table[57];
    u16Htotal += u16Hactive;

    u16Vactive = edid_table[61];
    u16Vactive <<= 4;
    u16Vactive &= 0xF00;
    u16Vactive += edid_table[59];

    u16Vtotal = edid_table[61];
    u16Vtotal <<= 8;
    u16Vtotal &= 0xF00;
    u16Vtotal += edid_table[60];
    u16Vtotal += u16Vactive;

    if(u16PixelClk > 34000) // pixel clock > 34000
    {
        if((u16Hactive==3840)&&(u16Vactive==2160))
        {
            if((u16Htotal!=4400)||(u16Vtotal!=2250))
            {
                memcpy(&edid_table[54], u8Const4k60StandardEdid, 18);
                edid_table[127] = CalEdidCheckSum(edid_table, 0);
            }
        }
    }
    else
    {
        if((u16Hactive==3840)&&(u16Vactive==2160))
        {
            if((u16Htotal!=4400)||(u16Vtotal!=2250))
            {
                memcpy(&edid_table[54], u8Const4k30StandardEdid, 18);
                edid_table[127] = CalEdidCheckSum(edid_table, 0);
            }
        }
    }
}
#endif

bool is_edid_compare_same(iTE_pu8 arry1,iTE_pu8 arry2)
{
    iTE_u16 i;
    bool ret=TRUE;

    for(i=0;i<256;i++)  // 127
    {
        if(*(arry1+i) != *(arry2+i))
        {
            ret = FALSE;
            break;
        }
    }
    return ret;
}

void LimitedVSDBMaxPixel(uint8_t* pEdidBlk1, uint8_t MaxPixelC)
{
    uint8_t u8BlkTag = 0, u8BlkLen = 0, DTDoffset, offset = 4;
    bool bVSDBFind = FALSE;

    DTDoffset = pEdidBlk1[2];

    do
    {
        u8BlkTag = pEdidBlk1[offset]>>5;
        u8BlkLen = pEdidBlk1[offset]&0x1F;
        if(u8BlkTag == 3)//VSDB block TAG
        {
            bVSDBFind = TRUE;
            break;
        }
        offset += (u8BlkLen+1);
    }while(offset < DTDoffset);

    if(bVSDBFind)
    {
        #if (_EN_USE_REMOVE_EDID_DEEPCOLOR == _MACRO_ENABLE)
        if((pEdidBlk1[offset+6]&0xF0)!=0)
        {
            pEdidBlk1[offset+6] &= 0x0F;//remove Deepcolor
        }
        #endif

        if(pEdidBlk1[offset+7] > MaxPixelC)//max pixel clock
        {
            pEdidBlk1[offset+7] = MaxPixelC;
        }
    }
}

#if Project_SCU61E_66341_AUTO_SWITCH
void SetAutoSwDetectTime(uint32_t u32SetTime)
{
    g_vu32AutoDectTimeCnt = u32SetTime;
}

void AutoSwDetectTime1MsLoop(void)
{
    if(g_vu32AutoDectTimeCnt > 0)
    {
        g_vu32AutoDectTimeCnt --;
    }
}

bool PollAutoSwDetectStatus(void)
{
    //detect signal status cycles time
    if(g_vu32AutoDectTimeCnt == 0)
    {
        SetAutoSwDetectTime(_TIME_AUTO_SWITCH_DETECT);
        return TRUE;
    }

    return FALSE;
}

#endif

void mapp_UserData_HdmiSelInit(void)
{
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    pstHdmiSwitchParam_t        pstHdmiSw = &pstPrjSaveVar->stHdmiSel;
    #if _ENABLE_MULTI_OUTPUT_CHIP
    u8 l_u8index;
    #endif

    #if _ENABLE_MULTI_OUTPUT_CHIP
    for(l_u8index=0;l_u8index<Def_outport_num;l_u8index++)
    {
        pstHdmiSw->bAutoSwitch[l_u8index] = Def_AutoSwitchMode;   //auto switch mode
        pstHdmiSw->u8SwChannel[l_u8index] = l_u8index/*Def_DefaultChannel*/;      //the first channel
    }
    #else
    pstHdmiSw->bAutoSwitch = Def_AutoSwitchMode;//auto switch mode
    pstHdmiSw->u8SwChannel = Def_DefaultChannel;//the first channel
    #endif
}

bool mapp_UserData_HdmiSelCheck(void)
{
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    pstHdmiSwitchParam_t        pstHdmiSw = &pstPrjSaveVar->stHdmiSel;
    u8 l_u8CheckSum;
    bool l_bState = TRUE;
    l_u8CheckSum = CRC8Calculate((uint8_t *)pstHdmiSw,((uint8_t *)&pstHdmiSw->u8CheckSum - (uint8_t *)pstHdmiSw));
    if(l_u8CheckSum!= pstHdmiSw->u8CheckSum)
    {
        mapp_UserData_HdmiSelInit();
        pstHdmiSw->u8CheckSum = l_u8CheckSum;

        l_bState = FALSE;
    }

    return l_bState;
}


#if 1//_ENABLE_MULTI_OUTPUT_CHIP
void SetUserData_HdmiSel(void)
{
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    pstHdmiSwitchParam_t        pstHdmiSw = &pstPrjSaveVar->stHdmiSel;

    //pstHdmiSw->u8SwChannel = HdmiRxSel;
    //pstHdmiSw->bAutoSwitch = isAutoMode;
    pstHdmiSw->u8CheckSum = CRC8Calculate((uint8_t *)pstHdmiSw,((uint8_t *)&pstHdmiSw->u8CheckSum - (uint8_t *)pstHdmiSw));

    #if ((_EN_EEPROM_FUN_SAVE_HDMI_SEL==_MACRO_ENABLE)||(_EN_EEPROM_FUN_SAVE_TYPE_SWITCH_MODE==_MACRO_ENABLE))
    SaveUserDataToEep(eEepDataIndex_HdmiSel,(ADDR_EEPROM_USER_START+((uint8_t *)pstHdmiSw-(uint8_t *)pstPrjSaveVar)),(uint8_t *)pstHdmiSw,sizeof(stHdmiSwitchParam_t),TIME_SAVE_TO_EEPROM_IMMEDIATELY, FALSE);
    #endif
}
#else
void SetUserData_HdmiSel(uint8_t HdmiRxSel, bool isAutoMode)
{
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    pstHdmiSwitchParam_t        pstHdmiSw = &pstPrjSaveVar->stHdmiSel;

    pstHdmiSw->u8SwChannel = HdmiRxSel;
    pstHdmiSw->bAutoSwitch = isAutoMode;
    pstHdmiSw->u8CheckSum = CRC8Calculate((uint8_t *)pstHdmiSw,((uint8_t *)&pstHdmiSw->u8CheckSum - (uint8_t *)pstHdmiSw));

    #if ((_EN_EEPROM_FUN_SAVE_HDMI_SEL==_MACRO_ENABLE)||(_EN_EEPROM_FUN_SAVE_TYPE_SWITCH_MODE==_MACRO_ENABLE))
    SaveUserDataToEep(eEepDataIndex_HdmiSel,(ADDR_EEPROM_USER_START+((uint8_t *)pstHdmiSw-(uint8_t *)pstPrjSaveVar)),(uint8_t *)pstHdmiSw,sizeof(stHdmiSwitchParam_t),TIME_SAVE_TO_EEPROM_IMMEDIATELY, FALSE);
    #endif
}
#endif

void mapp_UserData_HdmiSelAutoSave(void)
{
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    pstHdmiSwitchParam_t        pstHdmiSw = &pstPrjSaveVar->stHdmiSel;
    u8 l_u8CheckSum;
    l_u8CheckSum = CRC8Calculate((uint8_t *)pstHdmiSw,((uint8_t *)&pstHdmiSw->u8CheckSum - (uint8_t *)pstHdmiSw));
    if(l_u8CheckSum!= pstHdmiSw->u8CheckSum)
    {
        SetUserData_HdmiSel();
        pstHdmiSw->u8CheckSum = l_u8CheckSum;

    }
}

void mapp_UserDataHdcpInit(void)
{
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    pstHdcpStatusOutPort_t      pstHdcpStatusOp = &pstPrjSaveVar->stHdcpStatusOp;
    u8 l_u8Index;
    for(l_u8Index = 0;l_u8Index <_HDMI_O_PORT_NUM; l_u8Index++)
    {
        pstHdcpStatusOp->u8HdcpSta[l_u8Index] = Def_OutputHdcpMode;
    }
}

bool mapp_UserDataHdcpCheck(void)
{
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    pstHdcpStatusOutPort_t      pstHdcpStatusOp = &pstPrjSaveVar->stHdcpStatusOp;
    u8 l_u8CheckSum;
    bool l_bState = TRUE;

    l_u8CheckSum = CRC8Calculate((uint8_t *)pstHdcpStatusOp,((uint8_t *)&pstHdcpStatusOp->u8CheckSum - (uint8_t *)pstHdcpStatusOp));
    if(l_u8CheckSum != pstHdcpStatusOp->u8CheckSum)
    {
        mapp_UserDataHdcpInit();
        pstHdcpStatusOp->u8CheckSum = l_u8CheckSum;
        SetUserData_HDCP();
        l_bState = FALSE;
    }

    return l_bState;
}

void SetUserData_HDCP(void)
{
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    pstHdcpStatusOutPort_t      pstHdcpStatusOp = &pstPrjSaveVar->stHdcpStatusOp;

    //pstHdcpStatusOp->u8HdcpSta[u8OutIndex] = u8HdcpState;
    pstHdcpStatusOp->u8CheckSum = CRC8Calculate((uint8_t *)pstHdcpStatusOp,((uint8_t *)&pstHdcpStatusOp->u8CheckSum - (uint8_t *)pstHdcpStatusOp));

#if ((_EN_EEPROM_FUN_SAVE_HDCP_STATUS==_MACRO_ENABLE))
    SaveUserDataToEep(eEepDataIndex_HdcpStatus,(ADDR_EEPROM_USER_START+((uint8_t *)pstHdcpStatusOp-(uint8_t *)pstPrjSaveVar)),(uint8_t *)pstHdcpStatusOp,sizeof(stHdcpStatusOutPort_t),TIME_SAVE_TO_EEPROM_IMMEDIATELY, FALSE);
#endif
}

bool mapp_UserDataHdcpAutoSave(void)
{
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    pstHdcpStatusOutPort_t      pstHdcpStatusOp = &pstPrjSaveVar->stHdcpStatusOp;
    u8 l_u8CheckSum;
    bool l_bState = TRUE;

    l_u8CheckSum = CRC8Calculate((uint8_t *)pstHdcpStatusOp,((uint8_t *)&pstHdcpStatusOp->u8CheckSum - (uint8_t *)pstHdcpStatusOp));
    if(l_u8CheckSum != pstHdcpStatusOp->u8CheckSum)
    {
        SetUserData_HDCP();
        pstHdcpStatusOp->u8CheckSum = l_u8CheckSum;
        SetUserData_HDCP();
        l_bState = FALSE;
    }

    return l_bState;
}

#if _ENABLE_UART_LEARNING_MESSAGE_FUNCTION
void mapp_UserDataUartKeyValInit(u8 u8Index)
{
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    pstUartKeyValue_t           pstUartKey = &pstPrjSaveVar->stUartStudyKeyData[u8Index];

    memset(pstUartKey->sKeyValueBuffer, 0 ,SIZE_UART_KEY_VALUE_MAX);
    pstUartKey->u16KeyValueLen = 0;
    pstUartKey->u32BaudRate = 9600;

    pstUartKey->bUartValueHexOrAsciiFlag = FALSE;  // TRUE: HEX ; FALSE: ASCII
    pstUartKey->u16UartValueSendDelayTime = Def_Uart_MessageSendDelayTime;
    pstUartKey->u8UartValueSendMessageNum = Def_Uart_MessageCounter;
    pstUartKey->u16UartValueCounterDelayTime = Def_Uart_CounterDelayTime;
    #if _ENABLE_UART_LEARN_TO_MULTI_FUNCTION
    pstUartKey->u8SendPort = 0;
    pstUartKey->u8SendWhoFollow = 0;
    #endif
}

bool mapp_UserDataUartKeyValCheck(void)
{
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    u8 l_u8Index;
    u8 l_u8CheckSum;
    bool l_bState = TRUE;

    for(l_u8Index=0; l_u8Index<_NUM_UART_STUDY_KEYS; l_u8Index++)
    {
        pstUartKeyValue_t           pstUartKey = &pstPrjSaveVar->stUartStudyKeyData[l_u8Index];
        l_u8CheckSum = CRC8Calculate((uint8_t *)pstUartKey, ((uint8_t *)&pstUartKey->u8CheckSum - (uint8_t *)pstUartKey));
        if(l_u8CheckSum != pstUartKey->u8CheckSum)
        {
            mapp_UserDataUartKeyValInit(l_u8Index);
            pstUartKey->u8CheckSum = l_u8CheckSum;
            l_bState = FALSE;
        }
    }

    return l_bState;
}

void SetUserData_UartKeyVal(uint8_t u8KeyIndex)
{
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    pstUartKeyValue_t           pstUartKey = &pstPrjSaveVar->stUartStudyKeyData[u8KeyIndex];

    pstUartKey->u8CheckSum = CRC8Calculate((uint8_t *)pstUartKey, ((uint8_t *)&pstUartKey->u8CheckSum - (uint8_t *)pstUartKey));

    #if (_EN_EEPROM_FUN_SAVE_UART_STUDY_KEYS==_MACRO_ENABLE)
    SaveUserDataToEep(eEepDataIndex_UartKeys,(ADDR_EEPROM_USER_START+((uint8_t *)pstUartKey-(uint8_t *)pstPrjSaveVar)),(uint8_t *)pstUartKey,sizeof(stUartKeyValue_t),TIME_SAVE_TO_EEPROM_IMMEDIATELY, FALSE);
    #endif
}

void SetUserData_UartKeyValSaveAll(void)
{
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    pstUartKeyValue_t           pstUartKey = &pstPrjSaveVar->stUartStudyKeyData[0];

    pstUartKey->u8CheckSum = CRC8Calculate((uint8_t *)pstUartKey, ((uint8_t *)&pstUartKey->u8CheckSum - (uint8_t *)pstUartKey));

    #if (_EN_EEPROM_FUN_SAVE_UART_STUDY_KEYS==_MACRO_ENABLE)
    SaveUserDataToEep(eEepDataIndex_UartKeys,(ADDR_EEPROM_USER_START+((uint8_t *)pstUartKey-(uint8_t *)pstPrjSaveVar)),(uint8_t *)pstUartKey,(sizeof(stUartKeyValue_t)*(_NUM_UART_STUDY_KEYS)),TIME_SAVE_TO_EEPROM_IMMEDIATELY, FALSE);
    #endif
}

#endif

#if ((_IR_FUNCTION_USE&_IR_FUNCTION_KEY_STUDY)==_IR_FUNCTION_KEY_STUDY)
void mapp_UserDataIrStudyValInit(u8 u8Index)
{
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;

    memset((u8 *)pstPrjSaveVar->stIrStudyKeyData[u8Index].u16KeyWave, 0 ,IR_WAVE_STUDY_BIT_CNT_MAX*2);
    pstPrjSaveVar->stIrStudyKeyData[u8Index].u16BitCnt = 0;
}

bool mapp_UserDataIrStudyValCheck(void)
{
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    u8 l_u8Index;
    u8 l_u8CheckSum;
    bool l_bState = TRUE;

    for(l_u8Index=0; l_u8Index<_NUM_IR_STUDY_KEYS; l_u8Index++)
    {
        pstIrKeyWave_t           pstIrStudy = &pstPrjSaveVar->stIrStudyKeyData[l_u8Index];
        l_u8CheckSum = CRC8Calculate((uint8_t *)pstIrStudy, ((uint8_t *)&pstIrStudy->u8CheckSum - (uint8_t *)pstIrStudy));
        if(l_u8CheckSum != pstIrStudy->u8CheckSum)
        {
            mapp_UserDataIrStudyValInit(l_u8Index);
            pstIrStudy->u8CheckSum = l_u8CheckSum;
            l_bState = FALSE;
        }
    }

    return l_bState;
}

void SetUserData_IrStudyVal(uint8_t u8KeyIndex, bool bIsr)
{
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    pstIrKeyWave_t          pstIrStudy = &pstPrjSaveVar->stIrStudyKeyData[u8KeyIndex];

    pstIrStudy->u8CheckSum = CRC8Calculate((uint8_t *)pstIrStudy, ((uint8_t *)&pstIrStudy->u8CheckSum - (uint8_t *)pstIrStudy));

    #if ((_IR_FUNCTION_USE&_IR_FUNCTION_KEY_STUDY)==_IR_FUNCTION_KEY_STUDY)
    SaveUserDataToEep(eEepDataIndex_IrKeys,(ADDR_EEPROM_USER_START+((uint8_t *)pstIrStudy-(uint8_t *)pstPrjSaveVar)),(uint8_t *)pstIrStudy,sizeof(stIrKeyWave_t),TIME_SAVE_TO_EEPROM_IMMEDIATELY, bIsr);
    #endif
}

void SetUserData_IrStudyValSaveAll(bool bIsr)
{
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    pstIrKeyWave_t          pstIrStudy = &pstPrjSaveVar->stIrStudyKeyData[0];

    pstIrStudy->u8CheckSum = CRC8Calculate((uint8_t *)pstIrStudy, ((uint8_t *)&pstIrStudy->u8CheckSum - (uint8_t *)pstIrStudy));

    #if ((_IR_FUNCTION_USE&_IR_FUNCTION_KEY_STUDY)==_IR_FUNCTION_KEY_STUDY)
    SaveUserDataToEep(eEepDataIndex_IrKeys,(ADDR_EEPROM_USER_START+((uint8_t *)pstIrStudy-(uint8_t *)pstPrjSaveVar)),(uint8_t *)pstIrStudy,(sizeof(stIrKeyWave_t)*(_NUM_IR_STUDY_KEYS)),TIME_SAVE_TO_EEPROM_IMMEDIATELY, bIsr);
    #endif
}
#endif //add by wf8421

#if (_ENABLE_EDID_DEFAULT_NOT_TO_SAME&&_ENABLE_EDID_SWITCH_A_SINGLE_CHANNEL)
void mapp_UserDataEdidIndexInit(void)
{
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    pstEdidSelIndex_t           pstEdidIndex = &pstPrjSaveVar->stEdidSelIndex;

    pstEdidIndex->u8EdidFlashIndex = Def_UserEdidMode;

    #if _ENABLE_SAVE_CURRENT_EDID
    if(System_Default_Edid_table == NULL)
    {
        //System_Default_Edid_table = (iTE_u8 *)AllPrjDefEdidTabl;
        memcpy(pstEdidIndex->u8CurEdidbuf, AllPrjDefEdidTabl,256);
    }
    else
    {
        memcpy(pstEdidIndex->u8CurEdidbuf, System_Default_Edid_table,256);
    }
    #endif
    #if _ENABLE_EDID_SWITCH_A_SINGLE_CHANNEL
    {
//        u8 i;
//        for(i=0;i<_HDMI_INPUT_PORT_USED_NUMBER;i++)
//          pstEdidIndex->u8EdidChannelMode[i] = Def_UserEdidMode;

        pstEdidIndex->u8EdidChannelMode[0] = Def_UserEdidMode0;
        #if (_HDMI_INPUT_PORT_USED_NUMBER >= 2)
        pstEdidIndex->u8EdidChannelMode[1] = Def_UserEdidMode1;
        #endif //add by wf8421
        #if (_HDMI_INPUT_PORT_USED_NUMBER >= 3)
        pstEdidIndex->u8EdidChannelMode[2] = Def_UserEdidMode2;
        #endif //add by wf8421
        #if (_HDMI_INPUT_PORT_USED_NUMBER >= 4)
        pstEdidIndex->u8EdidChannelMode[3] = Def_UserEdidMode3;
        #endif //add by wf8421
        #if (_HDMI_INPUT_PORT_USED_NUMBER >= 5)
        pstEdidIndex->u8EdidChannelMode[4] = Def_UserEdidMode4;
        #endif //add by wf8421
        #if (_HDMI_INPUT_PORT_USED_NUMBER >= 6)
        pstEdidIndex->u8EdidChannelMode[5] = Def_UserEdidMode5;
        #endif //add by wf8421
        #if (_HDMI_INPUT_PORT_USED_NUMBER >= 7)
        pstEdidIndex->u8EdidChannelMode[6] = Def_UserEdidMode6;
        #endif //add by wf8421
        #if (_HDMI_INPUT_PORT_USED_NUMBER >= 8)
        pstEdidIndex->u8EdidChannelMode[7] = Def_UserEdidMode7;
        #endif //add by wf8421
    }
    #endif
}

#else
void mapp_UserDataEdidIndexInit(void)
{
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    pstEdidSelIndex_t           pstEdidIndex = &pstPrjSaveVar->stEdidSelIndex;

    pstEdidIndex->u8EdidFlashIndex = Def_UserEdidMode;

    #if _ENABLE_SAVE_CURRENT_EDID
    if(System_Default_Edid_table == NULL)
    {
        //System_Default_Edid_table = (iTE_u8 *)AllPrjDefEdidTabl;
        memcpy(pstEdidIndex->u8CurEdidbuf, AllPrjDefEdidTabl,256);
    }
    else
    {
        memcpy(pstEdidIndex->u8CurEdidbuf, System_Default_Edid_table,256);
    }
    #endif
    #if _ENABLE_EDID_SWITCH_A_SINGLE_CHANNEL
    {
        u8 i;
        for(i=0;i<_HDMI_INPUT_PORT_USED_NUMBER;i++)
            pstEdidIndex->u8EdidChannelMode[i] = Def_UserEdidMode;
    }
    #endif
}
#endif //add by wf8421

bool mapp_UserDataEdidIndexCheck(void)
{
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    pstEdidSelIndex_t           pstEdidIndex = &pstPrjSaveVar->stEdidSelIndex;
    u8 l_u8CheckSum;
    bool l_bState = TRUE;

    l_u8CheckSum = CRC8Calculate((uint8_t *)pstEdidIndex, ((uint8_t *)&pstEdidIndex->u8CheckSum - (uint8_t *)pstEdidIndex));
    if(l_u8CheckSum != pstEdidIndex->u8CheckSum)
    {
        mapp_UserDataEdidIndexInit();
        #if (_EN_EEPROM_FUN_SAVE_SYS_EDID_INDEX==_MACRO_ENABLE)
        SetUserData_EdidIndexSave();
        #endif
        l_bState = FALSE;
    }

    return l_bState;
}

void SetUserData_EdidIndexSave(void)
{
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    pstEdidSelIndex_t           pstEdidIndex = &pstPrjSaveVar->stEdidSelIndex;

    #if _ENABLE_SAVE_CURRENT_EDID
    memcpy(pstEdidIndex->u8CurEdidbuf,g_pu8It663xxEdidBuffer, 256);
    #endif
    pstEdidIndex->u8CheckSum = CRC8Calculate((uint8_t *)pstEdidIndex, ((uint8_t *)&pstEdidIndex->u8CheckSum - (uint8_t *)pstEdidIndex));

    #if (_EN_EEPROM_FUN_SAVE_SYS_EDID_INDEX==_MACRO_ENABLE)
    SaveUserDataToEep(eEepDataIndex_SysEdidIndex,(ADDR_EEPROM_USER_START+((uint8_t *)pstEdidIndex-(uint8_t *)pstPrjSaveVar)),(uint8_t *)pstEdidIndex,sizeof(stEdidSelIndex_t),TIME_SAVE_TO_EEPROM_IMMEDIATELY, FALSE);
    #endif
}

void mapp_UserDataProjectNameInit(void)
{
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    pstProjectNameSet_t         pstPrjNameSet = &pstPrjSaveVar->stPrjNameSet;

    memset(pstPrjNameSet->sPrjName, 0, SIZE_PRJ_NAME_CHARS_MAX);
    pstPrjNameSet->m_u8Len = 0;
}

bool mapp_UserDataProjectNameCheck(void)
{
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    pstProjectNameSet_t         pstPrjNameSet = &pstPrjSaveVar->stPrjNameSet;
    u8 l_u8CheckSum;
    bool l_bState = TRUE;

    l_u8CheckSum = CRC8Calculate((uint8_t *)pstPrjNameSet, ((uint8_t *)&pstPrjNameSet->u8CheckSum - (uint8_t *)pstPrjNameSet));
    if(l_u8CheckSum != pstPrjNameSet->u8CheckSum)
    {
        pstPrjNameSet->u8CheckSum = l_u8CheckSum;
        mapp_UserDataProjectNameInit();
        l_bState = FALSE;
    }

    return l_bState;
}

void SetUserData_ProjectName(char *strPrjName, uint8_t len)
{
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    pstProjectNameSet_t         pstPrjNameSet = &pstPrjSaveVar->stPrjNameSet;

    memset(pstPrjNameSet->sPrjName, 0, SIZE_PRJ_NAME_CHARS_MAX);
    memcpy(pstPrjNameSet->sPrjName, strPrjName, len);
    pstPrjNameSet->m_u8Len = len;

    pstPrjNameSet->u8CheckSum = CRC8Calculate((uint8_t *)pstPrjNameSet, ((uint8_t *)&pstPrjNameSet->u8CheckSum - (uint8_t *)pstPrjNameSet));

    SaveUserDataToEep(eEepDataIndex_ProjectName,(ADDR_EEPROM_USER_START+((uint8_t *)pstPrjNameSet-(uint8_t *)pstPrjSaveVar)),(uint8_t *)pstPrjNameSet,sizeof(stProjectNameSet_t),TIME_SAVE_TO_EEPROM_IMMEDIATELY, FALSE);
}


bool isRxHpdDetNull(uint8_t u8RxIndex)
{
    uint8_t u8TblElmtNum = 0;
    pstRxHpdDetDeclare_t pstRxHpd, pstRxHpdOrg = (pstRxHpdDetDeclare_t)&stProjectRxHpdDetTable[0];

    pstRxHpd = pstRxHpdOrg;
    while(pstRxHpd->pstDetPort != NULL)
    {
        u8TblElmtNum ++;
        pstRxHpd ++;
    }

    if(u8TblElmtNum == 0) return TRUE;
    if(u8RxIndex >= u8TblElmtNum) return TRUE;

    pstRxHpd = pstRxHpdOrg + u8RxIndex;
    if(pstRxHpd->pstDetPort == NULL) return TRUE;

    return FALSE;
}

bool isGetHpdDetConnect(uint8_t u8RxIndex)
{
    pstRxHpdDetDeclare_t pstRxHpd = (pstRxHpdDetDeclare_t)&stProjectRxHpdDetTable[u8RxIndex];

    if((pstRxHpd->pstDetPort->u8PortType>=PORT_GPIOA)&&(pstRxHpd->pstDetPort->u8PortType<=PORT_GPIOG))
    {
        if(pstRxHpd->pstDetPort->eInitLevel == ePinLevel_High)
        {
            //if((STM32_GPIO_PORT[pstRxHpd->pstDetPort->u8PortType-PORT_GPIOA]->IDR & pstRxHpd->pstDetPort->u16GpioPin) == pstRxHpd->pstDetPort->u16GpioPin)
            if(GPIO_ReadInputDataBit(STM32_GPIO_PORT[pstRxHpd->pstDetPort->u8PortType-PORT_GPIOA],pstRxHpd->pstDetPort->u16GpioPin))
            {
                return TRUE;
            }
        }
        else
        {
            //if((STM32_GPIO_PORT[pstRxHpd->pstDetPort->u8PortType-PORT_GPIOA]->IDR & pstRxHpd->pstDetPort->u16GpioPin) == 0)
            if(GPIO_ReadInputDataBit(STM32_GPIO_PORT[pstRxHpd->pstDetPort->u8PortType-PORT_GPIOA],pstRxHpd->pstDetPort->u16GpioPin)==0)
            {
                return TRUE;
            }
        }
    }

    return FALSE;
}

#if (IT663XX_VER != IT663XX_NONE)
#if _ENABLE_EDID_SWITCH_A_SINGLE_CHANNEL
eIT663xxGetEdidSrc_t GetIT663xxEdidSource(uint8_t u8ChipIndex,u8 u8RxPort)
{
    return g_eIt663xxGetEdidFrom[u8ChipIndex][u8RxPort];
}

void SetIT663xxEdidSource(uint8_t u8ChipIndex,u8 u8RxPort, eIT663xxGetEdidSrc_t eEdidSrc)
{
    g_eIt663xxGetEdidFrom[u8ChipIndex][u8RxPort] = eEdidSrc;
}
#else
eIT663xxGetEdidSrc_t GetIT663xxEdidSource(uint8_t u8ChipIndex)
{
    return g_eIt663xxGetEdidFrom[u8ChipIndex];
}

void SetIT663xxEdidSource(uint8_t u8ChipIndex, eIT663xxGetEdidSrc_t eEdidSrc)
{
    g_eIt663xxGetEdidFrom[u8ChipIndex] = eEdidSrc;
}
#endif
#endif //add by wf8421
void FillIT663xxEdidData(iTE_u8 *pBuffer)
{
    if(is_edid_valid(pBuffer))
    {
        memcpy(g_pu8It663xxEdidBuffer, pBuffer, 256);
    }
    else
    {
        memcpy(g_pu8It663xxEdidBuffer, System_Default_Edid_table, 256);
    }
}

#if _ENABLE_EDID_SWITCH_A_SINGLE_CHANNEL
void FillIT663xxEdidDataForSingleChannel(iTE_u8 *pBuffer,u8 u8InPort)
{
    if(is_edid_valid(pBuffer))
    {
        memcpy(g_u8SingleChannelEdidBuffer[u8InPort], pBuffer, 256);
    }
    else
    {
        memcpy(g_u8SingleChannelEdidBuffer[u8InPort], System_Default_Edid_table, 256);
    }
}

void GetIT663xxEdidDataForSingleChannel(iTE_u8 *pu8GetBuffer,u8 u8InPort)
{
    memcpy(pu8GetBuffer, g_u8SingleChannelEdidBuffer[u8InPort], 256);
}
#endif

#if 1
#ifndef _CLR_SAMSUN_EDID_SERIAL_NUM
#define _CLR_SAMSUN_EDID_SERIAL_NUM         _MACRO_ENABLE
#endif

#ifndef _CLR_YCBCR420_EDID_BLOCK
#define _CLR_YCBCR420_EDID_BLOCK            _MACRO_ENABLE
#endif

#ifndef _CLR_VIDEO_DATA_BLOCK_6G
#define _CLR_VIDEO_DATA_BLOCK_6G            _MACRO_ENABLE
#endif

#ifndef _CLR_VSDB_HDMI_FORUM_BLOCK
#define _CLR_VSDB_HDMI_FORUM_BLOCK          _MACRO_ENABLE
#endif

#ifndef _SET_AUDIO_DATA_BLOCK_LIMIT_PCM20
#define _SET_AUDIO_DATA_BLOCK_LIMIT_PCM20   _MACRO_ENABLE
#endif

#ifndef _SET_DOLBY_VISION_LIMIT_4K30
#define _SET_DOLBY_VISION_LIMIT_4K30        _MACRO_DISABLE
#endif

#ifndef _CLR_DOLBY_VISION_BLOCK
#define _CLR_DOLBY_VISION_BLOCK             _MACRO_DISABLE
#endif

#ifndef _CLR_HDR_BLOCK
#define _CLR_HDR_BLOCK                      _MACRO_DISABLE
#endif

#ifndef _SET_RANGE_LIMITS_BLOCK
#define _SET_RANGE_LIMITS_BLOCK             _MACRO_DISABLE
#endif

/***************************************************
**Function: mapp_EdidLimitTo4K30
**Input:    u8pInEdid(Input Edid data),u16len(Edid length),
            bEnInterlace(Is Interlace Timing use?)
            bOnlyPcm20(Is only pcm2.0?)
**Output:   u8pOutEdid(Output Edid data)
**Return:   Edid Status(Is Edid Ok?)
**Description:  Limited Edid to 4k30 when Edid up to 4K60
***************************************************/
bool mapp_EdidLimitTo4K30(u8 *u8pInEdid,
                                    u8 *u8pOutEdid,
                                    u16 u16len,
                                    bool bEnInterlace,
                                    bool bOnlyPcm20)
{
    u8 i;
    u8 l_u8CheckSum=0;
    uint8_t const l_u8DefaultEdidTiming[18] = //4k@30, 297M
    {
        0x04,0x74,0x00,0x30,0xF2,0x70,0x5A,0x80,0xB0,
        0x58,0x8A,0x00,0x20,0xC2,0x31,0x00,0x00,0x1E,
    };

    if(is_edid_valid(u8pInEdid) == FALSE)
    {
        return FALSE;
    }
    memcpy(u8pOutEdid,u8pInEdid,u16len);

    // block 0
    #if _CLR_SAMSUN_EDID_SERIAL_NUM
    if(u8pOutEdid[0x08]==0x4C&&u8pOutEdid[0x09]==0x2D)  // if ID is SAM, Change the ID serial number 00000000
    {
        u8pOutEdid[0x0C]=0x00;
        u8pOutEdid[0x0D]=0x00;
        u8pOutEdid[0x0E]=0x00;
        u8pOutEdid[0x0F]=0x00;
    }
    #endif

    if((((((u8pOutEdid[0x3A]>>4)<<8)|(u8pOutEdid[0x38]))==3840)
        ||((((u8pOutEdid[0x3A]>>4)<<8)|(u8pOutEdid[0x38]))==4096))
        &&((((u8pOutEdid[0x3D]>>4)<<8)|(u8pOutEdid[0x3B]))==2160))
    {
        if(((u8pOutEdid[0x37]<<8)|(u8pOutEdid[0x36]))>30000)
        {
            for(i=0;i<18;i++)
            {
                u8pOutEdid[0x36+i] = l_u8DefaultEdidTiming[i];
            }
        }
    }

    #if 0
    for(i=0x49;i<127;i++)
    {
        if((u8pOutEdid[i]==0x00)&&(u8pOutEdid[i+1]==0x00)&&(u8pOutEdid[i+2]==0x00)&&(u8pOutEdid[i+3]==0xFD))
        {
            if(u8pOutEdid[i+9]>0x1E)
            {
                u8pOutEdid[i+9] = 0x1E;
            }
        }
    }
    #endif

#if _SET_RANGE_LIMITS_BLOCK
    for(i=0x49;i<127;i++)
    {
        if((u8pOutEdid[i]==0x00)&&(u8pOutEdid[i+1]==0x00)&&(u8pOutEdid[i+2]==0x00)&&(u8pOutEdid[i+3]==0xFD))
        {
            if(u8pOutEdid[i+5]>0x17)
            {
                u8pOutEdid[i+5] = 0x17;
            }
            //if(u8pOutEdid[i+6]>0x55)
            //{
            //    u8pOutEdid[i+6] = 0x55;
            //}
            if(u8pOutEdid[i+7]>0x0F)
            {
                u8pOutEdid[i+7] = 0x0F;
            }
            //if(u8pOutEdid[i+8]>0x5C)
            //{
            //    u8pOutEdid[i+8] = 0x5C;
            //}
            if(u8pOutEdid[i+9]>0x1E)
            {
                u8pOutEdid[i+9] = 0x1E;
            }
        }
    }
#endif

    for(i=0;i<127;i++)
    {
        l_u8CheckSum += u8pOutEdid[i];
    }
    u8pOutEdid[0x7F] = 0x100-l_u8CheckSum;

    // block 1
    if((u8pOutEdid[0x7E]>0)&&(u16len>128))
    {
        if((u8pOutEdid[0x80] == 0x02) && (u8pOutEdid[0x81] == 0x03))
        {
            u8 l_u8DataBlockLen = u8pOutEdid[0x82];
            u8 j = 0;
            u8 l_u8Offect = 0;
            #if _CLR_VIDEO_DATA_BLOCK_6G
            u8 l_u8VdbId = 0;
            u8 l_pu8Vdb[31]={0};
            #endif

            for(i=4;i<l_u8DataBlockLen;i++)
            {
                l_u8Offect = u8pOutEdid[0x80+i]&0x1F;
                #if _CLR_VIDEO_DATA_BLOCK_6G
                if((u8pOutEdid[0x80+i]&0xE0)==0x40) // video data block
                {
                    for(j=0;j<l_u8Offect;j++)
                    {
                        if(bEnInterlace&&((((u8pOutEdid[0x80+i+1+j]&0x7F)>=96)&&((u8pOutEdid[0x80+i+1+j]&0x7F)<=97))
                            ||(((u8pOutEdid[0x80+i+1+j]&0x7F)>=101)&&((u8pOutEdid[0x80+i+1+j]&0x7F)<=102))
                            ||((u8pOutEdid[0x80+i+1+j]&0x7F)>=106)))
                            continue;

                        if((bEnInterlace==FALSE)&&(((u8pOutEdid[0x80+i+1+j]>=96)&&(u8pOutEdid[0x80+i+1+j]<=97))
                            ||((u8pOutEdid[0x80+i+1+j]>=101)&&(u8pOutEdid[0x80+i+1+j]<=102))
                            ||(u8pOutEdid[0x80+i+1+j]>=106)))  //  Delete timing up to 4K60
                        {
                            continue;
                        }
                        else
                        {
                            l_pu8Vdb[l_u8VdbId++] = u8pOutEdid[0x80+i+1+j];
                        }
                    }
                    if(l_u8VdbId != l_u8Offect)
                    {
                        u8pOutEdid[0x80+i] = (0x40|l_u8VdbId);
                        for(j=0;j<l_u8VdbId;j++)
                        {
                            u8pOutEdid[0x80+i+1+j] = l_pu8Vdb[j];
                        }
                        u8pOutEdid[0x82] = u8pOutEdid[0x82] - (l_u8Offect-l_u8VdbId);
                        l_u8DataBlockLen = u8pOutEdid[0x82];
                        for(j=(i+1+l_u8Offect);j<127;j++)
                        {
                            u8pOutEdid[0x80-(l_u8Offect-l_u8VdbId)+j] = u8pInEdid[0x80+j];
                        }
                    }
                    i += l_u8VdbId;
                }
                else
                #endif
                #if _SET_AUDIO_DATA_BLOCK_LIMIT_PCM20
                if(((u8pOutEdid[0x80+i]&0xE0)==0x20)&&(bOnlyPcm20 == TRUE)) // video data block
                {
                    u8 l_u8PCMData[] = {0x09,0x04,0x07};
                    u8pOutEdid[0x80+i] = ((u8pOutEdid[0x80+i]&0xE0)|0x03);
                    for(j=1;j<4;j++)
                    {
                        u8pOutEdid[0x80+i+j] = l_u8PCMData[j-1];
                    }
                    for(j=i;j<127;j++)
                    {
                        if((0x80+j+4)>255)
                        {
                            break;
                        }
                        if((0x80+j+l_u8Offect+1)<255)
                        {
                            u8pOutEdid[0x80+j+4] = u8pOutEdid[0x80+j+l_u8Offect+1];
                        }
                        else
                        {
                            u8pOutEdid[0x80+j+4] = 0;
                        }
                    }
                    u8pOutEdid[0x82] = u8pOutEdid[0x82] - (l_u8Offect-3);
                    l_u8DataBlockLen = u8pOutEdid[0x82];
                    i += 3;
                }
                else
                #endif
                #if _CLR_VSDB_HDMI_FORUM_BLOCK
                if((u8pOutEdid[0x80+i]&0xE0)==0x60)    // vsdb
                {
                    // IEEE Registration ID: 0xC45DD8
                    if((u8pOutEdid[0x80+i+1] == 0xD8)&&(u8pOutEdid[0x80+i+2] == 0x5D)&&(u8pOutEdid[0x80+i+3] == 0xC4))      //HDMI FORUM
                    {
                        for(j=i;j<127;j++)  // Delete hdmi forum block
                        {
                            if((0x80+j+l_u8Offect+1)<255)
                            {
                                u8pOutEdid[0x80+j] = u8pOutEdid[0x80+j+l_u8Offect+1];
                            }
                            else
                            {
                                u8pOutEdid[0x80+j] = 0;
                            }
                        }
                        u8pOutEdid[0x82] = u8pOutEdid[0x82] - (l_u8Offect)-1;
                        l_u8DataBlockLen = u8pOutEdid[0x82];
                        i -= 1; // return to previous index
                    }
                    else if((u8pOutEdid[0x80+i+1] == 0x03)&&(u8pOutEdid[0x80+i+2] == 0x0C)&&(u8pOutEdid[0x80+i+3] == 0x00))
                    {
                        if((u8pOutEdid[0x80+i+7] > 0x3C)) // max Clock > 300 0x3C*5 = 300
                        {
                            u8pOutEdid[0x80+i+7] = 0x3C;
                        }
                        i += l_u8Offect;
                    }
                    else
                        i += l_u8Offect;
                }
                else
                #endif
                #if 1
                if((u8pOutEdid[0x80+i]&0xE0)==0xE0)    // CTA Data
                {
                    #if _CLR_YCBCR420_EDID_BLOCK
                    if(u8pOutEdid[0x80+i+1]==0x0F)      // YCbCr 420 capability map
                    {
                        for(j=i;j<127;j++)  // Delete YCbCr 420 capability map
                        {
                            if((0x80+j+l_u8Offect+1)<255)
                            {
                                u8pOutEdid[0x80+j] = u8pOutEdid[0x80+j+l_u8Offect+1];
                            }
                            else
                            {
                                u8pOutEdid[0x80+j] = 0;
                            }
                        }
                        u8pOutEdid[0x82] = u8pOutEdid[0x82] - (l_u8Offect)-1;
                        l_u8DataBlockLen = u8pOutEdid[0x82];
                        i -= 1;
                    }
                    else if(u8pOutEdid[0x80+i+1]==0x0E)     // YCbCr 420 Video data
                    {
                        for(j=i;j<127;j++)  // Delete YCbCr 420 Video data
                        {
                            if((0x80+j+l_u8Offect+1)<255)
                            {
                                u8pOutEdid[0x80+j] = u8pOutEdid[0x80+j+l_u8Offect+1];
                            }
                            else
                            {
                                u8pOutEdid[0x80+j] = 0;
                            }
                        }
                        u8pOutEdid[0x82] = u8pOutEdid[0x82] - (l_u8Offect)-1;
                        l_u8DataBlockLen = u8pOutEdid[0x82];
                        i -= 1;
                    }
                    else
                    #endif
                    #if _CLR_HDR_BLOCK
                    if(u8pOutEdid[0x80+i+1]==0x06)     // HDR Static Metadata
                    {
                        for(j=i;j<127;j++)  // Delete HDR Static Metadata
                        {
                            if((0x80+j+l_u8Offect+1)<255)
                            {
                                u8pOutEdid[0x80+j] = u8pOutEdid[0x80+j+l_u8Offect+1];
                            }
                            else
                            {
                                u8pOutEdid[0x80+j] = 0;
                            }
                        }
                        u8pOutEdid[0x82] = u8pOutEdid[0x82] - (l_u8Offect)-1;
                        l_u8DataBlockLen = u8pOutEdid[0x82];
                        i -= 1;
                    }
                    else
                    #endif //add by wf8421
                    #if _SET_DOLBY_VISION_LIMIT_4K30
                    if(u8pOutEdid[0x80+i+1]==0x01)  // dolby vision
                    {
                        // IEEE Registration ID: 0x00D046
                        if((u8pOutEdid[0x80+i+2] == 0x46)&&(u8pOutEdid[0x80+i+3] == 0xD0)&&(u8pOutEdid[0x80+i+4] == 0x00))
                        {
                            if(l_u8Offect>6)
                            {
                                if(((u8pOutEdid[0x80+i+5]&BIT6)==0)&&(u8pOutEdid[0x80+i+5]&BIT1))
                                {
                                    u8pOutEdid[0x80+i+5] = u8pOutEdid[0x80+i+5]&(~BIT1);    // Remove support 2160p 60Hz
                                }
                                else
                                    i += l_u8Offect;
                            }
                            else
                                i += l_u8Offect;
                        }
                        else
                            i += l_u8Offect;
                    }
                    else
                    #endif
                    #if _CLR_DOLBY_VISION_BLOCK
                    if(u8pOutEdid[0x80+i+1]==0x01)  // dolby vision
                    {
                        // IEEE Registration ID: 0x00D046
                        if((u8pOutEdid[0x80+i+2] == 0x46)&&(u8pOutEdid[0x80+i+3] == 0xD0)&&(u8pOutEdid[0x80+i+4] == 0x00))
                        {
                            for(j=i;j<127;j++)  // Delete dolby vision block
                            {
                                if((0x80+j+l_u8Offect+1)<255)
                                {
                                    u8pOutEdid[0x80+j] = u8pOutEdid[0x80+j+l_u8Offect+1];
                                }
                                else
                                {
                                    u8pOutEdid[0x80+j] = 0;
                                }
                            }
                            u8pOutEdid[0x82] = u8pOutEdid[0x82] - (l_u8Offect)-1;
                            l_u8DataBlockLen = u8pOutEdid[0x82];
                            i -= 1;
                        }
                        else
                            i += l_u8Offect;
                    }
                    else
                    #endif
                        i += l_u8Offect;
                }
                else
                #endif
                {
                    i += l_u8Offect;
                }
            }
        }
        l_u8CheckSum = 0;
        for(i=0;i<127;i++)
        {
            l_u8CheckSum += u8pOutEdid[i+0x80];
        }
        u8pOutEdid[0x7F+0x80] = 0x100-l_u8CheckSum;
    }

    return TRUE;
}

/***************************************************
**Function: mapp_EdidLimitTo300M_Block0
**Input:    u8pInEdid(Input Edid data),u16len(Edid length),
**Output:   u8pOutEdid(Output Edid data)
**Return:   Edid Status(Is Edid Ok?)
**Description:  Limited Edid to 4k30 when Edid up to 4K60
***************************************************/
bool mapp_EdidLimitTo300M_Block0(u8 *u8pInEdid,
                                    u8 *u8pOutEdid,
                                    u16 u16len,
                                    bool bIsLimitTiming)
{
    u8 i;
    u8 l_u8CheckSum=0;
    uint8_t const l_u8DefaultEdidTiming[18] = //4k@30, 297M
    {
        0x04,0x74,0x00,0x30,0xF2,0x70,0x5A,0x80,0xB0,
        0x58,0x8A,0x00,0x20,0xC2,0x31,0x00,0x00,0x1E,
    };

    if(is_edid_valid(u8pInEdid) == FALSE)
    {
        return FALSE;
    }
    memcpy(u8pOutEdid,u8pInEdid,u16len);

    // block 0
    #if _CLR_SAMSUN_EDID_SERIAL_NUM
    if(u8pOutEdid[0x08]==0x4C&&u8pOutEdid[0x09]==0x2D)  // if ID is SAM, Change the ID serial number 00000000
    {
        u8pOutEdid[0x0C]=0x00;
        u8pOutEdid[0x0D]=0x00;
        u8pOutEdid[0x0E]=0x00;
        u8pOutEdid[0x0F]=0x00;
    }
    #endif

    if(bIsLimitTiming)
    {
        if((((((u8pOutEdid[0x3A]>>4)<<8)|(u8pOutEdid[0x38]))==3840)
            ||((((u8pOutEdid[0x3A]>>4)<<8)|(u8pOutEdid[0x38]))==4096))
            &&((((u8pOutEdid[0x3D]>>4)<<8)|(u8pOutEdid[0x3B]))==2160))
        {
            if(((u8pOutEdid[0x37]<<8)|(u8pOutEdid[0x36]))>30000)
            {
                for(i=0;i<18;i++)
                {
                    u8pOutEdid[0x36+i] = l_u8DefaultEdidTiming[i];
                }
            }
        }
    }
    #if _SET_RANGE_LIMITS_BLOCK
    for(i=0x49;i<127;i++)
    {
        if((u8pOutEdid[i]==0x00)&&(u8pOutEdid[i+1]==0x00)&&(u8pOutEdid[i+2]==0x00)&&(u8pOutEdid[i+3]==0xFD))
        {
            if(u8pOutEdid[i+5]>0x17)
            {
                u8pOutEdid[i+5] = 0x17;
            }
            //if(u8pOutEdid[i+6]>0x55)
            //{
            //    u8pOutEdid[i+6] = 0x55;
            //}
            if(u8pOutEdid[i+7]>0x0F)
            {
                u8pOutEdid[i+7] = 0x0F;
            }
            //if(u8pOutEdid[i+8]>0x5C)
            //{
            //    u8pOutEdid[i+8] = 0x5C;
            //}
            if(u8pOutEdid[i+9]>0x1E)
            {
                u8pOutEdid[i+9] = 0x1E;
            }
        }
    }
    #endif

    for(i=0;i<127;i++)
    {
        l_u8CheckSum += u8pOutEdid[i];
    }
    u8pOutEdid[0x7F] = 0x100-l_u8CheckSum;

    return TRUE;
}


/***************************************************
**Function: mapp_EdidLimitTo300M_Block1
**Input:    u8pInEdid(Input Edid data),u16len(Edid length),
            bEnInterlace(Is Interlace Timing use?)
            bOnlyPcm20(Is only pcm2.0?)
**Output:   u8pOutEdid(Output Edid data)
**Return:   Edid Status(Is Edid Ok?)
**Description:  Limited Edid to 4k30 when Edid up to 4K60
***************************************************/
bool mapp_EdidLimitTo300M_Block1(u8 *u8pInEdid,
                                    u8 *u8pOutEdid,
                                    u16 u16len,
                                    bool bRemoveVDB6G,
                                    bool bEnInterlace,
                                    bool bOnlyPcm20,
                                    bool bRemove420)
{
    u8 i;
    u8 l_u8CheckSum=0;
    memcpy(u8pOutEdid,u8pInEdid,u16len);

    // block 1
    {
        if((u8pOutEdid[0] == 0x02) && (u8pOutEdid[1] == 0x03))
        {
            u8 l_u8DataBlockLen = u8pOutEdid[2];
            u8 j=0;
            u8 l_u8Offect = 0;
            #if _CLR_VIDEO_DATA_BLOCK_6G
            u8 l_pu8Vdb[31]={0};
            u8 l_u8VdbId = 0;
            #endif

            for(i=4;i<l_u8DataBlockLen;i++)
            {
                l_u8Offect = u8pOutEdid[0+i]&0x1F;

                #if _CLR_VIDEO_DATA_BLOCK_6G
                if(((u8pOutEdid[0+i]&0xE0)==0x40)&&(bRemoveVDB6G == TRUE)) // video data block
                {
                    for(j=0;j<l_u8Offect;j++)
                    {
                        if(bEnInterlace&&((((u8pOutEdid[0+i+1+j]&0x7F)>=96)&&((u8pOutEdid[0+i+1+j]&0x7F)<=97))
                            ||(((u8pOutEdid[0+i+1+j]&0x7F)>=101)&&((u8pOutEdid[0+i+1+j]&0x7F)<=102))
                            ||((u8pOutEdid[0+i+1+j]&0x7F)>=106)))
                            continue;

                        if((bEnInterlace==FALSE)&&(((u8pOutEdid[0+i+1+j]>=96)&&(u8pOutEdid[0+i+1+j]<=97))
                            ||((u8pOutEdid[0+i+1+j]>=101)&&(u8pOutEdid[0+i+1+j]<=102))
                            ||(u8pOutEdid[0+i+1+j]>=106)))  //  Delete timing up to 4K60
                        {
                            continue;
                        }
                        else
                        {
                            l_pu8Vdb[l_u8VdbId++] = u8pOutEdid[0+i+1+j];
                        }
                    }
                    if(l_u8VdbId != l_u8Offect)
                    {
                        u8pOutEdid[0+i] = (0x40|l_u8VdbId);
                        for(j=0;j<l_u8VdbId;j++)
                        {
                            u8pOutEdid[0+i+1+j] = l_pu8Vdb[j];
                        }
                        u8pOutEdid[2] = u8pOutEdid[2] - (l_u8Offect-l_u8VdbId);
                        l_u8DataBlockLen = u8pOutEdid[2];
                        for(j=(i+1+l_u8Offect);j<127;j++)
                        {
                            u8pOutEdid[0+j-(l_u8Offect-l_u8VdbId)] = u8pInEdid[0+j];
                        }
                    }
                    i += l_u8VdbId;
                }
                else
                #endif
                #if _SET_AUDIO_DATA_BLOCK_LIMIT_PCM20
                if(((u8pOutEdid[0+i]&0xE0)==0x20)&&(bOnlyPcm20 == TRUE)) // video data block
                {
                    u8 l_u8PCMData[] = {0x09,0x04,0x07};
                    u8pOutEdid[0+i] = ((u8pOutEdid[0+i]&0xE0)|0x03);
                    for(j=1;j<4;j++)
                    {
                        u8pOutEdid[0+i+j] = l_u8PCMData[j-1];
                    }
                    for(j=i;j<127;j++)
                    {
                        if((0+j+4)>127)
                        {
                            break;
                        }
                        if((0+j+l_u8Offect+1)<127)
                        {
                            u8pOutEdid[0+j+4] = u8pOutEdid[0+j+l_u8Offect+1];
                        }
                        else
                        {
                            u8pOutEdid[0+j+4] = 0;
                        }
                    }
                    u8pOutEdid[2] = u8pOutEdid[2] - (l_u8Offect-3);
                    l_u8DataBlockLen = u8pOutEdid[2];
                    i += 3;
                }
                else
                #endif
                #if _CLR_VSDB_HDMI_FORUM_BLOCK
                if((u8pOutEdid[0+i]&0xE0)==0x60)    // vsdb
                {
                    if((u8pOutEdid[0+i+1] == 0xD8)&&(u8pOutEdid[0+i+2] == 0x5D)&&(u8pOutEdid[0+i+3] == 0xC4))      //HDMI FORUM
                    {
                        for(j=i;j<127;j++)  // Delete hdmi forum block
                        {
                            if((0+j+l_u8Offect+1)<127)
                            {
                                u8pOutEdid[0+j] = u8pOutEdid[0+j+l_u8Offect+1];
                            }
                            else
                            {
                                u8pOutEdid[0+j] = 0;
                            }
                        }
                        u8pOutEdid[2] = u8pOutEdid[2] - (l_u8Offect)-1;
                        l_u8DataBlockLen = u8pOutEdid[2];
                        i -= 1; // return to previous index
                    }
                    else if((u8pOutEdid[0+i+1] == 0x03)&&(u8pOutEdid[0+i+2] == 0x0C)&&(u8pOutEdid[0+i+3] == 0x00))
                    {
                        if((u8pOutEdid[0+i+7] > 0x3C)) // max Clock > 300 0x3C*5 = 300
                        {
                            u8pOutEdid[0+i+7] = 0x3C;
                        }
                        i += l_u8Offect;
                    }
                    else
                        i += l_u8Offect;
                }
                else
                #endif
                if((u8pOutEdid[0+i]&0xE0)==0xE0)    // CTA Data
                {
                    #if _CLR_YCBCR420_EDID_BLOCK
                    if((u8pOutEdid[0+i+1]==0x0F)&&(bRemove420==TRUE))      // YCbCr 420 capability map
                    {
                        for(j=i;j<127;j++)  // Delete YCbCr 420 capability map block
                        {
                            if((0+j+l_u8Offect+1)<127)
                            {
                                u8pOutEdid[0+j] = u8pOutEdid[0+j+l_u8Offect+1];
                            }
                            else
                            {
                                u8pOutEdid[0+j] = 0;
                            }
                        }
                        u8pOutEdid[2] = u8pOutEdid[2] - (l_u8Offect)-1;
                        l_u8DataBlockLen = u8pOutEdid[2];
                        i -= 1;
                    }
                    else if((u8pOutEdid[0+i+1]==0x0E)&&(bRemove420==TRUE))     // YCbCr 420 Video data
                    {
                        for(j=i;j<127;j++)  // Delete YCbCr 420 Video data block
                        {
                            if((0+j+l_u8Offect+1)<127)
                            {
                                u8pOutEdid[0+j] = u8pOutEdid[0+j+l_u8Offect+1];
                            }
                            else
                            {
                                u8pOutEdid[0+j] = 0;
                            }
                        }
                        u8pOutEdid[2] = u8pOutEdid[2] - (l_u8Offect)-1;
                        l_u8DataBlockLen = u8pOutEdid[2];
                        i -= 1;
                    }
                    else
                    #endif
                    #if _CLR_HDR_BLOCK
                    if(u8pOutEdid[0+i+1]==0x06)     // HDR Static Metadata
                    {
                        for(j=i;j<127;j++)  // Delete HDR Static Metadata
                        {
                            if((0x0+j+l_u8Offect+1)<255)
                            {
                                u8pOutEdid[0x0+j] = u8pOutEdid[0x0+j+l_u8Offect+1];
                            }
                            else
                            {
                                u8pOutEdid[0x0+j] = 0;
                            }
                        }
                        u8pOutEdid[0x2] = u8pOutEdid[0x2] - (l_u8Offect)-1;
                        l_u8DataBlockLen = u8pOutEdid[0x2];
                        i -= 1;
                    }
                    else
                    #endif //add by wf8421
                    #if _SET_DOLBY_VISION_LIMIT_4K30
                    if(u8pOutEdid[0+i+1]==0x01)  // dolby vision
                    {
                        // IEEE = 0x00D046
                        if((u8pOutEdid[0+i+2] == 0x46)&&(u8pOutEdid[0+i+3] == 0xD0)&&(u8pOutEdid[0+i+4] == 0x00))
                        {
                            if(l_u8Offect>6)
                            {
                                if(((u8pOutEdid[0+i+5]&BIT6)==0)&&(u8pOutEdid[0+i+5]&BIT1))
                                {
                                    u8pOutEdid[0+i+5] = u8pOutEdid[0+i+5]&(~BIT1);    // Remove support 2160p 60Hz
                                }
                                else
                                    i += l_u8Offect;
                            }
                            else
                                i += l_u8Offect;
                        }
                        else
                            i += l_u8Offect;
                    }
                    else
                    #endif
                    #if _CLR_DOLBY_VISION_BLOCK
                    if(u8pOutEdid[0+i+1]==0x01)  // dolby vision
                    {
                        if((u8pOutEdid[0+i+2] == 0x46)&&(u8pOutEdid[0+i+3] == 0xD0)&&(u8pOutEdid[0+i+4] == 0x00))
                        {
                            for(j=i;j<127;j++)  // Delete dolby vision block
                            {
                                if((0+j+l_u8Offect+1)<127)
                                {
                                    u8pOutEdid[0+j] = u8pOutEdid[0+j+l_u8Offect+1];
                                }
                                else
                                {
                                    u8pOutEdid[0+j] = 0;
                                }
                            }
                            u8pOutEdid[2] = u8pOutEdid[2] - (l_u8Offect)-1;
                            l_u8DataBlockLen = u8pOutEdid[2];
                            i -= 1;
                        }
                        else
                            i += l_u8Offect;
                    }
                    else
                    #endif
                    {
                        i += l_u8Offect;
                    }
                }
                else
                {
                    i += l_u8Offect;
                }
            }
        }
        l_u8CheckSum = 0;
        for(i=0;i<127;i++)
        {
            l_u8CheckSum += u8pOutEdid[i+0];
        }
        u8pOutEdid[0x7F+0] = 0x100-l_u8CheckSum;
    }

    return TRUE;
}

/***************************************************
**Function: mapp_EdidLimitTo1080p60
**Input:    u8pInEdid(Input Edid data),u16len(Edid length),
            bEnInterlace(Is Interlace Timing use?)
            bOnlyPcm20(Is only pcm2.0?)
**Output:   u8pOutEdid(Output Edid data)
**Return:   Edid Status(Is Edid Ok?)
**Description:  Limited Edid to 4k30 when Edid up to 4K60
***************************************************/
bool mapp_EdidLimitTo1080p60(u8 *u8pInEdid,
                                    u8 *u8pOutEdid,
                                    u16 u16len)
{
    u8 i;
    u8 l_u8CheckSum=0;
    uint8_t const l_u8DefaultEdidTiming[18] = //4k@30, 297M
    {
        0x02, 0x3A, 0x80, 0x18, 0x71, 0x38, 0x2D, 0x40, 0x58,
        0x2C, 0x45, 0x00, 0x20, 0xC2, 0x31, 0x00, 0x00, 0x1E
    };

    if(is_edid_valid(u8pInEdid) == FALSE)
    {
        return false;
    }
    memcpy(u8pOutEdid,u8pInEdid,u16len);

    // block 0
    #if _CLR_SAMSUN_EDID_SERIAL_NUM
    if(u8pOutEdid[0x08]==0x4C&&u8pOutEdid[0x09]==0x2D)  // if ID is SAM, Change the ID serial number 00000000
    {
        u8pOutEdid[0x0C]=0x00;
        u8pOutEdid[0x0D]=0x00;
        u8pOutEdid[0x0E]=0x00;
        u8pOutEdid[0x0F]=0x00;
    }
    #endif

//    if((((((u8pOutEdid[0x3A]>>4)<<8)|(u8pOutEdid[0x38]))==3840)
//        ||((((u8pOutEdid[0x3A]>>4)<<8)|(u8pOutEdid[0x38]))==4096))
//        &&((((u8pOutEdid[0x3D]>>4)<<8)|(u8pOutEdid[0x3B]))==2160))
    {
        if(((u8pOutEdid[0x37]<<8)|(u8pOutEdid[0x36]))>16500)
        {
            for(i=0;i<18;i++)
            {
                u8pOutEdid[0x36+i] = l_u8DefaultEdidTiming[i];
            }
        }
    }

    #if 0
    for(i=0x49;i<127;i++)
    {
        if((u8pOutEdid[i]==0x00)&&(u8pOutEdid[i+1]==0x00)&&(u8pOutEdid[i+2]==0x00)&&(u8pOutEdid[i+3]==0xFD))
        {
            if(u8pOutEdid[i+9]>0x1E)
            {
                u8pOutEdid[i+9] = 0x1E;
            }
        }
    }
    #endif

#if _SET_RANGE_LIMITS_BLOCK
    for(i=0x49;i<127;i++)
    {
        if((u8pOutEdid[i]==0x00)&&(u8pOutEdid[i+1]==0x00)&&(u8pOutEdid[i+2]==0x00)&&(u8pOutEdid[i+3]==0xFD))
        {
            if(u8pOutEdid[i+5]>0x17)
            {
                u8pOutEdid[i+5] = 0x17;
            }
            //if(u8pOutEdid[i+6]>0x55)
            //{
            //    u8pOutEdid[i+6] = 0x55;
            //}
            if(u8pOutEdid[i+7]>0x0F)
            {
                u8pOutEdid[i+7] = 0x0F;
            }
            //if(u8pOutEdid[i+8]>0x5C)
            //{
            //    u8pOutEdid[i+8] = 0x5C;
            //}
            if(u8pOutEdid[i+9]>0x1E)
            {
                u8pOutEdid[i+9] = 0x1E;
            }
        }
    }
#endif

    for(i=0;i<127;i++)
    {
        l_u8CheckSum += u8pOutEdid[i];
    }
    u8pOutEdid[0x7F] = 0x100-l_u8CheckSum;

    // block 1
    if((u8pOutEdid[0x7E]>0)&&(u16len>128))
    {
        const u8 cl_u8EdidCeaBlockDefault[128]={
                0x02, 0x03, 0x23, 0x72, 0x4B, 0x90, 0x9F, 0x22, 0x21, 0x20, 0x05, 0x14, 0x04, 0x13, 0x12, 0x03,
                0x23, 0x09, 0x1F, 0x07, 0x83, 0x01, 0x00, 0x00, 0x67, 0x03, 0x0C, 0x00, 0x10, 0x00, 0x00, 0x20,
                0xE2, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xD7,
            };
        memcpy(&u8pOutEdid[0x80],cl_u8EdidCeaBlockDefault,128);
        l_u8CheckSum = 0;
        for(i=0;i<127;i++)
        {
            l_u8CheckSum += u8pOutEdid[i+0x80];
        }
        u8pOutEdid[0x7F+0x80] = 0x100-l_u8CheckSum;
    }

    return true;
}

#endif

u16 mapp_GetEdidPhysicalAddress(u8 *pu8EdidBuf)
{
    u8 i;
    u16 l_u16PhyAddr = 0;

    if(pu8EdidBuf[0x7E] != 0)
    {
        for(i=0; i<128;i++)
        {
            if((pu8EdidBuf[0x80+i]&0xE0)==0x60)
            {
                if((pu8EdidBuf[0x80+i+1]==0x03)
                    &&(pu8EdidBuf[0x80+i+2]==0x0C)
                    &&(pu8EdidBuf[0x80+i+3]==0x00))
                {
                    l_u16PhyAddr = (pu8EdidBuf[0x80+i+4]<<8)|(pu8EdidBuf[0x80+i+5]);
                    break;
                }
            }
        }
    }
    return l_u16PhyAddr;
}

bool mapp_CopyEdidPhysicalAddress(u8 *pu8InEdidBuf,u8 *pu8OutEdidBuf)
{
    u8 i,j;
    u16 l_u16PhyAddr = 0;
    bool l_bRetSta = FALSE;
    u8 l_u8CheckSum = 0;

    if((pu8InEdidBuf[0x7E] != 0)&&(pu8OutEdidBuf[0x7E] != 0))
    {
        for(i=0; i<128;i++)
        {
            if((pu8InEdidBuf[0x80+i]&0xE0)==0x60)
            {
                if((pu8InEdidBuf[0x80+i+1]==0x03)
                    &&(pu8InEdidBuf[0x80+i+2]==0x0C)
                    &&(pu8InEdidBuf[0x80+i+3]==0x00))
                {
                    l_u16PhyAddr = (pu8InEdidBuf[0x80+i+4]<<8)|(pu8InEdidBuf[0x80+i+5]);
                    break;
                }
            }
        }

        for(i=0; i<128;i++)
        {
            if((pu8OutEdidBuf[0x80+i]&0xE0)==0x60)
            {
                if((pu8OutEdidBuf[0x80+i+1]==0x03)
                    &&(pu8OutEdidBuf[0x80+i+2]==0x0C)
                    &&(pu8OutEdidBuf[0x80+i+3]==0x00))
                {
                    //l_u16PhyAddr = (pu8InEdidBuf[0x80+i+4]<<8)|(pu8InEdidBuf[0x80+i+5]);
                    pu8OutEdidBuf[0x80+i+4] = (u8)((l_u16PhyAddr>>8)&0xFF);
                    pu8OutEdidBuf[0x80+i+5] = (u8)(l_u16PhyAddr&0xFF);

                    for(j=0;j<127;j++)
                    {
                        l_u8CheckSum += pu8OutEdidBuf[0x80+j];
                    }
                    pu8OutEdidBuf[0x80+0x7F] = 0x100-l_u8CheckSum;
                    l_bRetSta = TRUE;
                    break;
                }
            }
        }
    }

    return l_bRetSta;
}

//------------------------------------------------------------------------------
// Function:    HexAndAsciiChange
// Description: Hex 和 Ascii之间的转换
// cmode : 1 Hex to Ascii    0: Ascii to Hex
//------------------------------------------------------------------------------
u8 HexAndAsciiChange(u8 cvalue,u8 cmode)
{
    u8 reValue=0;

    if(cmode == 1)
    {
        if((cvalue&0x0F)>=0&&(cvalue&0x0F)<=9)
        {
            reValue=cvalue+'0';
        }
        else if((cvalue&0x0F)>=0x0A&&(cvalue&0x0F)<=0x0F)
        {
            reValue=cvalue-0x0A+'A';
        }
        else
        {
            reValue=cvalue;
            //printf("Change error: %x\n",cvalue);
        }
    }
    else
    {
        if(((cvalue)>='0'&&(cvalue)<='9'))
        {
            reValue=cvalue-'0';
        }
        else if((cvalue)>='A'&&(cvalue)<='F')
        {
            reValue=cvalue+0x0A-'A';
        }
        else if((cvalue)>='a'&&(cvalue)<='f')
        {
            reValue=cvalue+0x0A-'a';
        }
        else
        {
            reValue=cvalue;
            //printf("Change error: %x\n",cvalue);
        }
    }

    return reValue;
}

//========================================================================================
//------------------------------------------------------------------------------
// Function:    isNumberCmd
// Description: 判断数据是否为数字
//------------------------------------------------------------------------------
bool isNumberCmd(char *cmd, uint16_t len)
{
    uint16_t i;

    for(i=0; i<len; i++)
    {
        if((cmd[i]<'0')||(cmd[i]>'9'))
        {
            return FALSE;
        }
    }

    return TRUE;
}

//========================================================================================
// add by wf8421 20180426
//------------------------------------------------------------------------------
// Function:    PlatformTimerSet
// Description: 设置相关功能定时器时间
//------------------------------------------------------------------------------
void PlatformTimerSet( uint8_t index, u32 m_sec )
{
    switch ( index )
    {
        default:
            if (index<etINVALID)
            {
                gTimerCounters[index] = m_sec;
            }
            break;
    }
}

//------------------------------------------------------------------------------
// Function:    PlatformTimerSet
// Description: 获取相关功能定时器时间
//------------------------------------------------------------------------------
u32 PlatformTimerGet(uint8_t eTimer)
{
    return(gTimerCounters[eTimer]);
}

//------------------------------------------------------------------------------
// Function:    PlatformTimerSet
// Description: 定时器初始化
//------------------------------------------------------------------------------
void PlatformTimerInit (void)
{
    int i;
    //initializer timer counters in array
    for ( i = 0; i < etINVALID; i++ )
    {
        gTimerCounters[ i ] = 0;
    }

    //PlatformTimerSet(etKEY_Detect,5000);
}

//------------------------------------------------------------------------------
// Function:    PlatformTimerSet
// Description: 定时器递减函数
//------------------------------------------------------------------------------
void PlatformTimerTask(void)
{
    int i;
    for(i=0;i<etINVALID;i++)
    {
        if(gTimerCounters[i] > 0)
        {
            gTimerCounters[i]--;
        }
    }
}

//------------------------------------------------------------------------------
// Function:    PlatformTimerSet
// Description: 相关功能定时器时间是否为0
//------------------------------------------------------------------------------
uint8_t PlatformTimerExpired ( uint8_t timer )         //定时时间到函数
{
    if ( timer < etINVALID )
    {
        return( gTimerCounters[ timer ] == 0);             //定时时间到时返回1
    }

    return( 0 );                                           //定时时间还没到时返回0
}
//========================================================================================

#if 0
char *my_StrStr(char *inBuffer,char *inSerchStr)
{
    char *pCurrBuffer = inBuffer;
    while(*pCurrBuffer != '\0')
    {
        char *pCompareOne = pCurrBuffer;
        char *pCompareTow = inSerchStr;
        //while(tolower(*pCompareOne) == tolower(*pCompareTow))
        while((*pCompareOne) == (*pCompareTow))
        {
            pCompareOne++;
            pCompareTow++;
            if(*pCompareTow == '\0')
            {
                return (char*)pCompareOne;
            }
        }
        pCurrBuffer++;
    }
    return NULL;
}
#else
char *my_StrStr(char *inBuffer,char *inSerchStr)//参照strstr函数原型构建自己的strstr函数
{
	char*s1 = NULL;//创建一个空指针；
	char*s2 = NULL;
	char*cur = inBuffer;
	if (*inSerchStr == '\0')
	{
		return (char*)inBuffer;                    //如果我们所要找的子串inSerchStr为'\0',返回总串的首元素地址
	}

	while (*cur)//cur代表指向的是子串第一次出现的首元素地址
	{
		s1 = cur;                      //把cur指向字符串的首元素地址赋给s1
		s2 = (char*)inSerchStr;                    //把inSerchStr指向的首元素地址赋给s2，每一次循环都要从inSerchStr的首元素地址开始重新比较
		while ((*s1 != 0) && (*s2 != 0) && *s1 == *s2)           //只有在s1指向的首元素地址，s2指向的首元素地址都不为'\0'的时候，才可以在总串中找子串的首地址第一次出现的时候
		{                                            // 比较s1，s2指向的地址所对应的字符是否相等，若相等，各自指向的地址向后跳一个字节比较下一位是否相等在满足以上的条件下不断循环，若不满足，
			s1++;                                    //开始一下比较
			s2++;
		}
		if (*s2 == '\0')             //若经过上面的比较，s2指向的内容恰好是'\0',代表找到子串首次出现的地址
		{                              //若不满足以上情况的比较，开始以下比较
			return (char*)cur;
		}
		if (*s1 == '\0')
		{
			return NULL;            //若在s2指向的内容不为'\0'时，s1指向的内容率先为0，代表总串长度比子串长度短，必不存在子串，返回一个空指针
		}
		cur++;//代表当前cur指向的并不是子串第一次出现的首元素地址，指向的地址要往后跳一个字节
	}

	return NULL;//如果经过以上比较都不能得出结果，代表找不到子串，要返回空指针
}
#endif

char *my_CaseStrStr(char *inBuffer,char *inSerchStr)
{
    char *pCurrBuffer = inBuffer;
    while(*pCurrBuffer != '\0')
    {
        char *pCompareOne = pCurrBuffer;
        char *pCompareTow = inSerchStr;
        while(tolower(*pCompareOne) == tolower(*pCompareTow))
        {
            pCompareOne++;
            pCompareTow++;
            if(*pCompareTow == '\0')
            {
                return (char*)pCompareOne;
            }
        }
        pCurrBuffer++;
    }
    return NULL;
}

char *FindChar(char *buf,char Taildata,uint16_t Bitwide)
{
    //char MemberArr[4];
//  unsigned char Arrcur=0;
    unsigned char bitcnt=0;

    while(*buf!=Taildata)
    {
        buf++;
        bitcnt++;
        if(bitcnt>Bitwide)
            return 0;
    }
    return buf;
}

/*输入字符buf转成数字 返回数字*/
unsigned short StrToNum(char *Str,unsigned char cursor)
{
    unsigned char i;
    unsigned short Num=0;
    for(i=0;i<10;i++)
    {
        if(i<cursor)
        {
            //Num+=(Str[i]-0x30)*pow(10,cursor-1-i);
            Num=Num*10+(Str[i]-0x30);
        }
        else
            Str[i]=' ';
    }
    return Num;
}

/*数字转换成字符buf 返回buf的字符个数*/
unsigned short NumToStr(char* buf,char* Str,unsigned char member)
{
    unsigned char tempB=0,tempS=0,tempG=0;
    unsigned short i;
    unsigned short strcursor=0;
    for(i=0;i<member;i++)
    {
        tempB=buf[i]/100;
        if(tempB)
        {
            Str[strcursor++]=tempB+0x30;
        }
        tempS=buf[i]%100/10;
        if(tempS||tempB)
        {
            Str[strcursor++]=tempS+0x30;
        }
        tempG=buf[i]%10;
        if(tempS||tempB||tempG)
        {
            Str[strcursor++]=tempG+0x30;
            if(i+1<member)
                Str[strcursor++]=',';
        }
    }
    return strcursor;
}

/*在字符串中寻找数字 并返回数字 未找到返回-1*/
short GetNumofStr(char *buf,char Taildata,unsigned char Bitwide)
{
    char MemberArr[10];
    unsigned char Arrcur=0;
    unsigned char bitcnt=0;

    while(*buf!=Taildata)
    {
        if(*buf>='0'&&*buf<='9')
        {
            MemberArr[Arrcur]=*buf;
            Arrcur++;
        }
        else
            return -1;
        buf++;
        bitcnt++;
        if(bitcnt>Bitwide)
            return -1;
    }
    return StrToNum(MemberArr,bitcnt);
}

/*在字符串中寻找数字 并返回数字字符个数 未找到返回0*/
unsigned char GetLenOfStr(char *buf,char Taildata,unsigned char Bitwide)
{
    //char MemberArr[4];
    unsigned char Arrcur=0;
    unsigned char bitcnt=0;

    while(*buf!=Taildata)
    {
        if(*buf>='0'&&*buf<='9')
        {
            //MemberArr[Arrcur]=*buf;
            Arrcur++;
        }
        else
            return 0;
        buf++;
        bitcnt++;
        if(bitcnt>Bitwide)
            return 0;
    }
    return bitcnt;
}

//  mode = 1   high->low
//  mode = 0   low->high
void bubble_sort(u8* a, u8 n,u8 mode)
{
    u8 i, j, temp;
    for (j = 0; j < n - 1; j++)
        for (i = 0; i < n - 1 - j; i++)
        {
            if(!mode)
            {
                if(a[i] > a[i + 1])
                {
                    temp = a[i];
                    a[i] = a[i + 1];
                    a[i + 1] = temp;
                }
            }
            else
            {
                if(a[i] < a[i + 1])
                {
                    temp = a[i];
                    a[i] = a[i + 1];
                    a[i + 1] = temp;
                }
            }
        }
}

void mapp_RevisionMake(void)
{
    u16 l_u16Year;
    u8  l_u8Month;
    u8  l_u8Date;
    u8  l_u8Hour;
    u8  l_u8minute;
    u8  l_u8Second;

    switch(DataStr[2])      //月份字符转成日期数字，先判断第三个字幕，再判断第二个字幕，获取月份
    {
        case 'n':   // Jan / Jun
            {
                l_u8Month = DataStr[1] == 'a' ? 1 : 6;
            }
            break;
        case 'b':   // Feb
            {
                l_u8Month = 2;
            }
            break;
        case 'r':   // Mar / Apr
            {
                l_u8Month = DataStr[1] == 'a' ? 3 : 4;
            }
            break;
        case 'y':   // May
            {
                l_u8Month = 5;
            }
            break;
        case 'l':   // Jul
            {
                l_u8Month = 7;
            }
            break;
        case 'g':   // Aug
            {
                l_u8Month = 8;
            }
            break;
        case 'p':   // Sep
            {
                l_u8Month = 9;
            }
            break;
        case 't':   // Oct
            {
                l_u8Month = 10;
            }
            break;
        case 'v':   // Nov
            {
                l_u8Month = 11;
            }
            break;
        case 'c':   // Dec
            {
                l_u8Month = 12;
            }
            break;
    }

    l_u16Year = HexAndAsciiChange(DataStr[7],0)*1000
                +HexAndAsciiChange(DataStr[8],0)*100
                +HexAndAsciiChange(DataStr[9],0)*10
                +HexAndAsciiChange(DataStr[10],0);
    l_u8Date = ((DataStr[4]!=' ')?(HexAndAsciiChange(DataStr[4],0)*10):0)
                +HexAndAsciiChange(DataStr[5],0);

    l_u8Hour = HexAndAsciiChange(TimeStr[0],0)*10
                +HexAndAsciiChange(TimeStr[1],0);
    l_u8minute = HexAndAsciiChange(TimeStr[3],0)*10
                +HexAndAsciiChange(TimeStr[4],0);
    l_u8Second = HexAndAsciiChange(TimeStr[6],0)*10
                +HexAndAsciiChange(TimeStr[7],0);

    g_stTestVersion.u16Year  = l_u16Year;
    g_stTestVersion.u8Month  = l_u8Month;
    g_stTestVersion.u8Date   = l_u8Date;
    g_stTestVersion.u8Hour   = l_u8Hour;
    g_stTestVersion.u8minute = l_u8minute;
    g_stTestVersion.u8Second = l_u8Second;
}

bool buffer_compare(pu8 arry1,pu8 arry2,u16 u16Len)
{
	u16 i;
	bool ret=TRUE;

	for(i=0;i<u16Len;i++)
	{
		if(*(arry1+i) != *(arry2+i))
		{
			ret = FALSE;
			break;
		}
	}
	return ret;
}

u16 U16ToU8MsbCovert(pu16 arry1,pu8 arry2,u16 u16Len)
{
	u16 i;
	u16 l_u16RealLen=0;

	for(i=0;i<u16Len;i++)
	{
        arry2[l_u16RealLen++] = (arry1[i]>>8)&0xFF;
        arry2[l_u16RealLen++] = (arry1[i])&0xFF;
	}
	return l_u16RealLen;
}

u16 U8ToU16MsbCovert(pu8 arry1,pu16 arry2,u16 u16Len)
{
	u16 i;
	u16 l_u16RealLen=0;

	for(i=0;i<u16Len;i=i+2)
	{
        arry2[l_u16RealLen] = arry1[i];
        arry2[l_u16RealLen] = (arry2[l_u16RealLen]<<8) | arry1[i+1];
        l_u16RealLen++;
	}
	return l_u16RealLen;
}





