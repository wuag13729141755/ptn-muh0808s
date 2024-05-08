/**
  *****************************************************************************
  *
  * 项目代码：ITE SPLITER
  * 程序说明：主函数
  * 主控芯片：STM32F103RB,STM32F103RC,STM32F207VE
  * 晶振频率：8MHz(STM32F103RB,STM32F103RC),25MHz(STM32F207VE)
  * 硬件版本：V
  * 软件版本：V
  * 作    者：dengzhiqin
  * 编制日期：2017-11-27
  *
  *****************************************************************************
  *                                   Update
  * @Version : V2.1
  * @By      : wf8421
  * @Date    : 2021 / 02 / 24
  * @Brief   : add ADV7674 Chip Task
  *
  *****************************************************************************
**/
#include "includes.h"
volatile uint32_t ulHighFrequencyTimerTicks = 0UL;
#if _SYSTEM_SUPPORT_RTOS

#ifndef ENABLE_TASK_CREATE_IN_TASK
#define ENABLE_TASK_CREATE_IN_TASK              0
#endif
/*
**********************************************************************************************************
                                            宏定义
**********************************************************************************************************
*/
#ifndef STK_SIZE_TASK_CREATE
#define STK_SIZE_TASK_CREATE                        configMINIMAL_STACK_SIZE
#endif
#ifndef PRIORITY_TASK_CREATE
#define PRIORITY_TASK_CREATE                        1
#endif

#ifndef STK_SIZE_TASK_EEPROM
#define STK_SIZE_TASK_EEPROM                        512
#endif
#ifndef PRIORITY_TASK_EEPROM
#define PRIORITY_TASK_EEPROM                        3 // 1
#endif

#if (_EN_GPIO_USB_DETECT == _MACRO_ENABLE)
#ifndef STK_SIZE_TASK_USB_DETECT
#define STK_SIZE_TASK_USB_DETECT                    64
#endif
#ifndef PRIORITY_TASK_USB_DETECT
#define PRIORITY_TASK_USB_DETECT                    1
#endif
#endif

#ifndef STK_SIZE_TASK_UART
#define STK_SIZE_TASK_UART                          1024
#endif
#ifndef PRIORITY_TASK_UART
#define PRIORITY_TASK_UART                          6
#endif

#ifndef STK_SIZE_TASK_CEC
#define STK_SIZE_TASK_CEC                           128
#endif
#ifndef PRIORITY_TASK_CEC
#define PRIORITY_TASK_CEC                           6
#endif

#ifndef STK_SIZE_TASK_LED
#define STK_SIZE_TASK_LED                           128
#endif
#ifndef PRIORITY_TASK_LED
#define PRIORITY_TASK_LED                           4
#endif

#ifndef STK_SIZE_TASK_KEY
#define STK_SIZE_TASK_KEY                           256
#endif
#ifndef PRIORITY_TASK_KEY
#define PRIORITY_TASK_KEY                           5
#endif
#if (_IR_FUNCTION_USE != _IR_FUNCTION_NONE)
#ifndef STK_SIZE_TASK_IR
#define STK_SIZE_TASK_IR                            256
#endif
#ifndef PRIORITY_TASK_IR
#define PRIORITY_TASK_IR                            5
#endif
#endif

#if _ENABLE_STARTUP_TO_MATCHING_CHIPID
#ifndef STK_SIZE_TASK_CHID_ID_MATCH
#define STK_SIZE_TASK_CHID_ID_MATCH                 64
#endif
#ifndef PRIORITY_TASK_CHID_ID_MATCH
#define PRIORITY_TASK_CHID_ID_MATCH                 1
#endif
#endif

#if _ENABLE_EXTERNAL_CHIP_TASK
#ifndef STK_SIZE_TASK_EXTERNAL_CHIP
#define STK_SIZE_TASK_EXTERNAL_CHIP                 256
#endif
#ifndef PRIORITY_TASK_EXTERNAL_CHIP
#define PRIORITY_TASK_EXTERNAL_CHIP                 3
#endif
#endif

#if _ENABLE_CHIP_GSV_CHIP_SERIES
#ifndef STK_SIZE_TASK_GSV_CHIP
#define STK_SIZE_TASK_GSV_CHIP                      512
#endif
#ifndef PRIORITY_TASK_GSV_CHIP
#define PRIORITY_TASK_GSV_CHIP                      3
#endif
#endif

