#ifndef __PROJECT_BOARD_CONFIG_H__
#define __PROJECT_BOARD_CONFIG_H__

typedef enum _eProjectGpioName_t_
{
    // Led
    eGpioName_LedActive,

	//i2c
	eGpioName_I2cEepromScl,
	eGpioName_I2cEepromSda,
	eGpioName_I2cGSV2001Scl_1,
	eGpioName_I2cGSV2001Sda_1,
	eGpioName_I2cGSV2001Scl_2,
	eGpioName_I2cGSV2001Sda_2,
	eGpioName_I2cGSV2001Scl_3,
	eGpioName_I2cGSV2001Sda_3,
	eGpioName_I2cGSV2001Scl_4,
	eGpioName_I2cGSV2001Sda_4,
	eGpioName_I2cGSV2001Scl_5,
	eGpioName_I2cGSV2001Sda_5,
    eGpioName_I2cAlpuCScl,
    eGpioName_I2cAlpuCSda,

    //Reset
    eGpioName_Gsv2001Reset1,
    eGpioName_Gsv2001Reset2,
    eGpioName_Gsv2001Reset3,
    eGpioName_Gsv2001Reset4,
    eGpioName_Gsv2001Reset5,

	eGpioName_Max,
}eProjectGpioName_t;

typedef enum _eI2cDevice_t_
{
	eI2cDevice_Eeprom = 0,
	eI2cDevice_GSV2001_1,
	eI2cDevice_GSV2001_2,
	eI2cDevice_GSV2001_3,
	eI2cDevice_GSV2001_4,
	eI2cDevice_GSV2001_5,
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

#endif

