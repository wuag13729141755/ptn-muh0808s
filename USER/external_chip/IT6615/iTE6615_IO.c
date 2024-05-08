///*****************************************
//  Copyright (C) 2009-2019
//  ITE Tech. Inc. All Rights Reserved
//  Proprietary and Confidential
///*****************************************
//   @file   <iTE6615_IO.c>
//   @author Kuro.Chung@ite.com.tw
//   @date   2019/06/05
//   @fileversion: iTE6615_MCUSRC_1.03
//******************************************/
#if 0
#include "iTE6615_Config.h"
#include "iTE6615_MCU.h"
#include <stdio.h>
#include "iTE6615_IO.h"
#include "iTE6615_Debug.h"
#include "iTE6615_HDCP_Repeater_DEFINE.h"
#include "iTE6615_DEV_DEFINE.h"
void SetintActive(u1 bactive)
{
    bactive = 0;  //fake code
}


void set_8051_scl(u1 bit_value, u8 device)
{
    //     stall5us;
    switch (device)
    {
        case 0:
            DEV0_SCL_PORT = bit_value;
            break;
        case 1:
            DEV1_SCL_PORT = bit_value;
            break;
        case 2:
            DEV2_SCL_PORT = bit_value;
            break;
        case 3:
            DEV3_SCL_PORT = bit_value;
            break;
        case 4:
            DEV4_SCL_PORT = bit_value;
            break;
        default:
            DEV0_SCL_PORT = bit_value;
            break;
    }
    //     stall5us;
}

void set_8051_sda(u1 bit_value, u8 device)
{
    //     stall5us;
    switch (device)
    {
        case 0:
            DEV0_SDA_PORT = bit_value;
            break;
        case 1:
            DEV1_SDA_PORT = bit_value;
            break;
        case 2:
            DEV2_SDA_PORT = bit_value;
            break;
        case 3:
            DEV3_SDA_PORT = bit_value;
            break;
        case 4:
            DEV4_SDA_PORT = bit_value;
            break;

        default:
            DEV0_SDA_PORT = bit_value;
            break;
    }
    //     stall5us;
}

u1 get_8051_sda(u8 device)
{
    switch (device)
    {
        case 0:
            DEV0_SDA_PORT = 1;
            return DEV0_SDA_PORT;
            break;
        case 1:
            DEV1_SDA_PORT = 1;
            return DEV1_SDA_PORT;
            break;
        case 2:
            DEV2_SDA_PORT = 1;
            return DEV2_SDA_PORT;
            break;

        case 3:
            DEV3_SDA_PORT = 1;
            return DEV3_SDA_PORT;
            break;

        case 4:
            DEV4_SDA_PORT = 1;
            return DEV4_SDA_PORT;
            break;

        default:
            DEV0_SDA_PORT = 1;
            return DEV0_SDA_PORT;
            break;
    }
}

void i2c_8051_start(u8 device)
{
    set_8051_sda(HIGH, device);
    set_8051_scl(HIGH, device);
    set_8051_sda(LOW, device);
    set_8051_scl(LOW, device);
}

void i2c_8051_write(u8 byte_data, u8 device)
{
    u8 _DATA bit_cnt, tmp;
    u1 _DATA bit_value;

    for (bit_cnt = 0; bit_cnt<8; bit_cnt++) {
        tmp = (byte_data << bit_cnt) & 0x80;
        bit_value = tmp && 0x80;

        set_8051_sda(bit_value, device);
        set_8051_scl(HIGH, device);
        set_8051_scl(LOW, device);
    }
}

u1 i2c_8051_wait_ack(u8 device)
{
    u1 _DATA ack_bit_value;

    set_8051_sda(HIGH, device);
    set_8051_scl(HIGH, device);
    ack_bit_value = get_8051_sda(device);
    set_8051_scl(LOW, device);

    return ack_bit_value;
}

u8 i2c_8051_read(u8 device)
{
    u8 _DATA bit_cnt, byte_data;
    u1 _DATA bit_value;

    byte_data = 0;
    for (bit_cnt = 0; bit_cnt<8; bit_cnt++) {
        set_8051_scl(HIGH, device);

        bit_value = get_8051_sda(device);

        byte_data = (byte_data << 1) | bit_value;

        set_8051_scl(LOW, device);
    }

    return byte_data;
}

void i2c_8051_send_ack(u1 bit_value, u8 device)
{
    set_8051_sda(bit_value, device);
    set_8051_scl(HIGH, device);
    set_8051_scl(LOW, device);
    set_8051_sda(HIGH, device);
}

void i2c_8051_end(u8 device)
{
    set_8051_sda(LOW, device);
    set_8051_scl(HIGH, device);
    set_8051_sda(HIGH, device);
}

u1 i2c_write_byte(u8 address, u8 offset, u8 byteno, u8 *p_data, u8 device)
{

    u8 _DATA i;



    i2c_8051_start(device);

    i2c_8051_write(address & 0xFE, device);
    if (i2c_8051_wait_ack(device) == 1)    {
        i2c_8051_end(device);
        return 0;
    }

    i2c_8051_write(offset, device);
    if (i2c_8051_wait_ack(device) == 1)    {
        i2c_8051_end(device);
        return 0;
    }

    for (i = 0; i<byteno - 1; i++) {
        i2c_8051_write(*p_data, device);
        if (i2c_8051_wait_ack(device) == 1) {
            i2c_8051_end(device);
            return 0;
        }
        p_data++;
    }

    i2c_8051_write(*p_data, device);
    if (i2c_8051_wait_ack(device) == 1)    {
        i2c_8051_end(device);
        return 0;
    }
    else {
        i2c_8051_end(device);
        return 1;
    }
}

u1 i2c_read_byte(u8 address, u8 offset, u8 byteno, u8 *p_data, u8 device)
{
    u8 _DATA i;

    i2c_8051_start(device);

    i2c_8051_write(address & 0xFE, device);
    if (i2c_8051_wait_ack(device) == 1) {
        i2c_8051_end(device);
        return 0;
    }

    i2c_8051_write(offset, device);
    if (i2c_8051_wait_ack(device) == 1) {
        i2c_8051_end(device);
        return 0;
    }

    i2c_8051_start(device);

    i2c_8051_write(address | 0x01, device);
    if (i2c_8051_wait_ack(device) == 1) {
        i2c_8051_end(device);
        return 0;
    }

    for (i = 0; i<byteno - 1; i++) {
        *p_data = i2c_8051_read(device);
        i2c_8051_send_ack(LOW, device);

        p_data++;
    }

    *p_data = i2c_8051_read(device);
    i2c_8051_send_ack(HIGH, device);
    i2c_8051_end(device);

    return 1;
}
#endif


