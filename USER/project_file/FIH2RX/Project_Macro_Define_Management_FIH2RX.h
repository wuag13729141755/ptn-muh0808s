#ifndef __PROJECT_MACRO_DEFINE_MANAGEMENT_SCU61E_H_
#define __PROJECT_MACRO_DEFINE_MANAGEMENT_SCU61E_H_

/******************************************************************/
/**CHIPS*USE****/
/******************************************************************/
#define	IT666X_VER                                      IT666X_NONE
#define	IT663XX_VER                                     IT663XX_NONE
//use eeprom chip
#define	_CHIP_EEPROM_TYPE_USE							_EEPROM_CHIP_24C32
#define _ENABLE_CHIP_ITE66021                           _MACRO_ENABLE
#define IT66021_DEV_NUM                                 1
#define _ENABLE_CHIP_GSV2011                            _MACRO_ENABLE
#define AvGsv2k11DeviceNumber                           2
#define _ENABLE_CHIP_IDT5P49V6965                       _MACRO_ENABLE
#define DEV_IDT5V_NUM                                   1
#define _ENABLE_CHIP_GSV5100                            _MACRO_ENABLE
#define AvGsv5k1DeviceNumber                            1

//#define _ENABLE_SPI_FLASH                               _MACRO_ENABLE
//#define _ENABLE_TO_MATCHING_ALPU_C                      _MACRO_ENABLE

//i2c index of table for eeprom
#define _I2C_TABLE_INDEX_EEPROM							eI2cDevice_Eeprom
#define _I2C_TABLE_INDEX_GSV_2K11_0                     eI2cDevice_GSV2011_1
#define _I2C_TABLE_INDEX_GSV_2K11_1                     eI2cDevice_GSV2011_2
#define _I2C_TABLE_INDEX_IT660XX_0                      eI2cDevice_IT66021
#define _I2C_TABLE_INDEX_IDT5V49_0                      eI2cDevice_IDT5V49EE902
#define _I2C_TABLE_INDEX_ALPU_C_0                       eI2cDevice_ALPU_C
#define _I2C_TABLE_INDEX_GSV_5K1_0                      eI2cDevice_GSV5100

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

#define _NUM_UART_STUDY_KEYS					uart_study_key_type_max

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
#define _STRING_FACTORY_MODEL					"F1H2RX\r\n"
#define _STRING_FW_VERSION						"VERSION"
#define _STRING_FW_VER_NUM						"v1.0.2\r\n"
//#define _STRING_TEST_FW_VERSION				    "TEST VERSION T1.0.0.20230114\r\n"

#define DEF_MAIN_VER                            0xA5A8

//#define _EN_UART_DEBUG_PRINTF					        _MACRO_ENABLE
#define _ENABLE_GSV_CHIP_DEBUG_MESSAGE              _MACRO_ENABLE

#if _EN_UART_DEBUG_PRINTF
#define UART2_BAUDRATE                          115200
#else
#define UART2_BAUDRATE                          115200
#endif

#define _ENABLE_USER_PRINTF_FUNCTION            _MACRO_ENABLE
#define _ENABLE_SYSTEM_RESET_DELAY_TIME         _MACRO_ENABLE
#define _ENABLE_KEYPAD_LOCKED_FUNCTION          _MACRO_ENABLE
#define _ENABLE_NEW_USER_KEY_DEFINE_FUNCTION    _MACRO_ENABLE
#define _ENABLE_UART_SEMAPHORE_FUNCTION_BY_PRINT    _MACRO_ENABLE
//#define _ENABLE_MAIN_TASK_UART                  _MACRO_DISABLE
#define _ENABLE_EXTERNAL_CHIP_TASK              _MACRO_ENABLE   // Enable Task_ExternalChip.c
#define _ENABLE_CEC_FUNCTION_USE                _MACRO_DISABLE
#define _ENABLE_IT66021_OUTMODE_YUV444SDR       _MACRO_ENABLE
#define _ENABLE_MULTI_COMMAND_MATCH_FUNCTION    _MACRO_ENABLE

#define _ENABLE_RES_3840x2160_30                _MACRO_ENABLE
#define _ENABLE_RES_3840x2160_50                _MACRO_ENABLE
#define _ENABLE_RES_3840x2160_60                _MACRO_ENABLE

#define _DEBUG_PORT_SEL                         _UART_PORT_2

#define SIZE_UART_KEY_VALUE_MAX						200

#define NUM_OF_I2C_BUS_MAX				            (eI2cDevice_Max+2)
#define NUM_OF_SPI_BUS_MAX				            (eSpiDevice_Max+2)

//#define _4K_MODE

#ifdef _4K_MODE
#define AvHdmiOnEthernet                        0
#else
#define Gsv5k1UseFastEqMode                     1
#endif
#define AvGsv2k11ComBusCfg                      7
#define Gsv5k1RxSetTxBHdmiMode                  1
#define Gsv5k1WorkMode                          Gsv5k1RX
#define AvEnableAudioTTLInput                   1
#define Gsv5k1DisableTxAHdmi                    1
//#define RxHpdDelayExpireThreshold               0

//#define AvEnableInternalVideoGen                1
//#define AvForceDefaultEdid                      1
//#define AvGsv2kxTXSwitchHDMIAndDVI              (1)
//#define AvGsv2kxTXDVIFornatWithOutHDCP          (1)
#define RES_DEFAULT_MAX                         14

#define INPUT_CLOCK                             2700//2500

#ifndef AvGsvTxPhyValue_GTR340M
#define AvGsvTxPhyValue_GTR340M                 (0x34)
#endif

#ifndef AvGsvTxPhyValue_LEQ340M
#define AvGsvTxPhyValue_LEQ340M                 (0x50)
#endif

#ifndef STK_SIZE_TASK_GSV_CHIP
#define STK_SIZE_TASK_GSV_CHIP                      1024
#endif
#ifndef PRIORITY_TASK_GSV_CHIP
#define PRIORITY_TASK_GSV_CHIP                      3
#endif

#ifndef STK_SIZE_TASK_EXTERNAL_CHIP
#define STK_SIZE_TASK_EXTERNAL_CHIP                 256
#endif
#ifndef PRIORITY_TASK_EXTERNAL_CHIP
#define PRIORITY_TASK_EXTERNAL_CHIP                 2
#endif

#endif





