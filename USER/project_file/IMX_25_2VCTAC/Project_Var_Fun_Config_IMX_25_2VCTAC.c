#define __PROJECT_VAR_FUN_CONFIG_C__

#include "includes.h"

#if USER_FUNCTION_DEBUG
#define DBG_USER_FUN        Uart_printf
#else
#define DBG_USER_FUN(...)
#endif

u8 g_u8Gs29xxInitFlag = 1;
stProjectSpecVar_t  stIMX_25_2VCTACSpecVar;
static volatile u32 g_IMX_25_2VCTAC_Timers[emUserTimer_Max];
extern stProjectGlobalAllVar_t g_stPrjGloVar;

const uint8_t edid_IMX_25_2VCTAC_default[256] = // default edid
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

void GVXXRXInit(void);

//========================================================================================
// add by wf8421 20190820
//------------------------------------------------------------------------------
// Function:    mapp_MHD1616STX_PlatformTimerSet
// Description: 设置相关功能定时器时间
//------------------------------------------------------------------------------
void mapp_IMX_25_2VCTAC_PlatformTimerSet( uint8_t index, u32 m_sec )
{
    switch ( index )
    {
        default:
            if (index<emUserTimer_Max)
            {
                g_IMX_25_2VCTAC_Timers[index] = m_sec;
            }
            break;
    }
}

//------------------------------------------------------------------------------
// Function:    mapp_MHD1616STX_PlatformTimerGet
// Description: 获取相关功能定时器时间
//------------------------------------------------------------------------------
u32 mapp_IMX_25_2VCTAC_PlatformTimerGet(uint8_t eTimer)
{
    return(g_IMX_25_2VCTAC_Timers[eTimer]);
}

//------------------------------------------------------------------------------
// Function:    mapp_MHD1616STX_PlatformTimerInit
// Description: 定时器初始化
//------------------------------------------------------------------------------
void mapp_IMX_25_2VCTAC_PlatformTimerInit (void)
{
    int i;
    //initializer timer counters in array
    for ( i = 0; i < emUserTimer_Max; i++ )
    {
        g_IMX_25_2VCTAC_Timers[ i ] = 0;
    }

    //PlatformTimerSet(etKEY_Detect,5000);
}

//------------------------------------------------------------------------------
// Function:    mapp_MHD1616STX_PlatformTimerTask
// Description: 定时器递减函数
//------------------------------------------------------------------------------
void mapp_IMX_25_2VCTAC_PlatformTimerTask(void)
{
    int i;
    for(i=0;i<emUserTimer_Max;i++)
    {
        if(g_IMX_25_2VCTAC_Timers[i] > 0)
        {
            g_IMX_25_2VCTAC_Timers[i]--;
        }
    }
}

//------------------------------------------------------------------------------
// Function:    mapp_MHD1616STX_PlatformTimerExpired
// Description: 相关功能定时器时间是否为0
//------------------------------------------------------------------------------
uint8_t mapp_IMX_25_2VCTAC_PlatformTimerExpired ( uint8_t timer )           //定时时间到函数
{
    if ( timer < emUserTimer_Max )
    {
        return( g_IMX_25_2VCTAC_Timers[ timer ] == 0);              //定时时间到时返回1
    }

    return( 0 );                                           //定时时间还没到时返回0
}
//========================================================================================
void IMX_25_2VCTAC_1msTimerIrq(void)
{
    mapp_IMX_25_2VCTAC_PlatformTimerTask();
}

void IMX_25_2VCTAC_SpecificVarInit(void)
{
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    pstProjectSpecVar_t         pstSpecVar = &stIMX_25_2VCTACSpecVar;

    memset(pstSpecVar, 0, sizeof(stProjectSpecVar_t));
    pstPrjSaveVar->stSaveSpecVar.pvSpecVar = (void *)pstSpecVar;
    pstPrjSaveVar->stSaveSpecVar.u16SpecVarSize = sizeof(stProjectSpecVar_t);

}

void IMX_25_2VCTAC_SpecificVarDefault(void)
{
    IMX_25_2VCTAC_SpecBasicVarDefault();
    IMX_25_2VCTAC_SpecCustomEdidVarDefault();
}

void IMX_25_2VCTAC_SpecBasicVarDefault(void)
{
    pstProjectSpecVar_t         pstSpecVar = &stIMX_25_2VCTACSpecVar;

    pstSpecVar->stBasSpecVar.bPowerOnSta = TRUE;
    pstSpecVar->stBasSpecVar.bIsLock = FALSE;
}

void IMX_25_2VCTAC_SpecCustomEdidVarDefault(void)
{
    pstProjectSpecVar_t         pstSpecVar = &stIMX_25_2VCTACSpecVar;

    memcpy(pstSpecVar->stCustomEdidPre1.pu8EdidContext, System_Default_Edid_table, 256);
}

