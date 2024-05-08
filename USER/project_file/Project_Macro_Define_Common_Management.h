#ifndef __PROJECT_MACRO_DEFINE_COMMON_MANAGEMENT_H_
#define __PROJECT_MACRO_DEFINE_COMMON_MANAGEMENT_H_
/******************************************************************/
/**CHIPS*USE****/
/******************************************************************/
//use chip pca9539
#ifndef _EN_CHIP_PCA9539_USE
#define _EN_CHIP_PCA9539_USE                    _MACRO_DISABLE
#endif

//use eeprom chip
#ifndef _CHIP_EEPROM_TYPE_USE
#define _CHIP_EEPROM_TYPE_USE                   _EEPROM_CHIP_24CM01
#endif

//use chip TM1623
#ifndef _EN_CHIP_TM1623_USE
#define _EN_CHIP_TM1623_USE                     _MACRO_DISABLE
#endif

#ifndef _ENABLE_CHIP_LM74HC595
#define _ENABLE_CHIP_LM74HC595                  _MACRO_DISABLE
#endif

#ifndef _ENABLE_CHIP_LM74HC138
#define _ENABLE_CHIP_LM74HC138                  _MACRO_DISABLE
#endif

#ifndef _ENABLE_CHIP_PT2258
#define _ENABLE_CHIP_PT2258                     _MACRO_DISABLE
#endif

///////////////////////////GSV CHIP///////////////////////////////
#ifndef _ENABLE_CHIP_GSV2006
#define _ENABLE_CHIP_GSV2006                        _MACRO_DISABLE
#endif

#ifndef _ENABLE_CHIP_GSV2008
#define _ENABLE_CHIP_GSV2008                        _MACRO_DISABLE
#endif

#ifndef _ENABLE_CHIP_GSV2002
#define _ENABLE_CHIP_GSV2002                        _MACRO_DISABLE
#endif

#ifndef _ENABLE_CHIP_GSV2001
#define _ENABLE_CHIP_GSV2001                        _MACRO_DISABLE
#endif

#ifndef _ENABLE_CHIP_GSV2011
#define _ENABLE_CHIP_GSV2011                        _MACRO_DISABLE
#endif

#ifndef _ENABLE_CHIP_GSV5100
#define _ENABLE_CHIP_GSV5100                        _MACRO_DISABLE
#endif
#ifndef _ENABLE_CHIP_GSV_CHIP_SERIES
#define _ENABLE_CHIP_GSV_CHIP_SERIES                (_MACRO_ENABLE&&\
                                                        (_ENABLE_CHIP_GSV2006\
                                                        ||_ENABLE_CHIP_GSV2008\
                                                        ||_ENABLE_CHIP_GSV2002\
                                                        ||_ENABLE_CHIP_GSV2001\
                                                        ||_ENABLE_CHIP_GSV2011\
                                                        ||_ENABLE_CHIP_GSV5100))
#endif

#ifndef _EN_CHIP_ADN4604_USE
#define _EN_CHIP_ADN4604_USE                        _MACRO_DISABLE
#endif

#ifndef DEV_NUM_ADN4604
#define DEV_NUM_ADN4604                             0
#endif

#ifndef _EN_CHIP_LT8644_USE
#define _EN_CHIP_LT8644_USE                         _MACRO_DISABLE
#endif

#ifndef DEV_NUM_LT8644
#define DEV_NUM_LT8644                              0
#endif

//////////////////////////////////////////////////////////////////

///===============================================================
//i2c index of table for PCA9539
#ifndef _I2C_TABLE_INDEX_PCA9539
#define _I2C_TABLE_INDEX_PCA9539                0
#endif

//i2c index of table for eeprom
#ifndef _I2C_TABLE_INDEX_EEPROM
#define _I2C_TABLE_INDEX_EEPROM                 0
#endif

#ifndef _I2C_TABLE_INDEX_IT6615_0
#define _I2C_TABLE_INDEX_IT6615_0               0
#endif

#ifndef _I2C_TABLE_INDEX_IT6615_1
#define _I2C_TABLE_INDEX_IT6615_1               1
#endif

// it66021 i2c index
#ifndef _I2C_TABLE_INDEX_IT660XX_0
#define _I2C_TABLE_INDEX_IT660XX_0              0
#endif

#ifndef _I2C_TABLE_INDEX_IT660XX_1
#define _I2C_TABLE_INDEX_IT660XX_1              1
#endif

#ifndef _I2C_TABLE_INDEX_IT660XX_2
#define _I2C_TABLE_INDEX_IT660XX_2              2
#endif

#ifndef _I2C_TABLE_INDEX_IT660XX_3
#define _I2C_TABLE_INDEX_IT660XX_3              3
#endif

#ifndef _I2C_TABLE_INDEX_IT660XX_4
#define _I2C_TABLE_INDEX_IT660XX_4              4
#endif

#ifndef _I2C_TABLE_INDEX_IT660XX_5
#define _I2C_TABLE_INDEX_IT660XX_5              5
#endif

#ifndef _I2C_TABLE_INDEX_IT660XX_6
#define _I2C_TABLE_INDEX_IT660XX_6              6
#endif

#ifndef _I2C_TABLE_INDEX_IT660XX_7
#define _I2C_TABLE_INDEX_IT660XX_7              7
#endif

#ifndef _I2C_TABLE_INDEX_IDT5V49_0
#define _I2C_TABLE_INDEX_IDT5V49_0              0
#endif

#ifndef _I2C_TABLE_INDEX_IDT5V49_1
#define _I2C_TABLE_INDEX_IDT5V49_1              1
#endif

//i2c index of table for IT66351
#ifndef _I2C_TABLE_INDEX_IT66351
#define _I2C_TABLE_INDEX_IT66351                0
#endif

#ifndef _I2C_TABLE_INDEX_GSV_2K8_0
#define _I2C_TABLE_INDEX_GSV_2K8_0              0
#endif

#ifndef _I2C_TABLE_INDEX_GSV_2K8_1
#define _I2C_TABLE_INDEX_GSV_2K8_1              1
#endif

#ifndef _I2C_TABLE_INDEX_GSV_2K8_2
#define _I2C_TABLE_INDEX_GSV_2K8_2              2
#endif

#ifndef _I2C_TABLE_INDEX_GSV_2K8_3
#define _I2C_TABLE_INDEX_GSV_2K8_3              3
#endif

#ifndef _I2C_TABLE_INDEX_GSV_2K6_0
#define _I2C_TABLE_INDEX_GSV_2K6_0              0
#endif

#ifndef _I2C_TABLE_INDEX_GSV_2K6_1
#define _I2C_TABLE_INDEX_GSV_2K6_1              1
#endif

