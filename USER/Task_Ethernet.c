#include "includes.h"
#if (_ENABLE_MAIN_TASK_ETHERNET == _MACRO_ENABLE)
#include "ethnet.h"
#if _SYSTEM_SUPPORT_RTOS
void vTask_Ethernet(void *pvParameters)
{
	(void)pvParameters;

	for(;;)
	{
        ethnet_Process();
		vTaskDelay(10);
	}
}
#else
void vTask_Ethernet(void *pvParameters)
{
	(void)pvParameters;

    ethnet_Process();
}

#endif

#endif

