/**
 *****************************************************************************
  * Update
  * @Version : V1.0
  * @By      : dengzhiqin
  * @Date    : 2017 / 12 / 05
  * @Brief   : task of led
  *
  *****************************************************************************
**/
#include "includes.h"

led_work_status_fun led_work_fun_table[led_work_status_num-1];
led_work_run_t led_work_status_table[NUM_OF_LEDS_MAX];
led_work_status_t led_current_state[NUM_OF_LEDS_MAX];

#if (_EN_CHIP_PCA9539_USE == _MACRO_ENABLE)
extern uint16_t pca_output_val, pca_output_val_keep;
#endif

extern QueueHandle_t xQueue_Led;

extern const stLedDeclare_t stProjectLedTable[];

#if _EN_MAIN_TASK_LED
/**Led work main task**/
#if _SYSTEM_SUPPORT_RTOS
void vTaskLED(void *pvParameters)
{
    const TickType_t    xTicksToWait = 0;//20/portTICK_RATE_MS;
    pLed_work_run_t     ptLedWorkRunValue;
    led_work_status_fun   LedWorkFun;
    led_work_status_t     led_status;
    pstLedDeclare_t     pLedN, pLedS=(pstLedDeclare_t)&stProjectLedTable[0];
#if (_EN_CHIP_TM1623_USE == _MACRO_ENABLE)
    uint8_t l_u8Tm1623Dev = 0;
#endif

    (void)pvParameters;

    led_init();
    #if (_EN_CHIP_PCA9539_USE == _MACRO_ENABLE)
    pca9539_init();
    #endif

    #if (_EN_CHIP_TM1623_USE == _MACRO_ENABLE)
    TM1623_Init();
    #endif

    #if (_ENABLE_CHIP_LM74HC595== _MACRO_ENABLE)
    mdrv_595Init();
    #endif
    for(;;)
    {
        #if (_EN_CHIP_TM1623_USE == _MACRO_ENABLE)
        TM1623_Read_Key(l_u8Tm1623Dev,&TM1623_ksg_reg_val[l_u8Tm1623Dev][TM1623_PORT_KS1]);
        #endif

        xQueueReceive(xQueue_Led, (void *)&ptLedWorkRunValue, (TickType_t)xTicksToWait);

        pLedN = pLedS;
        while(pLedN->pstLedLocate != NULL)
        {
            led_status = led_work_status_table[pLedN-pLedS].status;
            if(led_status > led_work_none)
            {
                LedWorkFun = led_work_fun_table[led_status-1];
                LedWorkFun(pLedN-pLedS);
            }
            pLedN ++;
        }

        #if (_EN_CHIP_PCA9539_USE == _MACRO_ENABLE)
        if(pca_output_val != pca_output_val_keep)
        {
            pca9539_outputport_write(pca_output_val);
            pca_output_val_keep = pca_output_val;
        }
        #endif

        #if (_EN_CHIP_TM1623_USE == _MACRO_ENABLE)
        if(!isTM1623_SegVal_Same(l_u8Tm1623Dev))
        {
            uint8_t ii;

            for(ii=0;ii<TM1623_PORT_SEG_NUMBER;ii++)
            {
                if(g_u32TM1623SegUpdate[l_u8Tm1623Dev] & (1<<ii))
                {
                    TM1623_SegLed_Write(l_u8Tm1623Dev,(TM1623_PORT_G1xS18+ii),TM1623_ksg_reg_val[l_u8Tm1623Dev][TM1623_PORT_G1xS18+ii]);
                }
            }
            assignTM1623_SegVal_Same(l_u8Tm1623Dev);
        }
        l_u8Tm1623Dev++;
        if(l_u8Tm1623Dev>=DEV_NUM_TM1623)
            l_u8Tm1623Dev = 0;
        #endif
        #if (_ENABLE_CHIP_LM74HC595== _MACRO_ENABLE)
        mapp_Change595ValueProcess();
        #endif
        vTaskDelay(20);
    }
}
#else
void TaskLedInit(void)
{
    led_init();
#if (_EN_CHIP_PCA9539_USE == _MACRO_ENABLE)
    pca9539_init();
#endif

#if (_EN_CHIP_TM1623_USE == _MACRO_ENABLE)
    TM1623_Init();
#endif

#if (_ENABLE_CHIP_LM74HC595== _MACRO_ENABLE)
    mdrv_595Init();
#endif
}
void vTaskLED(void *pvParameters)
{
    led_work_status_fun   LedWorkFun;
    led_work_status_t     led_status;
    pstLedDeclare_t     pLedN, pLedS=(pstLedDeclare_t)&stProjectLedTable[0];
#if (_EN_CHIP_TM1623_USE == _MACRO_ENABLE)
    static uint8_t l_u8Tm1623Dev = 0;
#endif

    (void)pvParameters;

    {
        #if (_EN_CHIP_TM1623_USE == _MACRO_ENABLE)
        TM1623_Read_Key(l_u8Tm1623Dev,&TM1623_ksg_reg_val[l_u8Tm1623Dev][TM1623_PORT_KS1]);
        #endif

        pLedN = pLedS;
        while(pLedN->pstLedLocate != NULL)
        {
            led_status = led_work_status_table[pLedN-pLedS].status;
            if(led_status > led_work_none)
            {
                LedWorkFun = led_work_fun_table[led_status-1];
                LedWorkFun(pLedN-pLedS);
            }
            pLedN ++;
        }

        #if (_EN_CHIP_PCA9539_USE == _MACRO_ENABLE)
        if(pca_output_val != pca_output_val_keep)
        {
            pca9539_outputport_write(pca_output_val);
            pca_output_val_keep = pca_output_val;
        }
        #endif

        #if (_EN_CHIP_TM1623_USE == _MACRO_ENABLE)
        if(!isTM1623_SegVal_Same(l_u8Tm1623Dev))
        {
            uint8_t ii;

            for(ii=0;ii<TM1623_PORT_SEG_NUMBER;ii++)
            {
                if(g_u32TM1623SegUpdate[l_u8Tm1623Dev] & (1<<ii))
                {
                    TM1623_SegLed_Write(l_u8Tm1623Dev,(TM1623_PORT_G1xS18+ii),TM1623_ksg_reg_val[l_u8Tm1623Dev][TM1623_PORT_G1xS18+ii]);
                }
            }
            assignTM1623_SegVal_Same(l_u8Tm1623Dev);
        }
        l_u8Tm1623Dev++;
        if(l_u8Tm1623Dev>=DEV_NUM_TM1623)
            l_u8Tm1623Dev = 0;
        #endif
        #if (_ENABLE_CHIP_LM74HC595== _MACRO_ENABLE)
        mapp_Change595ValueProcess();
        #endif
    }
}

