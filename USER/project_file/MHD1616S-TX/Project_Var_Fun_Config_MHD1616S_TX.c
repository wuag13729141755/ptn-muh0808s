#define __PROJECT_VAR_FUN_CONFIG_C__

#include "includes.h"



stProjectSpecVar_t  stMHD1616STXSpecVar;

stWinConfigVar_t    g_stWinConfigVar[Def_outport_num];

static bool         sg_bChangePowerStateFlag = FALSE;
static volatile u32 g_MHD1616STX_Timers[emUserTimer_Max];
u8 g_u8BdAddr = 0x81;
stResolution_t g_stRxResTable[_HDMI_INPUT_PORT_USED_NUMBER];


extern stProjectGlobalAllVar_t g_stPrjGloVar;
extern key_val_stored_t s_tValStd;
//void SetMstarVgaResCmd(uint8_t u8ResIndex);
void mapp_SourceChangeToSetWinProcess(void);
void mapp_MHD1616STXIDT5VInit(void);

const uint8_t edid_MHD1616STX_default[256] = // default edid
{
    0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,
    0x55,0x04,0x00,0x00,0x00,0x00,0x00,0x00,
    0x32,0x1C,0x01,0x03,0x80,0x73,0x41,0x78,
    0x0A,0xCF,0x74,0xA3,0x57,0x4C,0xB0,0x23,
    0x09,0x48,0x4C,0x01,0x08,0x00,0xD1,0xC0,
    0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,
    0x01,0x01,0x01,0x01,0x01,0x01,0x04,0x74,
    0x00,0x30,0xF2,0x70,0x5A,0x80,0xB0,0x58,
    0x8A,0x00,0x20,0xC2,0x31,0x00,0x00,0x1E,
    0x02,0x3A,0x80,0x18,0x71,0x38,0x2D,0x40,
    0x58,0x2C,0x45,0x00,0xC4,0x8E,0x21,0x00,
    0x00,0x1E,0x00,0x00,0x00,0xFC,0x00,0x48,
    0x44,0x20,0x56,0x69,0x64,0x65,0x6F,0x0A,
    0x20,0x20,0x20,0x20,0x00,0x00,0x00,0xFD,
    0x00,0x3B,0x46,0x1F,0x8C,0x3C,0x00,0x0A,
    0x20,0x20,0x20,0x20,0x20,0x20,0x01,0xC0,
    0x02,0x03,0x23,0xF2,0x4E,0x05,0x84,0x13,
    0x14,0x90,0x1F,0x20,0x22,0x5D,0x5F,0x62,
    0x64,0x5E,0x63,0x23,0x09,0x1F,0x07,0x83,
    0x01,0x00,0x00,0x67,0x03,0x0C,0x00,0x10,
    0x00,0x00,0x44,0x8C,0x0A,0xD0,0x8A,0x20,
    0xE0,0x2D,0x10,0x10,0x3E,0x96,0x00,0xC4,
    0x8E,0x21,0x00,0x00,0x18,0x8C,0x0A,0xA0,
    0x14,0x51,0xF0,0x16,0x00,0x26,0x7C,0x43,
    0x00,0xC4,0x8E,0x21,0x00,0x00,0x98,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xE7,
};

//========================================================================================
// add by wf8421 20190820
//------------------------------------------------------------------------------
// Function:    mapp_MHD1616STX_PlatformTimerSet
// Description: 设置相关功能定时器时间
//------------------------------------------------------------------------------
void mapp_MHD1616STX_PlatformTimerSet( uint8_t index, u32 m_sec )
{
    switch ( index )
    {
        default:
            if (index<emUserTimer_Max)
            {
                g_MHD1616STX_Timers[index] = m_sec;
            }
            break;
    }
}

//------------------------------------------------------------------------------
// Function:    mapp_MHD1616STX_PlatformTimerGet
// Description: 获取相关功能定时器时间
//------------------------------------------------------------------------------
u32 mapp_MHD1616STX_PlatformTimerGet(uint8_t eTimer)
{
    return(g_MHD1616STX_Timers[eTimer]);
}

//------------------------------------------------------------------------------
// Function:    mapp_MHD1616STX_PlatformTimerInit
// Description: 定时器初始化
//------------------------------------------------------------------------------
void mapp_MHD1616STX_PlatformTimerInit (void)
{
    int i;
    //initializer timer counters in array
    for ( i = 0; i < emUserTimer_Max; i++ )
    {
        g_MHD1616STX_Timers[ i ] = 0;
    }

    //PlatformTimerSet(etKEY_Detect,5000);
}

//------------------------------------------------------------------------------
// Function:    mapp_MHD1616STX_PlatformTimerTask
// Description: 定时器递减函数
//------------------------------------------------------------------------------
void mapp_MHD1616STX_PlatformTimerTask(void)
{
    int i;
    for(i=0;i<emUserTimer_Max;i++)
    {
        if(g_MHD1616STX_Timers[i] > 0)
        {
            g_MHD1616STX_Timers[i]--;
        }
    }
}

//------------------------------------------------------------------------------
// Function:    mapp_MHD1616STX_PlatformTimerExpired
// Description: 相关功能定时器时间是否为0
//------------------------------------------------------------------------------
uint8_t mapp_MHD1616STX_PlatformTimerExpired ( uint8_t timer )           //定时时间到函数
{
    if ( timer < emUserTimer_Max )
    {
        return( g_MHD1616STX_Timers[ timer ] == 0);              //定时时间到时返回1
    }

    return( 0 );                                           //定时时间还没到时返回0
}
//========================================================================================

void MHD1616STX_1msTimerIrq(void)
{
    mapp_MHD1616STX_PlatformTimerTask();
}

void MHD1616STX_SpecificVarInit(void)
{
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    pstProjectSpecVar_t         pstSpecVar = &stMHD1616STXSpecVar;

    memset(pstSpecVar, 0, sizeof(stProjectSpecVar_t));
    pstPrjSaveVar->stSaveSpecVar.pvSpecVar = (void *)pstSpecVar;
    pstPrjSaveVar->stSaveSpecVar.u16SpecVarSize = sizeof(stProjectSpecVar_t);

}

void MHD1616STX_SpecificVarDefault(void)
{
    MHD1616STX_SpecBasicVarDefault();
}

void MHD1616STX_SpecBasicVarDefault(void)
{
    pstProjectSpecVar_t         pstSpecVar = &stMHD1616STXSpecVar;
    u8 i;
    const u8 cl_u8WinCfgVar[]= { 0x00,0x01,0x00,0x00,0x00,0x00,0x80,0x07,0x38,0x04,0x00,
        0x00,0x00,0x00,0x80,0x07,0x38,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,
        0x07,0x00,0x00,0x38,0x04,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x01,0x00,0x01};

    pstSpecVar->stBasSpecVar.bPowerOnSta = TRUE;
    pstSpecVar->stBasSpecVar.bIsLock = FALSE;
    pstSpecVar->stBasSpecVar.bIsDebugEn = FALSE;
    for(i=0;i<Def_outport_num;i++)
    {
        memcpy(&pstSpecVar->stBasSpecVar.stCurTimingVar[i],&res_table[1],sizeof(stTimingTable_t));
        memcpy(&pstSpecVar->stBasSpecVar.stWinCfgVar[i],&cl_u8WinCfgVar,sizeof(stWindowsConfig_t));
        pstSpecVar->stBasSpecVar.stWinCfgVar[i].u8LayerID = (i%2==0)?1:3;
        (i%2==0)?(pstSpecVar->stBasSpecVar.stWinCfgVar[i].u8Layer_D0=0x80):(pstSpecVar->stBasSpecVar.stWinCfgVar[i].u8Layer_D2=0x80);
        pstSpecVar->stBasSpecVar.u8TxHdcpMode[i] = 0;
        pstSpecVar->stBasSpecVar.u8TxFormat[i] = 1;

        pstSpecVar->stBasSpecVar.u8ResolutionId[i] = emRes_1920x1080_60;
        pstSpecVar->stBasSpecVar.u8TxSrc[i] = i;
    }
}