#ifndef _I2C_TABLE_INDEX_GSV_2K6_2
#define _I2C_TABLE_INDEX_GSV_2K6_2              2
#endif

#ifndef _I2C_TABLE_INDEX_GSV_2K6_3
#define _I2C_TABLE_INDEX_GSV_2K6_3              3
#endif

#ifndef _I2C_TABLE_INDEX_GSV_2K2_0
#define _I2C_TABLE_INDEX_GSV_2K2_0              0
#endif

#ifndef _I2C_TABLE_INDEX_GSV_2K2_1
#define _I2C_TABLE_INDEX_GSV_2K2_1              1
#endif

#ifndef _I2C_TABLE_INDEX_GSV_2K2_2
#define _I2C_TABLE_INDEX_GSV_2K2_2              2
#endif

#ifndef _I2C_TABLE_INDEX_GSV_2K2_3
#define _I2C_TABLE_INDEX_GSV_2K2_3              3
#endif

#ifndef _I2C_TABLE_INDEX_GSV_2K1_0
#define _I2C_TABLE_INDEX_GSV_2K1_0              0
#endif

#ifndef _I2C_TABLE_INDEX_GSV_2K1_1
#define _I2C_TABLE_INDEX_GSV_2K1_1              1
#endif

#ifndef _I2C_TABLE_INDEX_GSV_2K1_2
#define _I2C_TABLE_INDEX_GSV_2K1_2              2
#endif

#ifndef _I2C_TABLE_INDEX_GSV_2K1_3
#define _I2C_TABLE_INDEX_GSV_2K1_3              3
#endif

#ifndef _I2C_TABLE_INDEX_GSV_2K1_4
#define _I2C_TABLE_INDEX_GSV_2K1_4              4
#endif

#ifndef _I2C_TABLE_INDEX_GSV_2K1_5
#define _I2C_TABLE_INDEX_GSV_2K1_5              5
#endif

#ifndef _I2C_TABLE_INDEX_GSV_2K1_6
#define _I2C_TABLE_INDEX_GSV_2K1_6              6
#endif

#ifndef _I2C_TABLE_INDEX_GSV_2K1_7
#define _I2C_TABLE_INDEX_GSV_2K1_7              7
#endif

#ifndef _I2C_TABLE_INDEX_GSV_2K11_0
#define _I2C_TABLE_INDEX_GSV_2K11_0             0
#endif

#ifndef _I2C_TABLE_INDEX_GSV_2K11_1
#define _I2C_TABLE_INDEX_GSV_2K11_1             1
#endif

#ifndef _I2C_TABLE_INDEX_GSV_2K11_2
#define _I2C_TABLE_INDEX_GSV_2K11_2             2
#endif

#ifndef _I2C_TABLE_INDEX_GSV_2K11_3
#define _I2C_TABLE_INDEX_GSV_2K11_3             3
#endif

#ifndef _I2C_TABLE_INDEX_GSV_2K11_4
#define _I2C_TABLE_INDEX_GSV_2K11_4             4
#endif

#ifndef _I2C_TABLE_INDEX_GSV_2K11_5
#define _I2C_TABLE_INDEX_GSV_2K11_5             5
#endif

#ifndef _I2C_TABLE_INDEX_GSV_2K11_6
#define _I2C_TABLE_INDEX_GSV_2K11_6             6
#endif

#ifndef _I2C_TABLE_INDEX_GSV_2K11_7
#define _I2C_TABLE_INDEX_GSV_2K11_7             7
#endif

#ifndef _I2C_TABLE_INDEX_GSV_2K11_8
#define _I2C_TABLE_INDEX_GSV_2K11_8             8
#endif

#ifndef _I2C_TABLE_INDEX_GSV_2K11_9
#define _I2C_TABLE_INDEX_GSV_2K11_9             9
#endif

#ifndef _I2C_TABLE_INDEX_GSV_2K11_10
#define _I2C_TABLE_INDEX_GSV_2K11_10            10
#endif

#ifndef _I2C_TABLE_INDEX_GSV_2K11_11
#define _I2C_TABLE_INDEX_GSV_2K11_11            11
#endif

#ifndef _I2C_TABLE_INDEX_GSV_2K11_12
#define _I2C_TABLE_INDEX_GSV_2K11_12            12
#endif

#ifndef _I2C_TABLE_INDEX_GSV_2K11_13
#define _I2C_TABLE_INDEX_GSV_2K11_13            13
#endif

#ifndef _I2C_TABLE_INDEX_GSV_2K11_14
#define _I2C_TABLE_INDEX_GSV_2K11_14            14
#endif

#ifndef _I2C_TABLE_INDEX_GSV_2K11_15
#define _I2C_TABLE_INDEX_GSV_2K11_15            15
#endif

#ifndef _I2C_TABLE_INDEX_GSV_5K1_0
#define _I2C_TABLE_INDEX_GSV_5K1_0              0
#endif

#ifndef _I2C_TABLE_INDEX_GSV_5K1_1
#define _I2C_TABLE_INDEX_GSV_5K1_1              1
#endif

#ifndef _I2C_TABLE_INDEX_GSV_5K1_2
#define _I2C_TABLE_INDEX_GSV_5K1_2              2
#endif

#ifndef _I2C_TABLE_INDEX_GSV_5K1_3
#define _I2C_TABLE_INDEX_GSV_5K1_3              3
#endif

#ifndef _I2C_TABLE_INDEX_PT2258_1
#define _I2C_TABLE_INDEX_PT2258_1               0
#endif

#ifndef _I2C_TABLE_INDEX_ADN4604_1
#define _I2C_TABLE_INDEX_ADN4604_1              0
#endif

#ifndef _I2C_TABLE_INDEX_ADN4604_2
#define _I2C_TABLE_INDEX_ADN4604_2              1
#endif

#ifndef _I2C_TABLE_INDEX_ADN4604_3
#define _I2C_TABLE_INDEX_ADN4604_3              2
#endif

#ifndef _I2C_TABLE_INDEX_ADN4604_4
#define _I2C_TABLE_INDEX_ADN4604_4              3
#endif

#ifndef _I2C_TABLE_INDEX_LT8644_0
#define _I2C_TABLE_INDEX_LT8644_0               0
#endif

#ifndef _I2C_TABLE_INDEX_LT8644_1
#define _I2C_TABLE_INDEX_LT8644_1               1
#endif

#ifndef _I2C_TABLE_INDEX_LT8644_2
#define _I2C_TABLE_INDEX_LT8644_2               2
#endif

#ifndef _I2C_TABLE_INDEX_LT8644_3
#define _I2C_TABLE_INDEX_LT8644_3               3
#endif

