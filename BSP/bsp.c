/**
  *****************************************************************************
  *                                BSP函数
  *
  *                       (C) Copyright 2000-2020, ***
  *                             All Rights Reserved
  *****************************************************************************
  *
  * @File    : bsp.c
  * @By      : dengzhiqin
  * @Version : V1.0
  * @Date    : 2017 / 11 / 27
  *
  *****************************************************************************
**/
#include "includes.h"
#include "../USER/project_file/Project_File_Include_Config.C"

#if (defined(N32G45X))
GPIO_Module *STM32_GPIO_PORT[] = {GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOF, GPIOG};
#elif (defined(GD32F4XX))
uint32_t STM32_GPIO_PORT[] = {GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOF, GPIOG};
#else
GPIO_TypeDef *STM32_GPIO_PORT[] = {GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOF, GPIOG};
#endif

extern u8 const AllPrjDefEdidTabl;
extern const stIrWorkDeclare_t stProjectIrWorkTable;
extern const stUartDeclare_t stProjectUartWorkTable[];
extern const stCecDeclare_t stProjectCecWorkTable[];
#if _ENABLE_SYSTEM_RESET_DELAY_TIME
bool g_bSysResetFlag = FALSE;
#endif

#if (_EN_CHIP_PCA9539_USE == _MACRO_ENABLE)
extern uint16_t g_u16PcaPortConfig;
#endif

/**
  *****************************************************************************
  * @Name   : 初始化
  *
  * @Brief  : none
  *
  * @Input  : none
  *
  * @Output : none
  *
  * @Return : none
  *****************************************************************************
**/
void BSP_Init(void)
{
	/* 1. 初始化一个定时器中断，精度高于滴答定时器中断，这样才可以获得准确的系统信息 仅供调试目的，实际项
		  目中不要使用，因为这个功能比较影响系统实时性。
	   2. 为了正确获取FreeRTOS的调试信息，可以考虑将上面的关闭中断指令__set_PRIMASK(1); 注释掉。
	*/
#if 0//(_IR_FUNCTION_USE != _IR_FUNCTION_NONE)
	pstIrWorkDeclare_t 	pstIrWork = (pstIrWorkDeclare_t)&stProjectIrWorkTable;
#endif

	delay_init();

#if (defined(GD32F4XX))
    nvic_priority_group_set(NVIC_PRIGROUP_PRE4_SUB0);
#else
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
#endif
    #if (defined(STM32F2XX_HD)/*||defined(N32G45X)*/)
    SystemInit();
    #else
    //RCC_Configuration();
    #endif
	GPIO_Configuration();
	bsp_InitI2C();
    PlatformTimerInit();

#if 0//defined(N32G45X)
    sys_tick_init();
#endif
    drv_TimerFuncInit();

#if 0//(_IR_FUNCTION_USE != _IR_FUNCTION_NONE)
	if(pstIrWork->pstIrSenderPort != NULL)
	{
		TIMER3_Configuration();
	}
#endif

	drv_UartTaskInit();
#if _ENABLE_MAIN_TASK_ETHERNET
    ethnet_Init();
#endif

}

/**
*GPIO CONFIGURATION
**/
#if (defined(GD32F4XX))
void GPIO_Configuration(void)
{
	uint8_t i;

    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_GPIOC);
    rcu_periph_clock_enable(RCU_GPIOD);
    rcu_periph_clock_enable(RCU_GPIOE);
    rcu_periph_clock_enable(RCU_GPIOF);
//    rcu_periph_clock_enable(RCU_GPIOG);
//    rcu_periph_clock_enable(RCU_GPIOH);
//    rcu_periph_clock_enable(RCU_GPIOI);
    rcu_periph_clock_enable(RCU_SYSCFG);

#if(_IR_FUNCTION_USE != _IR_FUNCTION_NONE)
    #if (_IR_SENSOR_IN_PORT_USE == _IR_SENSOR_IN_GPIOB0)
        gpio_af_set(GPIOB, GPIO_AF_2, GPIO_PIN_0);
    #elif (_IR_SENSOR_IN_PORT_USE == _IR_SENSOR_IN_GPIOB8)
        gpio_af_set(GPIOB, GPIO_AF_2, GPIO_PIN_8);
    #elif (_IR_SENSOR_IN_PORT_USE == _IR_SENSOR_IN_GPIOB9)
        gpio_af_set(GPIOB, GPIO_AF_2, GPIO_PIN_9);
    #endif
