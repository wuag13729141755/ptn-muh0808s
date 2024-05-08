#ifndef __PROJECT_MACRO_DEFINE_MANAGEMENT_SCU61E_H_
#define __PROJECT_MACRO_DEFINE_MANAGEMENT_SCU61E_H_

/******************************************************************/
/**CHIPS*USE****/
/******************************************************************/
#define IT666X_VER                              IT666X_NONE
#define IT663XX_VER                             IT663XX_VER_1_30    //IT663XX_VER_1_21//

//use eeprom chip
#define _CHIP_EEPROM_TYPE_USE                           _EEPROM_CHIP_24C32

/////////////////////////////Chip Define/////////////////////////////////
#define _ENABLE_CHIP_ITE6615                    _MACRO_ENABLE
#define _ENABLE_CHIP_ITE66021                   _MACRO_ENABLE
#define _EN_CHIP_TM1623_USE                     _MACRO_ENABLE
#define _ENABLE_CHIP_IDT5V49EE902               _MACRO_ENABLE

#define IT66021_DEV_NUM             4

//=========================I2C INDEX====================================
//i2c index of table for eeprom
#define _I2C_TABLE_INDEX_EEPROM                         eI2cDevice_Eeprom

//i2c index of table for IT663xx - root
#define _I2C_TABLE_INDEX_IT663XX_0                      eI2cDevice_It66321_1
#define _I2C_TABLE_INDEX_IT663XX_1                      eI2cDevice_It66321_2
#define _I2C_TABLE_INDEX_IT663XX_2                      eI2cDevice_It66321_3
#define _I2C_TABLE_INDEX_IT663XX_3                      eI2cDevice_It66321_4

// it66021
#define _I2C_TABLE_INDEX_IT660XX_0                      eI2cDevice_It66021_1
#define _I2C_TABLE_INDEX_IT660XX_1                      eI2cDevice_It66021_2
#define _I2C_TABLE_INDEX_IT660XX_2                      eI2cDevice_It66021_3
#define _I2C_TABLE_INDEX_IT660XX_3                      eI2cDevice_It66021_4

// it6615
#define _I2C_TABLE_INDEX_IT6615_0                       eI2cDevice_It6615

#define _I2C_TABLE_INDEX_IDT5V49_0                      eI2cDevice_Idt5v49ee902

//=========================SPI INDEX====================================
#define _SPI_TABLE_INDEX_TM1623                         eSpiDevice_Tm1623

//===========================END========================================

//spi index of table for TM1623
//#define _SPI_TABLE_INDEX_TM1623                           0

// spi index of table for 74hc595
//#define _SPI_TABLE_INDEX_74HC595                          0

//ir function
#define _IR_FUNCTION_USE                                _IR_FUNCTION_REMOTE

//it663xx rx mhl function
//#define _EN_IT663XX_RX_MHL_FUNCTION                   _MACRO_ENABLE


//type-c port input
//#define _HDMI_INPUT_TYPE_C_PORT                       _HDMI_INPUT_4_BIT_INDEX

//DP port input
//#define _HDMI_INPUT_DP_PORT                           _HDMI_INPUT_5_BIT_INDEX

//snow display
#define Project_663xx_MANY_CHIPS_SIGNAL_REBOOT          _MACRO_ENABLE

/******************************************************************/
/**BOARD*GPIO****/
/******************************************************************/
//use gpio usb detect
//#define _EN_GPIO_USB_DETECT                           _MACRO_ENABLE

/***RCC*OF*GPIO***/
//ALL THE RCC OF GPIO
#ifndef _RCC_APB2Periph_GPIO
#define _RCC_APB2Periph_GPIO                            RCC_APB2Periph_GPIOA |\
                                                        RCC_APB2Periph_GPIOB |\
                                                        RCC_APB2Periph_GPIOC |\
                                                        RCC_APB2Periph_GPIOD |\
                                                        RCC_APB2Periph_AFIO
#endif

#define FW_VECT_OFFSET                                  0x5000

#define _NUM_SIMUL_GPIO_PINS_ONE_KEY_MAX                4//the min val is 1

/******************************************************************/
/**BOARD*FUNCTION****/
/******************************************************************/
//save hdmi sel (auto & manu) mode input channel
#define _EN_EEPROM_FUN_SAVE_HDMI_SEL            _MACRO_ENABLE