#ifndef _I2C_TABLE_INDEX_LT8644_4
#define _I2C_TABLE_INDEX_LT8644_4               4
#endif

//spi index of table for TM1623
#ifndef _SPI_TABLE_INDEX_TM1623
#define _SPI_TABLE_INDEX_TM1623                 0
#endif

#ifndef _SPI_TABLE_INDEX_TM1623_1
#define _SPI_TABLE_INDEX_TM1623_1               1
#endif

#ifndef _SPI_TABLE_INDEX_TM1623_2
#define _SPI_TABLE_INDEX_TM1623_2               2
#endif

#ifndef _SPI_TABLE_INDEX_TM1623_3
#define _SPI_TABLE_INDEX_TM1623_3               3
#endif

#ifndef _SPI_TABLE_INDEX_TM1623_4
#define _SPI_TABLE_INDEX_TM1623_4               4
#endif

#ifndef _SPI_TABLE_INDEX_74HC595
#define _SPI_TABLE_INDEX_74HC595			    0
#endif

#ifndef _SPI_TABLE_INDEX_FLASH
#define _SPI_TABLE_INDEX_FLASH			        0
#endif

#ifndef _CHIP_TABLE_INDEX_74HC138
#define _CHIP_TABLE_INDEX_74HC138			    0
#endif

//ir function
#ifndef _IR_FUNCTION_USE
#define _IR_FUNCTION_USE                        _IR_FUNCTION_NONE
#endif

//ir study keys number
#ifndef _IR_STUDY_KEYS_NUMBER
#define _IR_STUDY_KEYS_NUMBER                   1
#endif

//ir sensor input pin
#ifndef _IR_SENSOR_IN_PORT_USE
#define _IR_SENSOR_IN_PORT_USE                  _IR_SENSOR_IN_GPIOB9
#endif

#ifndef _NUM_SIMUL_GPIO_PINS_ONE_KEY_MAX
#define _NUM_SIMUL_GPIO_PINS_ONE_KEY_MAX        1//the min val is 1
#endif

#ifndef _NUM_SIMUL_KEYS_COMPOSITE_MAX
#define _NUM_SIMUL_KEYS_COMPOSITE_MAX           1//the min val is 1
#endif

#ifndef _NUM_COMPOSITE_GROUP_ONE_KEY_MAX
#define _NUM_COMPOSITE_GROUP_ONE_KEY_MAX        1//the min val is 1
#endif

#ifndef _EN_UART_DEBUG_PRINTF
#define _EN_UART_DEBUG_PRINTF                   _MACRO_DISABLE
#endif

/******************************************************************/
/**BOARD*GPIO****/
/******************************************************************/
//enable hdmi input cec pin receive function
#ifndef _EN_HDMI_INPUT_CEC_PIN_RECEIVE
#define _EN_HDMI_INPUT_CEC_PIN_RECEIVE          _MACRO_DISABLE
#endif

//enable hdmi output cec pin receive function
#ifndef _EN_HDMI_OUTPUT_CEC_PIN_RECEIVE
#define _EN_HDMI_OUTPUT_CEC_PIN_RECEIVE         _MACRO_DISABLE
#endif

//use gpio reset pca9539
#ifndef _EN_GPIO_PCA9539_RESET
#define _EN_GPIO_PCA9539_RESET                  _MACRO_DISABLE
#endif

//use gpio usb detect
#ifndef _EN_GPIO_USB_DETECT
#define _EN_GPIO_USB_DETECT                     _MACRO_DISABLE
#endif

//use gpio control 4052 A for uart transmit to valens or back to pc
#ifndef _EN_GPIO_UART_TRANS_BACK_4052_A
#define _EN_GPIO_UART_TRANS_BACK_4052_A         _MACRO_DISABLE
#endif

//use gpio control 4052 B for uart transmit to valens or back to pc
#ifndef _EN_GPIO_UART_TRANS_BACK_4052_B
#define _EN_GPIO_UART_TRANS_BACK_4052_B         _MACRO_DISABLE
#endif

/***RCC*OF*GPIO***/
//ALL THE RCC OF GPIO
#ifndef _RCC_APB2Periph_GPIO
#define _RCC_APB2Periph_GPIO                    RCC_APB2Periph_GPIOA |\
                                                RCC_APB2Periph_GPIOB |\
                                                RCC_APB2Periph_GPIOC |\
                                                RCC_APB2Periph_GPIOD |\
                                                RCC_APB2Periph_AFIO
#endif

#ifndef RCC_AHB1Periph_GPIO
#define RCC_AHB1Periph_GPIO                     RCC_AHB1Periph_GPIOA\
                                                |RCC_AHB1Periph_GPIOB\
                                                |RCC_AHB1Periph_GPIOC\
                                                |RCC_AHB1Periph_GPIOD\
                                                |RCC_AHB1Periph_GPIOE
#endif

/******************************************************************/
/**BOARD*FUNCTION****/
/******************************************************************/
/***Power*On*TV*When*Detected*Signal***/
//power on tv by cec when detected input signal
#ifndef _EN_CEC_POWER_ON_TV_DETECTED_SIGNAL
#define _EN_CEC_POWER_ON_TV_DETECTED_SIGNAL     _MACRO_DISABLE
#endif

/***Power*Off*TV*When*Detected*No*Signal***/
//power off tv by cec when detected input no signal
#ifndef _EN_CEC_POWER_OFF_TV_DETECTED_NO_SIGNAL
#define _EN_CEC_POWER_OFF_TV_DETECTED_NO_SIGNAL _MACRO_DISABLE
#endif

//power on tv by cec when boot restart
#ifndef _EN_CEC_POWER_ON_TV_BOOT_RESTART
#define _EN_CEC_POWER_ON_TV_BOOT_RESTART        _MACRO_DISABLE
#endif

/***IR*RECEIVE*BYPASS***/
#ifndef _EN_IR_RECEIVE_SEND_BYPASS
#define _EN_IR_RECEIVE_SEND_BYPASS              _MACRO_DISABLE
#endif

//save hdmi sel (auto & manu) mode input channel
#ifndef _EN_EEPROM_FUN_SAVE_HDMI_SEL
#define _EN_EEPROM_FUN_SAVE_HDMI_SEL            _MACRO_DISABLE
#endif

//save hdmi auto & manu switch mode
#ifndef _EN_EEPROM_FUN_SAVE_TYPE_SWITCH_MODE
#define _EN_EEPROM_FUN_SAVE_TYPE_SWITCH_MODE    _MACRO_DISABLE
#endif