#endif
#endif

/**function register**/
void led_work_fun_register(led_work_status_t fun_index, led_work_status_fun fun_name)
{
    led_work_fun_table[fun_index-1] = fun_name;
}

/**led work always off**/
void led_work_fun_off(uint8_t led_index)
{
  pLed_work_run_t pRun = &led_work_status_table[led_index];
  pstLedDeclare_t pLoc = (pstLedDeclare_t)&stProjectLedTable[led_index];

  if(pRun->status==pRun->pre_status) return;
  pRun->pre_status = led_work_always_off;

  #if (_EN_CHIP_PCA9539_USE == _MACRO_ENABLE)
  if(pLoc->pstLedLocate->u8PortType == PORT_PCA)
  {
    (pLoc->eLedOnLevel == ePinLevel_High)?\
      (pca_output_val &= (~(pLoc->pstLedLocate->u16GpioPin))):\
      (pca_output_val |= pLoc->pstLedLocate->u16GpioPin);
    return;
  }
  else
  #endif

  #if (_EN_CHIP_TM1623_USE == _MACRO_ENABLE)
//  if((pLoc->pstLedLocate->u8PortType >= PORT_TM1623_GRID1_L)&&(pLoc->pstLedLocate->u8PortType <= PORT_TM1623_GRID7_H))
  if(IS_TM1623_GRID(pLoc->pstLedLocate->u8PortType))
  {
    (pLoc->eLedOnLevel == ePinLevel_High)?\
      (TM1623_ksg_reg_val[TM1623_DEV_INDEX(pLoc->pstLedLocate->u8PortType)][TM1623_PORT_G1xS18+TM1623_PORT_NUM(pLoc->pstLedLocate->u8PortType)] &=
        (~(pLoc->pstLedLocate->u16GpioPin))):\
      (TM1623_ksg_reg_val[TM1623_DEV_INDEX(pLoc->pstLedLocate->u8PortType)][TM1623_PORT_G1xS18+TM1623_PORT_NUM(pLoc->pstLedLocate->u8PortType)] |=
         pLoc->pstLedLocate->u16GpioPin);
    return;
  }
  else
  #endif

#if (_ENABLE_CHIP_LM74HC595 == _MACRO_ENABLE)
  if(IS_595_GPIO(pLoc->pstLedLocate->u8PortType))
  {
    if(IS_595_GPIO_H16(pLoc->pstLedLocate->u8PortType))
    {
        (pLoc->eLedOnLevel == ePinLevel_High)?\
          mapp_Set595Value(mapp_Get595Value() & (~(pLoc->pstLedLocate->u16GpioPin << 16))):\
          mapp_Set595Value(mapp_Get595Value() | (pLoc->pstLedLocate->u16GpioPin << 16));
    }
    else
    {
        (pLoc->eLedOnLevel == ePinLevel_High)?\
          mapp_Set595Value(mapp_Get595Value() & (~(pLoc->pstLedLocate->u16GpioPin))):\
          mapp_Set595Value(mapp_Get595Value() | pLoc->pstLedLocate->u16GpioPin);
    }
    return;
  }
  else
#endif
#if (defined(GD32F4XX))
    if((pLoc->pstLedLocate->u8PortType >= PORT_GPIOA)&&(pLoc->pstLedLocate->u8PortType <= PORT_GPIOG))
    {
        (pLoc->eLedOnLevel == ePinLevel_High)?\
            gpio_bit_reset(STM32_GPIO_PORT[pLoc->pstLedLocate->u8PortType-PORT_GPIOA], pLoc->pstLedLocate->u16GpioPin):\
            gpio_bit_set(STM32_GPIO_PORT[pLoc->pstLedLocate->u8PortType-PORT_GPIOA], pLoc->pstLedLocate->u16GpioPin);
    }
#else
    if((pLoc->pstLedLocate->u8PortType >= PORT_GPIOA)&&(pLoc->pstLedLocate->u8PortType <= PORT_GPIOG))
    {
        (pLoc->eLedOnLevel == ePinLevel_High)?\
            GPIO_ResetBits(STM32_GPIO_PORT[pLoc->pstLedLocate->u8PortType-PORT_GPIOA], pLoc->pstLedLocate->u16GpioPin):\
            GPIO_SetBits(STM32_GPIO_PORT[pLoc->pstLedLocate->u8PortType-PORT_GPIOA], pLoc->pstLedLocate->u16GpioPin);
    }
#endif
}