#endif

	for(i=0; i<(sizeof(stProjectGpioTable)/sizeof(stGpioConfig_t)); i++)
	{
		pstGpioConfig_t pstGpio = (pstGpioConfig_t)&stProjectGpioTable[i];
		if((pstGpio->u8PortType>=PORT_GPIOA)&&(pstGpio->u8PortType<=PORT_GPIOG))
		{
            gpio_mode_set(STM32_GPIO_PORT[pstGpio->u8PortType-PORT_GPIOA],
                        pstGpio->eGpioMode,
                        pstGpio->eGpioPuPdType,
                        pstGpio->u16GpioPin);

			if(pstGpio->eGpioMode == GPIO_Mode_OUT)
			{
                gpio_output_options_set(STM32_GPIO_PORT[pstGpio->u8PortType-PORT_GPIOA],
                                pstGpio->eGpioOType,
                                GPIO_OSPEED_50MHZ,
                                pstGpio->u16GpioPin);
				if(pstGpio->eInitLevel == ePinLevel_High)
				{
					gpio_bit_write(STM32_GPIO_PORT[pstGpio->u8PortType-PORT_GPIOA], pstGpio->u16GpioPin,SET);
				}
				else
				{
					gpio_bit_write(STM32_GPIO_PORT[pstGpio->u8PortType-PORT_GPIOA], pstGpio->u16GpioPin,RESET);
				}
			}
		}
		#if (_EN_CHIP_PCA9539_USE == _MACRO_ENABLE)
		else if(pstGpio->u8PortType == PORT_PCA)
		{
			if(pstGpio->eGpioMode == GPIO_Mode_OUT)
			{
			    g_u16PcaPortConfig |= pstGpio->u16GpioPin;

				if(pstGpio->eInitLevel == ePinLevel_High)
				{
					 pca_output_val |= pstGpio->u16GpioPin;
				}
				else
				{
					pca_output_val &= (~pstGpio->u16GpioPin);
				}
			}
		}
		#endif
	}
}

#else
void GPIO_Configuration(void)
{
	uint8_t i;
#ifdef N32G45X
    GPIO_InitType    GPIO_InitStructure;
#else
	GPIO_InitTypeDef GPIO_InitStructure;
#endif

    #ifdef STM32F2XX_HD
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIO, ENABLE);
    #elif defined(N32G45X)
	RCC_EnableAPB2PeriphClk(_RCC_APB2Periph_GPIO, ENABLE);
    GPIO_ConfigPinRemap(GPIO_RMP_SW_JTAG_SW_ENABLE, ENABLE);
    #else
	RCC_APB2PeriphClockCmd(_RCC_APB2Periph_GPIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
    #endif


#if (defined(STM32F2XX_HD)&&(_IR_FUNCTION_USE != _IR_FUNCTION_NONE))
    #if (_IR_SENSOR_IN_PORT_USE == _IR_SENSOR_IN_GPIOB0)
        GPIO_PinAFConfig(GPIOB, GPIO_PinSource0, GPIO_AF_TIM3);
    #elif (_IR_SENSOR_IN_PORT_USE == _IR_SENSOR_IN_GPIOB8)
        GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_TIM4);
    #elif (_IR_SENSOR_IN_PORT_USE == _IR_SENSOR_IN_GPIOB9)
        GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_TIM4);
    #endif
