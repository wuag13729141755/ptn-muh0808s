#ifndef __PROJECT_BOARD_CONFIG_H__
#define __PROJECT_BOARD_CONFIG_H__

typedef enum _eProjectGpioName_t_
{
    // Led
    eGpioName_LedSta,
    eGpioName_Led1_D7,
    eGpioName_Led2_D8,
    eGpioName_Led3_D9,
    eGpioName_Led4_D10,
    eGpioName_LedSwitch_D11,
    eGpioName_LedCall_D12,
    eGpioName_LedS1_R,
    eGpioName_LedS1_G,
    eGpioName_LedS1_B,
    eGpioName_LedS2_R,
    eGpioName_LedS2_G,
    eGpioName_LedS2_B,
    eGpioName_LedS3_R,
    eGpioName_LedS3_G,
    eGpioName_LedS3_B,
    eGpioName_LedS4_R,
    eGpioName_LedS4_G,
    eGpioName_LedS4_B,
    eGpioName_LedS5_R,
    eGpioName_LedS5_G,
    eGpioName_LedS5_B,
    eGpioName_LedS6_R,
    eGpioName_LedS6_G,
    eGpioName_LedS6_B,
    eGpioName_LedS7_R,
    eGpioName_LedS7_G,
    eGpioName_LedS7_B,
    eGpioName_LedS8_R,
    eGpioName_LedS8_G,
    eGpioName_LedS8_B,
    eGpioName_LedS9_R,
    eGpioName_LedS9_G,
    eGpioName_LedS9_B,
    eGpioName_LedS10_R,
    eGpioName_LedS10_G,
    eGpioName_LedS10_B,
    eGpioName_LedS11_R,
    eGpioName_LedS11_G,
    eGpioName_LedS11_B,
    eGpioName_LedS12_R,
    eGpioName_LedS12_G,
    eGpioName_LedS12_B,
    eGpioName_LedS13_R,
    eGpioName_LedS13_G,
    eGpioName_LedS13_B,
    eGpioName_LedS14_R,
    eGpioName_LedS14_G,
    eGpioName_LedS14_B,
    eGpioName_LedS15_R,
    eGpioName_LedS15_G,
    eGpioName_LedS15_B,
    eGpioName_LedS16_R,
    eGpioName_LedS16_G,
    eGpioName_LedS16_B,
    eGpioName_LedS17_R,
    eGpioName_LedS17_G,
    eGpioName_LedS17_B,
    eGpioName_LedS18_R,
    eGpioName_LedS18_G,
    eGpioName_LedS18_B,
    eGpioName_LedS19_R,
    eGpioName_LedS19_G,
    eGpioName_LedS19_B,
    eGpioName_LedS20_R,
    eGpioName_LedS20_G,
    eGpioName_LedS20_B,
    eGpioName_LedS21_R,
    eGpioName_LedS21_G,
    eGpioName_LedS21_B,
    eGpioName_LedS22_R,
    eGpioName_LedS22_G,
    eGpioName_LedS22_B,
    eGpioName_LedS23_R,
    eGpioName_LedS23_G,
    eGpioName_LedS23_B,
    eGpioName_LedS24_R,
    eGpioName_LedS24_G,
    eGpioName_LedS24_B,
    eGpioName_LedS25_R,
    eGpioName_LedS25_G,
    eGpioName_LedS25_B,
    eGpioName_LedS26_R,
    eGpioName_LedS26_G,
    eGpioName_LedS26_B,
    eGpioName_LedS27_R,
    eGpioName_LedS27_G,
    eGpioName_LedS27_B,
    eGpioName_LedS28_R,
    eGpioName_LedS28_G,
    eGpioName_LedS28_B,
    eGpioName_LedS29_R,
    eGpioName_LedS29_G,
    eGpioName_LedS29_B,
    eGpioName_LedS30_R,
    eGpioName_LedS30_G,
    eGpioName_LedS30_B,
    eGpioName_LedS31_R,
    eGpioName_LedS31_G,
    eGpioName_LedS31_B,
    eGpioName_LedS32_R,
    eGpioName_LedS32_G,
    eGpioName_LedS32_B,
    eGpioName_LedS33_R,
    eGpioName_LedS33_G,
    eGpioName_LedS33_B,
    eGpioName_LedS34_R,
    eGpioName_LedS34_G,
    eGpioName_LedS34_B,
    eGpioName_LedS35_R,
    eGpioName_LedS35_G,
    eGpioName_LedS35_B,
    eGpioName_LedS36_R,
    eGpioName_LedS36_G,
    eGpioName_LedS36_B,
    eGpioName_LedS37_R,
    eGpioName_LedS37_G,
    eGpioName_LedS37_B,
    eGpioName_LedS38_R,
    eGpioName_LedS38_G,
    eGpioName_LedS38_B,

    // key
    eGpioName_KeyS1,
    eGpioName_KeyS2,
    eGpioName_KeyS3,
    eGpioName_KeyS4,
    eGpioName_KeyS5,
    eGpioName_KeyS6,
    eGpioName_KeyS7,
    eGpioName_KeyS8,
    eGpioName_KeyS9,
    eGpioName_KeyS10,
    eGpioName_KeyS11,
    eGpioName_KeyS12,
    eGpioName_KeyS13,
    eGpioName_KeyS14,
    eGpioName_KeyS15,
    eGpioName_KeyS16,
    eGpioName_KeyS17,
    eGpioName_KeyS18,
    eGpioName_KeyS19,
    eGpioName_KeyS20,
    eGpioName_KeyS21,
    eGpioName_KeyS22,
    eGpioName_KeyS23,
    eGpioName_KeyS24,
    eGpioName_KeyS25,
    eGpioName_KeyS26,
    eGpioName_KeyS27,
    eGpioName_KeyS28,
    eGpioName_KeyS29,
    eGpioName_KeyS30,
    eGpioName_KeyS31,
    eGpioName_KeyS32,
    eGpioName_KeyS33,
    eGpioName_KeyS34,
    eGpioName_KeyS35,
    eGpioName_KeyS36,
    eGpioName_KeyS37,
    eGpioName_KeyS38,

	//i2c
	eGpioName_I2cEepromScl,
	eGpioName_I2cEepromSda,
	eGpioName_I2cAlpuScl,
	eGpioName_I2cAlpuSda,

    //Spi
    eGpioName_SpiTm1623Clk1,
    eGpioName_SpiTm1623Cs1,
    eGpioName_SpiTm1623Dio1,
    eGpioName_SpiTm1623Clk2,
    eGpioName_SpiTm1623Cs2,
    eGpioName_SpiTm1623Dio2,
    eGpioName_SpiW5500Clk,
    eGpioName_SpiW5500Cs,
    eGpioName_SpiW5500Mosi,
    eGpioName_SpiW5500Miso,

	// reset
	eGpioName_W5500Rst,
    // ect
	eGpioName_W5500Int,
    eGpioName_TempIn,
    eGpioName_PhyRst,

	eGpioName_Max,
}eProjectGpioName_t;

typedef enum _eI2cDevice_t_
{
	eI2cDevice_Eeprom = 0,
	eI2cDevice_ADN4604,

	eI2cDevice_Max,
}eI2cDevice_t;

typedef enum _eSpiDevice_t_
{
	eSpiDevice_TM1623_1 = 0,
	eSpiDevice_TM1623_2,
	eSpiDevice_W5500,

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

void mapp_SetFanState(bool bEn);

#endif

