#ifndef __PROJECT_MACRO_DEFINE_MANAGEMENT_SCU61E_H_
#define __PROJECT_MACRO_DEFINE_MANAGEMENT_SCU61E_H_

/******************************************************************/
/**CHIPS*USE****/
/******************************************************************/
#define	IT666X_VER                                      IT666X_NONE
#define	IT663XX_VER                                     IT663XX_NONE//IT663XX_VER_1_30    //IT663XX_VER_1_21//
//use eeprom chip
#define	_CHIP_EEPROM_TYPE_USE							_EEPROM_CHIP_24C32
#define _ENABLE_CHIP_ITE66021                           _MACRO_ENABLE
#define IT66021_DEV_NUM                                 8

//i2c index of table for eeprom
#define _I2C_TABLE_INDEX_EEPROM							eI2cDevice_Eeprom


#define _I2C_TABLE_INDEX_IT660XX_0                     eI2cDevice_It66021_1
#define _I2C_TABLE_INDEX_IT660XX_1                     eI2cDevice_It66021_1
#define _I2C_TABLE_INDEX_IT660XX_2                     eI2cDevice_It66021_2
#define _I2C_TABLE_INDEX_IT660XX_3                     eI2cDevice_It66021_2
#define _I2C_TABLE_INDEX_IT660XX_4                     eI2cDevice_It66021_3
#define _I2C_TABLE_INDEX_IT660XX_5                     eI2cDevice_It66021_3
#define _I2C_TABLE_INDEX_IT660XX_6                     eI2cDevice_It66021_4
#define _I2C_TABLE_INDEX_IT660XX_7                     eI2cDevice_It66021_4

#define IT66021_PIN_ADDR_LOW0       1
#define IT66021_PIN_ADDR_LOW1       0
#define IT66021_PIN_ADDR_LOW2       1
#define IT66021_PIN_ADDR_LOW3       0
#define IT66021_PIN_ADDR_LOW4       1
#define IT66021_PIN_ADDR_LOW5       0
#define IT66021_PIN_ADDR_LOW6       1
#define IT66021_PIN_ADDR_LOW7       0


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
#define _STRING_FACTORY_MODEL					"MHD1616SRX"
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
#define _ENABLE_IT66021_OUTMODE_YUV444SDR       _MACRO_ENABLE
#define _ENABLE_EDID_SWITCH_A_SINGLE_CHANNEL    _MACRO_ENABLE


 // reset edid for input port
#define SIZE_UART_KEY_VALUE_MAX						200

#define Def_outport_num				                16
#define NUM_OF_I2C_BUS_MAX				            (eI2cDevice_Max+2)
#define NUM_OF_SPI_BUS_MAX				            (eSpiDevice_Max+2)

#define _HDMI_INPUT_PORT_USED_NUMBER                8
#endif