#endif

	for(i=0; i<(sizeof(stProjectGpioTable)/sizeof(stGpioConfig_t)); i++)
	{
		pstGpioConfig_t pstGpio = (pstGpioConfig_t)&stProjectGpioTable[i];
		if((pstGpio->u8PortType>=PORT_GPIOA)&&(pstGpio->u8PortType<=PORT_GPIOG))
		{
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
            #ifdef N32G45X
			GPIO_InitStructure.Pin = pstGpio->u16GpioPin;
            #else
			GPIO_InitStructure.GPIO_Pin = pstGpio->u16GpioPin;
            #endif
			GPIO_InitStructure.GPIO_Mode = pstGpio->eGpioMode;
            #ifdef STM32F2XX_HD
			GPIO_InitStructure.GPIO_OType = pstGpio->eGpioOType;
			GPIO_InitStructure.GPIO_PuPd = pstGpio->eGpioPuPdType;
			#endif
            #ifdef N32G45X
            GPIO_InitPeripheral(STM32_GPIO_PORT[pstGpio->u8PortType-PORT_GPIOA], &GPIO_InitStructure);
            #else
			GPIO_Init(STM32_GPIO_PORT[pstGpio->u8PortType-PORT_GPIOA], &GPIO_InitStructure);
            #endif

            #ifdef STM32F2XX_HD
			if(pstGpio->eGpioMode == GPIO_Mode_OUT)
			#else
			if((pstGpio->eGpioMode==GPIO_Mode_Out_OD)||(pstGpio->eGpioMode==GPIO_Mode_Out_PP))
            #endif
			{
				if(pstGpio->eInitLevel == ePinLevel_High)
				{
					GPIO_SetBits(STM32_GPIO_PORT[pstGpio->u8PortType-PORT_GPIOA], pstGpio->u16GpioPin);
				}
				else
				{
					GPIO_ResetBits(STM32_GPIO_PORT[pstGpio->u8PortType-PORT_GPIOA], pstGpio->u16GpioPin);
				}
			}
		}
		#if (_EN_CHIP_PCA9539_USE == _MACRO_ENABLE)
		else if(pstGpio->u8PortType == PORT_PCA)
		{
            #ifdef STM32F2XX_HD
			if(pstGpio->eGpioMode == GPIO_Mode_OUT)
			#else
			if((pstGpio->eGpioMode==GPIO_Mode_Out_OD)||(pstGpio->eGpioMode==GPIO_Mode_Out_PP))
            #endif
			{
			    g_u16PcaPortConfig |= pstGpio->u16GpioPin;

				if(pstGpio->eInitLevel == ePinLevel_High)
				{
					 pca_output_val |= pstGpio->u16GpioPin;
				}
				else
				{
					pca_output_val &= (~pstGpio->u16GpioPin);
				}
			}
		}
		#endif
	}
}
#endif

#if (_EN_WATCH_DOG == _MACRO_ENABLE)
//watch dog
void IWDG_Configuration(void)
{
   /* IWDG timeout equal to 2000 ms (the timeout may varies due to LSI frequency
      dispersion) */
   /* Enable write access to IWDG_PR and IWDG_RLR registers */
   IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
   /* IWDG counter clock: 40KHz(LSI) / 32 = 1.25 KHz */
   IWDG_SetPrescaler(IWDG_Prescaler_32);
   /* Set counter reload value to 1250 */
   IWDG_SetReload(2500);     // 2s   //2S
   /* Reload IWDG counter */
   IWDG_ReloadCounter();
   /* Enable IWDG (the LSI oscillator will be enabled by hardware) */
   IWDG_Enable();
}

void WatchDogFeed(uint16_t interval)
{
	static uint16_t feedDogCnt = 0;

	feedDogCnt ++;
	if(feedDogCnt >= interval)
	{
		feedDogCnt = 0;
		IWDG_ReloadCounter();
	}
}
#endif

#ifndef StartupMessage
//#define StartupMessage 					NULL
#endif

void ManufLogoPrintf(void)
{

    #ifdef StartupMessage
    {
        StartupMessage();
    }
    #else
    {
    	pstProjectGlobalAllVar_t	pstPrjAllVar = &g_stPrjGloVar;
    	pstProjectGlobalSaveVar_t	pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;

    	RS232_printf("\r\n");
    	RS232_printf(_STRING_MANF_NAME);
        if(pstPrjSaveVar->stPrjNameSet.m_u8Len != 0)
        {
        	RS232_printf(pstPrjSaveVar->stPrjNameSet.sPrjName);
        	RS232_printf("\r\n");
        }
        else
        {
        	RS232_printf(_STRING_FACTORY_MODEL);
        	RS232_printf("\r\n");
        }
    	RS232_printf(_STRING_FW_VERSION);
    }
    #endif
}

