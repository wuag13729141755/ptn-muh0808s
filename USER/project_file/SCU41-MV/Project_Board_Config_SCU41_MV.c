#include "includes.h"

#define DELAY_TIME_FOR_EEPROM               55
#define DELAY_TIME_FOR_ITE663XX             5
#define DELAY_TIME_FOR_ITE66021             25
#define DELAY_TIME_FOR_ITE6615              30

#define DELAY_TIME_FOR_TM1623_BYTE          5
#define DELAY_TIME_FOR_TM1623_CLK           3

#define DELAY_TIME_FOR_FPGA_FLASH_BYTE      5
#define DELAY_TIME_FOR_FPGA_FLASH_CLK       3

const stGpioConfig_t stProjectGpioTable[eGpioName_Max]=
{
    // key
	//eGpioName_KeyInput1 = 0,
    {PORT_TM1623_KVAL_0,        KS1_K1,     GPIO_Mode_IN_FLOATING, ePinLevel_High},
    //eGpioName_KeyInput2,
    {PORT_TM1623_KVAL_0,        KS2_K1,     GPIO_Mode_IN_FLOATING, ePinLevel_High},
    //eGpioName_KeyInput3,
    {PORT_TM1623_KVAL_1,        KS3_K1,     GPIO_Mode_IN_FLOATING, ePinLevel_High},
    //eGpioName_KeyInput4,
    {PORT_TM1623_KVAL_1,        KS4_K1,     GPIO_Mode_IN_FLOATING, ePinLevel_High},
    //eGpioName_KeyAuto,
    {PORT_TM1623_KVAL_2,        KS5_K1,     GPIO_Mode_IN_FLOATING, ePinLevel_High},
    //eGpioName_KeyWinA,
    {PORT_TM1623_KVAL_2,        KS6_K1,     GPIO_Mode_IN_FLOATING, ePinLevel_High},
    //eGpioName_KeyWinB,
    {PORT_TM1623_KVAL_3,        KS7_K1,     GPIO_Mode_IN_FLOATING, ePinLevel_High},
    //eGpioName_KeyWinC,
    {PORT_TM1623_KVAL_3,        KS8_K1,     GPIO_Mode_IN_FLOATING, ePinLevel_High},
    //eGpioName_KeyWinD,
    {PORT_TM1623_KVAL_0,        KS1_K2,     GPIO_Mode_IN_FLOATING, ePinLevel_High},
    //eGpioName_KeySwap,
    {PORT_TM1623_KVAL_0,        KS2_K2,     GPIO_Mode_IN_FLOATING, ePinLevel_High},
    //eGpioName_KeyResize,
    {PORT_TM1623_KVAL_1,        KS3_K2,     GPIO_Mode_IN_FLOATING, ePinLevel_High},
    //eGpioName_KeyMVMode1,
    {PORT_TM1623_KVAL_1,        KS4_K2,     GPIO_Mode_IN_FLOATING, ePinLevel_High},
    //eGpioName_KeyMVMode2,
    {PORT_TM1623_KVAL_2,        KS5_K2,     GPIO_Mode_IN_FLOATING, ePinLevel_High},
    //eGpioName_KeyMVMode3,
    {PORT_TM1623_KVAL_2,        KS6_K2,     GPIO_Mode_IN_FLOATING, ePinLevel_High},

    // led
    //eGpioName_LedPowerG,
	{PORT_GPIOC,          GPIO_Pin_14,  GPIO_Mode_Out_PP, ePinLevel_High},
    //eGpioName_LedPowerR,
    {PORT_GPIOC,          GPIO_Pin_15,  GPIO_Mode_Out_PP,  ePinLevel_Low},
    //eGpioName_LedInput1Y,
    {PORT_TM1623_GRID1_L,        SEG1,  GPIO_Mode_Out_PP, ePinLevel_High},
    //eGpioName_LedInput1B,
    {PORT_TM1623_GRID2_L,        SEG1,  GPIO_Mode_Out_PP, ePinLevel_High},
    //eGpioName_LedInput2Y,
    {PORT_TM1623_GRID1_L,        SEG2,  GPIO_Mode_Out_PP, ePinLevel_High},
    //eGpioName_LedInput2B,
    {PORT_TM1623_GRID2_L,        SEG2,  GPIO_Mode_Out_PP, ePinLevel_High},
    //eGpioName_LedInput3Y,
    {PORT_TM1623_GRID1_L,        SEG3,  GPIO_Mode_Out_PP, ePinLevel_High},
    //eGpioName_LedInput3B,
    {PORT_TM1623_GRID2_L,        SEG3,  GPIO_Mode_Out_PP, ePinLevel_High},
    //eGpioName_LedInput4Y,
    {PORT_TM1623_GRID1_L,        SEG4,  GPIO_Mode_Out_PP, ePinLevel_High},
    //eGpioName_LedInput4B,
    {PORT_TM1623_GRID2_L,        SEG4,  GPIO_Mode_Out_PP, ePinLevel_High},
    //eGpioName_LedAutoSwitch,
    {PORT_TM1623_GRID1_L,        SEG5,  GPIO_Mode_Out_PP, ePinLevel_High},
    //eGpioName_LedWindowA,
    {PORT_TM1623_GRID1_L,        SEG6,  GPIO_Mode_Out_PP, ePinLevel_High},
    //eGpioName_LedWindowB,
    {PORT_TM1623_GRID1_L,        SEG7,  GPIO_Mode_Out_PP, ePinLevel_High},
    //eGpioName_LedWindowC,
    {PORT_TM1623_GRID1_L,        SEG8,  GPIO_Mode_Out_PP, ePinLevel_High},
    //eGpioName_LedWindowD,
    {PORT_TM1623_GRID3_L,        SEG1,  GPIO_Mode_Out_PP, ePinLevel_High},
    //eGpioName_LedSwap,
    {PORT_TM1623_GRID3_L,        SEG2,  GPIO_Mode_Out_PP, ePinLevel_High},
    //eGpioName_LedResize,
    {PORT_TM1623_GRID3_L,        SEG3,  GPIO_Mode_Out_PP, ePinLevel_High},
    //eGpioName_LedMV2Win,
    {PORT_TM1623_GRID3_L,        SEG4,  GPIO_Mode_Out_PP, ePinLevel_High},
    //eGpioName_LedMV4Win1,
    {PORT_TM1623_GRID3_L,        SEG5,  GPIO_Mode_Out_PP, ePinLevel_High},
    //eGpioName_LedMV4Win2,
    {PORT_TM1623_GRID3_L,        SEG6,  GPIO_Mode_Out_PP, ePinLevel_High},

    // cec
    //eGpioName_CecOutput,
    {PORT_GPIOB,        GPIO_Pin_0, GPIO_Mode_Out_OD, ePinLevel_High},
    //eGpioName_CecInput1,
    {PORT_GPIOC,        GPIO_Pin_5, GPIO_Mode_Out_OD, ePinLevel_High},
    //eGpioName_CecInput2,
    {PORT_GPIOC,        GPIO_Pin_4, GPIO_Mode_Out_OD, ePinLevel_High},
    //eGpioName_CecInput3,
    {PORT_GPIOA,        GPIO_Pin_7, GPIO_Mode_Out_OD, ePinLevel_High},
    //eGpioName_CecInput4,
    {PORT_GPIOA,        GPIO_Pin_6, GPIO_Mode_Out_OD, ePinLevel_High},

    // 4052
    //eGpioName_Sw4052_It66321A,
    {PORT_GPIOA,        GPIO_Pin_5, GPIO_Mode_Out_OD, ePinLevel_High},
    //eGpioName_Sw4052_It66321B,
    {PORT_GPIOA,        GPIO_Pin_4, GPIO_Mode_Out_OD, ePinLevel_High},
    //eGpioName_Sw4052_It66021A,
    {PORT_GPIOC,        GPIO_Pin_3, GPIO_Mode_Out_OD, ePinLevel_High},
    //eGpioName_Sw4052_It66021B,
    {PORT_GPIOC,        GPIO_Pin_2, GPIO_Mode_Out_OD, ePinLevel_High},

    // SPI Switcher
    //eGpioName_SwQS3L384A,
    {PORT_GPIOC,        GPIO_Pin_6, GPIO_Mode_Out_PP, ePinLevel_High},
    //eGpioName_SwQS3L384B,
    {PORT_GPIOC,        GPIO_Pin_7, GPIO_Mode_Out_PP, ePinLevel_High},

    // I2C
    //eGpioName_I2cIt66321Scl,
    {PORT_GPIOC,        GPIO_Pin_1, GPIO_Mode_Out_OD, ePinLevel_High},
    //eGpioName_I2cIt66321Sda,
    {PORT_GPIOC,        GPIO_Pin_0, GPIO_Mode_Out_OD, ePinLevel_High},
    //eGpioName_I2cIt66021Scl,
    {PORT_GPIOA,        GPIO_Pin_1, GPIO_Mode_Out_OD, ePinLevel_High},
    //eGpioName_I2cIt66021Sda,
    {PORT_GPIOA,        GPIO_Pin_0, GPIO_Mode_Out_OD, ePinLevel_High},
    //eGpioName_I2cIt6615Scl,
    {PORT_GPIOB,        GPIO_Pin_4, GPIO_Mode_Out_OD, ePinLevel_High},
    //eGpioName_I2cIt6615Sda,
    {PORT_GPIOB,        GPIO_Pin_5, GPIO_Mode_Out_OD, ePinLevel_High},
    //eGpioName_I2cEepromScl,
    {PORT_GPIOB,        GPIO_Pin_6, GPIO_Mode_Out_OD, ePinLevel_High},
    //eGpioName_I2cEepromSda,
    {PORT_GPIOB,        GPIO_Pin_7, GPIO_Mode_Out_OD, ePinLevel_High},
    //eGpioName_I2cIdt5v49ee902Scl,
    {PORT_GPIOC,        GPIO_Pin_12,GPIO_Mode_Out_OD, ePinLevel_High},
    //eGpioName_I2cIdt5v49ee902Sda,
    {PORT_GPIOB,        GPIO_Pin_3, GPIO_Mode_Out_OD, ePinLevel_High},

    // SPI
    //eGpioName_Tm1623Cs,
    {PORT_GPIOB,        GPIO_Pin_14, GPIO_Mode_Out_OD, ePinLevel_High},
    //eGpioName_Tm1623Sclk,
    {PORT_GPIOB,        GPIO_Pin_13, GPIO_Mode_Out_OD, ePinLevel_High},
    //eGpioName_Tm1623Sda,
    {PORT_GPIOB,        GPIO_Pin_12, GPIO_Mode_Out_OD, ePinLevel_High},
    //eGpioName_FpgaFlashCs,
    {PORT_GPIOC,        GPIO_Pin_8, GPIO_Mode_Out_OD, ePinLevel_High},
    //eGpioName_FpgaFlashSclk,
    {PORT_GPIOC,        GPIO_Pin_11, GPIO_Mode_Out_OD, ePinLevel_High},
    //eGpioName_FpgaFlashMiso,
    {PORT_GPIOC,        GPIO_Pin_9, GPIO_Mode_IPU, ePinLevel_High},
    //eGpioName_FpgaFlashMosi,
    {PORT_GPIOC,        GPIO_Pin_10, GPIO_Mode_Out_OD, ePinLevel_High},

    // Ir
    //eGpioName_IrIn,
    {PORT_GPIOB,        GPIO_Pin_9, GPIO_Mode_IPD, ePinLevel_High},

    // IDT
    //eGpioName_IDTSclk,
    //{PORT_GPIOC,        GPIO_Pin_12, GPIO_Mode_Out_OD, ePinLevel_High},
    //eGpioName_IDTSda,
    //{PORT_GPIOB,        GPIO_Pin_3, GPIO_Mode_Out_OD, ePinLevel_High},

    // Fan
    //eGpioName_CtrlFan,
    {PORT_GPIOB,        GPIO_Pin_8, GPIO_Mode_Out_PP, ePinLevel_High},

    // SysReset
    //eGpioName_SysReset,
    {PORT_GPIOB,        GPIO_Pin_15, GPIO_Mode_Out_PP, ePinLevel_High},

    // Output 5V
    //eGpioName_CtrlOutput5V,
    {PORT_GPIOD,        GPIO_Pin_2, GPIO_Mode_Out_PP, ePinLevel_High},
    // Output HPD
    //eGpioName_OutputHPDPinDetect,
    {PORT_GPIOC,        GPIO_Pin_13, GPIO_Mode_IPD, ePinLevel_High},

    // UsbDetect
    //eGpioName_UsbDetect,
    {PORT_GPIOA,        GPIO_Pin_15, GPIO_Mode_IPD, ePinLevel_High},

};