/**led work always on**/
void led_work_fun_on(uint8_t led_index)
{
  pLed_work_run_t pRun = &led_work_status_table[led_index];
  pstLedDeclare_t pLoc = (pstLedDeclare_t)&stProjectLedTable[led_index];

  if(pRun->status==pRun->pre_status) return;
  pRun->pre_status = led_work_always_on;

  #if (_EN_CHIP_PCA9539_USE == _MACRO_ENABLE)
  if(pLoc->pstLedLocate->u8PortType == PORT_PCA)
  {
    (pLoc->eLedOnLevel == ePinLevel_High)?\
      (pca_output_val |= (pLoc->pstLedLocate->u16GpioPin)):\
      (pca_output_val &= (~(pLoc->pstLedLocate->u16GpioPin)));
    return;
  }
  else
  #endif

  #if (_EN_CHIP_TM1623_USE == _MACRO_ENABLE)
  if(IS_TM1623_GRID(pLoc->pstLedLocate->u8PortType))
  {
    (pLoc->eLedOnLevel == ePinLevel_High)?\
      (TM1623_ksg_reg_val[TM1623_DEV_INDEX(pLoc->pstLedLocate->u8PortType)][TM1623_PORT_G1xS18+TM1623_PORT_NUM(pLoc->pstLedLocate->u8PortType)] |=
         (pLoc->pstLedLocate->u16GpioPin)):\
      (TM1623_ksg_reg_val[TM1623_DEV_INDEX(pLoc->pstLedLocate->u8PortType)][TM1623_PORT_G1xS18+TM1623_PORT_NUM(pLoc->pstLedLocate->u8PortType)] &=
         (~(pLoc->pstLedLocate->u16GpioPin)));
    return;
  }
  else
  #endif
#if (_ENABLE_CHIP_LM74HC595 == _MACRO_ENABLE)
    if(IS_595_GPIO(pLoc->pstLedLocate->u8PortType))
    {
        if(IS_595_GPIO_H16(pLoc->pstLedLocate->u8PortType))
        {
            (pLoc->eLedOnLevel == ePinLevel_High)?\
              mapp_Set595Value(mapp_Get595Value() & (~(pLoc->pstLedLocate->u16GpioPin << 16))):\
              mapp_Set595Value(mapp_Get595Value() | (pLoc->pstLedLocate->u16GpioPin << 16));
        }
        else
        {
            (pLoc->eLedOnLevel == ePinLevel_High)?\
                mapp_Set595Value(mapp_Get595Value() | pLoc->pstLedLocate->u16GpioPin):\
                mapp_Set595Value(mapp_Get595Value() & (~(pLoc->pstLedLocate->u16GpioPin)));
        }
        return;
    }
    else
#endif

#if (defined(GD32F4XX))
    if((pLoc->pstLedLocate->u8PortType>=PORT_GPIOA)&&(pLoc->pstLedLocate->u8PortType<=PORT_GPIOG))
    {
        (pLoc->eLedOnLevel == ePinLevel_High)?\
            gpio_bit_set(STM32_GPIO_PORT[pLoc->pstLedLocate->u8PortType-PORT_GPIOA], pLoc->pstLedLocate->u16GpioPin):\
            gpio_bit_reset(STM32_GPIO_PORT[pLoc->pstLedLocate->u8PortType-PORT_GPIOA], pLoc->pstLedLocate->u16GpioPin);
    }
#else
    if((pLoc->pstLedLocate->u8PortType>=PORT_GPIOA)&&(pLoc->pstLedLocate->u8PortType<=PORT_GPIOG))
    {
        (pLoc->eLedOnLevel == ePinLevel_High)?\
            GPIO_SetBits(STM32_GPIO_PORT[pLoc->pstLedLocate->u8PortType-PORT_GPIOA], pLoc->pstLedLocate->u16GpioPin):\
            GPIO_ResetBits(STM32_GPIO_PORT[pLoc->pstLedLocate->u8PortType-PORT_GPIOA], pLoc->pstLedLocate->u16GpioPin);
    }
#endif
}

