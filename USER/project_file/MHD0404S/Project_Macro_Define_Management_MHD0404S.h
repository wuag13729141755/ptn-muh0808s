#ifndef __PROJECT_MACRO_DEFINE_MANAGEMENT_SCU61E_H_
#define __PROJECT_MACRO_DEFINE_MANAGEMENT_SCU61E_H_

/******************************************************************/
/**CHIPS*USE****/
/******************************************************************/
#define	IT666X_VER                                      IT666X_NONE
#define	IT663XX_VER                                     IT663XX_NONE    //IT663XX_VER_1_21//
#define _ENABLE_CHIP_ITE66021                           _MACRO_ENABLE
#define IT66021_DEV_NUM                                 4
#define _ENABLE_CHIP_GSV2011                            _MACRO_ENABLE
#define AvGsv2k11DeviceNumber                           4
#define _ENABLE_CHIP_IDT5V49EE902                       _MACRO_ENABLE
#define DEV_IDT5V_NUM                                   1
#define _ENABLE_SPI_FLASH                               _MACRO_ENABLE
#define _ENABLE_TO_MATCHING_ALPU_C                      _MACRO_ENABLE
//#define _EN_CHIP_TM1623_USE                             _MACRO_ENABLE

//use eeprom chip
#define	_CHIP_EEPROM_TYPE_USE							_EEPROM_CHIP_24CM01

//i2c index of table for eeprom
#define _I2C_TABLE_INDEX_EEPROM						    eI2cDevice_Eeprom
#define _I2C_TABLE_INDEX_IT660XX_0                      eI2cDevice_IT66021_1
#define _I2C_TABLE_INDEX_IT660XX_1                      eI2cDevice_IT66021_1
#define _I2C_TABLE_INDEX_IT660XX_2                      eI2cDevice_IT66021_2
#define _I2C_TABLE_INDEX_IT660XX_3                      eI2cDevice_IT66021_2
#define _I2C_TABLE_INDEX_GSV_2K11_0                     eI2cDevice_Gsv1016_1
#define _I2C_TABLE_INDEX_GSV_2K11_1                     eI2cDevice_Gsv1016_2
#define _I2C_TABLE_INDEX_GSV_2K11_2                     eI2cDevice_Gsv1016_3
#define _I2C_TABLE_INDEX_GSV_2K11_3                     eI2cDevice_Gsv1016_4
#define _I2C_TABLE_INDEX_IDT5V49_0                      eI2cDevice_IDT5V49EE902
#define _I2C_TABLE_INDEX_ALPU_C_0                       eI2cDevice_ALPU_C


#define IT66021_PIN_ADDR_LOW0       1
#define IT66021_PIN_ADDR_LOW1       0
#define IT66021_PIN_ADDR_LOW2       1
#define IT66021_PIN_ADDR_LOW3       0

#define _SPI_TABLE_INDEX_FLASH			        0

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
#define _STRING_FACTORY_MODEL					    "MHD0404S"
#define _STRING_FW_VERSION						    "VERSION"
#define _STRING_FW_VER_NUM						    "V1.0.0"

#define DEF_MAIN_VER                            0xA5A7

//#define _EN_UART_DEBUG_PRINTF					    _MACRO_ENABLE
#define _ENABLE_GSV_CHIP_DEBUG_MESSAGE              _MACRO_ENABLE

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
#define _ENABLE_EDID_SWITCH_A_SINGLE_CHANNEL    _MACRO_ENABLE
#define _ENABLE_USER_EDID_LEARNING_FUNCTION     _MACRO_ENABLE
#define _ENABLE_IT66021_OUTMODE_YUV444SDR       _MACRO_ENABLE

#define _ENABLE_UART_4                              _MACRO_ENABLE
#define _ENABLE_UART_5                              _MACRO_ENABLE
#define _ENABLE_UART_6                              _MACRO_ENABLE

//#define _DEBUG_PORT_SEL                         _UART_PORT_2

#define Def_outport_num				            4
#define _HDMI_INPUT_PORT_USED_NUMBER            4

#define NUM_OF_I2C_BUS_MAX				            (eI2cDevice_Max+2)
#define NUM_OF_SPI_BUS_MAX				            (eSpiDevice_Max+2)

#define Def_UserEdidMode                        EDID_MODE_INTERNAL6_1920x1080_60

#define INPUT_CLOCK                             2700//2500

//#define AvEnableTTLAudioModeI2S                 1
#define AvEnableAudioTTLInput                   1
#define AvGsv2k11CopyTxEdid                     1
//#define AvEnableForceOutHdcp                    1
//#define AvGsvTxPhyValue_LEQ340M                 (0x30)
#define AvGsv2kxTXSwitchHDMIAndDVI              (1)
#define AvGsv2kxTXDVIFornatWithOutHDCP          (1)

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





