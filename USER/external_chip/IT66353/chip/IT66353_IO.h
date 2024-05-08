
#ifndef __IT6635_IO_H__
#define __IT6635_IO_H__

u8 it6635_i2c_write(u8 addr, u8 offset, u8 length, u8 *buffer);
u8 it6635_i2c_read(u8 addr, u8 offset, u8 length, u8 *buffer);
void delay1ms(u16 ms);
void it6635_Timer_Task(void);
__tick get_tick_count(void);

#endif