bool MHD1616STX_SpecVarValidCheck(void)
{
    pstProjectSpecVar_t         pstSpecVar = &stMHD1616STXSpecVar;
    bool                        isSpecVarValid = TRUE;

    if(pstSpecVar->stBasSpecVar.u8CheckSum != CRC8Calculate((uint8_t *)&pstSpecVar->stBasSpecVar,((uint8_t *)&pstSpecVar->stBasSpecVar.u8CheckSum-(uint8_t *)&pstSpecVar->stBasSpecVar)))
    {
        MHD1616STX_SpecBasicVarDefault();
        pstSpecVar->stBasSpecVar.u8CheckSum = CRC8Calculate((uint8_t *)&pstSpecVar->stBasSpecVar,((uint8_t *)&pstSpecVar->stBasSpecVar.u8CheckSum-(uint8_t *)&pstSpecVar->stBasSpecVar));

        isSpecVarValid = FALSE;
    }

    return isSpecVarValid;
}

void SetUserData_SpecBasicVar(void)
{
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    pstProjectSpecVar_t         pstSpecVar = &stMHD1616STXSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;

    pstBasicVar->u8CheckSum = CRC8Calculate((uint8_t *)pstBasicVar, ((uint8_t *)&pstBasicVar->u8CheckSum-(uint8_t *)pstBasicVar));
    SaveUserDataToEep(eEepDataIndex_SpecBlock1,\
        (ADDR_EEPROM_USER_START+((uint8_t *)&pstPrjSaveVar->stSaveSpecVar-(uint8_t *)pstPrjSaveVar)),\
        (uint8_t *)pstBasicVar, sizeof(stBasicSpecVar_t), TIME_SAVE_TO_EEPROM_IMMEDIATELY, FALSE);
}

// the status when power on device
void MHD1616STX_PrjStatePowerOnInit(void)
{
    pstProjectSpecVar_t         pstSpecVar = &stMHD1616STXSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    u8 i;

    pFuncPrj1MsLoopHook = MHD1616STX_1msTimerIrq;
    pvvInitIDTFunc = mapp_MHD1616STXIDT5VInit;
    memset((u8*)&g_stRxResTable,0,sizeof(stResolution_t)*_HDMI_INPUT_PORT_USED_NUMBER);

    g_bEnableDbgFlag = pstBasicVar->bIsDebugEn;
    pstSpecVar->stBasSpecVar.bPowerOnSta = TRUE;

    for(i=0;i<Def_outport_num;i++)
    {
        mapp_HdmiTx5VEnable(i,TRUE);
        //g_stWinConfigVar[i].u8SrcPort = i+1;
    }
}

void MHD1616STX_SetPowerStateFlag(bool bstate)
{
    sg_bChangePowerStateFlag = bstate;
}

bool MHD1616STX_GetPowerStateFlag(void)
{
    return sg_bChangePowerStateFlag;
}

void mapp_MHD1616STXStartupMessage(void)
{
    RS232_printf("\r\n");
    RS232_printf("System Initializing...\r\n");
    RS232_printf("\r\n");
    RS232_printf("Initialization Finished!\r\n");
    RS232_printf("@%s %s",_STRING_FW_VERSION,_STRING_FW_VER_NUM);
}

void mapp_MHD1616STXLedProcess(void)
{
    static bool sl_bLedSta = FALSE;
    if(mapp_MHD1616STX_PlatformTimerExpired(emUserTimer_LedActive))
    {
        mapp_MHD1616STX_PlatformTimerSet(emUserTimer_LedActive,500);
        if(sl_bLedSta)
        {
            sl_bLedSta = FALSE;
            led_on(led_name_ActiveState);
        }
        else
        {
            sl_bLedSta = TRUE;
            led_off(led_name_ActiveState);
        }
    }
}

void mapp_MHD1616STXMainFunction(void)
{
    mapp_MHD1616STXLedProcess();
    mapp_MHD1616STXStartupToRecoveryState();
    mapp_SourceChangeToSetWinProcess();
}

void mapp_MHD1616STXIDT5VInit(void)
{
    uint8_t tmp, timeout=100;
    uint8_t l_u8DevId=0;

    for(l_u8DevId=0;l_u8DevId<2;l_u8DevId++)
    {
        do {
            delay_ms(5);
            i2c_IDT5V_write_one_byte(l_u8DevId,0x00,0x01);    //mode: Software Mode control
            tmp = i2c_IDT5V_read_one_byte(l_u8DevId,0x00);

        } while((tmp != 0x01) && --timeout);

        if(tmp != 0x01)
        {
            //Uart_printf("IDT_5V[%d]_init Fail\r\n",l_u8DevId);
            return;
        }

        i2c_IDT5V_write_one_byte(l_u8DevId,0x01,0x00);    //Select CONFIG0
        i2c_IDT5V_write_one_byte(l_u8DevId,0x02,0x00);    //outputs enabled
        i2c_IDT5V_write_one_byte(l_u8DevId,0x03,0xFF);    //output not suspend

       //0x96
        i2c_IDT5V_write_one_byte(l_u8DevId,0xC0,0x96);    //crystal/REFIN, auto switch mode, PLL3 normal, SRC0: DIV3, SRC1: PLL2
        //PLL0: 148.5M
        i2c_IDT5V_write_one_byte(l_u8DevId,0x10,0x04);    //D=3
        i2c_IDT5V_write_one_byte(l_u8DevId,0x18,0x16);    //N=11
        i2c_IDT5V_write_one_byte(l_u8DevId,0x1C,0x00);    //A=0, M=2*N=22 >10~8026
        //PLL1: 148.5M
        i2c_IDT5V_write_one_byte(l_u8DevId,0x28,0x04);
        i2c_IDT5V_write_one_byte(l_u8DevId,0x30,0x2c);
        i2c_IDT5V_write_one_byte(l_u8DevId,0x34,0x00);
        //PLL2: 148.5M
        i2c_IDT5V_write_one_byte(l_u8DevId,0x3C,0x10); //Loop Parameter
        i2c_IDT5V_write_one_byte(l_u8DevId,0x40,0x04);
        i2c_IDT5V_write_one_byte(l_u8DevId,0x48,0x2c);  //N=297
        i2c_IDT5V_write_one_byte(l_u8DevId,0x4C,0x80);    //Spread Spectrum Enable
        //PLL3: 148.5M
        i2c_IDT5V_write_one_byte(l_u8DevId,0x5C,0x04);//D=2   D>1~127
        i2c_IDT5V_write_one_byte(l_u8DevId,0x64,0x2c);//N=22  N>12~4095

            /*SRC6: PLL1 OUT3 OUT6
              SRC5: PLL3 OUT5
              SRC4: PLL0 OUT4
              SRC2: PLL2 OUT1 OUT2
            */
        i2c_IDT5V_write_one_byte(l_u8DevId,0xCC,0xBE);    //SRC6: PLL1, SRC5: PLL3, SRC4: PLL0
        i2c_IDT5V_write_one_byte(l_u8DevId,0xC4,0x5D);    //SRC3: PLL1, SRC2: PLL2
        i2c_IDT5V_write_one_byte(l_u8DevId,0x75,0x00);    //S3=0 - Both from DIV6, S1=0 - Both from DIV2
        i2c_IDT5V_write_one_byte(l_u8DevId,0x7A,0x05);    //OUT5 and OUT5b: invert clock, LVDS
        i2c_IDT5V_write_one_byte(l_u8DevId,0x79,0x05);    //OUT4 and OUT4b: invert clock, LVDS
        i2c_IDT5V_write_one_byte(l_u8DevId,0x78,0x05);    //OUT3 and OUT6: invert clock, LVDS
        i2c_IDT5V_write_one_byte(l_u8DevId,0x76,0x05);    //OUT1 and OUT2: invert clock, LVDS
        //DIV1~6: /2
        i2c_IDT5V_write_one_byte(l_u8DevId,0x88,0x0F);
        i2c_IDT5V_write_one_byte(l_u8DevId,0x90,0x0F);
        i2c_IDT5V_write_one_byte(l_u8DevId,0x94,0x0F);
        i2c_IDT5V_write_one_byte(l_u8DevId,0x9C,0x0F);
        i2c_IDT5V_write_one_byte(l_u8DevId,0xA0,0x0F);
        i2c_IDT5V_write_one_byte(l_u8DevId,0xA8,0x0F);
    }
}

void mapp_MHD1616STX_SendDataToFpag(pu8 pu8Data, u16 u16Len)
{
    pstUartCom_t pUartCommVar = &g_stUart2CommVar;
    pUartCommVar->SendBytes(pu8Data,u16Len,FALSE);
}

