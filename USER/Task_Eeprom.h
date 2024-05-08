#ifndef __TASK_EEPROM_H_
#define __TASK_EEPROM_H_

typedef enum _eEepDataIndex_t_
{
	eEepDataIndex_None = 0,

	#if ((_EN_EEPROM_FUN_SAVE_HDMI_SEL==_MACRO_ENABLE)||(_EN_EEPROM_FUN_SAVE_TYPE_SWITCH_MODE==_MACRO_ENABLE))
	eEepDataIndex_HdmiSel,
	#endif

	#if ((_IR_FUNCTION_USE&_IR_FUNCTION_KEY_STUDY)==_IR_FUNCTION_KEY_STUDY)
	eEepDataIndex_IrKeys,
	#endif

	#if (_EN_EEPROM_FUN_SAVE_UART_STUDY_KEYS==_MACRO_ENABLE)
	eEepDataIndex_UartKeys,
	#endif

	#if (_EN_EEPROM_FUN_SAVE_HDCP_STATUS==_MACRO_ENABLE)
	eEepDataIndex_HdcpStatus,
	#endif

	#if (_EN_EEPROM_FUN_SAVE_SYS_EDID_INDEX==_MACRO_ENABLE)
	eEepDataIndex_SysEdidIndex,
	#endif

	eEepDataIndex_ProjectName,

	eEepDataIndex_SpecBlock1,
	eEepDataIndex_SpecBlock2,
	eEepDataIndex_SpecBlock3,
	eEepDataIndex_SpecBlock4,
	eEepDataIndex_SpecBlock5,
	eEepDataIndex_SpecBlock6,

	eEepDataIndex_Max,
}eEepDataIndex_t;

typedef struct _stEepDataSave_t_
{
	void 			*pvBufferAdr;

	uint32_t 		u32EepSaveAdr;
	uint32_t 		u32EepSaveLen;
	uint16_t 		u16EepSaveAfT;//save to eeprom after this time
	bool            bEepSaveOkFlag;     // add by wf8421 20200409 use to save end flag
}stEepDataSave_t, *pstEepDataSave_t;

//save to eeprom after that time
#ifndef TIME_SAVE_TO_EEPROM_IMMEDIATELY
#define TIME_SAVE_TO_EEPROM_IMMEDIATELY		1
#endif
#ifndef TIME_SAVE_TO_EEPROM_TWO_SEC_LATER
#define TIME_SAVE_TO_EEPROM_TWO_SEC_LATER	200//10ms unit
#endif
/**********************************************************************/
//eeprom address assign
#define ADDR_EEPROM_CHECK_MAIN_VER          0
//#define ADDR_EEPROM_CHECK_SUB_VER           1
#define ADDR_EEPROM_USER_START			    2

#ifndef DEF_MAIN_VER
#define DEF_MAIN_VER        0xA5A5
#endif
/**********************************************************************/

void vTaskEeprom(void *pvParameters);
void SaveUserDataToEep(eEepDataIndex_t e_index, uint32_t E_addr, uint8_t *buffer, uint32_t len, uint16_t save_time, bool from_isr);
void EepromVarInit(void);
bool mapp_CheckAllDataHasSave(void);
void app_UserDataToDefault(void);


#endif






















