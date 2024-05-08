#ifndef __TASK_USBDECT_H_
#define __TASK_USBDECT_H_ 

#if (_EN_GPIO_USB_DETECT == _MACRO_ENABLE)

void vTaskUsbDect(void *pvParameters);
void UsbDectInit(void);

#endif //#if (_EN_GPIO_USB_DETECT == _MACRO_ENABLE)

#endif 






















