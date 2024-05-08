#ifndef __TASK_IR_H_
#define __TASK_IR_H_

#define IR_WAVE_STUDY_BIT_CNT_MAX				300

#define RM_PRO_NEC_LEADER_START_LOW				9000
#define RM_PRO_NEC_LEADER_START_HIGH			4500
#define RM_PRO_NEC_REPEAT_START_LOW				9000
#define RM_PRO_NEC_REPEAT_START_HIGH			2250

#define RM_PRO_NEC_LEADER_START					(RM_PRO_NEC_LEADER_START_HIGH+RM_PRO_NEC_LEADER_START_LOW)
#define RM_PRO_NEC_REPEAT_START					(RM_PRO_NEC_REPEAT_START_HIGH+RM_PRO_NEC_REPEAT_START_LOW)
#define RM_PRO_NEC_LOGIC_ONE					2250
#define RM_PRO_NEC_LOGIC_ZERO					1120

#define RM_PRO_NEC_THRESHOLD					400

#define RmValMin(x)								(x-RM_PRO_NEC_THRESHOLD)
#define RmValMax(x)								(x+RM_PRO_NEC_THRESHOLD)

#ifndef TIMER_CLK
#define TIMER_CLK                               (50)    //unit 1us
#endif

#if (defined(GD32F4XX))
#if (_IR_SENSOR_IN_PORT_USE == _IR_SENSOR_IN_GPIOB9)
#define IrSensor_IRQHandler					EXTI5_9_IRQHandler
#define IrSensor_IRQn						EXTI5_9_IRQn
#define IrSensor_Exti_Line					EXTI_9
#define IrSensor_PortSource					EXTI_SOURCE_GPIOB
#define IrSensor_PinSource					EXTI_SOURCE_PIN9
#define IrTimerXUse                         eSysHalTimer4
#define IrTimerXInputChannel                eSysHalTimerChannel4
#define IrTimerXUse_IRQHandler              TIMER4_IRQHandler

#elif (_IR_SENSOR_IN_PORT_USE == _IR_SENSOR_IN_GPIOB8)
#define IrSensor_IRQHandler					EXTI5_9_IRQHandler
#define IrSensor_IRQn						EXTI5_9_IRQn
#define IrSensor_Exti_Line					EXTI_8
#define IrSensor_PortSource					EXTI_SOURCE_GPIOB
#define IrSensor_PinSource					EXTI_SOURCE_PIN8
#define IrTimerXUse                         eSysHalTimer4
#define IrTimerXInputChannel                eSysHalTimerChannel3
#define IrTimerXUse_IRQHandler              TIMER4_IRQHandler

#elif (_IR_SENSOR_IN_PORT_USE == _IR_SENSOR_IN_GPIOB0)
#define IrSensor_IRQHandler					EXTI0_IRQHandler
#define IrSensor_IRQn						EXTI0_IRQn
#define IrSensor_Exti_Line					EXTI_0
#define IrSensor_PortSource					EXTI_SOURCE_GPIOB
#define IrSensor_PinSource					EXTI_SOURCE_PIN0
#define IrTimerXUse                         eSysHalTimer3
#define IrTimerXInputChannel                eSysHalTimerChannel3
#define IrTimerXUse_IRQHandler              TIMER3_IRQHandler
#elif (_IR_SENSOR_IN_PORT_USE ==_IR_SENSOR_IN_GPIOA0)
#define IrSensor_IRQHandler					EXTI0_IRQHandler
#define IrSensor_IRQn						EXTI0_IRQn
#define IrSensor_Exti_Line					EXTI_0
#define IrSensor_PortSource					EXTI_SOURCE_GPIOA
#define IrSensor_PinSource					EXTI_SOURCE_PIN0
#define IrTimerXUse                         eSysHalTimer3
#define IrTimerXInputChannel                eSysHalTimerChannel3
#define IrTimerXUse_IRQHandler              TIMER3_IRQHandler
#elif (_IR_SENSOR_IN_PORT_USE ==_IR_SENSOR_IN_GPIOA15)
#define IrSensor_IRQHandler					EXTI10_15_IRQHandler
#define IrSensor_IRQn						EXTI10_15_IRQn
#define IrSensor_Exti_Line					EXTI_15
#define IrSensor_PortSource					EXTI_SOURCE_GPIOA
#define IrSensor_PinSource					EXTI_SOURCE_PIN15
#define IrTimerXUse                         eSysHalTimer3
#define IrTimerXInputChannel                eSysHalTimerChannel3
#define IrTimerXUse_IRQHandler              TIMER3_IRQHandler
#endif

