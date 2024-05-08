#ifndef __PROJECT_MACRO_DEFINE_MANAGEMENT_SCU61E_H_
#define __PROJECT_MACRO_DEFINE_MANAGEMENT_SCU61E_H_

/******************************************************************/
/**CHIPS*USE****/
/******************************************************************/
#define	IT666X_VER                                      IT666X_NONE
#define	IT663XX_VER                                     IT663XX_NONE    //IT663XX_VER_1_21//
//#define _EN_CHIP_ADN4604_USE                            _MACRO_ENABLE
#define _EN_CHIP_TM1623_USE                             _MACRO_ENABLE
#define DEV_NUM_TM1623                                  2

//use eeprom chip
#define	_CHIP_EEPROM_TYPE_USE							_EEPROM_CHIP_24CM01

//i2c index of table for eeprom
#define _I2C_TABLE_INDEX_EEPROM						    eI2cDevice_Eeprom

//eSpiDevice_W5500,
#define _SPI_TABLE_INDEX_TM1623			                eSpiDevice_TM1623_1
#define _SPI_TABLE_INDEX_TM1623_1			            eSpiDevice_TM1623_2

/******************************************************************/
/**BOARD*GPIO****/
/******************************************************************/
//use gpio usb detect
//#define _EN_GPIO_USB_DETECT							_MACRO_ENABLE

/***RCC*OF*GPIO***/
//ALL THE RCC OF GPIO
#ifndef _RCC_APB2Periph_GPIO
#define _RCC_APB2Periph_GPIO							RCC_APB2Periph_GPIOA |\
														RCC_APB2Periph_GPIOB |\
														RCC_APB2Periph_GPIOC |\
														RCC_APB2Periph_GPIOD |\
														RCC_APB2Periph_AFIO
#endif

#define FW_VECT_OFFSET                                  0x10000

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

//save hdmi hdcp on/off status
//#define _EN_EEPROM_FUN_SAVE_HDCP_STATUS			_MACRO_ENABLE


//MANUFACTURER LOGO
#define _STRING_MANF_NAME 						    "\r\n"
#define _STRING_FACTORY_MODEL					    "IMX_25_2VCTAP"
#define _STRING_FW_VERSION						    "VERSION"
#define _STRING_FW_VER_NUM						    "v1.0.0"
#define _STRING_HW_VER_NUM						    "v1.0.0"

#define DEF_MAIN_VER                            0xA5A7

//#define _EN_UART_DEBUG_PRINTF					        _MACRO_ENABLE

#if _EN_UART_DEBUG_PRINTF
#define UART2_BAUDRATE                          115200
#else
#define UART2_BAUDRATE                          115200
#endif

#define _ENABLE_USER_PRINTF_FUNCTION            _MACRO_ENABLE
#define _EN_EEPROM_FUN_SAVE_SYS_EDID_INDEX		_MACRO_ENABLE
#define _ENABLE_SYSTEM_RESET_DELAY_TIME         _MACRO_ENABLE
#define _ENABLE_KEYPAD_LOCKED_FUNCTION          _MACRO_ENABLE
#define _ENABLE_NEW_USER_KEY_DEFINE_FUNCTION    _MACRO_ENABLE
#define _ENABLE_UART_SEMAPHORE_FUNCTION_BY_PRINT    _MACRO_ENABLE
#define _ENABLE_EXTERNAL_CHIP_TASK              _MACRO_ENABLE   // Enable Task_ExternalChip.c
#define _ENABLE_CEC_FUNCTION_USE                _MACRO_DISABLE
#define _ENABLE_MULTI_OUTPUT_CHIP			    _MACRO_ENABLE
#define _ENABLE_MULTI_COMMAND_MATCH_FUNCTION    _MACRO_ENABLE
//#define _ENABLE_EDID_SWITCH_A_SINGLE_CHANNEL    _MACRO_ENABLE
//#define _ENABLE_USER_EDID_LEARNING_FUNCTION     _MACRO_ENABLE
#define _ENABLE_MAIN_TASK_ETHERNET              _MACRO_ENABLE

#define _ENABLE_UART_2                              _MACRO_ENABLE
#define _ENABLE_UART_4                              _MACRO_ENABLE
//#define _ENABLE_UART_5                              _MACRO_ENABLE
//#define _ENABLE_UART_6                              _MACRO_ENABLE

//#define _DEBUG_PORT_SEL                         _UART_PORT_2

//#define Def_outport_num				            64
//#define _HDMI_INPUT_PORT_USED_NUMBER            64
//#define TM1623_PORT_TOTAL_NUMBER	0x13

#define NUM_OF_KEYS_MAX					40
#define NUM_OF_LEDS_MAX					125
#define NUM_OF_I2C_BUS_MAX				            (eI2cDevice_Max+2)
#define NUM_OF_SPI_BUS_MAX				            (eSpiDevice_Max+2)

//============================== FreeRtos ==============================================
#define STK_SIZE_TASK_EEPROM						512
#define PRIORITY_TASK_EEPROM						1

#if (_EN_GPIO_USB_DETECT == _MACRO_ENABLE)
#define STK_SIZE_TASK_USB_DETECT					64
#define PRIORITY_TASK_USB_DETECT					1
#endif

#define STK_SIZE_TASK_UART							2048
#define PRIORITY_TASK_UART							6

#define STK_SIZE_TASK_LED							128
#define PRIORITY_TASK_LED							4

#define STK_SIZE_TASK_KEY							128
#define PRIORITY_TASK_KEY							5

#define STK_SIZE_TASK_KEY_RESPONSE					256
#define PRIORITY_TASK_KEY_RESPONSE					7

#if _ENABLE_EXTERNAL_CHIP_TASK
#define STK_SIZE_TASK_EXTERNAL_CHIP					2048//256
#define PRIORITY_TASK_EXTERNAL_CHIP				    3
#endif

#endif





