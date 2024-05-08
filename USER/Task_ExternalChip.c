/******************************************************
**Copyright (c) 2008 PTN Electronics Limited.All Rights Reserved
**FileName:     Task_ExternalChip.c
**Writer:       wf8421
**Create Date:  2019-01-05
**Rewriter:     wf8421
**Rewrite Date: 2019-01-05
**Description:  外部简单控制芯片的task程序
**History:
**Version:      V1.0.0
******************************************************/
#define __TASK_EXTERNALCHIP_C__

#include "includes.h"
#if _ENABLE_EXTERNAL_CHIP_TASK
#if _ENABLE_CHIP_SIL9630
#include "Sii9630_main.h"
#endif

/*********************************************
**External chip Function
*********************************************/
void mapp_ExternalChip1MsTimer(void)
{
#if _ENABLE_CHIP_ITE66353
    it6635_Timer_Task();
#endif
}

#if _SYSTEM_SUPPORT_RTOS
void vTaskExternalChip(void *pvParameters)
{
    (void)pvParameters;

    #if _ENABLE_CHIP_ITE6615
    ITE6615_Init();
    #endif
    #if _ENABLE_CHIP_ITE66021
    it66021_init();
    #endif
    #if _ENABLE_CHIP_ITE66353
    it6635_init();
    #endif

    #if _ENABLE_CHIP_IDT5V49EE902||_ENABLE_CHIP_IDT5P49V6965
    IDT_5V_init();
    #endif
    #if _ENABLE_CHIP_SIL9630
    mapp_Sil9630Init();
    #endif

    #if _EN_CHIP_ADN4604_USE
    InitAdn4604();
    #endif

    #if _ENABLE_CHIP_ITE66121
    IT66121_init();
    #endif

    #if _ENABLE_CHIP_MS7200
    ms7200_init();
    #endif

    #if _EN_CHIP_LT8644_USE
    InitLT8644();
    #endif

    #if ENABLE_SERIES_LCD_FUNCTION
    initial_lcd();
    #endif

    for(;;)
    {
        #if _ENABLE_CHIP_ITE6615
        ITE6615_process();
        #endif
        #if _ENABLE_CHIP_ITE66021
        it66021_process();
        #endif
        #if _ENABLE_CHIP_IDT5V49EE902||_ENABLE_CHIP_IDT5P49V6965
        mapp_IDTProcess();
        #endif
        #if _ENABLE_CHIP_ITE66351
        it6635_dev_loop();
        #elif _ENABLE_CHIP_ITE66353
        it6635_dev_loop();
        #endif
        #if _EN_CHIP_ADN4604_USE
        mapp_ADN604Process();
        #endif
        #if _ENABLE_CHIP_ITE66121
        IT66121_Process();
        #endif
        #if _ENABLE_CHIP_MS7200
        ms7200_loop();
        #endif
        #if _EN_CHIP_LT8644_USE
        mapp_LT8644Process();
        #endif
        #if ENABLE_SERIES_LCD_FUNCTION
        lcd_main();
        #endif

        delay_ms(20);
    }
}
#else
void vTaskExternalChip(void *pvParameters)
{
    static u8 sl_u8LoopSta=0;
    (void)pvParameters;

    switch(sl_u8LoopSta)
    {
        case 0:
            {
                #if _ENABLE_CHIP_ITE6615
                ITE6615_Init();
                #endif
                #if _ENABLE_CHIP_ITE66021
                it66021_init();
                #endif
                #if _ENABLE_CHIP_ITE66353
                it6635_init();
                #endif

                #if _ENABLE_CHIP_IDT5V49EE902||_ENABLE_CHIP_IDT5P49V6965
                IDT_5V_init();
                #endif

                #if _EN_CHIP_ADN4604_USE
                InitAdn4604();
                #endif

                #if _ENABLE_CHIP_ITE66121
                IT66121_init();
                #endif

                #if _ENABLE_CHIP_MS7200
                ms7200_init();
                #endif
                #if _EN_CHIP_LT8644_USE
                InitLT8644();
                #endif

                #if ENABLE_SERIES_LCD_FUNCTION
                initial_lcd();
                #endif
                sl_u8LoopSta = 1;
            }
            break;
        case 1:
            {
                #if _ENABLE_CHIP_ITE6615
                ITE6615_process();
                #endif
                #if _ENABLE_CHIP_ITE66021
                it66021_process();
                #endif
                #if _ENABLE_CHIP_IDT5V49EE902||_ENABLE_CHIP_IDT5P49V6965
                mapp_IDTProcess();
                #endif
                #if _ENABLE_CHIP_ITE66351
                it6635_dev_loop();
                #elif _ENABLE_CHIP_ITE66353
                it6635_dev_loop();
                #endif
                #if _ENABLE_CHIP_SIL9630
                mapp_Sil9630_Task();
                #endif
                #if _EN_CHIP_ADN4604_USE
                mapp_ADN604Process();
                #endif
                #if _ENABLE_CHIP_ITE66121
                IT66121_Process();
                #endif
                #if _ENABLE_CHIP_MS7200
                ms7200_loop();
                #endif
                #if _EN_CHIP_LT8644_USE
                mapp_LT8644Process();
                #endif
                #if ENABLE_SERIES_LCD_FUNCTION
                lcd_main();
                #endif
            }
            break;
        default:
            sl_u8LoopSta = 0;
            break;
    }
}

#endif


#endif

