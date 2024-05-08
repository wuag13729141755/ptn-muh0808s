#ifndef __PROJECT_BOARD_CONFIG_H__
#define __PROJECT_BOARD_CONFIG_H__

typedef enum _eProjectGpioName_t_
{
    // Led
    eGpioName_LedActive,

	//i2c
	eGpioName_I2cEepromScl,
	eGpioName_I2cEepromSda,
	eGpioName_I2cIT66021Scl_1,
	eGpioName_I2cIT66021Sda_1,
	eGpioName_I2cIT66121Scl_1,
	eGpioName_I2cIT66121Sda_1,
    eGpioName_I2cAlpuCScl,
    eGpioName_I2cAlpuCSda,

    //spi
    eGpioName_GS2971Clk,
    eGpioName_GS2971Cs,
    eGpioName_GS2971Mosi,
    eGpioName_GS2971Miso,
    eGpioName_GS2972Clk,
    eGpioName_GS2972Cs,
    eGpioName_GS2972Mosi,
    eGpioName_GS2972Miso,

    //Reset
    eGpioName_IT66021Reset1,
    eGpioName_IT66121Reset1,
    eGpioName_GS2972Reset,

	eGpioName_Max,
}eProjectGpioName_t;

typedef enum _eI2cDevice_t_
{
	eI2cDevice_Eeprom = 0,
	eI2cDevice_IT66021_1,
	eI2cDevice_IT66121_1,
    eI2cDevice_ALPU_C,

	eI2cDevice_Max,
}eI2cDevice_t;

typedef enum _eSpiDevice_t_
{
	eSpiDevice_GS2971 = 0,
    eSpiDevice_GS2972,

	eSpiDevice_Max,
}eSpiDevice_t;

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
#if _ENABLE_CHIP_MS7200
extern stChipResetPinCfg_t stProjectMs7200ChipResetTable[];
#endif

void app_SetGpioState(u8 u8Gpio,u8 u8Sta);
bool app_GetGpioState(u8 u8Gpio);

#endif