/******************************************************
///Function Name:   send_cmd_to_fpga
///Description:     Send command to FPGA
///Input:           u8Addr(FPGA控制地址),u8Cmd(命令码),
///                 u16Len(数据长度),pu8Data(控制数据)
///Output:          NULL
///Return:          NULL
///Others:          NULL
******************************************************/
void send_cmd_to_fpga(uint8_t u8Addr,uint8_t u8Cmd,uint8_t* pu8Data,uint16_t u16Len)
{
    uint8_t l_u8Buff[100]={0xff,0xff,0xa5,0xfe,0xcc,0x01,0x00,0xaa};

    if(u16Len>99)return ;
    l_u8Buff[3]=u8Addr;
    l_u8Buff[4]=u8Cmd;

    l_u8Buff[5]=u16Len;
    l_u8Buff[6]=u16Len>>8;
    if(pu8Data!=NULL)
    {
        memcpy(l_u8Buff+7,pu8Data,u16Len);    // head
    }

    mapp_MHD1616STX_SendDataToFpag(l_u8Buff,7+u16Len);
}

void mapp_MHD1616STXWinDataCopy(u8 *u8Data, pstWindowsConfig_t pstWincfgDat)
{
    u8Data[0]  = pstWincfgDat->u8SrcAddr;
    u8Data[1]  = pstWincfgDat->u8LayerID;
    u8Data[2]  = pstWincfgDat->u8Layer_D0;
    u8Data[3]  = pstWincfgDat->u8Layer_D1;
    u8Data[4]  = pstWincfgDat->u8Layer_D2;
    u8Data[5]  = pstWincfgDat->u8Layer_D3;
    u8Data[6]  = pstWincfgDat->u16SrcAct_H&0xFF;
    u8Data[7]  = (pstWincfgDat->u16SrcAct_H>>8)&0xFF;
    u8Data[8]  = pstWincfgDat->u16SrcAct_V&0xFF;
    u8Data[9]  = (pstWincfgDat->u16SrcAct_V>>8)&0xFF;
    u8Data[10]  = pstWincfgDat->u16Src_X&0xFF;
    u8Data[11]  = (pstWincfgDat->u16Src_X>>8)&0xFF;
    u8Data[12]  = pstWincfgDat->u16Src_Y&0xFF;
    u8Data[13]  = (pstWincfgDat->u16Src_Y>>8)&0xFF;
    u8Data[14]  = pstWincfgDat->u16Src_H&0xFF;
    u8Data[15]  = (pstWincfgDat->u16Src_H>>8)&0xFF;
    u8Data[16]  = pstWincfgDat->u16Src_V&0xFF;
    u8Data[17]  = (pstWincfgDat->u16Src_V>>8)&0xFF;

    u8Data[18]  = pstWincfgDat->u32Wnd_X&0xFF;
    u8Data[19]  = (pstWincfgDat->u32Wnd_X>>8)&0xFF;
    u8Data[20]  = (pstWincfgDat->u32Wnd_X>>16)&0xFF;
    u8Data[21]  = (pstWincfgDat->u32Wnd_X>>24)&0xFF;
    u8Data[22]  = pstWincfgDat->u32Wnd_Y&0xFF;
    u8Data[23]  = (pstWincfgDat->u32Wnd_Y>>8)&0xFF;
    u8Data[24]  = (pstWincfgDat->u32Wnd_Y>>16)&0xFF;
    u8Data[25]  = (pstWincfgDat->u32Wnd_Y>>24)&0xFF;

    u8Data[26]  = pstWincfgDat->u32Wnd_H&0xFF;
    u8Data[27]  = (pstWincfgDat->u32Wnd_H>>8)&0xFF;
    u8Data[28]  = (pstWincfgDat->u32Wnd_H>>16)&0xFF;
    u8Data[29]  = (pstWincfgDat->u32Wnd_H>>24)&0xFF;
    u8Data[30]  = pstWincfgDat->u32Wnd_V&0xFF;
    u8Data[31]  = (pstWincfgDat->u32Wnd_V>>8)&0xFF;
    u8Data[32]  = (pstWincfgDat->u32Wnd_V>>16)&0xFF;
    u8Data[33]  = (pstWincfgDat->u32Wnd_V>>24)&0xFF;

    u8Data[34]  = pstWincfgDat->u32Srf_H&0xFF;
    u8Data[35]  = (pstWincfgDat->u32Srf_H>>8)&0xFF;
    u8Data[36]  = (pstWincfgDat->u32Srf_H>>16)&0xFF;
    u8Data[37]  = (pstWincfgDat->u32Srf_H>>24)&0xFF;
    u8Data[38]  = pstWincfgDat->u32Srf_V&0xFF;
    u8Data[39]  = (pstWincfgDat->u32Srf_V>>8)&0xFF;
    u8Data[40]  = (pstWincfgDat->u32Srf_V>>16)&0xFF;
    u8Data[41]  = (pstWincfgDat->u32Srf_V>>24)&0xFF;
    u8Data[42]  = pstWincfgDat->u8SrcInput;
}

void mapp_MHD1616STXWindowCfg(uint8_t out_ch,uint8_t in_ch)
{
    u8 l_u8Temp[50];
    stWindowsConfig_t stWinConfig;
    uint8_t layer_map[]={0x01,0x03,0x01,0x03,0x01,0x03,0x01,0x03,0x01,0x03,0x01,0x03,0x01,0x03,0x01,0x03};

    stWinConfig.u8SrcAddr = 0;
    stWinConfig.u8LayerID = layer_map[out_ch-1];
    stWinConfig.u8Layer_D0 = 0x00;
    stWinConfig.u8Layer_D1 = 0x00;
    stWinConfig.u8Layer_D2 = 0x00;
    stWinConfig.u8Layer_D3 = 0x00;
    switch(stWinConfig.u8LayerID)
    {
        case 0x01:
            stWinConfig.u8Layer_D0 = 0x80;
            break;
        case 0x02:
            stWinConfig.u8Layer_D1 = 0x80;
            break;
        case 0x03:
            stWinConfig.u8Layer_D2 = 0x80;
            break;
        case 0x04:
            stWinConfig.u8Layer_D3 = 0x80;
            break;
        default:
            break;
    }
//    stWinConfig.u16SrcAct_H = (g_stWinConfigVar[out_ch-1].stSrcWin.u16Width)?(g_stWinConfigVar[out_ch-1].stSrcWin.u16Width):1920;
//    stWinConfig.u16SrcAct_V = (g_stWinConfigVar[out_ch-1].stSrcWin.u16Height)?(g_stWinConfigVar[out_ch-1].stSrcWin.u16Height):1080;
    stWinConfig.u16SrcAct_H = (g_stRxResTable[in_ch-1].u16Width)?(g_stRxResTable[in_ch-1].u16Width):1920;
    stWinConfig.u16SrcAct_V = (g_stRxResTable[in_ch-1].u16Height)?(g_stRxResTable[in_ch-1].u16Height):1080;
    stWinConfig.u16Src_X    = 0;
    stWinConfig.u16Src_Y    = 0;
    stWinConfig.u16Src_H    = stWinConfig.u16SrcAct_H;
    stWinConfig.u16Src_V    = stWinConfig.u16SrcAct_V;
    stWinConfig.u32Wnd_X    = 0;
    stWinConfig.u32Wnd_Y    = 0;
    stWinConfig.u32Wnd_H    = g_stWinConfigVar[out_ch-1].stDstWin.u16Width;
    stWinConfig.u32Wnd_V    = g_stWinConfigVar[out_ch-1].stDstWin.u16Height;
    stWinConfig.u32Srf_H    = (((uint64_t)stWinConfig.u16Src_H<<16))/(uint64_t)stWinConfig.u32Wnd_H;
    stWinConfig.u32Srf_V    = (((uint64_t)stWinConfig.u16Src_V<<16))/(uint64_t)stWinConfig.u32Wnd_V;
    stWinConfig.u8SrcInput  = in_ch;

//    dbg_printf("data size  == %d \r\n",sizeof(stWindowsConfig_t));

    mapp_MHD1616STXWinDataCopy(l_u8Temp,&stWinConfig);
    send_cmd_to_fpga(out_ch,eFpgaRegOutWinCfg, l_u8Temp, 43);

    return;
}

