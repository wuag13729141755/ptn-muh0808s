/**
 *****************************************************************************
  *                                   Update
  * @Version : V1.0
  * @By      : dengzhiqin
  * @Date    : 2017 / 12 / 05
  * @Brief   : task of ITE66321
  *
  *****************************************************************************
**/
#include "includes.h"
#if(_PRJ_TYPE_SWITCH_SPLITER==_PRJ_SWITCHER_AND_SPLITER)||(_PRJ_TYPE_SWITCH_SPLITER==_PRJ_SWITCHER_ONLY)
#if (IT663XX_VER != IT663XX_NONE)
iTE_u8 	pre_gCurSysStatus = 0;
bool	g_bSuspendIt663xx = FALSE;//suspendit663xx
extern void It663xxIrq_xSemaphoreInit(void);
extern void It663xxSysInit_xSemaphoreInit(void);

void vTaskITE663XX(void *pvParameters)
{
	(void)pvParameters;
	#if(IT663XX_VER != IT663XX_VER_1_07)
    It663xxIrq_xSemaphoreInit();
    It663xxSysInit_xSemaphoreInit();
	IT6634_GlobalVarInit();
	#endif
	IT66341_Initialize();
	#if(SUH4AU_H2_EDID_POWER_ON_NO_EDID == _MACRO_ENABLE)
	//IT663XXProResumeOrSuspend(TRUE);
	#endif
	for(;;)
	{
		//KeyMessage_Main();

		if(isIT663XXProSuspend())
		{
			SwitcherSuspend_Running();
			vTaskDelay(50);
			continue;
		}

		IT6634_Main();
		IT6634_AuxFuncWork();

		vTaskDelay(50);
	}
}

bool isIT663XXProSuspend(void)
{
	return g_bSuspendIt663xx;
}

void IT663XXProResumeOrSuspend(bool bSta)
{
	g_bSuspendIt663xx = bSta;
}

void IT6634_AuxFuncWork(void)
{
	SwitcherOnly_Running();

	#if ((_EN_CEC_POWER_ON_TV_DETECTED_SIGNAL==_MACRO_ENABLE)&&(_PRJ_TYPE_SWITCH_SPLITER==_PRJ_SWITCHER_ONLY))
	IT663XX_PwrOnTvDtdSngl();
	#endif

	IT663XX_TxOutDisOrEn();
	IT663XX_TxAdoDisOrEn();
}

void IT663XX_TxOutDisOrEn(void)
{
	if((it66341_tx_out_status == tx_out_enable_stu)&&(it66341_tx_out_status!=pre_it66341_tx_out_status))
	{
		pre_it66341_tx_out_status = tx_out_enable_stu;
		if(gCurSysStatus==4)
		{
			pITE663xx_STM32_api->TxOutEnable();
			gCurSysStatus = pre_gCurSysStatus;
		}
	}
	else if(it66341_tx_out_status == tx_out_disable_stu)
	{
		if(gCurSysStatus != 4)
		{
			pre_gCurSysStatus = gCurSysStatus;
			gCurSysStatus = 4;
		}
	}
}

void IT663XX_TxAdoDisOrEn(void)
{
	if((it66341_tx_ado_status == tx_out_enable_stu)&&(it66341_tx_ado_status!=pre_it66341_tx_ado_status))
	{
		pre_it66341_tx_ado_status = tx_out_enable_stu;
		pITE663xx_STM32_api->TxAdoEnable();
	}
	else if((it66341_tx_ado_status == tx_out_disable_stu)&&(it66341_tx_ado_status!=pre_it66341_tx_ado_status))
	{
		pre_it66341_tx_ado_status = tx_out_disable_stu;
		pITE663xx_STM32_api->TxAdoDisable();
	}
}

#endif  //#if (IT663XX_VER != IT663XX_NONE)

#endif //#if(_PRJ_TYPE_SWITCH_SPLITER==_PRJ_SWITCHER_AND_SPLITER)||(_PRJ_TYPE_SWITCH_SPLITER==_PRJ_SWITCHER_ONLY)











