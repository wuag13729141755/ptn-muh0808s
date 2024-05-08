#include "includes.h"

#define DELAY_TIME_FOR_SPI_FLASH_BYTE      5
#define DELAY_TIME_FOR_SPI_FLASH_CLK       5

const stGpioConfig_t stProjectGpioTable[eGpioName_Max]=
{
    //eGpioName_LedActive,
    {PORT_GPIOE,       GPIO_Pin_11,   GPIO_Mode_OUT, GPIO_OType_PP,     GPIO_PuPd_UP,   ePinLevel_High},
    //eGpioName_LedIn1,
    {PORT_TM1623_GRID1_L(0),  SEG1,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_LedIn2,
    {PORT_TM1623_GRID1_L(0),  SEG2,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_LedIn3,
    {PORT_TM1623_GRID1_L(0),  SEG3,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_LedIn4,
    {PORT_TM1623_GRID1_L(0),  SEG4,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_LedIn5,
    {PORT_TM1623_GRID1_L(0),  SEG5,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_LedIn6,
    {PORT_TM1623_GRID1_L(0),  SEG6,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_LedIn7,
    {PORT_TM1623_GRID1_L(0),  SEG7,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_LedIn8,
    {PORT_TM1623_GRID1_L(0),  SEG8,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_LedOut1,
    {PORT_TM1623_GRID2_L(0),  SEG1,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_LedOut2,
    {PORT_TM1623_GRID2_L(0),  SEG2,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_LedOut3,
    {PORT_TM1623_GRID2_L(0),  SEG3,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_LedOut4,
    {PORT_TM1623_GRID2_L(0),  SEG4,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_LedOut5,
    {PORT_TM1623_GRID2_L(0),  SEG5,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_LedOut6,
    {PORT_TM1623_GRID2_L(0),  SEG6,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_LedOut7,
    {PORT_TM1623_GRID2_L(0),  SEG7,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_LedOut8,
    {PORT_TM1623_GRID2_L(0),  SEG8,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_LedAll,
    {PORT_TM1623_GRID3_L(0),  SEG3,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_LedLock,
    {PORT_TM1623_GRID3_L(0),  SEG4,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_LedMode,
    {PORT_TM1623_GRID3_L(0),  SEG5,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_LedEnter,
    {PORT_TM1623_GRID3_L(0),  SEG8,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_LedScene,
    {PORT_TM1623_GRID3_L(1),  SEG9,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_LedClear,
    {PORT_TM1623_GRID3_L(1), SEG10,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},

    // key
    //eGpioName_KeyIn1,
    {PORT_TM1623_KVAL_0(0),  KS1_K1,   GPIO_Mode_IN, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_KeyIn2,
    {PORT_TM1623_KVAL_0(0),  KS2_K1,   GPIO_Mode_IN, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_KeyIn3,
    {PORT_TM1623_KVAL_1(0),  KS3_K1,   GPIO_Mode_IN, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_KeyIn4,
    {PORT_TM1623_KVAL_1(0),  KS4_K1,   GPIO_Mode_IN, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_KeyIn5,
    {PORT_TM1623_KVAL_2(0),  KS5_K1,   GPIO_Mode_IN, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_KeyIn6,
    {PORT_TM1623_KVAL_2(0),  KS6_K1,   GPIO_Mode_IN, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_KeyIn7,
    {PORT_TM1623_KVAL_3(0),  KS7_K1,   GPIO_Mode_IN, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_KeyIn8,
    {PORT_TM1623_KVAL_3(0),  KS8_K1,   GPIO_Mode_IN, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_KeyOut1,
    {PORT_TM1623_KVAL_0(0),  KS1_K2,   GPIO_Mode_IN, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_KeyOut2,
    {PORT_TM1623_KVAL_0(0),  KS2_K2,   GPIO_Mode_IN, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_KeyOut3,
    {PORT_TM1623_KVAL_1(0),  KS3_K2,   GPIO_Mode_IN, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_KeyOut4,
    {PORT_TM1623_KVAL_1(0),  KS4_K2,   GPIO_Mode_IN, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_KeyOut5,
    {PORT_TM1623_KVAL_2(0),  KS5_K2,   GPIO_Mode_IN, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_KeyOut6,
    {PORT_TM1623_KVAL_2(0),  KS6_K2,   GPIO_Mode_IN, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_KeyOut7,
    {PORT_TM1623_KVAL_3(0),  KS7_K2,   GPIO_Mode_IN, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_KeyOut8,
    {PORT_TM1623_KVAL_3(0),  KS8_K2,   GPIO_Mode_IN, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_KeyAll,
    {PORT_TM1623_KVAL_1(0),  KS3_K3,   GPIO_Mode_IN, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_KeyLock,
    {PORT_TM1623_KVAL_1(0),  KS4_K3,   GPIO_Mode_IN, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_KeyMode,
    {PORT_TM1623_KVAL_2(0),  KS5_K3,   GPIO_Mode_IN, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_KeyEnter,
    {PORT_TM1623_KVAL_3(0),  KS8_K3,   GPIO_Mode_IN, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_KeyScene,
    {PORT_TM1623_KVAL_4(0),  KS9_K3,   GPIO_Mode_IN, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_KeyClear,
    {PORT_TM1623_KVAL_4(0), KS10_K3,   GPIO_Mode_IN, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},

    //i2c
    //eGpioName_I2cEepromScl
    {PORT_GPIOE,        GPIO_Pin_4,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_I2cEepromSda
    {PORT_GPIOE,        GPIO_Pin_5,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_I2cMS1826Scl_1,
    {PORT_GPIOD,       GPIO_Pin_10,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_I2cMS1826Sda_1,
    {PORT_GPIOD,       GPIO_Pin_11,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_I2cMS1826Scl_2,
    {PORT_GPIOD,       GPIO_Pin_12,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_I2cMS1826Sda_2,
    {PORT_GPIOD,       GPIO_Pin_13,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_I2cMS1826Scl_3,
    {PORT_GPIOD,       GPIO_Pin_14,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_I2cMS1826Sda_3,
    {PORT_GPIOD,       GPIO_Pin_15,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_I2cMS1826Scl_4,
    {PORT_GPIOD,        GPIO_Pin_0,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_I2cMS1826Sda_4,
    {PORT_GPIOD,        GPIO_Pin_1,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_I2cLT8644Scl_1,
    {PORT_GPIOC,        GPIO_Pin_6,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_I2cLT8644Sda_1,
    {PORT_GPIOC,        GPIO_Pin_7,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_I2cLT8644Scl_2,
    {PORT_GPIOC,        GPIO_Pin_8,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_I2cLT8644Sda_2,
    {PORT_GPIOC,        GPIO_Pin_9,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_I2cPCA9539Scl,
    {PORT_GPIOD,        GPIO_Pin_3,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_I2cPCA9539Sda,
    {PORT_GPIOD,        GPIO_Pin_4,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_I2cAlpuScl,
    {PORT_GPIOE,        GPIO_Pin_3,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_I2cAlpuSda,
    {PORT_GPIOE,        GPIO_Pin_2,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},

    //Spi
    //eGpioName_SpiTm1623Clk1,
    {PORT_GPIOB,        GPIO_Pin_4,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_SpiTm1623Cs1,
    {PORT_GPIOB,        GPIO_Pin_5,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_SpiTm1623Dio1,
    {PORT_GPIOB,        GPIO_Pin_3,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},

    //HDMI OUT 5V
    //eGpioName_OUT5V_HDMI1,
    {PORT_GPIOC,        GPIO_Pin_2,   GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_OUT5V_HDMI2,
    {PORT_GPIOC,        GPIO_Pin_0,   GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_OUT5V_HDMI3,
    {PORT_GPIOA,        GPIO_Pin_8,   GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_OUT5V_HDMI4,
    {PORT_GPIOE,        GPIO_Pin_6,   GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_OUT5V_HDMI5,
    {PORT_GPIOB,        GPIO_Pin_0,   GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_OUT5V_HDMI6,
    {PORT_GPIOB,        GPIO_Pin_1,   GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_OUT5V_HDMI7,
    {PORT_GPIOA,       GPIO_Pin_11,   GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_OUT5V_HDMI8,
    {PORT_GPIOA,       GPIO_Pin_12,   GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL,   ePinLevel_High},

    //Reset
    //eGpioName_MS1826Reset1,
    {PORT_GPIOE,        GPIO_Pin_7,   GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_MS1826Reset2,
    {PORT_GPIOE,        GPIO_Pin_8,   GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_MS1826Reset3,
    {PORT_GPIOE,        GPIO_Pin_9,   GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_MS1826Reset4,
    {PORT_GPIOE,       GPIO_Pin_10,   GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_PhyRst,
    {PORT_GPIOA,       GPIO_Pin_15,   GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_PCA9539Rst
    {PORT_GPIOD,        GPIO_Pin_7,   GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_LT8644Rst1,
    {PORT_GPIOE,       GPIO_Pin_12,   GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_LT8644Rst2,
    {PORT_GPIOE,       GPIO_Pin_13,   GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL,   ePinLevel_High},

    //MS1826_INT
    //eGpioName_MS1826Int1,
    {PORT_GPIOA,        GPIO_Pin_4,    GPIO_Mode_IN, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_MS1826Int2,
    {PORT_GPIOA,        GPIO_Pin_5,    GPIO_Mode_IN, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_MS1826Int3,
    {PORT_GPIOA,        GPIO_Pin_6,    GPIO_Mode_IN, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_MS1826Int4,
    {PORT_GPIOB,        GPIO_Pin_9,    GPIO_Mode_IN, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},

    // LCD
    //eGpioName_LCDSck,
    {PORT_GPIOB,       GPIO_Pin_15,   GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_LCDSda,
    {PORT_GPIOB,       GPIO_Pin_14,   GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_LCDRS,
    {PORT_GPIOB,        GPIO_Pin_8,   GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_LCDRst,
    {PORT_GPIOB,        GPIO_Pin_7,   GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_LCDBacklight,
    {PORT_GPIOB,        GPIO_Pin_6,   GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL,   ePinLevel_High},

    // esc
    //eGpioName_LT8644Update1,
    {PORT_GPIOE,       GPIO_Pin_14,   GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_LT8644Update2,
    {PORT_GPIOE,       GPIO_Pin_15,   GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_CecIn,
    {PORT_GPIOE,        GPIO_Pin_0,   GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_CecOut,
    {PORT_GPIOE,        GPIO_Pin_1,   GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL,   ePinLevel_High},

    // audio mute pin
    //eGpioName_Pca9539PinAudioMute1,
    {  PORT_PCA,        GPIO_Pin_0,  GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_Pca9539PinAudioMute2,
    {  PORT_PCA,        GPIO_Pin_1,  GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_Pca9539PinAudioMute3,
    {  PORT_PCA,        GPIO_Pin_2,  GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_Pca9539PinAudioMute4,
    {  PORT_PCA,        GPIO_Pin_3,  GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_Pca9539PinAudioMute5,
    {  PORT_PCA,        GPIO_Pin_4,  GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_Pca9539PinAudioMute6,
    {  PORT_PCA,        GPIO_Pin_5,  GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_Pca9539PinAudioMute7,
    {  PORT_PCA,        GPIO_Pin_6,  GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_Pca9539PinAudioMute8,
    {  PORT_PCA,        GPIO_Pin_7,  GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL,   ePinLevel_High},

};

const stKeyDeclare_t stProjectKeyTable[] =
{
    //key null
    {{NULL, \
    NULL, \
    NULL, \
    NULL}, \
    key_detect_mode_after, \
    KEY_LONG_INVALID, \
    {KEY_REPEAT_INVALID, KEY_REPEAT_START_DELAY_NONE}, \
    {{{NULL}, KEY_FILTER_TIME}},\
    NULL, \
    IR_REMOTE_KEY_INVALID, \
    NULL}
};

const stLedDeclare_t stProjectLedTable[] =
{
    //led_name_ActiveLed = 0,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_LedActive],             ePinLevel_Low},
    //led_name_LedInput1,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_LedIn1],                ePinLevel_High},
    //led_name_LedInput2,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_LedIn2],                ePinLevel_High},
    //led_name_LedInput3,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_LedIn3],                ePinLevel_High},
    //led_name_LedInput4,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_LedIn4],                ePinLevel_High},
    //led_name_LedInput5,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_LedIn5],                ePinLevel_High},
    //led_name_LedInput6,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_LedIn6],                ePinLevel_High},
    //led_name_LedInput7,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_LedIn7],                ePinLevel_High},
    //led_name_LedInput8,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_LedIn8],                ePinLevel_High},
    //led_name_LedOutput1,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_LedOut1],               ePinLevel_High},
    //led_name_LedOutput2,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_LedOut2],               ePinLevel_High},
    //led_name_LedOutput3,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_LedOut3],               ePinLevel_High},
    //led_name_LedOutput4,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_LedOut4],               ePinLevel_High},
    //led_name_LedOutput5,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_LedOut5],               ePinLevel_High},
    //led_name_LedOutput6,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_LedOut6],               ePinLevel_High},
    //led_name_LedOutput7,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_LedOut7],               ePinLevel_High},
    //led_name_LedOutput8,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_LedOut8],               ePinLevel_High},
    //led_name_LedAll,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_LedAll],                ePinLevel_High},
    //led_name_LedLock,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_LedLock],               ePinLevel_High},
    //led_name_LedMode,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_LedMode],               ePinLevel_High},
    //led_name_LedEnter,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_LedEnter],              ePinLevel_High},
    //led_name_LedScene,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_LedScene],              ePinLevel_High},
    //led_name_LedClear,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_LedClear],              ePinLevel_High},

    //led_name_End
    {(pstGpioConfig_t)NULL,                                                 ePinLevel_Low},// the last one
};

const stI2cDeclare_t stProjectI2cTable[] =
{
    //eI2cDevice_Eeprom = 0,
    {   (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cEepromScl],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cEepromSda],
        I2C_SPEED_TIME_FOR_75K,
        0,
        0,
        },
    //eI2cDevice_MS1826_1,
    {   (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cMS1826Scl_1],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cMS1826Sda_1],
        I2C_SPEED_TIME_FOR_400K,
        0,
        0,
        },
    //eI2cDevice_MS1826_2,
    {   (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cMS1826Scl_2],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cMS1826Sda_2],
        I2C_SPEED_TIME_FOR_400K,
        0,
        0,
        },
    //eI2cDevice_MS1826_3,
    {   (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cMS1826Scl_3],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cMS1826Sda_3],
        I2C_SPEED_TIME_FOR_400K,
        0,
        0,
        },
    //eI2cDevice_MS1826_4,
    {   (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cMS1826Scl_4],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cMS1826Sda_4],
        I2C_SPEED_TIME_FOR_400K,
        0,
        0,
        },
    //eI2cDevice_LT8644_1,
    {   (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cLT8644Scl_1],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cLT8644Sda_1],
        I2C_SPEED_TIME_FOR_100K,
        0,
        0,
        },
    //eI2cDevice_LT8644_2,
    {   (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cLT8644Scl_2],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cLT8644Sda_2],
        I2C_SPEED_TIME_FOR_100K,
        0,
        0,
        },
    //eI2cDevice_PCA9539,
    {   (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cPCA9539Scl],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cPCA9539Sda],
        I2C_SPEED_TIME_FOR_300K,
        0,
        0,
        },
    //eI2cDevice_ALPU_C,
    {   (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cAlpuCScl],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cAlpuCSda],
        I2C_SPEED_TIME_FOR_100K,
        0,
        0,
        },

    //i2c null
    {   NULL,
        NULL,
        0,
        0,
        0,
        },
};

const stI2CSwitchCtrl_t stProjectI2cSwitchTable[] =
{
    // switch null
    {NULL, NULL},
};

const stSpiDeclare_t stProjectSpiTable[] =
{
    // tm1623
    {   (pstGpioConfig_t)&stProjectGpioTable[eGpioName_SpiTm1623Dio1],
        NULL,
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_SpiTm1623Clk1],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_SpiTm1623Cs1],
        DELAY_TIME_FOR_SPI_FLASH_BYTE,
        DELAY_TIME_FOR_SPI_FLASH_CLK,
        0,
        },

    //spi null
    {NULL,                                                          NULL,\
     NULL,                                                          NULL,\
     100,                                                           100   },
};

const stRxHpdDetDeclare_t stProjectRxHpdDetTable[] =
{
    {NULL},
};

const stIT663xxRstDeclare_t stProjectIT663xxRstTable[] =
{
    {NULL,                                                      ePinLevel_High},
};

const stIrWorkDeclare_t stProjectIrWorkTable =
{
    //{(pstGpioConfig_t)&stProjectGpioTable[eGpioName_IrSensor], eIrRmProtocol_NEC},
    {NULL, eIrRmProtocol_None},
    NULL,
};

const st5VControl_t stProjectTx5VControlTable[] =
{
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_OUT5V_HDMI1]},
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_OUT5V_HDMI2]},
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_OUT5V_HDMI3]},
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_OUT5V_HDMI4]},
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_OUT5V_HDMI5]},
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_OUT5V_HDMI6]},
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_OUT5V_HDMI7]},
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_OUT5V_HDMI8]},
    //hdmi output 5v null
    {NULL},
};

const stUartDeclare_t stProjectUartWorkTable[] =
{
#if (_ENABLE_UART_1 == _MACRO_ENABLE)
    //uart port 1   --- function
    {   _UART_PORT_1,
        {115200, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No, (USART_Mode_Rx|USART_Mode_Tx), USART_HardwareFlowControl_None}, \
        FALSE,
        Uart1ProtocalDataReceive,
        Uart1ParserMethod,
        Uart1VarInit,
        Uart1VarTimeOut},
#endif
#if (_ENABLE_UART_2 == _MACRO_ENABLE)
    //uart port 2   --- function2
    {   _UART_PORT_2_PD5_6,
        {115200, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No, (USART_Mode_Rx|USART_Mode_Tx), USART_HardwareFlowControl_None},
        TRUE,
        Uart2ProtocalDataReceive,
        Uart2ParserMethod,
        Uart2VarInit,
        Uart2VarTimeOut},
#endif
#if (_ENABLE_UART_3 == _MACRO_ENABLE)
    //uart port 3   --- debug
    {   _UART_PORT_3_PD8_9,
        {115200, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No, (USART_Mode_Rx|USART_Mode_Tx), USART_HardwareFlowControl_None},
        FALSE,
        Uart3ProtocalDataReceive,
        Uart3ParserMethod,
        Uart3VarInit,
        Uart3VarTimeOut},
#endif
#if (_ENABLE_UART_4 == _MACRO_ENABLE)
    //uart port 4   --- reserve
    {   _UART_PORT_4,
        {115200, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No, (USART_Mode_Rx|USART_Mode_Tx), USART_HardwareFlowControl_None},
        FALSE,//TRUE,
        Uart4ProtocalDataReceive,
        Uart4ParserMethod,
        Uart4VarInit,
        Uart4VarTimeOut},
#endif
#if (_ENABLE_UART_5 == _MACRO_ENABLE)
    //uart port 5 --- gui
    {   _UART_PORT_5,
        {921600, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No, (USART_Mode_Rx|USART_Mode_Tx), USART_HardwareFlowControl_None},
        FALSE,
        Uart5ProtocalDataReceive,
        Uart5ParserMethod,
        Uart5VarInit,
        Uart5VarTimeOut},
#endif
#if (_ENABLE_UART_6 == _MACRO_ENABLE)
    //uart port 6   --- NOTUSE
    {   _UART_PORT_6,
        {9600, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No, (USART_Mode_Rx|USART_Mode_Tx), USART_HardwareFlowControl_None},
        FALSE,//TRUE,
        Uart6ProtocalDataReceive,
        Uart6ParserMethod,
        Uart6VarInit,
        Uart6VarTimeOut},
#endif

    //uart port none
    {   _UART_PORT_NONE,
        {115200, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No, (USART_Mode_Rx|USART_Mode_Tx), USART_HardwareFlowControl_None},
        FALSE,
        NULL,
        NULL,
        NULL,
        NULL}
};

const stCecDeclare_t stProjectCecWorkTable[] =
{
    //cec none
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_CecIn],         {eCecSrcDisType_Src, FALSE, 0, 0x00}},
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_CecOut],        {eCecSrcDisType_Dis, FALSE, 1, 0x40}},

    {NULL,                                                          {eCecSrcDisType_Src, FALSE, 0, 0x00}}
};

#if _ENABLE_NEW_USER_KEY_DEFINE_FUNCTION
const stNewKeyDeclare_t stProjectNewKeyTable[] =
{
    //emKeyName_In1 = 0,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_KeyIn1],        0,  emKeyvalueInput1,   ePinLevel_Low,  FALSE},
    //emKeyName_In2,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_KeyIn2],        0,  emKeyvalueInput2,   ePinLevel_Low,  FALSE},
    //emKeyName_In3,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_KeyIn3],        0,  emKeyvalueInput3,   ePinLevel_Low,  FALSE},
    //emKeyName_In4,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_KeyIn4],        0,  emKeyvalueInput4,   ePinLevel_Low,  FALSE},
    //emKeyName_In5,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_KeyIn5],        0,  emKeyvalueInput5,   ePinLevel_Low,  FALSE},
    //emKeyName_In6,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_KeyIn6],        0,  emKeyvalueInput6,   ePinLevel_Low,  FALSE},
    //emKeyName_In7,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_KeyIn7],        0,  emKeyvalueInput7,   ePinLevel_Low,  FALSE},
    //emKeyName_In8,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_KeyIn8],        0,  emKeyvalueInput8,   ePinLevel_Low,  FALSE},

    //emKeyName_Out1,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_KeyOut1],       0,  emKeyvalueOutput1,  ePinLevel_Low,  FALSE},
    //emKeyName_Out2,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_KeyOut2],       0,  emKeyvalueOutput2,  ePinLevel_Low,  FALSE},
    //emKeyName_Out3,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_KeyOut3],       0,  emKeyvalueOutput3,  ePinLevel_Low,  FALSE},
    //emKeyName_Out4,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_KeyOut4],       0,  emKeyvalueOutput4,  ePinLevel_Low,  FALSE},
    //emKeyName_Out5,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_KeyOut5],       0,  emKeyvalueOutput5,  ePinLevel_Low,  FALSE},
    //emKeyName_Out6,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_KeyOut6],       0,  emKeyvalueOutput6,  ePinLevel_Low,  FALSE},
    //emKeyName_Out7,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_KeyOut7],       0,  emKeyvalueOutput7,  ePinLevel_Low,  FALSE},
    //emKeyName_Out8,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_KeyOut8],       0,  emKeyvalueOutput8,  ePinLevel_Low,  FALSE},

    //emKeyName_All,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_KeyAll],        0,  emKeyvalueAll,      ePinLevel_Low,  FALSE},
    //emKeyName_Lock,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_KeyLock],       0,  emKeyvalueLock,     ePinLevel_Low,  FALSE},
    //emKeyName_Mode,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_KeyMode],       0,  emKeyvalueMode,     ePinLevel_Low,  FALSE},
    //emKeyName_Enter,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_KeyEnter],      0,  emKeyvalueEnter,    ePinLevel_Low,  FALSE},
    //emKeyName_Scene,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_KeyScene],      0,  emKeyvalueScene,    ePinLevel_Low,  FALSE},
    //emKeyName_Clear
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_KeyClear],      0,  emKeyvalueClear,    ePinLevel_Low,  FALSE},

    {(pstGpioConfig_t)NULL,                                         0,  BIT0,               ePinLevel_Low,  FALSE},// the last one
};

#endif


#if _ENABLE_CHIP_GSV_CHIP_SERIES
const stGsvChipResetPinCfg_t stProjectGsvChipResetTable[] =
{
//    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_Gsv2001Reset],        ePinLevel_Low},

    // none
    {NULL,ePinLevel_Low},
};
#endif

#if _ENABLE_CHIP_MS1826
const stMS1826ChipResetPinCfg_t stProjectMS1826ChipResetTable[] =
{
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_MS1826Reset1],        ePinLevel_Low},
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_MS1826Reset2],        ePinLevel_Low},
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_MS1826Reset3],        ePinLevel_Low},
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_MS1826Reset4],        ePinLevel_Low},

    // none
    {NULL,ePinLevel_Low},
};
#endif
#if _EN_CHIP_LT8644_USE
const stDrvLT8644VarConfig stProjectLT8644Config[] =
{
    {
        LT8644_DEV_ADDR_4,
        _I2C_TABLE_INDEX_LT8644_0,
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_LT8644Update1],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_LT8644Rst1],
        ePinLevel_Low,
    },
    {
        LT8644_DEV_ADDR_4,
        _I2C_TABLE_INDEX_LT8644_1,
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_LT8644Update2],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_LT8644Rst2],
        ePinLevel_Low,
    },

    // end
    {
        0,
        _I2C_TABLE_INDEX_LT8644_0,
        NULL,
        NULL,
        ePinLevel_Low
    }
};
#endif
#if ENABLE_SERIES_LCD_FUNCTION
const stSpiLcdPinCfg_t stProjectSpiLcdPinTable[] =
{
    // LcdBkLight
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_LCDBacklight],
    // LcdReset
    (pstGpioConfig_t)&stProjectGpioTable[eGpioName_LCDRst],
    // LcdCs
    NULL,//(pstGpioConfig_t)&stProjectGpioTable[eGpioName_LCD_CS],
    // LcdRs
    (pstGpioConfig_t)&stProjectGpioTable[eGpioName_LCDRS],
    // LcdSclk
    (pstGpioConfig_t)&stProjectGpioTable[eGpioName_LCDSck],
    // LcdData
    (pstGpioConfig_t)&stProjectGpioTable[eGpioName_LCDSda]},

    {NULL,NULL,NULL,NULL,NULL,NULL},
};
#endif

void mapp_SetHdmiTx5V(bool bEnable)
{
	pst5VControl_t p5vCtlPort = (pst5VControl_t)stProjectTx5VControlTable;
    while(p5vCtlPort->pstControlPort != NULL)
    {
		if(p5vCtlPort->pstControlPort->eInitLevel == _GPIO_PIN_LEVEL_LOW)
		{
			if(bEnable)
                GPIO_ResetBits(STM32_GPIO_PORT[p5vCtlPort->pstControlPort->u8PortType-PORT_GPIOA], p5vCtlPort->pstControlPort->u16GpioPin);
			else
                GPIO_SetBits(STM32_GPIO_PORT[p5vCtlPort->pstControlPort->u8PortType-PORT_GPIOA], p5vCtlPort->pstControlPort->u16GpioPin);
		}
		else
		{
			if(bEnable)
                GPIO_SetBits(STM32_GPIO_PORT[p5vCtlPort->pstControlPort->u8PortType-PORT_GPIOA], p5vCtlPort->pstControlPort->u16GpioPin);
			else
                GPIO_ResetBits(STM32_GPIO_PORT[p5vCtlPort->pstControlPort->u8PortType-PORT_GPIOA], p5vCtlPort->pstControlPort->u16GpioPin);
		}
		p5vCtlPort++;
    }
}

void mapp_BacklightEN(bool bSta)
{
    u8 l_u8EnPort = stProjectGpioTable[eGpioName_LCDBacklight].u8PortType;
    u16 l_u16EnPin = stProjectGpioTable[eGpioName_LCDBacklight].u16GpioPin;

    if(bSta)
    {
        GPIO_WriteBit(STM32_GPIO_PORT[l_u8EnPort-PORT_GPIOA],l_u16EnPin,Bit_SET);
    }
    else
    {
        GPIO_WriteBit(STM32_GPIO_PORT[l_u8EnPort-PORT_GPIOA],l_u16EnPin,Bit_RESET);
    }
}

void mapp_SetAudioMuteByPCA9539(u8 u8Num, bool bEn)
{
    u8 l_u8EnPort = stProjectGpioTable[u8Num+eGpioName_Pca9539PinAudioMute1].u8PortType;
    u16 l_u16EnPin = stProjectGpioTable[u8Num+eGpioName_Pca9539PinAudioMute1].u16GpioPin;

    if(l_u8EnPort != PORT_PCA)
        return;

    #if (_EN_CHIP_PCA9539_USE == _MACRO_ENABLE)
    if(bEn)
    {
        pca_output_val |= l_u16EnPin;
    }
    else
    {
        pca_output_val &= (~l_u16EnPin);
    }
    #endif
}