void mapp_MHD1616STXFpgaWinCmdUpdate(u32 u32DelayMs)
{
    u8 l_u8cmd = 0;
    delay_ms_cpu(u32DelayMs);
    send_cmd_to_fpga(0xFE,eFpgaRegOutUpdate,&l_u8cmd,1);
}

void mapp_MHD1616STX_TimingConfig(uint8_t ch,uint8_t idx)
{
    u8 l_u8Temp[50];
    if(/*idx==0||*/idx>=RES_DEFAULT_MAX)
        return ;
    memset(l_u8Temp,0,50);

//    dbg_printf("u16HActive  == %d \r\n",res_table[idx].u16HActive);
//    dbg_printf("u16HTotal   == %d \r\n",res_table[idx].u16HTotal);
//    dbg_printf("u16HBP      == %d \r\n",res_table[idx].u16HBP);
//    dbg_printf("u16HFP      == %d \r\n",res_table[idx].u16HFP);
//    dbg_printf("u8HSync     == %d \r\n",res_table[idx].u16HSync);
//    dbg_printf("u8HPol      == %d \r\n",res_table[idx].u8HPol);
//    dbg_printf("u16VActive  == %d \r\n",res_table[idx].u16VActive);
//    dbg_printf("u16VTotal   == %d \r\n",res_table[idx].u16VTotal);
//    dbg_printf("u16VBP      == %d \r\n",res_table[idx].u16VBP);
//    dbg_printf("u16VFP      == %d \r\n",res_table[idx].u16VFP);
//    dbg_printf("u8VSync     == %d \r\n",res_table[idx].u16VSync);
//    dbg_printf("u8VPol      == %d \r\n",res_table[idx].u8VPol);
//    dbg_printf("u8VClk      == %d \r\n",res_table[idx].u8VClk);
//    dbg_printf("u16PClk     == %d \r\n",res_table[idx].u16PClk);
//    dbg_printf("u8Vic       == %d \r\n",res_table[idx].u8Vic);
    l_u8Temp[0]  = res_table[idx].u16HActive&0xFF;
    l_u8Temp[1]  = (res_table[idx].u16HActive>>8)&0xFF;
    l_u8Temp[2]  = res_table[idx].u16HTotal&0xFF;
    l_u8Temp[3]  = (res_table[idx].u16HTotal>>8)&0xFF;
    l_u8Temp[4]  = res_table[idx].u16HBP&0xFF;
    l_u8Temp[5]  = (res_table[idx].u16HBP>>8)&0xFF;
    l_u8Temp[6]  = res_table[idx].u16HFP&0xFF;
    l_u8Temp[7]  = (res_table[idx].u16HFP>>8)&0xFF;
    l_u8Temp[8]  = res_table[idx].u16HSync&0xFF;
    l_u8Temp[9]  = (res_table[idx].u16HSync>>8)&0xFF;
    l_u8Temp[10] = res_table[idx].u8HPol&0xFF;
    l_u8Temp[11] = res_table[idx].u16VActive&0xFF;
    l_u8Temp[12] = (res_table[idx].u16VActive>>8)&0xFF;
    l_u8Temp[13] = res_table[idx].u16VTotal&0xFF;
    l_u8Temp[14] = (res_table[idx].u16VTotal>>8)&0xFF;
    l_u8Temp[15] = res_table[idx].u16VBP&0xFF;
    l_u8Temp[16] = (res_table[idx].u16VBP>>8)&0xFF;
    l_u8Temp[17] = res_table[idx].u16VFP&0xFF;
    l_u8Temp[18] = (res_table[idx].u16VFP>>8)&0xFF;
    l_u8Temp[19] = res_table[idx].u16VSync&0xFF;
    l_u8Temp[20] = (res_table[idx].u16VSync>>8)&0xFF;
    l_u8Temp[21] = res_table[idx].u8VPol&0xFF;

    send_cmd_to_fpga(ch,eFpgaRegOutTiming, l_u8Temp, 22);
}

void mapp_MHD1616STXResolutionConfig(u8 u8Out,u8 u8ResID)
{
    const u8 cl_u8ChMap[16] = {0,0,1,1,2,2,3,3,0,0,1,1,2,2,3,3};
    mapp_MHD1616STX_TimingConfig(u8Out+1,u8ResID);
    mapp_IDTSetPclk(u8Out/8,res_table[u8ResID].u16PClk,cl_u8ChMap[u8Out]);
    //delay_ms(10);
    SwitchResolutionToRefreshWindowns(u8Out,u8ResID);
}

void SwitchResolutionToRefreshWindowns(u8 u8Out,u8 u8ResID)
{
    pstProjectSpecVar_t         pstSpecVar = &stMHD1616STXSpecVar;
    pstBasicSpecVar_t           pstBasicSpecVar = &pstSpecVar->stBasSpecVar;
    u8 i;

    switch(u8Out)
    {
        case 0:
        case 1:
            {
                for(i=0;i<2;i++)
                {
                    g_stWinConfigVar[i].stDstWin.u16Width = res_table[u8ResID].u16HActive;
                    g_stWinConfigVar[i].stDstWin.u16Height= res_table[u8ResID].u16VActive;
                    mapp_GsvSetTimingChange(i);
                    mapp_MHD1616STXWindowCfg(i+1,pstBasicSpecVar->u8TxSrc[i]+1);
                    pstBasicSpecVar->u8ResolutionId[i]=u8ResID+1;
                }
            }
            break;
        case 2:
        case 3:
            {
                for(i=0;i<2;i++)
                {
                    g_stWinConfigVar[i+2].stDstWin.u16Width = res_table[u8ResID].u16HActive;
                    g_stWinConfigVar[i+2].stDstWin.u16Height = res_table[u8ResID].u16VActive;
                    mapp_GsvSetTimingChange(i+2);
                    mapp_MHD1616STXWindowCfg(i+1+2,pstBasicSpecVar->u8TxSrc[i+2]+1);
                    pstBasicSpecVar->u8ResolutionId[i+2]=u8ResID+1;
                }
            }
            break;
        case 4:
        case 5:
            {
                for(i=0;i<2;i++)
                {
                    g_stWinConfigVar[i+4].stDstWin.u16Width = res_table[u8ResID].u16HActive;
                    g_stWinConfigVar[i+4].stDstWin.u16Height = res_table[u8ResID].u16VActive;
                    mapp_GsvSetTimingChange(i+4);
                    mapp_MHD1616STXWindowCfg(i+1+4,pstBasicSpecVar->u8TxSrc[i+4]+1);
                    pstBasicSpecVar->u8ResolutionId[i+4]=u8ResID+1;
                }
            }
            break;
        case 6:
        case 7:
            {
                for(i=0;i<2;i++)
                {
                    g_stWinConfigVar[i+6].stDstWin.u16Width = res_table[u8ResID].u16HActive;
                    g_stWinConfigVar[i+6].stDstWin.u16Height = res_table[u8ResID].u16VActive;
                    mapp_GsvSetTimingChange(i+6);
                    mapp_MHD1616STXWindowCfg(i+1+6,pstBasicSpecVar->u8TxSrc[i+6]+1);
                    pstBasicSpecVar->u8ResolutionId[i+6]=u8ResID+1;
                }
            }
            break;
        case 8:
        case 9:
            {
                for(i=0;i<2;i++)
                {
                    g_stWinConfigVar[i+8].stDstWin.u16Width = res_table[u8ResID].u16HActive;
                    g_stWinConfigVar[i+8].stDstWin.u16Height = res_table[u8ResID].u16VActive;
                    mapp_GsvSetTimingChange(i+8);
                    mapp_MHD1616STXWindowCfg(i+1+8,pstBasicSpecVar->u8TxSrc[i+8]+1);
                    pstBasicSpecVar->u8ResolutionId[i+8]=u8ResID+1;
                }
            }
            break;
        case 10:
        case 11:
            {
                for(i=0;i<2;i++)
                {
                    g_stWinConfigVar[i+10].stDstWin.u16Width = res_table[u8ResID].u16HActive;
                    g_stWinConfigVar[i+10].stDstWin.u16Height = res_table[u8ResID].u16VActive;
                    mapp_GsvSetTimingChange(i+10);
                    mapp_MHD1616STXWindowCfg(i+1+10,pstBasicSpecVar->u8TxSrc[i+10]+1);
                    pstBasicSpecVar->u8ResolutionId[i+10]=u8ResID+1;
                }
            }
            break;
        case 12:
        case 13:
            {
                for(i=0;i<2;i++)
                {
                    g_stWinConfigVar[i+12].stDstWin.u16Width = res_table[u8ResID].u16HActive;
                    g_stWinConfigVar[i+12].stDstWin.u16Height = res_table[u8ResID].u16VActive;
                    mapp_GsvSetTimingChange(i+12);
                    mapp_MHD1616STXWindowCfg(i+1+12,pstBasicSpecVar->u8TxSrc[i+12]+1);
                    pstBasicSpecVar->u8ResolutionId[i+12]=u8ResID+1;
                }
            }
            break;
        case 14:
        case 15:
            {
                for(i=0;i<2;i++)
                {
                    g_stWinConfigVar[i+14].stDstWin.u16Width = res_table[u8ResID].u16HActive;
                    g_stWinConfigVar[i+14].stDstWin.u16Height = res_table[u8ResID].u16VActive;
                    mapp_GsvSetTimingChange(i+14);
                    mapp_MHD1616STXWindowCfg(i+1+14,pstBasicSpecVar->u8TxSrc[i+14]+1);
                    pstBasicSpecVar->u8ResolutionId[i+14]=u8ResID+1;
                }
            }
            break;
        default:
            break;
    }

    mapp_MHD1616STXFpgaWinCmdUpdate(10);
}