#if (_ENABLE_MAIN_TASK_ETHERNET == _MACRO_ENABLE)
#ifndef STK_SIZE_TASK_ETHERNET
#define STK_SIZE_TASK_ETHERNET                      2048//1024
#endif
#ifndef PRIORITY_TASK_ETHERNET
#define PRIORITY_TASK_ETHERNET                      2
#endif
#endif

#define WaitSysStable                               delay_ms
/*
**********************************************************************************************************
                                            函数声明
**********************************************************************************************************
*/
static void AppTaskCreate (void);
static void AppQueueCreate(void);
#if ENABLE_TASK_CREATE_IN_TASK
static void vTaskCreate(void *pvParameters);
#endif //add by wf8421

/*
**********************************************************************************************************
                                            变量声明
**********************************************************************************************************
*/
QueueHandle_t xQueue_Led = NULL;

QueueHandle_t xQueue_Key = NULL;

#if ((_IR_FUNCTION_USE&_IR_FUNCTION_KEY_STUDY)==_IR_FUNCTION_KEY_STUDY)
QueueHandle_t xQueue_IrStudy = NULL;
#endif

#if ((_IR_FUNCTION_USE&_IR_FUNCTION_REMOTE)==_IR_FUNCTION_REMOTE)
QueueHandle_t xQueue_IrDecode = NULL;
#endif

QueueHandle_t xQueue_CecTx = NULL;

QueueHandle_t xQueue_EProcess = NULL;

SemaphoreHandle_t xSemaphore_IIC[NUM_OF_I2C_BUS_MAX] = {NULL};

#if _ENABLE_SPI_FUNCTION
SemaphoreHandle_t xSemaphore_SPI[NUM_OF_SPI_BUS_MAX] = {NULL};
#endif

#if (configUSE_TIMERS==1)
extern void sys_tick_init(void);
#endif

/*
*********************************************************************************************************
*   函 数 名: main
*   功能说明: 标准c程序入口。
*   形    参：无
*   返 回 值: 无
*********************************************************************************************************
*/
int main(void)
{
#ifndef DEBUG_MODE_EN
    #ifdef STM32F2XX_HD
    //NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0xD000);
    #elif (defined(N32G45X))
    NVIC_SetVectorTable(NVIC_VectTab_FLASH, FW_VECT_OFFSET);
    #elif (defined(GD32F4XX))
    nvic_vector_table_set(NVIC_VECTTAB_FLASH,FW_VECT_OFFSET);
    #else
    NVIC_SetVectorTable(NVIC_VectTab_FLASH, FW_VECT_OFFSET);
    #endif
#endif

    /*
      在启动调度前，为了防止初始化STM32外设时有中断服务程序执行，这里禁止全局中断(除了NMI和HardFault)。
      这样做的好处是：
      1. 防止执行的中断服务程序中有FreeRTOS的API函数。
      2. 保证系统1正常启动，不受别的中断影响。
      3. 关于是否关闭全局中断，大家根据自己的实际情况设置即可。
      在移植文件port.c中的函数prvStartFirstTask中会重新开启全局中断。通过指令cpsie i开启，__set_PRIMASK(1)
      和cpsie i是等效的。

     */
    __set_PRIMASK(1);

    /* 硬件初始化 */
    BSP_Init();
#if (configUSE_TIMERS==1)
    sys_tick_init();
#endif
    /* 创建消息队列 */
    AppQueueCreate();

    /* 创建任务 */
    AppTaskCreate();

    /* 启动调度，开始执行任务 */
    vTaskStartScheduler();

    /*
      如果系统正常启动是不会运行到这里的，运行到这里极有可能是用于定时器任务或者空闲任务的
      heap空间不足造成创建失败，此要加大FreeRTOSConfig.h文件中定义的heap大小：
      #define configTOTAL_HEAP_SIZE       ( ( size_t ) ( 17 * 1024 ) )
    */
    while(1);
}