#else
#if (_IR_SENSOR_IN_PORT_USE == _IR_SENSOR_IN_GPIOB9)
#define IrSensor_IRQHandler					EXTI9_5_IRQHandler
#define IrSensor_IRQn						EXTI9_5_IRQn
#define IrSensor_Exti_Line					EXTI_Line9
#ifdef STM32F2XX_HD
#define IrSensor_PortSource					EXTI_PortSourceGPIOB
#define IrSensor_PinSource					EXTI_PinSource9
#else
#define IrSensor_PortSource					GPIO_PortSourceGPIOB
#define IrSensor_PinSource					GPIO_PinSource9
#endif
#define IrTimerXUse                         eSysHalTimer4
#define IrTimerXInputChannel                eSysHalTimerChannel4
#define IrTimerXUse_IRQHandler              TIM4_IRQHandler

#elif (_IR_SENSOR_IN_PORT_USE == _IR_SENSOR_IN_GPIOB8)
#define IrSensor_IRQHandler					EXTI9_5_IRQHandler
#define IrSensor_IRQn						EXTI9_5_IRQn
#define IrSensor_Exti_Line					EXTI_Line8
#ifdef STM32F2XX_HD
#define IrSensor_PortSource					EXTI_PortSourceGPIOB
#define IrSensor_PinSource					EXTI_PinSource8
#else
#define IrSensor_PortSource					GPIO_PortSourceGPIOB
#define IrSensor_PinSource					GPIO_PinSource8
#endif
#define IrTimerXUse                         eSysHalTimer4
#define IrTimerXInputChannel                eSysHalTimerChannel3
#define IrTimerXUse_IRQHandler              TIM4_IRQHandler

#elif (_IR_SENSOR_IN_PORT_USE == _IR_SENSOR_IN_GPIOB0)
#define IrSensor_IRQHandler					EXTI0_IRQHandler
#define IrSensor_IRQn						EXTI0_IRQn
#define IrSensor_Exti_Line					EXTI_Line0
#ifdef STM32F2XX_HD
#define IrSensor_PortSource					EXTI_PortSourceGPIOB
#define IrSensor_PinSource					EXTI_PinSource0
#else
#define IrSensor_PortSource					GPIO_PortSourceGPIOB
#define IrSensor_PinSource					GPIO_PinSource0
#endif
#define IrTimerXUse                         eSysHalTimer3
#define IrTimerXInputChannel                eSysHalTimerChannel3
#define IrTimerXUse_IRQHandler              TIM3_IRQHandler
#elif (_IR_SENSOR_IN_PORT_USE ==_IR_SENSOR_IN_GPIOA0)
#define IrSensor_IRQHandler					EXTI0_IRQHandler
#define IrSensor_IRQn						EXTI0_IRQn
#define IrSensor_Exti_Line					EXTI_Line0
#ifdef STM32F2XX_HD
#define IrSensor_PortSource					EXTI_PortSourceGPIOA
#define IrSensor_PinSource					EXTI_PinSource0
#else
#define IrSensor_PortSource					GPIO_PortSourceGPIOA
#define IrSensor_PinSource					GPIO_PinSource0
#endif
#define IrTimerXUse                         eSysHalTimer3
#define IrTimerXInputChannel                eSysHalTimerChannel3
#define IrTimerXUse_IRQHandler              TIM3_IRQHandler
#elif (_IR_SENSOR_IN_PORT_USE ==_IR_SENSOR_IN_GPIOA15)
#define IrSensor_IRQHandler					EXTI15_10_IRQHandler
#define IrSensor_IRQn						EXTI15_10_IRQn
#define IrSensor_Exti_Line					EXTI_Line15
#ifdef STM32F2XX_HD
#define IrSensor_PortSource					EXTI_PortSourceGPIOA
#define IrSensor_PinSource					EXTI_PinSource15
#else
#define IrSensor_PortSource					GPIO_PortSourceGPIOA
#define IrSensor_PinSource					GPIO_PinSource15
#endif
#define IrTimerXUse                         eSysHalTimer3
#define IrTimerXInputChannel                eSysHalTimerChannel3
#define IrTimerXUse_IRQHandler              TIM3_IRQHandler
#elif (_IR_SENSOR_IN_PORT_USE ==_IR_SENSOR_IN_GPIOA5)
#define IrSensor_IRQHandler					EXTI9_5_IRQHandler
#define IrSensor_IRQn						EXTI9_5_IRQn
#define IrSensor_Exti_Line					EXTI_Line5
#ifdef STM32F2XX_HD
#define IrSensor_PortSource					EXTI_PortSourceGPIOA
#define IrSensor_PinSource					EXTI_PinSource5
#else
#define IrSensor_PortSource					GPIO_PortSourceGPIOA
#define IrSensor_PinSource					GPIO_PinSource5
#endif
#define IrTimerXUse                         eSysHalTimer3
#define IrTimerXInputChannel                eSysHalTimerChannel3
#define IrTimerXUse_IRQHandler              TIM3_IRQHandler
#elif (_IR_SENSOR_IN_PORT_USE ==_IR_SENSOR_IN_GPIOA1)
#define IrSensor_IRQHandler					EXTI1_IRQHandler
#define IrSensor_IRQn						EXTI1_IRQn
#define IrSensor_Exti_Line					EXTI_Line1
#ifdef STM32F2XX_HD
#define IrSensor_PortSource					EXTI_PortSourceGPIOA
#define IrSensor_PinSource					EXTI_PinSource1
#else
#define IrSensor_PortSource					GPIO_PortSourceGPIOA
#define IrSensor_PinSource					GPIO_PinSource1
#endif
#define IrTimerXUse                         eSysHalTimer2
#define IrTimerXInputChannel                eSysHalTimerChannel2
#define IrTimerXUse_IRQHandler              TIM2_IRQHandler

