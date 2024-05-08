#include "includes.h"

#if (_IR_FUNCTION_USE != _IR_FUNCTION_NONE)

stIrCloneGoing_t g_stIrCloneRx;//is ir clone mode or ir study mode
stIrKeyWave_t g_stIrIdleKeyData;//Idle Buffer

#if ((_IR_FUNCTION_USE&_IR_FUNCTION_REMOTE)==_IR_FUNCTION_REMOTE)
stIrDecode_t g_stIrDecodeVal;
#endif

#if ((_IR_FUNCTION_USE&_IR_FUNCTION_KEY_STUDY)==_IR_FUNCTION_KEY_STUDY)
extern QueueHandle_t xQueue_IrStudy;
stIrStudy_t g_tIrStudyVar;
extern stProjectGlobalAllVar_t g_stPrjGloVar;
#endif

#if ((_IR_FUNCTION_USE&_IR_FUNCTION_REMOTE)==_IR_FUNCTION_REMOTE)
extern QueueHandle_t xQueue_IrDecode;
#endif
#if ENABLE_IR_COUNTED_USE_TIMER_CLOCK_50US
typedef enum _eIrReceiveIndex_t_
{
    emIr_None = 0,
    emIr_Receive,
    emIr_End,
    emIr_Hight,
    emIr_Low,

    emIr_Max,
}eIrReceiveIndex_t;

static u8 sg_u8IrIndex = emIr_None;
#endif
extern const stIrWorkDeclare_t stProjectIrWorkTable;
extern const stKeyDeclare_t stProjectKeyTable[];
extern stProjectGlobalAllVar_t g_stPrjGloVar;

static ir_timer_config_t g_IrTimerVar;
static void mdrv_IrDecodeProcIrqCallback(void);
void IrSensorExtiEnable(void);
void IrSensorExtiDisable(void);
vvFuncPoint pmapp_IrInitFunc = NULL;
#if (((_IR_FUNCTION_USE&_IR_FUNCTION_KEY_STUDY)==_IR_FUNCTION_KEY_STUDY)&&(ENABLE_IR_COUNTED_USE_TIMER_CLOCK_50US))
static bool g_bCarrierFlag = FALSE;
static volatile u16  g_u16IrSendCnt = 0;
u16 g_u16IrLearnKeyData[IR_WAVE_STUDY_BIT_CNT_MAX];
u16 g_u16IrLearnCnt = 0;
vvFuncPoint pmapp_IrKeyLearnOkFunc = NULL;
void IrLearnedOkPorcess(void);
#endif

