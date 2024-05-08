#ifndef __PROJECT_BOARD_CONFIG_H__
#define __PROJECT_BOARD_CONFIG_H__

typedef enum _eProjectGpioName_t_
{
    // key
	eGpioName_KeyInput1 = 0,
    eGpioName_KeyInput2,
    eGpioName_KeyInput3,
    eGpioName_KeyInput4,
    eGpioName_KeyAuto,
    eGpioName_KeyWinA,
    eGpioName_KeyWinB,
    eGpioName_KeyWinC,
    eGpioName_KeyWinD,
    eGpioName_KeySwap,
    eGpioName_KeyResize,
    eGpioName_KeyMVMode1,
    eGpioName_KeyMVMode2,
    eGpioName_KeyMVMode3,

    // led
    eGpioName_LedPowerG,
    eGpioName_LedPowerR,
    eGpioName_LedInput1Y,
    eGpioName_LedInput1B,
    eGpioName_LedInput2Y,
    eGpioName_LedInput2B,
    eGpioName_LedInput3Y,
    eGpioName_LedInput3B,
    eGpioName_LedInput4Y,
    eGpioName_LedInput4B,
    eGpioName_LedAutoSwitch,
    eGpioName_LedWindowA,
    eGpioName_LedWindowB,
    eGpioName_LedWindowC,
    eGpioName_LedWindowD,
    eGpioName_LedSwap,
    eGpioName_LedResize,
    eGpioName_LedMV2Win,
    eGpioName_LedMV4Win1,
    eGpioName_LedMV4Win2,

    // cec
    eGpioName_CecOutput,
    eGpioName_CecInput1,
    eGpioName_CecInput2,
    eGpioName_CecInput3,
    eGpioName_CecInput4,

    // 4052
    eGpioName_Sw4052_It66321A,
    eGpioName_Sw4052_It66321B,
    eGpioName_Sw4052_It66021A,
    eGpioName_Sw4052_It66021B,

    // SPI Switcher
    eGpioName_SwQS3L384A,
    eGpioName_SwQS3L384B,

    // I2C
    eGpioName_I2cIt66321Scl,
    eGpioName_I2cIt66321Sda,
    eGpioName_I2cIt66021Scl,
    eGpioName_I2cIt66021Sda,
    eGpioName_I2cIt6615Scl,
    eGpioName_I2cIt6615Sda,
    eGpioName_I2cEepromScl,
    eGpioName_I2cEepromSda,
    eGpioName_I2cIdt5v49ee902Scl,
    eGpioName_I2cIdt5v49ee902Sda,

    // SPI
    eGpioName_Tm1623Cs,
    eGpioName_Tm1623Sclk,
    eGpioName_Tm1623Sda,
    eGpioName_FpgaFlashCs,
    eGpioName_FpgaFlashSclk,
    eGpioName_FpgaFlashMiso,
    eGpioName_FpgaFlashMosi,

    // Ir
    eGpioName_IrIn,

    // IDT
    //eGpioName_IDTSclk,
    //eGpioName_IDTSda,

    // Fan
    eGpioName_CtrlFan,

    // SysReset
    eGpioName_SysReset,

    // Output 5V
    eGpioName_CtrlOutput5V,
    // Output HPD
    eGpioName_OutputHPDPinDetect,

    // UsbDetect
    eGpioName_UsbDetect,

	eGpioName_Max,
}eProjectGpioName_t;

typedef enum _eI2cDevice_t_
{
	eI2cDevice_Eeprom = 0,
	eI2cDevice_It66321_1,
    eI2cDevice_It66321_2,
    eI2cDevice_It66321_3,
    eI2cDevice_It66321_4,
    eI2cDevice_It66021_1,
    eI2cDevice_It66021_2,
    eI2cDevice_It66021_3,
    eI2cDevice_It66021_4,
    eI2cDevice_It6615,
    eI2cDevice_Idt5v49ee902,

	eI2cDevice_Max,
}eI2cDevice_t;

typedef enum _eSw4052Device_t_
{
	eSw4052Device_It66321 = 0,
    eSw4052Device_It66021,

	eSw4052Device_Max,
}eSw4052Device_t;

typedef enum _eSpiDevice_t_
{
	eSpiDevice_Tm1623 = 0,
    eSpiDevice_FpgaFlash,

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

bool mapp_UsbInputDetect(void);
void mapp_SetSysResetPin(u8 u8Mode);
void mapp_SetFpgaProgramPin(u8 u8Mode);
void mapp_SelectFpgaFlashUpdateMode(bool bEnable);
bool mapp_GetOutHPDState(void);
void mapp_SetStartupLedStateInit(void);

#endif