#ifndef SaveSpecificVarInit
#define SaveSpecificVarInit					__NOP
#endif

void SysSaveVarInit(void)
{
	pstProjectGlobalAllVar_t	pstPrjAllVar = &g_stPrjGloVar;
	pstProjectGlobalSaveVar_t	pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;

	memset(pstPrjSaveVar, 0, sizeof(stProjectGlobalSaveVar_t));

	SaveSpecificVarInit();
}

#ifndef SaveSpecificVarDefault
#define SaveSpecificVarDefault          __NOP
#endif
void SpecificSaveVarDefault(void)
{
    #ifdef SaveSpecificVarDefault
    SaveSpecificVarDefault();
    #endif
}

#ifndef PrjStatePowerOnInit
#define PrjStatePowerOnInit				    __NOP
#endif

void SystemVarInit(void)
{
	SysSaveVarInit();
    #if _EN_MAIN_TASK_EEPROM
	EepromVarInit();
    #else
    app_UserDataToDefault();
    #endif
	PrjStatePowerOnInit();

	if(System_Default_Edid_table == NULL)
	{
	#if  _ENABLE_IT663XX_EDID_BITE
		System_Default_Edid_table = (u8 *)edid_internal_9;
        memcpy(g_pu8It663xxEdidBuffer, System_Default_Edid_table,256);

	#else
		System_Default_Edid_table = (u8 *)AllPrjDefEdidTabl;
        memcpy(g_pu8It663xxEdidBuffer, System_Default_Edid_table,256);
	#endif
        #if _ENABLE_EDID_SWITCH_A_SINGLE_CHANNEL
        {
            u8 i;
            for(i=0; i<_HDMI_INPUT_PORT_USED_NUMBER; i++)
            {
                FillIT663xxEdidDataForSingleChannel(System_Default_Edid_table,i);
            }
        }
        #endif
	}
    #if _ENABLE_SYSTEM_RESET_DELAY_TIME
    g_bSysResetFlag = FALSE;
    #endif
}

bool RetFalse(void)
{
	return FALSE;
}

#ifndef EepSpecVarInvalid
#define EepSpecVarInvalid				    RetFalse
#endif

bool CheckSaveSpecVarValid(void)
{
	return EepSpecVarInvalid();
}

#ifndef SwitcherSuspend_LoopRunning
#define SwitcherSuspend_LoopRunning			__NOP
#endif

void SwitcherSuspend_Running(void)
{
	SwitcherSuspend_LoopRunning();
}

#ifndef SwitcherPrj_LoopRunning
#define SwitcherPrj_LoopRunning				__NOP
#endif

void SwitcherOnly_Running(void)
{
	SwitcherPrj_LoopRunning();
}

#ifndef SpliterPrj_LoopRunning
#define SpliterPrj_LoopRunning				__NOP
#endif

void SpliterOnly_Running(void)
{
	SpliterPrj_LoopRunning();
}

#ifndef mapp_SetTypeCResetPinFunction
#define mapp_SetTypeCResetPinFunction       __NOP
#endif

void SetTypeCResetPin(void)
{
    mapp_SetTypeCResetPinFunction();
}

#if _ENABLE_SYSTEM_RESET_DELAY_TIME
void mapp_SetSysResetModeDelayTime(u32 u32msTime)
{
    PlatformTimerSet(etSysResetTimer,u32msTime);
    g_bSysResetFlag = TRUE;
}

bool mapp_GetSysResetMode(void)
{
    if(g_bSysResetFlag)
    {
        if(PlatformTimerExpired(etSysResetTimer))
        {
            return TRUE;
        }
    }
    return FALSE;
}
#endif

#if _ENABLE_SAVE_CURRENT_EDID
#ifndef mapp_EdidInit
#define mapp_EdidInit       __NOP
#endif

void mapp_StarupEdidInit(void)
{
    mapp_EdidInit();
}
#endif

#ifndef PrjTaskC1LoopFun
#define PrjTaskC1LoopFun			__NOP
#endif

void fCallTaskC1Loop(void)
{
	PrjTaskC1LoopFun();
}


