#ifndef __PROJECT_BOARD_CONFIG_H__
#define __PROJECT_BOARD_CONFIG_H__

typedef enum _eProjectGpioName_t_
{
	//key
    eGpioName_VDa0 = 0,
    eGpioName_VDb0,
    eGpioName_VDc0,
    eGpioName_VDd0,
    eGpioName_VDe0,
    eGpioName_VDf0,
    eGpioName_VDg0,
    eGpioName_VDCom1,
    eGpioName_VDCom2,
    eGpioName_VDCom3,
    eGpioName_VDCom4,
    eGpioName_ActiveLed,

    // Key
    eGpioName_TxKeyLeft,
    eGpioName_TxKeyRight,
    eGpioName_RxKeyLeft,
    eGpioName_RxKeyRight,
    // I2C
    eGpioName_I2cEepromScl,
    eGpioName_I2cEepromSda,

    // GPIO
    eGpioName_IO_IROUT,
    eGpioName_IO_IRIN,

	eGpioName_Max,
}eProjectGpioName_t;

typedef enum _eI2cDevice_t_
{
	eI2cDevice_Eeprom = 0,

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

void mapp_GenIOWrite(eProjectGpioName_t eIoName,bool bSta);
void mapp_GenIORead(eProjectGpioName_t eIoName,bool *pbSta);
u8 mapp_GetRxKeySta(u8 u8RxPort);

#endif