#if _ENABLE_NEW_USER_KEY_DEFINE_FUNCTION
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
#else
const stKeyDeclare_t stProjectKeyTable[] =
{
    #if 0
    //key panel input 1    /**0**/
    {{(pstGpioConfig_t)&stProjectGpioTable[eGpioName_KeyInput1], \
    NULL, \
    NULL, \
    NULL}, \
    key_detect_mode_after, \
    KEY_LONG_TIME, \
    {KEY_REPEAT_INVALID, KEY_REPEAT_START_DELAY_NONE}, \
    {{{NULL}, KEY_FILTER_TIME}}, \
    key_response_panel_input_1, \
    IR_REMOTE_KEY_INVALID, \
    KeyPressCallBack},

    //eGpioName_KeyDisplayOn,
    {{(pstGpioConfig_t)&stProjectGpioTable[eGpioName_KeyDisplayOn], \
    NULL, \
    NULL, \
    NULL}, \
    key_detect_mode_after, \
    KEY_LONG_INVALID, \
    {KEY_REPEAT_INVALID, KEY_REPEAT_START_DELAY_NONE}, \
    {{{NULL}, KEY_FILTER_TIME}}, \
    key_response_panel_dis_on, \
    IR_REMOTE_KEY_INVALID, \
    KeyPressCallBack},

    //eGpioName_KeyDisplayOFF,
    {{(pstGpioConfig_t)&stProjectGpioTable[eGpioName_KeyDisplayOFF], \
    NULL, \
    NULL, \
    NULL}, \
    key_detect_mode_after, \
    KEY_LONG_INVALID, \
    {KEY_REPEAT_INVALID, KEY_REPEAT_START_DELAY_NONE}, \
    {{{NULL}, KEY_FILTER_TIME}}, \
    key_response_panel_dis_off, \
    IR_REMOTE_KEY_INVALID, \
    KeyPressCallBack},

    //key edid dip(4 pins)    /**13**/
    {{(pstGpioConfig_t)&stProjectGpioTable[eGpioName_KeyEdidDip0], \
    (pstGpioConfig_t)&stProjectGpioTable[eGpioName_KeyEdidDip1], \
    (pstGpioConfig_t)&stProjectGpioTable[eGpioName_KeyEdidDip2], \
    (pstGpioConfig_t)&stProjectGpioTable[eGpioName_KeyEdidDip3]}, \
    key_detect_mode_pre, \
    KEY_LONG_INVALID, \
    {KEY_REPEAT_INVALID, KEY_REPEAT_START_DELAY_NONE}, \
    {{{NULL}, KEY_FILTER_TIME}}, \
    key_response_edids, \
    IR_REMOTE_KEY_INVALID, \
    KeyPressCallBack},
    #endif

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
#endif

const stLedDeclare_t stProjectLedTable[] =
{

    //led_name_power_red = 0,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_LedPowerR],             ePinLevel_High},
    //led_name_power_Green,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_LedPowerG],             ePinLevel_High},
    //led_name_hdmi_input_yellow_1,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_LedInput1Y],            ePinLevel_High},
    //led_name_hdmi_input_blue_1,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_LedInput1B],            ePinLevel_High},
    //led_name_hdmi_input_yellow_2,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_LedInput2Y],            ePinLevel_High},
    //led_name_hdmi_input_blue_2,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_LedInput2B],            ePinLevel_High},
    //led_name_hdmi_input_yellow_3,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_LedInput3Y],            ePinLevel_High},
    //led_name_hdmi_input_blue_3,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_LedInput3B],            ePinLevel_High},
    //led_name_hdmi_input_yellow_4,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_LedInput4Y],            ePinLevel_High},
    //led_name_hdmi_input_blue_4,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_LedInput4B],            ePinLevel_High},
    //led_name_switch_auto,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_LedAutoSwitch],         ePinLevel_High},
    //led_name_window_A,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_LedWindowA],            ePinLevel_High},
    //led_name_window_B,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_LedWindowB],            ePinLevel_High},
    //led_name_window_C,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_LedWindowC],            ePinLevel_High},
    //led_name_window_D,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_LedWindowD],            ePinLevel_High},
    //led_name_swap,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_LedSwap],               ePinLevel_High},
    //led_name_resize,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_LedResize],             ePinLevel_High},
    //led_name_multiview_mode_1,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_LedMV2Win],             ePinLevel_High},
    //led_name_multiview_mode_2,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_LedMV4Win1],            ePinLevel_High},
    //led_name_multiview_mode_3,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_LedMV4Win2],            ePinLevel_High},// 0

    //led_name_End
    {(pstGpioConfig_t)NULL,                                                 ePinLevel_Low},// the last one
};

const stI2cDeclare_t stProjectI2cTable[] =
{

    // eI2cDevice_Eeprom = 0,
    {   (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cEepromScl],   // pstSclPort
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cEepromSda],   // pstSdaPort
        DELAY_TIME_FOR_EEPROM,                                   // u16I2cDelayT
        eSw4052Device_Max,                                              // u8SwitchPortDev
        0,                                                              // u8SwitchPortNum
        },

    // eI2cDevice_It66321_1,
    {   (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cIt66321Scl],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cIt66321Sda],
        DELAY_TIME_FOR_ITE663XX,
        eSw4052Device_It66321,
        0,
        },

    //eI2cDevice_It66321_2,
    {   (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cIt66321Scl],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cIt66321Sda],
        DELAY_TIME_FOR_ITE663XX,
        eSw4052Device_It66321,
        1,
        },

    //eI2cDevice_It66321_3,
    {   (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cIt66321Scl],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cIt66321Sda],
        DELAY_TIME_FOR_ITE663XX,
        eSw4052Device_It66321,
        2,
        },

    //eI2cDevice_It66321_4,
    {   (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cIt66321Scl],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cIt66321Sda],
        DELAY_TIME_FOR_ITE663XX,
        eSw4052Device_It66321,
        3,
        },

    //eI2cDevice_It66021_1,
    {   (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cIt66021Scl],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cIt66021Sda],
        DELAY_TIME_FOR_ITE66021,
        eSw4052Device_It66021,
        0,
        },

    //eI2cDevice_It66021_2,
    {   (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cIt66021Scl],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cIt66021Sda],
        DELAY_TIME_FOR_ITE66021,
        eSw4052Device_It66021,
        1,
        },

    //eI2cDevice_It66021_3,
    {   (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cIt66021Scl],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cIt66021Sda],
        DELAY_TIME_FOR_ITE66021,
        eSw4052Device_It66021,
        2,
        },

    //eI2cDevice_It66021_4,
    {   (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cIt66021Scl],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cIt66021Sda],
        DELAY_TIME_FOR_ITE66021,
        eSw4052Device_It66021,
        3,
        },

    //eI2cDevice_It6615,
    {   (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cIt6615Scl],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cIt6615Sda],
        DELAY_TIME_FOR_ITE6615,
        eSw4052Device_Max,
        0,
        },

    //eI2cDevice_ItIdt5v49ee902
    {   (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cIdt5v49ee902Scl],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cIdt5v49ee902Sda],
        DELAY_TIME_FOR_ITE6615,
        eSw4052Device_Max,
        0,
        },

    //eI2cDevice_Max i2c null
    {   NULL,                                                           // pstSclPort
        NULL,                                                           // pstSdaPort
        0,                                                              // u16I2cDelayT
        0,                                                              // u8SwitchPortDev
        0,                                                              // u8SwitchPortNum
        },
};

