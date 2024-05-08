/**
 *****************************************************************************
  * Update
  * @Version : V1.0
  * @By      : dengzhiqin
  * @Date    : 2017 / 12 / 05
  * @Brief   : task of keyboard
  *
  *****************************************************************************
**/
#include "includes.h"

#if !(_ENABLE_NEW_USER_KEY_DEFINE_FUNCTION)
stKeyRunning_t s_tBtn[NUM_OF_KEYS_MAX];
#endif
key_val_stored_t s_tValStd;
bool g_bKeyScanSuspend = FALSE;//suspend key scan

extern QueueHandle_t xQueue_Key;
extern const stKeyDeclare_t stProjectKeyTable[];

#if _SYSTEM_SUPPORT_RTOS
void vTaskKey(void *pvParameters)
{
    #if !(_ENABLE_NEW_USER_KEY_DEFINE_FUNCTION)
	pstKeyDeclare_t	pKeyN;
	pstKeyDeclare_t pKeyS=(pstKeyDeclare_t)&stProjectKeyTable[0];
    #endif
	(void)pvParameters;

#if !(_ENABLE_NEW_USER_KEY_DEFINE_FUNCTION)
	key_var_init();
#endif

	#if (Power_System_Delay_3_Sec)
	delay_ms(3000);
	#endif

	for(;;)
	{
		if(isKeyProSuspend())
		{
			vTaskDelay(10);
			continue;
		}

        #if _ENABLE_NEW_USER_KEY_DEFINE_FUNCTION
        mapp_KeypadScan();
        #else
		pKeyN = pKeyS;
		while(pKeyN->pstKeyLocate[0] != NULL)
 		{
			key_detect(pKeyN-pKeyS);
			pKeyN ++;
		}
        #endif

		vTaskDelay(10);
	}
}
#else
void vTaskKey(void *pvParameters)
{
	(void)pvParameters;

	if(isKeyProSuspend())
	{
        return;
	}

    #if _ENABLE_NEW_USER_KEY_DEFINE_FUNCTION
    mapp_KeypadScan();
    #endif

}
#endif

bool isKeyProSuspend(void)
{
	return g_bKeyScanSuspend;
}

void SetKeyProSuspendOrResume(bool bSta)
{
	g_bKeyScanSuspend = bSta;
}

void KeyStoreValSet(uint8_t val, eKeyValSource_t eKeySrc, uint8_t u8KeyName, bool bSndToRspne)
{
#if _SYSTEM_SUPPORT_RTOS
	const TickType_t xTicksToWait = 0;//portMAX_DELAY;
#endif
	pkey_val_stored_t pStd = &s_tValStd;

	pStd->StStoreVal[u8KeyName].u8StoreVal = val;
	pStd->StStoreVal[u8KeyName].eValSource = eKeySrc;
	pStd->valid_key_name = u8KeyName;

	if(bSndToRspne)
	{
#if _SYSTEM_SUPPORT_RTOS
		xQueueSend(xQueue_Key, (void *)&pStd, xTicksToWait);
#endif
	}
}

void key_store_val(uint8_t val, eKeyValSource_t eKeySrc, uint8_t key_name)
{
	KeyStoreValSet(val,eKeySrc,key_name,TRUE);
}

#if !(_ENABLE_NEW_USER_KEY_DEFINE_FUNCTION)
void key_var_init(void)
{
	uint8_t i;

	memset(s_tBtn, 0, sizeof(s_tBtn));
	for(i=0; i<NUM_OF_KEYS_MAX; i++)
	{
		s_tBtn[i].u8ScanValPre = 0xFF;
	}
}

