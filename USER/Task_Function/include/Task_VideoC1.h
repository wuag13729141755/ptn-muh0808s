#ifndef __TASK_VIDEOC1_H_
#define __TASK_VIDEOC1_H_

#if (_ENABLE_PANASONIC_CHIP_MN86478X == _MACRO_ENABLE)

#ifdef DEBUG_MODE_EN
	#if 0//(_EN_UART_DEBUG_PRINTF == _MACRO_ENABLE)
	#define Msg_VideoC1(x)					do{Uart_printf x;}while(0)
	#else
	#define Msg_VideoC1(x)
	#endif
#else
#define Msg_VideoC1(x)
#endif

extern vpu8FuncPoint 	vpu8FuncProjectModuleCtrl;
extern uint8_t *		g_pu8ModuleCtrlParam;
extern bool 			g_bCecApplyEn;
extern vvFuncPoint      pvvFunc787UserFunctionInit;

void HAL_IncTick(void);
uint32_t HAL_GetTick(void);

void vTaskVideoC1(void *pvParameters);

#endif // #if (INC_TASK_VIDEOCHIP1 == _MACRO_ENABLE)

#endif




