const stI2CSwitchCtrl_t stProjectI2cSwitchTable[] =
{
    // eSw4052Device_It66321 4052 to switch 66321 isc
    {   (pstGpioConfig_t)&stProjectGpioTable[eGpioName_Sw4052_It66321A],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_Sw4052_It66321B]
    },

    // eSw4052Device_It66021 4052 to switch 66021 isc
    {   (pstGpioConfig_t)&stProjectGpioTable[eGpioName_Sw4052_It66021A],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_Sw4052_It66021B]
    },

    // eSw4052Device_Max switch null
    {   NULL,
        NULL
    },
};

const stSpiDeclare_t stProjectSpiTable[] =
{
	// tm1623
	{   (pstGpioConfig_t)&stProjectGpioTable[eGpioName_Tm1623Sda],
        NULL,
	    (pstGpioConfig_t)&stProjectGpioTable[eGpioName_Tm1623Sclk],
	    (pstGpioConfig_t)&stProjectGpioTable[eGpioName_Tm1623Cs],
	    DELAY_TIME_FOR_TM1623_BYTE,
	    DELAY_TIME_FOR_TM1623_CLK,
        0,
        },

    // FPGA Flash
    {   (pstGpioConfig_t)&stProjectGpioTable[eGpioName_FpgaFlashMosi],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_FpgaFlashMiso],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_FpgaFlashSclk],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_FpgaFlashCs],
        DELAY_TIME_FOR_FPGA_FLASH_BYTE,
        DELAY_TIME_FOR_FPGA_FLASH_CLK,
        0,
        },

    //spi null
    {   NULL,                           // pstSpiMosi;
        NULL,                           // pstSpiMiso;
        NULL,                           // pstSpiClk;
        NULL,                           // pstSpiCs;
        DELAY_TIME_FOR_TM1623_BYTE,     // u16SpiDelayByte;
        DELAY_TIME_FOR_TM1623_CLK       // u16SpiDelayClk;
        },
};

