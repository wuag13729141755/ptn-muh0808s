#ifndef __PROJECT_MACRO_DEFINE_MANAGEMENT_SCU61E_H_
#define __PROJECT_MACRO_DEFINE_MANAGEMENT_SCU61E_H_

/******************************************************************/
/**CHIPS*USE****/
/******************************************************************/
//use eeprom chip
#define	_CHIP_EEPROM_TYPE_USE							_EEPROM_CHIP_24CM01
#define _ENABLE_CHIP_MS1826                             _MACRO_ENABLE
#define _ENABLE_TO_MATCHING_ALPU_C                      _MACRO_ENABLE
#define DEV_NUM_MS1826                                  4

//i2c index of table for eeprom
#define _I2C_TABLE_INDEX_EEPROM							eI2cDevice_Eeprom
#define _I2C_TABLE_INDEX_MS1826_0                       eI2cDevice_MS1826_1
#define _I2C_TABLE_INDEX_MS1826_1                       eI2cDevice_MS1826_2
#define _I2C_TABLE_INDEX_MS1826_2                       eI2cDevice_MS1826_3
#define _I2C_TABLE_INDEX_MS1826_3                       eI2cDevice_MS1826_4
#define _I2C_TABLE_INDEX_ALPU_C_0                       eI2cDevice_ALPU_C

#define _MS1826_WORK_MODE_0                             MS1826_SEAMLESS_MATRIX_SWITCHER_OUT
#define _MS1826_WORK_MODE_1                             MS1826_SEAMLESS_MATRIX_SWITCHER_OUT
#define _MS1826_WORK_MODE_2                             MS1826_SEAMLESS_MATRIX_SWITCHER_OUT
#define _MS1826_WORK_MODE_3                             MS1826_SEAMLESS_MATRIX_SWITCHER_OUT

/***RCC*OF*GPIO***/
//ALL THE RCC OF GPIO
#ifndef _RCC_APB2Periph_GPIO
#ifdef N32G45X
#define _RCC_APB2Periph_GPIO							RCC_APB2_PERIPH_GPIOA |\
                                                        RCC_APB2_PERIPH_GPIOB |\
                                                        RCC_APB2_PERIPH_GPIOC |\
                                                        RCC_APB2_PERIPH_GPIOD |\
                                                        RCC_APB2_PERIPH_AFIO
#else
#define _RCC_APB2Periph_GPIO							RCC_APB2Periph_GPIOA |\
                                                        RCC_APB2Periph_GPIOB |\
                                                        RCC_APB2Periph_GPIOC |\
                                                        RCC_APB2Periph_GPIOD |\
                                                        RCC_APB2Periph_AFIO
#endif
#endif
#ifdef DEBUG_MODE_EN
#define FW_VECT_OFFSET                                  0x0000
#else
#define FW_VECT_OFFSET                                  0x4000
#endif

#define _NUM_SIMUL_GPIO_PINS_ONE_KEY_MAX				4//the min val is 1

/******************************************************************/
/**BOARD*FUNCTION****/
/******************************************************************/
//save hdmi sel (auto & manu) mode input channel
#define _EN_EEPROM_FUN_SAVE_HDMI_SEL			_MACRO_ENABLE

//save hdmi auto & manu switch mode
#define _EN_EEPROM_FUN_SAVE_TYPE_SWITCH_MODE	_MACRO_ENABLE

//save uart study keys' value
#define _EN_EEPROM_FUN_SAVE_UART_STUDY_KEYS		_MACRO_ENABLE

/******************************************************************/
/***TIME*OF*SYSTEM*RUN***/
/******************************************************************/
//led normal blink toggle time
#define _TIME_INDICATIVE_LED_NORM_BLINK_TOGGLE	25//unit = 20ms, depends on the delay time of task_led

//led fast blink toggle time
#define _TIME_INDICATIVE_LED_FAST_BLINK_TOGGLE	5//unit = 20ms

//led slow blink toggle time
#define _TIME_INDICATIVE_LED_SLOW_BLINK_TOGGLE	50//unit = 20ms

//led moment on time
#define _TIME_INDICATIVE_LED_MOMENT_ON			50//unit = 20ms

//led moment off time
#define _TIME_INDICATIVE_LED_MOMENT_OFF			50//unit = 20ms

