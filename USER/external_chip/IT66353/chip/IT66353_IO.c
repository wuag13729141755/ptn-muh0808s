
#include "Datatype.h"

unsigned char g_i2c_dev_no;  // for 6635 evb

iTE_u1 i2c_read_byte( iTE_u8 address, iTE_u8 offset, iTE_u8 byteno, iTE_u8 *p_data, iTE_u8 device);
iTE_u1 i2c_write_byte( iTE_u8 address, iTE_u8 offset, iTE_u8 byteno, iTE_u8 *p_data, iTE_u8 device);

u8 it6635_i2c_write(u8 addr, u8 offset, u8 length, u8 *buffer)
{
    int result;
    result = i2c_write_byte(  addr, offset, length ,buffer, g_i2c_dev_no);
    if ( result == 0 )
    {
        return 1;
    }
    else
    {
        //printf("ERROR: it6635_i2c_write %02x %02x %02x\r\n", addr, offset, *buffer);
    }
    return 0;
}

u8 it6635_i2c_read(u8 addr, u8 offset, u8 length, u8 *buffer)
{
    int result;
    result = i2c_read_byte( addr, offset, length, buffer, g_i2c_dev_no);
    if ( result == 0 )
    {
        return 1;
    }
    else
    {
        //printf("ERROR: it6635_i2c_read %02x %02x %02x\r\n", addr, offset, length);
    }
    return 0;
}

void delay1ms(u16 ms)
{
    delay_ms(ms);
}

volatile u32 g_u32TickCount = 0;
void it6635_Timer_Task(void)
{
    g_u32TickCount++;
}

__tick get_tick_count(void)
{
    return g_u32TickCount;
}