uint16_t key_scan(pstKeyDeclare_t pKey, pstKeyRunning_t pKeyRun)
{
	uint8_t 			u8IoNum = 0, i;
	uint16_t 			u16ScanVal = 0xFF;
	pstGpioConfig_t 	*pIoN, *pIoS = &pKey->pstKeyLocate[0];
	pstGpioConfig_t		pIoNext;

	pIoN = pIoS;
	while(((pIoN-pIoS)<_NUM_SIMUL_GPIO_PINS_ONE_KEY_MAX)&&(*pIoN!=NULL))
	{
		u8IoNum ++;
		pIoN ++;
	}

	if(u8IoNum == 1)
	{
		if((pKey->pstKeyLocate[0]->u8PortType>=PORT_GPIOA)&&(pKey->pstKeyLocate[0]->u8PortType<=PORT_GPIOG))
		{
            #if (defined(N32G45X))
            if(GPIO_ReadInputDataBit(STM32_GPIO_PORT[pKey->pstKeyLocate[0]->u8PortType-PORT_GPIOA],pKey->pstKeyLocate[0]->u16GpioPin)==0)
            #elif (defined(GD32F4XX))
            if(gpio_input_bit_get(STM32_GPIO_PORT[pKey->pstKeyLocate[0]->u8PortType-PORT_GPIOA],pKey->pstKeyLocate[0]->u16GpioPin)==0)
            #else
			if((STM32_GPIO_PORT[pKey->pstKeyLocate[0]->u8PortType-PORT_GPIOA]->IDR & pKey->pstKeyLocate[0]->u16GpioPin)==0)
            #endif
			{
				u16ScanVal = 0;

				if(pKeyRun->u8State == key_scan_state_idle)
				{
					pKeyRun->u8State = key_scan_state_busy;
				}
			}
			else
			{
				pKeyRun->u8State = key_scan_state_idle;
			}
		}
#if 0//(_EN_CHIP_TM1623_USE==_MACRO_ENABLE)
		//else if((pKey->pstKeyLocate[0]->u8PortType>=PORT_TM1623_KVAL_0)&&(pKey->pstKeyLocate[0]->u8PortType<=PORT_TM1623_KVAL_4))
        else if(IS_TM1623_KVAL(pKey->pstKeyLocate[0]->u8PortType))
        {
			if((TM1623_ksg_reg_val[TM1623_PORT_KS1+pKey->pstKeyLocate[0]->u8PortType-PORT_TM1623_KVAL_0] & pKey->pstKeyLocate[0]->u16GpioPin)!=0)
			{
				u16ScanVal = 0;

				if(pKeyRun->u8State == key_scan_state_idle)
				{
					pKeyRun->u8State = key_scan_state_busy;
				}
			}
			else
			{
				pKeyRun->u8State = key_scan_state_idle;
			}
		}
#endif

		u16ScanVal |= KEY_SCAN_VALUE_TYPE_BUTTON;
	}
	else if(u8IoNum > 1)
	{
		u16ScanVal = 0;
		for(i=0; i<u8IoNum; i++)
		{
			pIoNext = *(pIoS+i);
			if((pIoNext->u8PortType>=PORT_GPIOA)&&(pIoNext->u8PortType<=PORT_GPIOG))
			{
                #if (defined(N32G45X))
                if(GPIO_ReadInputDataBit(STM32_GPIO_PORT[pKey->pstKeyLocate[0]->u8PortType-PORT_GPIOA],pKey->pstKeyLocate[0]->u16GpioPin))
                #elif (defined(GD32F4XX))
                if(gpio_input_bit_get(STM32_GPIO_PORT[pKey->pstKeyLocate[0]->u8PortType-PORT_GPIOA],pKey->pstKeyLocate[0]->u16GpioPin))
                #else
				if((STM32_GPIO_PORT[pIoNext->u8PortType-PORT_GPIOA]->IDR & pIoNext->u16GpioPin)==pIoNext->u16GpioPin)
                #endif
				{
					u16ScanVal |= (1<<i);
				}
			}
			#if 0//(_EN_CHIP_TM1623_USE==_MACRO_ENABLE)
			else if((pIoNext->u8PortType>=PORT_TM1623_KVAL_0)&&(pIoNext->u8PortType<=PORT_TM1623_KVAL_4))
			{
				if((TM1623_ksg_reg_val[TM1623_PORT_KS1+pIoNext->u8PortType-PORT_TM1623_KVAL_0] & pIoNext->u16GpioPin)!=0)
				{
					u16ScanVal |= (1<<i);
				}
			}
			#endif
		}

		if(pKeyRun->u8ScanValPre != (u16ScanVal&KEY_SCAN_VALUE_STORE_MASK))
		{
			if(pKeyRun->u8State == key_scan_state_idle)
			{
				pKeyRun->u8ScanValPre = (u16ScanVal&KEY_SCAN_VALUE_STORE_MASK);
				pKeyRun->u8State = key_scan_state_busy;
			}
			else
			{
				pKeyRun->u8ScanValPre = 0xFF;
				pKeyRun->u8State = key_scan_state_idle;
			}
		}

		u16ScanVal |= KEY_SCAN_VALUE_TYPE_DIP;
	}

	return u16ScanVal;
}