//save uart study keys' value
#ifndef _EN_EEPROM_FUN_SAVE_UART_STUDY_KEYS
#define _EN_EEPROM_FUN_SAVE_UART_STUDY_KEYS     _MACRO_DISABLE
#endif

//save hdmi hdcp on/off status
#ifndef _EN_EEPROM_FUN_SAVE_HDCP_STATUS
#define _EN_EEPROM_FUN_SAVE_HDCP_STATUS         _MACRO_DISABLE
#endif

//save system edid index status
#ifndef _EN_EEPROM_FUN_SAVE_SYS_EDID_INDEX
#define _EN_EEPROM_FUN_SAVE_SYS_EDID_INDEX      _MACRO_DISABLE
#endif

//num
#ifndef _NUM_IR_STUDY_KEYS
#define _NUM_IR_STUDY_KEYS                      2
#endif

#ifndef _NUM_UART_STUDY_KEYS
#define _NUM_UART_STUDY_KEYS                    2
#endif
/***EDID********/
#ifndef _DEFAULT_EDID_NUM
#define _DEFAULT_EDID_NUM                       _DEFAULT_EDID_720P_LPCM
#endif

/***WATCH*DOG***/
//enable watch dog
#ifndef _EN_WATCH_DOG
#define _EN_WATCH_DOG                           _MACRO_DISABLE
#endif


/******************************************************************/
/***TIME*OF*SYSTEM*RUN***/
/******************************************************************/
//led normal blink toggle time
#ifndef _TIME_INDICATIVE_LED_NORM_BLINK_TOGGLE
#define _TIME_INDICATIVE_LED_NORM_BLINK_TOGGLE  25//unit = 20ms, depends on the delay time of task_led
#endif

//led fast blink toggle time
#ifndef _TIME_INDICATIVE_LED_FAST_BLINK_TOGGLE
#define _TIME_INDICATIVE_LED_FAST_BLINK_TOGGLE  5//unit = 20ms
#endif

//led slow blink toggle time
#ifndef _TIME_INDICATIVE_LED_SLOW_BLINK_TOGGLE
#define _TIME_INDICATIVE_LED_SLOW_BLINK_TOGGLE  50//unit = 20ms
#endif

//led moment on time
#ifndef _TIME_INDICATIVE_LED_MOMENT_ON
#define _TIME_INDICATIVE_LED_MOMENT_ON          50//unit = 20ms
#endif

//led moment off time
#ifndef _TIME_INDICATIVE_LED_MOMENT_OFF
#define _TIME_INDICATIVE_LED_MOMENT_OFF         50//unit = 20ms
#endif

//led moment on time
#ifndef _TIME_INDICATIVE_LED_MOMENT_ON_3S
#define _TIME_INDICATIVE_LED_MOMENT_ON_3S       150//unit = 20ms
#endif

//led moment off time
#ifndef _TIME_INDICATIVE_LED_MOMENT_OFF_3S
#define _TIME_INDICATIVE_LED_MOMENT_OFF_3S      150//unit = 20ms
#endif

//led moment toggle time
#ifndef _TIME_INDICATIVE_LED_MOMENT_TOGGLE
#define _TIME_INDICATIVE_LED_MOMENT_TOGGLE      50//unit = 20ms
#endif

//auto switch detect time
#ifndef _TIME_AUTO_SWITCH_DETECT
#define _TIME_AUTO_SWITCH_DETECT                500//uint is 1ms
#endif

//cec power on tv signal detect after manual switch time
#ifndef _TIME_CEC_ON_TV_SD_AF_MAN_SW
#define _TIME_CEC_ON_TV_SD_AF_MAN_SW            2000//uint is 5ms(min.)
#endif

//auto switch detect time when res changed
#ifndef _TIME_AUTO_SWTICH_RES_CHNGED
#define _TIME_AUTO_SWTICH_RES_CHNGED                10000//uint is 1ms
#endif

//detect input hotplug time
#ifndef TIME_INPUT_HOTPLUG_DETECT
#define TIME_INPUT_HOTPLUG_DETECT               200//unit is 5ms
#endif

//if it is ir study mode, system quit ir study mode after 15s without operation
#ifndef TIME_QUIT_IR_STUDY_WITHOUT_OPERATE
#define TIME_QUIT_IR_STUDY_WITHOUT_OPERATE      6000//unit is 5ms(min.)
#endif

//delay time for HDCP encrypt when use VALENS
#ifndef _TIME_DELAY_FOR_HDCP_EN_USE_VALENS
#define _TIME_DELAY_FOR_HDCP_EN_USE_VALENS      0//unit is 1ms
#endif

/******************************************************************/
/***GENERAL*CONFIGURATION***/
/******************************************************************/
/***HDCP*METHOD***/
//HDCP MODE PROJECT ACTIVE
#ifndef _PRJ_TX_HDCP_MODE_ACTIVE
#define _PRJ_TX_HDCP_MODE_ACTIVE                /*tx_hdcp_force_off*/  \
                                                /*tx_hdcp_force_1_4*/  \
                                                /*tx_hdcp_force_2_2*/  \
                                                tx_hdcp_force_bypass
#endif


//hdmi input port used number
#ifndef _HDMI_INPUT_PORT_USED_NUMBER
#define _HDMI_INPUT_PORT_USED_NUMBER            2
#endif

/***********************************Macro****************************************/

#ifndef BootWaitTime
#define BootWaitTime                                1000
#endif

//use gpio hdmi input  led num
#ifndef _EN_GPIO_HDMI_INPUT_NUM_LED
#define _EN_GPIO_HDMI_INPUT_NUM_LED                 1
#endif

#ifndef UART2_BAUDRATE
#define UART2_BAUDRATE                              9600
#endif

#ifndef _ENABLE_EDID_SWITCH_USE_NEW_MODE
#define _ENABLE_EDID_SWITCH_USE_NEW_MODE            _MACRO_DISABLE
#endif

#ifndef _ENABLE_SUPPORT_HDCP_VERSION_CHECK_DISPLAY
#define _ENABLE_SUPPORT_HDCP_VERSION_CHECK_DISPLAY  _MACRO_ENABLE
#endif

// UART learning message function
#ifndef _ENABLE_UART_LEARNING_MESSAGE_FUNCTION
#define _ENABLE_UART_LEARNING_MESSAGE_FUNCTION      _MACRO_DISABLE
#endif

//Use user printf function
#ifndef _ENABLE_USER_PRINTF_FUNCTION
#define _ENABLE_USER_PRINTF_FUNCTION                _MACRO_DISABLE
#endif

// User EDID learning function
#ifndef _ENABLE_USER_EDID_LEARNING_FUNCTION
#define _ENABLE_USER_EDID_LEARNING_FUNCTION         _MACRO_DISABLE
#endif