void mapp_MHD1616STXWindowCfg2(uint8_t out_ch,pstWindowsConfig_t pstWinConfig)
{
    u8 l_u8Temp[50];

    memset(l_u8Temp,0,50);
    pstWinConfig->u8SrcAddr = 0;
    g_stWinConfigVar[out_ch-1].stSrcWin.u16Width = pstWinConfig->u16SrcAct_H;
    g_stWinConfigVar[out_ch-1].stSrcWin.u16Height = pstWinConfig->u16SrcAct_V;
    g_stWinConfigVar[out_ch-1].stDstWin.u16Width = pstWinConfig->u32Wnd_H;
    g_stWinConfigVar[out_ch-1].stDstWin.u16Height = pstWinConfig->u32Wnd_V;

    mapp_MHD1616STXWinDataCopy(l_u8Temp,pstWinConfig);
    send_cmd_to_fpga(out_ch,eFpgaRegOutWinCfg, l_u8Temp, 43);

    return;
}

void mapp_MHD1616STX_TimingConfig2(uint8_t ch,pstTimingTable_t pstTiming)
{
    u8 l_u8Temp[50];

    memset(l_u8Temp,0,50);
//    dbg_printf("u16HActive  == %d \r\n",pstTiming->u16HActive);
//    dbg_printf("u16HTotal   == %d \r\n",pstTiming->u16HTotal);
//    dbg_printf("u16HBP      == %d \r\n",pstTiming->u16HBP);
//    dbg_printf("u16HFP      == %d \r\n",pstTiming->u16HFP);
//    dbg_printf("u8HSync     == %d \r\n",pstTiming->u16HSync);
//    dbg_printf("u8HPol      == %d \r\n",pstTiming->u8HPol);
//    dbg_printf("u16VActive  == %d \r\n",pstTiming->u16VActive);
//    dbg_printf("u16VTotal   == %d \r\n",pstTiming->u16VTotal);
//    dbg_printf("u16VBP      == %d \r\n",pstTiming->u16VBP);
//    dbg_printf("u16VFP      == %d \r\n",pstTiming->u16VFP);
//    dbg_printf("u8VSync     == %d \r\n",pstTiming->u16VSync);
//    dbg_printf("u8VPol      == %d \r\n",pstTiming->u8VPol);
//    dbg_printf("u8VClk      == %d \r\n",pstTiming->u8VClk);
//    dbg_printf("u16PClk     == %d \r\n",pstTiming->u16PClk);
//    dbg_printf("u8Vic       == %d \r\n",pstTiming->u8Vic);
    l_u8Temp[0]  = pstTiming->u16HActive&0xFF;
    l_u8Temp[1]  = (pstTiming->u16HActive>>8)&0xFF;
    l_u8Temp[2]  = pstTiming->u16HTotal&0xFF;
    l_u8Temp[3]  = (pstTiming->u16HTotal>>8)&0xFF;
    l_u8Temp[4]  = pstTiming->u16HBP&0xFF;
    l_u8Temp[5]  = (pstTiming->u16HBP>>8)&0xFF;
    l_u8Temp[6]  = pstTiming->u16HFP&0xFF;
    l_u8Temp[7]  = (pstTiming->u16HFP>>8)&0xFF;
    l_u8Temp[8]  = pstTiming->u16HSync&0xFF;
    l_u8Temp[9]  = (pstTiming->u16HSync>>8)&0xFF;
    l_u8Temp[10] = pstTiming->u8HPol&0xFF;
    l_u8Temp[11] = pstTiming->u16VActive&0xFF;
    l_u8Temp[12] = (pstTiming->u16VActive>>8)&0xFF;
    l_u8Temp[13] = pstTiming->u16VTotal&0xFF;
    l_u8Temp[14] = (pstTiming->u16VTotal>>8)&0xFF;
    l_u8Temp[15] = pstTiming->u16VBP&0xFF;
    l_u8Temp[16] = (pstTiming->u16VBP>>8)&0xFF;
    l_u8Temp[17] = pstTiming->u16VFP&0xFF;
    l_u8Temp[18] = (pstTiming->u16VFP>>8)&0xFF;
    l_u8Temp[19] = pstTiming->u16VSync&0xFF;
    l_u8Temp[20] = (pstTiming->u16VSync>>8)&0xFF;
    l_u8Temp[21] = pstTiming->u8VPol&0xFF;

    send_cmd_to_fpga(ch,eFpgaRegOutTiming, l_u8Temp, 22);
    mapp_MHD1616STXFpgaWinCmdUpdate(10);
}

void mapp_MHD1616STXResolutionConfig2(u8 u8Out,pstTimingTable_t pstTiming)
{
    const u8 cl_u8ChMap[16] = {2,2,0,0,3,3,1,1,2,2,0,0,3,3,1,1};
    mapp_MHD1616STX_TimingConfig2(u8Out+1,pstTiming);
    mapp_IDTSetPclk(u8Out/8,pstTiming->u16PClk,cl_u8ChMap[u8Out]);
    //delay_ms(10);
    SwitchResolutionToRefreshWindowns2(u8Out,pstTiming);
}

