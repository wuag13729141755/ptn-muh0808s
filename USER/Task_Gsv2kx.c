#include "includes.h"

#if _ENABLE_CHIP_GSV_CHIP_SERIES
#include "av_main.h"
vvFuncPoint pvvFuncGsvUserFunctionInit = NULL;
vvFuncPoint pvvFuncGsvUserFunctionProcess = NULL;


#if _SYSTEM_SUPPORT_RTOS
void vTask_GsvSeriers(void *pvParameters)
{
	(void)pvParameters;

    Gsv_Init();
    if(pvvFuncGsvUserFunctionInit != NULL)
    {
        pvvFuncGsvUserFunctionInit();
    }
  	/* Infinite loop */
  	while (1)
  	{
        GsvMain();
        #if AvAllowHpdLowPixelClockRead
        mapp_GsvPowerManage();
        #endif
        if(pvvFuncGsvUserFunctionProcess != NULL)
        {
            pvvFuncGsvUserFunctionProcess();
        }
        vTaskDelay(10);
  	}
}
#else
void vTask_GsvSeriers(void *pvParameters)
{
    static u8 sl_u8LoopSta=0;
	(void)pvParameters;

    switch(sl_u8LoopSta)
    {
        default:
        case 0;
            {
                Gsv_Init();
                if(pvvFuncGsvUserFunctionInit != NULL)
                {
                    pvvFuncGsvUserFunctionInit();
                }
                sl_u8LoopSta=1;
            }
            break;
        case 1:
            {
                /* Infinite loop */
                GsvMain();
                #if AvAllowHpdLowPixelClockRead
                mapp_GsvPowerManage();
                #endif
                if(pvvFuncGsvUserFunctionProcess != NULL)
                {
                    pvvFuncGsvUserFunctionProcess();
                }
            }
            break;
    }
}

#endif

#endif