/**led work blink**/
void led_work_fun_blink(uint8_t led_index, led_work_status_t led_status, uint16_t toggle_time)
{
  pLed_work_run_t pRun = &led_work_status_table[led_index];
  pstLedDeclare_t pLoc = (pstLedDeclare_t)&stProjectLedTable[led_index];

  if(pRun->status!=pRun->pre_status)
  {
    pRun->pre_status = led_status;
    pRun->blink_toggle_time = 0;
  }

  pRun->blink_toggle_time ++;
  if(pRun->blink_toggle_time >= toggle_time)
  {
    pRun->blink_toggle_time = 0;
    #if (_EN_CHIP_PCA9539_USE == _MACRO_ENABLE)
    if(pLoc->pstLedLocate->u8PortType == PORT_PCA)
    {
      pca_output_val ^= pLoc->pstLedLocate->u16GpioPin;
      return;
    }
    else
    #endif

    #if (_EN_CHIP_TM1623_USE == _MACRO_ENABLE)
    if(IS_TM1623_GRID(pLoc->pstLedLocate->u8PortType))
    {
      TM1623_ksg_reg_val[TM1623_DEV_INDEX(pLoc->pstLedLocate->u8PortType)][TM1623_PORT_G1xS18+TM1623_PORT_NUM(pLoc->pstLedLocate->u8PortType)] ^=
        (pLoc->pstLedLocate->u16GpioPin);
      return;
    }
    else
    #endif

        #if (_ENABLE_CHIP_LM74HC595 == _MACRO_ENABLE)
        if(IS_595_GPIO(pLoc->pstLedLocate->u8PortType))
        {
            if(IS_595_GPIO_H16(pLoc->pstLedLocate->u8PortType))
            {
                mapp_Set595Value(mapp_Get595Value() ^ (pLoc->pstLedLocate->u16GpioPin << 16));
            }
            else
            {
                mapp_Set595Value(mapp_Get595Value() ^ pLoc->pstLedLocate->u16GpioPin);
            }
            return;
        }
        else
        #endif

    if((pLoc->pstLedLocate->u8PortType >= PORT_GPIOA)&&(pLoc->pstLedLocate->u8PortType <= PORT_GPIOG))
    {
//      STM32_GPIO_PORT[pLoc->pstLedLocate->u8PortType-PORT_GPIOA]->ODR ^= pLoc->pstLedLocate->u16GpioPin;

        #if (defined(GD32F4XX))
        gpio_port_write(STM32_GPIO_PORT[pLoc->pstLedLocate->u8PortType-PORT_GPIOA],
            gpio_output_port_get(STM32_GPIO_PORT[pLoc->pstLedLocate->u8PortType-PORT_GPIOA])^pLoc->pstLedLocate->u16GpioPin);
        #else
        GPIO_Write(STM32_GPIO_PORT[pLoc->pstLedLocate->u8PortType-PORT_GPIOA],
            GPIO_ReadOutputData(STM32_GPIO_PORT[pLoc->pstLedLocate->u8PortType-PORT_GPIOA])^pLoc->pstLedLocate->u16GpioPin);
        #endif
    }
  }
}

//led normal blink
void led_work_fun_norm_blink(uint8_t led_index)
{
  led_work_fun_blink(led_index, led_work_norm_blink, _TIME_INDICATIVE_LED_NORM_BLINK_TOGGLE);
}

//led fast blink
void led_work_fun_fast_blink(uint8_t led_index)
{
  led_work_fun_blink(led_index, led_work_fast_blink, _TIME_INDICATIVE_LED_FAST_BLINK_TOGGLE);
}

//led slow blink
void led_work_fun_slow_blink(uint8_t led_index)
{
  led_work_fun_blink(led_index, led_work_slow_blink, _TIME_INDICATIVE_LED_SLOW_BLINK_TOGGLE);
}