void SwitchResolutionToRefreshWindowns2(u8 u8Out,pstTimingTable_t pstTiming)
{
	pstProjectSpecVar_t			pstSpecVar = &stMHD1616STXSpecVar;
	pstBasicSpecVar_t			pstBasicSpecVar = &pstSpecVar->stBasSpecVar;
    u8 i;

    switch(u8Out)
    {
        case 0:
        case 1:
            {
                for(i=0;i<2;i++)
                {
                    g_stWinConfigVar[i].stDstWin.u16Height = pstTiming->u16HActive;
                    g_stWinConfigVar[i].stDstWin.u16Width= pstTiming->u16VActive;
                    pstBasicSpecVar->stWinCfgVar[i].u32Wnd_H = pstTiming->u16HActive;
                    pstBasicSpecVar->stWinCfgVar[i].u32Wnd_V= pstTiming->u16VActive;
                    mapp_GsvSetTimingChange(i);
                    mapp_MHD1616STXWindowCfg2(i+1,&pstBasicSpecVar->stWinCfgVar[i]);
//                    pstBasicSpecVar->u8ResolutionId[i]=u8ResID+1;
                }
            }
            break;
        case 2:
        case 3:
            {
                for(i=0;i<2;i++)
                {
                    g_stWinConfigVar[i+2].stDstWin.u16Height = pstTiming->u16HActive;
                    g_stWinConfigVar[i+2].stDstWin.u16Width = pstTiming->u16VActive;
                    pstBasicSpecVar->stWinCfgVar[i+2].u32Wnd_H = pstTiming->u16HActive;
                    pstBasicSpecVar->stWinCfgVar[i+2].u32Wnd_V= pstTiming->u16VActive;
                    mapp_GsvSetTimingChange(i+2);
                    mapp_MHD1616STXWindowCfg2(i+1+2,&pstBasicSpecVar->stWinCfgVar[i+2]);
//                    pstBasicSpecVar->u8ResolutionId[i+2]=u8ResID+1;
                }
            }
            break;
        case 4:
        case 5:
            {
                for(i=0;i<2;i++)
                {
                    g_stWinConfigVar[i+4].stDstWin.u16Height = pstTiming->u16HActive;
                    g_stWinConfigVar[i+4].stDstWin.u16Width = pstTiming->u16VActive;
                    pstBasicSpecVar->stWinCfgVar[i+4].u32Wnd_H = pstTiming->u16HActive;
                    pstBasicSpecVar->stWinCfgVar[i+4].u32Wnd_V= pstTiming->u16VActive;
                    mapp_GsvSetTimingChange(i+4);
                    mapp_MHD1616STXWindowCfg2(i+1+4,&pstBasicSpecVar->stWinCfgVar[i+4]);
//                    pstBasicSpecVar->u8ResolutionId[i+4]=u8ResID+1;
                }
            }
            break;
        case 6:
        case 7:
            {
                for(i=0;i<2;i++)
                {
                    g_stWinConfigVar[i+6].stDstWin.u16Height = pstTiming->u16HActive;
                    g_stWinConfigVar[i+6].stDstWin.u16Width = pstTiming->u16VActive;
                    pstBasicSpecVar->stWinCfgVar[i+6].u32Wnd_H = pstTiming->u16HActive;
                    pstBasicSpecVar->stWinCfgVar[i+6].u32Wnd_V= pstTiming->u16VActive;
                    mapp_GsvSetTimingChange(i+6);
                    mapp_MHD1616STXWindowCfg2(i+1+6,&pstBasicSpecVar->stWinCfgVar[i+6]);
//                    pstBasicSpecVar->u8ResolutionId[i+6]=u8ResID+1;
                }
            }
            break;
        case 8:
        case 9:
            {
                for(i=0;i<2;i++)
                {
                    g_stWinConfigVar[i+8].stDstWin.u16Height = pstTiming->u16HActive;
                    g_stWinConfigVar[i+8].stDstWin.u16Width = pstTiming->u16VActive;
                    pstBasicSpecVar->stWinCfgVar[i+8].u32Wnd_H = pstTiming->u16HActive;
                    pstBasicSpecVar->stWinCfgVar[i+8].u32Wnd_V= pstTiming->u16VActive;
                    mapp_GsvSetTimingChange(i+8);
                    mapp_MHD1616STXWindowCfg2(i+1+8,&pstBasicSpecVar->stWinCfgVar[i+8]);
//                    pstBasicSpecVar->u8ResolutionId[i+8]=u8ResID+1;
                }
            }
            break;
        case 10:
        case 11:
            {
                for(i=0;i<2;i++)
                {
                    g_stWinConfigVar[i+10].stDstWin.u16Height = pstTiming->u16HActive;
                    g_stWinConfigVar[i+10].stDstWin.u16Width = pstTiming->u16VActive;
                    pstBasicSpecVar->stWinCfgVar[i+10].u32Wnd_H = pstTiming->u16HActive;
                    pstBasicSpecVar->stWinCfgVar[i+10].u32Wnd_V= pstTiming->u16VActive;
                    mapp_GsvSetTimingChange(i+10);
                    mapp_MHD1616STXWindowCfg2(i+1+10,&pstBasicSpecVar->stWinCfgVar[i+10]);
//                    pstBasicSpecVar->u8ResolutionId[i+10]=u8ResID+1;
                }
            }
            break;
        case 12:
        case 13:
            {
                for(i=0;i<2;i++)
                {
                    g_stWinConfigVar[i+12].stDstWin.u16Height = pstTiming->u16HActive;
                    g_stWinConfigVar[i+12].stDstWin.u16Width = pstTiming->u16VActive;
                    pstBasicSpecVar->stWinCfgVar[i+12].u32Wnd_H = pstTiming->u16HActive;
                    pstBasicSpecVar->stWinCfgVar[i+12].u32Wnd_V= pstTiming->u16VActive;
                    mapp_GsvSetTimingChange(i+12);
                    mapp_MHD1616STXWindowCfg2(i+1+12,&pstBasicSpecVar->stWinCfgVar[i+12]);
//                    pstBasicSpecVar->u8ResolutionId[i+12]=u8ResID+1;
                }
            }
            break;
        case 14:
        case 15:
            {
                for(i=0;i<2;i++)
                {
                    g_stWinConfigVar[i+14].stDstWin.u16Height = pstTiming->u16HActive;
                    g_stWinConfigVar[i+14].stDstWin.u16Width = pstTiming->u16VActive;
                    pstBasicSpecVar->stWinCfgVar[i+14].u32Wnd_H = pstTiming->u16HActive;
                    pstBasicSpecVar->stWinCfgVar[i+14].u32Wnd_V= pstTiming->u16VActive;
                    mapp_GsvSetTimingChange(i+14);
                    mapp_MHD1616STXWindowCfg2(i+1+14,&pstBasicSpecVar->stWinCfgVar[i+14]);
//                    pstBasicSpecVar->u8ResolutionId[i+14]=u8ResID+1;
                }
            }
            break;
        default:
            break;
    }

    mapp_MHD1616STXFpgaWinCmdUpdate(10);
}

void mapp_MHD1616STXWincfgNoiseMute(bool bMute)
{
    u8 l_u8Data[1];

    l_u8Data[0] = bMute;
    send_cmd_to_fpga(0x01,eFpgaRegWinCfgAudioMuteEn,l_u8Data,1);
}

void mapp_GetFpgaVersion(u8 u8Addr)
{
    u8 l_u8Addr;
    u8 l_u8Cmd;
    u8 l_u8Data = 0;

    l_u8Addr = u8Addr;
    l_u8Cmd = eFpgaRegGetVersion;
	send_cmd_to_fpga(l_u8Addr,l_u8Cmd,&l_u8Data,1);
}

typedef enum _eStartupLoopType_t_
{
    eSt_SInit,
    eSt_GetFPGAVersion,
    eSt_ResolutionRecover,
    eSt_WindowsState,
    eSt_None,
}eStartupLoopType_t;

void mapp_MHD1616STXStartupToRecoveryState(void)
{
    u8 i;
    static bool sl_bStartupFlag = TRUE;
    static eStartupLoopType_t sl_eStartupLoop = eSt_None;

    if(!mapp_MHD1616STX_PlatformTimerExpired(emUserTimer_StartupTime))
        return;

    if(sl_bStartupFlag)
    {
        sl_bStartupFlag = FALSE;
        sl_eStartupLoop = eSt_SInit;
    }

    switch(sl_eStartupLoop)
    {
        case eSt_SInit:
            {
                sl_eStartupLoop++;
            }
            break;
        case eSt_GetFPGAVersion:
            {
                mapp_GetFpgaVersion(1);
                delay_ms(50);
//                mapp_GetFpgaVersion(5);
//                delay_ms(50);
//                mapp_GetFpgaVersion(9);
//                delay_ms(50);
//                mapp_GetFpgaVersion(13);
                mapp_MHD1616STX_PlatformTimerSet(emUserTimer_StartupTime,200);
//                delay_ms(50);
                if(g_u8VersionReceive)
                    sl_eStartupLoop++;
            }
            break;
        case eSt_ResolutionRecover:
            {
                pstProjectSpecVar_t         pstSpecVar = &stMHD1616STXSpecVar;
                pstBasicSpecVar_t           pstBasicSpecVar = &pstSpecVar->stBasSpecVar;
                for(i=0;i<Def_outport_num;i++)
                {
                    if(i%2==0)
                    {
                        mapp_MHD1616STXResolutionConfig(i,pstBasicSpecVar->u8ResolutionId[i]);
                        //mapp_MHD1616STXResolutionConfig2(i,&pstBasicSpecVar->stCurTimingVar[i]);
                    }
                }
                mapp_MHD1616STX_PlatformTimerSet(emUserTimer_StartupTime,200);
                sl_eStartupLoop++;
            }
            break;
        case eSt_WindowsState:
            {
//                for(i=0;i<Def_outport_num;i++)
//                {
//                    mapp_MHD1616STXWindowCfg(i+1,g_stWinConfigVar[i].u8SrcPort);
//                }
//                mapp_MHD1616STXFpgaWinCmdUpdate(10);
//                mapp_MHD1616STX_PlatformTimerSet(emUserTimer_StartupTime,500);
                sl_eStartupLoop++;
            }
            break;

        case eSt_None:
        default:
            break;
    }
}