#ifndef _ENABLE_HDMITX_HDCP_FUNCTION
#define _ENABLE_HDMITX_HDCP_FUNCTION                _MACRO_DISABLE
#endif

// user key define function
#ifndef _ENABLE_USER_KEY_DEFINE_FUNCTION
#define _ENABLE_USER_KEY_DEFINE_FUNCTION            _MACRO_DISABLE
#endif

#ifndef _ENABLE_STARTUP_TO_MATCHING_CHIPID
#define _ENABLE_STARTUP_TO_MATCHING_CHIPID          _MACRO_DISABLE
#endif

#ifndef _ENABLE_SYSTEM_RESET_DELAY_TIME
#define _ENABLE_SYSTEM_RESET_DELAY_TIME             _MACRO_DISABLE
#endif

#ifndef _ENABLE_USER_DEFINE_CEC_CMD_TO_DEVICE
#define _ENABLE_USER_DEFINE_CEC_CMD_TO_DEVICE       _MACRO_DISABLE
#endif

#ifndef _ENABLE_DETECT_SYNC_TO_SEND_CEC_AND_RS232CMD
#define _ENABLE_DETECT_SYNC_TO_SEND_CEC_AND_RS232CMD    _MACRO_DISABLE
#endif

#ifndef _ENABLE_SYNC_TO_SEND_CMD_BY_ONLY_AUTO_MODE
#define _ENABLE_SYNC_TO_SEND_CMD_BY_ONLY_AUTO_MODE  _MACRO_DISABLE
#endif

#ifndef _ENABLE_KEYPAD_LOCKED_FUNCTION
#define _ENABLE_KEYPAD_LOCKED_FUNCTION              _MACRO_DISABLE
#endif

// user key define function
#ifndef _ENABLE_NEW_USER_KEY_DEFINE_FUNCTION
#define _ENABLE_NEW_USER_KEY_DEFINE_FUNCTION        _MACRO_DISABLE
#endif

#ifndef _ENABLE_SWITCH_EDID_SKIP_TYPE_C_PORT
#define _ENABLE_SWITCH_EDID_SKIP_TYPE_C_PORT        _MACRO_DISABLE
#endif

#ifndef _ENABLE_EDID_SWITCH_UNREBOOT
#define _ENABLE_EDID_SWITCH_UNREBOOT                _MACRO_DISABLE
#endif

#ifndef _ENABLE_USB_SWITCH_BY_INPUT_PORT
#define _ENABLE_USB_SWITCH_BY_INPUT_PORT            _MACRO_DISABLE
#endif

#ifndef _DEBUG_PORT_SEL
#define _DEBUG_PORT_SEL                             _UART_PORT_NONE
#endif

#ifndef _ENABLE_SWITCH_PORT_INIT_CEC_FUNCTION
#define _ENABLE_SWITCH_PORT_INIT_CEC_FUNCTION       _MACRO_DISABLE
#endif

#ifndef _ENABLE_SWITCH_PORT_AFTER_A_FEW_TIME_LATER
#define _ENABLE_SWITCH_PORT_AFTER_A_FEW_TIME_LATER  _MACRO_DISABLE
#endif

// for test
#ifndef _ENABLE_START_UP_TO_ENABLE_EXTERNAL_AUDIO
#define _ENABLE_START_UP_TO_ENABLE_EXTERNAL_AUDIO   _MACRO_DISABLE
#endif

#ifndef _ENABLE_UART_SEMAPHORE_FUNCTION_BY_PRINT
#define _ENABLE_UART_SEMAPHORE_FUNCTION_BY_PRINT    _MACRO_DISABLE
#endif

#ifndef _ENABLE_TYPE_C_REPLUG_HPD
#define _ENABLE_TYPE_C_REPLUG_HPD                   _MACRO_DISABLE
#endif

#ifndef _ENABLE_SWITCH_PORT_TO_MUTE_I2S_AUDIO
#define _ENABLE_SWITCH_PORT_TO_MUTE_I2S_AUDIO       _MACRO_DISABLE
#endif

#ifndef _ENABLE_RESET_WIRELESS_POWER
#define _ENABLE_RESET_WIRELESS_POWER                _MACRO_DISABLE
#endif

#ifndef _ENABLE_CHIP_ITE66021
#define _ENABLE_CHIP_ITE66021                       _MACRO_DISABLE
#endif

#ifndef _ENABLE_CHIP_ITE6615
#define _ENABLE_CHIP_ITE6615                        _MACRO_DISABLE
#endif

#ifndef _ENABLE_EXTERNAL_CHIP_TASK
#define _ENABLE_EXTERNAL_CHIP_TASK                  _MACRO_DISABLE
#endif

#ifndef _ENABLE_DEBUG_CMD_BY_UART
#define _ENABLE_DEBUG_CMD_BY_UART                   _MACRO_DISABLE
#endif

#ifndef _ENABLE_SAVE_CURRENT_EDID
#define _ENABLE_SAVE_CURRENT_EDID                   _MACRO_DISABLE
#endif

#ifndef _ENABLE_CHIP_IDT5V49EE902
#define _ENABLE_CHIP_IDT5V49EE902                   _MACRO_DISABLE
#endif

#ifndef _ENABLE_CHIP_IDT5P49V6965
#define _ENABLE_CHIP_IDT5P49V6965                   _MACRO_DISABLE
#endif


#ifndef _DISABLE_EDID_BYPASS
#define _DISABLE_EDID_BYPASS                        _MACRO_DISABLE
#endif

#ifndef _ENABLE_SET_DISPALY_OFF_TIME
#define _ENABLE_SET_DISPALY_OFF_TIME                _MACRO_DISABLE
#endif

#ifndef _ENABLE_LED_WORK_MOMENT_3S
#define _ENABLE_LED_WORK_MOMENT_3S                  _MACRO_DISABLE
#endif

#ifndef _ENABLE_AUDIO_SOURCE_SWITCH_TIMER
#define _ENABLE_AUDIO_SOURCE_SWITCH_TIMER           _MACRO_DISABLE
#endif

#ifndef _ENABLE_IT6615_STARTUP_TODO_HDCP_TIMER
#define _ENABLE_IT6615_STARTUP_TODO_HDCP_TIMER      _MACRO_DISABLE
#endif

#ifndef _ENABLE_UART_SEND_USER_MESSAGE_FUNCTION
#define _ENABLE_UART_SEND_USER_MESSAGE_FUNCTION     _MACRO_DISABLE
#endif

