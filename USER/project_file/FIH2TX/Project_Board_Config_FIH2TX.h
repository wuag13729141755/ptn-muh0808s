#ifndef __PROJECT_BOARD_CONFIG_H__
#define __PROJECT_BOARD_CONFIG_H__

typedef enum _eProjectGpioName_t_
{
    // Led
    eGpioName_LedActive,

	//i2c
	eGpioName_I2cEepromScl,
	eGpioName_I2cEepromSda,
	eGpioName_I2cGSV2001Scl,
	eGpioName_I2cGSV2001Sda,
    eGpioName_I2cGSV5100Scl,
    eGpioName_I2cGSV5100Sda,
    eGpioName_I2cIT66353Scl,
    eGpioName_I2cIT66353Sda,
    eGpioName_I2cAlpuCScl,
    eGpioName_I2cAlpuCSda,

    //Reserve SPI
    eGpioName_SpiFlashClk,
    eGpioName_SpiFlashCs,
    eGpioName_SpiFlashMosi,
    eGpioName_SpiFlashMiso,

    //Reserve IO
    eGpioName_IO1,
    eGpioName_IO2,
    eGpioName_IO3,

    //Reset
    eGpioName_IT66353Reset,
    eGpioName_Gsv2001Reset,
    eGpioName_CS5100Reset,

    //USB5V Detect
    eGpioName_USB5VDet,

	eGpioName_Max,
}eProjectGpioName_t;

typedef enum _eI2cDevice_t_
{
	eI2cDevice_Eeprom = 0,
	eI2cDevice_GSV2001,
    eI2cDevice_GSV5100,
    eI2cDevice_IT66353,
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

void mapp_SetRst66353Control(bool bSta);

#endif

