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
#define _EN_CHIP_TM1623_USE                             _MACRO_ENABLE
#define DEV_NUM_TM1623                                  1
#define _EN_CHIP_LT8644_USE                             _MACRO_ENABLE
#define DEV_NUM_LT8644                                  2


//i2c index of table for eeprom
#define _I2C_TABLE_INDEX_EEPROM							eI2cDevice_Eeprom
#define _I2C_TABLE_INDEX_MS1826_0                       eI2cDevice_MS1826_1
#define _I2C_TABLE_INDEX_MS1826_1                       eI2cDevice_MS1826_2
#define _I2C_TABLE_INDEX_MS1826_2                       eI2cDevice_MS1826_3
#define _I2C_TABLE_INDEX_MS1826_3                       eI2cDevice_MS1826_4
#define _I2C_TABLE_INDEX_ALPU_C_0                       eI2cDevice_ALPU_C
#define _I2C_TABLE_INDEX_LT8644_0                       eI2cDevice_LT8644_1
#define _I2C_TABLE_INDEX_LT8644_1                       eI2cDevice_LT8644_2

#define _MS1826_WORK_MODE_0                             MATRIX_SWITCHER
#define _MS1826_WORK_MODE_1                             MATRIX_SWITCHER
#define _MS1826_WORK_MODE_2                             MATRIX_SWITCHER_OUT
#define _MS1826_WORK_MODE_3                             MATRIX_SWITCHER_OUT

#define _SPI_TABLE_INDEX_TM1623			                eSpiDevice_TM1623_1

/***RCC*OF*GPIO***/
//ALL THE RCC OF GPIO
#ifndef _RCC_APB2Periph_GPIO
#define _RCC_APB2Periph_GPIO							RCC_APB2Periph_GPIOA |\
														RCC_APB2Periph_GPIOB |\
														RCC_APB2Periph_GPIOC |\
														RCC_APB2Periph_GPIOD |\
														RCC_APB2Periph_AFIO
#endif
#ifdef DEBUG_MODE_EN
#define FW_VECT_OFFSET                                  0x0000
#else
#define FW_VECT_OFFSET                                  0x10000
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
#define _STRING_FACTORY_MODEL					"MUH0808S"
#define _STRING_FW_VERSION						"VERSION"
#define _STRING_FW_VER_NUM						"v1.0.1"

#define DEF_MAIN_VER                            0xA5A8

//#define _EN_UART_DEBUG_PRINTF					        _MACRO_ENABLE

#define _ENABLE_UART_1                              _MACRO_ENABLE
#define _ENABLE_UART_2                              _MACRO_ENABLE
#define _ENABLE_UART_3                              _MACRO_ENABLE
#define _ENABLE_UART_4                              _MACRO_ENABLE
#define _ENABLE_UART_5                              _MACRO_ENABLE
//#define _ENABLE_UART_6                              _MACRO_ENABLE
#define _ENABLE_UART_3_PD8_PD9                      _MACRO_ENABLE

#define _ENABLE_USER_PRINTF_FUNCTION            _MACRO_ENABLE
#define _ENABLE_SYSTEM_RESET_DELAY_TIME         _MACRO_ENABLE
//#define _ENABLE_KEYPAD_LOCKED_FUNCTION          _MACRO_ENABLE
#define _ENABLE_NEW_USER_KEY_DEFINE_FUNCTION    _MACRO_ENABLE
#define _ENABLE_UART_SEMAPHORE_FUNCTION_BY_PRINT    _MACRO_ENABLE
#define _ENABLE_MULTI_OUTPUT_CHIP			    _MACRO_ENABLE
#define _ENABLE_EXTERNAL_CHIP_TASK              _MACRO_ENABLE   // Enable Task_ExternalChip.c
#define _ENABLE_CEC_FUNCTION_USE                _MACRO_ENABLE
#define _ENABLE_MULTI_COMMAND_MATCH_FUNCTION    _MACRO_ENABLE
#define _ENABLE_USER_EDID_LEARNING_FUNCTION     _MACRO_ENABLE
#define _ENABLE_EDID_SWITCH_A_SINGLE_CHANNEL    _MACRO_ENABLE
//#define _SYSTEM_SUPPORT_RTOS					0
#define _ENABLE_USER_DEFINE_CEC_CMD_TO_DEVICE   _MACRO_ENABLE
#define ENABLE_SERIES_LCD_FUNCTION              _MACRO_ENABLE
//#define _ENABLE_MAIN_TASK_ETHERNET              _MACRO_ENABLE
#define _EN_EEPROM_FUN_SAVE_SYS_EDID_INDEX		_MACRO_ENABLE

#define _ENABLE_RES_3840x2160_30                _MACRO_ENABLE
#define _ENABLE_RES_4096x2160_30                _MACRO_ENABLE

//#define _DEBUG_PORT_SEL                         _UART_PORT_2

#define SIZE_UART_KEY_VALUE_MAX						200

#define NUM_OF_I2C_BUS_MAX				            (eI2cDevice_Max+2)
#define NUM_OF_SPI_BUS_MAX				            (eSpiDevice_Max+2)
#define NUM_OF_LEDS_MAX					            30
#define NUM_OF_CEC_PINS_MAX				            2

#define Def_outport_num				            8
#define _HDMI_INPUT_PORT_USED_NUMBER            8
//#define ENABLE_MS1826_FRAME_LOCK                1
#define MS1826_OSD_ENABLE                       _MACRO_ENABLE
#define MS1826_SPLITTER_USER_MODE_ENABLE        _MACRO_ENABLE


#define STK_SIZE_TASK_EEPROM						512
#define PRIORITY_TASK_EEPROM						3


#endif