/**led work moment function**/
void led_work_fun_moment_state(uint8_t led_index, led_work_status_t led_status, uint16_t toggle_time)
{
  pLed_work_run_t pRun = &led_work_status_table[led_index];
  pstLedDeclare_t pLoc = (pstLedDeclare_t)&stProjectLedTable[led_index];

  if(pRun->status!=pRun->pre_status)
  {
    pRun->pre_status = led_status;
    pRun->blink_toggle_time = toggle_time;
    #if (_EN_CHIP_PCA9539_USE == _MACRO_ENABLE)
    if(pLoc->pstLedLocate->u8PortType == PORT_PCA)
    {
      switch(led_status)
      {
                #if _ENABLE_LED_WORK_MOMENT_3S
                case led_work_moment_on_3s:
                #endif
        case led_work_moment_on:
          //led on
          (pLoc->eLedOnLevel == ePinLevel_High)?\
            (pca_output_val |= (pLoc->pstLedLocate->u16GpioPin)):\
            (pca_output_val &= (~(pLoc->pstLedLocate->u16GpioPin)));
          break;
                #if _ENABLE_LED_WORK_MOMENT_3S
                case led_work_moment_off_3s:
                #endif
        case led_work_moment_off:
          //led off
          (pLoc->eLedOnLevel == ePinLevel_High)?\
            (pca_output_val &= (~(pLoc->pstLedLocate->u16GpioPin))):\
            (pca_output_val |= (pLoc->pstLedLocate->u16GpioPin));
          break;
        case led_work_moment_toggle:
          //led toggle
          ((pca_output_val&pLoc->pstLedLocate->u16GpioPin)!= 0)?\
            (pca_output_val &= (~(pLoc->pstLedLocate->u16GpioPin))):\
            (pca_output_val |= (pLoc->pstLedLocate->u16GpioPin));
          break;

        default:
          break;
      }
    }
    else
    #endif

    #if (_EN_CHIP_TM1623_USE == _MACRO_ENABLE)
    if(IS_TM1623_GRID(pLoc->pstLedLocate->u8PortType))
    {
      switch(led_status)
      {
        #if _ENABLE_LED_WORK_MOMENT_3S
        case led_work_moment_on_3s:
        #endif
        case led_work_moment_on:
          //led on
          (pLoc->eLedOnLevel == ePinLevel_High)?\
            (TM1623_ksg_reg_val[TM1623_DEV_INDEX(pLoc->pstLedLocate->u8PortType)][TM1623_PORT_G1xS18+TM1623_PORT_NUM(pLoc->pstLedLocate->u8PortType)] |= (pLoc->pstLedLocate->u16GpioPin)):\
            (TM1623_ksg_reg_val[TM1623_DEV_INDEX(pLoc->pstLedLocate->u8PortType)][TM1623_PORT_G1xS18+TM1623_PORT_NUM(pLoc->pstLedLocate->u8PortType)] &= (~(pLoc->pstLedLocate->u16GpioPin)));
          break;
                #if _ENABLE_LED_WORK_MOMENT_3S
                case led_work_moment_off_3s:
                #endif
        case led_work_moment_off:
          //led off
          (pLoc->eLedOnLevel == ePinLevel_High)?\
            (TM1623_ksg_reg_val[TM1623_DEV_INDEX(pLoc->pstLedLocate->u8PortType)][TM1623_PORT_G1xS18+TM1623_PORT_NUM(pLoc->pstLedLocate->u8PortType)] &= (~(pLoc->pstLedLocate->u16GpioPin))):\
            (TM1623_ksg_reg_val[TM1623_DEV_INDEX(pLoc->pstLedLocate->u8PortType)][TM1623_PORT_G1xS18+TM1623_PORT_NUM(pLoc->pstLedLocate->u8PortType)] |= (pLoc->pstLedLocate->u16GpioPin));
          break;
        case led_work_moment_toggle:
          //led toggle
          ((TM1623_ksg_reg_val[TM1623_DEV_INDEX(pLoc->pstLedLocate->u8PortType)][TM1623_PORT_G1xS18+TM1623_PORT_NUM(pLoc->pstLedLocate->u8PortType)]&(pLoc->pstLedLocate->u16GpioPin))!=0)?\
            (TM1623_ksg_reg_val[TM1623_DEV_INDEX(pLoc->pstLedLocate->u8PortType)][TM1623_PORT_G1xS18+TM1623_PORT_NUM(pLoc->pstLedLocate->u8PortType)] &= (~(pLoc->pstLedLocate->u16GpioPin))):\
            (TM1623_ksg_reg_val[TM1623_DEV_INDEX(pLoc->pstLedLocate->u8PortType)][TM1623_PORT_G1xS18+TM1623_PORT_NUM(pLoc->pstLedLocate->u8PortType)] |= (pLoc->pstLedLocate->u16GpioPin));
          break;
        default:
          break;
      }
    }
    else
    #endif
    #if (_ENABLE_CHIP_LM74HC595== _MACRO_ENABLE)
    if(IS_595_GPIO(pLoc->pstLedLocate->u8PortType))
    {
      switch(led_status)
      {
        #if _ENABLE_LED_WORK_MOMENT_3S
        case led_work_moment_on_3s:
        #endif
        case led_work_moment_on:
          //led on
            if(IS_595_GPIO_H16(pLoc->pstLedLocate->u8PortType))
            {
                (pLoc->eLedOnLevel == ePinLevel_High)?\
                    mapp_Set595Value(mapp_Get595Value() & (~(pLoc->pstLedLocate->u16GpioPin << 16))):\
                    mapp_Set595Value(mapp_Get595Value() | (pLoc->pstLedLocate->u16GpioPin << 16));
            }
            else
            {
                (pLoc->eLedOnLevel == ePinLevel_High)?\
                    mapp_Set595Value(mapp_Get595Value() | (pLoc->pstLedLocate->u16GpioPin)):\
                    mapp_Set595Value(mapp_Get595Value() & (~(pLoc->pstLedLocate->u16GpioPin)));
            }
          break;
        #if _ENABLE_LED_WORK_MOMENT_3S
        case led_work_moment_off_3s:
        #endif
        case led_work_moment_off:
          //led off
            if(IS_595_GPIO_H16(pLoc->pstLedLocate->u8PortType))
            {
                (pLoc->eLedOnLevel == ePinLevel_High)?\
                    mapp_Set595Value(mapp_Get595Value() & (~(pLoc->pstLedLocate->u16GpioPin << 16))):\
                    mapp_Set595Value(mapp_Get595Value() | (pLoc->pstLedLocate->u16GpioPin << 16));
            }
            else
            {
                (pLoc->eLedOnLevel == ePinLevel_High)?\
                    mapp_Set595Value(mapp_Get595Value() & (~(pLoc->pstLedLocate->u16GpioPin))):\
                    mapp_Set595Value(mapp_Get595Value() | (pLoc->pstLedLocate->u16GpioPin));
            }
          break;
        case led_work_moment_toggle:
          //led toggle
            if(IS_595_GPIO_H16(pLoc->pstLedLocate->u8PortType))
            {
                ((mapp_Get595Value()&pLoc->pstLedLocate->u16GpioPin)!= 0)?\
                    mapp_Set595Value(mapp_Get595Value() & (~(pLoc->pstLedLocate->u16GpioPin << 16))):\
                    mapp_Set595Value(mapp_Get595Value() | (pLoc->pstLedLocate->u16GpioPin << 16));
            }
            else
            {
                ((mapp_Get595Value()&pLoc->pstLedLocate->u16GpioPin)!= 0)?\
                    mapp_Set595Value(mapp_Get595Value() & (~(pLoc->pstLedLocate->u16GpioPin))):\
                    mapp_Set595Value(mapp_Get595Value() | (pLoc->pstLedLocate->u16GpioPin));
            }
          break;
        default:
          break;
      }
    }
    else
    #endif

    if((pLoc->pstLedLocate->u8PortType >= PORT_GPIOA)&&(pLoc->pstLedLocate->u8PortType <= PORT_GPIOG))
    {
        switch(led_status)
        {
            #if _ENABLE_LED_WORK_MOMENT_3S
            case led_work_moment_on_3s:
            #endif
            case led_work_moment_on:
                // led on
                #if (defined(GD32F4XX))
                (pLoc->eLedOnLevel == ePinLevel_High)?\
                    gpio_bit_set(STM32_GPIO_PORT[pLoc->pstLedLocate->u8PortType-PORT_GPIOA], pLoc->pstLedLocate->u16GpioPin):\
                    gpio_bit_reset(STM32_GPIO_PORT[pLoc->pstLedLocate->u8PortType-PORT_GPIOA], pLoc->pstLedLocate->u16GpioPin);
                #else
                (pLoc->eLedOnLevel == ePinLevel_High)?\
                    GPIO_SetBits(STM32_GPIO_PORT[pLoc->pstLedLocate->u8PortType-PORT_GPIOA], pLoc->pstLedLocate->u16GpioPin):\
                    GPIO_ResetBits(STM32_GPIO_PORT[pLoc->pstLedLocate->u8PortType-PORT_GPIOA], pLoc->pstLedLocate->u16GpioPin);
                #endif
                break;
            #if _ENABLE_LED_WORK_MOMENT_3S
            case led_work_moment_off_3s:
            #endif
            case led_work_moment_off:
                //led off
                #if (defined(GD32F4XX))
                (pLoc->eLedOnLevel == ePinLevel_High)?\
                    gpio_bit_reset(STM32_GPIO_PORT[pLoc->pstLedLocate->u8PortType-PORT_GPIOA], pLoc->pstLedLocate->u16GpioPin):\
                    gpio_bit_set(STM32_GPIO_PORT[pLoc->pstLedLocate->u8PortType-PORT_GPIOA], pLoc->pstLedLocate->u16GpioPin);
                #else
                (pLoc->eLedOnLevel == ePinLevel_High)?\
                    GPIO_ResetBits(STM32_GPIO_PORT[pLoc->pstLedLocate->u8PortType-PORT_GPIOA], pLoc->pstLedLocate->u16GpioPin):\
                    GPIO_SetBits(STM32_GPIO_PORT[pLoc->pstLedLocate->u8PortType-PORT_GPIOA], pLoc->pstLedLocate->u16GpioPin);
                #endif
                break;
            case led_work_moment_toggle:
                //led toggle
                #if (defined(GD32F4XX))
                (gpio_output_bit_get(STM32_GPIO_PORT[pLoc->pstLedLocate->u8PortType-PORT_GPIOA],pLoc->pstLedLocate->u16GpioPin)!=0)?\
                    gpio_bit_reset(STM32_GPIO_PORT[pLoc->pstLedLocate->u8PortType-PORT_GPIOA], pLoc->pstLedLocate->u16GpioPin):\
                    gpio_bit_set(STM32_GPIO_PORT[pLoc->pstLedLocate->u8PortType-PORT_GPIOA], pLoc->pstLedLocate->u16GpioPin);
                #else
                (GPIO_ReadOutputDataBit(STM32_GPIO_PORT[pLoc->pstLedLocate->u8PortType-PORT_GPIOA],pLoc->pstLedLocate->u16GpioPin)!=0)?\
                    GPIO_ResetBits(STM32_GPIO_PORT[pLoc->pstLedLocate->u8PortType-PORT_GPIOA], pLoc->pstLedLocate->u16GpioPin):\
                    GPIO_SetBits(STM32_GPIO_PORT[pLoc->pstLedLocate->u8PortType-PORT_GPIOA], pLoc->pstLedLocate->u16GpioPin);
                #endif
                break;
            default:
                break;
        }
    }
  }

  if(pRun->blink_toggle_time > 0)
  {
    pRun->blink_toggle_time --;
    if(pRun->blink_toggle_time == 0)
    {
      //led off
      #if (_EN_CHIP_PCA9539_USE == _MACRO_ENABLE)
      if(pLoc->pstLedLocate->u8PortType == PORT_PCA)
      {
        switch(led_status)
        {
                    #if _ENABLE_LED_WORK_MOMENT_3S
                    case led_work_moment_on_3s:
                    #endif
          case led_work_moment_on:
            //led off
            (pLoc->eLedOnLevel == ePinLevel_High)?\
              (pca_output_val &= (~(pLoc->pstLedLocate->u16GpioPin))):\
              (pca_output_val |= pLoc->pstLedLocate->u16GpioPin);
            break;
                    #if _ENABLE_LED_WORK_MOMENT_3S
                    case led_work_moment_off_3s:
                    #endif
          case led_work_moment_off:
            //led on
            (pLoc->eLedOnLevel == ePinLevel_High)?\
              (pca_output_val |= pLoc->pstLedLocate->u16GpioPin):\
              (pca_output_val &= (~(pLoc->pstLedLocate->u16GpioPin)));
            break;
          case led_work_moment_toggle:
            //led toggle
            ((pca_output_val&pLoc->pstLedLocate->u16GpioPin)!=0)?\
              (pca_output_val &= (~(pLoc->pstLedLocate->u16GpioPin))):\
              (pca_output_val |= pLoc->pstLedLocate->u16GpioPin);
            break;
          default:
            break;
        }
      }
      else
      #endif

      #if (_EN_CHIP_TM1623_USE == _MACRO_ENABLE)
      if(IS_TM1623_GRID(pLoc->pstLedLocate->u8PortType))
      {
        TM1623_ksg_reg_val[TM1623_DEV_INDEX(pLoc->pstLedLocate->u8PortType)][TM1623_PORT_G1xS18+TM1623_PORT_NUM(pLoc->pstLedLocate->u8PortType)] ^=
          (pLoc->pstLedLocate->u16GpioPin);
        switch(led_status)
        {
          #if _ENABLE_LED_WORK_MOMENT_3S
          case led_work_moment_on_3s:
          #endif
          case led_work_moment_on:
            //led off
            (pLoc->eLedOnLevel == ePinLevel_High)?\
              (TM1623_ksg_reg_val[TM1623_DEV_INDEX(pLoc->pstLedLocate->u8PortType)][TM1623_PORT_G1xS18+TM1623_PORT_NUM(pLoc->pstLedLocate->u8PortType)] &= (~(pLoc->pstLedLocate->u16GpioPin))):\
              (TM1623_ksg_reg_val[TM1623_DEV_INDEX(pLoc->pstLedLocate->u8PortType)][TM1623_PORT_G1xS18+TM1623_PORT_NUM(pLoc->pstLedLocate->u8PortType)] |= pLoc->pstLedLocate->u16GpioPin);
            break;
                    #if _ENABLE_LED_WORK_MOMENT_3S
                    case led_work_moment_off_3s:
                    #endif
          case led_work_moment_off:
            //led on
            (pLoc->eLedOnLevel == ePinLevel_High)?\
              (TM1623_ksg_reg_val[TM1623_DEV_INDEX(pLoc->pstLedLocate->u8PortType)][TM1623_PORT_G1xS18+TM1623_PORT_NUM(pLoc->pstLedLocate->u8PortType)] |= pLoc->pstLedLocate->u16GpioPin):\
              (TM1623_ksg_reg_val[TM1623_DEV_INDEX(pLoc->pstLedLocate->u8PortType)][TM1623_PORT_G1xS18+TM1623_PORT_NUM(pLoc->pstLedLocate->u8PortType)] &= (~(pLoc->pstLedLocate->u16GpioPin)));
            break;
          case led_work_moment_toggle:
            ((TM1623_ksg_reg_val[TM1623_DEV_INDEX(pLoc->pstLedLocate->u8PortType)][TM1623_PORT_G1xS18+TM1623_PORT_NUM(pLoc->pstLedLocate->u8PortType)]&pLoc->pstLedLocate->u16GpioPin)!=0)?\
              (TM1623_ksg_reg_val[TM1623_DEV_INDEX(pLoc->pstLedLocate->u8PortType)][TM1623_PORT_G1xS18+TM1623_PORT_NUM(pLoc->pstLedLocate->u8PortType)] &= (~(pLoc->pstLedLocate->u16GpioPin))):\
              (TM1623_ksg_reg_val[TM1623_DEV_INDEX(pLoc->pstLedLocate->u8PortType)][TM1623_PORT_G1xS18+TM1623_PORT_NUM(pLoc->pstLedLocate->u8PortType)] |= pLoc->pstLedLocate->u16GpioPin);
            break;
          default:
            break;
        }
      }
      else
      #endif

      if((pLoc->pstLedLocate->u8PortType >= PORT_GPIOA)&&(pLoc->pstLedLocate->u8PortType <= PORT_GPIOG))
      {
        switch(led_status)
        {
        #if _ENABLE_LED_WORK_MOMENT_3S
        case led_work_moment_on_3s:
        #endif
          case led_work_moment_on:
            //led off
            #if (defined(GD32F4XX))
            (pLoc->eLedOnLevel == ePinLevel_High)?\
              gpio_bit_reset(STM32_GPIO_PORT[pLoc->pstLedLocate->u8PortType-PORT_GPIOA], pLoc->pstLedLocate->u16GpioPin):\
              gpio_bit_set(STM32_GPIO_PORT[pLoc->pstLedLocate->u8PortType-PORT_GPIOA], pLoc->pstLedLocate->u16GpioPin);
            #else
            (pLoc->eLedOnLevel == ePinLevel_High)?\
              GPIO_ResetBits(STM32_GPIO_PORT[pLoc->pstLedLocate->u8PortType-PORT_GPIOA], pLoc->pstLedLocate->u16GpioPin):\
              GPIO_SetBits(STM32_GPIO_PORT[pLoc->pstLedLocate->u8PortType-PORT_GPIOA], pLoc->pstLedLocate->u16GpioPin);
            #endif
            break;
        #if _ENABLE_LED_WORK_MOMENT_3S
        case led_work_moment_off_3s:
        #endif
          case led_work_moment_off:
            //led on
            #if (defined(GD32F4XX))
            (pLoc->eLedOnLevel == ePinLevel_High)?\
              gpio_bit_set(STM32_GPIO_PORT[pLoc->pstLedLocate->u8PortType-PORT_GPIOA], pLoc->pstLedLocate->u16GpioPin):\
              gpio_bit_reset(STM32_GPIO_PORT[pLoc->pstLedLocate->u8PortType-PORT_GPIOA], pLoc->pstLedLocate->u16GpioPin);
            #else
            (pLoc->eLedOnLevel == ePinLevel_High)?\
              GPIO_SetBits(STM32_GPIO_PORT[pLoc->pstLedLocate->u8PortType-PORT_GPIOA], pLoc->pstLedLocate->u16GpioPin):\
              GPIO_ResetBits(STM32_GPIO_PORT[pLoc->pstLedLocate->u8PortType-PORT_GPIOA], pLoc->pstLedLocate->u16GpioPin);
            #endif
            break;
          case led_work_moment_toggle:
            //led toggle
            #if (defined(GD32F4XX))
            (gpio_output_bit_get(STM32_GPIO_PORT[pLoc->pstLedLocate->u8PortType-PORT_GPIOA],pLoc->pstLedLocate->u16GpioPin)!=0)?\
              gpio_bit_reset(STM32_GPIO_PORT[pLoc->pstLedLocate->u8PortType-PORT_GPIOA], pLoc->pstLedLocate->u16GpioPin):\
              gpio_bit_set(STM32_GPIO_PORT[pLoc->pstLedLocate->u8PortType-PORT_GPIOA], pLoc->pstLedLocate->u16GpioPin);
            #else
            (GPIO_ReadOutputDataBit(STM32_GPIO_PORT[pLoc->pstLedLocate->u8PortType-PORT_GPIOA],pLoc->pstLedLocate->u16GpioPin)!=0)?\
              GPIO_ResetBits(STM32_GPIO_PORT[pLoc->pstLedLocate->u8PortType-PORT_GPIOA], pLoc->pstLedLocate->u16GpioPin):\
              GPIO_SetBits(STM32_GPIO_PORT[pLoc->pstLedLocate->u8PortType-PORT_GPIOA], pLoc->pstLedLocate->u16GpioPin);
            #endif
            break;
          default:
            break;
        }
      }

      led_cur_state_set(led_index, led_work_none);
      pRun->pre_status = led_work_none;
      pRun->status = led_work_none;
    }
  }
}

