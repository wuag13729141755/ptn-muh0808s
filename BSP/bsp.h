/**
  *****************************************************************************
  *                                bspº¯Êý
  *
  *                       (C) Copyright 2000-2020, ***
  *                             All Rights Reserved
  *****************************************************************************
  *
  * @File    : bsp.h
  * @By      : dengzhiqin
  * @Version : V1.0
  * @Date    : 2017 / 11 / 27
  *
  *****************************************************************************
**/

#ifndef _bsp_h
#define _bsp_h


#if (_EN_UART_DEBUG_PRINTF==_MACRO_ENABLE)
#define Msg_SemC(x)				Uart_printf x
#else
#define Msg_SemC(x)
#endif

/**
  ****************************** Support C++ **********************************
**/
#ifdef __cplusplus
	extern "C"{
#endif


#include "../USER/project_file/Project_File_Include_Config.h"

#if (defined(N32G45X))
extern GPIO_Module *STM32_GPIO_PORT[];
#elif (defined(GD32F4XX))
extern uint32_t STM32_GPIO_PORT[];
#else
extern GPIO_TypeDef *STM32_GPIO_PORT[];
#endif

/**
  *****************************************************************************
**/
void BSP_Init(void);
#ifndef STM32F2XX_HD
void RCC_Configuration(void);
#endif

void GPIO_Configuration(void);
#if (_EN_WATCH_DOG == _MACRO_ENABLE)
void IWDG_Configuration(void);
void WatchDogFeed(uint16_t interval);
#endif
void SystemVarInit(void);
void ManufLogoPrintf(void);
void SwitcherSuspend_Running(void);
void SwitcherOnly_Running(void);
bool CheckSaveSpecVarValid(void);
void SpliterOnly_Running(void);
#if _ENABLE_SYSTEM_RESET_DELAY_TIME
void mapp_SetSysResetModeDelayTime(u32 u32msTime);
bool mapp_GetSysResetMode(void);
#endif
void SpecificSaveVarDefault(void);
void SetTypeCResetPin(void);
#if _ENABLE_SAVE_CURRENT_EDID
void mapp_StarupEdidInit(void);
#endif
void fCallTaskC1Loop(void);

/**
  ****************************** Support C++ **********************************
**/
#ifdef __cplusplus
    }
#endif
/**
  *****************************************************************************
**/


#endif  /* end bsp.h */
