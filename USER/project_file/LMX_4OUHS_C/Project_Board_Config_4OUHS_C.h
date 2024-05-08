#ifndef __PROJECT_BOARD_CONFIG_H__
#define __PROJECT_BOARD_CONFIG_H__

typedef enum _eProjectGpioName_t_
{
    // Led
    eGpioName_LedActive,

	//i2c
	eGpioName_I2cEepromScl,
	eGpioName_I2cEepromSda,
    eGpioName_I2cMS1826Scl,
    eGpioName_I2cMS1826Sda,
    eGpioName_I2cAlpuCScl,
    eGpioName_I2cAlpuCSda,

    //Reserve SPI
    eGpioName_SpiFlashClk,
    eGpioName_SpiFlashCs,
    eGpioName_SpiFlashMosi,
    eGpioName_SpiFlashMiso,

    //HDMI OUT 5V
    eGpioName_OUT5V_HDMI1,
    eGpioName_OUT5V_HDMI2,
    eGpioName_OUT5V_HDMI3,
    eGpioName_OUT5V_HDMI4,

    //AUDIO MUTE
    eGpioName_AUDIO_MUTE1,
    eGpioName_AUDIO_MUTE2,
    eGpioName_AUDIO_MUTE3,
    eGpioName_AUDIO_MUTE4,

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
    eI2cDevice_ALPU_C,

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
void mapp_SetHdmiTx5V(bool bEnable);
void mapp_SetAudioMute(u8 u8AudioID,bool bEnable);

#endif