#if (defined(N32G45X))
static  void mdrv_IrRemoteNecTimeInit(eSysHalTimer_t etim, eSysHalTimerChannel_t echannel)
{
    NVIC_InitType           NVIC_InitStructure;
    EXTI_InitType           EXTI_InitStructure;
    TIM_TimeBaseInitType    TIM_TimeBaseStructure;
    #if !(ENABLE_IR_COUNTED_USE_TIMER_CLOCK_50US)
    TIM_ICInitType          TIM_ICInitStructure;
    #endif

    g_IrTimerVar.irDecode = mdrv_IrDecodeProcIrqCallback;
    switch(etim)
    {
        case eSysHalTimer1:
            g_IrTimerVar.RCC_APBPeriphX = RCC_APB2_PERIPH_TIM1;
            g_IrTimerVar.timerX = TIM1;
            g_IrTimerVar.NVIC_IRQChannelX=TIM1_UP_IRQn;
            RCC_EnableAPB2PeriphClk(g_IrTimerVar.RCC_APBPeriphX,ENABLE); //enable rcc of TIM3
            break;
        case eSysHalTimer2:
            g_IrTimerVar.RCC_APBPeriphX = RCC_APB1_PERIPH_TIM2;
            g_IrTimerVar.timerX = TIM2;
            g_IrTimerVar.NVIC_IRQChannelX=TIM2_IRQn;
            RCC_EnableAPB1PeriphClk(g_IrTimerVar.RCC_APBPeriphX,ENABLE); //enable rcc of TIM3
            break;
        case eSysHalTimer3:
            g_IrTimerVar.RCC_APBPeriphX = RCC_APB1_PERIPH_TIM3;
            g_IrTimerVar.timerX = TIM3;
            g_IrTimerVar.NVIC_IRQChannelX=TIM3_IRQn;
            RCC_EnableAPB1PeriphClk(g_IrTimerVar.RCC_APBPeriphX,ENABLE); //enable rcc of TIM
            break;
        case eSysHalTimer4:
            g_IrTimerVar.RCC_APBPeriphX = RCC_APB1_PERIPH_TIM4;
            g_IrTimerVar.timerX = TIM4;
            g_IrTimerVar.NVIC_IRQChannelX=TIM4_IRQn;
            RCC_EnableAPB1PeriphClk(g_IrTimerVar.RCC_APBPeriphX,ENABLE); //enable rcc of TIM
            break;
        case eSysHalTimer5:
            g_IrTimerVar.RCC_APBPeriphX = RCC_APB1_PERIPH_TIM5;
            g_IrTimerVar.timerX = TIM5;
            g_IrTimerVar.NVIC_IRQChannelX=TIM5_IRQn;
            RCC_EnableAPB1PeriphClk(g_IrTimerVar.RCC_APBPeriphX,ENABLE); //enable rcc of TIM
            break;
        /*case eSysHalTimer6:
            g_IrTimerVar.RCC_APBPeriphX = RCC_APB1Periph_TIM6;
            g_IrTimerVar.timerX = TIM6;
            g_IrTimerVar.NVIC_IRQChannelX=TIM6_IRQn;
            RCC_APB1PeriphClockCmd(g_IrTimerVar.RCC_APBPeriphX,ENABLE); //enable rcc of TIM
            break;
        case eSysHalTimer7:
            g_IrTimerVar.RCC_APBPeriphX = RCC_APB1Periph_TIM7;
            g_IrTimerVar.timerX = TIM7;
            g_IrTimerVar.NVIC_IRQChannelX=TIM7_IRQn;
            RCC_APB1PeriphClockCmd(g_IrTimerVar.RCC_APBPeriphX,ENABLE); //enable rcc of TIM
            break;
            */
        case eSysHalTimer8:
            g_IrTimerVar.RCC_APBPeriphX = RCC_APB2_PERIPH_TIM8;
            g_IrTimerVar.timerX = TIM8;
            #ifdef STM32F2XX_HD
            g_IrTimerVar.NVIC_IRQChannelX=TIM8_UP_TIM13_IRQn;
            #else
            g_IrTimerVar.NVIC_IRQChannelX=TIM8_UP_IRQn;
            #endif
            RCC_EnableAPB2PeriphClk(g_IrTimerVar.RCC_APBPeriphX,ENABLE); //enable rcc of TIM
            break;
    }

    switch(echannel)
    {
        case eSysHalTimerChannel1:
            g_IrTimerVar.TIM_ChannelX=TIM_Channel_1;
            g_IrTimerVar.TIM_IT_CCX = TIM_IT_CC1;
            g_IrTimerVar.TIM_OCXPolarityConfig=TIM_ConfigOc1Polarity;
            g_IrTimerVar.TIM_GetCaptureX=TIM_GetCap1;
            break;
        case eSysHalTimerChannel2:
            g_IrTimerVar.TIM_ChannelX=TIM_Channel_2;
            g_IrTimerVar.TIM_IT_CCX = TIM_IT_CC2;
            g_IrTimerVar.TIM_OCXPolarityConfig=TIM_ConfigOc2Polarity;
            g_IrTimerVar.TIM_GetCaptureX=TIM_GetCap2;
            break;
        case eSysHalTimerChannel3:
            g_IrTimerVar.TIM_ChannelX=TIM_Channel_3;
            g_IrTimerVar.TIM_IT_CCX = TIM_IT_CC3;
            g_IrTimerVar.TIM_OCXPolarityConfig=TIM_ConfigOc3Polarity;
            g_IrTimerVar.TIM_GetCaptureX=TIM_GetCap3;
            break;
        case eSysHalTimerChannel4:
            g_IrTimerVar.TIM_ChannelX=TIM_Channel_4;
            g_IrTimerVar.TIM_IT_CCX = TIM_IT_CC4;
            g_IrTimerVar.TIM_OCXPolarityConfig=TIM_ConfigOc4Polarity;
            g_IrTimerVar.TIM_GetCaptureX=TIM_GetCap4;
            break;
    }

    /* Enable the EXTIx global Interrupt (with higher priority) */
    NVIC_InitStructure.NVIC_IRQChannel = IrSensor_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* Connect EXTI Line */
    #ifdef STM32F2XX_HD
    SYSCFG_EXTILineConfig(IrSensor_PortSource, IrSensor_PinSource);
    #else
    GPIO_EXTILineConfig(IrSensor_PortSource, IrSensor_PinSource);
    #endif

    /* Configure CEC pin to generate an EXTI interrupt on falling edge */
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_InitStructure.EXTI_Line = IrSensor_Exti_Line;
    EXTI_Init(&EXTI_InitStructure);

#if ENABLE_IR_COUNTED_USE_TIMER_CLOCK_50US
    TIM_DeInit(g_IrTimerVar.timerX);
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    // 设置50us的定时器
    TIM_TimeBaseStructure.TIM_Period = TIMER_CLK;
    TIM_TimeBaseStructure.TIM_Prescaler = (SystemCoreClock/1000000-1);
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM向上计式
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(g_IrTimerVar.timerX, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx

    TIM_ClearFlag(g_IrTimerVar.timerX, TIM_FLAG_Update);
    /* Enable TIM2,TIM3 Update interrupt [TIM2溢出中断允许]*/
    TIM_ITConfig(g_IrTimerVar.timerX, TIM_IT_Update, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = g_IrTimerVar.NVIC_IRQChannelX; //TIMx中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; //先占优先级0级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; //从优先级3级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
    NVIC_Init(&NVIC_InitStructure); //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
#else
    TIM_DeInit(g_IrTimerVar.timerX);
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    #ifdef STM32F2XX_HD
    TIM_TimeBaseStructure.TIM_Period = 64000; //设定计数器自动重装值 最大64ms溢出
    TIM_TimeBaseStructure.TIM_Prescaler =(60-1); //预,1M的计数频率,1us加1.
    #else
    TIM_TimeBaseStructure.TIM_Period = 64000; //设定计数器自动重装值 最大64ms溢出
    TIM_TimeBaseStructure.TIM_Prescaler =(72-1); //预,1M的计数频率,1us加1.
    #endif
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM向上计式
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(g_IrTimerVar.timerX, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx

    TIM_ICInitStructure.TIM_Channel = g_IrTimerVar.TIM_ChannelX; // 选择输入端 IC2映射到TI5上
    #if 0//def STM32F2XX_HD
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;
    #else
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising; //上升沿捕获
    #endif
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1; //配置输入分频,不分频
    #if 0//def STM32F2XX_HD
    TIM_ICInitStructure.TIM_ICFilter = 0x08;//IC4F=0011 配置输入滤波器 8个定时器时钟周期滤波
    #else
    TIM_ICInitStructure.TIM_ICFilter = 0x03;//IC4F=0011 配置输入滤波器 8个定时器时钟周期滤波
    #endif
    TIM_ICInit(g_IrTimerVar.timerX, &TIM_ICInitStructure);//初始化定时器输入捕获通道

    NVIC_InitStructure.NVIC_IRQChannel = g_IrTimerVar.NVIC_IRQChannelX; //TIMx中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; //先占优先级0级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
    NVIC_Init(&NVIC_InitStructure); //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
    /* Clear TIMx update pending flag */
    TIM_ClearFlag(g_IrTimerVar.timerX, TIM_FLAG_Update);
    TIM_ITConfig( g_IrTimerVar.timerX,TIM_IT_Update|g_IrTimerVar.TIM_IT_CCX,ENABLE);//允许更新中断 ,允许CC2IE捕获中断
#endif
    TIM_Cmd(g_IrTimerVar.timerX, DISABLE); //使能定时器5
}
#elif (defined(GD32F4XX))
static  void mdrv_IrRemoteNecTimeInit(eSysHalTimer_t etim, eSysHalTimerChannel_t echannel)
{
    NVIC_InitTypeDef            NVIC_InitStructure;
    EXTI_InitTypeDef            EXTI_InitStructure;
    TIM_TimeBaseInitTypeDef     TIM_TimeBaseStructure;
    timer_oc_parameter_struct   timer_ocintpara;
    timer_parameter_struct      timer_initpara;

    g_IrTimerVar.irDecode = mdrv_IrDecodeProcIrqCallback;
    switch(etim)
    {
        case eSysHalTimer1:
            g_IrTimerVar.timerX = TIMER1;
            g_IrTimerVar.NVIC_IRQChannelX=TIMER1_IRQn;
            rcu_periph_clock_enable(RCU_TIMER1);
            break;
        case eSysHalTimer2:
            g_IrTimerVar.timerX = TIMER2;
            g_IrTimerVar.NVIC_IRQChannelX=TIMER2_IRQn;
            rcu_periph_clock_enable(RCU_TIMER2);
            break;
        case eSysHalTimer3:
            g_IrTimerVar.timerX = TIMER3;
            g_IrTimerVar.NVIC_IRQChannelX=TIMER3_IRQn;
            rcu_periph_clock_enable(RCU_TIMER3);
            break;
        case eSysHalTimer4:
            g_IrTimerVar.timerX = TIMER4;
            g_IrTimerVar.NVIC_IRQChannelX=TIMER4_IRQn;
            rcu_periph_clock_enable(RCU_TIMER4);
            break;
        case eSysHalTimer5:
            g_IrTimerVar.timerX = TIMER5;
            g_IrTimerVar.NVIC_IRQChannelX=TIMER5_DAC_IRQn;
            rcu_periph_clock_enable(RCU_TIMER5);
            break;
        /*case eSysHalTimer6:
            g_IrTimerVar.timerX = TIMER6;
            g_IrTimerVar.NVIC_IRQChannelX=TIMER6_IRQn;
            rcu_periph_clock_enable(RCU_TIMER6);
            break;
        case eSysHalTimer7:
            g_IrTimerVar.timerX = TIMER7;
            g_IrTimerVar.NVIC_IRQChannelX=TIMER7_UP_TIMER12_IRQn;
            rcu_periph_clock_enable(RCU_TIMER7);
            break;
            */
        case eSysHalTimer8:
            g_IrTimerVar.timerX = TIMER8;
            g_IrTimerVar.NVIC_IRQChannelX=TIMER0_BRK_TIMER8_IRQn;
            rcu_periph_clock_enable(RCU_TIMER8);
            break;
    }

//    rcu_periph_clock_enable(KEY_CLK[key_num]);
//    rcu_periph_clock_enable(RCU_SYSCFG);

    /* Enable the EXTIx global Interrupt (with higher priority) */
    NVIC_InitStructure.NVIC_IRQChannel = IrSensor_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* Connect EXTI Line */
    #ifdef STM32F2XX_HD
    SYSCFG_EXTILineConfig(IrSensor_PortSource, IrSensor_PinSource);
    #else
    GPIO_EXTILineConfig(IrSensor_PortSource, IrSensor_PinSource);
    #endif

    nvic_irq_enable(IrSensor_IRQn, 2U, 0U);

    /* connect key EXTI line to key GPIO pin */
    syscfg_exti_line_config(IrSensor_PortSource, IrSensor_PinSource);

    /* configure key EXTI line */
    exti_init(IrSensor_Exti_Line, EXTI_INTERRUPT, EXTI_TRIG_FALLING);
    exti_interrupt_flag_clear(IrSensor_Exti_Line);

#if ENABLE_IR_COUNTED_USE_TIMER_CLOCK_50US
    rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4);
    timer_deinit(g_IrTimerVar.timerX);

    timer_initpara.prescaler         = TIMER_CLK;
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = (SystemCoreClock/1000000-1);
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    timer_init(g_IrTimerVar.timerX,&timer_initpara);

    /* TIMER6 channel control update interrupt enable */
    timer_interrupt_enable(g_IrTimerVar.timerX,TIMER_INT_UP);
    nvic_irq_enable(g_IrTimerVar.NVIC_IRQChannelX,2,0);
    timer_enable(g_IrTimerVar.timerX);

#endif
}

#else
static  void mdrv_IrRemoteNecTimeInit(eSysHalTimer_t etim, eSysHalTimerChannel_t echannel)
{
    NVIC_InitTypeDef            NVIC_InitStructure;
    EXTI_InitTypeDef            EXTI_InitStructure;
    TIM_TimeBaseInitTypeDef     TIM_TimeBaseStructure;
    #if !(ENABLE_IR_COUNTED_USE_TIMER_CLOCK_50US)
    TIM_ICInitTypeDef           TIM_ICInitStructure;
    #endif

    #ifdef STM32F2XX_HD
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);   // enable system config
    #endif

    g_IrTimerVar.irDecode = mdrv_IrDecodeProcIrqCallback;
    switch(etim)
    {
        case eSysHalTimer1:
            g_IrTimerVar.RCC_APBPeriphX = RCC_APB2Periph_TIM1;
            g_IrTimerVar.timerX = TIM1;
            #ifdef STM32F2XX_HD
            g_IrTimerVar.NVIC_IRQChannelX=TIM1_UP_TIM10_IRQn;
            #else
            g_IrTimerVar.NVIC_IRQChannelX=TIM1_UP_IRQn;
            #endif
            RCC_APB2PeriphClockCmd(g_IrTimerVar.RCC_APBPeriphX,ENABLE); //enable rcc of TIM3
            break;
        case eSysHalTimer2:
            g_IrTimerVar.RCC_APBPeriphX = RCC_APB1Periph_TIM2;
            g_IrTimerVar.timerX = TIM2;
            g_IrTimerVar.NVIC_IRQChannelX=TIM2_IRQn;
            RCC_APB1PeriphClockCmd(g_IrTimerVar.RCC_APBPeriphX,ENABLE); //enable rcc of TIM3
            break;
        case eSysHalTimer3:
            g_IrTimerVar.RCC_APBPeriphX = RCC_APB1Periph_TIM3;
            g_IrTimerVar.timerX = TIM3;
            g_IrTimerVar.NVIC_IRQChannelX=TIM3_IRQn;
            RCC_APB1PeriphClockCmd(g_IrTimerVar.RCC_APBPeriphX,ENABLE); //enable rcc of TIM
            break;
#ifndef STM32F10X_MD
        case eSysHalTimer4:
            g_IrTimerVar.RCC_APBPeriphX = RCC_APB1Periph_TIM4;
            g_IrTimerVar.timerX = TIM4;
            g_IrTimerVar.NVIC_IRQChannelX=TIM4_IRQn;
            RCC_APB1PeriphClockCmd(g_IrTimerVar.RCC_APBPeriphX,ENABLE); //enable rcc of TIM
            break;
        case eSysHalTimer5:
            g_IrTimerVar.RCC_APBPeriphX = RCC_APB1Periph_TIM5;
            g_IrTimerVar.timerX = TIM5;
            g_IrTimerVar.NVIC_IRQChannelX=TIM5_IRQn;
            RCC_APB1PeriphClockCmd(g_IrTimerVar.RCC_APBPeriphX,ENABLE); //enable rcc of TIM
            break;
        /*case eSysHalTimer6:
            g_IrTimerVar.RCC_APBPeriphX = RCC_APB1Periph_TIM6;
            g_IrTimerVar.timerX = TIM6;
            g_IrTimerVar.NVIC_IRQChannelX=TIM6_IRQn;
            RCC_APB1PeriphClockCmd(g_IrTimerVar.RCC_APBPeriphX,ENABLE); //enable rcc of TIM
            break;
        case eSysHalTimer7:
            g_IrTimerVar.RCC_APBPeriphX = RCC_APB1Periph_TIM7;
            g_IrTimerVar.timerX = TIM7;
            g_IrTimerVar.NVIC_IRQChannelX=TIM7_IRQn;
            RCC_APB1PeriphClockCmd(g_IrTimerVar.RCC_APBPeriphX,ENABLE); //enable rcc of TIM
            break;
            */
        case eSysHalTimer8:
            g_IrTimerVar.RCC_APBPeriphX = RCC_APB2Periph_TIM8;
            g_IrTimerVar.timerX = TIM8;
            #ifdef STM32F2XX_HD
            g_IrTimerVar.NVIC_IRQChannelX=TIM8_UP_TIM13_IRQn;
            #else
            g_IrTimerVar.NVIC_IRQChannelX=TIM8_UP_IRQn;
            #endif
            RCC_APB2PeriphClockCmd(g_IrTimerVar.RCC_APBPeriphX,ENABLE); //enable rcc of TIM
            break;
#endif
    }

    switch(echannel)
    {
        case eSysHalTimerChannel1:
            g_IrTimerVar.TIM_ChannelX=TIM_Channel_1;
            g_IrTimerVar.TIM_IT_CCX = TIM_IT_CC1;
            g_IrTimerVar.TIM_OCXPolarityConfig=TIM_OC1PolarityConfig;
            g_IrTimerVar.TIM_GetCaptureX=TIM_GetCapture1;
            break;
        case eSysHalTimerChannel2:
            g_IrTimerVar.TIM_ChannelX=TIM_Channel_2;
            g_IrTimerVar.TIM_IT_CCX = TIM_IT_CC2;
            g_IrTimerVar.TIM_OCXPolarityConfig=TIM_OC2PolarityConfig;
            g_IrTimerVar.TIM_GetCaptureX=TIM_GetCapture2;
            break;
        case eSysHalTimerChannel3:
            g_IrTimerVar.TIM_ChannelX=TIM_Channel_3;
            g_IrTimerVar.TIM_IT_CCX = TIM_IT_CC3;
            g_IrTimerVar.TIM_OCXPolarityConfig=TIM_OC3PolarityConfig;
            g_IrTimerVar.TIM_GetCaptureX=TIM_GetCapture3;
            break;
        case eSysHalTimerChannel4:
            g_IrTimerVar.TIM_ChannelX=TIM_Channel_4;
            g_IrTimerVar.TIM_IT_CCX = TIM_IT_CC4;
            g_IrTimerVar.TIM_OCXPolarityConfig=TIM_OC4PolarityConfig;
            g_IrTimerVar.TIM_GetCaptureX=TIM_GetCapture4;
            break;
    }

    /* Enable the EXTIx global Interrupt (with higher priority) */
    NVIC_InitStructure.NVIC_IRQChannel = IrSensor_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* Connect EXTI Line */
    #ifdef STM32F2XX_HD
    SYSCFG_EXTILineConfig(IrSensor_PortSource, IrSensor_PinSource);
    #else
    GPIO_EXTILineConfig(IrSensor_PortSource, IrSensor_PinSource);
    #endif

    /* Configure CEC pin to generate an EXTI interrupt on falling edge */
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_InitStructure.EXTI_Line = IrSensor_Exti_Line;
    EXTI_Init(&EXTI_InitStructure);

#if ENABLE_IR_COUNTED_USE_TIMER_CLOCK_50US
    TIM_DeInit(g_IrTimerVar.timerX);
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    // 设置50us的定时器
    #ifdef STM32F2XX_HD
    TIM_TimeBaseStructure.TIM_Period = TIMER_CLK;
    TIM_TimeBaseStructure.TIM_Prescaler = (SystemCoreClock/1000000/2-1);
    #else
    TIM_TimeBaseStructure.TIM_Period = TIMER_CLK;
    TIM_TimeBaseStructure.TIM_Prescaler = (SystemCoreClock/1000000-1);//(72-1);
    #endif
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM向上计式
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(g_IrTimerVar.timerX, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx

    TIM_ClearFlag(g_IrTimerVar.timerX, TIM_FLAG_Update);
    /* Enable TIM2,TIM3 Update interrupt [TIM2溢出中断允许]*/
    TIM_ITConfig(g_IrTimerVar.timerX, TIM_IT_Update, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = g_IrTimerVar.NVIC_IRQChannelX; //TIMx中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; //先占优先级0级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; //从优先级3级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
    NVIC_Init(&NVIC_InitStructure); //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
#else
    TIM_DeInit(g_IrTimerVar.timerX);
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    #ifdef STM32F2XX_HD
    TIM_TimeBaseStructure.TIM_Period = 64000; //设定计数器自动重装值 最大64ms溢出
    TIM_TimeBaseStructure.TIM_Prescaler =(60-1); //预,1M的计数频率,1us加1.
    #else
    TIM_TimeBaseStructure.TIM_Period = 64000; //设定计数器自动重装值 最大64ms溢出
    TIM_TimeBaseStructure.TIM_Prescaler =(72-1); //预,1M的计数频率,1us加1.
    #endif
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM向上计式
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(g_IrTimerVar.timerX, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx

    TIM_ICInitStructure.TIM_Channel = g_IrTimerVar.TIM_ChannelX; // 选择输入端 IC2映射到TI5上
    #if 0//def STM32F2XX_HD
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;
    #else
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising; //上升沿捕获
    #endif
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1; //配置输入分频,不分频
    #if 0//def STM32F2XX_HD
    TIM_ICInitStructure.TIM_ICFilter = 0x08;//IC4F=0011 配置输入滤波器 8个定时器时钟周期滤波
    #else
    TIM_ICInitStructure.TIM_ICFilter = 0x03;//IC4F=0011 配置输入滤波器 8个定时器时钟周期滤波
    #endif
    TIM_ICInit(g_IrTimerVar.timerX, &TIM_ICInitStructure);//初始化定时器输入捕获通道

    NVIC_InitStructure.NVIC_IRQChannel = g_IrTimerVar.NVIC_IRQChannelX; //TIMx中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; //先占优先级0级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
    NVIC_Init(&NVIC_InitStructure); //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
    /* Clear TIMx update pending flag */
    TIM_ClearFlag(g_IrTimerVar.timerX, TIM_FLAG_Update);
    TIM_ITConfig( g_IrTimerVar.timerX,TIM_IT_Update|g_IrTimerVar.TIM_IT_CCX,ENABLE);//允许更新中断 ,允许CC2IE捕获中断
#endif
    TIM_Cmd(g_IrTimerVar.timerX, DISABLE); //使能定时器5
}
#endif

#if ENABLE_IR_COUNTED_USE_TIMER_CLOCK_50US

#define SCAN_NEC_LEADER_START_LOW       (RM_PRO_NEC_LEADER_START_LOW/TIMER_CLK)
#define SCAN_NEC_LEADER_START_HIGH      (RM_PRO_NEC_LEADER_START_HIGH/TIMER_CLK)
#define SCAN_NEC_REPEAT_START_LOW       (RM_PRO_NEC_REPEAT_START_LOW/TIMER_CLK)
#define SCAN_NEC_REPEAT_START_HIGH      (RM_PRO_NEC_REPEAT_START_HIGH/TIMER_CLK)

#define SCAN_NEC_LOGIC_ONE_HIGH         (560/TIMER_CLK)
#define SCAN_NEC_LOGIC_ONE_LOW          (1690/TIMER_CLK)
#define SCAN_NEC_LOGIC_ZERO_HIGH        (560/TIMER_CLK)
#define SCAN_NEC_LOGIC_ZERO_LOW         (560/TIMER_CLK)

#define SCAN_NEC_LEADER_START           (SCAN_NEC_LEADER_START_HIGH+SCAN_NEC_LEADER_START_LOW)
#define SCAN_NEC_REPEAT_START           (SCAN_NEC_REPEAT_START_HIGH+SCAN_NEC_REPEAT_START_LOW)
#define SCAN_NEC_LOGIC_ONE              (2250/TIMER_CLK)
#define SCAN_NEC_LOGIC_ZERO             (1120/TIMER_CLK)

#define SCAN_NEC_LOGIC_END              (60000/TIMER_CLK)

#define ScanValMin(x)                   (x-10)
#define ScanValMax(x)                   (x+10)
void IrRemoteScanModeParser(pstIrKeyWave_t pstWave);

#if ((_IR_FUNCTION_USE&_IR_FUNCTION_KEY_STUDY)==_IR_FUNCTION_KEY_STUDY)
static void mdrv_IrDecodeProcIrqCallback(void)
{
    pstIrCloneGoing_t           pstIrClone = &g_stIrCloneRx;
    pstIrWorkDeclare_t          pstIrWork = (pstIrWorkDeclare_t)&stProjectIrWorkTable;
    pstIrSensorConfig_t         pstIrSensor = &pstIrWork->stIrSensor;
    u8 l_u8Port = pstIrSensor->pstIrSensorPort->u8PortType;
    u16 l_u16Pin = pstIrSensor->pstIrSensorPort->u16GpioPin;
    static u16 sl_u16HighSta = 0;
    static u16 sl_u16LowSta = 0;
    //static u8  sl_u8WaveIndex = 0;

    if(pstIrClone->eIrCloneMode==eIrWorkMode_Send)
    {
        if(TIM_GetITStatus(g_IrTimerVar.timerX,TIM_IT_Update)!=RESET)
        {
            g_u16IrSendCnt++;
            TIM_ClearFlag(g_IrTimerVar.timerX,TIM_IT_Update);
        }
    }
    else
    {
        pstIrClone->pKeyCursor = &g_stIrIdleKeyData;
        if(TIM_GetITStatus(g_IrTimerVar.timerX,TIM_IT_Update)!=RESET)
        {
            //GPIO_ToggleBits(GPIOA,GPIO_Pin_15);
            if(!GPIO_ReadInputDataBit(STM32_GPIO_PORT[l_u8Port-PORT_GPIOA],l_u16Pin))
            {
                sl_u16LowSta++;
                if(sg_u8IrIndex == emIr_Receive)
                {
                    if(sl_u16HighSta!=0)
                    {
                        #if 1
                        if(sl_u16LowSta>=SCAN_NEC_LOGIC_END)
                        {
                            sl_u16LowSta = SCAN_NEC_LOGIC_END;
                        }
                        #endif
                        pstIrClone->pKeyCursor->u16KeyWave[pstIrClone->u16BitCnt] = sl_u16HighSta;
                        pstIrClone->u16BitCnt++;
                        sl_u16HighSta = 0;
                        if(pstIrClone->u16BitCnt>=(IR_WAVE_STUDY_BIT_CNT_MAX-1))
                        {
                            sg_u8IrIndex = emIr_End;
                        }
                    }
                }
            }
            else
            {
                if(sg_u8IrIndex == emIr_Receive)
                {
                    if(sl_u16LowSta!=0)
                    {
                        #if 1
                        if(sl_u16LowSta>=SCAN_NEC_LOGIC_END)
                        {
                            sl_u16LowSta = SCAN_NEC_LOGIC_END;
                        }
                        #endif
                        pstIrClone->pKeyCursor->u16KeyWave[pstIrClone->u16BitCnt] = sl_u16LowSta;
                        pstIrClone->u16BitCnt++;
                        sl_u16LowSta = 0;
                        if(pstIrClone->u16BitCnt>=(IR_WAVE_STUDY_BIT_CNT_MAX-1))
                        {
                            sg_u8IrIndex = emIr_End;
                        }
                    }
                }
                sl_u16HighSta++;
                if((sl_u16HighSta>=SCAN_NEC_LOGIC_END)||(pstIrClone->u16BitCnt == (IR_WAVE_STUDY_BIT_CNT_MAX-1)))
                {
                    pstIrClone->pKeyCursor->u16KeyWave[pstIrClone->u16BitCnt] = sl_u16HighSta/4;
                    pstIrClone->u16BitCnt++;
                    sl_u16HighSta= 0;
                    sl_u16LowSta = 0;
                    sg_u8IrIndex = emIr_End;
                }
            }

            if(sg_u8IrIndex == emIr_End)
            {
                pstIrClone->pKeyCursor->u16BitCnt = pstIrClone->u16BitCnt;
                pstIrClone->u16BitCnt = 0;
                #if ((_IR_FUNCTION_USE&_IR_FUNCTION_REMOTE)==_IR_FUNCTION_REMOTE)
                IrRemoteScanModeParser(&g_stIrIdleKeyData);
                #endif
                TIM_Cmd(g_IrTimerVar.timerX, DISABLE);
                IrSensorExtiEnable();
    //            pstIrClone->pKeyCursor->u16BitCnt = 0;
                if(pstIrClone->eIrCloneMode == eIrWorkMode_Study)
                {
                    pstIrClone->eIrCloneMode = eIrWorkMode_StudyOk;
                }
            }
            }

        TIM_ClearFlag(g_IrTimerVar.timerX,TIM_IT_Update);
    }
}

#else
static void mdrv_IrDecodeProcIrqCallback(void)
{
    pstIrCloneGoing_t           pstIrClone = &g_stIrCloneRx;
    pstIrWorkDeclare_t          pstIrWork = (pstIrWorkDeclare_t)&stProjectIrWorkTable;
    pstIrSensorConfig_t         pstIrSensor = &pstIrWork->stIrSensor;
    u8 l_u8Port = pstIrSensor->pstIrSensorPort->u8PortType;
    u16 l_u16Pin = pstIrSensor->pstIrSensorPort->u16GpioPin;
    static u16 sl_u16HighSta = 0;
    static u16 sl_u16LowSta = 0;
    //static u8  sl_u8WaveIndex = 0;

    pstIrClone->pKeyCursor = &g_stIrIdleKeyData;
    if(TIM_GetITStatus(g_IrTimerVar.timerX,TIM_IT_Update)!=RESET)
    {
        //GPIO_ToggleBits(GPIOA,GPIO_Pin_15);
        if(!GPIO_ReadInputDataBit(STM32_GPIO_PORT[l_u8Port-PORT_GPIOA],l_u16Pin))
        {
            sl_u16LowSta++;
            if(sg_u8IrIndex == emIr_Receive)
            {
                if(sl_u16HighSta!=0)
                {
                    pstIrClone->pKeyCursor->u16KeyWave[pstIrClone->pKeyCursor->u16BitCnt] = sl_u16HighSta;
                    pstIrClone->pKeyCursor->u16BitCnt++;
                    sl_u16HighSta = 0;
                    if(pstIrClone->pKeyCursor->u16BitCnt>=66)
                    {
                        sg_u8IrIndex = emIr_End;
                    }
                }
            }
        }
        else
        {
            if(sg_u8IrIndex == emIr_Receive)
            {
                if(sl_u16LowSta!=0)
                {
                    pstIrClone->pKeyCursor->u16KeyWave[pstIrClone->pKeyCursor->u16BitCnt] = sl_u16LowSta;
                    pstIrClone->pKeyCursor->u16BitCnt++;
                    sl_u16LowSta = 0;
                    if(pstIrClone->pKeyCursor->u16BitCnt>=66)
                    {
                        sg_u8IrIndex = emIr_End;
                    }
                }
            }
            sl_u16HighSta++;
            if(sl_u16HighSta>=SCAN_NEC_LOGIC_END)
            {
                sl_u16HighSta= 0;
                sl_u16LowSta = 0;
                sg_u8IrIndex = emIr_None;
                #if ((_IR_FUNCTION_USE&_IR_FUNCTION_REMOTE)==_IR_FUNCTION_REMOTE)
                IrRemoteScanModeParser(&g_stIrIdleKeyData);
                #endif
                TIM_Cmd(g_IrTimerVar.timerX, DISABLE);
                IrSensorExtiEnable();
                pstIrClone->pKeyCursor->u16BitCnt = 0;
            }
        }

        if(sg_u8IrIndex == emIr_End)
        {

        }

        TIM_ClearFlag(g_IrTimerVar.timerX,TIM_IT_Update);
    }
}
#endif

#if ((_IR_FUNCTION_USE&_IR_FUNCTION_REMOTE)==_IR_FUNCTION_REMOTE)
void IrRemoteScanModeParser(pstIrKeyWave_t pstWave)
{
    pstIrWorkDeclare_t          pstIrWork = (pstIrWorkDeclare_t)&stProjectIrWorkTable;
    pstIrSensorConfig_t         pstIrSensor = &pstIrWork->stIrSensor;
    uint16_t                    u16Index;
    uint32_t                    u32Result = 0, u32Tmp;

    if(pstIrSensor->eIrRmPro == eIrRmProtocol_NEC)
    {
        u32Tmp = (pstWave->u16KeyWave[0] + pstWave->u16KeyWave[1]);

        if((u32Tmp>=ScanValMin(SCAN_NEC_LEADER_START))&&(u32Tmp<=ScanValMax(SCAN_NEC_LEADER_START)))
        {
            for(u16Index=2; u16Index<pstWave->u16BitCnt; u16Index+=2)
            {
                u32Tmp = (pstWave->u16KeyWave[u16Index] + pstWave->u16KeyWave[u16Index+1]);

                if((u32Tmp>=ScanValMin(SCAN_NEC_LOGIC_ONE))&&(u32Tmp<=ScanValMax(SCAN_NEC_LOGIC_ONE)))
                {
                    u32Result <<= 1;
                    u32Result +=  1;
                }
                else if((u32Tmp>=ScanValMin(SCAN_NEC_LOGIC_ZERO))&&(u32Tmp<=ScanValMax(SCAN_NEC_LOGIC_ZERO)))
                {
                    u32Result <<= 1;
                    u32Result +=  0;
                }
                else
                {
                    break;
                }
            }

            if(u16Index == 66)
            {
                pstIrDecode_t   pstIrDec = &g_stIrDecodeVal;
                BaseType_t      xHigherPriorityTaskWoken = pdFALSE;

                pstIrDec->u8AddrLow     = ((u32Result>>24)&0xFF);
                pstIrDec->u8AddrHigh    = ((u32Result>>16)&0xFF);
                pstIrDec->u8CommandN    = ((u32Result>>8)&0xFF);
                pstIrDec->u8CommandE    = (u32Result&0xFF);
                pstIrDec->bIsRepeat     = FALSE;

                xQueueSendFromISR(xQueue_IrDecode, (void *)&pstIrDec, &xHigherPriorityTaskWoken);
                portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
            }
        }
        else if((u32Tmp>=ScanValMin(SCAN_NEC_REPEAT_START))&&(u32Tmp<=ScanValMax(SCAN_NEC_REPEAT_START)))
        {
                pstIrDecode_t   pstIrDec = &g_stIrDecodeVal;
                BaseType_t      xHigherPriorityTaskWoken = pdFALSE;

                pstIrDec->bIsRepeat     = TRUE;

                xQueueSendFromISR(xQueue_IrDecode, (void *)&pstIrDec, &xHigherPriorityTaskWoken);
                portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        }
    }
}
#endif

#else

static void mdrv_IrDecodeProcIrqCallback(void)
{
    #if ((_IR_FUNCTION_USE&_IR_FUNCTION_KEY_STUDY)==_IR_FUNCTION_KEY_STUDY)
    pstIrStudy_t ptIrWork = &g_tIrStudyVar;
    #endif
    pstIrCloneGoing_t           pstIrClone = &g_stIrCloneRx;
    static volatile uint8_t     svu8DetectIrRepeat = 0;//after timeout, do not need to detect repeat
    uint16_t                    u16Dval;
    pstIrWorkDeclare_t          pstIrWork = (pstIrWorkDeclare_t)&stProjectIrWorkTable;
    pstIrSensorConfig_t         pstIrSensor = &pstIrWork->stIrSensor;

    pstIrClone->pKeyCursor = &g_stIrIdleKeyData;

    if(TIM_GetITStatus(g_IrTimerVar.timerX,TIM_IT_Update)!=RESET)
    {
    #if ((_IR_FUNCTION_USE&_IR_FUNCTION_BYPASS) == _IR_FUNCTION_BYPASS)
        IrSendDisable38kOut();
    #endif

        if((pstIrClone->eIrCloneMode==eIrWorkMode_Study)&&(svu8DetectIrRepeat==0))
        {
            pstIrClone->pKeyCursor->u16BitCnt = pstIrClone->u16BitCnt;
        }
        else if(pstIrClone->eIrCloneMode==eIrWorkMode_None)
        {
            pstIrClone->pKeyCursor->u16BitCnt = pstIrClone->u16BitCnt;
        }
        pstIrClone->u16BitCnt = 0;
    #if !ENABLE_IR_COUNTED_USE_TIMER_CLOCK_50US
        #if 0//def STM32F2XX_HD
        g_IrTimerVar.TIM_OCXPolarityConfig(g_IrTimerVar.timerX,TIM_ICPolarity_Falling); //CC4P=0 设置为上升沿捕获
        #else
        g_IrTimerVar.TIM_OCXPolarityConfig(g_IrTimerVar.timerX,TIM_ICPolarity_Rising); //CC4P=0 设置为上升沿捕获
        #endif
    #endif
        if((pstIrClone->eIrCloneMode==eIrWorkMode_Study)&&(svu8DetectIrRepeat<6))//start a new study after 6*64ms
        {
            svu8DetectIrRepeat ++;
        }
        else
        {
        #if ((_IR_FUNCTION_USE&_IR_FUNCTION_REMOTE) == _IR_FUNCTION_REMOTE)
            if(pstIrClone->eIrCloneMode==eIrWorkMode_None)
            {
                IrRemoteParser(&g_stIrIdleKeyData);
            }
        #endif

            svu8DetectIrRepeat = 0;
            TIM_Cmd(g_IrTimerVar.timerX, DISABLE);
            IrSensorExtiEnable();
        }

    #if ((_IR_FUNCTION_USE&_IR_FUNCTION_KEY_STUDY)==_IR_FUNCTION_KEY_STUDY)
        if(pstIrClone->eIrCloneMode==eIrWorkMode_Study)
        {
            //if(pstIrClone->pKeyCursor != &g_stIrIdleKeyData)
            if(pstIrClone->u8IrKeyIndex != ir_study_key_type_none)
            {
                ptIrWork->bIsFirstEnterIrStudy = FALSE;
                //IR_STUDY_START_ISR(ir_study_key_type_none);
                IR_NONE_START_ISR(ir_study_key_type_none);
                /*
                pstIrClone->pKeyCursor->u8CheckSum = CalIrStudyKeyCheckSum(pstIrClone->pKeyCursor);
                SaveUserDataToEep(eEepDataIndex_IrKeys,
                                (ADDR_EEPROM_USER_START+((uint8_t *)pstIrStudy-(uint8_t *)pstPrjSaveVar)),
                                (uint8_t *)&g_stIrIdleKeyData[ir_study_key_type_none+1],
                                (sizeof(stIrKeyWave_t)*(ir_study_key_type_max-1)),
                                TIME_SAVE_TO_EEPROM_IMMEDIATELY,
                                TRUE);
                */
                IrStudyDataSave(pstIrClone->u8IrKeyIndex-ir_study_key_type_dis_on, pstIrClone->pKeyCursor, TRUE);
            }
        }
    #endif

        TIM_ClearFlag(g_IrTimerVar.timerX,TIM_IT_Update|g_IrTimerVar.TIM_IT_CCX);
    }

    if(TIM_GetITStatus(g_IrTimerVar.timerX,g_IrTimerVar.TIM_IT_CCX)!=RESET)
    {
        u16Dval = g_IrTimerVar.TIM_GetCaptureX(g_IrTimerVar.timerX);//读取CCR1也可以清CC1IF标志位
        if(pstIrClone->eIrCloneMode==eIrWorkMode_Study)
        {
            if(svu8DetectIrRepeat != 0)// reset when detected repeat within 6*64ms
            {
                svu8DetectIrRepeat = 1;
                TIM_ClearFlag(g_IrTimerVar.timerX,g_IrTimerVar.TIM_IT_CCX);
                return;
            }

            if(pstIrClone->u16BitCnt < (IR_WAVE_STUDY_BIT_CNT_MAX-1))
            {
                *(pstIrClone->pKeyCursor->u16KeyWave + (pstIrClone->u16BitCnt++)) = u16Dval;
            }
        }
        else if(pstIrClone->eIrCloneMode==eIrWorkMode_None)
        {
            if(pstIrClone->u16BitCnt < (IR_WAVE_STUDY_BIT_CNT_MAX-1))
            {
                *(pstIrClone->pKeyCursor->u16KeyWave + (pstIrClone->u16BitCnt++)) = u16Dval;
            }
        }
        else
        {
            pstIrClone->u16BitCnt++;
        }

    #if ((_IR_FUNCTION_USE&_IR_FUNCTION_BYPASS) == _IR_FUNCTION_BYPASS)
        if((pstIrClone->u16BitCnt%2)==0) IrSendEnable38kOut();
        else IrSendDisable38kOut();
    #endif

        if(STM32_GPIO_PORT[pstIrSensor->pstIrSensorPort->u8PortType-PORT_GPIOA]->IDR & pstIrSensor->pstIrSensorPort->u16GpioPin)//上升沿捕获
        {
            g_IrTimerVar.TIM_OCXPolarityConfig(g_IrTimerVar.timerX,TIM_ICPolarity_Falling); //CC1P=1 设置为下降沿捕获
        }
        else //下降沿捕获
        {
            g_IrTimerVar.TIM_OCXPolarityConfig(g_IrTimerVar.timerX,TIM_ICPolarity_Rising); //CC4P=0 设置为上升沿捕获
        }
        TIM_SetCounter(g_IrTimerVar.timerX,0); //清空定时器值
        TIM_ClearFlag(g_IrTimerVar.timerX,g_IrTimerVar.TIM_IT_CCX);
    }

}
#endif

void IrTimerXUse_IRQHandler(void)
{
    g_IrTimerVar.irDecode();
}

void vTaskIR(void *pvParameters)
{
    (void)pvParameters;

    Remote_Init();
    for(;;)
    {
        #if ((_IR_FUNCTION_USE&_IR_FUNCTION_REMOTE)==_IR_FUNCTION_REMOTE)
        IrDecodeStateUpdate();
        #endif

        #if (((_IR_FUNCTION_USE&_IR_FUNCTION_KEY_STUDY)==_IR_FUNCTION_KEY_STUDY))
        #if (ENABLE_IR_COUNTED_USE_TIMER_CLOCK_50US)
        IrLearnedOkPorcess();
        #else
        IrStudyStateUpdate();
        #endif
        #endif

        vTaskDelay(10);
    }
}

#if ((_IR_FUNCTION_USE&_IR_FUNCTION_REMOTE)==_IR_FUNCTION_REMOTE)
#if _ENABLE_NEW_USER_KEY_DEFINE_FUNCTION
pIrKeyScan pmapp_IrKeyScanFunc = NULL;

void IrDecodeStateUpdate(void)
{
    pstIrDecode_t           pstIrDecState;
    const TickType_t        xTicksToWait = 0;//20/portTICK_RATE_MS;
    BaseType_t              xResult;
    pstIrWorkDeclare_t      pstIrwork = (pstIrWorkDeclare_t)&stProjectIrWorkTable;
    u32                     l_u32ReceiveIrValue;
    u16                     l_u16CustomerCode;
    u8                      l_u8Cmd;
    xResult = xQueueReceive(xQueue_IrDecode, (void *)&pstIrDecState, (TickType_t)xTicksToWait);
    if(xResult == pdPASS)
    {
        if(pstIrwork->stIrSensor.eIrRmPro == eIrRmProtocol_NEC)
        {
            l_u32ReceiveIrValue = (pstIrDecState->u8AddrLow<<24)
                                    |(pstIrDecState->u8AddrHigh<<16)
                                    |(pstIrDecState->u8CommandN<<8)
                                    |(pstIrDecState->u8CommandE);
            if(l_u32ReceiveIrValue != 0)
            {
                l_u16CustomerCode = (pstIrDecState->u8AddrLow<<8)|(pstIrDecState->u8AddrHigh);
                l_u8Cmd = (pstIrDecState->u8CommandN);
                if(pmapp_IrKeyScanFunc != NULL)
                {
                    pmapp_IrKeyScanFunc(l_u16CustomerCode,l_u8Cmd,pstIrDecState->bIsRepeat);
                }
            }
        }
    }
}

#else
void IrDecodeStateUpdate(void)
{
    pstIrDecode_t               pstIrDecState;
    const TickType_t            xTicksToWait = 0;//20/portTICK_RATE_MS;
    BaseType_t                  xResult;
    pstKeyDeclare_t             pstKey, pstKeyOrg = (pstKeyDeclare_t)&stProjectKeyTable[0];
    pstIrWorkDeclare_t          pstIrwork = (pstIrWorkDeclare_t)&stProjectIrWorkTable;
    static uint16_t             su16IrRepeatTimeCnt[NUM_OF_KEYS_MAX] = {0};
    static bool                 sbIrWaitRelease[NUM_OF_KEYS_MAX] = {FALSE};

    xResult = xQueueReceive(xQueue_IrDecode, (void *)&pstIrDecState, (TickType_t)xTicksToWait);
    if(xResult == pdPASS)
    {
        pstKey = pstKeyOrg;
        while(pstKey->funKeyResponse != NULL)
        {
            if(pstIrwork->stIrSensor.eIrRmPro == eIrRmProtocol_NEC)
            {
                if((pstIrDecState->u8AddrLow==(((pstKey->u32IrValue)>>24)&0xFF))&&\
                    (pstIrDecState->u8AddrHigh==(((pstKey->u32IrValue)>>16)&0xFF))&&\
                    (pstIrDecState->u8CommandN==(((pstKey->u32IrValue)>>8)&0xFF))&&\
                    (pstIrDecState->u8CommandE==((pstKey->u32IrValue)&0xFF)))
                {
                    if(!pstIrDecState->bIsRepeat)
                    {
                        key_store_val(KEY_NORMAL_PRESS_VAL, eKeyValSource_IrRemote, (pstKey-pstKeyOrg));
                        su16IrRepeatTimeCnt[pstKey-pstKeyOrg] = 0 ;
                        sbIrWaitRelease[pstKey-pstKeyOrg] = FALSE;
                    }
                    else
                    {
                        if(pstKey->u16LongPressTime != KEY_LONG_INVALID)
                        {
                            if(!sbIrWaitRelease[pstKey-pstKeyOrg])
                            {
                                su16IrRepeatTimeCnt[pstKey-pstKeyOrg] ++;
                                if((su16IrRepeatTimeCnt[pstKey-pstKeyOrg]*108)>=(pstKey->u16LongPressTime*10))
                                {
                                    key_store_val(KEY_LONG_PRESS_VAL, eKeyValSource_IrRemote, (pstKey-pstKeyOrg));
                                    su16IrRepeatTimeCnt[pstKey-pstKeyOrg] = 0 ;
                                    sbIrWaitRelease[pstKey-pstKeyOrg] = TRUE;
                                }
                            }
                        }
                    }
                }
            }
            pstKey ++;
        }
    }
}
#endif
#endif

#if (((_IR_FUNCTION_USE&_IR_FUNCTION_KEY_STUDY)==_IR_FUNCTION_KEY_STUDY))
#if (!ENABLE_IR_COUNTED_USE_TIMER_CLOCK_50US)
void IrStudyStateUpdate(void)
{
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    pstIrStudy_t                pstIrStudyState;
    pstIrCloneGoing_t           pstIrClone = &g_stIrCloneRx;
    pstIrKeyWave_t              pIrStoreData = &pstPrjSaveVar->stIrStudyKeyData[0];
    const TickType_t            xTicksToWait = 0;//20/portTICK_RATE_MS;
    BaseType_t                  xResult;

    xResult = xQueueReceive(xQueue_IrStudy, (void *)&pstIrStudyState, (TickType_t)xTicksToWait);
    if(xResult == pdPASS)
    {
        if(pstIrClone->eIrCloneMode==eIrWorkMode_Send) return;

        pstIrClone->eIrCloneMode = pstIrStudyState->eIrWorkMode;
        pIrStoreData += pstIrStudyState->u8StudyKeyIndex-ir_study_key_type_dis_on;
        pstIrClone->u8IrKeyIndex = (uint8_t)(pstIrStudyState->u8StudyKeyIndex);
        pstIrClone->pKeyCursor = pIrStoreData;
        if(pstIrClone->eIrCloneMode == eIrWorkMode_Send)
        {
            if(pstIrClone->pKeyCursor->u16BitCnt > 0)
            {
                TIM_Cmd(TIM7, ENABLE);
                pstIrClone->u16SendModeBitCnt = 0;
            }
            else
            {
                pstIrClone->eIrCloneMode = eIrWorkMode_None;
                pstIrClone->pKeyCursor = &g_stIrIdleKeyData;
            }
        }
    }
}

void IrStudyDataSave(u8 u8Index,pstIrKeyWave_t pIrKeyVal, bool bIsr)
{
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    pstIrKeyWave_t              pIrStoreData = &pstPrjSaveVar->stIrStudyKeyData[0];
    u16 i;

    pIrStoreData += u8Index;
    pIrStoreData->u16BitCnt = pIrKeyVal->u16BitCnt;
    memset((u8*)pIrStoreData->u16KeyWave,0,IR_WAVE_STUDY_BIT_CNT_MAX*2);
    for(i=0;i<pIrKeyVal->u16BitCnt;i++)
    {
        pIrStoreData->u16KeyWave[i] = pIrKeyVal->u16KeyWave[i];
    }
    SetUserData_IrStudyVal(u8Index, bIsr);
}
#endif

void SetIrLearnMode(eIrWorkMode_t eIrWorkMode)
{
    pstIrCloneGoing_t           pstIrClone = &g_stIrCloneRx;

    pstIrClone->eIrCloneMode = eIrWorkMode;
}

eIrWorkMode_t GetIrLearnMode(void)
{
    pstIrCloneGoing_t           pstIrClone = &g_stIrCloneRx;

    return pstIrClone->eIrCloneMode;
}

#endif

void Remote_Init(void)
{
    pstIrWorkDeclare_t  pstIrWork = (pstIrWorkDeclare_t)&stProjectIrWorkTable;

#if ((_IR_FUNCTION_USE&_IR_FUNCTION_KEY_STUDY)==_IR_FUNCTION_KEY_STUDY)
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
#endif //add by wf8421

    memset(&g_stIrCloneRx, 0, sizeof(stIrCloneGoing_t));
#if ((_IR_FUNCTION_USE&_IR_FUNCTION_REMOTE)==_IR_FUNCTION_REMOTE)
    memset(&g_stIrDecodeVal, 0, sizeof(stIrDecode_t));
#endif

    if(pstIrWork->pstIrSenderPort != NULL)
    {
        #ifdef STM32F2XX_HD
        TIMER7_Configuration(263,3);
        #elif (defined(GD32F4XX))
        TIMER7_Configuration(52,42);
        #else
            #ifdef STM32F10X_MD
            TIMER4_Configuration(105,9);
            #else
            TIMER7_Configuration(106,9);
            #endif
        #endif
    }

    mdrv_IrRemoteNecTimeInit(IrTimerXUse,IrTimerXInputChannel);

#if ((_IR_FUNCTION_USE&_IR_FUNCTION_KEY_STUDY)==_IR_FUNCTION_KEY_STUDY)
    memcpy(&g_stIrIdleKeyData,&pstPrjSaveVar->stIrStudyKeyData[0],sizeof(stIrKeyWave_t));
#endif //add by wf8421
    g_stIrCloneRx.pKeyCursor = &g_stIrIdleKeyData;

    if(pmapp_IrInitFunc!=NULL)
    {
        pmapp_IrInitFunc();
    }
}

void IrSensorExtiEnable(void)
{
    EXTI->IMR |= IrSensor_Exti_Line;
}

void IrSensorExtiDisable(void)
{
    EXTI->IMR &= ~ IrSensor_Exti_Line;
}

void IrSensor_IRQHandler(void)
{
    if(EXTI_GetITStatus(IrSensor_Exti_Line) != RESET)
    {
        IrSensorExtiDisable();

        #if ENABLE_IR_COUNTED_USE_TIMER_CLOCK_50US
        sg_u8IrIndex = emIr_Receive;
        #endif
        TIM_Cmd(g_IrTimerVar.timerX, ENABLE);
        TIM_SetCounter(g_IrTimerVar.timerX,0);

        #if ((_IR_FUNCTION_USE&_IR_FUNCTION_BYPASS) == _IR_FUNCTION_BYPASS)
        IrSendEnable38kOut();
        #endif

        /* Clear EXTI line x pending bit */
        EXTI_ClearITPendingBit(IrSensor_Exti_Line);
    }
}

#if 0
void TIMER4_Configuration(void)
{
    NVIC_InitTypeDef            NVIC_InitStructure;
    EXTI_InitTypeDef            EXTI_InitStructure;
    TIM_TimeBaseInitTypeDef     TIM_TimeBaseStructure;
    TIM_ICInitTypeDef           TIM_ICInitStructure;

    #ifdef STM32F2XX_HD
    //RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE); //enable rcc of TIM4
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);   // enable system config
    #endif
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE); //enable rcc of TIM4

    /* Enable the EXTIx global Interrupt (with higher priority) */
    NVIC_InitStructure.NVIC_IRQChannel = IrSensor_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* Connect EXTI Line */
    #ifdef STM32F2XX_HD
    SYSCFG_EXTILineConfig(IrSensor_PortSource, IrSensor_PinSource);
    #else
    GPIO_EXTILineConfig(IrSensor_PortSource, IrSensor_PinSource);
    #endif

    /* Configure CEC pin to generate an EXTI interrupt on falling edge */
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_InitStructure.EXTI_Line = IrSensor_Exti_Line;
    EXTI_Init(&EXTI_InitStructure);

    TIM_DeInit(TIM4);
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_Period = 64000; //设定计数器自动重装值 最大64ms溢出
    TIM_TimeBaseStructure.TIM_Prescaler =(72-1); //预,1M的计数频率,1us加1.
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM向上计式
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx

    TIM_ICInitStructure.TIM_Channel = TIM_Channel_4; // 选择输入端 IC2映射到TI5上
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising; //上升沿捕获
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1; //配置输入分频,不分频
    TIM_ICInitStructure.TIM_ICFilter = 0x03;//IC4F=0011 配置输入滤波器 8个定时器时钟周期滤波
    TIM_ICInit(TIM4, &TIM_ICInitStructure);//初始化定时器输入捕获通道

    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn; //TIM5中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; //先占优先级0级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; //从优先级3级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
    NVIC_Init(&NVIC_InitStructure); //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
    /* Clear TIM4 update pending flag */
    TIM_ClearFlag(TIM4, TIM_FLAG_Update);
    TIM_ITConfig( TIM4,TIM_IT_Update|TIM_IT_CC4,ENABLE);//允许更新中断 ,允许CC2IE捕获中断

    TIM_Cmd(TIM4, DISABLE); //使能定时器5
}

void TIM4_IRQHandler(void)
{
    pstIrCloneGoing_t           pstIrClone = &g_stIrCloneRx;
    static volatile uint8_t     svu8DetectIrRepeat = 0;//after timeout, do not need to detect repeat
    uint16_t                    u16Dval;
    pstIrWorkDeclare_t          pstIrWork = (pstIrWorkDeclare_t)&stProjectIrWorkTable;
    pstIrSensorConfig_t         pstIrSensor = &pstIrWork->stIrSensor;

    if(TIM_GetITStatus(TIM4,TIM_IT_Update)!=RESET)
    {
        #if ((_IR_FUNCTION_USE&_IR_FUNCTION_BYPASS) == _IR_FUNCTION_BYPASS)
        IrSendDisable38kOut();
        #endif

        if((pstIrClone->eIrCloneMode==eIrWorkMode_Study)&&(svu8DetectIrRepeat==0))
        {
            pstIrClone->pKeyCursor->u16BitCnt = pstIrClone->u16BitCnt;
        }
        else if(pstIrClone->eIrCloneMode==eIrWorkMode_None)
        {
            pstIrClone->pKeyCursor->u16BitCnt = pstIrClone->u16BitCnt;
        }
        pstIrClone->u16BitCnt = 0;
        TIM_OC4PolarityConfig(TIM4,TIM_ICPolarity_Rising); //CC4P=0 设置为上升沿捕获

        if((pstIrClone->eIrCloneMode==eIrWorkMode_Study)&&(svu8DetectIrRepeat<6))//start a new study after 6*64ms
        {
            svu8DetectIrRepeat ++;
        }
        else
        {
            #if ((_IR_FUNCTION_USE&_IR_FUNCTION_REMOTE) == _IR_FUNCTION_REMOTE)
            if(pstIrClone->eIrCloneMode==eIrWorkMode_None)
            {
                IrRemoteParser(&g_stIrIdleKeyData);
            }
            #endif

            svu8DetectIrRepeat = 0;
            TIM_Cmd(TIM4, DISABLE);
            IrSensorExtiEnable();
        }

        #if ((_IR_FUNCTION_USE&_IR_FUNCTION_KEY_STUDY)==_IR_FUNCTION_KEY_STUDY)
        if(pstIrClone->eIrCloneMode==eIrWorkMode_Study)
        {
            if(pstIrClone->pKeyCursor != &g_stIrIdleKeyData)
            {
                //ptIrWork->bisFirstEnterIrStudy = FALSE;
                //IR_STUDY_START_ISR(ir_study_key_type_none);
                //pstIrClone->pKeyCursor->u8CheckSum = CalIrStudyKeyCheckSum(pstIrClone->key_cursor);
                //SaveUserDataToEep(ee_data_ir_study_key, ADDR_EEPROM_USER_IR_STUDY_KEY, (uint8_t *)&g_IrStudyKeyData[ir_study_key_type_none+1], (sizeof(stIrKeyWave_t)*(ir_study_key_type_max-1)), TIME_SAVE_TO_EEPROM_IMMEDIATELY, TRUE);
            }
        }
        #endif

        TIM_ClearFlag(TIM4,TIM_IT_Update|TIM_IT_CC4);
    }

    if(TIM_GetITStatus(TIM4,TIM_IT_CC4)!=RESET)
    {
        u16Dval = TIM_GetCapture4(TIM4);//读取CCR1也可以清CC1IF标志位
        if(pstIrClone->eIrCloneMode==eIrWorkMode_Study)
        {
            if(svu8DetectIrRepeat != 0)// reset when detected repeat within 6*64ms
            {
                svu8DetectIrRepeat = 1;
                TIM_ClearFlag(TIM4,TIM_IT_CC4);
                return;
            }

            if(pstIrClone->u16BitCnt < (IR_WAVE_STUDY_BIT_CNT_MAX-1))
            {
                *(pstIrClone->pKeyCursor->u16KeyWave + (pstIrClone->u16BitCnt++)) = u16Dval;
            }
        }
        else if(pstIrClone->eIrCloneMode==eIrWorkMode_None)
        {
            if(pstIrClone->u16BitCnt < (IR_WAVE_STUDY_BIT_CNT_MAX-1))
            {
                *(pstIrClone->pKeyCursor->u16KeyWave + (pstIrClone->u16BitCnt++)) = u16Dval;
            }
        }
        else
        {
            pstIrClone->u16BitCnt++;
        }

        #if ((_IR_FUNCTION_USE&_IR_FUNCTION_BYPASS) == _IR_FUNCTION_BYPASS)
        if((pstIrClone->u16BitCnt%2)==0) IrSendEnable38kOut();
        else IrSendDisable38kOut();
        #endif

        if(STM32_GPIO_PORT[pstIrSensor->pstIrSensorPort->u8PortType-PORT_GPIOA]->IDR & pstIrSensor->pstIrSensorPort->u16GpioPin)//上升沿捕获
        {
            TIM_OC4PolarityConfig(TIM4,TIM_ICPolarity_Falling); //CC1P=1 设置为下降沿捕获
        }
        else //下降沿捕获
        {
            TIM_OC4PolarityConfig(TIM4,TIM_ICPolarity_Rising); //CC4P=0 设置为上升沿捕获
        }
        TIM_SetCounter(TIM4,0); //清空定时器值
        TIM_ClearFlag(TIM4,TIM_IT_CC4);
    }
}
#endif

#if ((_IR_FUNCTION_USE&_IR_FUNCTION_REMOTE)==_IR_FUNCTION_REMOTE)
void IrRemoteParser(pstIrKeyWave_t pstWave)
{
    pstIrWorkDeclare_t          pstIrWork = (pstIrWorkDeclare_t)&stProjectIrWorkTable;
    pstIrSensorConfig_t         pstIrSensor = &pstIrWork->stIrSensor;
    uint16_t                    u16Index;
    uint32_t                    u32Result = 0, u32Tmp;

    if(pstIrSensor->eIrRmPro == eIrRmProtocol_NEC)
    {
        u32Tmp = (pstWave->u16KeyWave[0] + pstWave->u16KeyWave[1]);

        if((u32Tmp>=RmValMin(RM_PRO_NEC_LEADER_START))&&(u32Tmp<=RmValMax(RM_PRO_NEC_LEADER_START)))
        {
            for(u16Index=2; u16Index<pstWave->u16BitCnt; u16Index+=2)
            {
                u32Tmp = (pstWave->u16KeyWave[u16Index] + pstWave->u16KeyWave[u16Index+1]);

                if((u32Tmp>=RmValMin(RM_PRO_NEC_LOGIC_ONE))&&(u32Tmp<=RmValMax(RM_PRO_NEC_LOGIC_ONE)))
                {
                    u32Result <<= 1;
                    u32Result +=  1;
                }
                else if((u32Tmp>=RmValMin(RM_PRO_NEC_LOGIC_ZERO))&&(u32Tmp<=RmValMax(RM_PRO_NEC_LOGIC_ZERO)))
                {
                    u32Result <<= 1;
                    u32Result +=  0;
                }
                else
                {
                    break;
                }
            }

            if(u16Index == 66)
            {
                pstIrDecode_t   pstIrDec = &g_stIrDecodeVal;
                BaseType_t      xHigherPriorityTaskWoken = pdFALSE;

                pstIrDec->u8AddrLow     = ((u32Result>>24)&0xFF);
                pstIrDec->u8AddrHigh    = ((u32Result>>16)&0xFF);
                pstIrDec->u8CommandN    = ((u32Result>>8)&0xFF);
                pstIrDec->u8CommandE    = (u32Result&0xFF);
                pstIrDec->bIsRepeat     = FALSE;

                xQueueSendFromISR(xQueue_IrDecode, (void *)&pstIrDec, &xHigherPriorityTaskWoken);
                portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
            }
        }
        else if((u32Tmp>=RmValMin(RM_PRO_NEC_REPEAT_START))&&(u32Tmp<=RmValMax(RM_PRO_NEC_REPEAT_START)))
        {
                pstIrDecode_t   pstIrDec = &g_stIrDecodeVal;
                BaseType_t      xHigherPriorityTaskWoken = pdFALSE;

                pstIrDec->bIsRepeat     = TRUE;

                xQueueSendFromISR(xQueue_IrDecode, (void *)&pstIrDec, &xHigherPriorityTaskWoken);
                portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        }
    }
}
#endif

#if (defined(GD32F4XX))
//Time(us) = u16Period*u16Prescaler/HCLK(MHz)
//Freq(Khz) = (HCLK(Mhz)*1000)/(u16Period*u16Prescaler)
void TIMER7_Configuration(uint16_t u16Period,uint16_t u16Prescaler) // 38K Hz
{
    timer_parameter_struct timer_initpara;

    rcu_periph_clock_enable(RCU_TIMER7);
    rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4);
    timer_deinit(TIMER7);

    timer_initpara.prescaler         = (u16Prescaler - 1);
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = (u16Period - 1);
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    timer_init(TIMER7,&timer_initpara);

    /* TIMER2 channel control update interrupt enable */
    timer_interrupt_enable(TIMER7,TIMER_INT_UP);

    nvic_irq_enable(TIMER7_UP_TIMER12_IRQn,2,0);
    timer_enable(TIMER7);

}

void IrSendEnable38kOut(void)
{
    timer_enable(TIMER7);
}

void IrSendDisable38kOut(void)
{
    pstIrWorkDeclare_t          pstIrWork = (pstIrWorkDeclare_t)&stProjectIrWorkTable;
    pstGpioConfig_t                 pstIrSender = pstIrWork->pstIrSenderPort;

    timer_disable(TIMER7);
    if(pstIrSender != NULL)
    {
        GPIO_WriteBit(STM32_GPIO_PORT[pstIrSender->u8PortType-PORT_GPIOA], pstIrSender->u16GpioPin, Bit_SET);
    }
}

/* Timer3 interrupt service routine */
void TIMER7_UP_TIMER12_IRQHandler(void)
{
    #if ((_IR_FUNCTION_USE&_IR_FUNCTION_KEY_STUDY)==_IR_FUNCTION_KEY_STUDY)
    pstIrCloneGoing_t           pstIrClone = &g_stIrCloneRx;
    static uint16_t             su16BitTimeCnt = 0;
    uint32_t                    u32KeepTime = 0;
    bool                        bEn38kOut = FALSE;
    #endif
    pstIrWorkDeclare_t          pstIrWork = (pstIrWorkDeclare_t)&stProjectIrWorkTable;
    pstGpioConfig_t             pstIrSender = pstIrWork->pstIrSenderPort;

    if(timer_interrupt_flag_get(TIMER7,TIMER_INT_UP) == SET)
    {
        #if  ((_IR_FUNCTION_USE&_IR_FUNCTION_KEY_STUDY)==_IR_FUNCTION_KEY_STUDY)
        if(pstIrClone->eIrCloneMode == eIrWorkMode_Send)
        {
            u32KeepTime = (pstIrClone->pKeyCursor->u16KeyWave[pstIrClone->u16SendModeBitCnt]*76)/1000;
            if(su16BitTimeCnt<u32KeepTime)
            {
                su16BitTimeCnt ++;
            }
            else
            {
                su16BitTimeCnt = 0;
                pstIrClone->u16SendModeBitCnt++;
                if(pstIrClone->u16SendModeBitCnt >= pstIrClone->pKeyCursor->u16BitCnt)
                {
                    pstIrClone->eIrCloneMode = eIrWorkMode_None;
                    pstIrClone->pKeyCursor = &g_stIrIdleKeyData;
                    timer_disable(TIMER7);
                    if(pstIrSender != NULL)
                    {
                        gpio_bit_write(STM32_GPIO_PORT[pstIrSender->u8PortType-PORT_GPIOA], pstIrSender->u16GpioPin, SET);
                    }
                }
            }
            if((pstIrClone->u16SendModeBitCnt%2)==0) bEn38kOut = TRUE;
            if(pstIrSender != NULL)
            {
                if(bEn38kOut)
                {
                    FlagStatus l_BitPreSta = (FlagStatus)gpio_output_bit_get(STM32_GPIO_PORT[pstIrSender->u8PortType-PORT_GPIOA],
                                                pstIrSender->u16GpioPin);
                    gpio_bit_write(STM32_GPIO_PORT[pstIrSender->u8PortType-PORT_GPIOA],
                            pstIrSender->u16GpioPin,
                            (l_BitPreSta==RESET) ? SET : RESET);
                }
                /*
                else
                {
                    GPIO_WriteBit(STM32_GPIO_PORT[pstIrSender->u8PortType-PORT_GPIOA],
                            pstIrSender->u16GpioPin,
                            Bit_RESET);
                }
                */
            }
        }
        else
        {
            if(pstIrSender != NULL)
            {
                gpio_bit_write(STM32_GPIO_PORT[pstIrSender->u8PortType-PORT_GPIOA],
                        pstIrSender->u16GpioPin,
                        (FlagStatus)(1 - gpio_output_bit_get(STM32_GPIO_PORT[pstIrSender->u8PortType-PORT_GPIOA], pstIrSender->u16GpioPin)));
            }
        }
        #else
        if(pstIrSender != NULL)
        {
            gpio_bit_write(STM32_GPIO_PORT[pstIrSender->u8PortType-PORT_GPIOA], pstIrSender->u16GpioPin, (FlagStatus)(1 - gpio_output_bit_get(STM32_GPIO_PORT[pstIrSender->u8PortType-PORT_GPIOA], pstIrSender->u16GpioPin)));
        }
        #endif

        timer_interrupt_flag_clear(TIMER7,TIMER_INT_UP);
    }
}

#else
//Time(us) = u16Period*u16Prescaler/HCLK(MHz)
//Freq(Khz) = (HCLK(Mhz)*1000)/(u16Period*u16Prescaler)
#ifdef STM32F10X_MD
void TIMER4_Configuration(uint16_t u16Period,uint16_t u16Prescaler)
{
   TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
   NVIC_InitTypeDef NVIC_InitStructure;

   //Enable TIM3 RCC.
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE );

   TIM_DeInit(TIM4);
   /* TIM3 configuration */
   TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
   TIM_TimeBaseStructure.TIM_Period = u16Period-1;// 105; // 8000/105 = 76K
   TIM_TimeBaseStructure.TIM_Prescaler = u16Prescaler-1;// 8; //divied by 9
   TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
   TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
   TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

   NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn; //TIM5中断
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; //先占优先级0级
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; //从优先级3级
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
   NVIC_Init(&NVIC_InitStructure); //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
   /* Clear TIM3 update pending flag */
   TIM_ClearFlag(TIM4, TIM_FLAG_Update);
   TIM_ITConfig( TIM4,TIM_IT_Update,ENABLE);//允许更新中断 ,允许CC2IE捕获中断

   /* TIM3 enable counter */
   TIM_Cmd(TIM4, DISABLE);
}