//save hdmi auto & manu switch mode
#define _EN_EEPROM_FUN_SAVE_TYPE_SWITCH_MODE    _MACRO_ENABLE

//save uart study keys' value
#define _EN_EEPROM_FUN_SAVE_UART_STUDY_KEYS     _MACRO_ENABLE

//save hdmi hdcp on/off status
#define _EN_EEPROM_FUN_SAVE_HDCP_STATUS         _MACRO_ENABLE


/***WATCH*DOG***/
//enable watch dog
//#define _EN_WATCH_DOG                         _MACRO_ENABLE

/******************************************************************/
/***TIME*OF*SYSTEM*RUN***/
/******************************************************************/
//led normal blink toggle time
#define _TIME_INDICATIVE_LED_NORM_BLINK_TOGGLE  25//unit = 20ms, depends on the delay time of task_led

//led fast blink toggle time
#define _TIME_INDICATIVE_LED_FAST_BLINK_TOGGLE  10//unit = 20ms

//led slow blink toggle time
#define _TIME_INDICATIVE_LED_SLOW_BLINK_TOGGLE  50//unit = 20ms

//led moment on time
#define _TIME_INDICATIVE_LED_MOMENT_ON          50//unit = 20ms

//led moment off time
#define _TIME_INDICATIVE_LED_MOMENT_OFF         50//unit = 20ms

//led moment toggle time
#define _TIME_INDICATIVE_LED_MOMENT_TOGGLE      10//unit = 20ms

//auto switch detect time
#define _TIME_AUTO_SWITCH_DETECT                500//uint is 1ms

/******************************************************************/
/***GENERAL*CONFIGURATION***/
/******************************************************************/
/***HDCP*METHOD***/
//HDCP MODE PROJECT ACTIVE
#define _PRJ_TX_HDCP_MODE_ACTIVE                /*tx_hdcp_force_off*/  \
                                                /*tx_hdcp_force_1_4*/  \
                                                /*tx_hdcp_force_2_2*/  \
                                                tx_hdcp_force_bypass

//HDCP BYPASS MODE MAX_VERSION=1.4(do not effect when _PRJ_TX_HDCP_MODE_ACTIVE != tx_hdcp_force_bypass)
#define _PRJ_TX_HDCP_BYPASS_MAX_1_4             _MACRO_DISABLE

/***THE*CHIP*SYSTEM*USE***/
//PROJECT USE SWITCHER(IT66341/IT66321/IT6634) & SPLITER(IT6663/IT6664)
#define _PRJ_TYPE_SWITCH_SPLITER                /*_PRJ_SWITCHER_AND_SPLITER*/ \
                                                _PRJ_SWITCHER_ONLY    \
                                                /*_PRJ_SPLITER_ONLY*/

//SWITCHER TYPE (do not need to set when _PRJ_TYPE_SWITCH_SPLITER==_PRJ_SPLITER_ONLY)
#define _SWITCHER_TYPE_IN_OUT                   /*_SWITCHER_TYPE_4_TO_1*/ \
                                                /*_SWITCHER_TYPE_2_TO_1*/ \
                                                /*_SWITCHER_TYPE_4_TO_3*/ \
                                                /*_SWITCHER_TYPE_6_TO_1*/ \
                                                _SWITCHER_TYPE_4_TO_1_SEAMLESS

//DETECT TYPE WHEN AUTO SWITCH (do not need to set when _PRJ_TYPE_SWITCH_SPLITER==_PRJ_SPLITER_ONLY)
#define _DETECT_TYPE_AUTO_SWITCH                _AUTO_SWITCH_DETECT_TYPE_HPD_AND_TMDS

/***hdmi*input*port*logic*index***/
//boot rx port for ite device 0
#define _HDMI_INPUT_NODE_RX_ITE_DEV_0           _HDMI_INPUT_PHYSIC_INDEX_INVALID

//boot rx port for ite device 1
#define _HDMI_INPUT_NODE_RX_ITE_DEV_1           _HDMI_INPUT_PHYSIC_INDEX_INVALID

