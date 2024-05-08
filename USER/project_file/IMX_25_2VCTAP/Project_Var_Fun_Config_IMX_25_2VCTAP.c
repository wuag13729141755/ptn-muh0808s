#define __PROJECT_VAR_FUN_CONFIG_C__

#include "includes.h"


stProjectSpecVar_t  stIMX_25_2VCTAPSpecVar;
static bool         sg_bChangePowerStateFlag = FALSE;

static volatile u32 g_IMX_25_2VCTAP_Timers[emUserTimer_Max];

extern stProjectGlobalAllVar_t g_stPrjGloVar;

void IMX_25_2VCTAP_1msTimerIrq(void);

//========================================================================================
// add by wf8421 20190820
//------------------------------------------------------------------------------
// Function:    mapp_MHD1616STX_PlatformTimerSet
// Description: 设置相关功能定时器时间
//------------------------------------------------------------------------------
void mapp_IMX_25_2VCTAP_PlatformTimerSet( uint8_t index, u32 m_sec )
{
    switch ( index )
    {
        default:
            if (index<emUserTimer_Max)
            {
                g_IMX_25_2VCTAP_Timers[index] = m_sec;
            }
            break;
    }
}

//------------------------------------------------------------------------------
// Function:    mapp_MHD1616STX_PlatformTimerGet
// Description: 获取相关功能定时器时间
//------------------------------------------------------------------------------
u32 mapp_IMX_25_2VCTAP_PlatformTimerGet(uint8_t eTimer)
{
    return(g_IMX_25_2VCTAP_Timers[eTimer]);
}

//------------------------------------------------------------------------------
// Function:    mapp_MHD1616STX_PlatformTimerInit
// Description: 定时器初始化
//------------------------------------------------------------------------------
void mapp_IMX_25_2VCTAP_PlatformTimerInit (void)
{
    int i;
    //initializer timer counters in array
    for ( i = 0; i < emUserTimer_Max; i++ )
    {
        g_IMX_25_2VCTAP_Timers[ i ] = 0;
    }

    //PlatformTimerSet(etKEY_Detect,5000);
}

//------------------------------------------------------------------------------
// Function:    mapp_MHD1616STX_PlatformTimerTask
// Description: 定时器递减函数
//------------------------------------------------------------------------------
void mapp_IMX_25_2VCTAP_PlatformTimerTask(void)
{
    int i;
    for(i=0;i<emUserTimer_Max;i++)
    {
        if(g_IMX_25_2VCTAP_Timers[i] > 0)
        {
            g_IMX_25_2VCTAP_Timers[i]--;
        }
    }
}

//------------------------------------------------------------------------------
// Function:    mapp_MHD1616STX_PlatformTimerExpired
// Description: 相关功能定时器时间是否为0
//------------------------------------------------------------------------------
uint8_t mapp_IMX_25_2VCTAP_PlatformTimerExpired ( uint8_t timer )           //定时时间到函数
{
    if ( timer < emUserTimer_Max )
    {
        return( g_IMX_25_2VCTAP_Timers[ timer ] == 0);              //定时时间到时返回1
    }

    return( 0 );                                           //定时时间还没到时返回0
}
//========================================================================================
void IMX_25_2VCTAP_1msTimerIrq(void)
{
    mapp_IMX_25_2VCTAP_PlatformTimerTask();
}

void IMX_25_2VCTAP_SpecificVarInit(void)
{
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    pstProjectSpecVar_t         pstSpecVar = &stIMX_25_2VCTAPSpecVar;

    memset(pstSpecVar, 0, sizeof(stProjectSpecVar_t));
    pstPrjSaveVar->stSaveSpecVar.pvSpecVar = (void *)pstSpecVar;
    pstPrjSaveVar->stSaveSpecVar.u16SpecVarSize = sizeof(stProjectSpecVar_t);

}

