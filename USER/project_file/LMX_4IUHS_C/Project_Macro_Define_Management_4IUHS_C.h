#ifndef __PROJECT_MACRO_DEFINE_MANAGEMENT_SCU61E_H_
#define __PROJECT_MACRO_DEFINE_MANAGEMENT_SCU61E_H_

//#define ENABLE_CARD_YBJ_RECORD                  1

/******************************************************************/
/**CHIPS*USE****/
/******************************************************************/
//use eeprom chip
#define	_CHIP_EEPROM_TYPE_USE							_EEPROM_CHIP_24C32
#define _ENABLE_CHIP_MS1826                             _MACRO_ENABLE
#define _ENABLE_TO_MATCHING_ALPU_C                      _MACRO_ENABLE

//#define _ENABLE_SPI_FLASH                               _MACRO_ENABLE
//#define _ENABLE_TO_MATCHING_ALPU_C                      _MACRO_ENABLE

//i2c index of table for eeprom
#define _I2C_TABLE_INDEX_EEPROM							eI2cDevice_Eeprom
#define _I2C_TABLE_INDEX_MS1826_0                       eI2cDevice_MS1826
#define _I2C_TABLE_INDEX_ALPU_C_0                       eI2cDevice_ALPU_C

/***RCC*OF*GPIO***/
//ALL THE RCC OF GPIO
#ifndef _RCC_APB2Periph_GPIO
#define _RCC_APB2Periph_GPIO							RCC_APB2Periph_GPIOA |\
														RCC_APB2Periph_GPIOB |\
														RCC_APB2Periph_GPIOC |\
														RCC_APB2Periph_GPIOD |\
														RCC_APB2Periph_AFIO
#endif
#define FW_VECT_OFFSET                                  0x0000

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
#define _STRING_FACTORY_MODEL					"LMX-4OUHS-C\r\n"
#define _STRING_FW_VERSION						"VERSION"
#if ENABLE_CARD_YBJ_RECORD
#define _STRING_FW_VER_NUM						"V1.0.0"
#else
#define _STRING_FW_VER_NUM						"V1.0.1"
#endif
//#define _STRING_CARD_TYPE				        "aEND"
//#define _STRING_CARD_TYPE_NUM			        "6END"
#define _STRING_CARD_TYPE				        "zEND"
#define _STRING_CARD_TYPE_NUM			        "6END"

#define DEF_MAIN_VER                            0xA5A7

//#define _EN_UART_DEBUG_PRINTF					        _MACRO_ENABLE
#define _ENABLE_GSV_CHIP_DEBUG_MESSAGE              _MACRO_ENABLE
//#define EANBLE_IT6635_DBG

#define _ENABLE_USER_PRINTF_FUNCTION            _MACRO_ENABLE
#define _ENABLE_SYSTEM_RESET_DELAY_TIME         _MACRO_ENABLE
//#define _ENABLE_KEYPAD_LOCKED_FUNCTION          _MACRO_ENABLE
#define _ENABLE_NEW_USER_KEY_DEFINE_FUNCTION    _MACRO_ENABLE
#define _ENABLE_UART_SEMAPHORE_FUNCTION_BY_PRINT    _MACRO_ENABLE
#define _ENABLE_MULTI_OUTPUT_CHIP			    _MACRO_ENABLE
//#define _ENABLE_EXTERNAL_CHIP_TASK              _MACRO_ENABLE   // Enable Task_ExternalChip.c
#define _ENABLE_CEC_FUNCTION_USE                _MACRO_DISABLE
#define _ENABLE_MULTI_COMMAND_MATCH_FUNCTION    _MACRO_ENABLE
#define _ENABLE_USER_EDID_LEARNING_FUNCTION     _MACRO_ENABLE
#define _ENABLE_EDID_SWITCH_A_SINGLE_CHANNEL    _MACRO_ENABLE

#define _ENABLE_UART_1                              _MACRO_ENABLE
#define _ENABLE_UART_3                              _MACRO_ENABLE

#define _ENABLE_RES_3840x2160_30                _MACRO_ENABLE
#define _ENABLE_RES_4096x2160_30                _MACRO_ENABLE

#define SIZE_UART_KEY_VALUE_MAX						200

#define NUM_OF_I2C_BUS_MAX				            (eI2cDevice_Max+2)
#define NUM_OF_SPI_BUS_MAX				            (eSpiDevice_Max+2)

#define Def_outport_num				            4
#define _HDMI_INPUT_PORT_USED_NUMBER            4

#define MS1826_OSD_ENABLE                       _MACRO_ENABLE
//#define MS1826_VIPSS1_DEINT                     _MACRO_ENABLE
//#define SEAMLESS_MATRIX_USE_DSC                 _MACRO_ENABLE
#define PRINTF_CHAR_MAX                         1024
#define MS1826_TX_DRIVER_USER_CONFIG_ENABLE     _MACRO_ENABLE


#endif
