#ifndef __PROJECT_VAR_FUN_CONFIG_H__
#define __PROJECT_VAR_FUN_CONFIG_H__

#define SaveSpecificVarInit				IMX_25_2VCTAC_SpecificVarInit
#define EepSpecVarInvalid				IMX_25_2VCTAC_SpecVarValidCheck
#define PrjStatePowerOnInit				IMX_25_2VCTAC_PrjStatePowerOnInit
#if _ENABLE_NEW_USER_KEY_DEFINE_FUNCTION
#define mapp_NewKeyPadProcess           mapp_KeyFunctionProcess
#endif
#define StartupMessage 					mapp_IMX_25_2VCTACStartupMessage
#define SaveSpecificVarDefault          IMX_25_2VCTAC_SpecificVarDefault


typedef enum _led_index_name_t_
{
    led_name_ActiveLed= 0,

    led_name_End,
	led_name_max_number
}led_index_name_t;

#if _ENABLE_NEW_USER_KEY_DEFINE_FUNCTION
typedef enum _key_counter_index_name_
{
    emKeyName_Source = 0,

    emKeyName_max,
}key_counter_index_name_t;
#endif

typedef enum _emUserTimerType_t_
{
    emUserTimer_LedActive = 0,
    emUserTimer_SwitchPortTime,
    emUserTimer_AutoSwitch,

    emUserTimer_Max,
}emUserTimerType_t;

typedef enum _eSourceInputType_t_
{
    emSrcInput_Hdmi1 = 0,
    emSrcInput_Hdmi2,

    emSrcInput_Max,
}eSourceInputType_t;

typedef __packed struct _stBasicSpecVar_t_
{
	bool			bPowerOnSta;
	bool			bIsLock;
    uint8_t         u8baudrateId;

	uint8_t			u8CheckSum;
}stBasicSpecVar_t, *pstBasicSpecVar_t;

typedef __packed struct _stCustomEdid_t_
{
	uint8_t 		pu8EdidContext[256];

	uint8_t			u8CheckSum;
}stCustomEdid_t, *pstCustomEdid_t;

typedef struct _stProjectSpecVar_t_
{
	stBasicSpecVar_t	stBasSpecVar;
	stCustomEdid_t		stCustomEdidPre1;
}stProjectSpecVar_t, *pstProjectSpecVar_t;

typedef struct _stEdidUpgrData_t_
{
	uint8_t 		u8EdidRecv[256];
	uint16_t		u16RecvLen;
}stEdidUpgrData_t, *pstEdidUpgrData_t;

#ifdef __PROJECT_VAR_FUN_CONFIG_C__
#define INTERFACE
#else
#define INTERFACE       extern
#endif

INTERFACE void IMX_25_2VCTAC_SpecificVarInit(void);
INTERFACE void IMX_25_2VCTAC_SpecBasicVarDefault(void);
INTERFACE void IMX_25_2VCTAC_SpecCustomEdidVarDefault(void);
INTERFACE bool IMX_25_2VCTAC_SpecVarValidCheck(void);
// the status when power on device
INTERFACE void IMX_25_2VCTAC_PrjStatePowerOnInit(void);
INTERFACE void IMX_25_2VCTAC_LedWork_Loop(void);
INTERFACE void IMX_25_2VCTAC_FuncWork_LoopRunning(void);
INTERFACE void SetUserData_SpecBasicVar(void);
INTERFACE void SetUserData_SpecCustomEdid(uint8_t u8CustomEdidIndex);
INTERFACE void mapp_IMX_25_2VCTACStartupMessage(void);

#undef INTERFACE
#endif