void IMX_25_2VCTAP_SpecificVarDefault(void)
{
    IMX_25_2VCTAP_SpecBasicVarDefault();
}

void IMX_25_2VCTAP_SpecBasicVarDefault(void)
{
    pstProjectSpecVar_t         pstSpecVar = &stIMX_25_2VCTAPSpecVar;
    u8 i,j;

    pstSpecVar->stBasSpecVar.bPowerOnSta = TRUE;
    pstSpecVar->stBasSpecVar.bIsLock = FALSE;
    pstSpecVar->stBasSpecVar.u8InRs232BaudrateID = baud_rate_9600;
    pstSpecVar->stBasSpecVar.u8OutRs232BaudrateID = baud_rate_9600;
    for(i=0;i<32;i++)
    {
        pstSpecVar->stBasSpecVar.stScene[i].u8SceneSaveFlag = 0;
        for(j=0;j<Def_outport_num;j++)
            pstSpecVar->stBasSpecVar.stScene[i].u8VideoCh[j] = 0;
    }
    for(i=0;i<Def_outport_num;i++)
    {
        pstSpecVar->stBasSpecVar.u8TxFormatSta[i] = emTxFormat_Hdmi;
        pstSpecVar->stBasSpecVar.u8TxResolutionID[i] = emRes_1920x1080_60;
        pstSpecVar->stBasSpecVar.u8AudioSource[i] = i+1;
        pstSpecVar->stBasSpecVar.u8TxHdcpMode[i] = 0;
    }
    pstSpecVar->stBasSpecVar.u8AudioSource[Def_outport_num] = Def_outport_num+1;
    pstSpecVar->stBasSpecVar.u8AudioSource[Def_outport_num+1] = Def_outport_num+2;
    pstSpecVar->stBasSpecVar.u8ExternalAudioMute[0] = 0;
    pstSpecVar->stBasSpecVar.u8ExternalAudioMute[1] = 0;
    pstSpecVar->stBasSpecVar.u8UartMode = 0;
    pstSpecVar->stBasSpecVar.stIPVar.u8IPAddr[0] = 192;
    pstSpecVar->stBasSpecVar.stIPVar.u8IPAddr[1] = 168;
    pstSpecVar->stBasSpecVar.stIPVar.u8IPAddr[2] = 0;
    pstSpecVar->stBasSpecVar.stIPVar.u8IPAddr[3] = 178;
    pstSpecVar->stBasSpecVar.stIPVar.u8NetMask[0] = 255;
    pstSpecVar->stBasSpecVar.stIPVar.u8NetMask[1] = 255;
    pstSpecVar->stBasSpecVar.stIPVar.u8NetMask[2] = 255;
    pstSpecVar->stBasSpecVar.stIPVar.u8NetMask[3] = 0;
    pstSpecVar->stBasSpecVar.stIPVar.u8NetGate[0] = 192;
    pstSpecVar->stBasSpecVar.stIPVar.u8NetGate[1] = 168;
    pstSpecVar->stBasSpecVar.stIPVar.u8NetGate[2] = 0;
    pstSpecVar->stBasSpecVar.stIPVar.u8NetGate[3] = 1;
    pstSpecVar->stBasSpecVar.stIPVar.u16SrcPort = 4001;
    pstSpecVar->stBasSpecVar.bIsDebugEn = FALSE;

}


bool IMX_25_2VCTAP_SpecVarValidCheck(void)
{
    pstProjectSpecVar_t         pstSpecVar = &stIMX_25_2VCTAPSpecVar;
    bool                        isSpecVarValid = TRUE;

    if(pstSpecVar->stBasSpecVar.u8CheckSum != CRC8Calculate((uint8_t *)&pstSpecVar->stBasSpecVar,((uint8_t *)&pstSpecVar->stBasSpecVar.u8CheckSum-(uint8_t *)&pstSpecVar->stBasSpecVar)))
    {
        IMX_25_2VCTAP_SpecBasicVarDefault();
        pstSpecVar->stBasSpecVar.u8CheckSum = CRC8Calculate((uint8_t *)&pstSpecVar->stBasSpecVar,((uint8_t *)&pstSpecVar->stBasSpecVar.u8CheckSum-(uint8_t *)&pstSpecVar->stBasSpecVar));

        isSpecVarValid = FALSE;
    }

    return isSpecVarValid;
}