void IrSendEnable38kOut(void)
{
    TIM_Cmd(TIM4, ENABLE);
}

void IrSendDisable38kOut(void)
{
    pstIrWorkDeclare_t          pstIrWork = (pstIrWorkDeclare_t)&stProjectIrWorkTable;
    pstGpioConfig_t                 pstIrSender = pstIrWork->pstIrSenderPort;

    TIM_Cmd(TIM4, DISABLE);
    if(pstIrSender != NULL)
    {
        GPIO_WriteBit(STM32_GPIO_PORT[pstIrSender->u8PortType-PORT_GPIOA], pstIrSender->u16GpioPin, Bit_SET);
    }
}

/* Timer3 interrupt service routine */
void TIM4_IRQHandler(void)
{
    #if (((_IR_FUNCTION_USE&_IR_FUNCTION_KEY_STUDY)==_IR_FUNCTION_KEY_STUDY)&&(!ENABLE_IR_COUNTED_USE_TIMER_CLOCK_50US))
    pstIrCloneGoing_t           pstIrClone = &g_stIrCloneRx;
    static uint16_t             su16BitTimeCnt = 0;
    uint32_t                    u32KeepTime = 0;
    bool                        bEn38kOut = FALSE;
    pstIrWorkDeclare_t          pstIrWork = (pstIrWorkDeclare_t)&stProjectIrWorkTable;
    pstGpioConfig_t             pstIrSender = pstIrWork->pstIrSenderPort;
    #endif

    if(TIM_GetITStatus(TIM4, TIM_IT_Update) == SET)
    {
        #if  ((_IR_FUNCTION_USE&_IR_FUNCTION_KEY_STUDY)==_IR_FUNCTION_KEY_STUDY)
        #if ENABLE_IR_COUNTED_USE_TIMER_CLOCK_50US
        mapp_SendCarrierFunction();
        #else
        if(pstIrClone->eIrCloneMode == eIrWorkMode_Send)
        {
            u32KeepTime = (pstIrClone->pKeyCursor->u16KeyWave[pstIrClone->u16SendModeBitCnt]*76)/1000;
            if(su16BitTimeCnt<u32KeepTime)
            {
                su16BitTimeCnt ++;
            }
            else
            {
                su16BitTimeCnt = 0;
                pstIrClone->u16SendModeBitCnt++;
                if(pstIrClone->u16SendModeBitCnt >= pstIrClone->pKeyCursor->u16BitCnt)
                {
                    pstIrClone->eIrCloneMode = eIrWorkMode_None;
                    pstIrClone->pKeyCursor = &g_stIrIdleKeyData;
                    TIM_Cmd(TIM4, DISABLE);
                    if(pstIrSender != NULL)
                    {
                        GPIO_WriteBit(STM32_GPIO_PORT[pstIrSender->u8PortType-PORT_GPIOA], pstIrSender->u16GpioPin, Bit_SET);
                    }
                }
            }
            if((pstIrClone->u16SendModeBitCnt%2)==0) bEn38kOut = TRUE;
            if(pstIrSender != NULL)
            {
                if(bEn38kOut)
                {
                    BitAction l_BitPreSta = (BitAction)GPIO_ReadOutputDataBit(STM32_GPIO_PORT[pstIrSender->u8PortType-PORT_GPIOA],
                                                pstIrSender->u16GpioPin);
                    GPIO_WriteBit(STM32_GPIO_PORT[pstIrSender->u8PortType-PORT_GPIOA],
                            pstIrSender->u16GpioPin,
                            (l_BitPreSta==Bit_RESET) ? Bit_SET : Bit_RESET);
                }
                /*
                else
                {
                    GPIO_WriteBit(STM32_GPIO_PORT[pstIrSender->u8PortType-PORT_GPIOA],
                            pstIrSender->u16GpioPin,
                            Bit_RESET);
                }
                */
            }
        }
        else
        {
            if(pstIrSender != NULL)
            {
                GPIO_WriteBit(STM32_GPIO_PORT[pstIrSender->u8PortType-PORT_GPIOA],
                        pstIrSender->u16GpioPin,
                        (BitAction)(1 - GPIO_ReadOutputDataBit(STM32_GPIO_PORT[pstIrSender->u8PortType-PORT_GPIOA], pstIrSender->u16GpioPin)));
            }
        }
        #endif
        #else
        if(pstIrSender != NULL)
        {
            GPIO_WriteBit(STM32_GPIO_PORT[pstIrSender->u8PortType-PORT_GPIOA], pstIrSender->u16GpioPin, (BitAction)(1 - GPIO_ReadOutputDataBit(STM32_GPIO_PORT[pstIrSender->u8PortType-PORT_GPIOA], pstIrSender->u16GpioPin)));
        }
        #endif

        TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
    }
}

