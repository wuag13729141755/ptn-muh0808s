#include "includes.h"

#if (_ENABLE_PANASONIC_CHIP_MN86478X == _MACRO_ENABLE)
#include "../USER/VideoChip_Mn86478x/Src/apl/Hdmilink.h"
#include "../USER/VideoChip_Mn86478x/Src/HDMIDrv/drvctl/include/HDMIDrv_drvctl.h"

volatile uint32_t 				g_u32SysTick;

/* Variables -----------------------------------------------------------------*/
uint32_t 						g_u32MN788aTimer = 0;

uint8_t 						LowPowerFlag = 0;
uint8_t 						ResetBuffer = 0;
bool 							g_bCecApplyEn = FALSE;
vvFuncPoint                     pvvFunc787UserFunctionInit = NULL;
/* Function prototypes -------------------------------------------------------*/
extern void Reset_787(void);
void OSD_Polling(unsigned char GenId);
void ModuleControl(uint8_t u8DevId);
extern unsigned char HDMI_CheckLowPower(void);
unsigned char CEC_Initialize(unsigned char devId);
void CEC_Application(unsigned char devId, unsigned char Id);

void vTaskVideoC1(void *pvParameters)
{
	uint8_t 					i;
	uint32_t					u32StartTime, u32DiffTime;

	#if (_ENABLE_MN86478X_CEC_APPLICATION == _MACRO_ENABLE)
	uint32_t 					u32StartCecCnt = 0;
	#endif

	HDMIDRV_RESULT				ret;
	#if 0
	HdmiDrvInf					LinkInf;
	uint8_t 					TimeBuffer = 0;
	#endif

	#if (_ENABLE_MN86478X_CEC_APPLICATION == _MACRO_ENABLE)
	unsigned char 				CECId;
	#endif

	(void)pvParameters;

	HW_RST:
  	Reset_787();

  	ret = HDMIDRV_RESULT_SUCCESS;

  	for (i = 0; i < HDMIDRV_DEVICE_NUMBER; i++)
  	{
    	ret |= HDMI_Init(i);
  	}

	#if (_ENABLE_MN86478X_CEC_APPLICATION == _MACRO_ENABLE)
	CECId = CEC_Initialize(0);
	#endif

  	if(ret == HDMIDRV_RESULT_SUCCESS)
  	{
    	Msg_VideoC1(("[HDMI] S/W Init : Success.\n"));
  	}
  	else
  	{
    	Msg_VideoC1(("[HDMI] S/W Init : Fail.\n"));
    	goto HW_RST;
  	}
    if(pvvFunc787UserFunctionInit != NULL)
    {
        pvvFunc787UserFunctionInit();
    }

	u32StartTime = HDMIDrv_GetTime();

  	/* Infinite loop */
  	while (1)
  	{
  		u32DiffTime = HdmiDrvFwCalcTime(u32StartTime);
	  	if(u32DiffTime >= 18)
	  	{
	  		u32StartTime = HDMIDrv_GetTime();

		  	if (LowPowerFlag == 0)
		  	{
			  	ret = HDMIDRV_RESULT_SUCCESS;
			  	for (i = 0; i < HDMIDRV_DEVICE_NUMBER; i++)
			  	{
				  	ret |= HDMI_Polling(i);
			  	}
			  	if (ret != HDMIDRV_RESULT_SUCCESS)
			  	{
				  	break;
			  	}

			  	///* ---------- CEC ---------- */
				//#if (_ENABLE_MN86478X_CEC_APPLICATION == _MACRO_ENABLE)
			  	//{
				//   	CEC_Application(0, CECId);
			  	//}
				//#endif

			  	/* ---------- OSD ---------- */
			  	//for (i = 0; i < 2; i++)
			  	{
				  	//OSD_Polling(i);
				  	//OSD_Polling(0);
			  	}

					#if 0
					TimeBuffer++;
					if (TimeBuffer >= 50) // Auto Switch Rx Port every 1s once Input Signal Invalid
					{
						ret = HDMIDrv_LinkInfo(0, 0, &LinkInf);
						if (ret == HDMIDRV_RESULT_SUCCESS)
						{
							if (LinkInf.RxInf->videoState != HDMIDRV_VIDEOSTATE_VALID)
							{
								HDMI_AutoRxPortSwitch(0, 0);
							}
						}
						TimeBuffer = 0;
					}
					#endif
		  	}
		  	else
		  	{
			  	if (LowPowerFlag > 1)
			  	{
                    LowPowerFlag = 0;
				  	goto HW_RST;
			  	}
			  	else
			  	{
				  	ResetBuffer++;
				  	if (ResetBuffer > 50)
				  	{
					  	i = HDMI_CheckLowPower();
					  	//printf("[HDMI] CheckLowPower ret = 0x%02x\n", i);
					  	ResetBuffer = 0;
				  	}
			  	}
		  	}
	  	}
	  	else
	  	{
		  	ModuleControl(0);
	  	}

		/* ---------- CEC ---------- */
		#if (_ENABLE_MN86478X_CEC_APPLICATION == _MACRO_ENABLE)
		{
			if(u32StartCecCnt < 3000) u32StartCecCnt++;

			if(u32StartCecCnt >= 3000)
			{
				if(g_bCecApplyEn) CEC_Application(0, CECId);
			}
		}
		#endif

		fCallTaskC1Loop();

		vTaskDelay(1);
  	}
}

void HAL_IncTick(void)
{
	g_u32SysTick ++;
}

uint32_t HAL_GetTick(void)
{
	return g_u32SysTick;
}

#endif  // #if (INC_TASK_VIDEOCHIP1 == _MACRO_ENABLE)























