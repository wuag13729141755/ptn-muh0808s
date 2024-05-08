#ifndef __PROJECT_BOARD_CONFIG_H__
#define __PROJECT_BOARD_CONFIG_H__

typedef enum _eProjectGpioName_t_
{
    // Led
    eGpioName_LedHdmiIn1,
    eGpioName_LedHdmiIn2,
    eGpioName_LedHdmiIn3,
    eGpioName_LedHdmiIn4,
    eGpioName_LedHdmiOut1,
    eGpioName_LedHdmiOut2,
    eGpioName_LedHdmiOut3,
    eGpioName_LedHdmiOut4,
    eGpioName_LedPwrOn,
    eGpioName_LedPwrOff,

    //key
    eGpioName_Key1,
    eGpioName_Key2,
    eGpioName_Key3,
    eGpioName_Key4,
    eGpioName_Key5,
    eGpioName_Key6,
    eGpioName_Key7,
    eGpioName_Key8,
    eGpioName_KeyPwr,

	//i2c
	eGpioName_I2cEepromScl,
	eGpioName_I2cEepromSda,
    eGpioName_I2cMS1826Scl,
    eGpioName_I2cMS1826Sda,

    //AUDIO
    eGpioName_I2SSwitch1,
    eGpioName_I2SSwitch2,
    eGpioName_I2SSwitch3,
    eGpioName_I2SSwitch4,
    eGpioName_SpdifSwitch1,
    eGpioName_SpdifSwitch2,
    eGpioName_SpdifSwitch3,
    eGpioName_SpdifSwitch4,

    //Reset
    eGpioName_MS1826Reset,

    //MS1826_INT
    eGpioName_MS1826Int,

	eGpioName_Max,
}eProjectGpioName_t;

typedef enum _eI2cDevice_t_
{
	eI2cDevice_Eeprom = 0,
    eI2cDevice_MS1826,

	eI2cDevice_Max,
}eI2cDevice_t;

extern const stGpioConfig_t stProjectGpioTable[eGpioName_Max];
extern const stKeyDeclare_t stProjectKeyTable[];
extern const stLedDeclare_t stProjectLedTable[];
extern const stI2cDeclare_t stProjectI2cTable[];
extern const stSpiDeclare_t stProjectSpiTable[];
extern const stRxHpdDetDeclare_t stProjectRxHpdDetTable[];
extern const stIT663xxRstDeclare_t stProjectIT663xxRstTable[];
extern const stIrWorkDeclare_t stProjectIrWorkTable;
extern const st5VControl_t stProjectTx5VControlTable[];
extern const stUartDeclare_t stProjectUartWorkTable[];
extern const stCecDeclare_t stProjectCecWorkTable[];
#if _ENABLE_NEW_USER_KEY_DEFINE_FUNCTION
extern const stNewKeyDeclare_t stProjectNewKeyTable[];
#endif
extern const stI2CSwitchCtrl_t stProjectI2cSwitchTable[];
#if _ENABLE_CHIP_GSV_CHIP_SERIES
extern const stGsvChipResetPinCfg_t stProjectGsvChipResetTable[];
#endif
#if _ENABLE_CHIP_MS1826
extern const stMS1826ChipResetPinCfg_t stProjectMS1826ChipResetTable[];
#endif
void mapp_SetAudioMode(u8 u8AudioID,bool bIsInput);

#endif