#endif
#endif

typedef enum _eSysHalTimer_t_
{
	eSysHalTimer1 = 0,
	eSysHalTimer2,
	eSysHalTimer3,
	eSysHalTimer4,
    eSysHalTimer5,
    //eSysHalTimer6,
    //eSysHalTimer7,
    eSysHalTimer8,
}eSysHalTimer_t;

typedef enum _eSysHalTimerChannel_t_
{
	eSysHalTimerChannel1=0,
	eSysHalTimerChannel2,
	eSysHalTimerChannel3,
	eSysHalTimerChannel4,
}eSysHalTimerChannel_t;

typedef enum _eIrWorkMode_t_
{
	eIrWorkMode_None = 0,
	eIrWorkMode_Study,
	eIrWorkMode_Send,

    eIrWorkMode_StudyOk,    //add by wf8421 20220927

	eIrWorkMode_Max,
}eIrWorkMode_t;

typedef enum _ir_study_key_type_t_
{
	ir_study_key_type_none = 0,
	ir_study_key_type_dis_on,
	ir_study_key_type_dis_off,

	ir_study_key_type_max,
}ir_study_key_type_t;

typedef struct _stIrStudy_t_
{
	eIrWorkMode_t		eIrWorkMode;
	uint8_t				u8StudyKeyIndex;
	bool				bIsFirstEnterIrStudy;
	volatile uint32_t	vu32StudyIdleCnt;
	uint16_t			u16BlinkLedCnt;
}stIrStudy_t, *pstIrStudy_t;

typedef __packed struct _stIrKeyWave_t_
{
	uint16_t 	u16KeyWave[IR_WAVE_STUDY_BIT_CNT_MAX];
	uint8_t  	u8CheckSum;
	uint16_t 	u16BitCnt;
}stIrKeyWave_t, *pstIrKeyWave_t;

typedef struct _stIrCloneGoing_t_
{
	eIrWorkMode_t 		eIrCloneMode;
	uint8_t 			u8IrKeyIndex;
	pstIrKeyWave_t 		pKeyCursor;//cursor for study mode
	uint16_t 			u16BitCnt;//bit cnt for study mode
	uint16_t 			u16SendModeBitCnt;//bit cnt for send mode
}stIrCloneGoing_t, *pstIrCloneGoing_t;

typedef struct _stIrDecode_t_
{
	uint8_t 			u8AddrLow;
	uint8_t				u8AddrHigh;
	uint8_t				u8CommandN;
	uint8_t				u8CommandE;

	bool 				bIsRepeat;
}stIrDecode_t, *pstIrDecode_t;