#else
void TIMER7_Configuration(uint16_t u16Period,uint16_t u16Prescaler)
{
   TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
   NVIC_InitTypeDef NVIC_InitStructure;

   //Enable TIM3 RCC.
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE );

   TIM_DeInit(TIM7);
   /* TIM3 configuration */
   TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
   TIM_TimeBaseStructure.TIM_Period = u16Period-1;// 105; // 8000/105 = 76K
   TIM_TimeBaseStructure.TIM_Prescaler = u16Prescaler-1;// 8; //divied by 9
   TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
   TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
   TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);

   NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn; //TIM5中断
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; //先占优先级0级
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; //从优先级3级
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
   NVIC_Init(&NVIC_InitStructure); //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
   /* Clear TIM3 update pending flag */
   TIM_ClearFlag(TIM7, TIM_FLAG_Update);
   TIM_ITConfig( TIM7,TIM_IT_Update,ENABLE);//允许更新中断 ,允许CC2IE捕获中断

   /* TIM3 enable counter */
   TIM_Cmd(TIM7, DISABLE);
}

void IrSendEnable38kOut(void)
{
    TIM_Cmd(TIM7, ENABLE);
}

void IrSendDisable38kOut(void)
{
    pstIrWorkDeclare_t          pstIrWork = (pstIrWorkDeclare_t)&stProjectIrWorkTable;
    pstGpioConfig_t                 pstIrSender = pstIrWork->pstIrSenderPort;

    TIM_Cmd(TIM7, DISABLE);
    if(pstIrSender != NULL)
    {
        GPIO_WriteBit(STM32_GPIO_PORT[pstIrSender->u8PortType-PORT_GPIOA], pstIrSender->u16GpioPin, Bit_SET);
    }
}