/*
*********************************************************************************************************
*   函 数 名: AppQueueCreate
*   功能说明: 创建关联队列
*   形    参：无
*   返 回 值: 无
*********************************************************************************************************
*/
static void AppQueueCreate(void)
{
    uint8_t i=0;

    xQueue_Led = xQueueCreate(1, sizeof(pLed_work_run_t));
    if(xQueue_Led == 0)
    {
        Msg_SemC(("xQueue_Led create failed\r\n"));
    }

    xQueue_Key = xQueueCreate(1, sizeof(pkey_val_stored_t));
    if(xQueue_Key == 0)
    {
        Msg_SemC(("xQueue_Key create failed\r\n"));
    }

    #if ((_IR_FUNCTION_USE&_IR_FUNCTION_KEY_STUDY)==_IR_FUNCTION_KEY_STUDY)
    xQueue_IrStudy = xQueueCreate(1, sizeof(pstIrStudy_t));
    if(xQueue_IrStudy == 0)
    {
        Msg_SemC(("xQueue_IrStudy create failed\r\n"));
    }
    #endif

    #if ((_IR_FUNCTION_USE&_IR_FUNCTION_REMOTE)==_IR_FUNCTION_REMOTE)
    xQueue_IrDecode = xQueueCreate(1, sizeof(pstIrDecode_t));
    if(xQueue_IrDecode == 0)
    {
        Msg_SemC(("xQueue_IrDecode create failed\r\n"));
    }
    #endif

    xQueue_CecTx = xQueueCreate(1, sizeof(pstCecTxMsg_t));
    if(xQueue_CecTx == 0)
    {
        Msg_SemC(("xQueue_CecTx create failed\r\n"));
    }

    xQueue_EProcess = xQueueCreate(1, sizeof(pstEepDataSave_t));
    if(xQueue_EProcess == 0)
    {
        Msg_SemC(("xQueue_EProcess create failed\r\n"));
    }

    for(i=0; i<NUM_OF_I2C_BUS_MAX; i++)
    {
        xSemaphore_IIC[i] = xSemaphoreCreateMutex();
    }

    #if _ENABLE_SPI_FUNCTION
    for(i=0; i<NUM_OF_SPI_BUS_MAX; i++)
    {
        xSemaphore_SPI[i] = xSemaphoreCreateMutex();
    }
    #endif
}