/**led work moment on**/
void led_work_fun_moment_on(uint8_t led_index)
{
  led_work_fun_moment_state(led_index, led_work_moment_on, _TIME_INDICATIVE_LED_MOMENT_ON);
}

/**led work moment off**/
void led_work_fun_moment_off(uint8_t led_index)
{
  led_work_fun_moment_state(led_index, led_work_moment_off, _TIME_INDICATIVE_LED_MOMENT_OFF);
}

#if _ENABLE_LED_WORK_MOMENT_3S
void led_work_fun_moment_on_3s(uint8_t led_index)
{
  led_work_fun_moment_state(led_index, led_work_moment_on_3s, _TIME_INDICATIVE_LED_MOMENT_ON_3S);
}

/**led work moment off**/
void led_work_fun_moment_off_3s(uint8_t led_index)
{
  led_work_fun_moment_state(led_index, led_work_moment_off_3s, _TIME_INDICATIVE_LED_MOMENT_OFF_3S);
}
#endif

/**led work moment toggle**/
void led_work_fun_moment_toggle(uint8_t led_index)
{
  led_work_fun_moment_state(led_index, led_work_moment_toggle, _TIME_INDICATIVE_LED_MOMENT_TOGGLE);
}

void led_init(void)
{
  led_work_fun_register(led_work_always_off,    led_work_fun_off);
  led_work_fun_register(led_work_always_on,     led_work_fun_on);
  led_work_fun_register(led_work_moment_on,     led_work_fun_moment_on);
  led_work_fun_register(led_work_moment_off,    led_work_fun_moment_off);
  led_work_fun_register(led_work_moment_toggle, led_work_fun_moment_toggle);
  led_work_fun_register(led_work_norm_blink,    led_work_fun_norm_blink);
  led_work_fun_register(led_work_fast_blink,    led_work_fun_fast_blink);
  led_work_fun_register(led_work_slow_blink,    led_work_fun_slow_blink);
    #if _ENABLE_LED_WORK_MOMENT_3S
  led_work_fun_register(led_work_moment_on_3s,    led_work_fun_moment_on_3s);
  led_work_fun_register(led_work_moment_off_3s,   led_work_fun_moment_off_3s);
    #endif

  memset(led_work_status_table, 0, (sizeof(led_work_run_t)*NUM_OF_LEDS_MAX));
  memset(led_current_state, 0, (sizeof(led_work_status_t)*NUM_OF_LEDS_MAX));
}