/* Timer3 interrupt service routine */
void TIM7_IRQHandler(void)
{
    #if ((_IR_FUNCTION_USE&_IR_FUNCTION_KEY_STUDY)==_IR_FUNCTION_KEY_STUDY)
    pstIrCloneGoing_t           pstIrClone = &g_stIrCloneRx;
    static uint16_t             su16BitTimeCnt = 0;
    uint32_t                    u32KeepTime = 0;
    bool                        bEn38kOut = FALSE;
    #endif
    pstIrWorkDeclare_t          pstIrWork = (pstIrWorkDeclare_t)&stProjectIrWorkTable;
    pstGpioConfig_t             pstIrSender = pstIrWork->pstIrSenderPort;

    if(TIM_GetITStatus(TIM7, TIM_IT_Update) == SET)
    {
        #if  ((_IR_FUNCTION_USE&_IR_FUNCTION_KEY_STUDY)==_IR_FUNCTION_KEY_STUDY)
        #if ENABLE_IR_COUNTED_USE_TIMER_CLOCK_50US
        mapp_SendCarrierFunction();
        #else
        if(pstIrClone->eIrCloneMode == eIrWorkMode_Send)
        {
            u32KeepTime = (pstIrClone->pKeyCursor->u16KeyWave[pstIrClone->u16SendModeBitCnt]*76)/1000;
            if(su16BitTimeCnt<u32KeepTime)
            {
                su16BitTimeCnt ++;
            }
            else
            {
                su16BitTimeCnt = 0;
                pstIrClone->u16SendModeBitCnt++;
                if(pstIrClone->u16SendModeBitCnt >= pstIrClone->pKeyCursor->u16BitCnt)
                {
                    pstIrClone->eIrCloneMode = eIrWorkMode_None;
                    pstIrClone->pKeyCursor = &g_stIrIdleKeyData;
                    TIM_Cmd(TIM7, DISABLE);
                    if(pstIrSender != NULL)
                    {
                        GPIO_WriteBit(STM32_GPIO_PORT[pstIrSender->u8PortType-PORT_GPIOA], pstIrSender->u16GpioPin, Bit_SET);
                    }
                }
            }
            if((pstIrClone->u16SendModeBitCnt%2)==0) bEn38kOut = TRUE;
            if(pstIrSender != NULL)
            {
                if(bEn38kOut)
                {
                    BitAction l_BitPreSta = (BitAction)GPIO_ReadOutputDataBit(STM32_GPIO_PORT[pstIrSender->u8PortType-PORT_GPIOA],
                                                pstIrSender->u16GpioPin);
                    GPIO_WriteBit(STM32_GPIO_PORT[pstIrSender->u8PortType-PORT_GPIOA],
                            pstIrSender->u16GpioPin,
                            (l_BitPreSta==Bit_RESET) ? Bit_SET : Bit_RESET);
                }
                /*
                else
                {
                    GPIO_WriteBit(STM32_GPIO_PORT[pstIrSender->u8PortType-PORT_GPIOA],
                            pstIrSender->u16GpioPin,
                            Bit_RESET);
                }
                */
            }
        }
        else
        {
            if(pstIrSender != NULL)
            {
                GPIO_WriteBit(STM32_GPIO_PORT[pstIrSender->u8PortType-PORT_GPIOA],
                        pstIrSender->u16GpioPin,
                        (BitAction)(1 - GPIO_ReadOutputDataBit(STM32_GPIO_PORT[pstIrSender->u8PortType-PORT_GPIOA], pstIrSender->u16GpioPin)));
            }
        }
        #endif
        #else
        if(pstIrSender != NULL)
        {
            GPIO_WriteBit(STM32_GPIO_PORT[pstIrSender->u8PortType-PORT_GPIOA], pstIrSender->u16GpioPin, (BitAction)(1 - GPIO_ReadOutputDataBit(STM32_GPIO_PORT[pstIrSender->u8PortType-PORT_GPIOA], pstIrSender->u16GpioPin)));
        }
        #endif

        TIM_ClearITPendingBit(TIM7, TIM_IT_Update);
    }
}
#endif
#endif