const stRxHpdDetDeclare_t stProjectRxHpdDetTable[] =
{
    //hdmi input1 hpd
    //{(pstGpioConfig_t)&stProjectGpioTable[eGpioName_HdmiDetect1]},

    //hdmi input2 hpd
    //{(pstGpioConfig_t)&stProjectGpioTable[eGpioName_HdmiDetect2]},

    //hdmi input3 hpd
    //{(pstGpioConfig_t)&stProjectGpioTable[eGpioName_HdmiDetect3]},

    //hdmi input4 hpd
    //{(pstGpioConfig_t)&stProjectGpioTable[eGpioName_HdmiDetect4]},

    //hdmi input hpd null
    {NULL},
};

const stIT663xxRstDeclare_t stProjectIT663xxRstTable[] =
{
    //{(pstGpioConfig_t)&stProjectGpioTable[eGpioName_SysReset],    ePinLevel_Low},

    {NULL,                                                      ePinLevel_High},
};

const stIrWorkDeclare_t stProjectIrWorkTable =
{
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_IrIn], eIrRmProtocol_NEC},
    //{NULL, eIrRmProtocol_None},
    NULL,
};

const st5VControl_t stProjectTx5VControlTable[] =
{
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_CtrlOutput5V]},
    //hdmi output 5v null
    {NULL},
};

