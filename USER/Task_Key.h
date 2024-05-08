#ifndef __TASK_KEY_H_
#define __TASK_KEY_H_

#include <stdint.h>
//long press
#define KEY_LONG_INVALID				0//long time press is invalid
#define KEY_LONG_TIME					300//unit is 10ms, 300 equal 3000ms
//filter time
#define KEY_FILTER_TIME 				5//unit is 10ms, 5 equal 50ms
//repeat press
#define KEY_REPEAT_INVALID				0//Repeat is invalid
#define KEY_REPEAT_TIME					50//unit is 10ms, 50 equal 500ms
#define KEY_REPEAT_TIME_100MS			10//unit is 10ms, 10 equal 100ms

#define KEY_REPEAT_START_DELAY_NONE		0//use for key repeat invalid
#define KEY_REPEAT_START_DELAY_1S		100//unit is 10ms, 100 equal 1000ms

//press val
#define KEY_NORMAL_PRESS_VAL			0
#define KEY_LONG_PRESS_VAL				1
#define KEY_COMPOSITE_VAL				2

//key scan value type mask
#define KEY_SCAN_VALUE_TYPE_SHIFT		8
#define KEY_SCAN_VALUE_TYPE_MASK		(0xFF<<KEY_SCAN_VALUE_TYPE_SHIFT)
#define KEY_SCAN_VALUE_TYPE_BUTTON		(0x01<<KEY_SCAN_VALUE_TYPE_SHIFT)
#define KEY_SCAN_VALUE_TYPE_DIP			(0x02<<KEY_SCAN_VALUE_TYPE_SHIFT)

#define KEY_SCAN_VALUE_STORE_SHIFT		0
#define KEY_SCAN_VALUE_STORE_MASK		(0xFF<<KEY_SCAN_VALUE_STORE_SHIFT)

typedef enum _key_scan_state_t_
{
	key_scan_state_idle = 0,
	key_scan_state_busy,
	key_scan_val_stored,
}key_scan_state_t;

typedef struct _stKeyRunning_t_
{
	uint16_t 		u16Cnt;
	uint16_t 		u16LongCnt;
	uint8_t 		u8State;
	uint8_t 		u8StatePre;
	uint16_t 		u16RepeatCnt;
	uint8_t 		u8ScanValPre;
}stKeyRunning_t, *pstKeyRunning_t;

#if (_EN_GPIO_EDID_MODE_SWITCH==_MACRO_ENABLE)
typedef enum _kval_edidm_t_   //key of EDID mode, l.res, preset, h.res
{
	kval_edidm_none = 0,
	kval_edidm_l_res,
	kval_edidm_preset,
	kval_edidm_h_res,

	kval_edidm_max
}kval_edidm_t;
#endif

#if (_EN_GPIO_EDID_SELECT_SWITCH==_MACRO_ENABLE)
typedef enum _kval_edids_t_	//key of EDID select, 4 bits switch
{
	kval_edids_none = 0,

	kval_edids_select0,
	kval_edids_select1,
	kval_edids_select2,
	kval_edids_select3,
	kval_edids_select4,
	kval_edids_select5,
	kval_edids_select6,
	kval_edids_select7,
	kval_edids_select8,
	kval_edids_select9,
	kval_edids_selectA,
	kval_edids_selectB,
	kval_edids_selectC,
	kval_edids_selectD,
	kval_edids_selectE,
	kval_edids_selectF,

	kval_edids_max
}kval_edids_t;
#endif

void vTaskKey(void *pvParameters);
bool isKeyProSuspend(void);
void SetKeyProSuspendOrResume(bool bSta);
void key_var_init(void);
void key_detect(uint8_t key_name);
void KeyStoreValSet(uint8_t val, eKeyValSource_t eKeySrc, uint8_t u8KeyName, bool bSndToRspne);
void key_store_val(uint8_t val, eKeyValSource_t eKeySrc, uint8_t key_name);
#if _ENABLE_NEW_USER_KEY_DEFINE_FUNCTION
void mapp_KeypadScan(void);
#endif

#define KeyStoreValWithoutResp(x,y,z) KeyStoreValSet(x,y,z,FALSE)

#endif






