/*
*********************************************************************************************************
*   函 数 名: AppTaskCreate
*   功能说明: 创建应用任务
*   形    参：无
*   返 回 值: 无
*********************************************************************************************************
*/
static void AppTaskCreate(void)
{
#if ENABLE_TASK_CREATE_IN_TASK
    xTaskCreate(vTaskCreate,
                "vTaskCreate",
                STK_SIZE_TASK_CREATE,
                NULL,
                PRIORITY_TASK_CREATE,
                NULL);
#else //add by wf8421

    WaitSysStable(BootWaitTime);//1000ms

    SystemVarInit();
    ManufLogoPrintf();
#if _ENABLE_TO_MATCHING_ALPU_C
    alpu_func();
#endif

    #if _EN_MAIN_TASK_LED
    xTaskCreate( vTaskLED,          /* 任务函数  */
                 "vTaskLED",        /* 任务名    */
                 STK_SIZE_TASK_LED, /* 任务栈大小，单位word，也就是4字节 */
                 NULL,              /* 任务参数  */
                 PRIORITY_TASK_LED, /* 任务优先级*/
                 NULL ); /* 任务句柄  */
    #endif

    #if _EN_MAIN_TASK_EEPROM
    xTaskCreate( vTaskEeprom,
                 "vTaskEeprom",
                 STK_SIZE_TASK_EEPROM,
                 NULL,
                 PRIORITY_TASK_EEPROM,
                 NULL );
    #endif

    #if (_EN_GPIO_USB_DETECT == _MACRO_ENABLE)
    xTaskCreate( vTaskUsbDect,
                 "vTaskUsbDect",
                 STK_SIZE_TASK_USB_DETECT,
                 NULL,
                 PRIORITY_TASK_USB_DETECT,
                 NULL );
    #endif

    #if _ENABLE_MAIN_TASK_KEY
    xTaskCreate( vTaskKey,
                "vTaskKey",
                STK_SIZE_TASK_KEY,
                NULL,
                PRIORITY_TASK_KEY,
                NULL);
    #endif

    #if (_IR_FUNCTION_USE != _IR_FUNCTION_NONE)
    xTaskCreate( vTaskIR,
                "vTaskIR",
                STK_SIZE_TASK_IR,
                NULL,
                PRIORITY_TASK_IR,
                NULL);
    #endif

    #if _ENABLE_MAIN_TASK_UART
    xTaskCreate( vTaskUart,
                "vTaskUart",
                STK_SIZE_TASK_UART,
                NULL,
                PRIORITY_TASK_UART,
                NULL);
    #endif

    #if (_ENABLE_CEC_FUNCTION_USE == _MACRO_ENABLE)
    xTaskCreate( vTaskCEC,
                "vTaskCEC",
                STK_SIZE_TASK_CEC,
                NULL,
                PRIORITY_TASK_CEC,
                NULL);
    #endif

    #if ((_ENABLE_STARTUP_TO_MATCHING_CHIPID == _MACRO_ENABLE)&&(!defined(DEBUG_MODE_EN)))
    xTaskCreate( vTaskCheckChipID,
                 "vTaskCheckChipID",
                 STK_SIZE_TASK_CHID_ID_MATCH,
                 NULL,
                 PRIORITY_TASK_CHID_ID_MATCH,
                 NULL );
    #endif

    #if _ENABLE_EXTERNAL_CHIP_TASK
    xTaskCreate( vTaskExternalChip,
                 "vTaskExternalChip",
                 STK_SIZE_TASK_EXTERNAL_CHIP,
                 NULL,
                 PRIORITY_TASK_EXTERNAL_CHIP,
                 NULL );
    #endif

    #if _ENABLE_CHIP_GSV_CHIP_SERIES
    xTaskCreate( vTask_GsvSeriers,
                 "vTask_GsvSeriers",
                 STK_SIZE_TASK_GSV_CHIP,
                 NULL,
                 PRIORITY_TASK_GSV_CHIP,
                 NULL );
    #endif

    #if (_ENABLE_MAIN_TASK_ETHERNET == _MACRO_ENABLE)
    xTaskCreate( vTask_Ethernet,
                 "vTask_Ethernet",
                 STK_SIZE_TASK_ETHERNET,
                 NULL,
                 PRIORITY_TASK_ETHERNET,
                 NULL );
    #endif

    #if (_ENABLE_CHIP_MS1826 == _MACRO_ENABLE)
    vTaskCreate_MS1826();
    #endif

#endif //add by wf8421
}

#if ENABLE_TASK_CREATE_IN_TASK
//the task of create tasks
static void vTaskCreate(void *pvParameters)
{
    (void)pvParameters;

    WaitSysStable(BootWaitTime);//1000ms

    SystemVarInit();
    ManufLogoPrintf();

    #if _EN_MAIN_TASK_LED
    xTaskCreate( vTaskLED,          /* 任务函数  */
                 "vTaskLED",        /* 任务名    */
                 STK_SIZE_TASK_LED, /* 任务栈大小，单位word，也就是4字节 */
                 NULL,              /* 任务参数  */
                 PRIORITY_TASK_LED, /* 任务优先级*/
                 NULL ); /* 任务句柄  */
    #endif

    #if _EN_MAIN_TASK_EEPROM
    xTaskCreate( vTaskEeprom,
                 "vTaskEeprom",
                 STK_SIZE_TASK_EEPROM,
                 NULL,
                 PRIORITY_TASK_EEPROM,
                 NULL );
    #endif

    #if (_EN_GPIO_USB_DETECT == _MACRO_ENABLE)
    xTaskCreate( vTaskUsbDect,
                 "vTaskUsbDect",
                 STK_SIZE_TASK_USB_DETECT,
                 NULL,
                 PRIORITY_TASK_USB_DETECT,
                 NULL );
    #endif

    #if _ENABLE_MAIN_TASK_KEY
    xTaskCreate( vTaskKey,
                "vTaskKey",
                STK_SIZE_TASK_KEY,
                NULL,
                PRIORITY_TASK_KEY,
                NULL);
    #endif

    #if (_IR_FUNCTION_USE != _IR_FUNCTION_NONE)
    xTaskCreate( vTaskIR,
                "vTaskIR",
                STK_SIZE_TASK_IR,
                NULL,
                PRIORITY_TASK_IR,
                NULL);
    #endif

    #if _ENABLE_MAIN_TASK_UART
    xTaskCreate( vTaskUart,
                "vTaskUart",
                STK_SIZE_TASK_UART,
                NULL,
                PRIORITY_TASK_UART,
                NULL);
    #endif

    #if (_ENABLE_CEC_FUNCTION_USE == _MACRO_ENABLE)
    xTaskCreate( vTaskCEC,
                "vTaskCEC",
                STK_SIZE_TASK_CEC,
                NULL,
                PRIORITY_TASK_CEC,
                NULL);
    #endif

    #if ((_ENABLE_STARTUP_TO_MATCHING_CHIPID == _MACRO_ENABLE)&&(!defined(DEBUG_MODE_EN)))
    xTaskCreate( vTaskCheckChipID,
                 "vTaskCheckChipID",
                 STK_SIZE_TASK_CHID_ID_MATCH,
                 NULL,
                 PRIORITY_TASK_CHID_ID_MATCH,
                 NULL );
    #endif

    #if _ENABLE_EXTERNAL_CHIP_TASK
    xTaskCreate( vTaskExternalChip,
                 "vTaskExternalChip",
                 STK_SIZE_TASK_EXTERNAL_CHIP,
                 NULL,
                 PRIORITY_TASK_EXTERNAL_CHIP,
                 NULL );
    #endif

    #if _ENABLE_CHIP_GSV_CHIP_SERIES
    xTaskCreate( vTask_GsvSeriers,
                 "vTask_GsvSeriers",
                 STK_SIZE_TASK_GSV_CHIP,
                 NULL,
                 PRIORITY_TASK_GSV_CHIP,
                 NULL );
    #endif

    vTaskDelete(NULL);
}
#endif //add by wf8421

