#ifndef __PROJECT_BOARD_CONFIG_H__
#define __PROJECT_BOARD_CONFIG_H__

typedef enum _eProjectGpioName_t_
{
    // Led
    eGpioName_LedActive,
    eGpioName_LedIn1_D3,
    eGpioName_LedIn2_D4,
    eGpioName_LedIn3_D5,
    eGpioName_LedIn4_D6,
    eGpioName_LedIn5_D7,
    eGpioName_LedIn6_D8,
    eGpioName_LedIn7_D9,
    eGpioName_LedIn8_D10,
    eGpioName_LedOut1_D16,
    eGpioName_LedOut2_D17,
    eGpioName_LedOut3_D19,
    eGpioName_LedOut4_D20,
    eGpioName_LedOut5_D21,
    eGpioName_LedOut6_D22,
    eGpioName_LedOut7_D23,
    eGpioName_LedOut8_D24,
    eGpioName_LedAll_D13,
    eGpioName_LedLock_D14,
    eGpioName_LedMode_D15,
    eGpioName_LedEnter_D29,
    eGpioName_LedScene_D27,
    eGpioName_LedClear_D28,

    // key
    eGpioName_KeyIn1_K1,
    eGpioName_KeyIn2_K2,
    eGpioName_KeyIn3_K3,
    eGpioName_KeyIn4_K4,
    eGpioName_KeyIn5_K5,
    eGpioName_KeyIn6_K6,
    eGpioName_KeyIn7_K7,
    eGpioName_KeyIn8_K8,
    eGpioName_KeyOut1_K14,
    eGpioName_KeyOut2_K15,
    eGpioName_KeyOut3_K16,
    eGpioName_KeyOut4_K17,
    eGpioName_KeyOut5_K18,
    eGpioName_KeyOut6_K19,
    eGpioName_KeyOut7_K20,
    eGpioName_KeyOut8_K21,
    eGpioName_KeyAll_K11,
    eGpioName_KeyLock_K12,
    eGpioName_KeyMode_K13,
    eGpioName_KeyEnter_K24,
    eGpioName_KeyScene_K25,
    eGpioName_KeyClear_K26,

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
    eGpioName_I2cLT8644Scl_1,
    eGpioName_I2cLT8644Sda_1,
    eGpioName_I2cLT8644Scl_2,
    eGpioName_I2cLT8644Sda_2,
    eGpioName_I2cPCA9539Scl,
    eGpioName_I2cPCA9539Sda,
    eGpioName_I2cAlpuCScl,
    eGpioName_I2cAlpuCSda,

    //Reserve SPI
    eGpioName_SpiTm1623Clk1,
    eGpioName_SpiTm1623Cs1,
    eGpioName_SpiTm1623Dio1,

    //HDMI OUT 5V
    eGpioName_OUT5V_HDMI1,
    eGpioName_OUT5V_HDMI2,
    eGpioName_OUT5V_HDMI3,
    eGpioName_OUT5V_HDMI4,
    eGpioName_OUT5V_HDMI5,
    eGpioName_OUT5V_HDMI6,
    eGpioName_OUT5V_HDMI7,
    eGpioName_OUT5V_HDMI8,

    //Reset
    eGpioName_MS1826Reset1,
    eGpioName_MS1826Reset2,
    eGpioName_MS1826Reset3,
    eGpioName_MS1826Reset4,
    eGpioName_PhyRst,
    eGpioName_PCA9539Rst,
    eGpioName_LT8644Rst1,
    eGpioName_LT8644Rst2,

    //MS1826_INT
    eGpioName_MS1826Int1,
    eGpioName_MS1826Int2,
    eGpioName_MS1826Int3,
    eGpioName_MS1826Int4,

    // LCD
    eGpioName_LCDSck,
    eGpioName_LCDSda,
    eGpioName_LCDRS,
    eGpioName_LCDRst,
    eGpioName_LCDBacklight,

    // esc
    eGpioName_LT8644Update1,
    eGpioName_LT8644Update2,
    eGpioName_CecIn,
    eGpioName_CecOut,

	eGpioName_Max,
}eProjectGpioName_t;

typedef enum _eI2cDevice_t_
{
	eI2cDevice_Eeprom = 0,
    eI2cDevice_MS1826_1,
    eI2cDevice_MS1826_2,
    eI2cDevice_MS1826_3,
    eI2cDevice_MS1826_4,
    eI2cDevice_LT8644_1,
    eI2cDevice_LT8644_2,
    eI2cDevice_PCA9539,
    eI2cDevice_ALPU_C,

	eI2cDevice_Max,
}eI2cDevice_t;

typedef enum _eSpiDevice_t_
{
	eSpiDevice_TM1623_1 = 0,

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