void SetUserData_SpecBasicVar(void)
{
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    pstProjectSpecVar_t         pstSpecVar = &stIMX_25_2VCTAPSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;

    pstBasicVar->u8CheckSum = CRC8Calculate((uint8_t *)pstBasicVar, ((uint8_t *)&pstBasicVar->u8CheckSum-(uint8_t *)pstBasicVar));
    SaveUserDataToEep(eEepDataIndex_SpecBlock1,\
        (ADDR_EEPROM_USER_START+((uint8_t *)&pstPrjSaveVar->stSaveSpecVar-(uint8_t *)pstPrjSaveVar)),\
        (uint8_t *)pstBasicVar, sizeof(stBasicSpecVar_t), TIME_SAVE_TO_EEPROM_IMMEDIATELY, FALSE);
}

// the status when power on device
void IMX_25_2VCTAP_PrjStatePowerOnInit(void)
{
//    pstProjectSpecVar_t         pstSpecVar = &stIMX_25_2VCTAPSpecVar;
//    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;

    pFuncPrj1MsLoopHook = IMX_25_2VCTAP_1msTimerIrq;
    g_bEnableDbgFlag = true;//pstBasicVar->bIsDebugEn;
    mapp_RevisionMake();
}

void mapp_IMX_25_2VCTAPLedProcess(void)
{
    static bool sl_bLedSta = FALSE;
    if(mapp_IMX_25_2VCTAP_PlatformTimerExpired(emUserTimer_LedActive))
    {
        mapp_IMX_25_2VCTAP_PlatformTimerSet(emUserTimer_LedActive,500);
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
        {
            static u8 sl_u8Ledstar = led_name_Led1D7;

            led_off(sl_u8Ledstar);
            if(sl_u8Ledstar<led_name_LedCallD12)
                sl_u8Ledstar++;
            else
                sl_u8Ledstar = led_name_Led1D7;
            led_on(sl_u8Ledstar);
        }
    }
}

void IMX_25_2VCTAP_AllLedOff(void)
{
}

void mapp_IMX_25_2VCTAPMainFunction(void)
{
    mapp_IMX_25_2VCTAPLedProcess();
}