void mapp_SendCmdToSwitchBD(u8 u8DevAddr,u8 u8Cmd,pu8 pu8Data, u16 u16Len)
{
    pstUartCom_t pUartCommVar = &g_stUart1CommVar;
    u8 l_u8SendValue[1100];
    u16 i=0;
    u16 l_u16index=0;

    memset(l_u8SendValue,0,1100);
    l_u8SendValue[l_u16index++] = 0xFF;
    l_u8SendValue[l_u16index++] = 0xFF;
    l_u8SendValue[l_u16index++] = 0xA5;
    l_u8SendValue[l_u16index++] = u8DevAddr;
    l_u8SendValue[l_u16index++] = u8Cmd;
    l_u8SendValue[l_u16index++] = u16Len&0xFF;
    l_u8SendValue[l_u16index++] = (u16Len>>8)&0xFF;
    for(i=0;i<u16Len;i++)
    {
        l_u8SendValue[l_u16index+i] = pu8Data[i];
    }

    pUartCommVar->SendBytes(l_u8SendValue,l_u16index+u16Len,FALSE);
//    {
//        pstUartCom_t pUartDbgVar = &g_stUart3CommVar;
//        pUartDbgVar->SendBytes(l_u8SendValue,l_u16index+u16Len,FALSE);
//    }
}

void mapp_MHD1616STXSendCmdToSWBD(u8 u8cmd,u8 *pu8Data, u16 u16Len)
{
    mapp_SendCmdToSwitchBD(g_u8BdAddr,u8cmd,pu8Data,u16Len);
}

void mapp_SourceChangeToSetWinProcess(void)
{
    u8 l_u8Src,l_u8OutIdex;
    pstProjectSpecVar_t         pstSpecVar = &stMHD1616STXSpecVar;
    pstBasicSpecVar_t           pstBasicSpecVar = &pstSpecVar->stBasSpecVar;

    if(!mapp_MHD1616STX_PlatformTimerExpired(emUserTimer_TimingChange))
        return;
    mapp_MHD1616STX_PlatformTimerSet(emUserTimer_TimingChange,500);

    for(l_u8Src = 0;l_u8Src<_HDMI_INPUT_PORT_USED_NUMBER;l_u8Src++)
    {
        if(g_stRxResTable[l_u8Src].bChangeFlag)
        {
            for(l_u8OutIdex = 0; l_u8OutIdex<Def_outport_num;l_u8OutIdex++)
            {
                if(pstBasicSpecVar->u8TxSrc[l_u8OutIdex] == (l_u8Src))
                {
                    mapp_MHD1616STXWindowCfg(l_u8OutIdex+1,(l_u8Src+1));
                }
            }

            mapp_MHD1616STXFpgaWinCmdUpdate(10);
            g_stRxResTable[l_u8Src].bChangeFlag = FALSE;
        }
    }
}


#define DM_FW_BYTES_MIX         (15*1024)

#define DM_FW_BYTES_MAX         (190*1024)  //100KB
#define DM_FW_DATA_ADDR         0x8033800
#define DM_FW_FLAG_ADDR         0x8063000
#define PAGE_SIZE               0x800

#define DM_FW_FLAG_INVALID      0x44332211
#define DM_FW_FLAG_VALID        0x11223344
#define DM_FW_SIZE_ADDR         (DM_FW_FLAG_ADDR+4)

typedef struct _stUpdateVar_t_
{
    u8 u8UpdateMode;
    u8 u8UpdateFlag;
    u32 u32TotalBytes;
    u32 u32PageIndex;
    u16 u16PackIndex;
    u8 u8LastData[1024];
}stUpdateVar_t;

stUpdateVar_t g_stMcuUpdateVar;

#if 0
uint8_t mcu_file_updata(uint8_t step ,uint8_t *data ,uint16_t len)
{
    uint16_t i,j=0;
    uint8_t f_sta;
    uint32_t* buff;
    image_header_t packet;

    static uint8_t LeftBytesTab[4]={0};
    static uint8_t LeftBytes=0;
//    static uint8_t frist_packet=1;

    switch(step)
    {
        case 0:
            dbg_printf("iap updata mode %d\r\n",g_stMcuUpdateVar.u8UpdateMode);
            dbg_printf("iap start Erase Sector\r\n");
            Flash_Unlocked();
            f_sta = Flash_Erase(DM_FW_DATA_ADDR);     /* 1Kbyte *3 */
            if(f_sta != 0)
            {
                Flash_Locked();
                return 0;
            }
            Flash_Locked();
            dbg_printf("Erase Sector finish\r\n");
            g_stMcuUpdateVar.u32TotalBytes = 0;
            g_stMcuUpdateVar.u8UpdateFlag=1;
            LeftBytes=0;
            memset(LeftBytesTab,0,4);
            mapp_MHD1616STX_PlatformTimerSet(emUserTimer_FirmwareUpdate,10000);
            g_stMcuUpdateVar.u32PageIndex = 0;
            break;
        case 1:
            g_stMcuUpdateVar.u16PackIndex++;

            if(g_stMcuUpdateVar.u32PageIndex<FW_VECT_OFFSET)
            {
                g_stMcuUpdateVar.u32PageIndex +=len;
                return 0;
            }
            else
                g_stMcuUpdateVar.u32PageIndex +=len;

            //printf("LeftBytes[%d]\r\n",LeftBytes);
            if(LeftBytes)
            {
                while(LeftBytes<=3)
                {
                    if(len>j)
                    {
                        LeftBytesTab[LeftBytes++] = *(data+j);
                    }
                    else
                    {
                        LeftBytesTab[LeftBytes++] = 0xFF;
                    }
                    j++;
                }

                Flash_Unlocked();
                Flash_WriteWord(DM_FW_DATA_ADDR+g_stMcuUpdateVar.u32TotalBytes,*(u32 *)LeftBytesTab);
                Flash_Locked();
                //printf("d=%08x\r\n",*(u32 *)LeftBytesTab);
                LeftBytes =0;

                g_stMcuUpdateVar.u32TotalBytes=g_stMcuUpdateVar.u32TotalBytes+4;

                if (len >j)
                {
                    /* update data pointer */
                    data = (data+j);
                    len = len -j;
                }
            }


            buff = (uint32_t*)(data);

            if((g_stMcuUpdateVar.u32TotalBytes  + len) > DM_FW_BYTES_MAX)
            {
               return 0;
            }
            Flash_Unlocked();
            for(i = 0; i < len/4; i++)
            {
                f_sta = Flash_WriteWord(DM_FW_DATA_ADDR+g_stMcuUpdateVar.u32TotalBytes+i*4,  *(buff+i));
                if(f_sta != 0)
                {
                    dbg_printf("E=%d\r\n",f_sta);
                    Flash_Erase(DM_FW_FLAG_ADDR);    //Parameter
                    Flash_WriteWord(DM_FW_FLAG_ADDR, DM_FW_FLAG_INVALID);
                    break;
                }
            }
            //??
            i=len%4;
            //dbg_printf("i[%d]\r\n",i);
            if (i>0)
            {
                    LeftBytes=0;
                    for(;i>0;i--)
                  {
                          LeftBytesTab[LeftBytes++] = *(data+ len-i);
                       // dbg_printf("%02X ",*(data+ len-i));
                  }

                    g_stMcuUpdateVar.u32TotalBytes +=len-i;
            }
            else
                g_stMcuUpdateVar.u32TotalBytes +=len;
            if(len<1024)
                memcpy(g_stMcuUpdateVar.u8LastData,data,len);

            Flash_Locked();
            //printf(".");
            break;
        case 2:
            Flash_Unlocked();

            Flash_Erase(DM_FW_FLAG_ADDR);  //Parameter

            g_stMcuUpdateVar.u32TotalBytes +=len;
            if(g_stMcuUpdateVar.u32TotalBytes>DM_FW_BYTES_MIX)
            {
                memcpy((uint8_t*)&packet,g_stMcuUpdateVar.u8LastData, sizeof(image_header_t));
                dbg_printf("iap end\r\n");
                dbg_printf("total_bytes [%d]\r\n",g_stMcuUpdateVar.u32TotalBytes );
                dbg_printf("ih_magic [0x%x]\r\n",packet.ih_magic );
                dbg_printf("ih_arch [%d]\r\n",packet.ih_arch );
                dbg_printf("ih_name [%s]\r\n",packet.ih_name );
                if((packet.ih_magic==0x4e545047)&&(packet.ih_arch==2)&&!strncmp((char*)packet.ih_name,_STRING_FACTORY_MODEL,strlen(_STRING_FACTORY_MODEL)))
                {
                    dbg_printf("data check ok\r\n");

                    f_sta =Flash_WriteWord(DM_FW_FLAG_ADDR, DM_FW_FLAG_VALID);
                    dbg_printf("f_sta=%d\r\n",f_sta);
                    Flash_WriteWord(DM_FW_SIZE_ADDR, g_stMcuUpdateVar.u32TotalBytes );
                }
//                else
//                {
//                    printf("data check fail\r\n");
//                }
            }
            else
            {
                  dbg_printf("iap err\r\n");
                  Flash_WriteWord(DM_FW_FLAG_ADDR, DM_FW_FLAG_INVALID);
            }
            Flash_Locked();
            g_stMcuUpdateVar.u8UpdateFlag=0;
            SYS_SoftReset();
            break;
    }
    return 1;
}
#else
int8_t FLASH_If_Erase(uint32_t StartSector)
{
  uint32_t FlashAddress;

  FlashAddress = StartSector;

  while (FlashAddress <   DM_FW_FLAG_ADDR)
  {
	if (FLASH_EraseOnePage(FlashAddress) == FLASH_COMPL)
	{
		FlashAddress += PAGE_SIZE;

			  IWDG_CntReload( 0xff0);
	}
	else
	{
		return -1;
	}
  }

  return 0;
}