bool IMX_25_2VCTAC_SpecVarValidCheck(void)
{
    pstProjectSpecVar_t         pstSpecVar = &stIMX_25_2VCTACSpecVar;
    bool                        isSpecVarValid = TRUE;

    if(pstSpecVar->stBasSpecVar.u8CheckSum != CRC8Calculate((uint8_t *)&pstSpecVar->stBasSpecVar,((uint8_t *)&pstSpecVar->stBasSpecVar.u8CheckSum-(uint8_t *)&pstSpecVar->stBasSpecVar)))
    {
        IMX_25_2VCTAC_SpecBasicVarDefault();
        pstSpecVar->stBasSpecVar.u8CheckSum = CRC8Calculate((uint8_t *)&pstSpecVar->stBasSpecVar,((uint8_t *)&pstSpecVar->stBasSpecVar.u8CheckSum-(uint8_t *)&pstSpecVar->stBasSpecVar));

        isSpecVarValid = FALSE;
    }

    if(pstSpecVar->stCustomEdidPre1.u8CheckSum != CRC8Calculate((uint8_t *)&pstSpecVar->stCustomEdidPre1,((uint8_t *)&pstSpecVar->stCustomEdidPre1.u8CheckSum-(uint8_t *)&pstSpecVar->stCustomEdidPre1)))
    {
        memcpy(pstSpecVar->stCustomEdidPre1.pu8EdidContext, System_Default_Edid_table, 256);
        pstSpecVar->stCustomEdidPre1.u8CheckSum = CRC8Calculate((uint8_t *)&pstSpecVar->stCustomEdidPre1,((uint8_t *)&pstSpecVar->stCustomEdidPre1.u8CheckSum-(uint8_t *)&pstSpecVar->stCustomEdidPre1));

        isSpecVarValid = FALSE;
    }

    return isSpecVarValid;
}

void SetUserData_SpecBasicVar(void)
{
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    pstProjectSpecVar_t         pstSpecVar = &stIMX_25_2VCTACSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;

    pstBasicVar->u8CheckSum = CRC8Calculate((uint8_t *)pstBasicVar, ((uint8_t *)&pstBasicVar->u8CheckSum-(uint8_t *)pstBasicVar));
    SaveUserDataToEep(eEepDataIndex_SpecBlock1,\
        (ADDR_EEPROM_USER_START+((uint8_t *)&pstPrjSaveVar->stSaveSpecVar-(uint8_t *)pstPrjSaveVar)),\
        (uint8_t *)pstBasicVar, sizeof(stBasicSpecVar_t), TIME_SAVE_TO_EEPROM_IMMEDIATELY, FALSE);
}

void SetUserData_SpecCustomEdid(uint8_t u8CustomEdidIndex)
{
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    pstProjectSpecVar_t         pstSpecVar = &stIMX_25_2VCTACSpecVar;
    pstCustomEdid_t             pstEdidOp, pstEdidOrg = &pstSpecVar->stCustomEdidPre1;

    pstEdidOp = pstEdidOrg+u8CustomEdidIndex;
    pstEdidOp->u8CheckSum = CRC8Calculate((uint8_t *)pstEdidOp, ((uint8_t *)&pstEdidOp->u8CheckSum-(uint8_t *)pstEdidOp));
    SaveUserDataToEep((eEepDataIndex_t)(eEepDataIndex_SpecBlock2+u8CustomEdidIndex),\
        (ADDR_EEPROM_USER_START+((uint8_t *)&pstPrjSaveVar->stSaveSpecVar-(uint8_t *)pstPrjSaveVar)+((uint8_t *)pstEdidOp-(uint8_t *)pstSpecVar)),\
        (uint8_t *)pstEdidOp, sizeof(stCustomEdid_t), TIME_SAVE_TO_EEPROM_IMMEDIATELY, FALSE);
}

// the status when power on device
void IMX_25_2VCTAC_PrjStatePowerOnInit(void)
{
    System_Default_Edid_table = (iTE_u8 *)edid_IMX_25_2VCTAC_default;
    g_bEnableDbgFlag = TRUE;

    // prj 1ms loop hook
    pFuncPrj1MsLoopHook = IMX_25_2VCTAC_1msTimerIrq;
    app_SetGpioState(eGpioName_IT66121Reset1,0);
    delay_ms(10);
    app_SetGpioState(eGpioName_IT66121Reset1,1);
}

void IMX_25_2VCTAC_LedWork_Loop(void)
{
    static bool sl_bLedSta = FALSE;
    if(!mapp_IMX_25_2VCTAC_PlatformTimerExpired(emUserTimer_LedActive))
        return;
    mapp_IMX_25_2VCTAC_PlatformTimerSet(emUserTimer_LedActive,500);

    if(sl_bLedSta)
    {
        sl_bLedSta = FALSE;
        led_on(led_name_ActiveLed);
    }
    else
    {
        sl_bLedSta = TRUE;
        led_off(led_name_ActiveLed);
    }
}

// the status of led when running system
void IMX_25_2VCTAC_FuncWork_LoopRunning(void)
{
    IMX_25_2VCTAC_LedWork_Loop();
    if(g_u8Gs29xxInitFlag)
    {
        g_u8Gs29xxInitFlag = 0;
        GVXXRXInit();
    }
}