void mapp_IMX_25_2VCTAPStartupMessage(void)
{
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    pstProjectSpecVar_t         pstSpecVar = &stIMX_25_2VCTAPSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    pstUartCom_t pUarControltVar = &g_stUart4CommVar;

    Uart_printf("\r\n");
    if(pstPrjSaveVar->stPrjNameSet.m_u8Len != 0)
    {
        Uart_printf("product name:%s\r\n",pstPrjSaveVar->stPrjNameSet.sPrjName);
    }
    Uart_printf("ip %d.%d.%d.%d\r\n",pstBasicVar->stIPVar.u8IPAddr[0],
                                pstBasicVar->stIPVar.u8IPAddr[1],
                                pstBasicVar->stIPVar.u8IPAddr[2],
                                pstBasicVar->stIPVar.u8IPAddr[3]);
    Uart_printf("mask %d.%d.%d.%d\r\n",pstBasicVar->stIPVar.u8NetMask[0],
                                pstBasicVar->stIPVar.u8NetMask[1],
                                pstBasicVar->stIPVar.u8NetMask[2],
                                pstBasicVar->stIPVar.u8NetMask[3]);
    Uart_printf("gateway %d.%d.%d.%d\r\n",pstBasicVar->stIPVar.u8NetGate[0],
                                pstBasicVar->stIPVar.u8NetGate[1],
                                pstBasicVar->stIPVar.u8NetGate[2],
                                pstBasicVar->stIPVar.u8NetGate[3]);
    Uart_printf("%s\r\n",pstBasicVar->bIsLock==1?"lock":"unlock");
    Uart_printf("\r\n");
    if(pstPrjSaveVar->stPrjNameSet.m_u8Len != 0)
    {
        pUarControltVar->SendPrintf("product name:%s\r\n",pstPrjSaveVar->stPrjNameSet.sPrjName);
        delay_ms(50);
    }
    pUarControltVar->SendPrintf("ip %d.%d.%d.%d\r\n",pstBasicVar->stIPVar.u8IPAddr[0],
                                pstBasicVar->stIPVar.u8IPAddr[1],
                                pstBasicVar->stIPVar.u8IPAddr[2],
                                pstBasicVar->stIPVar.u8IPAddr[3]);
    delay_ms(50);
    pUarControltVar->SendPrintf("mask %d.%d.%d.%d\r\n",pstBasicVar->stIPVar.u8NetMask[0],
                                pstBasicVar->stIPVar.u8NetMask[1],
                                pstBasicVar->stIPVar.u8NetMask[2],
                                pstBasicVar->stIPVar.u8NetMask[3]);
    delay_ms(50);
    pUarControltVar->SendPrintf("gateway %d.%d.%d.%d\r\n",pstBasicVar->stIPVar.u8NetGate[0],
                                pstBasicVar->stIPVar.u8NetGate[1],
                                pstBasicVar->stIPVar.u8NetGate[2],
                                pstBasicVar->stIPVar.u8NetGate[3]);
    delay_ms(50);
    pUarControltVar->SendPrintf("%s\r\n",pstBasicVar->bIsLock==1?"lock":"unlock");
    delay_ms(50);
}

#define DM_FW_BYTES_MIX         (15*1024)

#define DM_FW_BYTES_MAX         (192*1024)  //100KB
#define DM_FW_DATA_ADDR         0x8040000
#define DM_FW_FLAG_ADDR         0x8070000
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

uint8_t UpdateFunction(uint8_t u8Id,uint8_t u8Step ,uint8_t *pu8Data ,uint16_t u16Len)
{
    uint8_t l_u8RetVal = 1;

    mapp_IMX_25_2VCTAP_PlatformTimerSet(emUserTimer_CardInfo,10000);
    g_stMcuUpdateVar.u8UpdateMode = u8Id;
    switch(u8Step)
    {
        case 0:
            {
                mapp_IMX_25_2VCTAP_PlatformTimerSet(emUserTimer_FirmwareUpdate,10000);
                g_stMcuUpdateVar.u8UpdateFlag=1;
                l_u8RetVal = mcu_file_updata(0,0,0);
                g_stMcuUpdateVar.u16PackIndex = 0;
            }
            break;
        case 1:
            {
                g_stMcuUpdateVar.u16PackIndex++;
                l_u8RetVal=mcu_file_updata(1,pu8Data,u16Len);
            }
            break;
        case 2:
            {
                l_u8RetVal=mcu_file_updata(2,pu8Data,u16Len);
                g_stMcuUpdateVar.u8UpdateFlag=0;
            }
            break;
        default:
            break;
    }

    return l_u8RetVal;
}

