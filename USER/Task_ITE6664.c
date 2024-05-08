/**
 *****************************************************************************
  * Update
  * @Version : V1.0
  * @By      : dengzhiqin
  * @Date    : 2017 / 12 / 05
  * @Brief   : task of ITE6664
  *
  *****************************************************************************
**/
#include "includes.h"
#if ((IT666X_VER != IT666X_NONE)&&((_PRJ_TYPE_SWITCH_SPLITER==_PRJ_SWITCHER_AND_SPLITER)||(_PRJ_TYPE_SWITCH_SPLITER==_PRJ_SPLITER_ONLY)))

void vTaskITE6664(void *pvParameters)
{
	(void)pvParameters;

	IT6664_Initial();

	for(;;)
	{
		//#if (_PRJ_TYPE_SWITCH_SPLITER==_PRJ_SPLITER_ONLY)
		//KeyMessage_Main();
		//#endif

		IT6664_MainLoop();

		IT666X_AuxFuncWork();

		vTaskDelay(50);
	}
}

void IT666X_AuxFuncWork(void)
{
	SpliterOnly_Running();

	it6664_edid_update();

	it6664_led_work();
}

#endif //#if (_PRJ_TYPE_SWITCH_SPLITER==_PRJ_SWITCHER_AND_SPLITER)||(_PRJ_TYPE_SWITCH_SPLITER==_PRJ_SPLITER_ONLY)

void DelayXms(uint16_t bMs)
{
	delay_ms(bMs);
	//delay_ms(bMs);
}

void mDelay(iTE_u16 Delay_Count)
{
	DelayXms(Delay_Count);
}

void mSleep(iTE_u16 Delay_Count)
{
	DelayXms(Delay_Count);
}