//hdmi input logic index
#define _HDMI_INPUT_1_LOGIC_INDEX               (_HDMI_INPUT_1_PHYSIC_INDEX)
#define _HDMI_INPUT_2_LOGIC_INDEX               (_HDMI_INPUT_2_PHYSIC_INDEX)//(_HDMI_INPUT_1_PHYSIC_INDEX<<_HDMI_INPUT_PHYSIC_SHIFT)|(_HDMI_INPUT_NODE_RX_ITE_DEV_0)
#define _HDMI_INPUT_3_LOGIC_INDEX               (_HDMI_INPUT_3_PHYSIC_INDEX)//(_HDMI_INPUT_3_PHYSIC_INDEX<<_HDMI_INPUT_PHYSIC_SHIFT)|(_HDMI_INPUT_NODE_RX_ITE_DEV_0)
#define _HDMI_INPUT_4_LOGIC_INDEX               (_HDMI_INPUT_4_PHYSIC_INDEX)//(_HDMI_INPUT_4_PHYSIC_INDEX<<_HDMI_INPUT_PHYSIC_SHIFT)|(_HDMI_INPUT_NODE_RX_ITE_DEV_0)

#define _HDMI_INPUT_1_RX_PORT                   (IN_RX0)
#define _HDMI_INPUT_2_RX_PORT                   (IN_RX0)
#define _HDMI_INPUT_3_RX_PORT                   (IN_RX0)
#define _HDMI_INPUT_4_RX_PORT                   (IN_RX0)

#define _HDMI_INPUT_1_DEV                       (0)
#define _HDMI_INPUT_2_DEV                       (1)
#define _HDMI_INPUT_3_DEV                       (2)
#define _HDMI_INPUT_4_DEV                       (3)

//hdmi input port used number
#define _HDMI_INPUT_PORT_USED_NUMBER            4

//SCU61E HDMI AUTO SWITCH
#define Project_SCU61E_66341_AUTO_SWITCH        _MACRO_ENABLE

//MANUFACTURER LOGO
#define _STRING_MANF_NAME                       "\r\n"
#define _STRING_FACTORY_MODEL                   "SCU41-MV\r\n"
#define _STRING_FW_VERSION                      "VERSION"
#define _STRING_FW_VER_NUM                      "V1.0.4\r\n"    // 1.0.4客户测试用,未发布
#define _STRING_TEST_FW_VERSION                 "T1.0.4.200827\r\n"
/*
** 20190812
** 更改电源指示灯为IO口控制
*/
#define DEF_MAIN_VER                            0xA5A5

//#define _EN_UART_DEBUG_PRINTF                   _MACRO_ENABLE

#if _EN_UART_DEBUG_PRINTF
#define UART2_BAUDRATE                          115200
#else
#define UART2_BAUDRATE                          9600
#endif

#define _DEBUG_PORT_SEL                         _UART_PORT_2