uint8_t mcu_file_updata(uint8_t step ,uint8_t *data ,uint16_t len)
{
    uint16_t i,j=0;
#ifdef GD32F4XX
    fmc_state_enum f_sta;
#elif defined(STM32F2XX_HD)
    FLASH_Status f_sta;
#endif
    uint32_t* buff;
    image_header_t packet;

    static uint8_t LeftBytesTab[4]={0};
    static uint8_t LeftBytes=0;
//    static uint8_t frist_packet=1;

    switch(step)
    {
        case 0:
//            dbg_printf("iap updata mode %d\r\n",g_stMcuUpdateVar.u8UpdateMode);
            dbg_printf("iap start Erase Sector\r\n");
        #ifdef GD32F4XX
            fmc_unlock();
            i=0;
            j=((GD32F4FLASH_GetFlashSector(DM_FW_FLAG_ADDR)>>3)-(GD32F4FLASH_GetFlashSector(DM_FW_DATA_ADDR)>>3));
            do{
                f_sta = fmc_sector_erase(GD32F4FLASH_GetFlashSector(DM_FW_DATA_ADDR)+(i<<3));
                if(f_sta != 0)
                {
                    fmc_lock();
                    return 0;
                }
                i++;
            }while(i>j);
            fmc_lock();

        #elif defined(STM32F2XX_HD)
            FLASH_Unlock();
            i=0;
            j=((STM32F4FLASH_GetFlashSector(DM_FW_FLAG_ADDR)>>3)-(STM32F4FLASH_GetFlashSector(DM_FW_DATA_ADDR)>>3));
            do{
                f_sta = FLASH_EraseSector(STM32F4FLASH_GetFlashSector(DM_FW_DATA_ADDR)+(i<<3), VoltageRange_3);  /* 128 Kbyte *3 */
                if(f_sta != FLASH_COMPLETE)
                {
                    dbg_printf("err f_sta=%d\r\n",f_sta);
                    FLASH_Lock();
                    return 0;
                }
            }while(i>j);
            FLASH_Lock();
        #endif
            dbg_printf("Erase Sector finish\r\n");
            g_stMcuUpdateVar.u32TotalBytes = 0;
//            g_stMcuUpdateVar.u8UpdateFlag=1;
            LeftBytes=0;
            memset(LeftBytesTab,0,4);
//            mapp_IMX_25_2VCTAP_PlatformTimerSet(emUserTimer_FirmwareUpdate,10000);
            g_stMcuUpdateVar.u32PageIndex = 0;
            break;
        case 1:
            if(g_stMcuUpdateVar.u32PageIndex<FW_VECT_OFFSET)
            {
                g_stMcuUpdateVar.u32PageIndex +=len;
                return 0;
            }
            else
                g_stMcuUpdateVar.u32PageIndex +=len;
#if 0
            if((g_stMcuUpdateVar.u8UpdateMode==2)&&(frist_packet))
            {
                frist_packet=0;
                if(len>64)
                {
                    for(i=0;i<64;i++)
                    {
                        dbg_printf("%c",*(data+i));
                    }

                    data = (data+64);
                    len=len-64;
                }
                else
                    return 0;

            }
#endif

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

            #ifdef GD32F4XX
                fmc_unlock();
                fmc_word_program(DM_FW_DATA_ADDR+g_stMcuUpdateVar.u32TotalBytes,*(u32 *)LeftBytesTab);
                fmc_lock();
            #elif defined(STM32F2XX_HD)
                FLASH_Unlock();
                FLASH_ProgramWord(DM_FW_DATA_ADDR+g_stMcuUpdateVar.u32TotalBytes,*(u32 *)LeftBytesTab);
                FLASH_Lock();
            #endif
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
        #ifdef GD32F4XX
            fmc_unlock();
            for(i = 0; i < len/4; i++)
            {
                f_sta = fmc_word_program(DM_FW_DATA_ADDR+g_stMcuUpdateVar.u32TotalBytes+i*4,  *(buff+i));
                if(f_sta != FMC_READY)
                {
                    dbg_printf("E=%d\r\n",f_sta);
                    fmc_sector_erase(GD32F4FLASH_GetFlashSector(DM_FW_FLAG_ADDR));    //Parameter
                    fmc_word_program(DM_FW_FLAG_ADDR, DM_FW_FLAG_INVALID);
                    break;
                }
            }
        #elif defined(STM32F2XX_HD)
            FLASH_Unlock();
            for(i = 0; i < len/4; i++)
            {

              f_sta = FLASH_ProgramWord(DM_FW_DATA_ADDR+g_stMcuUpdateVar.u32TotalBytes+i*4,  *(buff+i));
                if(f_sta != FLASH_COMPLETE)
                {
                    FLASH_EraseSector(STM32F4FLASH_GetFlashSector(DM_FW_FLAG_ADDR), VoltageRange_3);  //Parameter
                    FLASH_ProgramWord(DM_FW_FLAG_ADDR, DM_FW_FLAG_INVALID);
                }
            }
        #endif
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
        #ifdef GD32F4XX
            fmc_lock();
        #elif defined(STM32F2XX_HD)
            FLASH_Lock();
        #endif
            //printf(".");
            break;
        case 2:
        #ifdef GD32F4XX
            fmc_unlock();
            fmc_sector_erase(GD32F4FLASH_GetFlashSector(DM_FW_FLAG_ADDR));
        #elif defined(STM32F2XX_HD)
            FLASH_Unlock();
            FLASH_EraseSector(STM32F4FLASH_GetFlashSector(DM_FW_FLAG_ADDR), VoltageRange_3);
        #endif

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

                #ifdef GD32F4XX
                    f_sta =fmc_word_program(DM_FW_FLAG_ADDR, DM_FW_FLAG_VALID);
                    dbg_printf("f_sta=%d\r\n",f_sta);
                    fmc_word_program(DM_FW_SIZE_ADDR, g_stMcuUpdateVar.u32TotalBytes );
                #elif defined(STM32F2XX_HD)
                    f_sta =FLASH_ProgramWord(DM_FW_FLAG_ADDR, DM_FW_FLAG_VALID);
                    dbg_printf("f_sta=%d\r\n",f_sta);
                    FLASH_ProgramWord(DM_FW_SIZE_ADDR, g_stMcuUpdateVar.u32TotalBytes );
                #endif
                }
//                else
//                {
//                    printf("data check fail\r\n");
//                }
            }
            else
            {
                dbg_printf("iap err\r\n");
                #ifdef GD32F4XX
                fmc_word_program(DM_FW_FLAG_ADDR, DM_FW_FLAG_INVALID);
                #elif defined(STM32F2XX_HD)
                FLASH_ProgramWord(DM_FW_FLAG_ADDR, DM_FW_FLAG_INVALID);
                #endif
            }
            #ifdef GD32F4XX
            fmc_lock();
            #elif defined(STM32F2XX_HD)
            FLASH_Lock();
            #endif
            g_stMcuUpdateVar.u8UpdateFlag=0;
            SYS_SoftReset();
            break;
    }
    return 1;
}

u8 mapp_FirmwareUpdateProcess(pstUartCom_t pComVar)
{
    if(g_stMcuUpdateVar.u8UpdateFlag==1)
    {
        mapp_IMX_25_2VCTAP_PlatformTimerSet(emUserTimer_FirmwareUpdate,2000);
//        mcu_file_updata(1,(uint8_t*)pComVar->pRecBuffer,pComVar->u16RecCnt);
        UpdateFunction(g_stMcuUpdateVar.u8UpdateMode,1,(uint8_t*)pComVar->pRecBuffer,pComVar->u16RecCnt);

        pComVar->SendPrintf("pack %d\r\n",g_stMcuUpdateVar.u16PackIndex);
        return 1;
    }
    else
        return 0;
}

void mapp_FirmwareUpdateTimeOutFunction(void)
{
    if(g_stMcuUpdateVar.u8UpdateFlag==1&&mapp_IMX_25_2VCTAP_PlatformTimerExpired(emUserTimer_FirmwareUpdate))
    {
//        mcu_file_updata(2,NULL,0);
        UpdateFunction(g_stMcuUpdateVar.u8UpdateMode,2,NULL,0);
    }
}