void key_detect(uint8_t u8KeyIndex)
{
	uint16_t 			u16ScanVal;
	pstKeyDeclare_t 	pstKey = (pstKeyDeclare_t)&stProjectKeyTable[u8KeyIndex];
	pstKeyRunning_t 	pstKeyRun = &s_tBtn[u8KeyIndex];

	u16ScanVal = key_scan(pstKey, pstKeyRun);

	switch(pstKeyRun->u8State)
	{
		case key_scan_state_busy:
			if(pstKeyRun->u16Cnt < KEY_FILTER_TIME)
			{
				pstKeyRun->u16Cnt ++;
			}
			else
			{
				if((pstKey->eDetectMode == key_detect_mode_pre)||(pstKey->stRepeat.u16RepeatSpeed != KEY_REPEAT_INVALID))
				{
					if((u16ScanVal&KEY_SCAN_VALUE_TYPE_MASK)==KEY_SCAN_VALUE_TYPE_BUTTON)
					{
						key_store_val(KEY_NORMAL_PRESS_VAL, eKeyValSource_Panel, u8KeyIndex);
					}
					else if((u16ScanVal&KEY_SCAN_VALUE_TYPE_MASK)==KEY_SCAN_VALUE_TYPE_DIP)
					{
						key_store_val((uint8_t)((u16ScanVal&KEY_SCAN_VALUE_STORE_MASK)>>KEY_SCAN_VALUE_STORE_SHIFT), eKeyValSource_Panel, u8KeyIndex);
					}
				}
				pstKeyRun->u16LongCnt = KEY_FILTER_TIME;
				pstKeyRun->u8State = key_scan_val_stored;
			}
			break;
		case key_scan_val_stored:
			if(pstKey->stRepeat.u16RepeatSpeed != KEY_REPEAT_INVALID)
			{
				if(pstKeyRun->u16RepeatCnt < (pstKey->stRepeat.u16RepeatSpeed + pstKey->stRepeat.u16RepeatStartDelay))
				{
					pstKeyRun->u16RepeatCnt ++;
				}
				else
				{
					if((u16ScanVal&KEY_SCAN_VALUE_TYPE_MASK)==KEY_SCAN_VALUE_TYPE_BUTTON)
					{
						key_store_val(KEY_NORMAL_PRESS_VAL, eKeyValSource_Panel, u8KeyIndex);
					}
					else if((u16ScanVal&KEY_SCAN_VALUE_TYPE_MASK)==KEY_SCAN_VALUE_TYPE_DIP)
					{
						key_store_val((uint8_t)((u16ScanVal&KEY_SCAN_VALUE_STORE_MASK)>>KEY_SCAN_VALUE_STORE_SHIFT), eKeyValSource_Panel, u8KeyIndex);
					}
					pstKeyRun->u16RepeatCnt = pstKey->stRepeat.u16RepeatStartDelay;
				}
			}

			if(pstKey->u16LongPressTime != KEY_LONG_INVALID)
			{
				if(pstKeyRun->u16LongCnt < pstKey->u16LongPressTime)
				{
					pstKeyRun->u16LongCnt ++;
				}
				else if(pstKeyRun->u16LongCnt == pstKey->u16LongPressTime)
				{
					if((u16ScanVal&KEY_SCAN_VALUE_TYPE_MASK)==KEY_SCAN_VALUE_TYPE_BUTTON)
					{
						key_store_val(KEY_LONG_PRESS_VAL, eKeyValSource_Panel, u8KeyIndex);
					}
					pstKeyRun->u16LongCnt ++;
				}
			}
			break;
		case key_scan_state_idle:
			if(pstKey->eDetectMode==key_detect_mode_after)
			{
				if(pstKeyRun->u8StatePre == key_scan_val_stored)
				{
					if((pstKey->u16LongPressTime==KEY_LONG_INVALID)||(pstKeyRun->u16LongCnt<pstKey->u16LongPressTime))
					{
						if((u16ScanVal&KEY_SCAN_VALUE_TYPE_MASK)==KEY_SCAN_VALUE_TYPE_BUTTON)
						{
							key_store_val(KEY_NORMAL_PRESS_VAL, eKeyValSource_Panel, u8KeyIndex);
						}
						else if((u16ScanVal&KEY_SCAN_VALUE_TYPE_MASK)==KEY_SCAN_VALUE_TYPE_DIP)
						{
							key_store_val((uint8_t)((u16ScanVal&KEY_SCAN_VALUE_STORE_MASK)>>KEY_SCAN_VALUE_STORE_SHIFT), eKeyValSource_Panel, u8KeyIndex);
						}
					}
				}
			}
		default:
			pstKeyRun->u16Cnt = 0;
			pstKeyRun->u16LongCnt = 0;
			pstKeyRun->u16RepeatCnt = 0;
			break;
	}

	pstKeyRun->u8StatePre = pstKeyRun->u8State;
}
#endif

#if _ENABLE_NEW_USER_KEY_DEFINE_FUNCTION

#ifndef mapp_NewKeyPadProcess
#define mapp_NewKeyPadProcess   __NOP
#endif
void mapp_KeypadScan(void)
{
    mapp_NewKeyPadProcess();
}
#endif