/////////////////////////Function Define//////////////////////
#define _ENABLE_USER_PRINTF_FUNCTION            _MACRO_ENABLE
#define _EN_EEPROM_FUN_SAVE_SYS_EDID_INDEX      _MACRO_ENABLE
#define _ENABLE_USER_EDID_LEARNING_FUNCTION     _MACRO_ENABLE
#define _ENABLE_HDMITX_HDCP_FUNCTION            _MACRO_ENABLE
//#define _ENABLE_STARTUP_TO_MATCHING_CHIPID      _MACRO_ENABLE
#define _ENABLE_SYSTEM_RESET_DELAY_TIME         _MACRO_ENABLE
#define _ENABLE_UART_LEARNING_MESSAGE_FUNCTION  _MACRO_ENABLE
#define _ENABLE_USER_DEFINE_CEC_CMD_TO_DEVICE   _MACRO_ENABLE
#define _ENABLE_DETECT_SYNC_TO_SEND_CEC_AND_RS232CMD    _MACRO_ENABLE
#define _ENABLE_KEYPAD_LOCKED_FUNCTION          _MACRO_ENABLE
#define _ENABLE_NEW_USER_KEY_DEFINE_FUNCTION    _MACRO_ENABLE
#define _ENABLE_EDID_SWITCH_UNREBOOT            _MACRO_ENABLE
#define _EN_GPIO_USB_DETECT                     _MACRO_ENABLE
//#define _ENABLE_UART_SEMAPHORE_FUNCTION_BY_PRINT    _MACRO_ENABLE
#define _ENABLE_SWITCH_PORT_AFTER_A_FEW_TIME_LATER  _MACRO_ENABLE
#define _ENABLE_SUPPORT_HDCP_VERSION_CHECK_DISPLAY  _MACRO_DISABLE
#define _DISABLE_EDID_BYPASS                        _MACRO_ENABLE
#define _ENABLE_IT66321_TX_HDMI_NO_HDCP             _MACRO_ENABLE
#define _ENABLE_LED_WORK_MOMENT_3S                  _MACRO_ENABLE
#define _ENABLE_AUDIO_SOURCE_SWITCH_TIMER           _MACRO_ENABLE
#define _ENABLE_IT6615_STARTUP_TODO_HDCP_TIMER      _MACRO_ENABLE
#define _ENABLE_UART_SEND_USER_MESSAGE_FUNCTION     _MACRO_ENABLE
#define _ENABLE_EDID_SWITCH_A_SINGLE_CHANNEL        _MACRO_ENABLE
#define _ENABLE_IT663XX_MULTI_CHIP_IN_USE           _MACRO_ENABLE
#define _ENABLE_MV_SOURCE_SWAP                      _MACRO_ENABLE
#define _ENABLE_SWITCH_LED_TIMER                    _MACRO_ENABLE
#define _ENABLE_WIN_CONFIG_UPD_TIMER                _MACRO_ENABLE
#define _ENABLE_FOURCE_ON_IT6615_STATE              _MACRO_ENABLE
#define _ENABLE_SWITCH_PORT_AND_AUDIO               _MACRO_ENABLE
#define _ENABLE_AUTO_POWER_FUNCTION                 _MACRO_ENABLE
//#define _ENABLE_SWITCH_PORT_INIT_CEC_FUNCTION       _MACRO_ENABLE
#define _ENABLE_THE_BEST_RESOLUTION_BY_DISPLAY      _MACRO_ENABLE
#define _ENABLE_INPUT_PORT_HDCP_FOURCE_ON_14_OR_22  _MACRO_ENABLE
//#define _ENABLE_IT663xx_TX_NO_SUPPORT_HDMI20        _MACRO_ENABLE
#define ENABLE_IT6634_LIMIT_6G_TO_ENC_SCDC          _MACRO_ENABLE    //
#define _ENABLE_IT663xx_REPLUG_INPUT_CABLE_CLEAN_BUFFER _MACRO_ENABLE
#define _ENABLE_IT666X_CLOSE_HDCP		            _MACRO_ENABLE

#define _ENABLE_EXTERNAL_CHIP_TASK                  _MACRO_ENABLE   // Enable Task_ExternalChip.c

#define Def_InDetMode                               emTMDS
// reset edid for input port
//#define PortResetForEdid        (RX_PORT_BIT_IND_2|RX_PORT_BIT_IND_3)

#define NUM_OF_CEC_PINS_MAX                         5
#define NUM_OF_I2C_BUS_MAX                          (eI2cDevice_Max+2)

#define Def_UserEdidMode                         EDID_MODE_INTERNAL2
#define SIZE_UART_KEY_VALUE_MAX                     200

#define EDID_START_ADDR_SAME_WITH_BOOT              0x0803D000

#define Def_RxFunc_Rx0                      (RX_FUN_HDCP_2)//(RX_FUN_FORCE_ON_HALF|RX_FUN_HDCP_2)
#define Def_RxFunc_Rx1                      (RX_FUN_HDCP_2)//(RX_FUN_FORCE_ON_HALF|RX_FUN_HDCP_2)
#define Def_RxFunc_Rx2                      (RX_FUN_HDCP_2)//(RX_FUN_FORCE_ON_HALF|RX_FUN_HDCP_2)
#define Def_RxFunc_Rx3                      (RX_FUN_HDCP_2)//(RX_FUN_FORCE_ON_HALF|RX_FUN_HDCP_2)

#define Def_Uart_CounterDelayTime                   500

#if _ENABLE_UART_LEARNING_MESSAGE_FUNCTION
#define _NUM_UART_STUDY_KEYS                    uart_study_key_type_max
#endif

#endif