#ifndef _ENABLE_EDID_SWITCH_A_SINGLE_CHANNEL
#define _ENABLE_EDID_SWITCH_A_SINGLE_CHANNEL        _MACRO_DISABLE
#endif

#ifndef _ENABLE_IT663XX_MULTI_CHIP_IN_USE
#define _ENABLE_IT663XX_MULTI_CHIP_IN_USE           _MACRO_DISABLE
#endif

#ifndef _ENABLE_MV_SOURCE_SWAP
#define _ENABLE_MV_SOURCE_SWAP                      _MACRO_DISABLE
#endif

#ifndef _ENABLE_TYPE_C_EDID_BY_PASS_ONLY
#define _ENABLE_TYPE_C_EDID_BY_PASS_ONLY            _MACRO_DISABLE
#endif

#ifndef _ENABLE_SWITCH_LED_TIMER
#define _ENABLE_SWITCH_LED_TIMER                    _MACRO_DISABLE
#endif

#ifndef _ENABLE_WIN_CONFIG_UPD_TIMER
#define _ENABLE_WIN_CONFIG_UPD_TIMER                _MACRO_DISABLE
#endif

#ifndef _ENABLE_FOURCE_ON_IT6615_STATE              // IT6615 Hpd,video ourput and TMDS stable force on
#define _ENABLE_FOURCE_ON_IT6615_STATE              _MACRO_DISABLE
#endif

#ifndef _ENABLE_SWITCH_PORT_AND_AUDIO
#define _ENABLE_SWITCH_PORT_AND_AUDIO               _MACRO_DISABLE
#endif

#ifndef _ENABLE_AUTO_POWER_FUNCTION                 // Auto standby function when no signal input
#define _ENABLE_AUTO_POWER_FUNCTION                 _MACRO_DISABLE
#endif

#ifndef _ENABLE_FAST_TO_SWITCH_PORT                 // Fast switch function
#define _ENABLE_FAST_TO_SWITCH_PORT                 _MACRO_DISABLE
#endif

#ifndef _ENABLE_INPUT_PORT_HDCP_FOURCE_ON_14_OR_22      // Input port hdcp support to force on hdcp1.4 or hdcp2.2
#define _ENABLE_INPUT_PORT_HDCP_FOURCE_ON_14_OR_22  _MACRO_DISABLE
#endif

#ifndef _ENABLE_CEC_FUNCTION_USE
#define _ENABLE_CEC_FUNCTION_USE                    _MACRO_ENABLE
#endif

#ifndef _ENABLE_THE_BEST_RESOLUTION_BY_DISPLAY      // Output resolution from the best timing by display
#define _ENABLE_THE_BEST_RESOLUTION_BY_DISPLAY      _MACRO_DISABLE
#endif

#ifndef _ENABLE_RES_1600x900_60
#define _ENABLE_RES_1600x900_60                     _MACRO_DISABLE
#endif

#ifndef _ENABLE_RES_1680x1050_60
#define _ENABLE_RES_1680x1050_60                    _MACRO_DISABLE
#endif

#ifndef _ENABLE_RES_1400x1050_60
#define _ENABLE_RES_1400x1050_60                    _MACRO_DISABLE
#endif

#ifndef _ENABLE_RES_1280x800_60
#define _ENABLE_RES_1280x800_60                     _MACRO_DISABLE
#endif

#ifndef _ENABLE_RES_1280x720_50
#define _ENABLE_RES_1280x720_50                     _MACRO_DISABLE
#endif

#ifndef _ENABLE_RES_1920x1080_50
#define _ENABLE_RES_1920x1080_50                    _MACRO_DISABLE
#endif

#ifndef _ENABLE_RES_3840x2160_60
#define _ENABLE_RES_3840x2160_60                    _MACRO_DISABLE
#endif

#ifndef _ENABLE_RES_3840x2160_50
#define _ENABLE_RES_3840x2160_50                    _MACRO_DISABLE
#endif

#ifndef _ENABLE_RES_3840x2160_30
#define _ENABLE_RES_3840x2160_30                    _MACRO_DISABLE
#endif

#ifndef _ENABLE_RES_4096x2160_30
#define _ENABLE_RES_4096x2160_30                    _MACRO_DISABLE
#endif

#ifndef _ENABLE_RES_4096x2160_60
#define _ENABLE_RES_4096x2160_60                    _MACRO_DISABLE
#endif

#ifndef _ENABLE_RES_4096x2160_50
#define _ENABLE_RES_4096x2160_50                    _MACRO_DISABLE
#endif

#ifndef _SWITCH_CHIP_NUM
#define _SWITCH_CHIP_NUM                            1
#endif  //#ifndef _SWITCH_CHIP_NUM

//modify edid to standard
#ifndef FUNCTION_MACRO_EDID_MODIFY_TO_STANDARD
#define FUNCTION_MACRO_EDID_MODIFY_TO_STANDARD		_MACRO_DISABLE
#endif

#ifndef _ENABLE_MULTI_OUTPUT_CHIP
#define _ENABLE_MULTI_OUTPUT_CHIP				    _MACRO_DISABLE
#endif

#ifndef _ENABLE_AUTO_TURN_TYPE
#define _ENABLE_AUTO_TURN_TYPE	                    _MACRO_DISABLE
#endif

#ifndef Def_outport_num
#define Def_outport_num				                1
#endif

#ifndef ENABLE_NEW_COMMAND_FUNCTION
#define ENABLE_NEW_COMMAND_FUNCTION                 _MACRO_DISABLE
#endif

#ifndef _ENABLE_EDID_SWITCH_AUDIO
#define _ENABLE_EDID_SWITCH_AUDIO                   _MACRO_DISABLE
#endif

#ifndef _ENABLE_SWITCH_TO_USBC_EDID
#define _ENABLE_SWITCH_TO_USBC_EDID                 _MACRO_DISABLE
#endif

#ifndef _ENABLE_UART_KEY_VALUE_MAX_BUG
#define _ENABLE_UART_KEY_VALUE_MAX_BUG              _MACRO_DISABLE
#endif

#ifndef _ENABLE_DEBUG_FIRST_POWER_ON_NO_EDID_
#define _ENABLE_DEBUG_FIRST_POWER_ON_NO_EDID_       _MACRO_DISABLE
#endif

#ifndef _ENABLE_DEBUG_LG_27UD69_FLASH_
#define _ENABLE_DEBUG_LG_27UD69_FLASH_              _MACRO_DISABLE
#endif

#ifndef _ENABLE_UART_1
#define _ENABLE_UART_1                              _MACRO_DISABLE
#endif

#ifndef _ENABLE_UART_2
#define _ENABLE_UART_2                              _MACRO_DISABLE
#endif