#else
int main(void)
{

#ifndef DEBUG_MODE_EN
    #ifdef STM32F2XX_HD
        //NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0xD000);
    #elif (defined(N32G45X))
        NVIC_SetVectorTable(NVIC_VectTab_FLASH, FW_VECT_OFFSET);
    #elif (defined(GD32F4XX))
        nvic_vector_table_set(NVIC_VECTTAB_FLASH,FW_VECT_OFFSET);
    #else
        NVIC_SetVectorTable(NVIC_VectTab_FLASH, FW_VECT_OFFSET);
    #endif
#endif
    /* 硬件初始化 */
    BSP_Init();

    SystemVarInit();
    ManufLogoPrintf();
#if _ENABLE_TO_MATCHING_ALPU_C
    alpu_func();
#endif
#if _EN_MAIN_TASK_LED
    TaskLedInit();
#endif
#if (_ENABLE_CEC_FUNCTION_USE == _MACRO_ENABLE)
    appCecInit();
#endif

    while(1)
    {
        #if _EN_MAIN_TASK_LED
        vTaskLED(NULL);
        #endif

        #if _EN_MAIN_TASK_EEPROM
        vTaskEeprom(NULL);
        #endif

        #if 0//(_EN_GPIO_USB_DETECT == _MACRO_ENABLE)
        vTaskUsbDect(NULL);
        #endif

        #if _ENABLE_MAIN_TASK_KEY
        vTaskKey(NULL);
        #endif

        #if 0//(_IR_FUNCTION_USE != _IR_FUNCTION_NONE)
        vTaskIR(NULL);
        #endif

        #if _ENABLE_MAIN_TASK_UART
        vTaskUart(NULL);
        #endif

        #if (_ENABLE_CEC_FUNCTION_USE == _MACRO_ENABLE)
        vTaskCEC(NULL);
        #endif

        #if 0//((_ENABLE_STARTUP_TO_MATCHING_CHIPID == _MACRO_ENABLE)&&(!defined(DEBUG_MODE_EN)))
        vTaskCheckChipID(NULL);
        #endif

        #if _ENABLE_EXTERNAL_CHIP_TASK
        vTaskExternalChip(NULL);
        #endif

        #if _ENABLE_CHIP_GSV_CHIP_SERIES
        vTask_GsvSeriers(NULL);
        #endif

        #if (_ENABLE_MAIN_TASK_ETHERNET == _MACRO_ENABLE)
        vTask_Ethernet(NULL);
        #endif

        #if (_ENABLE_CHIP_MS1826 == _MACRO_ENABLE)
        vTask_Ms1826(NULL);
        #endif
    }
}
#endif



