#ifndef __PROJECT_MACRO_DEFINE_MANAGEMENT_SCU61E_H_
#define __PROJECT_MACRO_DEFINE_MANAGEMENT_SCU61E_H_

/******************************************************************/
/**CHIPS*USE****/
/******************************************************************/
#define	IT666X_VER                                      IT666X_NONE
#define	IT663XX_VER                                     IT663XX_NONE
//use eeprom chip
#define	_CHIP_EEPROM_TYPE_USE							_EEPROM_CHIP_24C32
//#define _ENABLE_CHIP_ITE66021                           _MACRO_ENABLE
//#define IT66021_DEV_NUM                                 1
#define _ENABLE_CHIP_ITE66121                           _MACRO_ENABLE
#define IT66121_DEV_NUM                                 1
#define _ENABLE_CHIP_MS7200                             _MACRO_ENABLE

//#define _ENABLE_TO_MATCHING_ALPU_C                      _MACRO_ENABLE

//i2c index of table for eeprom
#define _I2C_TABLE_INDEX_EEPROM						    eI2cDevice_Eeprom
//#define _I2C_TABLE_INDEX_IT660XX_0                      eI2cDevice_IT66021_1
#define _I2C_TABLE_INDEX_MS7200_0                       eI2cDevice_IT66021_1
#define _I2C_TABLE_INDEX_IT66121_0                      eI2cDevice_IT66121_1

#define IT66121_PIN_ADDR_LOW0       0

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
#define _STRING_FACTORY_MODEL					"IMX_25_2VCTAC\r\n"
#define _STRING_FW_VERSION						"VERSION"
#define _STRING_FW_VER_NUM						"V1.0.0\r\n"
#define _STRING_TEST_FW_VERSION				    "TEST VERSION T1.0.0.20230131\r\n"

#define DEF_MAIN_VER                            0xA5A7

//#define _EN_UART_DEBUG_PRINTF					        _MACRO_ENABLE
#define _ENABLE_GSV_CHIP_DEBUG_MESSAGE              _MACRO_ENABLE
//#define EANBLE_IT6635_DBG

#define _ENABLE_UART_2                              _MACRO_ENABLE
#define _ENABLE_UART_3                              _MACRO_ENABLE

#define _ENABLE_USER_PRINTF_FUNCTION            _MACRO_ENABLE
#define _ENABLE_SYSTEM_RESET_DELAY_TIME         _MACRO_ENABLE
#define _ENABLE_KEYPAD_LOCKED_FUNCTION          _MACRO_ENABLE
#define _ENABLE_NEW_USER_KEY_DEFINE_FUNCTION    _MACRO_ENABLE
#define _ENABLE_UART_SEMAPHORE_FUNCTION_BY_PRINT    _MACRO_ENABLE
//#define _ENABLE_MAIN_TASK_UART                  _MACRO_DISABLE
#define _ENABLE_EXTERNAL_CHIP_TASK              _MACRO_ENABLE   // Enable Task_ExternalChip.c
#define _ENABLE_CEC_FUNCTION_USE                _MACRO_DISABLE
#define _ENABLE_MULTI_COMMAND_MATCH_FUNCTION    _MACRO_ENABLE
//#define HDMITX_AUTO_MONITOR_INPUT

#define _DEBUG_PORT_SEL                         _UART_PORT_2

#define SIZE_UART_KEY_VALUE_MAX						200

#define NUM_OF_I2C_BUS_MAX				            (eI2cDevice_Max+2)
#define NUM_OF_SPI_BUS_MAX				            (eSpiDevice_Max+2)

#define HDMIRX_OUTPUT_VID_MODE                  eYUV422_Sep_Sync_SDR
#define INPUT_YCBCR422_DATA_BIT                 20
#define IT66021_AUDIO_OUT                       I2S

#define CONFIG_INPUT_AUDIO_SPDIF                I2S

#ifndef MS7200_CS_MODE
#define MS7200_CS_MODE      DVOUT_CS_MODE_YUV422//refer to DVOUT_CS_MODE_E
#endif
#ifndef MS7200_BW_MODE
#define MS7200_BW_MODE      DVOUT_BW_MODE_16_24BIT//refer to DVOUT_BW_MODE_E
#endif
#ifndef MS7200_DR_MODE
#define MS7200_DR_MODE      DVOUT_DR_MODE_SDR//refer to DVOUT_DR_MODE_E
#endif
#ifndef MS7200_SY_MODE
#define MS7200_SY_MODE      DVOUT_SY_MODE_HSVSDE//DVOUT_SY_MODE_EMBANDED//refer to DVOUT_SY_MODE_E
#endif

#endif