const stUartDeclare_t stProjectUartWorkTable[] =
{
    //uart port 1--GUI
    {   _UART_PORT_1,
        {9600, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No, (USART_Mode_Rx|USART_Mode_Tx), USART_HardwareFlowControl_None}, \
        FALSE,
        Uart1ProtocalDataReceive,
        Uart1ParserMethod,
        Uart1VarInit,
        Uart1VarTimeOut},

    //uart port 2--RS232
    {   _UART_PORT_2,
        {UART2_BAUDRATE, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No, (USART_Mode_Rx|USART_Mode_Tx), USART_HardwareFlowControl_None},
        TRUE,
        UartProtocalDataReceive,
        UartParserMethod,
        UartVarInit,
        UartVarTimeout},

    //uart port 3--FPGA
    {   _UART_PORT_3,
        {115200, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No, (USART_Mode_Rx|USART_Mode_Tx), USART_HardwareFlowControl_None},
        FALSE,
        Uart3ProtocalDataReceive,
        Uart3ParserMethod,
        Uart3VarInit,
        Uart3VarTimeOut},

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
    //cec hdmi input 1
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_CecInput1],     {eCecSrcDisType_Src, FALSE, 0, 0x00}},
    //cec hdmi input 2
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_CecInput2],     {eCecSrcDisType_Src, FALSE, 1, 0x00}},
    //cec hdmi input 3
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_CecInput3],     {eCecSrcDisType_Src, FALSE, 2, 0x00}},
    //cec hdmi input 4
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_CecInput4],     {eCecSrcDisType_Src, FALSE, 3, 0x00}},
    //cec hdmi output
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_CecOutput],     {eCecSrcDisType_Dis, FALSE, 0, 0x04}},

    //cec none
    {NULL,                                                          {eCecSrcDisType_Src, FALSE, 0, 0x00}}
};