#ifndef _ENABLE_UART_3
#define _ENABLE_UART_3                              _MACRO_DISABLE
#endif

#ifndef _ENABLE_UART_4
#define _ENABLE_UART_4                              _MACRO_DISABLE
#endif

#ifndef _ENABLE_UART_5
#define _ENABLE_UART_5                              _MACRO_DISABLE
#endif

#ifndef _ENABLE_UART_6
#define _ENABLE_UART_6                              _MACRO_DISABLE
#endif

#ifndef _ENABLE_UART_7
#define _ENABLE_UART_7                              _MACRO_DISABLE
#endif

#ifndef _ENABLE_UART_3_PD8_PD9
#define _ENABLE_UART_3_PD8_PD9                      _MACRO_DISABLE
#endif

#ifndef _ENABLE_PCA9539_LED_CONTROL
#define _ENABLE_PCA9539_LED_CONTROL                 _MACRO_DISABLE
#endif

#ifndef _ENABLE_MULTI_VIEW_TO_UNUSED_HDCP_BYPASS
#define _ENABLE_MULTI_VIEW_TO_UNUSED_HDCP_BYPASS    _MACRO_DISABLE
#endif

#ifndef ENABLE_IR_COUNTED_USE_TIMER_CLOCK_50US
#define ENABLE_IR_COUNTED_USE_TIMER_CLOCK_50US      _MACRO_DISABLE
#endif

#ifndef _ENABLE_IT663xx_RECONNECT_CABLE_HPD_LOW_TO_HIGH
#define _ENABLE_IT663xx_RECONNECT_CABLE_HPD_LOW_TO_HIGH _MACRO_DISABLE
#endif

#ifndef _ENABLE_UART_LEARN_TO_MULTI_FUNCTION
#define _ENABLE_UART_LEARN_TO_MULTI_FUNCTION        _MACRO_DISABLE
#endif

#ifndef _ENABLE_AUTO_RELAY_FUNCTION                 // Auto standby function when no signal input
#define _ENABLE_AUTO_RELAY_FUNCTION                 _MACRO_DISABLE
#endif

#ifndef ENABLE_IT6634_SWITCH_EDID_LONG_TIME_HPD_LOW_TO_HIGH
#define ENABLE_IT6634_SWITCH_EDID_LONG_TIME_HPD_LOW_TO_HIGH     _MACRO_DISABLE    //
#endif

#ifndef _EN_MAIN_TASK_LED
#define _EN_MAIN_TASK_LED                           _MACRO_ENABLE
#endif

#ifndef _EN_MAIN_TASK_EEPROM
#define _EN_MAIN_TASK_EEPROM                        (_CHIP_EEPROM_TYPE_USE != _EEPROM_CHIP_NO_USE)//_MACRO_ENABLE
#endif


#ifndef _ENABLE_CHIP_ITE66351
#define _ENABLE_CHIP_ITE66351                       _MACRO_DISABLE
#endif

#ifndef _ENABLE_CHIP_ITE66353
#define _ENABLE_CHIP_ITE66353                       _MACRO_DISABLE
#endif

//i2c index of table for IT6635X
#ifndef _I2C_TABLE_INDEX_IT6635X_0
#define _I2C_TABLE_INDEX_IT6635X_0                  0
#endif

#ifndef _I2C_TABLE_INDEX_IT6635X_1
#define _I2C_TABLE_INDEX_IT6635X_1                  1
#endif

#ifndef _I2C_TABLE_INDEX_IT6635X_2
#define _I2C_TABLE_INDEX_IT6635X_2                  2
#endif

#ifndef _I2C_TABLE_INDEX_IT6635X_3
#define _I2C_TABLE_INDEX_IT6635X_3                  3
#endif

#ifndef _IT6635X_SWADDR_0
#define _IT6635X_SWADDR_0                           0
#endif

#ifndef _IT6635X_SWADDR_1
#define _IT6635X_SWADDR_1                           0
#endif

#ifndef _IT6635X_SWADDR_2
#define _IT6635X_SWADDR_2                           0
#endif

#ifndef _IT6635X_SWADDR_3
#define _IT6635X_SWADDR_3                           0
#endif

#ifndef _IT6635X_RXADDR_0
#define _IT6635X_RXADDR_0                           0
#endif

#ifndef _IT6635X_RXADDR_1
#define _IT6635X_RXADDR_1                           0
#endif

#ifndef _IT6635X_RXADDR_2
#define _IT6635X_RXADDR_2                           0
#endif

#ifndef _IT6635X_RXADDR_3
#define _IT6635X_RXADDR_3                           0
#endif

#ifndef _IT6635X_CECADDR_0
#define _IT6635X_CECADDR_0                          0
#endif

#ifndef _IT6635X_CECADDR_1
#define _IT6635X_CECADDR_1                          0
#endif

#ifndef _IT6635X_CECADDR_2
#define _IT6635X_CECADDR_2                          0
#endif

#ifndef _IT6635X_CECADDR_3
#define _IT6635X_CECADDR_3                          0
#endif

#ifndef _IT6635X_EDIDADDR_0
#define _IT6635X_EDIDADDR_0                         0
#endif

#ifndef _IT6635X_EDIDADDR_1
#define _IT6635X_EDIDADDR_1                         0
#endif

#ifndef _IT6635X_EDIDADDR_2
#define _IT6635X_EDIDADDR_2                         0
#endif

#ifndef _IT6635X_EDIDADDR_3
#define _IT6635X_EDIDADDR_3                         0
#endif

#ifndef _ENABLE_IT666X_TX_DEBUG_DELAY
#define _ENABLE_IT666X_TX_DEBUG_DELAY               _MACRO_DISABLE
#endif

#ifndef _ENABLE_MAIN_TASK_UART
#define _ENABLE_MAIN_TASK_UART                      _MACRO_ENABLE
#endif

#ifndef _ENABLE_MAIN_TASK_KEY
#define _ENABLE_MAIN_TASK_KEY                       _MACRO_ENABLE
#endif

#ifndef _ENABLE_MAIN_TASK_ETHERNET
#define _ENABLE_MAIN_TASK_ETHERNET                  _MACRO_DISABLE
#endif

#ifndef _ENABLE_SW4052_CS_PIN
#define _ENABLE_SW4052_CS_PIN		                _MACRO_DISABLE
#endif

#ifndef _HDMI_O_PORT_NUM
#define _HDMI_O_PORT_NUM                            1
#endif

#ifndef _HDMI_I_PORT_NUM
#define _HDMI_I_PORT_NUM                            1
#endif

#ifndef _CSC_AV_INFO_UART_PORT
#define _CSC_AV_INFO_UART_PORT                      _UART_PORT_NONE
#endif

