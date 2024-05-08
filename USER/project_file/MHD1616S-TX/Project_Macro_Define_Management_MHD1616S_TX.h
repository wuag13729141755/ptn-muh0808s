#ifndef __PROJECT_MACRO_DEFINE_MANAGEMENT_SCU61E_H_
#define __PROJECT_MACRO_DEFINE_MANAGEMENT_SCU61E_H_

/******************************************************************/
/**CHIPS*USE****/
/******************************************************************/
#define	IT666X_VER                                      IT666X_NONE
#define	IT663XX_VER                                     IT663XX_NONE//IT663XX_VER_1_30    //IT663XX_VER_1_21//
//#define _ENABLE_CHIP_ITE66351                       _MACRO_ENABLE
//use eeprom chip
#define	_CHIP_EEPROM_TYPE_USE							_EEPROM_CHIP_24C32
#define _ENABLE_CHIP_GSV2011                            _MACRO_ENABLE
#define AvGsv2k11DeviceNumber                           16
#define _ENABLE_CHIP_IDT5V49EE902                       _MACRO_ENABLE
#define DEV_IDT5V_NUM                                   2

#define _EN_CHIP_PCA9539_USE                            _MACRO_ENABLE

//i2c index of table for eeprom
#define _I2C_TABLE_INDEX_EEPROM							eI2cDevice_Eeprom

#define _I2C_TABLE_INDEX_PCA9539                        eI2cDevice_PCA9539

#define _I2C_TABLE_INDEX_GSV_2K11_0                     eI2cDevice_Gsv1016_1
#define _I2C_TABLE_INDEX_GSV_2K11_1                     eI2cDevice_Gsv1016_2
#define _I2C_TABLE_INDEX_GSV_2K11_2                     eI2cDevice_Gsv1016_3
#define _I2C_TABLE_INDEX_GSV_2K11_3                     eI2cDevice_Gsv1016_4
#define _I2C_TABLE_INDEX_GSV_2K11_4                     eI2cDevice_Gsv1016_5
#define _I2C_TABLE_INDEX_GSV_2K11_5                     eI2cDevice_Gsv1016_6
#define _I2C_TABLE_INDEX_GSV_2K11_6                     eI2cDevice_Gsv1016_7
#define _I2C_TABLE_INDEX_GSV_2K11_7                     eI2cDevice_Gsv1016_8
#define _I2C_TABLE_INDEX_GSV_2K11_8                     eI2cDevice_Gsv1016_9
#define _I2C_TABLE_INDEX_GSV_2K11_9                     eI2cDevice_Gsv1016_10
#define _I2C_TABLE_INDEX_GSV_2K11_10                    eI2cDevice_Gsv1016_11
#define _I2C_TABLE_INDEX_GSV_2K11_11                    eI2cDevice_Gsv1016_12
#define _I2C_TABLE_INDEX_GSV_2K11_12                    eI2cDevice_Gsv1016_13
#define _I2C_TABLE_INDEX_GSV_2K11_13                    eI2cDevice_Gsv1016_14
#define _I2C_TABLE_INDEX_GSV_2K11_14                    eI2cDevice_Gsv1016_15
#define _I2C_TABLE_INDEX_GSV_2K11_15                    eI2cDevice_Gsv1016_16

#define _I2C_TABLE_INDEX_IDT5V49_0                      eI2cDevice_IDT5V49_1
#define _I2C_TABLE_INDEX_IDT5V49_1                      eI2cDevice_IDT5V49_2

//type-c port input
#define _HDMI_INPUT_TYPE_C_PORT				        _HDMI_INPUT_4_BIT_INDEX


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

#define FW_VECT_OFFSET                                  0x4000

//#define _NUM_SIMUL_GPIO_PINS_ONE_KEY_MAX				4//the min val is 1

/******************************************************************/
/**BOARD*FUNCTION****/
/******************************************************************/
//save hdmi sel (auto & manu) mode input channel
#define _EN_EEPROM_FUN_SAVE_HDMI_SEL			_MACRO_ENABLE

//save hdmi auto & manu switch mode
#define _EN_EEPROM_FUN_SAVE_TYPE_SWITCH_MODE	_MACRO_ENABLE

//save uart study keys' value
#define _EN_EEPROM_FUN_SAVE_UART_STUDY_KEYS		_MACRO_ENABLE

//save hdmi hdcp on/off status
//#define _EN_EEPROM_FUN_SAVE_HDCP_STATUS			_MACRO_ENABLE


//#define _NUM_UART_STUDY_KEYS					uart_study_key_type_max

/***WATCH*DOG***/
//enable watch dog
//#define _EN_WATCH_DOG							_MACRO_ENABLE

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

#define BootWaitTime                            1

//MANUFACTURER LOGO
#define _STRING_MANF_NAME 						"\r\n"
#define _STRING_FACTORY_MODEL					"MHD1616STX"
#define _STRING_FW_VERSION						"VERSION"
#define _STRING_FW_VER_NUM						"V1.0.0"

#define DEF_MAIN_VER                            0xA5A7

//#define _ENABLE_GSV_CHIP_DEBUG_MESSAGE              _MACRO_ENABLE


#define _ENABLE_USER_PRINTF_FUNCTION            _MACRO_ENABLE
#define _ENABLE_SYSTEM_RESET_DELAY_TIME         _MACRO_ENABLE
#define _ENABLE_NEW_USER_KEY_DEFINE_FUNCTION    _MACRO_ENABLE
#define _ENABLE_UART_SEMAPHORE_FUNCTION_BY_PRINT    _MACRO_ENABLE
//#define _ENABLE_MAIN_TASK_UART                  _MACRO_DISABLE
#define _ENABLE_EXTERNAL_CHIP_TASK              _MACRO_ENABLE   // Enable Task_ExternalChip.c
#define _ENABLE_CEC_FUNCTION_USE                _MACRO_DISABLE
#define _ENABLE_MULTI_OUTPUT_CHIP			    _MACRO_ENABLE
#define _ENABLE_MULTI_COMMAND_MATCH_FUNCTION    _MACRO_ENABLE

#define AvEnableTTLAudioModeI2S                 1
#define AvEnableAudioTTLInput                   1
#define AvGsv2k11CopyTxEdid                     1

//#define _DEBUG_PORT_SEL                         _UART_PORT_2

 // reset edid for input port
#define SIZE_UART_KEY_VALUE_MAX						200

#define Def_outport_num				                16
#define _HDMI_INPUT_PORT_USED_NUMBER                16
#define NUM_OF_I2C_BUS_MAX				            (eI2cDevice_Max+2)
#define NUM_OF_SPI_BUS_MAX				            (eSpiDevice_Max+2)

#define INPUT_CLOCK                             2700//2500

#define AvGsvTxPhyValue_LEQ340M                 (0x30)

#endif