typedef struct _ir_timer_config_t
{
	uint32_t RCC_APBPeriphX;
#if (defined(N32G45X))
    TIM_Module* timerX;
#elif (defined(GD32F4XX))
    uint32_t timerX;
#else
	TIM_TypeDef* timerX;
#endif
	uint16_t TIM_ChannelX;
	uint8_t NVIC_IRQChannelX;
	uint16_t TIM_IT_CCX;
	void (*irDecode)(void);
#if (defined(N32G45X))
    void (*TIM_OCXPolarityConfig)(TIM_Module* TIMx, uint16_t TIM_OCPolarity);
#elif (defined(GD32F4XX))
    void (*TIM_OCXPolarityConfig)(uint32_t TIMx, uint16_t TIM_OCPolarity);
#else
	void (*TIM_OCXPolarityConfig)(TIM_TypeDef* TIMx, uint16_t TIM_OCPolarity);
#endif
    #ifdef STM32F2XX_HD
	uint32_t (*TIM_GetCaptureX)(TIM_TypeDef* TIMx);
    #elif (defined(N32G45X))
	uint16_t (*TIM_GetCaptureX)(TIM_Module* TIMx);
    #elif (defined(GD32F4XX))
	uint16_t (*TIM_GetCaptureX)(uint32_t TIMx);
    #else
	uint16_t (*TIM_GetCaptureX)(TIM_TypeDef* TIMx);
    #endif
}ir_timer_config_t;

#if _ENABLE_NEW_USER_KEY_DEFINE_FUNCTION
typedef void (*pIrKeyScan)(u16,u8,bool);
extern pIrKeyScan pmapp_IrKeyScanFunc;
#endif

#if ((_IR_FUNCTION_USE&_IR_FUNCTION_KEY_STUDY)==_IR_FUNCTION_KEY_STUDY)
extern stIrStudy_t g_tIrStudyVar;
#endif
extern vvFuncPoint pmapp_IrInitFunc;

#if (((_IR_FUNCTION_USE&_IR_FUNCTION_KEY_STUDY)==_IR_FUNCTION_KEY_STUDY)&&(ENABLE_IR_COUNTED_USE_TIMER_CLOCK_50US))
extern u16 g_u16IrLearnKeyData[IR_WAVE_STUDY_BIT_CNT_MAX];
extern u16 g_u16IrLearnCnt;
extern vvFuncPoint pmapp_IrKeyLearnOkFunc;
#endif

void vTaskIR(void *pvParameters);
#if ((_IR_FUNCTION_USE&_IR_FUNCTION_REMOTE)==_IR_FUNCTION_REMOTE)
void IrDecodeStateUpdate(void);
#endif

#if ((_IR_FUNCTION_USE&_IR_FUNCTION_REMOTE)==_IR_FUNCTION_REMOTE)
void IrRemoteParser(pstIrKeyWave_t pstWave);
#endif

#if ((_IR_FUNCTION_USE&_IR_FUNCTION_KEY_STUDY)==_IR_FUNCTION_KEY_STUDY)
void ir_work_status_assign(eIrWorkMode_t eIrWorkStu, uint8_t u8KeyIndex, bool bFromIsr);

#define IR_SEND_START(key) 			ir_work_status_assign(eIrWorkMode_Send, key, FALSE)
#define IR_STUDY_START(key) 		ir_work_status_assign(eIrWorkMode_Study, key, FALSE)
#define IR_NONE_START(key) 			ir_work_status_assign(eIrWorkMode_None, key, FALSE)

#define IR_SEND_START_ISR(key) 		ir_work_status_assign(eIrWorkMode_Send, key, TRUE)
#define IR_STUDY_START_ISR(key) 	ir_work_status_assign(eIrWorkMode_Study, key, TRUE)
#define IR_NONE_START_ISR(key) 		ir_work_status_assign(eIrWorkMode_None, key, TRUE)

void IrStudyStateUpdate(void);
void IrStudyDataSave(u8 u8Index,pstIrKeyWave_t pIrKeyVal, bool bIsr);
void SetIrLearnMode(eIrWorkMode_t eIrWorkMode);
eIrWorkMode_t GetIrLearnMode(void);
#endif

void Remote_Init(void);
#ifdef STM32F10X_MD
void TIMER4_Configuration(uint16_t u16Period,uint16_t u16Prescaler);
#else
void TIMER7_Configuration(uint16_t u16Period,uint16_t u16Prescaler);
#endif
//void TIMER4_Configuration(void);
#if ((_IR_FUNCTION_USE&_IR_FUNCTION_BYPASS)==_IR_FUNCTION_BYPASS)
void IrSendEnable38kOut(void);
void IrSendDisable38kOut(void);
#endif
#if (((_IR_FUNCTION_USE&_IR_FUNCTION_KEY_STUDY)==_IR_FUNCTION_KEY_STUDY)&&(ENABLE_IR_COUNTED_USE_TIMER_CLOCK_50US))
void mapp_SendCarrierFunction(void);
void mapp_IrSendIsr(void);
void mapp_IrSendDirect(u16 *pu16Wave, u16 u16Len);
#endif

#endif




























