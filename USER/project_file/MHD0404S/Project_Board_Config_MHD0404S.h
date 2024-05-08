#ifndef __PROJECT_BOARD_CONFIG_H__
#define __PROJECT_BOARD_CONFIG_H__

typedef enum _eProjectGpioName_t_
{
    // Led
    eGpioName_LedSta,

	//i2c
	eGpioName_I2cEepromScl,
	eGpioName_I2cEepromSda,
	eGpioName_I2cIT66021Scl1,
	eGpioName_I2cIT66021Sda1,
    eGpioName_I2cIT66021Scl2,
    eGpioName_I2cIT66021Sda2,
    eGpioName_I2cGSV2011Scl1,
    eGpioName_I2cGSV2011Sda1,
    eGpioName_I2cGSV2011Scl2,
    eGpioName_I2cGSV2011Sda2,
    eGpioName_I2cGSV2011Scl3,
    eGpioName_I2cGSV2011Sda3,
    eGpioName_I2cGSV2011Scl4,
    eGpioName_I2cGSV2011Sda4,
    eGpioName_I2cIDT5V49EE902Scl,
    eGpioName_I2cIDT5V49EE902Sda,
    eGpioName_I2cAlpuCScl,
    eGpioName_I2cAlpuCSda,

    //Spi
    eGpioName_SpiFlashClk,
    eGpioName_SpiFlashCs,
    eGpioName_SpiFlashMosi,
    eGpioName_SpiFlashMiso,

	// reset
	eGpioName_IT66021Rst,
    eGpioName_Gsv2011Rst,
    eGpioName_FpgaRst1,
    eGpioName_FpgaRst2,

    // FAN control
	eGpioName_FANControl,

    // QS3384Q
	eGpioName_QS3384QBEA1,
    eGpioName_QS3384QBEB1,
    eGpioName_QS3384QBEA2,
    eGpioName_QS3384QBEB2,

    // Hdmi TX 5V
	eGpioName_HdmiTx5V1,
    eGpioName_HdmiTx5V2,
    eGpioName_HdmiTx5V3,
    eGpioName_HdmiTx5V4,

    // Hdmi RX HPD
	eGpioName_HdmiRxHpd1,
    eGpioName_HdmiRxHpd2,
    eGpioName_HdmiRxHpd3,
    eGpioName_HdmiRxHpd4,

	eGpioName_Max,
}eProjectGpioName_t;

typedef enum _eI2cDevice_t_
{
	eI2cDevice_Eeprom = 0,
	eI2cDevice_IT66021_1,
    eI2cDevice_IT66021_2,
    eI2cDevice_Gsv1016_1,
    eI2cDevice_Gsv1016_2,
    eI2cDevice_Gsv1016_3,
    eI2cDevice_Gsv1016_4,
    eI2cDevice_IDT5V49EE902,
    eI2cDevice_ALPU_C,

	eI2cDevice_Max,
}eI2cDevice_t;

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
#if _EN_CHIP_ADN4604_USE
extern const stDrvADN4604VarConfig stProjectAdn4604Config[];
#endif
#if _ENABLE_CHIP_GSV_CHIP_SERIES
extern const stGsvChipResetPinCfg_t stProjectGsvChipResetTable[];
#endif
void mapp_SetFanState(bool bEn);
void mapp_SetHdmiTx5V(bool bEnable);
void mapp_SetFpgaResetPin(u8 u8ID,u8 u8Mode);
void mapp_SelectFpgaFlashUpdateMode(u8 u8ID,bool bEnable);

#endif

