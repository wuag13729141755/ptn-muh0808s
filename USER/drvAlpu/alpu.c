#include "includes.h"
#if _ENABLE_TO_MATCHING_ALPU_C
//#include "alpu.h"

u8 g_u8AlpuDeviceID  = _I2C_TABLE_INDEX_ALPU_C_0;
unsigned char DEVICE_ADDRESS=0x7a;
//The DEVICE ADDRESS is modified to 0x3d, if you use the 7bit address without Write/Read flag
void _alpu_delay_ms(unsigned int i);

#if 0
unsigned char dec_tx_data[16];
unsigned char dec_rx_data[16];
unsigned char dec_dx_data[8];
unsigned char buffer_data[8];

void _alpuc_bypass(void)
{
    int i;

    for ( i=0; i<8; i++ )
    {
        dec_dx_data[i] = dec_tx_data[i] ^ 0x01;
    }
}
#endif

unsigned char _i2c_write(unsigned char device_addr, unsigned char sub_addr, unsigned char *buff, int ByteNo)
{
    u8 l_u8Ret = 0;
    l_u8Ret = i2c_write_byte(device_addr,sub_addr,ByteNo,buff,g_u8AlpuDeviceID);
//	l_u8Ret = i2c_write_bytes(g_u8AlpuDeviceID, device_addr,(uint16_t)sub_addr, buff, ByteNo, FALSE);
//    return l_u8Ret;
    return l_u8Ret?0:1;
}

unsigned char _i2c_read(unsigned char device_addr, unsigned char sub_addr, unsigned char *buff, int ByteNo)
{
    u8 l_u8Ret = 0;
    l_u8Ret = i2c_read_byte(device_addr,sub_addr,ByteNo,buff,g_u8AlpuDeviceID);
//	l_u8Ret = i2c_read_bytes(g_u8AlpuDeviceID, device_addr,(uint16_t)sub_addr, buff, ByteNo, FALSE);
//    return l_u8Ret;
    return l_u8Ret?0:1;
}

extern unsigned char alpuc_process(unsigned char *, unsigned char *);
#if 0
unsigned char _alpuc_process(void)
{
    unsigned char error_code,sub_address=0;
    int i;

    //Seed Generate
    for(i=0; i<8; i++)
    {
        dec_tx_data[i] = _alpu_rand();
    }
    //Write Seed Data to ALPU-C
    sub_address = 0x80;
    error_code = _i2c_write(DEVICE_ADDRESS, sub_address, dec_tx_data, 8);
    if ( error_code )
        return error_code ;

    _alpu_delay_ms(1);
    //Read Result Data from ALPU-C
    error_code = _i2c_read(DEVICE_ADDRESS, sub_address, dec_rx_data, 8);
    if ( error_code )
        return error_code ;

    //Bypass Operation
    _alpuc_bypass();

    //Compare the encoded data and received data
    for (i=0; i<8; i++)
    {
        if(dec_dx_data[i] != dec_rx_data[i])
            return 60;
    }
    return 0;
}
#endif

void _alpu_delay_ms(unsigned int i) // Need for ALPUC lib.
{
    delay_ms(i);
}

unsigned char _alpu_rand(void)  // Need for ALPUC lib.
{
    static unsigned long seed; // 2byte, must be a static variable

    seed = seed + rand(); // rand(); <------------------ add time value
    seed = seed * 1103515245 + 12345;

    return (seed/65536) % 32768;
}

int alpu_func(void)
{
    int j;
    unsigned char error_code;
    static  unsigned char  error_cnt=0;
    unsigned char dx_data[8];
    unsigned char tx_data[8];

//    for(i=0; i<8; i++)  tx_data[i] =_alpu_rand();
    memset(dx_data,0,8);
    memset(tx_data,0,8);
    for(j=0; j<10; j++)
    {
        error_code = alpuc_process(tx_data,dx_data);

        if ( error_code )
        {
            dbg_printf("\r\nAlpu-C Encryption Test Fail!!!\r\n");
            error_cnt++;
            if(error_cnt>5)
            {
                while(1)
                {
                    RS232_printf("ALPU buffer error or no chip!!!\r\n");
                    //led_toggle(WORK_LED_PORT,WORK_LED_PIN);
                    delay_ms(2000);
                }
            }
			dbg_printf("\r\nError Code : %d",error_code);
        }
        else
        {
            //sys_temp.aplu_status=0;
            dbg_printf("\r\nAlpu-C Encryption Test Success!!!\r\n");
        }

#if 0
    {
        int i;
        dbg_printf("\r\n");
        dbg_printf("========================ALPU-C IC Encryption========================\r\n");
        dbg_printf(" tx Data : ");
        for (i=0; i<8; i++)
            dbg_printf("0x%02x ", tx_data[i]);
        dbg_printf("\r\n");
        dbg_printf(" dx Data : ");
        for (i=0; i<8; i++)
            dbg_printf("0x%02x ",dx_data[i]);
        dbg_printf("\r\n");
        dbg_printf("====================================================================\r\n");
    }
#endif
    }

    //other functions..
    return 0;
}

#endif





/* EOF */