void mapp_IMX_25_2VCTACStartupMessage(void)
{
    RS232_printf("\r\n");
    RS232_printf("System Initializing...\r\n");
    RS232_printf("\r\n");
    RS232_printf("Initialization Finished!\r\n");
    RS232_printf("@%s %s",_STRING_FW_VERSION,_STRING_FW_VER_NUM);
}

void drv_delay(void)
{
  BYTE i =1;
  while(i<255)
  	i++;
}

WORD GVXX_ReadReg(WORD wAddr)
{
	BYTE i = 0;
	//BYTE  bTemp =0;
	WORD wReturn = 0;
	app_SetGpioState(eGpioName_GS2971Clk,0);
	app_SetGpioState(eGpioName_GS2971Cs,0);
	drv_delay();  // 5ns
	wAddr |=BIT15;  // Read
	for(i = 0; i< 16; i++)
	{
		//bTemp =(((wAddr & 0x8000)==0)? 0:1);
		if (((wAddr & BIT15)==0)? 1:0)
		{
			app_SetGpioState(eGpioName_GS2971Mosi,0);
		}
		else
		{
			app_SetGpioState(eGpioName_GS2971Mosi,1);
		}
		wAddr = wAddr << 1;
		app_SetGpioState(eGpioName_GS2971Clk,1);
		drv_delay();
		app_SetGpioState(eGpioName_GS2971Clk,0);
	}
	drv_delay(); // t5
	for(i = 0; i< 16; i++)
	{
		wReturn= wReturn<<1;
		app_SetGpioState(eGpioName_GS2971Clk,1);
		drv_delay();
		wReturn |= (app_GetGpioState(eGpioName_GS2971Miso)==0?0:1);
	    app_SetGpioState(eGpioName_GS2971Clk,0);
		drv_delay();
	}
	app_SetGpioState(eGpioName_GS2971Cs,1);
	return wReturn;

}



void GVXX_WritedReg(WORD wAddr,WORD wValue)
{
	BYTE i = 0;
	//WORD wReturn = 0;
	app_SetGpioState(eGpioName_GS2971Clk,0);
	app_SetGpioState(eGpioName_GS2971Cs,0);

	drv_delay();  // 5ns
	wAddr &=~BIT15;	// write
	for(i = 0; i< 16; i++)	// write addr
	{
		if (((wAddr & BIT15)==0)? 1:0)
		{
			app_SetGpioState(eGpioName_GS2971Mosi,0);
		}
		else
		{
			app_SetGpioState(eGpioName_GS2971Mosi,1);
		}

		wAddr = wAddr << 1;
		drv_delay();
		app_SetGpioState(eGpioName_GS2971Clk,1);
		drv_delay();
		app_SetGpioState(eGpioName_GS2971Clk,0);
	}
	drv_delay(); // t4  37.1ns
	for(i = 0; i< 16; i++)	// write value
	{
		if(((wValue & BIT15)==0)? 1:0)
		{
			app_SetGpioState(eGpioName_GS2971Mosi,0);
		}
		else
		{
			app_SetGpioState(eGpioName_GS2971Mosi,1);
		}
		drv_delay();
		wValue = wValue << 1;
		app_SetGpioState(eGpioName_GS2971Clk,1);
		drv_delay();
		app_SetGpioState(eGpioName_GS2971Clk,0);
	}
	app_SetGpioState(eGpioName_GS2971Cs,1);
}

void GVXXRXInit(void)
{
    u8 l_u8RegValue[2] = {0,0};

   app_SetGpioState(eGpioName_GS2972Reset,0);
   delay_ms(20);
   app_SetGpioState(eGpioName_GS2972Reset,1);
   delay_ms(20);

//   app_SetGpioState(eGpioName_GS2971Clk,0);//这里要先拉低，要不然操作7601的时序就会出错
#if 1
   l_u8RegValue[0] = 0x00;
   l_u8RegValue[1] = 0x01;
   drv_spi_bus_write(eSpiDevice_GS2971,0x039,2,l_u8RegValue,eSpiU16Addr);
   //配置音频输出为 标准IIS模式，24BIT
   delay_ms(20);
   l_u8RegValue[0] = 0x00;
   l_u8RegValue[1] = 0xF0;
   drv_spi_bus_write(eSpiDevice_GS2971,0x200,2,l_u8RegValue,eSpiU16Addr);
   delay_ms(20);
#endif
#if 0
  GVXX_WritedReg(0x039,0x0001);
  delay_ms(20);
  GVXX_WritedReg(0x200,0x00F0);
  delay_ms(20);
#endif


  // halAssertSDI_OUT_EN_Pin()
   //halReleaseSDI_OUT_EN_Pin();
/*	for(i = 0; i< 16; i++)
	{
		RegValue =  GVXX_ReadReg(i);
		printf(" GVreg %x = %x\n",i,RegValue);
	}
	*/

/*
  RegValue = GVXX_ReadReg(0x009);
  RegValue &= ~(BIT9|BIT8|BIT7|BIT6|BIT5);
  RegValue |= (BIT8|BIT6|BIT5);
  GVXX_WritedReg(0x009,RegValue);
 */
//    dbg_printf("GVXXRXInit\r\n");

}