#if _ENABLE_NEW_USER_KEY_DEFINE_FUNCTION
const stNewKeyDeclare_t stProjectNewKeyTable[] =
{
    //emKeyName_Input1 = 0,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_KeyInput1],     0,  BIT0,   ePinLevel_High,  FALSE},// 0
    //emKeyName_Input2,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_KeyInput2],     0,  BIT1,   ePinLevel_High,  FALSE},// 0
    //emKeyName_Input3,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_KeyInput3],     0,  BIT2,   ePinLevel_High,  FALSE},// 0
    //emKeyName_Input4,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_KeyInput4],     0,  BIT3,   ePinLevel_High,  FALSE},// 0
    //emKeyName_Auto,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_KeyAuto],       0,  BIT4,   ePinLevel_High,  FALSE},// 0
    //emKeyName_WinA,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_KeyWinA],       0,  BIT5,   ePinLevel_High,  FALSE},// 0
    //emKeyName_WinB,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_KeyWinB],       0,  BIT6,   ePinLevel_High,  FALSE},// 0
    //emKeyName_WinC,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_KeyWinC],       0,  BIT7,   ePinLevel_High,  FALSE},// 0
    //emKeyName_WinD,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_KeyWinD],       0,  BIT8,   ePinLevel_High,  FALSE},// 0
    //emKeyName_Swap,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_KeySwap],       0,  BIT9,   ePinLevel_High,  FALSE},// 0
    //emKeyName_Resize,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_KeyResize],     0,  BIT10,  ePinLevel_High,  FALSE},// 0
    //emKeyName_MVMode1,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_KeyMVMode1],    0,  BIT11,  ePinLevel_High,  FALSE},// 0
    //emKeyName_MVMode2,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_KeyMVMode2],    0,  BIT12,  ePinLevel_High,  FALSE},// 0
    //emKeyName_MVMode3,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_KeyMVMode3],    0,  BIT13,  ePinLevel_High,  FALSE},// 0

    //emKeyName_max,
    {(pstGpioConfig_t)NULL,                                         0,  BIT0,   ePinLevel_Low,  FALSE},// the last one
};

