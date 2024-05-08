#ifndef __PROJECT_BOARD_CONFIG_H__
#define __PROJECT_BOARD_CONFIG_H__

typedef enum _eProjectGpioName_t_
{
	eGpioName_ActiveLed = 0,
	eGpioName_IT66021Reset,
    eGpioName_FPGAReset,

    // I2C
    eGpioName_I2cEepromScl,
    eGpioName_I2cEepromSda,
    eGpioName_I2cIT66021Scl1,
    eGpioName_I2cIT66021Sda1,
    eGpioName_I2cIT66021Scl2,
    eGpioName_I2cIT66021Sda2,
    eGpioName_I2cIT66021Scl3,
    eGpioName_I2cIT66021Sda3,
    eGpioName_I2cIT66021Scl4,
    eGpioName_I2cIT66021Sda4,

    // SPI
    eGpioName_FpgaFlashCs,
    eGpioName_FpgaFlashSclk,
    eGpioName_FpgaFlashMiso,
    eGpioName_FpgaFlashMosi,

    // SPI Switcher
    eGpioName_SwQS3L384A1,
    eGpioName_SwQS3L384B1,
    eGpioName_SwQS3L384A2,
    eGpioName_SwQS3L384B2,

    // Hdmi Rx Hpd Pin
    eGpioName_HdmiRxHpd1,
    eGpioName_HdmiRxHpd2,
    eGpioName_HdmiRxHpd3,
    eGpioName_HdmiRxHpd4,
    eGpioName_HdmiRxHpd5,
    eGpioName_HdmiRxHpd6,
    eGpioName_HdmiRxHpd7,
    eGpioName_HdmiRxHpd8,
    // ID detect
    eGpioName_IDDetect,

	eGpioName_Max,
}eProjectGpioName_t;

typedef enum _eI2cDevice_t_
{
	eI2cDevice_Eeprom = 0,
    eI2cDevice_It66021_1,
    eI2cDevice_It66021_2,
    eI2cDevice_It66021_3,
    eI2cDevice_It66021_4,

	eI2cDevice_Max,
}eI2cDevice_t;

typedef enum _eSw4052Device_t_
{
	eSw4052Device_Max,
}eSw4052Device_t;

typedef enum _eSpiDevice_t_
{
	eSpiDevice_FpgaFlash = 0,

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

bool mapp_GetIDPinState(void);

#endif

