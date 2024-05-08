#include "includes.h"

#define DELAY_TIME_FOR_SPI_FLASH_BYTE      5
#define DELAY_TIME_FOR_SPI_FLASH_CLK       5

const stGpioConfig_t stProjectGpioTable[eGpioName_Max]=
{
    //eGpioName_LedActive,
    {PORT_GPIOE,       GPIO_Pin_11,   GPIO_Mode_OUT, GPIO_OType_PP,     GPIO_PuPd_UP,   ePinLevel_High},
    //eGpioName_LedIn1_D3,
    {PORT_TM1623_GRID1_L(0),  SEG1,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_LedIn2_D4,
    {PORT_TM1623_GRID2_L(0),  SEG1,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_LedIn3_D5,
    {PORT_TM1623_GRID3_L(0),  SEG1,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_LedIn4_D6,
    {PORT_TM1623_GRID1_L(0),  SEG2,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_LedIn5_D7,
    {PORT_TM1623_GRID2_L(0),  SEG2,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_LedIn6_D8,
    {PORT_TM1623_GRID3_L(0),  SEG2,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_LedIn7_D9,
    {PORT_TM1623_GRID1_L(0),  SEG3,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_LedIn8_D10,
    {PORT_TM1623_GRID2_L(0),  SEG3,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_LedIn9_D11,
    {PORT_TM1623_GRID3_L(0),  SEG3,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_LedIn10_D12,
    {PORT_TM1623_GRID1_L(0),  SEG4,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_LedIn11_D30,
    {PORT_TM1623_GRID3_H(0),  SEG9,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_LedOut1_D16,
    {PORT_TM1623_GRID2_L(0),  SEG5,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_LedOut2_D17,
    {PORT_TM1623_GRID3_L(0),  SEG5,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_LedOut3_D19,
    {PORT_TM1623_GRID1_L(0),  SEG6,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_LedOut4_D20,
    {PORT_TM1623_GRID2_L(0),  SEG6,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_LedOut5_D21,
    {PORT_TM1623_GRID3_L(0),  SEG6,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_LedOut6_D22,
    {PORT_TM1623_GRID1_L(0),  SEG7,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_LedOut7_D23,
    {PORT_TM1623_GRID2_L(0),  SEG7,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_LedOut8_D24,
    {PORT_TM1623_GRID3_L(0),  SEG7,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_LedOut9_D25,
    {PORT_TM1623_GRID1_L(0),  SEG8,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_LedOut10_D26,
    {PORT_TM1623_GRID2_L(0),  SEG8,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_LedOut11_D31,
    {PORT_TM1623_GRID3_H(0), SEG10,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_LedAll_D13,
    {PORT_TM1623_GRID2_L(0),  SEG4,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_LedLock_D14,
    {PORT_TM1623_GRID3_L(0),  SEG4,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_LedMode_D15,
    {PORT_TM1623_GRID1_L(0),  SEG5,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_LedEnter_D29,
    {PORT_TM1623_GRID3_L(0),  SEG8,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_LedScene_D27,
    {PORT_TM1623_GRID1_H(0),  SEG9,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_LedClear_D28,
    {PORT_TM1623_GRID2_H(0),  SEG9,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},

    // key
    //eGpioName_KeyIn1_K1,
    {PORT_TM1623_KVAL_0(0),  KS1_K1,   GPIO_Mode_IN, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_KeyIn2_K2,
    {PORT_TM1623_KVAL_0(0),  KS1_K2,   GPIO_Mode_IN, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_KeyIn3_K3,
    {PORT_TM1623_KVAL_0(0),  KS1_K3,   GPIO_Mode_IN, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_KeyIn4_K4,
    {PORT_TM1623_KVAL_0(0),  KS2_K1,   GPIO_Mode_IN, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_KeyIn5_K5,
    {PORT_TM1623_KVAL_0(0),  KS2_K2,   GPIO_Mode_IN, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_KeyIn6_K6,
    {PORT_TM1623_KVAL_0(0),  KS2_K3,   GPIO_Mode_IN, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_KeyIn7_K7,
    {PORT_TM1623_KVAL_1(0),  KS3_K1,   GPIO_Mode_IN, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_KeyIn8_K8,
    {PORT_TM1623_KVAL_1(0),  KS3_K2,   GPIO_Mode_IN, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_KeyIn9_K9,
    {PORT_TM1623_KVAL_1(0),  KS3_K3,   GPIO_Mode_IN, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_KeyIn10_K10,
    {PORT_TM1623_KVAL_1(0),  KS4_K1,   GPIO_Mode_IN, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_KeyIn11_K27,
    {PORT_TM1623_KVAL_4(0),  KS9_K3,   GPIO_Mode_IN, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_KeyOut1_K14,
    {PORT_TM1623_KVAL_2(0),  KS5_K2,   GPIO_Mode_IN, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_KeyOut2_K15,
    {PORT_TM1623_KVAL_2(0),  KS5_K3,   GPIO_Mode_IN, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_KeyOut3_K16,
    {PORT_TM1623_KVAL_2(0),  KS6_K1,   GPIO_Mode_IN, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_KeyOut4_K17,
    {PORT_TM1623_KVAL_2(0),  KS6_K2,   GPIO_Mode_IN, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_KeyOut5_K18,
    {PORT_TM1623_KVAL_2(0),  KS6_K3,   GPIO_Mode_IN, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_KeyOut6_K19,
    {PORT_TM1623_KVAL_3(0),  KS7_K1,   GPIO_Mode_IN, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_KeyOut7_K20,
    {PORT_TM1623_KVAL_3(0),  KS7_K2,   GPIO_Mode_IN, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_KeyOut8_K21,
    {PORT_TM1623_KVAL_3(0),  KS7_K3,   GPIO_Mode_IN, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_KeyOut9_K22,
    {PORT_TM1623_KVAL_3(0),  KS8_K1,   GPIO_Mode_IN, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_KeyOut10_K23,
    {PORT_TM1623_KVAL_3(0),  KS8_K2,   GPIO_Mode_IN, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_KeyOut11_K28,
    {PORT_TM1623_KVAL_4(0), KS10_K3,   GPIO_Mode_IN, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_KeyAll_K11,
    {PORT_TM1623_KVAL_1(0),  KS4_K2,   GPIO_Mode_IN, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_KeyLock_K12,
    {PORT_TM1623_KVAL_1(0),  KS4_K3,   GPIO_Mode_IN, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_KeyMode_K13,
    {PORT_TM1623_KVAL_2(0),  KS5_K1,   GPIO_Mode_IN, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_KeyEnter_K24,
    {PORT_TM1623_KVAL_3(0),  KS8_K3,   GPIO_Mode_IN, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_KeyScene_K25,
    {PORT_TM1623_KVAL_4(0),  KS9_K1,   GPIO_Mode_IN, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_KeyClear_K26,
    {PORT_TM1623_KVAL_4(0),  KS9_K2,   GPIO_Mode_IN, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},

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
    //eGpioName_I2cLT8644Scl_3,
    {PORT_GPIOA,        GPIO_Pin_4,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_I2cLT8644Sda_3,
    {PORT_GPIOA,        GPIO_Pin_5,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_I2cLT8644Scl_4,
    {PORT_GPIOA,        GPIO_Pin_6,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_I2cLT8644Sda_4,
    {PORT_GPIOA,        GPIO_Pin_8,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_I2cAlpuCScl,
    {PORT_GPIOE,        GPIO_Pin_3,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_I2cAlpuCSda,
    {PORT_GPIOE,        GPIO_Pin_2,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},

    //Spi
    //eGpioName_SpiTm1623Clk1,
    {PORT_GPIOB,        GPIO_Pin_4,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_SpiTm1623Cs1,
    {PORT_GPIOB,        GPIO_Pin_5,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_SpiTm1623Dio1,
    {PORT_GPIOB,        GPIO_Pin_3,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},

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
    //eGpioName_LT8644Rst3,
    {PORT_GPIOD,        GPIO_Pin_3,   GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_LT8644Rst4,
    {PORT_GPIOD,        GPIO_Pin_4,   GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL,   ePinLevel_High},

    //MS1826_INT
    //eGpioName_MS1826Int1,
    {PORT_GPIOB,        GPIO_Pin_0,    GPIO_Mode_IN, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_MS1826Int2,
    {PORT_GPIOC,        GPIO_Pin_0,    GPIO_Mode_IN, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_MS1826Int3,
    {PORT_GPIOC,        GPIO_Pin_2,    GPIO_Mode_IN, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
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
    //eGpioName_LT8644Update3,
    {PORT_GPIOE,        GPIO_Pin_0,   GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_LT8644Update4,
    {PORT_GPIOE,        GPIO_Pin_1,   GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_CecIn,
    {PORT_GPIOB,        GPIO_Pin_1,   GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_CecOut,
    {PORT_GPIOD,        GPIO_Pin_7,   GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL,   ePinLevel_High},
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
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_LedIn1_D3],             ePinLevel_High},
    //led_name_LedInput2,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_LedIn2_D4],             ePinLevel_High},
    //led_name_LedInput3,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_LedIn3_D5],             ePinLevel_High},
    //led_name_LedInput4,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_LedIn4_D6],             ePinLevel_High},
    //led_name_LedInput5,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_LedIn5_D7],             ePinLevel_High},
    //led_name_LedInput6,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_LedIn6_D8],             ePinLevel_High},
    //led_name_LedInput7,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_LedIn7_D9],             ePinLevel_High},
    //led_name_LedInput8,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_LedIn8_D10],            ePinLevel_High},
    //led_name_LedInput9,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_LedIn9_D11],            ePinLevel_High},
    //led_name_LedInput10,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_LedIn10_D12],           ePinLevel_High},
    //led_name_LedInput10Plus,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_LedIn11_D30],           ePinLevel_High},

    //led_name_LedOutput1,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_LedOut1_D16],           ePinLevel_High},
    //led_name_LedOutput2,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_LedOut2_D17],           ePinLevel_High},
    //led_name_LedOutput3,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_LedOut3_D19],           ePinLevel_High},
    //led_name_LedOutput4,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_LedOut4_D20],           ePinLevel_High},
    //led_name_LedOutput5,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_LedOut5_D21],           ePinLevel_High},
    //led_name_LedOutput6,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_LedOut6_D22],           ePinLevel_High},
    //led_name_LedOutput7,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_LedOut7_D23],           ePinLevel_High},
    //led_name_LedOutput8,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_LedOut8_D24],           ePinLevel_High},
    //led_name_LedOutput9,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_LedOut9_D25],           ePinLevel_High},
    //led_name_LedOutput10,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_LedOut10_D26],          ePinLevel_High},
    //led_name_LedOutput10Plus,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_LedOut11_D31],          ePinLevel_High},

    //led_name_LedAll,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_LedAll_D13],            ePinLevel_High},
    //led_name_LedLock,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_LedLock_D14],           ePinLevel_High},
    //led_name_LedMode,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_LedMode_D15],           ePinLevel_High},
    //led_name_LedEnter,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_LedEnter_D29],          ePinLevel_High},
    //led_name_LedScene,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_LedScene_D27],          ePinLevel_High},
    //led_name_LedClear,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_LedClear_D28],          ePinLevel_High},

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
    //eI2cDevice_LT8644_3,
    {   (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cLT8644Scl_3],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cLT8644Sda_3],
        I2C_SPEED_TIME_FOR_100K,
        0,
        0,
        },
    //eI2cDevice_LT8644_4,
    {   (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cLT8644Scl_4],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cLT8644Sda_4],
        I2C_SPEED_TIME_FOR_100K,
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
//    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_OUT5V_HDMI1]},
//    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_OUT5V_HDMI2]},
//    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_OUT5V_HDMI3]},
//    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_OUT5V_HDMI4]},
//    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_OUT5V_HDMI5]},
//    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_OUT5V_HDMI6]},
//    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_OUT5V_HDMI7]},
//    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_OUT5V_HDMI8]},
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
        FALSE,
        Uart2ProtocalDataReceive,
        Uart2ParserMethod,
        Uart2VarInit,
        Uart2VarTimeOut},
#endif
#if (_ENABLE_UART_3 == _MACRO_ENABLE)
    //uart port 3   --- debug
    {   _UART_PORT_3_PD8_9,
        {115200, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No, (USART_Mode_Rx|USART_Mode_Tx), USART_HardwareFlowControl_None},
        TRUE,
        Uart3ProtocalDataReceive,
        Uart3ParserMethod,
        Uart3VarInit,
        Uart3VarTimeOut},
#endif
#if (_ENABLE_UART_4 == _MACRO_ENABLE)
    //uart port 4   --- to out board
    {   _UART_PORT_4_PC10_11,
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
        {115200, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No, (USART_Mode_Rx|USART_Mode_Tx), USART_HardwareFlowControl_None},
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
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_KeyIn1_K1],     0,  emKeyvalueInput1,   ePinLevel_Low,  FALSE},
    //emKeyName_In2,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_KeyIn2_K2],     0,  emKeyvalueInput2,   ePinLevel_Low,  FALSE},
    //emKeyName_In3,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_KeyIn3_K3],     0,  emKeyvalueInput3,   ePinLevel_Low,  FALSE},
    //emKeyName_In4,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_KeyIn4_K4],     0,  emKeyvalueInput4,   ePinLevel_Low,  FALSE},
    //emKeyName_In5,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_KeyIn5_K5],     0,  emKeyvalueInput5,   ePinLevel_Low,  FALSE},
    //emKeyName_In6,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_KeyIn6_K6],     0,  emKeyvalueInput6,   ePinLevel_Low,  FALSE},
    //emKeyName_In7,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_KeyIn7_K7],     0,  emKeyvalueInput7,   ePinLevel_Low,  FALSE},
    //emKeyName_In8,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_KeyIn8_K8],     0,  emKeyvalueInput8,   ePinLevel_Low,  FALSE},
    //emKeyName_In9,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_KeyIn9_K9],     0,  emKeyvalueInput9,   ePinLevel_Low,  FALSE},
    //emKeyName_In10,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_KeyIn10_K10],   0, emKeyvalueInput10,   ePinLevel_Low,  FALSE},
    //emKeyName_In10Plus,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_KeyIn11_K27],   0, emKeyvalueInput11,   ePinLevel_Low,  FALSE},

    //emKeyName_Out1,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_KeyOut1_K14],   0,  emKeyvalueOutput1,  ePinLevel_Low,  FALSE},
    //emKeyName_Out2,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_KeyOut2_K15],   0,  emKeyvalueOutput2,  ePinLevel_Low,  FALSE},
    //emKeyName_Out3,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_KeyOut3_K16],   0,  emKeyvalueOutput3,  ePinLevel_Low,  FALSE},
    //emKeyName_Out4,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_KeyOut4_K17],   0,  emKeyvalueOutput4,  ePinLevel_Low,  FALSE},
    //emKeyName_Out5,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_KeyOut5_K18],   0,  emKeyvalueOutput5,  ePinLevel_Low,  FALSE},
    //emKeyName_Out6,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_KeyOut6_K19],   0,  emKeyvalueOutput6,  ePinLevel_Low,  FALSE},
    //emKeyName_Out7,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_KeyOut7_K20],   0,  emKeyvalueOutput7,  ePinLevel_Low,  FALSE},
    //emKeyName_Out8,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_KeyOut8_K21],   0,  emKeyvalueOutput8,  ePinLevel_Low,  FALSE},
    //emKeyName_Out9,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_KeyOut9_K22],   0,  emKeyvalueOutput9,  ePinLevel_Low,  FALSE},
    //emKeyName_Out10,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_KeyOut10_K23],  0, emKeyvalueOutput10,  ePinLevel_Low,  FALSE},
    //emKeyName_Out10Plus,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_KeyOut11_K28],  0, emKeyvalueOutput11,  ePinLevel_Low,  FALSE},

    //emKeyName_All,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_KeyAll_K11],    0,  emKeyvalueAll,      ePinLevel_Low,  FALSE},
    //emKeyName_Lock,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_KeyLock_K12],   0,  emKeyvalueLock,     ePinLevel_Low,  FALSE},
    //emKeyName_Mode,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_KeyMode_K13],   0,  emKeyvalueMode,     ePinLevel_Low,  FALSE},
    //emKeyName_Enter,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_KeyEnter_K24],  0,  emKeyvalueEnter,    ePinLevel_Low,  FALSE},
    //emKeyName_Scene,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_KeyScene_K25],  0,  emKeyvalueScene,    ePinLevel_Low,  FALSE},
    //emKeyName_Clear
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_KeyClear_K26],  0,  emKeyvalueClear,    ePinLevel_Low,  FALSE},

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
    {
        LT8644_DEV_ADDR_4,
        _I2C_TABLE_INDEX_LT8644_2,
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_LT8644Update3],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_LT8644Rst3],
        ePinLevel_Low,
    },
    {
        LT8644_DEV_ADDR_4,
        _I2C_TABLE_INDEX_LT8644_3,
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_LT8644Update4],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_LT8644Rst4],
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