uint8_t mcu_file_updata(uint8_t step ,uint8_t *data ,uint16_t len)
{
	uint16_t i,j=0;
	FLASH_STS f_sta;
	uint32_t* buff;
	image_header_t packet;

	static uint8_t LeftBytesTab[4]={0};
	static uint8_t LeftBytes=0;

	switch(step)
	{
		case 0:
//			dbg_printf("iap updata mode %d\r\n",sys_temp.mcu_updata_mode);
			dbg_printf("iap start Erase Sector\r\n");
			FLASH_Unlock();
			if(FLASH_If_Erase(DM_FW_DATA_ADDR) != 0)        /* 1Kbyte *3 */
			{
				FLASH_Lock();
				return 0;
			}
			FLASH_Lock();
			dbg_printf("Erase Sector finish\r\n");
			g_stMcuUpdateVar.u32TotalBytes = 0;
			g_stMcuUpdateVar.u8UpdateFlag=1;
			LeftBytes=0;
			memset(LeftBytesTab,0,4);
            g_stMcuUpdateVar.u32PageIndex = 0;
            mapp_MHD1616STX_PlatformTimerSet(emUserTimer_FirmwareUpdate,10000);
			break;
		case 1:
            g_stMcuUpdateVar.u16PackIndex++;

            if(g_stMcuUpdateVar.u32PageIndex<FW_VECT_OFFSET)
            {
                g_stMcuUpdateVar.u32PageIndex +=len;
                return 0;
            }
            else
                g_stMcuUpdateVar.u32PageIndex +=len;
			//dbg_printf("LeftBytes[%d]\r\n",LeftBytes);
			if(LeftBytes)
			{
				while(LeftBytes<=3)
				{
					if(len>j)
					{
						LeftBytesTab[LeftBytes++] = *(data+j);
					}
					else
					{
						LeftBytesTab[LeftBytes++] = 0xFF;
					}
					j++;
				}

				FLASH_Unlock();
				FLASH_ProgramWord(DM_FW_DATA_ADDR+g_stMcuUpdateVar.u32TotalBytes,*(u32 *)LeftBytesTab);
				FLASH_Lock();
				//dbg_printf("d=%08x\r\n",*(u32 *)LeftBytesTab);
				LeftBytes =0;

				g_stMcuUpdateVar.u32TotalBytes=g_stMcuUpdateVar.u32TotalBytes+4;

				if (len >j)
				{
					/* update data pointer */
					data = (data+j);
					len = len -j;
				}
			}


			buff = (uint32_t*)(data);

			if((g_stMcuUpdateVar.u32TotalBytes  + len) > DM_FW_BYTES_MAX)
			{
			   return 0;
			}
			FLASH_Unlock();
			for(i = 0; i < len/4; i++)
			{
				f_sta = FLASH_ProgramWord(DM_FW_DATA_ADDR+g_stMcuUpdateVar.u32TotalBytes+i*4,  *(buff+i));
				if(f_sta != FLASH_COMPL)
				{
					dbg_printf("E=%d\r\n",f_sta);
					FLASH_EraseOnePage(DM_FW_FLAG_ADDR);	//Parameter
					FLASH_ProgramWord(DM_FW_FLAG_ADDR, DM_FW_FLAG_INVALID);
					break;
				}
			}
			//??
			i=len%4;
			//dbg_printf("i[%d]\r\n",i);
			if (i>0)
			{
					LeftBytes=0;
					for(;i>0;i--)
				  {
						  LeftBytesTab[LeftBytes++] = *(data+ len-i);
					   // dbg_printf("%02X ",*(data+ len-i));
				  }

					g_stMcuUpdateVar.u32TotalBytes +=len-i;
			}
			else
				g_stMcuUpdateVar.u32TotalBytes +=len;
            if(len<1024)
                memcpy(g_stMcuUpdateVar.u8LastData,data,len);

			FLASH_Lock();
			//dbg_printf(".");
			break;
		case 2:
			FLASH_Unlock();


			FLASH_EraseOnePage(DM_FW_FLAG_ADDR);  //Parameter

			g_stMcuUpdateVar.u32TotalBytes +=len;
			if(g_stMcuUpdateVar.u32TotalBytes>DM_FW_BYTES_MIX)
			{
                memcpy((uint8_t*)&packet,g_stMcuUpdateVar.u8LastData, sizeof(image_header_t));
				dbg_printf("iap end\r\n");
				dbg_printf("total_bytes [%d]\r\n",g_stMcuUpdateVar.u32TotalBytes );
				dbg_printf("ih_magic [0x%x]\r\n",packet.ih_magic );
				dbg_printf("ih_arch [%d]\r\n",packet.ih_arch );
				dbg_printf("ih_name [%s]\r\n",packet.ih_name );
                if((packet.ih_magic==0x4e545047)&&(packet.ih_arch==2)&&!strncmp((char*)packet.ih_name,_STRING_FACTORY_MODEL,strlen(_STRING_FACTORY_MODEL)))
				{
					dbg_printf("data check ok\r\n");

					f_sta =FLASH_ProgramWord(DM_FW_FLAG_ADDR, DM_FW_FLAG_VALID);
					dbg_printf("f_sta=%d\r\n",f_sta);
					FLASH_ProgramWord(DM_FW_SIZE_ADDR, g_stMcuUpdateVar.u32TotalBytes );
				}
//				  else
//				  {
//					  dbg_printf("data check fail\r\n");
//				  }
			}
			else
			{
				  dbg_printf("iap err\r\n");
				  FLASH_ProgramWord(DM_FW_FLAG_ADDR, DM_FW_FLAG_INVALID);
			}
			FLASH_Lock();
			g_stMcuUpdateVar.u8UpdateFlag=0;
			RCC_EnableAPB1PeriphClk(RCC_APB1_PERIPH_WWDG,ENABLE);
			WWDG_Enable(0x40);
			break;
	}
	return 1;
}
#endif

u8 mapp_FirmwareUpdateProcess(pstUartCom_t pComVar)
{
    if(g_stMcuUpdateVar.u8UpdateFlag==1)
    {
        mapp_MHD1616STX_PlatformTimerSet(emUserTimer_FirmwareUpdate,5500);
        mcu_file_updata(1,(uint8_t*)pComVar->pRecBuffer,pComVar->u16RecCnt);

        pComVar->SendPrintf("pack %d\r\n",g_stMcuUpdateVar.u16PackIndex);
        dbg_printf("pack %d\r\n",g_stMcuUpdateVar.u16PackIndex);
        return 1;
    }
    else
        return 0;
}

void mapp_FirmwareUpdateTimeOutFunction(void)
{
    if(g_stMcuUpdateVar.u8UpdateFlag==1&&mapp_MHD1616STX_PlatformTimerExpired(emUserTimer_FirmwareUpdate))
    {
        mcu_file_updata(2,NULL,0);
    }
}

