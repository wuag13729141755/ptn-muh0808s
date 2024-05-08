#ifndef __PROJECT_BOARD_CONFIG_H__
#define __PROJECT_BOARD_CONFIG_H__

typedef enum _eProjectGpioName_t_
{
	eGpioName_ActiveLed = 0,
	eGpioName_GsvReset,
    eGpioName_PCA9539Reset,
    eGpioName_FPGAReset,

    // I2C
    eGpioName_I2cEepromScl,
    eGpioName_I2cEepromSda,
    eGpioName_I2cPCA9539Scl,
    eGpioName_I2cPCA9539Sda,
    eGpioName_I2cGsv1016Scl1,
    eGpioName_I2cGsv1016Sda1,
    eGpioName_I2cGsv1016Scl2,
    eGpioName_I2cGsv1016Sda2,
    eGpioName_I2cGsv1016Scl3,
    eGpioName_I2cGsv1016Sda3,
    eGpioName_I2cGsv1016Scl4,
    eGpioName_I2cGsv1016Sda4,
    eGpioName_I2cIDT5V49Scl1,
    eGpioName_I2cIDT5V49Sda1,
    eGpioName_I2cIDT5V49Scl2,
    eGpioName_I2cIDT5V49Sda2,
    // I2C sw
    eGpioName_HC4052SwA1,
    eGpioName_HC4052SwB1,
    eGpioName_HC4052SwA2,
    eGpioName_HC4052SwB2,
    eGpioName_HC4052SwA3,
    eGpioName_HC4052SwB3,
    eGpioName_HC4052SwA4,
    eGpioName_HC4052SwB4,

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
    eGpioName_SwQS3L384A3,
    eGpioName_SwQS3L384B3,
    eGpioName_SwQS3L384A4,
    eGpioName_SwQS3L384B4,

    // Hdmi Tx 5V control
    eGpioName_Hdmi5VTx1,
    eGpioName_Hdmi5VTx2,
    eGpioName_Hdmi5VTx3,
    eGpioName_Hdmi5VTx4,
    eGpioName_Hdmi5VTx5,
    eGpioName_Hdmi5VTx6,
    eGpioName_Hdmi5VTx7,
    eGpioName_Hdmi5VTx8,
    eGpioName_Hdmi5VTx9,
    eGpioName_Hdmi5VTx10,
    eGpioName_Hdmi5VTx11,
    eGpioName_Hdmi5VTx12,
    eGpioName_Hdmi5VTx13,
    eGpioName_Hdmi5VTx14,
    eGpioName_Hdmi5VTx15,
    eGpioName_Hdmi5VTx16,

	eGpioName_Max,
}eProjectGpioName_t;

typedef enum _eI2cDevice_t_
{
	eI2cDevice_Eeprom = 0,
	eI2cDevice_PCA9539,
    eI2cDevice_Gsv1016_1,
    eI2cDevice_Gsv1016_2,
    eI2cDevice_Gsv1016_3,
    eI2cDevice_Gsv1016_4,
    eI2cDevice_Gsv1016_5,
    eI2cDevice_Gsv1016_6,
    eI2cDevice_Gsv1016_7,
    eI2cDevice_Gsv1016_8,
    eI2cDevice_Gsv1016_9,
    eI2cDevice_Gsv1016_10,
    eI2cDevice_Gsv1016_11,
    eI2cDevice_Gsv1016_12,
    eI2cDevice_Gsv1016_13,
    eI2cDevice_Gsv1016_14,
    eI2cDevice_Gsv1016_15,
    eI2cDevice_Gsv1016_16,
    eI2cDevice_IDT5V49_1,
    eI2cDevice_IDT5V49_2,

	eI2cDevice_Max,
}eI2cDevice_t;

typedef enum _eSw4052Device_t_
{
    eSw4052Device_Gsv1016_1 = 0,
    eSw4052Device_Gsv1016_2,
    eSw4052Device_Gsv1016_3,
    eSw4052Device_Gsv1016_4,

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

void mapp_HdmiTx5VEnable(u8 u8HdmiDev, bool bEn);

#endif