/**led work status change**/
void led_work_status_assign(uint8_t led_name, led_work_status_t led_status)
{
  pLed_work_run_t ptLedWorkRunValue, pt;
#if _SYSTEM_SUPPORT_RTOS
  const TickType_t xTicksToWait = 0;//portMAX_DELAY;
#endif

  if(led_cur_state_get(led_name)==led_status) return;
  if((led_cur_state_get(led_name)==led_work_moment_on)&&(led_status==led_work_always_off)) return;
  if((led_cur_state_get(led_name)==led_work_moment_off)&&(led_status==led_work_always_on)) return;
    #if _ENABLE_LED_WORK_MOMENT_3S
  if((led_cur_state_get(led_name)==led_work_moment_on_3s)&&(led_status==led_work_always_off)) return;
  if((led_cur_state_get(led_name)==led_work_moment_off_3s)&&(led_status==led_work_always_on)) return;
    #endif
  led_cur_state_set(led_name, led_status);

  ptLedWorkRunValue = led_work_status_table;

  pt = ptLedWorkRunValue + led_name;
  pt->status = led_status;
  pt->pre_status = led_work_none;
  pt->blink_toggle_time = 0;
#if _SYSTEM_SUPPORT_RTOS
  xQueueSend(xQueue_Led, (void *)&ptLedWorkRunValue, xTicksToWait);
#endif
}

void led_cur_state_set(uint8_t led_name, led_work_status_t led_status)
{
  led_current_state[led_name] = led_status;
}

led_work_status_t led_cur_state_get(uint8_t led_name)
{
  return led_current_state[led_name];
}









