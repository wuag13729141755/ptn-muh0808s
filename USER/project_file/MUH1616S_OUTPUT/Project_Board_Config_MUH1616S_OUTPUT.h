#ifndef __PROJECT_BOARD_CONFIG_H__
#define __PROJECT_BOARD_CONFIG_H__

typedef enum _eProjectGpioName_t_
{
    // Led
    eGpioName_LedActive,

	//i2c
	eGpioName_I2cEepromScl,
	eGpioName_I2cEepromSda,
    eGpioName_I2cMS1826Scl_1,
    eGpioName_I2cMS1826Sda_1,
    eGpioName_I2cMS1826Scl_2,
    eGpioName_I2cMS1826Sda_2,
    eGpioName_I2cMS1826Scl_3,
    eGpioName_I2cMS1826Sda_3,
    eGpioName_I2cMS1826Scl_4,
    eGpioName_I2cMS1826Sda_4,
    eGpioName_I2cPCA9539Scl,
    eGpioName_I2cPCA9539Sda,
    eGpioName_I2cPCA9539Scl_2,
    eGpioName_I2cPCA9539Sda_2,
    eGpioName_I2cAlpuCScl,
    eGpioName_I2cAlpuCSda,

    //HDMI OUT 5V
    eGpioName_OUT5V_HDMI1,
    eGpioName_OUT5V_HDMI2,
    eGpioName_OUT5V_HDMI3,
    eGpioName_OUT5V_HDMI4,
    eGpioName_OUT5V_HDMI5,
    eGpioName_OUT5V_HDMI6,
    eGpioName_OUT5V_HDMI7,
    eGpioName_OUT5V_HDMI8,
    eGpioName_OUT5V_HDMI9,
    eGpioName_OUT5V_HDMI10,
    eGpioName_OUT5V_HDMI11,
    eGpioName_OUT5V_HDMI12,
    eGpioName_OUT5V_HDMI13,
    eGpioName_OUT5V_HDMI14,
    eGpioName_OUT5V_HDMI15,
    eGpioName_OUT5V_HDMI16,

    //Reset
    eGpioName_MS1826Reset1,
    eGpioName_MS1826Reset2,
    eGpioName_MS1826Reset3,
    eGpioName_MS1826Reset4,
    eGpioName_PCA9539Rst,

	eGpioName_Max,
}eProjectGpioName_t;

typedef enum _eI2cDevice_t_
{
	eI2cDevice_Eeprom = 0,
    eI2cDevice_MS1826_1,
    eI2cDevice_MS1826_2,
    eI2cDevice_MS1826_3,
    eI2cDevice_MS1826_4,
    eI2cDevice_PCA9539,
    eI2cDevice_PCA9539_2,
    eI2cDevice_ALPU_C,

	eI2cDevice_Max,
}eI2cDevice_t;

typedef enum _eSpiDevice_t_
{
//	eSpiDevice_TM1623_1 = 0,

	eSpiDevice_Max,
}eSpiDevice_t;

#if _ENABLE_NEW_USER_KEY_DEFINE_FUNCTION
typedef enum _emKeyValueType
{
    emKeyvalueInput1       = BIT0,
    emKeyvalueInput2       = BIT1,
    emKeyvalueInput3       = BIT2,
    emKeyvalueInput4       = BIT3,
    emKeyvalueInput5       = BIT4,
    emKeyvalueInput6       = BIT5,
    emKeyvalueInput7       = BIT6,
    emKeyvalueInput8       = BIT7,
    emKeyvalueAll          = BIT11,
    emKeyvalueLock         = BIT12,
    emKeyvalueMode         = BIT13,

    emKeyvalueOutput1      = BIT14,
    emKeyvalueOutput2      = BIT15,
    emKeyvalueOutput3      = BIT16,
    emKeyvalueOutput4      = BIT17,
    emKeyvalueOutput5      = BIT18,
    emKeyvalueOutput6      = BIT19,
    emKeyvalueOutput7      = BIT20,
    emKeyvalueOutput8      = BIT21,
    emKeyvalueEnter        = BIT25,
    emKeyvalueScene        = BIT26,
    emKeyvalueClear        = BIT27,
}emKeyValueType;
#endif

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
#if _EN_CHIP_LT8644_USE
extern const stDrvLT8644VarConfig stProjectLT8644Config[];
#endif
#if ENABLE_SERIES_LCD_FUNCTION
extern const stSpiLcdPinCfg_t stProjectSpiLcdPinTable[];
#endif

void mapp_SetHdmiTx5V(bool bEnable);
void mapp_BacklightEN(bool bSta);

#endif

