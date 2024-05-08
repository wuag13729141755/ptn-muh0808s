#include "includes.h"

#if (_EN_GPIO_USB_DETECT == _MACRO_ENABLE)

void vTaskUsbDect(void *pvParameters)
{
	(void)pvParameters;

	UsbDectInit();
	for(;;)
	{
		if(mapp_UsbInputDetect())
		{
			//software reset mcu
			NVIC_SystemReset();
		}
		vTaskDelay(10);
	}
}

void UsbDectInit(void)
{

}

#endif
