#if (((_IR_FUNCTION_USE&_IR_FUNCTION_KEY_STUDY)==_IR_FUNCTION_KEY_STUDY)&&(!ENABLE_IR_COUNTED_USE_TIMER_CLOCK_50US))
void ir_work_status_assign(eIrWorkMode_t ir_work_status, ir_study_key_type_t key, bool from_isr)
{
    pstIrStudy_t ptIrWork;
    const TickType_t xTicksToWait = 0;//portMAX_DELAY;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    ptIrWork = &g_tIrStudyVar;

    ptIrWork->eIrWorkMode = ir_work_status;
    ptIrWork->u8StudyKeyIndex = key;

    if(!from_isr)
    {
        xQueueSend(xQueue_IrStudy, (void *)&ptIrWork, xTicksToWait);
    }
    else
    {
        xQueueSendFromISR(xQueue_IrStudy, (void *)&ptIrWork, &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}
#endif

#if (((_IR_FUNCTION_USE&_IR_FUNCTION_KEY_STUDY)==_IR_FUNCTION_KEY_STUDY)&&(ENABLE_IR_COUNTED_USE_TIMER_CLOCK_50US))
void mapp_SendIrState(bool bSta)
{
    pstIrWorkDeclare_t      pstIrWork = (pstIrWorkDeclare_t)&stProjectIrWorkTable;
    pstGpioConfig_t         pstIrSender = pstIrWork->pstIrSenderPort;

    if(bSta)
    {
        GPIO_WriteBit(STM32_GPIO_PORT[pstIrSender->u8PortType-PORT_GPIOA], pstIrSender->u16GpioPin, Bit_SET);
    }
    else
    {
        GPIO_WriteBit(STM32_GPIO_PORT[pstIrSender->u8PortType-PORT_GPIOA], pstIrSender->u16GpioPin, Bit_RESET);
    }
}

void mapp_SendCarrierFunction(void)
{
    pstIrWorkDeclare_t      pstIrWork = (pstIrWorkDeclare_t)&stProjectIrWorkTable;
    pstGpioConfig_t         pstIrSender = pstIrWork->pstIrSenderPort;

    if(g_bCarrierFlag)
    {
        if(pstIrSender != NULL)
        {
            if(GPIO_ReadOutputDataBit(STM32_GPIO_PORT[pstIrSender->u8PortType-PORT_GPIOA],pstIrSender->u16GpioPin))
            {
                mapp_SendIrState(FALSE);

            }
            else
            {
                mapp_SendIrState(TRUE);
            }
        }
    }
}

void mapp_SetLineToSta(u8 u8sta)
{
    if(!u8sta)
    {
        g_bCarrierFlag = FALSE;
        mapp_SendIrState(FALSE);
    }
    else
    {
        g_bCarrierFlag = TRUE;
        mapp_SendIrState(TRUE);
    }
}

#if 0
void mapp_IrSendIsr(void)
{
    static u16 sl_u16IrCnt=0;
    pstIrCloneGoing_t           pstIrClone = &g_stIrCloneRx;

    if(pstIrClone->pKeyCursor->u16KeyWave[sl_u16IrCnt]!=0)
    {
        if((sl_u16IrCnt%2)==0)        // high
        {
//            if(g_u16IrSendCnt == 0)
            {
                mapp_SetLineToSta(HIGH);
            }
            if(g_u16IrSendCnt >= (pstIrClone->pKeyCursor->u16KeyWave[sl_u16IrCnt]))
            {
                g_u16IrSendCnt = 0;
                sl_u16IrCnt++;
            }
        }
        else        // low
        {
//            if(g_u16IrSendCnt == 0)
            {
                mapp_SetLineToSta(LOW);
            }
            if(g_u16IrSendCnt >= (pstIrClone->pKeyCursor->u16KeyWave[sl_u16IrCnt]))
            {
                g_u16IrSendCnt = 0;
                sl_u16IrCnt++;
            }
        }
    }
    else
    {
        mapp_SetLineToSta(LOW);
        sl_u16IrCnt++;
    }

    if(sl_u16IrCnt>=pstIrClone->pKeyCursor->u16BitCnt)
    {
        sl_u16IrCnt = 0;
        mapp_SendIrState(TRUE);
        IrSendDisable38kOut();
        TIM_Cmd(g_IrTimerVar.timerX, DISABLE);
        IrSensorExtiEnable();
        pstIrClone->eIrCloneMode = eIrWorkMode_None;
    }
}

void mapp_IrSendDirect(u16 *pu16Wave, u16 u16Len)
{
    u16 i;
    pstIrCloneGoing_t           pstIrClone = &g_stIrCloneRx;

    if(u16Len==0)   // 无数据，跳出去
        return;
    if(u16Len > IR_WAVE_STUDY_BIT_CNT_MAX)
    {
        u16Len = IR_WAVE_STUDY_BIT_CNT_MAX;
    }
    for(i=0;i<pstIrClone->pKeyCursor->u16BitCnt;i++)
    {
        pstIrClone->pKeyCursor->u16KeyWave[i] = pu16Wave[i];
    }
    pstIrClone->pKeyCursor->u16BitCnt = u16Len;

    g_bCarrierFlag = FALSE;     // disable carrier
    IrSensorExtiDisable();
    TIM_Cmd(g_IrTimerVar.timerX, ENABLE);
    TIM_SetCounter(g_IrTimerVar.timerX,0);
    IrSendEnable38kOut();
    pstIrClone->eIrCloneMode  = eIrWorkMode_Send;
}

#else
void mapp_IrSendDirect(u16 *pu16Wave, u16 u16Len)
{
    u16 i;
    pstIrCloneGoing_t           pstIrClone = &g_stIrCloneRx;

    if(u16Len==0)   // 无数据，跳出去
        return;
    if(u16Len > IR_WAVE_STUDY_BIT_CNT_MAX)
    {
        u16Len = IR_WAVE_STUDY_BIT_CNT_MAX;
    }

    g_bCarrierFlag = FALSE;     // disable carrier
    IrSensorExtiDisable();
    TIM_Cmd(g_IrTimerVar.timerX, ENABLE);
    TIM_SetCounter(g_IrTimerVar.timerX,0);
    IrSendEnable38kOut();
    pstIrClone->eIrCloneMode  = eIrWorkMode_Send;

    for(i=0;i<u16Len;i++)
    {
        if(pu16Wave[i]!=0)
        {
            if((i%2)==0)        // high
            {
                g_u16IrSendCnt = 0;
                mapp_SetLineToSta(HIGH);
                while(g_u16IrSendCnt < (pu16Wave[i]))
                {
                }
                g_u16IrSendCnt = 0;
            }
            else        // low
            {
                g_u16IrSendCnt = 0;
                mapp_SetLineToSta(LOW);
                while(g_u16IrSendCnt < (pu16Wave[i]))
                {
                }
                g_u16IrSendCnt = 0;
            }
        }
        else
        {
            mapp_SetLineToSta(LOW);
        }
        if(i>=u16Len)
        {
            mapp_SendIrState(TRUE);
            break;
        }
    }
    mapp_SendIrState(TRUE);
    IrSendDisable38kOut();
    TIM_Cmd(g_IrTimerVar.timerX, DISABLE);
    IrSensorExtiEnable();
    #if 0//def DEBUG_IR
    for(i=0;i<u16Len;i++)
    {
        if((i>0)&&((i==2)||((i-2)%8)==0))
        {
            //Uart_printf("\r\n");
        }
        IR_DEBUG("%04x,",pu16Wave[i]);
    }
    IR_DEBUG("\r\n");
    #endif

    pstIrClone->eIrCloneMode = eIrWorkMode_None;
}
#endif

void IrLearnedOkPorcess(void)
{
    pstIrCloneGoing_t           pstIrClone = &g_stIrCloneRx;

    if(pstIrClone->eIrCloneMode == eIrWorkMode_StudyOk)
    {
        u16 i;

        for(i=0;i<pstIrClone->pKeyCursor->u16BitCnt;i++)
        {
            g_u16IrLearnKeyData[i] = pstIrClone->pKeyCursor->u16KeyWave[i];
        }
        g_u16IrLearnCnt = pstIrClone->pKeyCursor->u16BitCnt;
        pstIrClone->eIrCloneMode = eIrWorkMode_None;
//        mapp_IrSendDirect(g_u16IrLearnKeyData,g_u16IrLearnCnt);
        if(pmapp_IrKeyLearnOkFunc != NULL)
        {
            pmapp_IrKeyLearnOkFunc();
        }
        pstIrClone->eIrCloneMode = eIrWorkMode_Study;
    }
}

#endif
#endif//#if (_IR_FUNCTION_USE != _IR_FUNCTION_NONE)





