#endif

bool mapp_UsbInputDetect(void)
{
    u8 l_u8PortType = stProjectGpioTable[eGpioName_UsbDetect].u8PortType;
    u16 l_u16GpioPin = stProjectGpioTable[eGpioName_UsbDetect].u16GpioPin;

    if(STM32_GPIO_PORT[l_u8PortType-PORT_GPIOA]->IDR & l_u16GpioPin)
        return TRUE;//FALSE;
    return FALSE;//TRUE;
}

void mapp_SetSysResetPin(u8 u8Mode)
{
    u8 l_u8EnPort = stProjectGpioTable[eGpioName_SysReset].u8PortType;
    u16 l_u16EnPin = stProjectGpioTable[eGpioName_SysReset].u16GpioPin;
    if(u8Mode)
    {
        GPIO_WriteBit(STM32_GPIO_PORT[l_u8EnPort-PORT_GPIOA],l_u16EnPin,Bit_RESET);
    }
    else
    {
        GPIO_WriteBit(STM32_GPIO_PORT[l_u8EnPort-PORT_GPIOA],l_u16EnPin,Bit_SET);
    }
}

void mapp_SetFpgaProgramPin(u8 u8Mode)
{
    u8 l_u8EnPort = stProjectGpioTable[eGpioName_CtrlFan].u8PortType;
    u16 l_u16EnPin = stProjectGpioTable[eGpioName_CtrlFan].u16GpioPin;
    if(u8Mode)
    {
        GPIO_WriteBit(STM32_GPIO_PORT[l_u8EnPort-PORT_GPIOA],l_u16EnPin,Bit_SET);
    }
    else
    {
        GPIO_WriteBit(STM32_GPIO_PORT[l_u8EnPort-PORT_GPIOA],l_u16EnPin,Bit_RESET);
    }
}