#ifndef _ENABLE_HDCP_AUTO_BY_SINK
#define _ENABLE_HDCP_AUTO_BY_SINK                   _MACRO_DISABLE
#endif

#ifndef _ENABLE_CPLD_FUNCTION
#define _ENABLE_CPLD_FUNCTION		                _MACRO_DISABLE
#endif

#ifndef _EN_GPIO_EDID_MODE_SWITCH
#define _EN_GPIO_EDID_MODE_SWITCH                   _MACRO_DISABLE
#endif //add by wf8421

#ifndef _EN_GPIO_EDID_SELECT_SWITCH
#define _EN_GPIO_EDID_SELECT_SWITCH                 _MACRO_DISABLE
#endif //add by wf8421

#ifndef FUNCTION_MACRO_FROCE_RXEDID_BY
#define FUNCTION_MACRO_FROCE_RXEDID_BY              _MACRO_DISABLE
#endif

#ifndef _ENABLE_GSV_CHIP_DEBUG_MESSAGE
#define _ENABLE_GSV_CHIP_DEBUG_MESSAGE              _MACRO_DISABLE
#endif

#ifndef FUNCTION_MACRO_EN_DITHER_SETTING
#define FUNCTION_MACRO_EN_DITHER_SETTING            _MACRO_DISABLE
#endif //add by wf8421

#ifndef _ENABLE_EDID_DEFAULT_NOT_TO_SAME
#define _ENABLE_EDID_DEFAULT_NOT_TO_SAME            _MACRO_DISABLE
#endif

#ifndef _ENABLE_IT66021_OUTMODE_YUV444SDR
#define _ENABLE_IT66021_OUTMODE_YUV444SDR           _MACRO_DISABLE
#endif

#ifndef _ENABLE_MULTI_COMMAND_MATCH_FUNCTION
#define _ENABLE_MULTI_COMMAND_MATCH_FUNCTION        _MACRO_DISABLE
#endif

#ifndef MaxCmdlist
#define MaxCmdlist                                  20
#endif

#ifndef FW_VECT_OFFSET
#define FW_VECT_OFFSET                              0x0
#endif

#ifndef _ENABLE_SPI_FLASH
#define _ENABLE_SPI_FLASH                           _MACRO_DISABLE
#endif

#ifndef _ENABLE_SPI_FUNCTION
#define _ENABLE_SPI_FUNCTION                        ((_EN_CHIP_TM1623_USE==_MACRO_ENABLE)||(_ENABLE_CHIP_LM74HC595 == _MACRO_ENABLE)||(_ENABLE_CPLD_FUNCTION == _MACRO_ENABLE)||_ENABLE_SPI_FLASH)
#endif

#ifndef _ENABLE_TO_MATCHING_ALPU_C
#define _ENABLE_TO_MATCHING_ALPU_C                  _MACRO_DISABLE
#endif

#ifndef _I2C_TABLE_INDEX_ALPU_C_0
#define _I2C_TABLE_INDEX_ALPU_C_0                   0
#endif

#ifndef _ENABLE_CHIP_MS1826
#define _ENABLE_CHIP_MS1826                         _MACRO_DISABLE
#endif

#ifndef _I2C_TABLE_INDEX_MS1826_0
#define _I2C_TABLE_INDEX_MS1826_0                   0
#endif

#ifndef _I2C_TABLE_INDEX_MS1826_1
#define _I2C_TABLE_INDEX_MS1826_1                   1
#endif

#ifndef _I2C_TABLE_INDEX_MS1826_2
#define _I2C_TABLE_INDEX_MS1826_2                   2
#endif

#ifndef _I2C_TABLE_INDEX_MS1826_3
#define _I2C_TABLE_INDEX_MS1826_3                   3
#endif

#ifndef _I2C_TABLE_INDEX_MS1826_4
#define _I2C_TABLE_INDEX_MS1826_4                   4
#endif

#ifndef _I2C_TABLE_INDEX_MS1826_5
#define _I2C_TABLE_INDEX_MS1826_5                   5
#endif

#ifndef _ENABLE_TIMING_STRUCT_DEFINE
#define _ENABLE_TIMING_STRUCT_DEFINE                 (_MACRO_ENABLE&&\
                                                        (_ENABLE_CHIP_GSV2011\
                                                        ||_ENABLE_CHIP_MS1826))
#endif

#ifndef _ENABLE_CHIP_ITE66121
#define _ENABLE_CHIP_ITE66121                       _MACRO_DISABLE
#endif

#ifndef _I2C_TABLE_INDEX_IT66121_0
#define _I2C_TABLE_INDEX_IT66121_0                  0
#endif

#ifndef _I2C_TABLE_INDEX_IT66121_1
#define _I2C_TABLE_INDEX_IT66121_1                  1
#endif

#ifndef _I2C_TABLE_INDEX_IT66121_2
#define _I2C_TABLE_INDEX_IT66121_2                  2
#endif

#ifndef _I2C_TABLE_INDEX_IT66121_3
#define _I2C_TABLE_INDEX_IT66121_3                  3
#endif

#ifndef _ENABLE_CHIP_MS7200
#define _ENABLE_CHIP_MS7200                         _MACRO_DISABLE
#endif

#ifndef _I2C_TABLE_INDEX_MS7200_0
#define _I2C_TABLE_INDEX_MS7200_0                   0
#endif

#ifndef _I2C_TABLE_INDEX_MS7200_1
#define _I2C_TABLE_INDEX_MS7200_1                   1
#endif

#ifndef _I2C_TABLE_INDEX_MS7200_2
#define _I2C_TABLE_INDEX_MS7200_2                   2
#endif

#ifndef _I2C_TABLE_INDEX_MS7200_3
#define _I2C_TABLE_INDEX_MS7200_3                   3
#endif

#ifndef _I2C_TABLE_INDEX_MS7200_4
#define _I2C_TABLE_INDEX_MS7200_4                   4
#endif

/**********************************************************
定义是否支持嵌入式操作系统
**********************************************************/
#ifndef _SYSTEM_SUPPORT_RTOS
#define _SYSTEM_SUPPORT_RTOS					    1	/* 嵌入式操作系统支持开关 */
													//0：不支持
													//1：支持
#endif

#ifndef ENABLE_CARD_YBJ_RECORD
#define ENABLE_CARD_YBJ_RECORD                      0
#endif

#ifndef ENABLE_SERIES_LCD_FUNCTION
#define ENABLE_SERIES_LCD_FUNCTION                  _MACRO_DISABLE
#endif


#endif  //#ifndef __PROJECT_MACRO_DEFINE_COMMON_MANAGEMENT_H_






