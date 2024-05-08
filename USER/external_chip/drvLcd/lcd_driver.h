#ifndef __LCD_DRIVER_H_
#define __LCD_DRIVER_H_

extern vvFuncPoint pSeriesLcdInitFunc;
extern vvFuncPoint pSeriesLcdProcess;

void initial_lcd(void);
void lcd_main(void);
void transfer_command(unsigned char data);
void transfer_data(unsigned char data);
void display_string_8x16(uint page,uint column,unsigned char u8InverColor,unsigned char *text);
void display_graphic_128x64(unsigned char page,unsigned char column,unsigned char *dp);
void full_display(unsigned char data_left,unsigned char data_right);
void display_graphic_32x64(unsigned char page,unsigned char column,unsigned char *dp);
void display_graphic_32x32(unsigned char page,unsigned char column,unsigned char *dp);
void display_graphic_16x16(unsigned char page,unsigned char column,unsigned char *dp);
void display_graphic_8x16(unsigned char page,unsigned char column,unsigned char *u8data,unsigned char u8InverColor);
void display_string_5x8_1(uint page,uint column,unsigned char *text);
void clear_screen(void);
void display_line_update(unsigned char u8Addr,char *pString,unsigned char u8Len);

#endif /* __LCD_DRIVER_H_ */

