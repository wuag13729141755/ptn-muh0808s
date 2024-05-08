#ifndef __EEPROM_H_
#define __EEPROM_H_ 

#if (_CHIP_EEPROM_TYPE_USE != _EEPROM_CHIP_NO_USE)

#if (_EN_UART_DEBUG_PRINTF==_MACRO_ENABLE)
#define Msg_Eep(x)				Uart_printf x
#else
#define Msg_Eep(x)
#endif

#if (_CHIP_EEPROM_TYPE_USE == _EEPROM_CHIP_24CM01)
#define DEVICE_ADDR_AT24CM01				0xA0

#define AT24CM01_MAX_ADDR					131072
#define AT24CM01_PAGE_SIZE					256

#define AT24CM01_DELAY						5//the unit is ms
#elif (_CHIP_EEPROM_TYPE_USE == _EEPROM_CHIP_24C32)
#define DEVICE_ADDR_AT24CM01				0xA0

#define AT24CM01_MAX_ADDR					4096
#define AT24CM01_PAGE_SIZE					8

#define AT24CM01_DELAY						5//the unit is ms

#endif

void eeprom_write(uint32_t Address, uint8_t *pDatas, uint32_t length);
void eeprom_read(uint32_t Address, uint8_t *pDatas, uint32_t length);

#endif//#if (_CHIP_EEPROM_TYPE_USE != _EEPROM_CHIP_NO_USE)

#endif 



























