#include "includes.h"

#include "ms1826_videoprocess.h"

#ifndef STK_SIZE_TASK_MS1826
#define STK_SIZE_TASK_MS1826                        2048
#endif
#ifndef PRIORITY_TASK_MS1826
#define PRIORITY_TASK_MS1826                        3
#endif

vvFuncPoint pvvFuncMS1826UserFunctionInit = NULL;
vvFuncPoint pvvFuncMS1826UserFunctionProcess = NULL;
static bool sg_bMS1826HoldFlag = FALSE;

const stMS1826Config_T cg_stMs1826DevCfgTable[] =
{
#include <gd32f4xx.h>
    {_I2C_TABLE_INDEX_MS1826_0,_MS1826_WORK_MODE_0},
#if (DEV_NUM_MS1826>1)
    {_I2C_TABLE_INDEX_MS1826_1,_MS1826_WORK_MODE_1},
#endif
#if (DEV_NUM_MS1826>2)
    {_I2C_TABLE_INDEX_MS1826_2,_MS1826_WORK_MODE_2},
#endif
#if (DEV_NUM_MS1826>3)
    {_I2C_TABLE_INDEX_MS1826_3,_MS1826_WORK_MODE_3},
#endif
#if (DEV_NUM_MS1826>4)
    {_I2C_TABLE_INDEX_MS1826_4,_MS1826_WORK_MODE_4},
#endif
#if (DEV_NUM_MS1826>5)
    {_I2C_TABLE_INDEX_MS1826_5,_MS1826_WORK_MODE_5},
#endif
};

extern void MS1826_Reset(void);
void vTask_Ms1826(void *pvParameters)
{
    u8 l_u8OsdTimer = 0;
	(void)pvParameters;
    MS1826_Reset();

    for(g_u8MS1826DevIndex=0;g_u8MS1826DevIndex<DEV_NUM_MS1826;g_u8MS1826DevIndex++)
    {
        g_u8MS1826_I2CDevID = (u8)cg_stMs1826DevCfgTable[g_u8MS1826DevIndex].u8I2cIndex;
        app_ms1826_init(g_u8MS1826DevIndex,cg_stMs1826DevCfgTable[g_u8MS1826DevIndex].u8WorkMode);
        video_process_init(g_u8MS1826DevIndex);
    }
  	for(;;)
  	{
        for(g_u8MS1826DevIndex=0;g_u8MS1826DevIndex<DEV_NUM_MS1826;g_u8MS1826DevIndex++)
        {
            g_u8MS1826_I2CDevID = (u8)cg_stMs1826DevCfgTable[g_u8MS1826DevIndex].u8I2cIndex;
            video_process(g_u8MS1826DevIndex);
            app_MS1826FunctionProcess(g_u8MS1826DevIndex);
            vTaskDelay(1);
        }

  	}
}
void vTaskCreate_MS1826(void)
{
    xTaskCreate( vTask_Ms1826,
                 "vTask_Ms1826",
                 STK_SIZE_TASK_MS1826,
                 NULL,
                 PRIORITY_TASK_MS1826,
                 NULL );
}
 






















