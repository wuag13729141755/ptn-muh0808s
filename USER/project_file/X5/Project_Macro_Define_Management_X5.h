#ifndef __PROJECT_MACRO_DEFINE_MANAGEMENT_SCU61E_H_
#define __PROJECT_MACRO_DEFINE_MANAGEMENT_SCU61E_H_

/******************************************************************/
/**CHIPS*USE****/
/******************************************************************/
#define	IT666X_VER                                      IT666X_NONE
#define	IT663XX_VER                                     IT663XX_NONE//IT663XX_VER_1_30    //IT663XX_VER_1_21//
//use eeprom chip
#define	_CHIP_EEPROM_TYPE_USE							_EEPROM_CHIP_24C32

//i2c index of table for eeprom
#define _I2C_TABLE_INDEX_EEPROM							0

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
//#define _EN_EEPROM_FUN_SAVE_HDMI_SEL			_MACRO_ENABLE

//save hdmi auto & manu switch mode
//#define _EN_EEPROM_FUN_SAVE_TYPE_SWITCH_MODE	_MACRO_ENABLE

//save uart study keys' value
//#define _EN_EEPROM_FUN_SAVE_UART_STUDY_KEYS		_MACRO_ENABLE

//save hdmi hdcp on/off status
//#define _EN_EEPROM_FUN_SAVE_HDCP_STATUS			_MACRO_ENABLE


#define _IR_FUNCTION_USE                            _IR_FUNCTION_KEY_STUDY
#define _IR_SENSOR_IN_PORT_USE                      _IR_SENSOR_IN_GPIOA1


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

//auto switch detect time
#define _TIME_AUTO_SWITCH_DETECT				500//uint is 1ms


//hdmi input port used number
#define _HDMI_INPUT_PORT_USED_NUMBER			3


//MANUFACTURER LOGO
#define _STRING_MANF_NAME 						"\r\n"
#define _STRING_FACTORY_MODEL					"X5\r\n"
#define _STRING_FW_VERSION						"VERSION"
#define _STRING_FW_VER_NUM						"V1.0.0\r\n"
#define _STRING_TEST_FW_VERSION				    "TEST VERSION T1.0.0.190912\r\n"

#define DEF_MAIN_VER                            0xA5A7

//#define _EN_UART_DEBUG_PRINTF					        _MACRO_ENABLE

#if _EN_UART_DEBUG_PRINTF
#define UART2_BAUDRATE                          115200
#else
#define UART2_BAUDRATE                          115200
#endif

#define _ENABLE_USER_PRINTF_FUNCTION            _MACRO_ENABLE
#define _ENABLE_SYSTEM_RESET_DELAY_TIME         _MACRO_ENABLE
//#define _ENABLE_KEYPAD_LOCKED_FUNCTION          _MACRO_ENABLE
#define _ENABLE_NEW_USER_KEY_DEFINE_FUNCTION    _MACRO_ENABLE
#define _ENABLE_UART_SEMAPHORE_FUNCTION_BY_PRINT    _MACRO_ENABLE
//#define _ENABLE_MAIN_TASK_UART                  _MACRO_DISABLE
#define _EN_MAIN_TASK_LED                       _MACRO_DISABLE
#define _EN_MAIN_TASK_EEPROM                    _MACRO_DISABLE
#define _ENABLE_CEC_FUNCTION_USE                _MACRO_DISABLE
#define ENABLE_IR_COUNTED_USE_TIMER_CLOCK_50US  _MACRO_ENABLE

 // reset edid for input port
#define SIZE_UART_KEY_VALUE_MAX						200

#if (_IR_FUNCTION_USE != _IR_FUNCTION_NONE)
#define STK_SIZE_TASK_IR                            512
#define PRIORITY_TASK_IR                            5
#endif

#endif