//led moment toggle time
#define _TIME_INDICATIVE_LED_MOMENT_TOGGLE		10//unit = 20ms

//MANUFACTURER LOGO
#define _STRING_MANF_NAME 						"\r\n"
#define _STRING_FACTORY_MODEL					"MUH1616S_OUTPUT"
#define _STRING_FW_VERSION						"VERSION"
#define _STRING_FW_VER_NUM						"v1.0.0"

#define DEF_MAIN_VER                            0xA5A5

//#define _EN_UART_DEBUG_PRINTF					        _MACRO_ENABLE
#define _ENABLE_GSV_CHIP_DEBUG_MESSAGE              _MACRO_ENABLE
//#define EANBLE_IT6635_DBG

//#define _ENABLE_UART_1                              _MACRO_ENABLE
//#define _ENABLE_UART_2                              _MACRO_ENABLE
#define _ENABLE_UART_3                              _MACRO_ENABLE
//#define _ENABLE_UART_4                              _MACRO_ENABLE
#define _ENABLE_UART_5                              _MACRO_ENABLE
//#define _ENABLE_UART_6                              _MACRO_ENABLE
//#define _ENABLE_UART_3_PD8_PD9                      _MACRO_ENABLE

#define _ENABLE_USER_PRINTF_FUNCTION            _MACRO_ENABLE
#define _ENABLE_SYSTEM_RESET_DELAY_TIME         _MACRO_ENABLE
//#define _ENABLE_KEYPAD_LOCKED_FUNCTION          _MACRO_ENABLE
#define _ENABLE_NEW_USER_KEY_DEFINE_FUNCTION    _MACRO_ENABLE
#define _ENABLE_UART_SEMAPHORE_FUNCTION_BY_PRINT    _MACRO_ENABLE
#define _ENABLE_MULTI_OUTPUT_CHIP			    _MACRO_ENABLE
//#define _ENABLE_EXTERNAL_CHIP_TASK              _MACRO_ENABLE   // Enable Task_ExternalChip.c
#define _ENABLE_CEC_FUNCTION_USE                _MACRO_ENABLE
#define _ENABLE_MULTI_COMMAND_MATCH_FUNCTION    _MACRO_ENABLE
#define _ENABLE_USER_EDID_LEARNING_FUNCTION     _MACRO_ENABLE
#define _ENABLE_EDID_SWITCH_A_SINGLE_CHANNEL    _MACRO_ENABLE
//#define _SYSTEM_SUPPORT_RTOS					0
//#define _ENABLE_USER_DEFINE_CEC_CMD_TO_DEVICE   _MACRO_ENABLE
//#define ENABLE_SERIES_LCD_FUNCTION              _MACRO_ENABLE
//#define _ENABLE_MAIN_TASK_ETHERNET              _MACRO_ENABLE

#define _ENABLE_RES_3840x2160_30                _MACRO_ENABLE
#define _ENABLE_RES_4096x2160_30                _MACRO_ENABLE

//#define _DEBUG_PORT_SEL                         _UART_PORT_2

#define NUM_OF_I2C_BUS_MAX				            (eI2cDevice_Max+2)
#define NUM_OF_SPI_BUS_MAX				            (eSpiDevice_Max+2)
#define NUM_OF_LEDS_MAX					            27
#define NUM_OF_CEC_PINS_MAX				            2

#define Def_outport_num				            16
#define _HDMI_INPUT_PORT_USED_NUMBER            16
//#define ENABLE_MS1826_FRAME_LOCK                1


#define STK_SIZE_TASK_EEPROM						1024
#define PRIORITY_TASK_EEPROM						6

#define STK_SIZE_TASK_UART                          2048
#define PRIORITY_TASK_UART                          6

#define STK_SIZE_TASK_CEC                           128
#define PRIORITY_TASK_CEC                           7

#define STK_SIZE_TASK_LED                           256
#define PRIORITY_TASK_LED                           3

#define STK_SIZE_TASK_KEY                           128
#define PRIORITY_TASK_KEY                           4

#define STK_SIZE_TASK_MS1826                        2048
#define PRIORITY_TASK_MS1826                        6

#define STK_SIZE_TASK_EXTERNAL_CHIP                 256
#define PRIORITY_TASK_EXTERNAL_CHIP                 3

#endif