void mapp_SelectFpgaFlashUpdateMode(bool bEnable)
{
    u8 l_u8QS3L384APort = stProjectGpioTable[eGpioName_SwQS3L384A].u8PortType;
    u16 l_u16QS3L384APin = stProjectGpioTable[eGpioName_SwQS3L384A].u16GpioPin;
    u8 l_u8QS3L384BPort = stProjectGpioTable[eGpioName_SwQS3L384B].u8PortType;
    u16 l_u16QS3L384BPin = stProjectGpioTable[eGpioName_SwQS3L384B].u16GpioPin;

    if(bEnable)
    {
        GPIO_WriteBit(STM32_GPIO_PORT[l_u8QS3L384APort-PORT_GPIOA],l_u16QS3L384APin,Bit_SET);
        GPIO_WriteBit(STM32_GPIO_PORT[l_u8QS3L384BPort-PORT_GPIOA],l_u16QS3L384BPin,Bit_RESET);
    }
    else
    {
        GPIO_WriteBit(STM32_GPIO_PORT[l_u8QS3L384APort-PORT_GPIOA],l_u16QS3L384APin,Bit_RESET);
        GPIO_WriteBit(STM32_GPIO_PORT[l_u8QS3L384BPort-PORT_GPIOA],l_u16QS3L384BPin,Bit_SET);
    }
}

bool mapp_GetOutHPDState(void)
{
    u8 l_u8PortType = stProjectGpioTable[eGpioName_OutputHPDPinDetect].u8PortType;
    u16 l_u16GpioPin = stProjectGpioTable[eGpioName_OutputHPDPinDetect].u16GpioPin;

    if(STM32_GPIO_PORT[l_u8PortType-PORT_GPIOA]->IDR & l_u16GpioPin)
        return TRUE;
    return FALSE;
}

void mapp_SetStartupLedStateInit(void)
{
    u8 l_u8PortType = stProjectGpioTable[eGpioName_LedPowerG].u8PortType;
    u16 l_u16GpioPin = stProjectGpioTable[eGpioName_LedPowerG].u16GpioPin;

    TM1623_SegLed_Write((l_u8PortType-PORT_TM1623_GRID1_L),l_u16GpioPin);
}

