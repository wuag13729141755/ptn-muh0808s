///*****************************************
//  Copyright (C) 2009-2019
//  ITE Tech. Inc. All Rights Reserved
//  Proprietary and Confidential
///*****************************************
//   @file   <IT6635.c>
//   @author Kenneth.Hung@ite.com.tw
//   @date   2019/06/24
//   @fileversion: IT6635_SAMPLE_1.20
//******************************************/
#define _SHOW_PRAGMA_MSG
#include "IT6635/chip/IT6635_config.h"
#include "IT6635/chip/IT6635_platform.h"
#include "IT6635/chip/IT6635_debug.h"
#include "IT6635/chip/it6635_drv.h"
#include "IT6635/chip/it6635_EQ.h"
#include "IT6635/chip/it6635.h"

#if DEBUG_FSM_CHANGE
#define __init_str_SYS_FSM_STATE
#include "IT6635_drv_h_str.h"
#endif

//for CEC
#if IT6635_EN_CEC
#include "IT6635/chip/it6635_cec.h"
#include "IT6635/Customer/IT6635_CecSys.h"
#endif
//for CEC

static u8 g_is_set_io = 0;
u8 dev_state = DEV_FW_VAR_INIT;
IT6635_DEVICE_DATA gdev;

static const u8 s_default_edid_block0[] =
{
 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00,
 0x1E, 0x6D, 0x01, 0x00, 0x01, 0x01, 0x01, 0x01,
 0x01, 0x1A, 0x01, 0x03, 0x80, 0xA0, 0x5A, 0x78,
 0x0A, 0xEE, 0x91, 0xA3, 0x54, 0x4C, 0x99, 0x26,
 0x0F, 0x50, 0x54, 0xA1, 0x08, 0x00, 0x31, 0x40,
 0x45, 0x40, 0x61, 0x40, 0x71, 0x40, 0x81, 0x80,
 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x08, 0xE8,
 0x00, 0x30, 0xF2, 0x70, 0x5A, 0x80, 0xB0, 0x58,
 0x8A, 0x00, 0x40, 0x84, 0x63, 0x00, 0x00, 0x1E,
 0x02, 0x3A, 0x80, 0x18, 0x71, 0x38, 0x2D, 0x40,
 0x58, 0x2C, 0x45, 0x00, 0x40, 0x84, 0x63, 0x00,
 0x00, 0x1E, 0x00, 0x00, 0x00, 0xFD, 0x00, 0x3A,
 0x3E, 0x1E, 0x88, 0x3C, 0x00, 0x0A, 0x20, 0x20,
 0x20, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0xFC,
 0x00, 0x4C, 0x47, 0x20, 0x54, 0x56, 0x0A, 0x20,
 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x01, 0x9F,
};

static const u8 s_default_edid_block1_p0[] =
{
 0x02, 0x03, 0x4B, 0xF1, 0x58, 0x61, 0x60, 0x10,
 0x1F, 0x04, 0x13, 0x05, 0x14, 0x03, 0x02, 0x12,
 0x20, 0x21, 0x22, 0x15, 0x01, 0x5D, 0x5E, 0x5F,
 0x65, 0x66, 0x62, 0x63, 0x64, 0x29, 0x3D, 0x06,
 0xC0, 0x15, 0x07, 0x50, 0x09, 0x57, 0x07, 0x6E,
 0x03, 0x0C, 0x00, 0x30, 0x00, 0xB8, 0x3C, 0x20,
 0x00, 0x80, 0x01, 0x02, 0x03, 0x04, 0x67, 0xD8,
 0x5D, 0xC4, 0x01, 0x78, 0x80, 0x03, 0xE3, 0x05,
 0xC0, 0x00, 0xE4, 0x0F, 0x03, 0x00, 0x18, 0xE3,
 0x06, 0x05, 0x01, 0x01, 0x1D, 0x80, 0x18, 0x71,
 0x1C, 0x16, 0x20, 0x58, 0x2C, 0x25, 0x00, 0x40,
 0x84, 0x63, 0x00, 0x00, 0x9E, 0x66, 0x21, 0x50,
 0xB0, 0x51, 0x00, 0x1B, 0x30, 0x40, 0x70, 0x36,
 0x00, 0x40, 0x84, 0x63, 0x00, 0x00, 0x1E, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x29,
};

static const u8 s_default_edid_block1_p1[] =
{
 0x02, 0x03, 0x4B, 0xF1, 0x58, 0x61, 0x60, 0x10,
 0x1F, 0x04, 0x13, 0x05, 0x14, 0x03, 0x02, 0x12,
 0x20, 0x21, 0x22, 0x15, 0x01, 0x5D, 0x5E, 0x5F,
 0x65, 0x66, 0x62, 0x63, 0x64, 0x29, 0x3D, 0x06,
 0xC0, 0x15, 0x07, 0x50, 0x09, 0x57, 0x07, 0x6E,
 0x03, 0x0C, 0x00, 0x30, 0x00, 0xB8, 0x3C, 0x20,
 0x00, 0x80, 0x01, 0x02, 0x03, 0x04, 0x67, 0xD8,
 0x5D, 0xC4, 0x01, 0x78, 0x80, 0x03, 0xE3, 0x05,
 0xC0, 0x00, 0xE4, 0x0F, 0x03, 0x00, 0x18, 0xE3,
 0x06, 0x05, 0x01, 0x01, 0x1D, 0x80, 0x18, 0x71,
 0x1C, 0x16, 0x20, 0x58, 0x2C, 0x25, 0x00, 0x40,
 0x84, 0x63, 0x00, 0x00, 0x9E, 0x66, 0x21, 0x50,
 0xB0, 0x51, 0x00, 0x1B, 0x30, 0x40, 0x70, 0x36,
 0x00, 0x40, 0x84, 0x63, 0x00, 0x00, 0x1E, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x29,
};

static const u8 s_default_edid_block1_p2[] =
{
    0x02, 0x03, 0x30, 0x71, 0x4D, 0x90, 0x22, 0x20, 0x05, 0x04, 0x03, 0x02, 0x01, 0x61, 0x60, 0x5D,
    0x5E, 0x5F, 0x23, 0x09, 0x07, 0x07, 0x6D, 0x03, 0x0C, 0x00, 0x10, 0x00, 0xB8, 0x3C, 0x20, 0x00,
    0x60, 0x01, 0x02, 0x03, 0x67, 0xD8, 0x5D, 0xC4, 0x01, 0x78, 0x80, 0x03, 0xE3, 0x0F, 0x00, 0x06,
    0x02, 0x3A, 0x80, 0x18, 0x71, 0x38, 0x2D, 0x40, 0x58, 0x2C, 0x45, 0x00, 0x58, 0x54, 0x21, 0x00,
    0x00, 0x1A, 0x56, 0x5E, 0x00, 0xA0, 0xA0, 0xA0, 0x29, 0x50, 0x30, 0x20, 0x35, 0x00, 0x58, 0x54,
    0x21, 0x00, 0x00, 0x1A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xAA,
};

static const u8 s_default_edid_block1_p3[] =
{
    0x02, 0x03, 0x30, 0x71, 0x4D, 0x90, 0x22, 0x20, 0x05, 0x04, 0x03, 0x02, 0x01, 0x61, 0x60, 0x5D,
    0x5E, 0x5F, 0x23, 0x09, 0x07, 0x07, 0x6D, 0x03, 0x0C, 0x00, 0x10, 0x00, 0xB8, 0x3C, 0x20, 0x00,
    0x60, 0x01, 0x02, 0x03, 0x67, 0xD8, 0x5D, 0xC4, 0x01, 0x78, 0x80, 0x03, 0xE3, 0x0F, 0x00, 0x06,
    0x02, 0x3A, 0x80, 0x18, 0x71, 0x38, 0x2D, 0x40, 0x58, 0x2C, 0x45, 0x00, 0x58, 0x54, 0x21, 0x00,
    0x00, 0x1A, 0x56, 0x5E, 0x00, 0xA0, 0xA0, 0xA0, 0x29, 0x50, 0x30, 0x20, 0x35, 0x00, 0x58, 0x54,
    0x21, 0x00, 0x00, 0x1A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xAA,
};


////////////////////////////////////////////////////////////////////////////////

u8 g_enable_io_log = 0;
#define PR_IO(x)  { if (g_enable_io_log)  pr_info x ; }
//#define PR_IO(x)


void _pr_buf(void *buffer, int length)
{
    int i;
    u8 *buf = (u8 *)buffer;
    int pr_len = 16;

    while ( length )
    {
        if ( length < pr_len )
        {
            pr_len = length;
        }

        for ( i = 0; i < pr_len; i++ )
        {
            PR_INFO(("%02X ",*buf));
            buf++;
        }
        PR_INFO(("\r\n"));
        length -= pr_len;
    }
}


u8 h2swwr(u8 offset, u8 wdata)
{
    if ( g_is_set_io == 0 ) {
        //PR_IO(("    ---> h2swwr(0x%02x, 0x%02x);\r\n", offset, wdata));
        PR_IO(("    w %02x %02x %02x\r\n", offset, wdata, SWAddr));
    }
    else{
        g_is_set_io = 0;
    }

    if ( (gdev.opts.EnRxDDCBypass) && (offset == 0x3C) )
    {
        PR_ERR(("****h2swwr 0x3C 0x%02X\r\n", wdata));
    }

    return it6635_i2c_write(SWAddr, offset, 1, &wdata);
}

u8 h2swrd(u8 offset)
{
    u8 rddata;

    it6635_i2c_read(SWAddr, offset, 1, &rddata);

    return rddata;
}

u8 h2swset(u8 offset, u8 mask, u8 wdata)
{
    u8 temp;

    g_is_set_io = 1;
    //PR_IO(("    ---> h2swset(0x%02x, 0x%02x, 0x%02x);\r\n", offset, mask, wdata));
    PR_IO(("    s %02x %02x %02x %02x\r\n", offset, mask, wdata, SWAddr));

    temp = h2swrd(offset);
    temp = (temp & ((~mask) & 0xFF)) + (mask & wdata);

    return h2swwr(offset, temp);
}

void h2swbrd(u8 offset, u8 length, u8 *rddata)
{
    if ( length > 0 )
    {
        it6635_i2c_read(SWAddr, offset, length, rddata);
    }
}

void h2swbwr(u8 offset, u8 length, u8 *rddata)
{
    PR_IO(("    ---> h2swbwr(0x%02x, 0x%02x);\r\n", offset,*rddata));
    if ( length > 0 )
    {
        it6635_i2c_write(SWAddr, offset, length, rddata);
    }
}

////////////////////////////////////////////////////////////////////////////////
u8 h2rxedidwr(u8 offset, u8 *wrdata, u8 length)
{
    //PR_IO(("    ---> h2rxedidwr(0x%02x, 0x%02x);\r\n", offset, length));
    #if 0
    u8 i;
    for (i=0 ; i<length ; i++)
    {
        PR_IO(("w %02x %02x %02x\r\n", offset+i, wrdata[i], RXEDIDAddr));
    }
    #endif

    return it6635_i2c_write(RXEDIDAddr, offset, length, wrdata);
}

u8 h2rxedidrd(u8 offset, u8 *wrdata, u8 length)
{
    return it6635_i2c_read(RXEDIDAddr, offset, length, wrdata);
}

u8 h2rxwr(u8 offset, u8 rdata)
{
    if ( g_is_set_io == 0 ) {
        //PR_IO(("    ---> h2swwr(0x%02x, 0x%02x);\r\n", offset, rdata));
        PR_IO(("w %02x %02x %02x\r\n", offset, rdata, RXAddr));
    }
    else{
        g_is_set_io = 0;
    }

    return it6635_i2c_write(RXAddr, offset, 1, &rdata);
}

u8 h2rxrd(u8 offset)
{
    u8 rddata;

    it6635_i2c_read(RXAddr, offset, 1, &rddata);

    return rddata;
}

u8 h2rxset(u8 offset, u8 mask, u8 wdata)
{
    u8 temp;

    g_is_set_io = 1;
    //PR_IO(("    ---> h2swset(0x%02x, 0x%02x, 0x%02x);\r\n", offset, mask, wdata));
    PR_IO(("    s %02x %02x %02x %02x\r\n", offset, mask, wdata, RXAddr));

    temp = h2rxrd(offset);
    temp = (temp & ((~mask) & 0xFF)) + (mask & wdata);

    return h2rxwr(offset, temp);
}

void h2rxbrd(u8 offset, u8 length, u8 *rddata)
{
    if ( length > 0 )
    {
        it6635_i2c_read(RXAddr, offset, length, rddata);
    }
}

void h2rxbwr(u8 offset, u8 length, u8 *rddata)
{
    PR_IO(("    ---> h2rxbwr(0x%02x, 0x%02x);\r\n", offset,*rddata));
    if ( length > 0 )
    {
        it6635_i2c_write(RXAddr, offset, length, rddata);
    }
}

#if IT6635_EN_CEC

u8 cecwr(u8 offset, u8 wdata)
{
    PR_IO(("    ---> cecwr(0x%02x, 0x%02x);\r\n", offset, wdata));
    return it6635_i2c_write(CECAddr, offset, 1, &wdata);
}

u8 cecrd(u8 offset)
{
    u8 rddata;

    it6635_i2c_read(CECAddr, offset, 1, &rddata);

    return rddata;
}

u8 cecset(u8 offset, u8 mask, u8 rdata)
{
    u8 temp;

    temp = cecrd(offset);
    temp = (temp & ((~mask) & 0xFF)) + (mask & rdata);

    return cecwr(offset, temp);
}

void cecbrd(u8 offset, u8 length, u8 *rddata)
{
    if ( length > 0 )
    {
        it6635_i2c_read(CECAddr, offset, length, rddata);
    }
}

void cecbwr(u8 offset, u8 length, u8 *rddata)
{
    PR_IO(("    ---> cecbwr(0x%02x, 0x%02x);\r\n", offset,*rddata));
    if ( length > 0 )
    {
        it6635_i2c_write(CECAddr, offset, length, rddata);
    }
}

#endif

void chgrxbank(u8 bankno)
{
    h2rxset(0x0f, 0x03, bankno & 0x03);
}

void chgswbank(u8 bankno)
{
    h2swset(0x0f, 0x03, bankno & 0x03);
}

bool _tx_is_sink_hpd_high(void)
{
    if ( h2swrd(0x11) & 0x20 )
    {
        return TRUE;
    }
    return FALSE;
}

bool _tx_ddcwait(void)
{
    u8 ddcwaitcnt, ddc_status;

    ddcwaitcnt = 0;
    do
    {
        ddcwaitcnt++;
        delay1ms(DDCWAITTIME);
    }
    while ( (h2swrd(0x1B) & 0x80) == 0x00 && ddcwaitcnt < DDCWAITNUM );

    if ( ddcwaitcnt == DDCWAITNUM )
    {
        ddc_status = h2swrd(0x1B) & 0xFE;
        PR_ERR(("TXP DDC Bus Status = %02X\r\n", ddc_status));
        PR_ERR(("ERROR: TX DDC Bus Wait TimeOut => "));

        if ( h2swrd(0x27) & 0x80 )
        {
            PR_ERR(("DDC Bus Hang !!!\r\n"));
            //Do not handle the DDC Bus Hang here
            //h2txwr(port, 0x2E, 0x0F);  // Abort DDC Command
            //h2txwr(port, 0x16, 0x08);  // Clear Interrupt
        }
        else if ( ddc_status & 0x20 )
        {
            PR_ERR(("DDC NoACK !!!\r\n"));
        }
        else if ( ddc_status & 0x10 )
        {
            PR_ERR(("DDC WaitBus !!!\r\n"));
        }
        else if ( ddc_status & 0x08 )
        {
            PR_ERR(("DDC ArbiLose !!!\r\n"));
        }
        else
        {
            PR_ERR(("UnKnown Issue !!!\r\n"));
        }

        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

u8 _tx_scdc_write(u8 offset, u8 wdata)
{
    int ddcwaitsts;
    u8 reg3C;

    if ( gdev.opts.EnRxDDCBypass )
    {
        PR_ERR(("EnRxDDCBypass:Abort SCDC write\r\n"));
        return FALSE;
    }

    if ( (h2swrd(0x11) & 0x20) == 0x00 )
    {
        PR_ERR(("HPD-Low:Abort SCDC write\r\n"));
        return FALSE;
    }

    reg3C = h2swrd(0x3C);
    h2swset(0x3C, 0x01, 0x01);          // Enable PC DDC Mode
    h2swwr(0x3D, 0x09);                 // DDC FIFO Clear
    h2swwr(0x3E, 0xA8);                 // EDID Address
    h2swwr(0x3F, offset);               // EDID Offset
    h2swwr(0x40, 0x01);                 // ByteNum[7:0]
    h2swwr(0x42, wdata);                 // WrData
    h2swwr(0x3D, 0x01);                 // Sequential Burst Write
    ddcwaitsts = _tx_ddcwait();
    h2swwr(0x3C, reg3C);          // Disable PC DDC Mode

    if ( ddcwaitsts == 0 )
    {
        PR_INFO(("SCDC wr %02x %02x, ddcwaitsts = %d\r\n", offset, wdata, ddcwaitsts));
    }

    return ddcwaitsts;
}

u8 _tx_scdc_read(u8 offset, u8 *data_buf)
{
    int ddcwaitsts;
    u8 reg3C;

    if ( gdev.opts.EnRxDDCBypass )
    {
        PR_ERR(("EnRxDDCBypass:Abort SCDC read\r\n"));
        return FALSE;
    }

    if ( (h2swrd(0x11) & 0x20) == 0x00 )
    {
        PR_ERR(("HPD-Low:Abort SCDC read\r\n"));
        return FALSE;
    }

    reg3C = h2swrd(0x3C);
    h2swset(0x3C, 0x01, 0x01);          // Enable PC DDC Mode
    h2swwr(0x3D, 0x09);                 // DDC FIFO Clear
    h2swwr(0x3E, 0xA8);                 // EDID Address
    h2swwr(0x3F, offset);               // EDID Offset
    h2swwr(0x40, 0x01);                 // ByteNum[7:0]
                                        //h2swwr(0x42, data);                 // WrData
    h2swwr(0x3D, 0x00);                 // Sequential Burst Write
    ddcwaitsts = _tx_ddcwait();
    h2swwr(0x3C, reg3C);          // Disable PC DDC Mode

    if ( ddcwaitsts == 0 )
    {
        PR_ERR(("SCDC rd %02x ddcwaitsts = %d\r\n", offset, ddcwaitsts));
    }
    else
    {
        *data_buf = h2swrd(0x42);
    }

    return ddcwaitsts;
}

u8 _tx_hdcp_write(u8 offset, u8 data)
{
    int ddcwaitsts;

    if ( gdev.opts.EnRxDDCBypass )
    {
        PR_ERR(("EnRxDDCBypass:Abort HDCP write\r\n"));
        return FALSE;
    }

    if ( (h2swrd(0x11) & 0x20) == 0x00 )
    {
        PR_ERR(("HPD-Low:Abort HDCP write\r\n"));
        return FALSE;
    }

    h2swset(0x3C, 0x01, 0x01);          // Enable PC DDC Mode
    h2swwr(0x3D, 0x09);                 // DDC FIFO Clear
    h2swwr(0x3E, 0x74);                 // EDID Address
    h2swwr(0x3F, offset);               // EDID Offset
    h2swwr(0x40, 0x01);                 // ByteNum[7:0]
    h2swwr(0x42, data);                 // WrData
    h2swwr(0x3D, 0x01);                 // Sequential Burst Write
    ddcwaitsts = _tx_ddcwait();
    h2swset(0x3C, 0x01, 0x00);          // Disable PC DDC Mode

    if ( ddcwaitsts == 0 )
    {
        PR_INFO(("SCDC wr %02x %02x, ddcwaitsts = %d\r\n", offset, data, ddcwaitsts));
    }

    return ddcwaitsts;
}

u8 _tx_hdcp_read(u8 offset, u8 *data_buf, u8 len)
{
    int ddcwaitsts;

    if ( gdev.opts.EnRxDDCBypass )
    {
        PR_ERR(("EnRxDDCBypass:Abort HDCP read\r\n"));
        return FALSE;
    }


    if ( (h2swrd(0x11) & 0x20) == 0x00 )
    {
        PR_ERR(("HPD-Low:Abort HDCP read\r\n"));
        return FALSE;
    }

    h2swset(0x3C, 0x01, 0x01);          // Enable PC DDC Mode
    h2swwr(0x3D, 0x09);                 // DDC FIFO Clear
    h2swwr(0x3E, 0x74);                 // EDID Address
    h2swwr(0x3F, offset);               // EDID Offset
    h2swwr(0x40, len);                 // ByteNum[7:0]
                                        //h2swwr(0x42, data);                 // WrData
    h2swwr(0x3D, 0x00);                 // Sequential Burst Write
    ddcwaitsts = _tx_ddcwait();
    h2swset(0x3C, 0x01, 0x00);          // Disable PC DDC Mode

    if ( ddcwaitsts == 0 )
    {
        PR_ERR(("SCDC rd %02x ddcwaitsts = %d\r\n", offset, ddcwaitsts));
    }
    else
    {
        u8 i;

        PR_INFO(("HDCP read - %02X : ", offset));
        for( i=0 ; i<len ; i++ )
        {
            data_buf[i] = h2swrd(0x42);
            PR_INFO(("%02X ", data_buf[i]));
        }

        PR_INFO(("\r\n"));
    }

    return ddcwaitsts;
}

u8 _tx_scdc_read_ced(u8 *data_buf)
{
    int ddcwaitsts;
    u8 i;

    if ( (h2swrd(0x11) & 0x20) == 0x00 )
    {
        PR_ERR(("HPD-Low:Abort SCDC read\r\n"));
        return FALSE;
    }

    h2swset(0x3C, 0x01, 0x01);          // Enable PC DDC Mode
    h2swwr(0x3D, 0x09);                 // DDC FIFO Clear
    h2swwr(0x3E, 0xA8);                 // EDID Address
    h2swwr(0x3F, 0x50);                 // EDID Offset
    h2swwr(0x40, 0x06);                 // ByteNum[7:0]
                                        //h2swwr(0x42, data);                 // WrData
    h2swwr(0x3D, 0x00);                 // Sequential Burst Write
    ddcwaitsts = _tx_ddcwait();
    h2swset(0x3C, 0x01, 0x00);          // Disable PC DDC Mode

    if ( ddcwaitsts == 0 )
    {
        PR_ERR(("SCDC rd ced ddcwaitsts = %d\r\n", ddcwaitsts));
    }
    else
    {
        for ( i=0 ; i<6 ; i++)
        {
            data_buf[i] = h2swrd(0x42);
        }
    }

    return ddcwaitsts;
}

void _tx_power_down(void)
{
    #if DO_TX_POWER_DOWN
    h2swset(0xD3, 0x80, 0x00);
    h2swset(0xD1, 0x60, 0x60);
    #endif
}

void _tx_power_on(void)
{
    h2swset(0xD3, 0x80, 0x80);   // Reg_XP_ALPWDB=1
    h2swset(0xD1, 0x60, 0x00);   // Reg_XP_PWDI=0, Reg_XP_PWDPLL=0
}

void _tx_show_sink_ced(void)
{
    u8 ced_valid;
    u8 i;
    u8 pr_ced = 0;
    u8 ced_value[6];
    static u8 read_from_scdc = 0;

    ced_valid = h2swrd(0xB0);
    if ( ced_valid )
    {
        PR_INFO(("Begin READ CED:\r\n"));
        pr_ced = 1;
        for (i=0 ; i<6 ; i++)
        {
            if ( ced_valid & (0x01<<i) ) // 0x5? error status is valid
            {
                h2swset(0xAC, 0xE0, (i<<5)); // offset select
                ced_value[i] = h2swrd(0xB1);
            }
        }
        h2swwr(0xAD, 0xFF); // clear CED valid on 0xB0
    }
    else
    {
        #if 0
        if ( read_from_scdc > 10 )
        {
            #if 0
            for ( i = 0; i < 6; i++ )
            {
                _tx_scdc_read(0x50+i, &ced_value[i]);

            }
            #else
            _tx_scdc_read_ced(&ced_value[0]);
            #endif

            PR_INFO(("SCDC: "));
            pr_ced = 1;
            read_from_scdc = 0;
        }
        else
        {
            read_from_scdc++;
        }
        #else
        read_from_scdc = read_from_scdc; // suppress warning
        #endif
    }

    if ( pr_ced )
    {
        for (i=0 ; i<3 ; i++)
        {
            PR_INFO(("ced_valid = %02X, ch%d V=%d err=%04X\r\n", ced_valid, i, (ced_value[2*i+1]>>7)&0x01, ((ced_value[2*i+1]&0xEF)<<8)+ced_value[2*i] ));
        }
        PR_INFO(("\r\n"));
    }
    UNUSED(ced_value[0]);
}

void _tx_ovwr_hdmi_clk(u8 ratio)
{
    switch ( ratio )
    {
        case HDMI_MODE_AUTO:
            h2swset(0xB2, 0x03, 0x00);
            break;
        case HDMI_MODE_14:
            h2swset(0xB2, 0x03, 0x01);
            break;
        case HDMI_MODE_20:
            h2swset(0xB2, 0x03, 0x03);
            break;
    }
}

void _tx_ovwr_h20_scrb(u8 scrb)
{
    switch ( scrb )
    {
        case HDMI_MODE_AUTO:
            h2swset(0xB2, 0x0C, 0x00);
            break;
        case HDMI_MODE_14:
            h2swset(0xB2, 0x0C, 0x08);
            break;
        case HDMI_MODE_20:
            h2swset(0xB2, 0x0C, 0x0c);
            break;
    }
}

void _tx_ovwr_hdmi_mode(u8 mode)
{
    switch ( mode )
    {
        case HDMI_MODE_AUTO:
            h2swset(0xB2, 0x0F, 0x00);
            break;
        case HDMI_MODE_14:
            h2swset(0xB2, 0x0F, 0x05);
            break;
        case HDMI_MODE_20:
            h2swset(0xB2, 0x0F, 0x0F);
            break;
    }
}

void _tx_setup_afe(u32 vclk)
{
    u8 H2ON_PLL, DRV_TERMON, DRV_RTERM, DRV_ISW, DRV_ISWC, DRV_TPRE, DRV_NOPE, H2ClkRatio;

    PR_INFO(("_tx_setup_afe %lu\r\n", vclk));

    //h2rxset(0x23, 0x04, 0x04);

    if ( vclk > 100000UL ) // IP_VCLK05 > 50MHz
    {
        h2swset(0xD1, 0x07, 0x04);
    }
    else
    {
        h2swset(0xD1, 0x07, 0x03);
    }

    if ( vclk > 162000UL ) // IP_VCLK05 > 81MHz
    {
        h2swset(0xD4, 0x04, 0x04);
    }
    else
    {
        h2swset(0xD4, 0x04, 0x00);
    }

    h2swset(0xd8, 0xf0, 0x00);
    if ( vclk > 300000UL )  // single-end swing = 520mV
    {
        DRV_TERMON = 1;
        DRV_RTERM = 0x5;
        DRV_ISW = 0x0E;
        DRV_ISWC = 0x0B;
        DRV_TPRE = 0x0;
        DRV_NOPE = 0;
        H2ON_PLL = 1;
        h2swset(0xd8, 0xf0, 0x30);
    }
    else if ( vclk > 100000UL )  // single-end swing = 450mV
    {
        DRV_TERMON = 1;
        DRV_RTERM = 0x1;
        DRV_ISW = 0x9;
        DRV_ISWC = 0x9;
        DRV_TPRE = 0;
        DRV_NOPE = 1;
        H2ON_PLL = 0;
    }
    else                    // single-end swing = 500mV
    {
        DRV_TERMON = 0;
        DRV_RTERM = 0x0;
        DRV_ISW = 0x3;
        DRV_ISWC = 0x3;
        DRV_TPRE = 0;
        DRV_NOPE = 1;
        H2ON_PLL = 0;
    }

    h2swset(0xD0, 0x08, (H2ON_PLL << 3));
    h2swset(0xD3, 0x1F, DRV_ISW);
    h2swset(0xD4, 0xF0, 0x50);
    h2swset(0xD5, 0xBF, (DRV_NOPE << 7) + (DRV_TERMON << 5) + DRV_RTERM);
    h2swset(0xD7, 0x1F, DRV_ISWC);
    h2swset(0xD6, 0x0F, DRV_TPRE);


    H2ClkRatio = (h2swrd(0xb2) & 0x02) >> 1;    // RegH2ClkRatio from TX

    PR_INFO(("TX Output H2ClkRatio=%d ...\r\n", H2ClkRatio));

    //delay1ms(10);
    //h2rxset(0x23, 0x04, 0x00);
    UNUSED(H2ClkRatio);
}

u8 _rx_calc_edid_sum(u8 *edid )
{
    u8 i;
    u16 sum = 0x100;

    for (i=0 ; i<127 ; i++)
    {
        sum = sum - edid[i];
    }
    return (sum&0xFF);
}

void _rx_caof_init(u8 port)
{
    u8 reg08;
    u8 failcnt;

    h2swset(0x59 + port, 0x20, 0x20);   // new at IT6635B0
    h2swset(0x05 + port, 0x01, 0x01);   // IPLL RST, it6635
    _rx_auto_power_down_enable(port,0);
    _rx_term_power_down(port, 0x00);    // disable PWD CHx termination

    delay1ms(1);
    chgrxbank(3);
    h2rxset(0x3A, 0x80, 0x00);   // Reg_CAOFTrg low
    h2rxset(0xA0, 0x80, 0x80);
    h2rxset(0xA1, 0x80, 0x80);
    h2rxset(0xA2, 0x80, 0x80);
    chgrxbank(0);

    h2rxset(0x2A, 0x41, 0x41);   // CAOF RST and CAOFCLK inversion
    delay1ms(1);
    h2rxset(0x2A, 0x40, 0x00);   // deassert CAOF RST
    h2rxwr(0x25, 0x00);          // Disable AFE PWD
    h2rxset(0x3C, 0x10, 0x00);   // disable PLLBufRst

    chgrxbank(3);
    h2rxset(0x3B, 0xC0, 0x00);   // Reg_ENSOF, Reg_ENCAOF
    h2rxset(0x48, 0x80, 0x80);   // for read back sof value registers
    delay1ms(10);
    h2rxset(0x3A, 0x80, 0x80);   // Reg_CAOFTrg high

    // wait for INT Done
    chgrxbank(0);
    reg08 = 0;
    failcnt = 0;

    while ( reg08 == 0x00 )
    {
        reg08 = h2rxrd(0x08) & 0x10;

        if ( reg08 == 0 )
        {
            failcnt++;
            if ( failcnt >= 4 )
            {
                PR_ERR(("ERROR: CAOF fail !!!\r\n"));

                chgrxbank(3);
                h2rxset(0x3A, 0x80, 0x00); // disable CAOF_Trig
                chgrxbank(0);
                h2rxset(0x2A, 0x40, 0x40); // reset CAOF when caof fail
                h2rxset(0x2A, 0x40, 0x00);
                break;
            }
        }

        delay1ms(2);
    }


    chgrxbank(3);
    h2rxset(0x48, 0x80, 0x80);

    PR_INFO(("CAOF_Int=%02x, Status=%02X\r\n\r\n",(h2rxrd(0x59) & 0xC0),((h2rxrd(0x5A) << 4) + (h2rxrd(0x59) & 0x0F))));
    chgrxbank(0);

    h2swset(0x59+port, 0x20, 0x00);
    h2swset(0x05+port, 0x01, 0x00);

    h2rxset(0x08, 0x30, 0x30);
    h2rxset(0x3C, 0x10, 0x10);

    chgrxbank(3);
    h2rxset(0x3A, 0x80, 0x00); // Reg_CAOFTrg low
    h2rxset(0xA0, 0x80, 0x00);
    h2rxset(0xA1, 0x80, 0x00);
    h2rxset(0xA2, 0x80, 0x00);
    chgrxbank(0);

    _rx_auto_power_down_enable(port, DEFAULT_RX_AUTO_POWER_DOWN);
}

void _rx_show_ced_info(void)
{
    u8 symlock = (h2rxrd(0x14) & 0x38) >> 3;
    u8 ch;

    if ( 0x38 != symlock )
    {
        PR_INFO(("symlock = %02X\r\n", symlock));
    }
    else
    {
        for (ch=0 ; ch<3 ; ch++)
        {
            if ( gdev.vars.RxCEDErrValid & (0x01<<ch) )
            {
                PR_INFO(("ch_%d CED=0x%04X\r\n", ch, gdev.vars.RxCEDErr[ch]));
            }
            else
            {
                PR_INFO(("ch_%d CED=invalid\r\n", ch));
            }
        }
    }
}


void _rx_setup_afe(u32 vclk)
{
    chgrxbank(3);

    if ( vclk >= (1024UL * 102UL) )
    {
        h2rxset(0xA7, 0x40, 0x40);
    }
    else
    {
        h2rxset(0xA7, 0x40, 0x00);
    }

    chgrxbank(0);
}

u8 _rx_is_any_ch_symlock(void)
{
    if ( (h2rxrd(0x14) & 0x38) )
    {
        return 1;
    }
    return 0;
}

u8 _rx_is_all_ch_symlock(void)
{
    if ( (h2rxrd(0x14) & 0x38) == 0x38 )
    {
        DBG_SYMLOCK_1();
        //it6635_txoe(1);
        return 1;
    }
    DBG_SYMLOCK_0();
    return 0;
}

u8 _rx_is_ch_symlock(u8 ch)
{
    if ( (h2rxrd(0x14) & (0x08 << ch)) )
    {
        return 1;
    }
    return 0;
}

bool _rx_is_5v_active(void)
{
    return (h2rxrd(0x13) & 0x01)?TRUE:FALSE;
}

u8 _rx_is_clock_stable(void)
{
    if ( it6635_get_port_info0(gdev.vars.Rx_active_port, (PI_CLK_STABLE | PI_CLK_VALID | PI_5V) ) )
    {
        DBG_CLKSTABLE_1();
        return 1;
    }
    else
    {
        DBG_CLKSTABLE_0();
        return 0;
    }
}

#if EN_AUTO_RS
u8 _rx_need_hpd_toggle(void)
{
    u8 hdcp_sts;
    if ( gdev.vars.Rev >= 0xC0 )
    {
        hdcp_sts = h2swrd(0xB3);
        if ( hdcp_sts & BIT5 )
        {
            PR_INFO(("HDCP 2 done\r\n"));
            return 1;
        }
        if ( hdcp_sts & BIT6 )
        {
            PR_INFO(("HDCP 1 done\r\n"));
            return 1;
        }
        if ( hdcp_sts & BIT7 )
        {
            PR_INFO(("HDCP acc\r\n"));
            //return 0;
        }
    }
    else
    {
        if ( _sw_get_timer0_interrupt() )
        {
            PR_INFO(("TXOE timeout 2\r\n"));
            return 1;
        }
    }

    return 0;

    #if 0
    // todo: need more information
    return 1;
    #endif
}
#endif

void _rx_int_enable(void)
{
    // Set RX Interrupt Enable
    h2rxwr(0x53, 0xFF);           // Enable RxIntEn[7:0]
    h2rxwr(0x54, 0xFF);           // Enable RxIntEn[15:8]
    h2rxwr(0x55, 0xFF);           // Enable RxIntEn[23:16]
    h2rxwr(0x56, 0xFF);           // Enable RxIntEn[31:24]
    h2rxwr(0x57, 0xFF);           // Enable RxIntEn[39:32]
    h2rxwr(0x5D, 0xF7);           // Enable BKIntEn[7:0], but timer int
    h2rxwr(0x5E, 0xFF);           // Enable BKIntEn[15:8]
    h2rxwr(0x5F, 0xFF);           // Enable BKIntEn[23:16]
    h2rxset(0x60, 0x20, 0x20);    // RegEnIntOut=1
}

void _rx_wdog_rst(u8 port)
{
    #if 0
    u8 mask;
    mask = (0x10 << port) | (1 << port);
    h2swset(0x16, mask, mask);
    delay1ms(1);
    h2swset(0x16, mask, 0x00);
    #else
    port=port; // suppress compiler warning
    h2swset(0x2b, 0x01, 0x00);
    delay1ms(2);
    h2swset(0x2b, 0x01, 0x01);
    #endif
}

void _rx_ovwr_hdmi_clk(u8 port, u8 ratio)
{
    switch ( ratio )
    {
        case HDMI_MODE_AUTO:
            h2swset(0x51 + port, 0x28, 0x00);
            break;
        case HDMI_MODE_14:
            h2swset(0x51 + port, 0x28, 0x20);
            break;
        case HDMI_MODE_20:
            h2swset(0x51 + port, 0x28, 0x28);
            break;
    }
}

void _rx_ovwr_h20_scrb(u8 port, u8 scrb)
{
    switch ( scrb )
    {
        case HDMI_MODE_AUTO:
            h2swset(0x51 + port, 0x30, 0x00);
            break;
        case HDMI_MODE_14:
            h2swset(0x51 + port, 0x30, 0x20);
            break;
        case HDMI_MODE_20:
            h2swset(0x51 + port, 0x30, 0x30);
            break;
    }
}

void _rx_ovwr_hdmi_mode(u8 port, u8 mode)
{
    switch ( mode )
    {
        case HDMI_MODE_AUTO:
            h2swset(0x51 + port, 0x38, 0x00);
            h2swset(0x98 + port, 0xC0, 0x00);
            break;
        case HDMI_MODE_14:
            h2swset(0x51 + port, 0x38, 0x20);
            h2swset(0x98 + port, 0xC0, 0x00);
            break;
        case HDMI_MODE_20:
            h2swset(0x51 + port, 0x38, 0x38);
            h2swset(0x98 + port, 0xC0, 0xC0);
            break;
    }
}


void _rx_set_hpd(u8 port, u8 hpd_value, u8 term_value)
{
    if ( port < RX_PORT_COUNT )
    {
        switch ( term_value )
        {
            case TERM_LOW:
                term_value = 0xFF;
                break;
            case TERM_HIGH:
                term_value = 0x00;
                break;
            case TERM_FOLLOW_TX:
                if ( h2swrd(0x11) & 0x40 )
                {
                    term_value = 0x00;
                }
                else
                {
                    term_value = 0xFF;
                }
                break;
            case TERM_FOLLOW_HPD:
                 if ( hpd_value )           //yuan:
                {
                    term_value = 0x00;
                }
                else
                {
                    term_value = 0xFF;
                }
                break;
            default:
                if ( hpd_value )
                {
                    term_value = 0x00;
                }
                else
                {
                    term_value = 0xFF;
                }
                break;
        }
        //if (gdev.vars.RxHPDFlag[port] != value)
        {
            gdev.vars.RxHPDFlag[port] = hpd_value;
            if ( hpd_value )
            {
                if ( gdev.vars.Rx_active_port == port )
                {
                    DBG_TM(RX_HPD_HIGH);
                }

                if ( (gdev.opts.DisableEdidRam & (1<<port)) == 0 )
                {
                    _rx_edid_ram_enable(port);
                }

                #if RX_HPD_OUTPUT_INVERSE
                if ( (RX_HPD_OUTPUT_INVERSE_MAP) & (1<<port) )
                {
                    h2swset(0x4C + port, 0xC0, 0x40);   // RXHPD=0
                }
                else
                {
                    h2swset(0x4C + port, 0xC0, 0xC0);   // RXHPD=1
                }
                #else
                h2swset(0x4C + port, 0xC0, 0xC0);   // RXHPD=1
                #endif

                #if 0
                if ( gdev.vars.Rx_active_port == port )
                {
                    _rx_term_power_down(port,0x7e);     // term power down = 0
                }
                else
                {
                    #if NON_ACTIVE_PORT_DETECT_CLOCK
                    _rx_term_power_down(port,0x7e);     // term power down = 0
                    #else
                    _rx_term_power_down(port,0xFF);     // term power down = 0
                    #endif
                }
                #else
                _rx_term_power_down(port, term_value);
                #endif
            }
            else
            {
                if ( gdev.vars.Rx_active_port == port )
                {
                    DBG_TM(RX_HPD_LOW);
                }


                _rx_edid_ram_disable(port);

                _rx_term_power_down(port, term_value);

                #if RX_HPD_OUTPUT_INVERSE
                if ( (RX_HPD_OUTPUT_INVERSE_MAP) & (1<<port) )
                {
                    h2swset(0x4C + port, 0xC0, 0xC0);   // RXHPD=1
                }
                else
                {
                    h2swset(0x4C + port, 0xC0, 0x40);   // RXHPD=0
                }
                #else
                h2swset(0x4C + port, 0xC0, 0x00);   // RXHPD=0
                #endif

                if ( port == gdev.vars.Rx_active_port )
                {
                    h2swset(0xB2, 0x0A, 0x0A); // clear H2Mode
                }
            }
            PR_INFO(("Set RxP%d HPD = %d %02x\r\n",(int)port,(int)hpd_value, (int)term_value));
        }
    }
    else
    {
        PR_ERR(("Invaild port %d\r\n", port));
    }
}

void _rx_set_hpd_all(u8 value)
{
    u8 i;
    for ( i = 0; i < RX_PORT_COUNT; i++ )
    {
        _rx_set_hpd(i, value, TERM_FOLLOW_HPD);
    }
}

void _rx_set_hpd_with_5v_all(u8 non_active_port_only)
{
    u8 i;
    for ( i = 0; i < RX_PORT_COUNT; i++ )
    {
        if ( non_active_port_only )
        {
            if ( gdev.vars.Rx_active_port == i )
            {
                continue;
            }
        }

        if ( it6635_get_port_info0(i, PI_5V) )
        {
            _rx_set_hpd(i, 1, TERM_FOLLOW_HPD);
        }
        else
        {
            _rx_set_hpd(i, 0, TERM_FOLLOW_HPD);
        }
    }
}

u8 _rx_get_all_port_5v(void)
{
    u8 i;
    u8 ret = 0;
    for ( i = 0; i < RX_PORT_COUNT; i++ )
    {
        if ( it6635_get_port_info0(i, PI_5V) )
        {
            ret |= (1 << i);
        }
    }

    return ret;
}

void _rx_handle_output_err(void)
{
#if EN_AUTO_RS
    if ( gdev.opts.EnableAutoEQ )
    {
        if ( gdev.vars.try_fixed_EQ )
        {
            PR_INFO(("*** fixed EQ fail\r\n"));
            gdev.vars.try_fixed_EQ = 0;
            _eq_reset_txoe_ready();
            _eq_reset_state();
            it6635_fsm_chg(RX_CHECK_EQ);
        }
        else
        {
            it6635_auto_eq_adjust();
        }
    }
#endif
}

void _rx_auto_power_down_enable(u8 port, u8 enable)
{
    if ( enable )
    {
        //h2swset(0x90 + port, 0x3D, 0x3D);
        h2swset(0x90 + port, 0x3D, 0x1D);
    }
    else
    {
        h2swset(0x90 + port, 0x3D, 0x00);
    }
}

void _rx_auto_power_down_enable_all(u8 enable)
{
    u8 i;
    for ( i = 0; i < RX_PORT_COUNT; i++ )
    {
        _rx_auto_power_down_enable(i, enable);
    }
}

void _rx_term_power_down(u8 port, u8 channel)
{
    h2swset(0x88 + port, 0xFF, channel);
}


void _sw_int_enable(u8 port, u8 enable)
{
    if ( enable )
    {
        h2swwr(0x30 + port * 2, 0xff);  // Enable Switch RX Port Interrupt
        h2swset(0x31 + port * 2, 0x01, 0x01);
    }
    else
    {
        h2swwr(0x30 + port * 2, 0x00);  // Disable Switch RX Port Interrupt
        h2swset(0x31 + port * 2, 0x01, 0x00);
        h2swwr(0x20 + port * 2, 0xff);
        h2swwr(0x21 + port * 2, 0xff);
    }

}

void _sw_int_enable_all(u8 enable)
{
    u8 i;
    for ( i = 0; i < RX_PORT_COUNT; i++ )
    {
        _sw_int_enable(i, enable);
    }
}

void _sw_config_timer0(u8 count)
{
    // init timer = count[6:0] * 10 ms
    h2swwr(0x1C, count);
}

void _sw_enable_timer0(void)
{
    h2swset(0x38, 0x02, 0x02);
}

void _sw_disable_timer0(void)
{
    // disable timer will also clear timer interrupt flag
    h2swset(0x38, 0x02, 0x00);
}

#if EN_AUTO_RS
u8 _sw_get_timer0_interrupt(void)
{
    return ( (h2swrd(0x28)&0x02) );
}
#endif

void _sw_clear_timer0_interrupt(void)
{
    h2swset(0x28, 0x02, 0x02);
}

void _sw_config_timer1(u8 count)
{
    // init timer = count[6:0] * 10 ms
    // init timer = BIT7|count[6:0] * 100 ms
    h2swwr(0x1D, count);
}

void _sw_enable_timer1(void)
{
    h2swset(0x38, 0x04, 0x04);
}

void _sw_disable_timer1(void)
{
    h2swset(0x38, 0x04, 0x00);
}

u8 _sw_get_timer1_interrupt(void)
{
    return ( (h2swrd(0x28)&0x04) );
}

void _sw_clear_timer1_interrupt(void)
{
    h2swset(0x28, 0x04, 0x04);
}

void _sw_enable_txoe_timer_check(void)
{
    _sw_disable_timer0();
    _sw_config_timer0(45); // 450ms time out
    _sw_clear_timer0_interrupt();
    _sw_enable_timer0();
}

void _sw_disable_txoe_timer_check(void)
{
    _sw_disable_timer0();
}

void _sw_enable_hpd_toggle_timer(u8 timeout)
{
    // init timer = count[6:0] * 10 ms
    // init timer = BIT7|count[6:0] * 100 ms
    _sw_config_timer1(timeout); // HPT toggle time out

    _sw_clear_timer1_interrupt();
    _sw_enable_timer1();
}

void _sw_disable_hpd_toggle_timer(void)
{
    _sw_disable_timer1();
}

u8 _sw_check_hpd_toggle_timer(void)
{
    return _sw_get_timer1_interrupt();
}

void _sw_reset_scdc_monitor(void)
{
    h2swwr(0xAD, 0xFF);
}

void _sw_monitor_and_fix_scdc_write(void)
{
    u8 reg;
    reg = h2swrd(0xAD);
    if ( reg & 0x10 ) // P0SCDCWrReg20hVld
    {
        PR_INFO(("## src SCDC wr %02X\r\n", reg));

        if ( gdev.vars.current_hdmi_mode == HDMI_MODE_20 )
        {
            if ( (reg&0x03) != 0x03 )
            {
                _tx_scdc_write(0x20, 0x03);
            }
        }
        else if ( gdev.vars.current_hdmi_mode == HDMI_MODE_14 )
        {
            if ( (reg&0x03) != 0x00 )
            {
                _tx_scdc_write(0x20, 0x00);
            }
        }
        _sw_reset_scdc_monitor();
    }
}

void _sw_clear_hdcp_status(void)
{
    h2swwr(0xB0, 0x00);
}

void _sw_show_hdcp_status(void)
{
    u8 hdcp_sts;
    if ( gdev.vars.Rev >= 0xC0 )
    {
        hdcp_sts = h2swrd(0xB3);
        if ( hdcp_sts & BIT5 )
        {
            PR_INFO(("HDCP 2 done\r\n"));
            _sw_clear_hdcp_status();
        }
        if ( hdcp_sts & BIT6 )
        {
            PR_INFO(("HDCP 1 done\r\n"));
            _sw_clear_hdcp_status();
        }
    }
}

void _sw_sdi_check(void)
{
    u8 port;
    u8 reg6C, reg70;
    port = gdev.vars.Rx_active_port;

    if ( gdev.vars.sdi_stable_count < 8 )
    {
        if (  it6635_get_port_info0(port, PI_CLK_STABLE) )
        {
            gdev.vars.sdi_stable_count++;
        }
        else
        {
            gdev.vars.sdi_stable_count = 0;
        }
    }
    else
    {
        // perform check
        gdev.vars.sdi_stable_count = 0;
        reg6C = h2swrd(0x6c+port);
        reg70 = h2swrd(0x70+port);

        if ( reg70 & BIT3 )
        {
            reg6C = reg6C/8;
        }
        else if ( reg70 & BIT2 )
        {
            reg6C = reg6C/4;
        }
        else if ( reg70 & BIT1 )
        {
            reg6C = reg6C/2;
        }
        else
        {
            //reg6C = reg6C/1;
        }

        if ( reg6C < 22 )
        {
            reg70 = h2swrd(0x61+port*3);
            if ( 0 == (reg70 & BIT1) )
            {
                //need re-calculate RDetIPLL_HS1P48G
                reg70 = 1<<port;
                h2swset(0x2A, reg70, reg70);
                PR_INFO(("check_for_sdi recheck ...\r\n"));
            }
            else
            {
                gdev.vars.check_for_sdi = 0;
                PR_INFO(("check_for_sdi disabled ...\r\n"));
            }
        }
        else
        {
            gdev.vars.check_for_sdi = 0;
        }
    }
}

void _sw_hdcp_access_enable(u8 enable)
{
    if ( gdev.vars.spmon == 2 )
    {
        return;
    }

    if ( enable )
    {
        h2swwr(0xAB, 0x60);
        //h2swset(0x3C, 0x01, 0x00);
    }
    else
    {
        h2swwr(0xAB, 0x74);
        //h2swset(0x3C, 0x01, 0x01);
    }
}

void _tx_init(void)
{
    PR_INFO(("TX Port Initial Setting ...\r\n"));

    if ( gdev.debug.ForceRxOn )
    {
        // for ATC electrical test
        h2swwr(0xFF, 0xC3);
        h2swwr(0xFF, 0xA5);
        h2swset(0xF4, 0x80, gdev.debug.ForceRxOn << 7);
        h2swwr(0xFF, 0xFF);
    }

    h2swset(0x50, 0x0B, 0x08);

    h2swset(0x3A, 0xC0, (1 << 7) + (0 << 6));
    h2swset(0x3B, 0x03, 0); // DDC 75K
    h2swset(0x43, 0xFC, (0 << 7) + (0 << 5) + (0 << 4) + (2 << 2));
    h2swset(0xA9, 0xC0, (gdev.opts.EnTxChSwap << 7) + (gdev.opts.EnTxPNSwap << 6));

    // Enable HPD and RxSen Interrupt
    h2swset(0x37, 0x60, 0x60);

    _tx_power_down();

    h2swset(0xBD, 0x01, gdev.opts.EnTxVCLKInv);
    h2swset(0xA9, 0x20, gdev.opts.EnTxOutD1t << 5);

    h2swset(0x50, 0x03, gdev.vars.Rx_active_port);
    it6635_enable_tx_port(1);
}

void _tx_reset(void)
{
    PR_INFO(("TX Reset\r\n"));

    h2swset(0x09, 0x01, 0x01);    // RegSoftTxVRst=1
    h2swset(0x09, 0x01, 0x00);    // RegSoftTxVRst=0

    // Enable TX DDC Master Reset
    h2swset(0x3B, 0x10, 0x10);    // DDC Master Reset
    h2swset(0x3B, 0x10, 0x00);

    _tx_init();
}

void _rx_init(void)
{
    // Add RX initial option setting here
    h2rxset(0x34, 0x01, 0x01);  // Reg_AutoRCLK=1 (default)
    h2rxset(0x21, 0x40, 0x40);  // Reg_AutoEDIDRst=1
    h2rxwr(0x3A, 0xCB);         // to reduce RxDeskew Err and Chx LagErr
    h2rxset(0x3B, 0x20, 0x20);  // CED_Opt
    h2swset(0x44, 0x08, 0x00);
    h2rxset(0x29, 0x40, 0x00);
}

void _rx_reset(void)
{
    PR_INFO(("RX Reset\r\n"));

    h2rxset(0x29, 0x40, 0x00);
    h2swset(0x44, 0x08, 0x08);
    h2rxwr(0x23, 0x01);   // SWRst=1
    h2rxwr(0x22, 0x08);   // RegRst=1

    h2rxwr(0x23, 0xAF);
    delay1ms(1);
    h2rxwr(0x23, 0xA0);

    _rx_init();
}

void _sw_init(void)
{
    u8 port;
    // H2SW Initial Setting
    h2swset(0x44, 0x03, RCLKFreqSel);
    delay1ms(1);

    it6635_init_rclk();

    // Enable Slave Address
    h2swwr(0xEF, RXAddr | 0x01);

    #if IT6635_EN_CEC
    if ( gdev.opts.EnCEC )
    {
        // if CEC is enabled, we should have a accurate RCLK.
        u16 cec_timer_unit;

        h2swwr(0xEE, (CECAddr | 0x01));
        cecset(0x08, 0x01, 0x01);

        cec_timer_unit = gdev.vars.RCLK / (16*10);
        Cec_Init( 0xff&cec_timer_unit );
    }
    else
    #endif
    {
        u8 tmp;
        h2swwr(0xEE, (CECAddr | 0x01));
        //cecset(0x0d, 0x10, 0x00); // Disable CEC_IOPU
        tmp = 0x40;
        it6635_i2c_write(CECAddr, 0x10, 1, &tmp); // Disable CEC_IOPU
        h2swwr(0xEE, (CECAddr & 0xFE));
    }

    h2swset(0x44, 0x40, 0x00);  // EnRxPort2Pwd=0
    delay1ms(10);

    _rx_caof_init(gdev.vars.Rx_active_port);

    // Setup INT Pin: Active Low & Open-Drain
    h2swset(0x11, 0x07, 0x03);

    // Enable SW Interrupt
    h2swset(0x37, 0xE0, 0xE0);
    h2swset(0x38, 0xF9, 0xF9);

    // enable non main port to power down
    h2swset(0x15, 0x08, 0 << 3);

    h2swset(0x2B, 0x02, 0x00);

    h2swset(0x50, 0xf0, 0x00);

    // P0~P3 auto power downs
#if 0
    _rx_auto_power_down_enable_all(1);
#else
    _rx_auto_power_down_enable_all(DEFAULT_RX_AUTO_POWER_DOWN);

    _rx_term_power_down(P0, 0);
    _rx_term_power_down(P1, 0);
    _rx_term_power_down(P2, 0);
    _rx_term_power_down(P3, 0);

#endif

    h2swset(0xF5, 0xE0, (gdev.opts.EnRxDDCBypass << 7) + (gdev.opts.EnRxPWR5VBypass << 6) + (gdev.opts.EnRxHPDBypass << 5));
    if ( gdev.opts.EnRxDDCBypass == 1 )
    {
        h2swset(0x3C, 0x01, 0x01); // disable DDCRegen by set RegTxMastersel=1
        _rx_edid_ram_disable(P0);
        _rx_edid_ram_disable(P1);
        _rx_edid_ram_disable(P2);
        _rx_edid_ram_disable(P3);
    }
    else
    {
        // config EDID RAM
        for (port=0 ; port<RX_PORT_COUNT ; port++)
        {
            if ( gdev.opts.DisableEdidRam & (1<<port) )
            {
                _rx_edid_ram_disable(port);
                _rx_edid_address_disable(port);
            }
            else
            {
                _rx_edid_ram_enable(port);
                _rx_edid_address_enable(port);
            }
        }
    }

    // disable EDID read/write to clear P0AutoH2Mode and AutoScrbEn
    h2swset(0xB2, 0x60, 0x00);
    //h2swset(0xB2, 0x40, 0x00);

    // enable TX port latch ERROR count
    h2swset(0xAC, 0x11, 0x11);

    // ddc monitor
    h2swwr(0xB0, 0x00);

}

void _sw_reset(void)
{
    PR_INFO(("Switch Reset\r\n"));

    h2swset(0x44, 0xA0, 0x80);  // ForceWrUpd=1 and SWGateRCLK=0
    h2swset(0x0A, 0x01, 0x01);  // SoftRstAll=1
    h2swset(0x0A, 0x02, 0x02);  // SoftSWRRst=1

    _sw_init();
}

// To have accurate RCLK,
// we should use "it6635_cal_rclk" instead of "it6635_init_rclk"
#if IT6635_EN_CEC
void it6635_cal_rclk(void)
{
    u8 i;
    u8 timer_int, timer_flt, wclk_high_ext;
    u32 wclk_valid_num, wclk_high_num, wclk_high_num_b, wclk_high_num_c;
    u32 sum, rclk_tmp, rclk, rddata;

    sum = 0;
    for ( i = 0; i < 5; i++ )
    {
        h2swset(0x11, 0x80, 0x80);
        delay1ms(99);
        h2swset(0x11, 0x80, 0x00);

        rddata = h2swrd(0x12);
        rddata += (h2swrd(0x13) << 8);
        rddata += (h2swrd(0x14) << 16);

        sum += rddata;
    }
    sum /= 5;
    rclk = sum / 100;

    PR_INFO(("RCLK=%d kHz\r\n\r\n", rclk));

    timer_int = rclk / 1000;
    timer_flt = (rclk - timer_int * 1000) * 256 / 1000;

    h2swwr(0x1E, timer_int);
    h2swwr(0x1F, timer_flt);

    rclk_tmp = (rclk)*(1 << RCLKFreqSel);

    wclk_valid_num = (8UL * rclk_tmp + 625) / 1250UL;
    wclk_high_num = (8 * rclk_tmp + 3125) / 6250UL;
    h2swset(0x2C, 0x3F, (u8)wclk_high_num & 0xFF);
    h2swwr(0x2D, (u8)wclk_valid_num & 0xFF);

    wclk_high_num_b = 32UL * rclk_tmp / (37125UL);
    wclk_high_num = 32UL * rclk_tmp - (wclk_high_num_b * 37125UL);
    wclk_high_ext = wclk_high_num * 2 / 37125UL;
    h2swwr(0x2E, (wclk_high_ext << 6) + ((u8)wclk_high_num_b));

    wclk_high_num_c = 4UL * rclk_tmp / 10625UL;
    wclk_high_num = 4UL * rclk_tmp - (wclk_high_num_c * 10625UL);
    wclk_high_ext = wclk_high_num * 4 / 10625UL;
    h2swwr(0x2F, (wclk_high_ext << 6) + ((u8)wclk_high_num_c));

    gdev.vars.RCLK = rclk;
}
#endif


void it6635_init_rclk(void)
{
#if IT6635_EN_CEC
    if ( gdev.opts.EnCEC )
    {
        it6635_cal_rclk();
    }
    else
#endif
    {
        #if 0
            // RCLK=20000 kHz
            h2swwr(0x1e, 0x14);
            h2swwr(0x1f, 0x00);
            h2swset(0x2c, 0x3f, 0x1a);
            h2swwr(0x2d, 0x80);
            h2swwr(0x2e, 0x11);
            h2swwr(0x2f, 0x87);
            gdev.vars.RCLK = 20000;
        #else
            #if 0
            // RCLK=19569 kHz
            h2swwr(0x1e, 0x13);
            h2swwr(0x1f, 0x91);
            h2swset(0x2c, 0x3f, 0x19);
            h2swwr(0x2d, 0x7d);
            h2swwr(0x2e, 0x50);
            h2swwr(0x2f, 0x47);
            gdev.vars.RCLK = 19569;
            #endif

            // RCLK=18562 kHz
            h2swwr(0x1e, 0x12);
            h2swwr(0x1f, 0x90);
            h2swset(0x2c, 0x3f, 0x18);
            h2swwr(0x2d, 0x77);
            h2swwr(0x2e, 0x10);
            h2swwr(0x2f, 0xc6);
            gdev.vars.RCLK = 18562;

        #endif
    }
}

u8 it6635_get_port_info1(u8 port, u8 info)
{
    u8 tmp;
    tmp = h2swrd(0x61 + port * 3);

    if ( (tmp & info) == info )
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

u8 it6635_get_port_info0(u8 port, u8 info)
{
    u8 tmp;
    tmp = h2swrd(0x60 + port * 3);

    if ( (tmp & info) == info )
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void it6635_enable_tx_port(u8 enable)
{
    h2swset(0x50, 0x08, (enable << 3));
}

void it6635_txoe(u8 enable)
{
    if ( gdev.vars.current_txoe == enable )
    {
        return;
    }

    PR_INFO(("TXOE=%d align=%d\r\n", enable, gdev.vars.txoe_alignment));

    if ( enable )
    {
        if ( gdev.vars.current_hdmi_mode == HDMI_MODE_20 )
        {
            _tx_ovwr_hdmi_mode(HDMI_MODE_20);
            _tx_scdc_write(0x20, 0x03);
        }
        else if ( gdev.vars.current_hdmi_mode == HDMI_MODE_14 )
        {
            _tx_ovwr_hdmi_mode(HDMI_MODE_14);
            _tx_scdc_write(0x20, 0x00); // todo: ? check if safe to send this?
        }

        //h2swset(0xD4, 0x03, 0x01);      // Set DRV_RST='0'
        h2swset(0xD4, 0x03, 0x00);      // Set DRV_RST='0'
        //DBG_TXOE_1();
        //DBG_TM(TXOE1);

        #if 0
        if ( gdev.opts.EnTxChSwap )
        {
            h2swwr(0xFF, 0xC3);
            h2swwr(0xFF, 0xA5);
            h2swset(0xF4, 0x80, 0x00);  // forcerxon=0 when enabling channel swap
            h2swwr(0xFF, 0xFF);
        }
        #endif

        REPORT_TXOE_1();
    }
    else
    {
        REPORT_TXOE_0();
        //DBG_TM(TXOE0);
        if ( gdev.vars.txoe_alignment )
        {
            h2swset(0xD4, 0x03, 0x01);      // Set DRV_RST='1'
        }
        //DBG_TXOE_0();
        //h2swset(0xD4, 0x03, 0x01);
    }

    gdev.vars.current_txoe = enable;
}

void it6635_set_tx_5v(u8 output_value)
{
    static u8 current_5v = 1;

    if ( current_5v != output_value )
    {
        current_5v = output_value;
        PR_INFO(("TX 5V output=%d\r\n", output_value));
    }

    if ( output_value )
    {
        h2swset(0xF4, 0x0C, 0x0C);    // TXPWR5V=1
    }
    else
    {
        h2swset(0xF4, 0x0C, 0x08);    // TXPWR5V=0
    }
}


u32 it6635_get_rx_vclk(u8 port)
{
    u32 tmds_clk;
    #if USING_WDOG
    u16 tmds_clk_speed;
    u8  wdog_clk_div;
    u8  sw_reg20;

    if ( port >= RX_PORT_COUNT )
    {
        PR_ERR(("it6635_get_rx_vclk p=%u\r\n", port));
        return 0;
    }

__RETRY_VCLK:

    _rx_wdog_rst(gdev.vars.Rx_active_port);

    wdog_clk_div = h2swrd(0x70 + port) & 0x07;

    if ( wdog_clk_div & 0x04 )
        wdog_clk_div = 8;
    else if ( wdog_clk_div & 0x02 )
        wdog_clk_div = 4;
    else if ( wdog_clk_div & 0x01 )
        wdog_clk_div = 2;
    else
        wdog_clk_div = 1;

    tmds_clk_speed = h2swrd(0x6C + port);

    sw_reg20 = h2swrd(0x20 + port * 2);
    if ( sw_reg20 & 0x7C )
    {
        PR_ERR(("it6635_get_rx_vclk sw_reg20=%02x\r\n", sw_reg20));
        tmds_clk_speed = ((tmds_clk_speed * 2) >> (RCLKFreqSel));
        tmds_clk = gdev.vars.RCLK * 256 * wdog_clk_div / tmds_clk_speed;

        PR_ERR(("RXP%d WatchDog detect TMDSCLK = %d kHz (div=%d, 6C=%02X)\r\n", port, tmds_clk, wdog_clk_div, tmds_clk_speed ));

        tmds_clk_speed = 0;
    }

    if ( tmds_clk_speed )
    {
        tmds_clk_speed = ((tmds_clk_speed * 2) >> (RCLKFreqSel));
        tmds_clk = gdev.vars.RCLK * 256 * wdog_clk_div / tmds_clk_speed;

        PR_INFO(("RXP%d WatchDog detect TMDSCLK = %d kHz (div=%d, 6C=%02X)\r\n", port, tmds_clk, wdog_clk_div, tmds_clk_speed ));
    }
    else
    {
        PR_ERR(("TMDSCLKSpeed == 0 p=%u\r\n", port));
        tmds_clk = 0;
    }
    #else

    u8 clk;

    if ( port >= RX_PORT_COUNT )
    {
        PR_ERR(("it6635_get_rx_vclk p=%u\r\n", port));
        return 0;
    }

    clk = h2swrd(0x61 + port*3);

    // the assigned tmds_clk value should refer to _tx_setup_afe()
    if ( clk & 0x04 )
    {
        PR_INFO(("RXP%d clock > 340M\r\n", port ));
        tmds_clk=340000UL;
    }
    else if ( clk & 0x02 )
    {
        PR_INFO(("RXP%d clock > 148M\r\n", port ));
        tmds_clk=163000UL;
    }
    else if ( clk & 0x01 )
    {
        PR_INFO(("RXP%d clock > 100M\r\n", port ));
        tmds_clk=148500UL;
    }
    else
    {
        PR_INFO(("RXP%d clock < 100M\r\n", port ));
        tmds_clk=74000UL;
    }
    #endif

    return tmds_clk;
}

void it6635_detect_port(u8 port)
{
    u8 sw_reg20;
    u8 sw_reg21;
    u8 rddata;
    u8 sts_off0;

    sw_reg20 = h2swrd(0x20 + port * 2);
    sw_reg21 = h2swrd(0x21 + port * 2) & 0x01;

    if ( sw_reg20 )
    {
        sts_off0 = 0x60 + port * 3;
        rddata = h2swrd(sts_off0);

        if ( sw_reg20 & 0x01 )
        {
            PR_INFO(("--RXP-%d 5V Chg => 5V = %d\r\n", port,(rddata & 0x01)));
            if ( gdev.vars.Rx_active_port != port )
            {
                if ( (rddata & 0x01) )
                {
                    // 5V presents
                    if ( gdev.opts.NonActivePortReplyHPD )
                    {
                        _rx_set_hpd(port, 1,TERM_FOLLOW_HPD);
                        sw_reg20 &= 0x01;
                    }
                    else
                    {
                        _rx_set_hpd(port, 0,TERM_FOLLOW_HPD);
                    }
                }
                else
                {
                    _rx_set_hpd(port, 0,TERM_FOLLOW_HPD);
                }
            }
        }

        if ( sw_reg20 & 0x02 )
        {
            PR_INFO(("--RXP-%d RX Clock Valid Change => RxCLK_Valid = %d\r\n", port,(rddata & 0x08) >> 3));
        }

        if ( sw_reg20 & 0x04 )
        {
            PR_INFO(("--RXP-%d RX Clock Stable Change => RxCLK_Stb = %d\r\n\r\n", port,(rddata & 0x10) >> 4));
        }

        if ( sw_reg20 & 0x08 )
        {
            PR_INFO(("--RXP-%d RX Clock Frequency Change ...\r\n", port));
        }

        sts_off0 = 0x61 + port * 3;
        rddata = h2swrd(sts_off0);

        if ( sw_reg20 & 0x10 )
        {
            PR_INFO(("--RXP-%d RX Clock Ratio Change => Clk_Ratio = %d \r\n", port,(rddata & 0x40) >> 6));
        }

        if ( sw_reg20 & 0x20 )
        {
            PR_INFO(("--RXP%d RX Scrambling Enable Change Interrupt => Scr_En = %d \r\n",port,(rddata & 0x80) >> 7));
        }

        sts_off0 = 0x62 + port * 3;
        rddata = h2swrd(sts_off0);

        if ( sw_reg20 & 0x40 )
        {
            PR_INFO(("--RXP%d RX Scrambling Status Change Interrupt => ScrbSts = %d \r\n",port,(rddata & 0x02) >> 1));
        }

        if ( sw_reg20 & 0x80 )
        {
            PR_INFO(("--RXP%d RX HDMI2 Detected Interrupt => HDMI2DetSts = %d \r\n",port,(rddata & 0x3C) >> 2));
        }

        h2swwr(0x20 + port * 2, sw_reg20);
    }

    if ( sw_reg21 )
    {

        h2swwr(0x21 + port * 2, sw_reg21);

#if 1
        if ( sw_reg21 & 0x01 )
        {
            PR_INFO(("--RXP%d EDID Bus Hang Interrupt !!!\r\n", port));
        }
#endif
    }
}

void it6635_detect_ports(void)
{
    u8 i;

    for ( i = 0; i < 4; i++ )
    {
        if ( gdev.vars.Rx_active_port != i )
        {
            it6635_detect_port(i);
        }
    }
}


void it6635_rx_irq(void)
{
    u8 rddata, hdmi_int;
    u8 rx_reg05, rx_reg06, rx_reg10;


    rddata = h2rxrd(0x96);
    hdmi_int = (rddata & 0x40) >> 6;

    if ( hdmi_int )
    {
        rx_reg05 = h2rxrd(0x05);
        rx_reg06 = h2rxrd(0x06);
        rx_reg10 = h2rxrd(0x10);
        h2rxwr(0x05, rx_reg05);
        h2rxwr(0x06, rx_reg06);

        if ( rx_reg05 & 0x01 )
        {
            PR_INFO(("RX5V change\r\n"));

            _eq_reset_txoe_ready();
            _eq_reset_state();
            it6635_auto_detect_hdmi_encoding();

            if ( gdev.opts.TryFixedEQFirst )
            {
                gdev.vars.try_fixed_EQ = 1;
            }

            if ( 0 == _rx_is_5v_active() )
            {
                it6635_fsm_chg_delayed(RX_UNPLUG);
            }
        }

        if ( rx_reg05 & 0x10 )
        {
            PR_INFO(("RX HDMIMode Change Interrupt => HDMIMode = %d\r\n",(h2rxrd(0x13) & 0x02) >> 1));
        }

        if ( rx_reg05 & 0x40 )
        {
            PR_INFO(("RX DeSkew Error Interrupt !!!\r\n"));
        }

        if ( rx_reg05 & 0x80 )
        {
            PR_INFO(("RXP H2V FIFO Skew Fail Interrupt !!!\r\n"));
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if ( rx_reg06 & 0x01 )
        {
            u8 symlock = ((h2rxrd(0x13) & 0x80) >> 7);
            PR_INFO(("RX CHx SymLock Change Interrupt => RxSymLock = %d\r\n", symlock));
            if ( symlock )
            {
                //gdev.vars.count_fsm_err = 0;
            }
        }

        if ( rx_reg06 & 0x02 )
        {
            PR_INFO(("RX CH0 SymFIFORst Interrupt !!!\r\n"));
        }

        if ( rx_reg06 & 0x04 )
        {
            PR_INFO(("RX CH1 SymFIFORst Interrupt !!!\r\n"));
        }

        if ( rx_reg06 & 0x08 )
        {
            PR_INFO(("RX CH2 SymFIFORst Interrupt !!!\r\n"));
        }

        if ( rx_reg06 & 0x10 )
        {
            PR_INFO(("RX CH0 SymLockRst Interrupt !!!\r\n"));
        }

        if ( rx_reg06 & 0x20 )
        {
            PR_INFO(("RX CH1 SymLockRst Interrupt !!!\r\n"));
        }

        if ( rx_reg06 & 0x40 )
        {
            PR_INFO(("RX CH2 SymLockRst Interrupt !!!\r\n"));
        }

        if ( rx_reg06 & 0x80 )
        {
            PR_INFO(("RX FSM Fail Interrupt !!!\r\n"));
            gdev.vars.count_fsm_err++;
            if ( gdev.vars.count_fsm_err > 20 )
            {
                if ( gdev.opts.FixIncorrectHdmiEnc )
                {
                    it6635_fix_incorrect_hdmi_encoding();
                }
                _eq_reset_txoe_ready();
                _eq_reset_state();
                it6635_fsm_chg(RX_WAIT_CLOCK);
                gdev.vars.count_fsm_err = 0;
            }
        }
        else
        {
            if ( gdev.vars.count_fsm_err > 0 )
            {
                gdev.vars.count_fsm_err--;
            }
        }

        #if EN_H14_SKEW
        {
            u8 rx_reg07;
            rx_reg07 = h2rxrd(0x07);
            h2rxwr(0x07, rx_reg07);

            if ( rx_reg07 & 0x01 )
            {
                PR_INFO(("RX CH0 Lag Err\r\n"));
                _rx_skew_adj(0);
            }
            if ( rx_reg07 & 0x02 )
            {
                PR_INFO(("RX CH1 Lag Err\r\n"));
                _rx_skew_adj(1);
            }
            if ( rx_reg07 & 0x04 )
            {
                PR_INFO(("RX CH2 Lag Err\r\n"));
                _rx_skew_adj(2);
            }
        }
        #endif

        if ( rx_reg10 & 0x08 )
        {
            h2rxwr(0x10, 0x08);
            PR_INFO(("RX FW Timer Interrupt ...\r\n"));
        }
    }
}

void it6635_sw_irq(u8 port)
{
    u8 sw_reg20;
    u8 sw_reg21;
    u8 rddata;
    u8 sts_off0;


    sw_reg20 = h2swrd(0x20 + port * 2);
    sw_reg21 = h2swrd(0x21 + port * 2) & 0x01;

    if ( sw_reg20 || sw_reg21 )
    {
        h2swwr(0x20 + port * 2, sw_reg20);
        h2swwr(0x21 + port * 2, sw_reg21);

        sts_off0 = 0x60 + port * 3;

        if ( sw_reg20 & 0x01 )
        {
            // not here
            rddata = h2swrd(sts_off0);
            PR_INFO(("RX-P%d PWR5V Change Interrupt => PWR5V = %d\r\n", port,(rddata & 0x01)));
            //_rx_wdog_rst(port);
            if ( 0 == (rddata & 0x01) )
            {
                it6635_fsm_chg_delayed(RX_UNPLUG);
            }
        }

        if ( sw_reg20 & 0x02 )
        {
            rddata = h2swrd(sts_off0);
            PR_INFO(("RXP%d RX Clock Valid Change Interrupt => RxCLK_Valid = %d\r\n", port,(rddata & 0x08) >> 3));
            if ( port == gdev.vars.Rx_active_port )
            {
                if ( 0 == (rddata & 0x08) ) // clock not valid
                {
                    DBG_TM(CLK_UNSTABLE);
                    if ( gdev.vars.RxHPDFlag[gdev.vars.Rx_active_port] > 0 )
                    {
                        it6635_fsm_chg_delayed(RX_WAIT_CLOCK);
                    }
                }
                else
                {
                    DBG_TM(CLK_STABLE);
                }
            }
        }

        if ( sw_reg20 & 0x04 )
        {
            delay1ms(10);
            rddata = h2swrd(sts_off0);
            PR_INFO(("RXP%d RX Clock Stable Change Interrupt => RxCLK_Stb = %d\r\n\r\n", port,(rddata & 0x10) >> 4));
            if ( 0 == (rddata & 0x10) )
            {
                DBG_CLKSTABLE_0();
                DBG_SYMLOCK_0();
                if ( gdev.vars.RxHPDFlag[port] )
                {
                    it6635_fsm_chg_delayed(RX_WAIT_CLOCK);
                }
            }
            else
            {
                DBG_CLKSTABLE_1();
            }
        }


        if ( sw_reg20 & 0x08 )
        {
            PR_INFO(("RXP%d RX Clock Frequency Change Interrupt ...\r\n", port));
        }

        if ( sw_reg20 & 0x10 )
        {
            u8 new_ratio = (h2swrd(0x61 + port * 3) & 0x40) >> 6;

            PR_INFO(("RXP%d RX Clock Ratio Change Interrupt => Clk_Ratio = %d \r\n", port, new_ratio));

            if ( gdev.vars.Rx_active_port == port )
            {
                if ( new_ratio != gdev.vars.clock_ratio )
                {
                    it6635_fsm_chg_delayed(RX_WAIT_CLOCK);
                }
            }
        }

        if ( sw_reg20 & 0x20 )
        {
            PR_INFO(("RXP%d RX Scrambling Enable Change Interrupt => Scr_En = %d \r\n",port,(h2swrd(0x61 + port * 3) & 0x80) >> 7));
        }

        if ( sw_reg20 & 0x40 )
        {
            PR_INFO(("RXP%d RX Scrambling Status Change Interrupt => ScrbSts = %d \r\n",port,(h2swrd(0x62 + port * 3) & 0x02) >> 1));
        }

        if ( sw_reg20 & 0x80 )
        {
            PR_INFO(("RXP%d RX HDMI2 Detected Interrupt => HDMI2DetSts = %d \r\n",port,(h2swrd(0x62 + port * 3) & 0x3C) >> 2));
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if ( sw_reg21 & 0x01 )
        {
            PR_INFO(("RXP%d EDID Bus Hang Interrupt !!!\r\n", port));
        }
    }
}


void it6635_tx_irq(void)
{
    u8 sw_reg27;
    u8 sw_reg28;
    u8 rddata;
    u8 reg3C;

    sw_reg27 = h2swrd(0x27);
    sw_reg28 = h2swrd(0x28) & ~(0x02|0x04);
    h2swwr(0x27, sw_reg27);
    h2swwr(0x28, sw_reg28);

    if ( sw_reg27 & 0x20 )
    {
        rddata = h2swrd(0x11);

        if ( (rddata & 0x20) )
        {
            PR_INFO((" => HPD High\r\n"));
        }
        else
        {
            PR_INFO((" => HPD Low\r\n"));

            if ( gdev.vars.state_sys_fsm != RX_TOGGLE_HPD &&
                 gdev.vars.state_sys_fsm != RX_UNPLUG )
            {
                it6635_fsm_chg_delayed(TX_UNPLUG);
            }
        }
    }

    if ( sw_reg27 & 0x40 )
    {
        PR_INFO(("TX RxSen Change Int\r\n"));

        if ( h2swrd(0x11) & 0x40 )
        {
            // rxsen = 1
        }
        else
        {
            // rxsen = 0
            //_rx_int_enable_all(0);
            //_rx_set_hpd_all(0);
            //it6635_fsm_chg( TX_WAIT_HPD );
        }
    }

    if ( sw_reg27 & 0x80 )
    {
        PR_INFO(("TX DDC Bus Hang Interrupt ...\r\n"));

        if ( gdev.opts.EnRxDDCBypass == false )
        {
            reg3C = h2swrd(0x3C);
            h2swset(0x3C, 0x01, 0x01);
            h2swwr(0x3D, 0x0A);     // Generate SCL Clock
            h2swwr(0x3C, reg3C);
        }
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if ( sw_reg28 & 0x02 )
    {
        //PR_INFO(("SW User Timer 0 Interrupt ...\r\n"));
    }

    if ( sw_reg28 & 0x04 )
    {
        //PR_INFO(("SW User Timer 1 Interrupt ...\r\n"));
    }

    if ( sw_reg28 & 0x08 )
    {
        PR_INFO(("TX DDC Command Fail Interrupt !!!\r\n"));
        if ( gdev.opts.EnRxDDCBypass == false )
        {
            reg3C = h2swrd(0x3C);
            h2swset(0x3C, 0x01, 0x01);
            h2swwr(0x3D, 0x0F);
            h2swwr(0x3C, reg3C);
        }

    }

    if ( sw_reg28 & 0x80 )
    {
        PR_INFO(("TX DDC FIFO Error Interrupt !!!\r\n"));
        if ( gdev.opts.EnRxDDCBypass == false )
        {
            reg3C = h2swrd(0x3C);
            h2swset(0x3C, 0x01, 0x01);
            h2swwr(0x3D, 0x09);
            h2swwr(0x3C, reg3C);
        }
    }
}

//#if DEBUG_FSM_CHANGE
//void __it6635_fsm_chg(u8 new_state, int caller)
//#else
void it6635_fsm_chg( u8 new_state )
//#endif
{

#if DEBUG_FSM_CHANGE
    if ( new_state <= IDLE && gdev.vars.state_sys_fsm <= IDLE )
    {
        PR_INFO2(("state_fsm %s -> %s (%d)\r\n", s__SYS_FSM_STATE[gdev.vars.state_sys_fsm], s__SYS_FSM_STATE[new_state], caller));
    }
    else
    {
        PR_ERR(("state_fsm %d, new %d -> %d (%d)\r\n", gdev.vars.state_sys_fsm, new_state, caller));
    }
#else
    PR_INFO2(("state_fsm %d -> %d\r\n", gdev.vars.state_sys_fsm, new_state));
#endif

    if ( RX_PORT_CHANGE != new_state )
    {
        if ( gdev.vars.state_sys_fsm == new_state )
        {
            PR_INFO2(("fsm_chg ret\r\n"));
            return;
        }
    }


    if ( new_state == RX_WAIT_CLOCK )
    {
        if ( gdev.vars.state_sys_fsm > new_state )
        {
            // don't change before fsm goes to WAIT_RX_STABLE
            //return;
        }
    }


    gdev.vars.state_sys_fsm = (_SYS_FSM_STATE)new_state;
    gdev.vars.fsm_return = 0;

    switch ( gdev.vars.state_sys_fsm )
    {
        case RX_TOGGLE_HPD:
            _sw_enable_hpd_toggle_timer(gdev.vars.hpd_toggle_timeout);
            break;

        case RX_PORT_CHANGE:

            it6635_txoe(0);

            DBG_TM(RX_SWITCH_PORT);
            PR_INFO(("Active port change from P%d to P%d\r\n", gdev.vars.Rx_active_port, gdev.vars.Rx_new_port));

            if ( gdev.vars.clock_ratio > 0 && gdev.opts.EnRxDDCBypass == false )
            {
                _tx_scdc_write(0x20,0x00);
            }

            if ( gdev.opts.TurnOffTx5VWhenSwitchPort )
            {
                it6635_set_tx_5v(0);
            }

            //_rx_int_enable(gdev.vars.Rx_active_port, 1);
            //_rx_set_hpd(gdev.vars.Rx_active_port, 0);
            //_rx_wdog_rst(gdev.vars.Rx_prev_port);

            if ( gdev.opts.NonActivePortReplyHPD == 0 )
            {
                _rx_set_hpd(gdev.vars.Rx_active_port, 0, TERM_FOLLOW_HPD);
            }

            // set gdev.vars.force_hpd_state to SW_HPD_AUTO
            // this to reset the force hpd low in previous active port
            // remove this line if you want to keep HPD low after port changing
            gdev.vars.force_hpd_state = SW_HPD_AUTO;

            gdev.vars.Rx_active_port = gdev.vars.Rx_new_port;
            h2swset(0x50, 0x03, gdev.vars.Rx_active_port);
            it6635_set_RS(gdev.EQ.FixedRsIndex[gdev.vars.Rx_active_port]);
            gdev.EQ.sys_aEQ = SysAEQ_RUN;
            it6635_auto_detect_hdmi_encoding();
            _eq_reset_state();
            _eq_reset_txoe_ready();
            break;

        case TX_OUTPUT:
            gdev.vars.count_symlock_lost = 0;
            _sw_disable_hpd_toggle_timer();

            if ( (gdev.opts.FixIncorrectHdmiEnc) && (gdev.opts.EnRxDDCBypass == FALSE) )
            {
                if ( gdev.vars.current_hdmi_mode == HDMI_MODE_20 )
                {
                    _tx_scdc_write(0x20, 0x03);
                }
                else if ( gdev.vars.current_hdmi_mode == HDMI_MODE_14 )
                {
                    // todo: to check sink support SCDC ?
                    _tx_scdc_write(0x20, 0x00);
                }
                it6635_auto_detect_hdmi_encoding();
            }

            _sw_hdcp_access_enable(1);
            it6635_txoe(1);

            if ( gdev.vars.spmon == 1 )
            {
                if ( (gdev.opts.DisableEdidRam & (1<<gdev.vars.Rx_active_port)) == 0 )
                {
                    _rx_edid_ram_enable(gdev.vars.Rx_active_port);
                }
            }
            delay1ms(100);
            break;

        case TX_OUTPUT_PREPARE:
            gdev.vars.count_try_force_hdmi_mode = 0;
            h2rxwr(0x05, 0xFF);
            h2rxwr(0x06, 0xFF);
            h2rxwr(0x07, 0xFF);
            break;

        #if EN_AUTO_RS
        case RX_CHECK_EQ:
            gdev.vars.count_symlock_fail=0;
            //_sw_hdcp_access_enable(0);
            break;
        #endif

        case SETUP_AFE:

            _rx_term_power_down(gdev.vars.Rx_active_port,0x00);
            gdev.vars.vclk = it6635_get_rx_vclk(gdev.vars.Rx_active_port);

            if ( gdev.vars.vclk )
            {
                gdev.vars.clock_ratio = ((h2swrd(0x61 + gdev.vars.Rx_active_port * 3) >> 6) & 1);
                PR_INFO2(("Clk Ratio = %d\r\n", gdev.vars.clock_ratio));

                if ( gdev.vars.clock_ratio > 0 )
                {
                    if ( gdev.vars.vclk < 300000UL )
                    {
                        gdev.vars.vclk = 300001UL;
                    }
                    h2rxset(0x3B, 0x10, 0x00); // CED opt for HDBaseT disabled
                }
                else
                {
                    if ( gdev.vars.vclk >= 300000UL )
                    {
                        gdev.vars.vclk = 297000UL;
                    }
                    h2rxset(0x3B, 0x10, 0x10); // CED opt for HDBaseT enabled
                }

                #if 0 // for 8-7 480p
                if ( gdev.vars.vclk < 35000UL ) {
                    PR_INFO2(("## ATC 480P\r\n"));
                    //h2rxset(0x3c, 0x01, 0x00);
                    h2swset(0x2b, 0x02, 0x00);
                }
                else{
                    //h2rxset(0x3c, 0x01, 0x01);
                    h2swset(0x2b, 0x02, 0x02);
                }
                #endif
                _tx_power_on();
                _rx_setup_afe(gdev.vars.vclk);
                _tx_setup_afe(gdev.vars.vclk);
                //h2swset(0x55, 0x01, 0x00);
                _sw_enable_txoe_timer_check();




            }
            break;

        case RX_WAIT_CLOCK:

            #if POWER_DOWN_WHILE_WAITING_CLOCK
            _tx_power_down();

            #endif

            _sw_disable_timer0();
            _sw_clear_hdcp_status();
            //_rx_wdog_rst(gdev.vars.Rx_active_port);

            if ( (0 == gdev.vars.txoe_alignment) || (gdev.vars.is_hdmi20_sink == 0) )
            {
                it6635_txoe(1);
            }
            else
            {
                it6635_txoe(0);
            }

            #if EN_AUTO_RS
            gdev.vars.RxCEDErrRec[1][0]=0xffff;
            gdev.vars.RxCEDErrRec[1][1]=0xffff;
            gdev.vars.RxCEDErrRec[1][2]=0xffff;
            gdev.EQ.manu_eq_fine_tune_count[0]=0;
            gdev.EQ.manu_eq_fine_tune_count[1]=0;
            gdev.EQ.manu_eq_fine_tune_count[2]=0;
            gdev.EQ.ced_err_avg_prev[0] = 0x8888;
            gdev.EQ.ced_err_avg_prev[1] = 0x8888;
            gdev.EQ.ced_err_avg_prev[2] = 0x8888;
            gdev.EQ.ced_acc_count = 0;
            #endif
            gdev.vars.count_symlock = 0;
            gdev.vars.count_unlock = 0;
            gdev.vars.check_for_hpd_toggle = 0;
            gdev.vars.sdi_stable_count = 0;
            gdev.vars.check_for_sdi = 1;
            break;

        case RX_HPD:
            it6635_txoe(0);
            _rx_int_enable();

            #if 1
            //_rx_ovwr_hdmi_clk(gdev.vars.Rx_active_port, HDMI_MODE_14);
            //_rx_ovwr_h20_scrb(gdev.vars.Rx_active_port, 0);
            #else
            _rx_ovwr_hdmi_clk(gdev.vars.Rx_active_port, RX_CLK_H20);
            _rx_ovwr_h20_scrb(gdev.vars.Rx_active_port, 1);
            #endif


            //gdev.vars.Rx_prev_port = gdev.vars.Rx_active_port;
            //_rx_int_enable_all(0);
            //_rx_set_hpd_all(0);
            _sw_hdcp_access_enable(1);
            _sw_int_enable(gdev.vars.Rx_active_port, 1);

            _rx_wdog_rst(gdev.vars.Rx_active_port);
            if ( gdev.vars.spmon == 1 )
            {
                if ( (gdev.opts.DisableEdidRam & (1<<gdev.vars.Rx_active_port)) == 0 )
                {
                    _rx_edid_ram_disable(gdev.vars.Rx_active_port);
                }
            }
            _rx_set_hpd(gdev.vars.Rx_active_port, 1, TERM_FOLLOW_HPD);
            //_rx_set_hpd_all(1);
            //caof_ini();
            //h2swset(0x55, 0x01, 0x00);

            //_rx_ovr_hdmi_clk(gdev.vars.Rx_active_port, RX_CLK_H20);
            //_rx_ovr_h20_scrb(gdev.vars.Rx_active_port, 1);
            //_tx_ovr_hdmi_clk(RX_CLK_H20);
            //_tx_ovr_h20_scrb(RX_CLK_H20);
            break;

        case TX_GOT_HPD:
            it6635_txoe(0);



            if ( gdev.opts.ParseEDIDFromSink && gdev.vars.edid_ready == 0 )
            {
                if ( gdev.opts.EnRxDDCBypass == false )
                {
                    it6635_parse_edid(EDID_SRC_EXT_SINK);
                }
                gdev.vars.edid_ready = 1;
            }

            #if IT6635_EN_CEC
            if(gdev.opts.EnCEC)
            {
                u8 u8phyAB = (gdev.vars.PhyAdr[0] << 4) | (gdev.vars.PhyAdr[1] & 0xF);
                u8 u8phyCD = (gdev.vars.PhyAdr[2] << 4) | (gdev.vars.PhyAdr[3] & 0xF);

                CecSys_Init(u8phyAB, u8phyCD, gdev.vars.Rx_active_port);
            }
            #endif

            gdev.vars.txoe_alignment = TXOE_ALIGN;

            if ( gdev.opts.NonActivePortReplyHPD )
            {
                _rx_set_hpd_with_5v_all(true);//yuan:
            }

            break;

        case TX_WAIT_HPD:
            it6635_txoe(0);
            if ( _rx_is_5v_active() )
            {
                it6635_set_tx_5v(1);
            }
            else
            {
                it6635_set_tx_5v(0);
            }

            //it6635_set_RS(10);
            break;

        case TX_UNPLUG:
            it6635_txoe(0);
            gdev.vars.edid_ready = 0;
            //_rx_int_enable(gdev.vars.Rx_active_port, 0);
            //_rx_set_hpd(gdev.vars.Rx_active_port, 0, TERM_FOLLOW_HPD);//yuan:TERM_FOLLOW_TX
            _rx_set_hpd_all(0);//yuan:
            h2swset(0xB2, 0x0A, 0x0A);  // W1C AutoH2Mode and AutoScrbEn
            _tx_power_down();
            break;

        case RX_UNPLUG:
            it6635_txoe(0);
            //_rx_int_enable(gdev.vars.Rx_active_port, 0);
            _rx_set_hpd(gdev.vars.Rx_active_port, 0, TERM_FOLLOW_TX);
            it6635_set_tx_5v(_rx_is_5v_active());
            //gdev.EQ.sys_aEQ = SysAEQ_RUN;
            h2swset(0xB2, 0x0A, 0x0A);  // W1C AutoH2Mode and AutoScrbEn
            _tx_power_down();

            break;
        default:
            break;
    }
}

//#if DEBUG_FSM_CHANGE
//void __it6635_fsm_chg2(u8 new_state, int caller)
//#else
void it6635_fsm_chg_delayed( u8 new_state )
//#endif
{

#if DEBUG_FSM_CHANGE
    if ( new_state <= IDLE && gdev.vars.state_sys_fsm <= IDLE )
    {
        PR_INFO2(("#state_fsm %s -> %s (%d)\r\n", s__SYS_FSM_STATE[gdev.vars.state_sys_fsm], s__SYS_FSM_STATE[new_state], caller));
    }
    else
    {
        PR_ERR(("#state_fsm %d, new %d -> %d (%d)\r\n", gdev.vars.state_sys_fsm, new_state, caller));
    }
#else
    PR_INFO2(("#state_fsm %d -> %d\r\n", gdev.vars.state_sys_fsm, new_state));
#endif

    it6635_fsm_chg(new_state);
    gdev.vars.fsm_return = 1;
}

void it6635_auto_detect_hdmi_encoding(void)
{
    _rx_ovwr_hdmi_mode(gdev.vars.Rx_active_port, HDMI_MODE_AUTO);
    _tx_ovwr_hdmi_mode(HDMI_MODE_AUTO);
    gdev.vars.current_hdmi_mode = HDMI_MODE_AUTO;
    PR_INFO(("HDMI_MODE=AUTO \r\n"));
}

void it6635_force_hdmi20(void)
{
    _rx_ovwr_hdmi_mode(gdev.vars.Rx_active_port, HDMI_MODE_20);
    _tx_ovwr_hdmi_mode(HDMI_MODE_20);
    gdev.vars.current_hdmi_mode = HDMI_MODE_20;
    PR_INFO(("HDMI_MODE=F20\r\n"));
}

void it6635_force_hdmi14(void)
{
    _rx_ovwr_hdmi_mode(gdev.vars.Rx_active_port, HDMI_MODE_14);
    _tx_ovwr_hdmi_mode(HDMI_MODE_14);
    gdev.vars.current_hdmi_mode = HDMI_MODE_14;
    PR_INFO(("HDMI_MODE=F14\r\n"));
}


void it6635_fix_incorrect_hdmi_encoding(void)
{
    /*
    if ( (gdev.vars.current_hdmi_mode == HDMI_MODE_AUTO) ||
         (gdev.vars.current_hdmi_mode == HDMI_MODE_14) )
    {
        if ( gdev.vars.vclk < 200000UL )
        {
            // try HDMI 2.0
            _rx_ovwr_hdmi_mode(gdev.vars.Rx_active_port, HDMI_MODE_20);
            _tx_ovwr_hdmi_mode(HDMI_MODE_20);
            gdev.vars.current_hdmi_mode = HDMI_MODE_20;
            PR_INFO(("HDMI_MODE=F20\r\n"));
        }
    }
    else
    {
        // try HDMI 1.4
        _rx_ovwr_hdmi_mode(gdev.vars.Rx_active_port, HDMI_MODE_14);
        _tx_ovwr_hdmi_mode(HDMI_MODE_14);
        gdev.vars.current_hdmi_mode = HDMI_MODE_14;
        PR_INFO(("HDMI_MODE=F14\r\n"));
    }
    */

    switch ( gdev.vars.current_hdmi_mode )
    {
        case HDMI_MODE_AUTO:
            if ( gdev.vars.vclk < 200000UL )
            {
                // try HDMI 2.0
                it6635_force_hdmi20();
            }
            break;
        case HDMI_MODE_20:
            // try HDMI 1.4
            it6635_force_hdmi14();
            break;
        case HDMI_MODE_14:
            // try HDMI 2.0
            it6635_force_hdmi20();
            break;
        default:
            // try HDMI 2.0
            it6635_force_hdmi20();
            break;
    }
}

#if EN_AUTO_RS
void it6635_fsm_EQ_check(void)
{
    static u8 aeq_retry = 0;
    u8 eq_state;

    if ( _rx_is_clock_stable() )
    {
        _rx_show_ced_info();

        if ( _eq_get_txoe_ready() == 1 )
        {
            _eq_load_previous();
            PR_INFO(("EQ restore2 !\r\n"));
            //it6635_fsm_chg(TX_OUTPUT);
            it6635_fsm_chg(TX_OUTPUT_PREPARE);
        }
        else
        {
            eq_state = _eq_get_state();
            PR_INFO(("[%d] eq_state=%d\r\n", __LINE__,(int)eq_state));

            if ( eq_state == SysAEQ_RUN )
            {
                _eq_set_txoe_ready(0);
                if ( it6635_auto_eq_adjust() )
                {
                    gdev.vars.check_for_hpd_toggle = 1;
                    _eq_set_state(SysAEQ_DONE);
                    it6635_fsm_chg(TX_OUTPUT_PREPARE);
                    PR_INFO(("EQ done !\r\n"));
                }
                else
                {
                    aeq_retry++;
                    if ( aeq_retry > 5 )
                    {
                        aeq_retry = 0;
                        gdev.vars.check_for_hpd_toggle = 1;
                        _eq_set_state(SysAEQ_DONE);
                        it6635_fsm_chg(TX_OUTPUT_PREPARE);
                        PR_INFO(("EQ give up !\r\n"));
                    }
                }
            }
            else
            {
                if ( eq_state == SysAEQ_DONE )
                {
                    _eq_load_previous();
                    it6635_fsm_chg(TX_OUTPUT_PREPARE);
                    PR_INFO(("EQ restore !\r\n"));
                }
                else if ( eq_state == SysAEQ_OFF )
                {
                    _eq_load_default();
                    it6635_fsm_chg(TX_OUTPUT_PREPARE);
                    PR_INFO(("EQ default !\r\n"));
                }
                else
                {
                    PR_ERR(("??eq_state=%d\r\n",eq_state));
                }
            }
        }
    }
    else
    {
        it6635_fsm_chg(RX_WAIT_CLOCK);
    }
}
#endif

void it6635_fsm(void)
{
    static u8 prep_count = 0;
    static u8 prep_fail_count = 0;
LOOP_FSM:

    //_rx_is_all_ch_symlock();

    switch ( gdev.vars.state_sys_fsm )
    {
        case RX_TOGGLE_HPD:
            if ( (gdev.opts.NonActivePortReplyHPD == 0) && (gdev.opts.TurnOffTx5VWhenSwitchPort == 0) )
            {
                _sw_disable_hpd_toggle_timer();
                it6635_fsm_chg(RX_UNPLUG);
            }
            else
            {
                if ( _sw_check_hpd_toggle_timer() )
                {
                    _sw_disable_hpd_toggle_timer();
                    it6635_fsm_chg(RX_UNPLUG);
                }
                else
                {
                    // keep waiting hpd toggle
                }
            }
            break;

        case RX_PORT_CHANGE:
            _rx_set_hpd(gdev.vars.Rx_active_port, 0, TERM_FOLLOW_HPD);
            _rx_caof_init(gdev.vars.Rx_active_port);
            gdev.vars.hpd_toggle_timeout = HPD_TOGGLE_TIMEOUT;
            it6635_fsm_chg(RX_TOGGLE_HPD);
            break;

        case TX_OUTPUT:

            //if ( gdev.opts.WaitSymlockBeforeTXOE )
            {
                if ( 0 == _rx_is_all_ch_symlock() )
                {
                    gdev.vars.count_symlock_lost++;
                    PR_ERR(("RX Symlock lost %d\r\n", gdev.vars.count_symlock_lost));
                    if ( gdev.vars.count_symlock_lost > 8 )
                    {
                        //it6635_set_tx_5v(0);
                        gdev.vars.count_symlock_lost = 0;
                        _rx_handle_output_err();
                    }
                }
                else
                {
                    gdev.vars.count_symlock_lost = 0;
                }
            }

            if ( _rx_monitor_ced_err() )
            {
                _rx_handle_output_err();
            }

            //_sw_show_hdcp_status();

            if ( gdev.opts.FixIncorrectHdmiEnc )
            {
                //check if source send incorrect SCDC clock ratio after 6635 sent.
                if ( gdev.vars.current_hdmi_mode != HDMI_MODE_AUTO && gdev.opts.EnRxDDCBypass == false )
                {
                    _sw_monitor_and_fix_scdc_write();
                }
            }

            if ( gdev.vars.check_for_sdi )
            {
                _sw_sdi_check();
            }
            delay1ms(100);
            //_tx_show_sink_ced();

            //_pr_port_info(gdev.vars.Rx_active_port);

            break;

        case TX_OUTPUT_PREPARE:
        #if EN_AUTO_RS
            // check symbol lock before tx output
            if ( 0 == _rx_is_any_ch_symlock() )
            {
                PR_ERR(("RxChk-SymUnlock\r\n"));
                gdev.vars.count_symlock_fail++;
                if ( gdev.vars.count_symlock_fail > 3 )
                {
                    gdev.vars.count_symlock_fail = 0;

                    // can not get any channel symbol lock,
                    // the HDMI encoding may be incorrect
                    if ( gdev.opts.FixIncorrectHdmiEnc )
                    {
                        if ( gdev.vars.count_try_force_hdmi_mode < 6 )
                        {
                            gdev.vars.count_try_force_hdmi_mode++;
                            it6635_fix_incorrect_hdmi_encoding();
                        }
                        else
                        {
                            gdev.vars.count_try_force_hdmi_mode = 0;
                            it6635_fsm_chg(RX_WAIT_CLOCK);
                        }
                    }
                    else
                    {
                        _eq_reset_state();
                        _eq_reset_txoe_ready();
                        it6635_fsm_chg(RX_CHECK_EQ);
                    }
                }
            }
            else
            {
                _eq_set_txoe_ready(1);

                if ( (gdev.vars.check_for_hpd_toggle == 1) && (gdev.vars.current_txoe == 0) && (_rx_need_hpd_toggle()) )
                {
                    DBG_TM(AEQ_TOGGLE_HPD);
                    it6635_set_tx_5v(0);
                    _rx_set_hpd(gdev.vars.Rx_active_port, 0, TERM_FOLLOW_HPD);
                    it6635_fsm_chg(RX_TOGGLE_HPD);
                }
                else
                {
                    #if CHECK_INT_BEFORE_TXOE
                    u8 reg05 = h2rxrd(0x05);
                    u8 reg06 = h2rxrd(0x06);
                    u8 reg07 = h2rxrd(0x07);

                    if ( reg05==0 && reg06 ==0 && reg07 ==0)
                    {
                        prep_count++;
                    }
                    else
                    {
                        PR_ERR(("RX reg: 05=%02X, 06=%02X 07=%02X\r\n", reg05, reg06, reg07));
                        h2rxwr(0x05, reg05);
                        h2rxwr(0x06, reg06);
                        h2rxwr(0x07, reg07);
                        prep_count = 0;
                        prep_fail_count++;
                    }

                    if ( prep_count == 1 )
                    {
                        _sw_hdcp_access_enable(1);//yuan:0
                    }

                    if ( prep_count >= 4 )
                    {
                        prep_count = 0;
                        it6635_fsm_chg(TX_OUTPUT);
                    }
                    else
                    {
                        if ( prep_fail_count > 20)
                        {
                            prep_fail_count = 0;
                            it6635_fsm_chg(RX_WAIT_CLOCK);
                        }
                    }
                    #else
                    it6635_fsm_chg(TX_OUTPUT);
                    #endif
                }
            }
        #endif
            break;

        case RX_CHECK_EQ:
        #if EN_AUTO_RS
            it6635_fsm_EQ_check();
        #endif
            break;

        case SETUP_AFE:
            prep_count = 0;
            prep_fail_count = 0;
            if ( gdev.vars.vclk == 0 )
            {
                it6635_fsm_chg(RX_WAIT_CLOCK);
            }
            else
            {
            #if EN_AUTO_RS
                if ( gdev.vars.try_fixed_EQ )
                {
                    _eq_set_txoe_ready(1);
                    //it6635_fsm_chg(TX_OUTPUT);
                    it6635_fsm_chg(TX_OUTPUT_PREPARE);
                }
                else
                {
                    if ( gdev.opts.EnableAutoEQ )
                    {
                        it6635_fsm_chg(RX_CHECK_EQ);
                    }
                    else
                    {
                        _eq_set_txoe_ready(1);
                        //it6635_fsm_chg(TX_OUTPUT);
                        it6635_fsm_chg(TX_OUTPUT_PREPARE);
                    }
                }
            #else
                _eq_set_txoe_ready(1);
                it6635_fsm_chg(TX_OUTPUT);
                //it6635_fsm_chg(TX_OUTPUT_PREPARE);
            #endif
            }

            break;

        case RX_WAIT_CLOCK:

            if ( _rx_is_clock_stable() )
            {
                _rx_clear_ced_err();
                //_sw_enable_txoe_timer_check();
                //_sw_hdcp_access_enable(0);
                it6635_fsm_chg(SETUP_AFE);
            }
            else
            {
                if ( gdev.vars.RxHPDFlag[gdev.vars.Rx_active_port] == 0 )
                {
                    it6635_fsm_chg(RX_UNPLUG);
                }

                if ( gdev.vars.current_hdmi_mode != HDMI_MODE_AUTO )
                {
                    gdev.vars.count_wait_clock++;
                    if ( gdev.vars.count_wait_clock > 100 )
                    {
                        gdev.vars.count_wait_clock = 0;
                        it6635_auto_detect_hdmi_encoding();
                        it6635_fsm_chg(RX_UNPLUG);
                    }
                }
            }
            break;

        case RX_HPD:
            it6635_fsm_chg(RX_WAIT_CLOCK);
            break;

        case TX_GOT_HPD:
            it6635_fsm_chg(RX_HPD);
            //scdcwr(0x30, 0x01);
            break;

        case TX_WAIT_HPD:
            if ( 0 == _rx_is_5v_active() )
            {
                it6635_fsm_chg_delayed(RX_UNPLUG);
            }
            if ( _tx_is_sink_hpd_high() )
            {
                it6635_fsm_chg(TX_GOT_HPD);
            }
            break;

        case TX_UNPLUG:
            if ( _rx_is_5v_active() )
            {
                it6635_fsm_chg_delayed(TX_WAIT_HPD);
            }
            else
            {
                it6635_fsm_chg_delayed(RX_UNPLUG);
            }
            break;

        case RX_UNPLUG:
            if ( _rx_is_5v_active() )
            {
                if ( gdev.vars.force_hpd_state == SW_HPD_LOW )
                {
                    break;
                }

                if ( gdev.vars.state_sys_fsm != RX_TOGGLE_HPD )
                {
                    it6635_fsm_chg_delayed(TX_WAIT_HPD);
                }
            }
            else
            {
                if ( _rx_get_all_port_5v() )
                {
                    //it6635_fsm_chg2(TX_WAIT_HPD);
                }
            }
            break;

        case IDLE:
            break;
    }

    if ( gdev.vars.fsm_return == 0 )
    {
        gdev.vars.fsm_return = 1;
        goto LOOP_FSM;
    }
}


void it6635_irq(void)
{
    u8 sys_int_sts;
    //u8 port = gdev.vars.TxPortSel;

    #if 0
    static u8 stable = 0;
    static u8 unstable = 0;
    if ( _rx_is_clock_stable() && (1 == _rx_is_all_ch_symlock()) )
    {
        stable++;
        unstable = 0;
        if ( stable > 2 )
        {
            stable = 0;
            it6635_txoe(1);
        }
        #if 0
        idle(20);
        if ( _rx_is_clock_stable() && (1 == _rx_is_all_ch_symlock()) )
        {

        }
        #endif
    }
    else
    {
        unstable++;
        stable = 0;
        if ( unstable > 2 )
        {
            unstable = 0;
            it6635_txoe(0);
        }

        //it6635_txoe(0);
    }
    #endif

    it6635_detect_ports();

    if ( gdev.vars.state_sys_fsm == RX_TOGGLE_HPD )
    {
        return;
    }

    #if 0
    sys_int_sts = h2swrd(0xB3);
    if ( sys_int_sts & 0x80 )
    {
        PR_INFO(("---HDCP acc\r\n"));
        if ( gdev.vars.state_sys_fsm != TX_OUTPUT )
        {
            //_sw_ddc_enable(0);
        }
        _sw_clear_hdcp_status();

    }
    #endif

   //PR_INFO(("--%02X\r\n", h2swrd(0xB3)));
   _sw_clear_hdcp_status();//yuan:////_sw_clear_hdcp_status();

    sys_int_sts = h2swrd(0x0C);

    if ( sys_int_sts == 0x00 )
    {
        return;
    }

    if ( sys_int_sts & 0x01 )
    {
        //for( port=0 ; port<RX_PORT_COUNT ; port++ )
        {
            //it6635_rx_irq(port);

        }

        it6635_rx_irq();


    }

    if ( sys_int_sts & 0x10 )
    {
        it6635_sw_irq(gdev.vars.Rx_active_port);
        it6635_tx_irq();


    }

    #if IT6635_EN_CEC
    if ( gdev.opts.EnCEC && (sys_int_sts & 0x80)  )
    {
        Cec_Irq();
    }
    if(gdev.opts.EnCEC)
    {
        CecSys_TxHandler();
        CecSys_RxHandler();
    }
    #endif
}

bool it6635_device_init(void)
{
    static u8 hpd_wait_count;
    bool init_done = FALSE;

    switch ( gdev.vars.state_dev_init )
    {
        case 0:
            DBG_CLKSTABLE_0();
            DBG_SYMLOCK_0();

            it6635_set_tx_5v(0);

            _sw_reset();
            _rx_set_hpd_all(0);
            _rx_reset();
            _tx_reset();

            it6635_txoe(0);

            _sw_disable_timer0();
            _sw_disable_timer1();
            //_sw_config_timer1(50);
            //_sw_enable_timer1();

            // config default RS
            it6635_set_RS(gdev.EQ.FixedRsIndex[gdev.vars.Rx_active_port]);

            if ( gdev.opts.ParseEDIDFromSink )
            {
                it6635_set_tx_5v(1);
                gdev.vars.state_dev_init = 1;
                hpd_wait_count = 0;
            }
            else
            {
                gdev.vars.state_dev_init = 2;
            }
            break;

        case 1:
            if ( _tx_is_sink_hpd_high() )
            {
                if ( gdev.opts.EnRxDDCBypass == false )
                {
                    if ( it6635_parse_edid(EDID_SRC_EXT_SINK) )
                    {
                        gdev.vars.edid_ready = 1;
                    }
                    gdev.vars.state_dev_init = 3;
                }
                else
                {
                    gdev.vars.edid_ready = 1;
                    gdev.vars.state_dev_init = 3;
                }
            }
            else
            {
                hpd_wait_count++;
                if ( hpd_wait_count > 200 )
                {
                    //gdev.vars.state_dev_init = 2;
                    hpd_wait_count = 0;
                    PR_INFO(("waiting HPD...\r\n"));
                }
                //it6635_set_tx_5v(_rx_is_5v_active());
            }
            break;

        case 2:
            // load FW default EDID
            PR_INFO(("Using internal EDID...\r\n"));
            if ( gdev.opts.EnRxDDCBypass == false )
            {
                if ( gdev.vars.Rev >= 0xC0 )
                {
                    #if ( COPY_EDID_FROM_SINK == 0 )
                    it6635_set_internal_EDID(0, &s_default_edid_block0[0], EDID_PORT_ALL);
                    it6635_set_internal_EDID(1, &s_default_edid_block1_p0[0], EDID_PORT_0);
                    it6635_set_internal_EDID(1, &s_default_edid_block1_p1[0], EDID_PORT_1);
                    it6635_set_internal_EDID(1, &s_default_edid_block1_p2[0], EDID_PORT_2);
                    it6635_set_internal_EDID(1, &s_default_edid_block1_p3[0], EDID_PORT_3);
                    #endif
                }
                else
                {
                    it6635_parse_edid(EDID_SRC_INTERNAL);
                }
            }

            gdev.vars.edid_ready = 1;

            gdev.vars.state_dev_init = 3;

            break;

        case 3:
            _sw_int_enable_all(1);
            if ( gdev.opts.NonActivePortReplyHPD )
            {
                _rx_set_hpd_with_5v_all(true);
            }
            PR_INFO(("Active port = P%d\r\n", gdev.vars.Rx_active_port));

            it6635_set_tx_5v(_rx_is_5v_active());

            init_done = TRUE;
            break;

        default:
            gdev.vars.state_dev_init = 0;
            break;
    }

    return init_done;
}

void it6635_vars_init(void)
{
    //
    // HW options
    //-------------------------------------------------------------------------
    gdev.debug.ForceRxOn = FALSE;      // TRUE: for ATC electrical test

    // Bypass options
    gdev.opts.EnRxDDCBypass = FALSE;
    gdev.opts.EnRxPWR5VBypass = FALSE;
    gdev.opts.EnRxHPDBypass = FALSE;

    // TxSwap
    gdev.opts.EnTxPNSwap = TX_PN_SWAP;
    gdev.opts.EnTxChSwap = TX_PN_SWAP;

    // Tx Output Clock Inverse
    gdev.opts.EnTxVCLKInv = FALSE;

    // Tx Output delay 1T option
    gdev.opts.EnTxOutD1t = FALSE;

    gdev.opts.EnCEC = IT6635_EN_CEC;

    //
    // FW options
    //-------------------------------------------------------------------------
    gdev.opts.EnableAutoEQ = ENABLE_AUTO_EQ;
    gdev.opts.TryFixedEQFirst = TRY_FIXED_EQ;
    gdev.opts.TurnOffTx5VWhenSwitchPort = 1;
    gdev.opts.ParseEDIDFromSink = COPY_EDID_FROM_SINK;
    gdev.opts.DisableEdidRam = (DISABLE_P0_EDID_RAM<<0)|(DISABLE_P1_EDID_RAM<<1)|(DISABLE_P2_EDID_RAM<<2)|(DISABLE_P3_EDID_RAM<<3);
    gdev.opts.FixIncorrectHdmiEnc = 1;

    gdev.EQ.FixedRsIndex[0] = DEFAULT_RS_IDX;
    gdev.EQ.FixedRsIndex[1] = DEFAULT_RS_IDX;
    gdev.EQ.FixedRsIndex[2] = DEFAULT_RS_IDX;
    gdev.EQ.FixedRsIndex[3] = DEFAULT_RS_IDX;
    gdev.vars.Rx_active_port = DEFAULT_PORT;
    gdev.opts.NonActivePortReplyHPD = NON_ACTIVE_PORT_REPLY_HPD;
    gdev.vars.txoe_alignment = TXOE_ALIGN;
    //-------------------------------------------------------------------------


    // FW Variables init:
    gdev.vars.state_dev_init = 0;
    gdev.vars.VSDBOffset = 0xFF;
    gdev.vars.PhyAdr[0] = 0;
    gdev.vars.PhyAdr[1] = 0;
    gdev.vars.PhyAdr[2] = 0;
    gdev.vars.PhyAdr[3] = 0;

    gdev.vars.RxHPDFlag[0] = 1;//-1;
    gdev.vars.RxHPDFlag[1] = 1;//-1;
    gdev.vars.RxHPDFlag[2] = 1;//-1;
    gdev.vars.RxHPDFlag[3] = 1;//-1;

    gdev.vars.count_eq_check = 0;
    gdev.vars.count_fsm_err = 0;
    gdev.vars.count_unlock = 0;
    gdev.vars.state_sys_fsm = RX_UNPLUG;
    gdev.EQ.AutoEQ_state = AEQ_OFF;
    gdev.EQ.sys_aEQ = SysAEQ_RUN;
    gdev.vars.edid_ready = 0;
    gdev.vars.current_txoe = 0xFF;
    gdev.vars.check_for_hpd_toggle = 0;
    gdev.vars.sdi_stable_count = 0;
    gdev.vars.check_for_sdi = 1;
    gdev.vars.force_hpd_state = SW_HPD_AUTO; // 1 : auto, don't modify here

    if ( gdev.opts.TryFixedEQFirst )
    {
        gdev.vars.try_fixed_EQ = 1;
    }

    it6635_auto_detect_hdmi_encoding();
    _eq_reset_state();
    _eq_reset_txoe_ready();
}

#if CHECK_DEV_PRESENT
bool it6635_is_device_lost(void)
{
    u8 vendor_id[2] = { 0 };

    vendor_id[0] = h2swrd(0x00);
    vendor_id[1] = h2swrd(0x01);
    if ( vendor_id[0] == 0x54 && vendor_id[1] == 0x49 )
    {
        return FALSE;
    }
    return TRUE;
}
#endif

bool it6635_is_device_ready(void)
{
    u8 vendor_id[2] = { 0 };

    vendor_id[0] = h2swrd(0x00);
    vendor_id[1] = h2swrd(0x01);
    if ( vendor_id[0] == 0x54 && vendor_id[1] == 0x49 )
    {
        vendor_id[0] = 0;
        vendor_id[1] = 0;

        vendor_id[1] = h2swrd(0x03);
        if ( vendor_id[1] == 0x66 )
        {
            vendor_id[0] = h2swrd(0x02);
            if ( vendor_id[0] == 0x35 )
            {
                gdev.vars.Rev = h2swrd(0x04);
                PR_INFO(("Find 6635 %02x !! \r\n",(int)gdev.vars.Rev));
                return TRUE;
            }
        }
        else if ( vendor_id[1] == 0x35 )
        {
            vendor_id[0] = h2swrd(0x04);
            if ( vendor_id[0] == 0x66 )
            {
                gdev.vars.Rev = h2swrd(0x05);
                PR_INFO(("Find 6635 %02x !! \r\n",(int)gdev.vars.Rev));
                return TRUE;
            }
        }
    }
    PR_INFO(("Find 6635 fail !!\r\n"));

    return FALSE;
}



bool it6635_read_edid(u8 block, u8 offset, int length, u8 *edid_buffer)
{
    bool result = false;
    int off = block * 128 + offset;
    u8 reg3C;
    int retry = 0;


    offset = off % 256;
    reg3C = h2swrd(0x3C);

__RETRY:

    h2swset(0x3C, 0x01, 0x01);    // Enable PC DDC Mode
    h2swset(0x38, 0x08, 0x08);    // Enable DDC Command Fail Interrupt
    h2swset(0x37, 0x80, 0x80);    // Enable DDC Bus Hang Interrupt

    h2swwr(0x3D, 0x09);                  // DDC FIFO Clear
    h2swwr(0x3E, 0xA0);                  // EDID Address
    h2swwr(0x3F, offset);                // EDID Offset
    h2swwr(0x40, length);                // Read ByteNum[7:0]
    h2swwr(0x41, block);           // EDID Segment

    if ( _tx_is_sink_hpd_high() )
    {
        h2swwr(0x3D, 0x03);              // EDID Read Fire

        if ( _tx_ddcwait() )
        {
            h2swbrd(0x42, length, edid_buffer);
            result = true;
        }
        else
        {
            PR_ERR(("ERROR: DDC EDID Read Fail !!!\r\n"));
            if ( retry > 0 )
            {
                retry--;
                delay1ms(100);
                goto __RETRY;
            }
        }
    }
    else
    {
        PR_ERR(("Abort EDID read becasue of detecting unplug !!!\r\n"));
    }

    h2swwr(0x3C, reg3C);   // restore PC DDC Mode

    return result;
}

/**
 * it6635_read_one_block_edid - will read 128 byte EDID data
 *
 * @block: EDID block number. ( 0,1,2 or 3 )
 *
 * @edid_buffer: 128 byte EDID data buffer to store data.
 *
 * Read 128 byte EDID data from assigned block.
 */
bool it6635_read_one_block_edid(u8 block, u8 *edid_buffer)
{
    u8 offset;
    u8 i;
    u8 read_len = 16;

    offset = 0;
    for ( i = 0; i < 128 / read_len; i++ )
    {
        if ( it6635_read_edid(block, offset, read_len, edid_buffer) )
        {
            edid_buffer += read_len;
            offset += read_len;
            continue;
        }
        else
        {
            PR_ERR(("ERROR: read edid block 0, offset %d, length %d fail.\r\r\n",(int)offset,(int)read_len));
            return false;
        }
    }

    return true;
}

void it6635_parse_edid_for_vsdb(u8 *edid)
{
    int off;
    u8 tag;
    u8 len;

    // to find HDMI2.0 VSDB in EDID

    if ( edid[0] == 0x02 ) // CTA ext tag
    {
        off = 4;
        while ( off < 0x100 )
        {
            tag = (edid[off] >> 5) & 0x7;
            len = (edid[off] & 0x1F) + 1;

            if ( tag == 0x03 ) // VSDB
            {
                if ( (edid[off+1] == 0xD8) && (edid[off+2] == 0x5D) && (edid[off+3] == 0xC4) )
                {
                    gdev.vars.is_hdmi20_sink = 1;
                    break;
                }
            }

            if ( len == 0 )
                break;
            off += len;
        }
    }

    PR_INFO(("HDMI2 sink=%d\n", gdev.vars.is_hdmi20_sink));
}
/**
 * it6635_parse_edid_cea_ext - parse necessary data for RX
 * EDID
 *
 * @edid: 128 byte EDID data buffer that contains HDMI CEA ext
 *
 * Before set RX EDID, must call it6635_parse_edid_cea_ext to
 * initialize some variables.
 */
void it6635_parse_edid_cea_ext(u8 *edid)
{
    int off;
    u8 tag;
    u8 len;

    // to find VSDB in EDID

    if ( edid[0] == 0x02 ) // CTA ext tag
    {
        off = 4;
        while ( off < 0x100 )
        {
            tag = (edid[off] >> 5) & 0x7;
            len = (edid[off] & 0x1F) + 1;

            if ( tag == 0x03 ) // VSDB
            {
                if ( (edid[off + 1] == 0x03) && (edid[off + 2] == 0x0C) && (edid[off + 3] == 0x00) )
                //(edid[off+1] == 0xD8) && (edid[off+2] == 0x5D) && (edid[off+3] == 0xC4) )
                {
                    gdev.vars.VSDBOffset = ((u8)off) + 0x80 + 4;
                    gdev.vars.PhyAdr[0] = (edid[off + 4] >> 4) & 0xF;
                    gdev.vars.PhyAdr[1] = edid[off + 4] & 0xF;
                    gdev.vars.PhyAdr[2] = (edid[off + 5] >> 4) & 0xF;
                    gdev.vars.PhyAdr[3] = edid[off + 5] & 0xF;
                    gdev.vars.EdidChkSum[1] = (0x100 - edid[0x7F] - edid[off + 4] - edid[off + 5]) & 0xFF;
                    break;
                }
            }

            if ( len == 0 )
                break;
            off += len;
        }
    }
}

/**
 * it6635_rx_set_edid - write data to EDID RAM
 *
 * @edid: 128 byte EDID data buffer.
 *
 * @block: EDID block number (0,1,2 or 3)
 *
 * Before calling it6635_rx_set_edid() with block==1, must call
 * it6635_parse_edid_cea_ext to initialize some variables.
 */
void it6635_rx_set_edid(u8 *edid, u8 block)
{
    int i;
    u8 wcount = 16;
    u8 phyAB, phyCD;
    u8 mask;
    u16 sum;

    PR_INFO(("Set RX Internal EDID ...\r\n"));

    // RegEnEDIDSlaveAdr=1
    h2rxwr(0x4B, RXEDIDAddr | 0x01);
    h2rxset(0x4C, 0x0F, 0x0F);

    for ( i = 0; i < 128; i += wcount )
    {
        h2rxedidwr(i + 128 * block, edid, wcount);
        edid += wcount;
    }

    if ( block == 0 )
    {
        for ( i = 0; i < 4; i++ )
        {
            _rx_edid_set_chksum(i, gdev.vars.EdidChkSum[0]);
        }
    }

    if ( block == 1 )
    {
        // todo  : how to handle VSDB offset not found ?
        h2swwr(0xe9, gdev.vars.VSDBOffset);    // VSDB_Offset

        phyAB = (gdev.vars.PhyAdr[0] << 4) | (gdev.vars.PhyAdr[1] & 0xF);
        phyCD = (gdev.vars.PhyAdr[2] << 4) | (gdev.vars.PhyAdr[3] & 0xF);

        for ( i = 0; i < 4; i++ )
        {
            h2swwr(0xd9 + i * 2, phyAB);    // Port0 VSDB_AB
            h2swwr(0xda + i * 2, phyCD);    // Port0 VSDB_CD
        }

        for ( i = 0; i < 4; i++ )
        {
            if ( gdev.vars.PhyAdr[i] == 0 )
            {
                mask = 0xF0 >> (4 * (i & 0x01));
                for ( wcount = 0; wcount < 4; wcount++ )
                {
                    phyAB = wcount + 1;
                    if ( mask == 0xF0 )
                    {
                        phyAB = phyAB << 4;
                    }
                    h2swset(0xd9 + wcount * 2 + i / 2, mask, phyAB);
                }
                break;
            }
        }

        for ( i = 0; i < 4; i++ )
        {
            phyAB = h2swrd(0xd9 + i * 2);    // Port(i) VSDB_AB
            phyCD = h2swrd(0xda + i * 2);    // Port(i) VSDB_CD

            // Port(i) Block1_ChkSum
            sum = (0x100 - gdev.vars.EdidChkSum[1] - phyAB - phyCD) & 0xFF;
            h2swwr(0xe2 + i * 2, (u8)sum);

            //if ( gdev.vars.Rev >= 0xC )
            {
                mask = 1<<i;
                h2rxset(0x4C, 0x0F, mask);

                h2rxedidwr(gdev.vars.VSDBOffset, &phyAB, 1);
                h2rxedidwr(gdev.vars.VSDBOffset+1, &phyCD, 1);
                phyAB = (u8)sum;
                h2rxedidwr(128+127, &phyAB, 1);
            }

        }
    }

    // RegEnEDIDSlaveAdr=0
    h2rxwr(0x4B, RXEDIDAddr);
}


bool it6635_parse_edid(u8 edid_source)
{
    u8 edid_tmp[128];
    u8 extblock;
    u8 i;
    u16 j;

    gdev.vars.spmon = 0;
    gdev.vars.is_hdmi20_sink = 0;

    if ( edid_source == EDID_SRC_EXT_SINK )
    {
        if ( false == it6635_read_one_block_edid(0, edid_tmp) )
        {
            PR_ERR(("ERROR: read edid block 0\r\n"));
            goto __err_exit;
        }
    }
    else
    {
        for (i=0 ; i<128 ; i++)
        {
            edid_tmp[i] = s_default_edid_block0[i];
        }
    }

    if ( (edid_tmp[0x08] == 0x5A) &&
         (edid_tmp[0x09] == 0x63) &&
         (edid_tmp[0x0a] == 0x32) &&
         (edid_tmp[0x0b] == 0x0e) )
    {
        gdev.vars.spmon = 1;
    }

    if ( (edid_tmp[0x71] == 0x4C) &&
         (edid_tmp[0x72] == 0x47) &&
         (edid_tmp[0x74] == 0x54) &&
         (edid_tmp[0x75] == 0x56) )
    {
        gdev.vars.spmon = 2;
    }


    // read Ext block no
    extblock = edid_tmp[0x7E];
    gdev.vars.EdidChkSum[0] = edid_tmp[0x7F];

    #if FIX_EDID_FOR_ATC_4BLOCK_CTS
    if ( extblock > 1 )
    {
        edid_tmp[0x7E] = 1;
        gdev.vars.EdidChkSum[0] = _rx_calc_edid_sum(edid_tmp);
    }
    #endif

    _pr_buf(edid_tmp, 128);
    it6635_rx_set_edid(edid_tmp, 0);

    if ( extblock > 3 )
    {
        PR_ERR(("Warning: Extblock = %d\r\n",(int)extblock));
        extblock = 3;
    }

    for ( i = 1; i <= extblock; i++ )
    {
        if ( edid_source == EDID_SRC_EXT_SINK )
        {
            if ( false == it6635_read_one_block_edid(i, edid_tmp) )
            {
                PR_ERR(("ERROR: read edid block %d\r\n",(int)i));
                goto __err_exit;
            }

            #if DO_TX_EDID_ERROR_TO_LOAD_DEFAULT
            if((i==1)&&(edid_tmp[0]!=2&&edid_tmp[1]!=3))
            {
                PR_ERR(("ERROR: Edid Data error, Load Default %d\r\n",(int)i));
                for (j=0 ; j<128 ; j++)
                {
                    edid_tmp[j] = s_default_edid_block1_p0[j+i*128-128];
                }
            }
            #endif
        }
        else
        {
            for (j=0 ; j<128 ; j++)
            {
                edid_tmp[j] = s_default_edid_block1_p0[j+i*128-128];
            }
        }

        gdev.vars.VSDBOffset = 0;

        if ( i==1 ) // assume our sink has only 2 block EDID
        {
            if ( gdev.vars.spmon == 2 )
            {
                if ( edid_tmp[0x7F] != 0x6A)
                {
                    gdev.vars.spmon = 0;
                }
            }

            _pr_buf(edid_tmp, 128);
            it6635_parse_edid_cea_ext(edid_tmp);

            if ( gdev.vars.VSDBOffset )
            {
                it6635_rx_set_edid(edid_tmp, i);

                // break;
                // break parsing here will make the 4 block EDID CTS fail.
            }
            else
            {
                it6635_set_internal_EDID(1, edid_tmp, EDID_PORT_ALL);
            }
        }
    } // for i

    #if FIX_EDID_FOR_ATC_4BLOCK_CTS

    if ( ( gdev.vars.VSDBOffset == 0 ) &&
         ( extblock > 1 ) ) // to pass CTS 4 block EDID test
    {
        // set block 1 to default EDID
        for (j=0 ; j<128 ; j++)
        {
            edid_tmp[j] = s_default_edid_block1_p0[j];
        }
        gdev.vars.VSDBOffset = 0;
        it6635_parse_edid_cea_ext(edid_tmp);
        it6635_rx_set_edid(edid_tmp, 1);
    }

    #else
    if ( gdev.vars.VSDBOffset == 0 )
    {
        PR_ERR(("VSDB offset not found\r\n"));
    }
    #endif

    return true;

__err_exit:
    return false;
}

u8 g_u8it6635NewPort = 0;
bool g_bit6635SwitchFlag = FALSE;
void it6635_SwitchPort(u8 u8Port)
{
    g_u8it6635NewPort = u8Port;
    g_bit6635SwitchFlag = TRUE;
}
#if 0
u8  g_u8Ite66351RxHdcpStatus[_HDMI_INPUT_PORT_USED_NUMBER] = {0,0,0};
#endif
vvFuncPoint pvvFuncIT66351AutoDetectPort = NULL;

void it6635_AutoDetectPortSource(void)
{
    #if 0
    u8 i,preIte66351RxHdcpStatus[_HDMI_INPUT_PORT_USED_NUMBER];//,preTxHpd_Detect;

    for(i=0;i<_HDMI_INPUT_PORT_USED_NUMBER;i++)
        preIte66351RxHdcpStatus[i]=isGetHpdDetConnect(i);

    for(i=0;i<_HDMI_INPUT_PORT_USED_NUMBER;i++)
    {
        if(preIte66351RxHdcpStatus[i]!=g_u8Ite66351RxHdcpStatus[i])
            if(preIte66351RxHdcpStatus[i])
            {
                MFP31_SetSwitchPort(i);
            }
            else if(i==g_u8it6635NewPort)
            {
                if(isGetHpdDetConnect(2))
                    MFP31_SetSwitchPort(2);
                else if(isGetHpdDetConnect(0))
                    MFP31_SetSwitchPort(0);
                else if(isGetHpdDetConnect(1))
                    MFP31_SetSwitchPort(1);
            }
    }

    for(i=0;i<_HDMI_INPUT_PORT_USED_NUMBER;i++)
        g_u8Ite66351RxHdcpStatus[i]=preIte66351RxHdcpStatus[i];
    #else
    if(pvvFuncIT66351AutoDetectPort != NULL)
    {
        pvvFuncIT66351AutoDetectPort();
    }
    #endif
}

static u8 It6635_RxPortsDefinde[]={P0,P3,P2};
void it6635_PortSwitchProcess(void)
{
    if(g_bit6635SwitchFlag)
    {
        g_bit6635SwitchFlag = FALSE;
        PR_INFO(("it66351 switch to port: %d\r\n",It6635_RxPortsDefinde[g_u8it6635NewPort]));
        {
            u8 i;
            u8 l_u85Vsta=0;
            u8 l_u8SigSta=0;
            for(i=0;i<=P3;i++)
            {
                l_u85Vsta=it6635_is_5v_present(i)<<i;
                l_u8SigSta=it6635_is_clock_detected(i)<<i;
            }
            PR_INFO(("it66351 Rx port 5V sta: %x\r\n",l_u85Vsta));
            PR_INFO(("it66351 Rx port Sig sta: %x\r\n",l_u8SigSta));
            PR_INFO(("it66351 Rx 5V sta: %d\r\n",_rx_is_5v_active()));

        }
        //_rx_set_hpd_all(0);
        _rx_set_hpd(It6635_RxPortsDefinde[g_u8it6635NewPort], 0, TERM_FOLLOW_HPD);
        delay1ms(200);//yuan:
        it6635_set_active_port(It6635_RxPortsDefinde[g_u8it6635NewPort]);
    }
}

void it6635_dev_loop(void)
{
    #if CHECK_DEV_PRESENT
    if ( dev_state < DEV_WAIT_DEVICE_READY ) {
        if ( it6635_is_device_lost() )
        {
            dev_state = DEV_FW_VAR_INIT;
        }
    }
    #endif

    switch ( dev_state )
    {
        case DEV_DEVICE_LOOP:
            it6635_fsm();
            it6635_irq();
            _rx_update_ced_err_from_hw();
            it6635_AutoDetectPortSource();
            it6635_PortSwitchProcess();
            break;

        case DEV_DEVICE_INIT:
            if ( it6635_device_init() )
            {
                dev_state = DEV_DEVICE_LOOP;
            }
            break;

        case DEV_WAIT_DEVICE_READY:
            if ( it6635_is_device_ready() )
            {
                dev_state = DEV_DEVICE_INIT;
            }
            break;

        case DEV_FW_VAR_INIT:
            it6635_vars_init();
            dev_state = DEV_WAIT_DEVICE_READY;
            break;

        default:
            break;
    }

}

///////

//
// APIs:
//

void it6635_dump_register(void)
{
    #define BUF_LEN 16
    u8 regbuf[BUF_LEN];
    int i,j;


    pr_info("\n*** Switch Register:\n");
    pr_info("   | ");
    for (j=0;j<BUF_LEN;j++)
    {
        if ( j==(BUF_LEN/2) )
        {
            pr_info("- ");
        }
        pr_info("%02X ", j);
    }
    pr_info("\n");
    for (j=0;j<BUF_LEN;j++)
    {
        if ( j==(BUF_LEN/2) )
        {
            pr_info("- ");
        }
        pr_info("---", j);
    }
    pr_info("\n");

    for (i=0;i<256;i+=BUF_LEN)
    {
        pr_info("%02X | ", i);
        h2swbrd( i, BUF_LEN, regbuf );
        for (j=0;j<BUF_LEN;j++)
        {
            if ( j==(BUF_LEN/2) )
            {
                pr_info("- ");
            }
            pr_info("%02X ", regbuf[j]);
        }
        pr_info("\n");
    }
    pr_info("\n");

    pr_info("\n*** RX Register:\n");
    pr_info("   | ");
    for (j=0;j<BUF_LEN;j++)
    {
        if ( j==(BUF_LEN/2) )
        {
            pr_info("- ");
        }
        pr_info("%02X ", j);
    }
    pr_info("\n");
    for (j=0;j<BUF_LEN;j++)
    {
        if ( j==(BUF_LEN/2) )
        {
            pr_info("- ");
        }
        pr_info("---", j);
    }
    pr_info("\n");

    for (i=0;i<256;i+=BUF_LEN)
    {
        pr_info("%02X | ", i);
        h2rxbrd( i, BUF_LEN, regbuf );
        for (j=0;j<BUF_LEN;j++)
        {
            if ( j==(BUF_LEN/2) )
            {
                pr_info("- ");
            }
            pr_info("%02X ", regbuf[j]);
        }
        pr_info("\n");
    }
    pr_info("\n");

    //pr_info("\nADDR:0xB2:\n");
    pr_info("   | ");
    for (j=0;j<BUF_LEN;j++)
    {
        if ( j==(BUF_LEN/2) )
        {
            //pr_info("- ");
        }
        //pr_info("%02X ", j);
    }
    pr_info("\n");
    for (j=0;j<BUF_LEN;j++)
    {
        if ( j==(BUF_LEN/2) )
        {
            //pr_info("- ");
        }
        //pr_info("---", j);
    }
    pr_info("\n");

    chgrxbank(3);
    for (i=0;i<256;i+=BUF_LEN)
    {
        pr_info("%03X| ", i+0x300);
        h2rxbrd( i, BUF_LEN, regbuf );
        for (j=0;j<BUF_LEN;j++)
        {
            if ( j==(BUF_LEN/2) )
            {
                pr_info("- ");
            }
            pr_info("%02X ", regbuf[j]);
        }
        pr_info("\n");
    }
    pr_info("\n");

    //pr_info("\nADDR:0xB2:\n");
    pr_info("   | ");
    for (j=0;j<BUF_LEN;j++)
    {
        if ( j==(BUF_LEN/2) )
        {
            //pr_info("- ");
        }
        //pr_info("%02X ", j);
    }
    pr_info("\n");
    for (j=0;j<BUF_LEN;j++)
    {
        if ( j==(BUF_LEN/2) )
        {
            //pr_info("- ");
        }
        //pr_info("---", j);
    }
    pr_info("\n");

    chgrxbank(5);
    for (i=0xA0;i<256;i+=BUF_LEN)
    {
        pr_info("%03X| ", i+0x500);
        h2rxbrd( i, BUF_LEN, regbuf );
        for (j=0;j<BUF_LEN;j++)
        {
            if ( j==(BUF_LEN/2) )
            {
                pr_info("- ");
            }
            pr_info("%02X ", regbuf[j]);
        }
        pr_info("\n");
    }
    pr_info("\n");

    chgrxbank(0);

    #if IT6635_EN_CEC
    pr_info("\n*** CEC Register:\n");
    pr_info("   | ");
    for (j=0;j<BUF_LEN;j++)
    {
        if ( j==(BUF_LEN/2) )
        {
            pr_info("- ");
        }
        pr_info("%02X ", j);
    }
    pr_info("\n");
    for (j=0;j<BUF_LEN;j++)
    {
        if ( j==(BUF_LEN/2) )
        {
            pr_info("- ");
        }
        pr_info("---", j);
    }
    pr_info("\n");

    for (i=0;i<256;i+=BUF_LEN)
    {
        pr_info("%02X | ", i);
        cecbrd( i, BUF_LEN, regbuf );
        for (j=0;j<BUF_LEN;j++)
        {
            if ( j==(BUF_LEN/2) )
            {
                pr_info("- ");
            }
            pr_info("%02X ", regbuf[j]);
        }
        pr_info("\n");
    }
    pr_info("\n");
    #endif
}

bool it6635_write_edid(u8 block, u8 offset, int length, u8 *data_buffer)
{
    bool result = false;
    int off = block * 128 + offset;
    u8 reg3C;
    u8 segment = off / 256;

    offset = off % 256;
    reg3C = h2swrd(0x3C);

    h2swset(0xF5, 0x80, (1 << 7));
    h2swset(0x3C, 0x01, 0x01); // disable DDCRegen by set RegTxMastersel=1

    h2swset(0x3C, 0x01, 0x01);    // Enable PC DDC Mode
    h2swset(0x38, 0x08, 0x08);    // Enable DDC Command Fail Interrupt
    h2swset(0x37, 0x80, 0x80);    // Enable DDC Bus Hang Interrupt

    h2swwr(0x3D, 0x09);                  // DDC FIFO Clear
    h2swwr(0x3E, 0xA0);                  // EDID Address
    h2swwr(0x3F, offset);                // EDID Offset
    h2swwr(0x40, length);                // Read ByteNum[7:0]
    h2swwr(0x41, segment / 2);           // EDID Segment

    while ( length )
    {
        h2swwr(0x42, *data_buffer);
        length--;
        data_buffer++;
    }

    h2swwr(0x3D, 0x07);              // EDID Write Fire

    if ( _tx_ddcwait() )
    {
        result = true;
    }
    else
    {
        PR_ERR(("ERROR: DDC EDID Write Fail !!!\r\n"));
    }

    h2swwr(0x3C, reg3C);   // restore PC DDC Mode

    h2swset(0xF5, 0x80, (gdev.opts.EnRxDDCBypass << 7));
    if ( gdev.opts.EnRxDDCBypass == 0 )
    {
        h2swset(0x3C, 0x01, 0x00); // enable DDCRegen by set RegTxMastersel=1
    }

    return result;
}

bool it6635_write_one_block_edid(u8 block, u8 *edid_buffer)
{
    u8 offset;
    u8 i;
    u8 op_len = 16;

    offset = 0;
    for ( i = 0; i < 128 / op_len; i++ )
    {
        if ( it6635_write_edid(block, offset, op_len, edid_buffer) )
        {
            edid_buffer += op_len;
            offset += op_len;
            continue;
        }
        else
        {
            PR_ERR(("ERROR: write edid block 0, offset %d, length %d fail.\r\r\n",(int)offset,(int)op_len));
            return false;
        }
    }

    return true;
}

u8 it6635_is_5v_present(u8 port)
{
    if ( port <= P3 )
    {
        if ( it6635_get_port_info0( port, PI_5V ) )
        {
            return 1;
        }
    }
    else
    {
        PR_ERR(("Invalid port number:%d\r\n", port));
    }

    return 0;
}

u8 it6635_is_clock_detected(u8 port)
{
    if ( port <= P3 )
    {
        if ( it6635_get_port_info0( port, (PI_CLK_STABLE | PI_CLK_VALID | PI_5V) ) )
        {
            return 1;
        }
    }
    else
    {
        PR_ERR(("Invalid port number:%d\r\n", port));
    }

    return 0;
}

bool it6635_set_active_port(u8 port)
{
    if ( port <= P3 )
    {
        if ( gdev.vars.Rx_active_port != port )
        {
            gdev.vars.Rx_new_port = port;
            it6635_fsm_chg_delayed(RX_PORT_CHANGE);

            return TRUE;
        }
    }
    else
    {
        PR_ERR(("Invalid port number:%d\r\n", port));
    }

    return FALSE;
}

unsigned char it6635_get_active_port(void)
{
    return gdev.vars.Rx_active_port;
}

void it6635_set_option(IT6635_DEVICE_OPTION *Opts)
{
    if ( Opts )
    {
        gdev.opts.EnableAutoEQ = Opts->EnableAutoEQ;
        gdev.opts.EnRxDDCBypass = Opts->EnRxDDCBypass;
    }
}

void it6635_get_option(IT6635_DEVICE_OPTION *Opts)
{
    if ( Opts )
    {
        Opts->EnableAutoEQ = gdev.opts.EnableAutoEQ;
        Opts->EnRxDDCBypass = gdev.opts.EnRxDDCBypass;
    }
}

void it6635_dev_restart(void)
{
    //gdev.vars.Rx_prev_port = -1;
    gdev.vars.state_sys_fsm = RX_UNPLUG;

    dev_state = DEV_WAIT_DEVICE_READY;
}

u8 it6635_get_RS(void)
{
    return gdev.EQ.FixedRsIndex[gdev.vars.Rx_active_port];
}

void it6635_set_RS(u8 rs_index)
{
    u8 rs[3];
    if ( rs_index < 14 )
    {
        gdev.EQ.FixedRsIndex[gdev.vars.Rx_active_port] = rs_index;

        rs[0] = RS_Value[rs_index] | 0x80;
        rs[1] = RS_Value[rs_index] | 0x80;
        rs[2] = RS_Value[rs_index] | 0x80;
        _rx_set_rs_3ch(rs);

        chgrxbank(3);
        h2rxbrd(0x27, 3, rs);
        //h2rxset(0x22, 0x40, 0x00);
        chgrxbank(0);
        PR_INFO(("==> RS set to %d, %02x %02x %02x\r\n", gdev.EQ.FixedRsIndex[gdev.vars.Rx_active_port], rs[0], rs[1], rs[2]));
    }
}

void it6635_set_ch_RS(u8 ch, u8 rs_index)
{
    u8 rs;
    if ( rs_index < 14 )
    {
        rs = RS_Value[rs_index] | 0x80;
        _rx_set_rs(ch, rs);
    }
}

void it6635_set_rx_hpd(u8 hpd_value)
{
    _rx_set_hpd(gdev.vars.Rx_active_port, hpd_value, TERM_FOLLOW_HPD);
}

/**
 * it6635_set_internal_EDID - write data to EDID RAM
 *
 * @edid: 128 byte EDID data buffer.
 *
 * @block: EDID block number (0,1,2 or 3)
 *
 * Calling it6635_set_internal_EDID() with block==0, target_port
 * will be ignored.
 *
 * when block == 1, target_port is a bitmap from 0x1 to 0xF
 *
 * ex: set port 1 EDID: target_port = EDID_PORT_1
 *     set port 1,3 EDID: target_port = EDID_PORT_1|EDID_PORT_3
 */
void it6635_set_internal_EDID( u8 block, u8 const *edid, u8 target_port )
{
    int i;
    u8 wcount = 16;

    if ( block > 1 )
    {
        PR_ERR(("Invalid block %d\r\n", block));
        return;
    }

    h2rxwr(0x4B, RXEDIDAddr | 0x01); // enable EDID RAM i2c address

    if ( (block == 0)  )
    {
        h2rxset(0x4C, 0x0F, 0x0F); // for block 0, all ports share 128 byte RAM
    }
    else if ( (block == 1)  )
    {
        h2rxset(0x4C, 0x0F, target_port);  // for block 1, select port to be written
    }

    for ( i = 0; i < 128; i += wcount )
    {
        h2rxedidwr(i + 128 * block, (u8 *)edid, wcount);
        edid += wcount;
    }

    h2rxwr(0x4B, RXEDIDAddr);  // disable EDID RAM i2c address
}

/**
 * it6635_get_internal_EDID - read data from EDID RAM
 *
 * @edid: 128 byte EDID data buffer.
 *
 * @block: EDID block number (0,1,2 or 3)
 *
 * Calling it6635_get_internal_EDID() with block==0, target_port
 * will be ignored.
 *
 * when block == 1, target_port can be EDID_PORT_0 to EDID_PORT_3
 *               and only 1 port at a time.
 */
void it6635_get_internal_EDID( u8 block, u8 *edid, u8 target_port )
{
    int i;
    u8 wcount = 16;
    u8 tport;

    if ( block > 1 )
    {
        PR_ERR(("Invalid block %d\r\n", block));
        return;
    }

    h2rxwr(0x4B, RXEDIDAddr | 0x01); // enable EDID RAM i2c address

    if ( (block == 1)  )
    {
        switch( target_port )
        {
            case EDID_PORT_1:
                tport = 1;
                break;

            case EDID_PORT_2:
                tport = 2;
                break;

            case EDID_PORT_3:
                tport = 3;
                break;

            default:
                tport = 0;
                break;
        }

        h2rxset(0x4C, 0x30, tport<<4); // for block 1, select port to be read
    }

    for ( i = 0; i < 128; i += wcount )
    {
        h2rxedidrd(i + 128 * block, edid, wcount);
        edid += wcount;
    }

    h2rxwr(0x4B, RXEDIDAddr); // disable EDID RAM i2c address
}


/**
 * it6635_change_default_RS - set the default RS index for each
 * port
 *
 * @port: port number can be P0~P3.
 *
 * @new_rs_idx: RS index from 0 to 13
 *
 * @update_hw: 0: only update the vaule in RAM
 *             1: update the value to RAM and Hardware register
 *                ( for active port only )
 *
 */
void it6635_change_default_RS( u8 port, u8 new_rs_idx, u8 update_hw )
{
    if ( port <= P3 )
    {
        gdev.EQ.FixedRsIndex[port] = new_rs_idx;
        if ( update_hw && (port == gdev.vars.Rx_active_port) )
        {
            it6635_set_RS( new_rs_idx );
        }
    }
    else
    {
        PR_ERR(("Invalid port number:%d\r\n", port));
    }
}


/**
 * it6635_force_rx_hpd :
 * to force active port HPD low or auto control by driver
 *
 * @hpd_state: 0: Force HPD of active port to low
 *             1: HPD of active port is controlled by it6635
 *                driver
 *
 * gdev.vars.force_hpd_state will reset to SW_HPD_AUTO when
 * active port changed by it6635_fsm_chg(RX_PORT_CHANGE)
 *
 */
void it6635_force_rx_hpd( u8 hpd_state )
{
    gdev.vars.force_hpd_state = hpd_state;

    if ( hpd_state ) // hpd 0 --> hpd auto
    {
        // nothing to do here:
        // hpd will be controlled by it6635_fsm()
    }
    else // hpd auto --> hpd 0
    {
        _rx_set_hpd(gdev.vars.Rx_active_port, hpd_state, TERM_FOLLOW_HPD);
        it6635_fsm_chg_delayed(RX_UNPLUG);
    }
}


/**
 * it6635_toggle_hpd : to make HPD toggle for active port with a
 * given duration.
 *
 * @ms_duration: duration of HPD low in millisecond.
 * range from 10ms to 12700ms
 *
 */
bool it6635_toggle_hpd(u16 ms_duration)
{
    u8 timeout;
    bool ret=true;

    if ( ms_duration <= (0x7F*10) )
    {
        timeout = ms_duration/10;
    }
    else if ( ms_duration <= (0x7F*100) )
    {
        timeout = ms_duration/100;
        timeout |= (BIT7);
    }
    else
    {
        ret = false;
    }

    _rx_set_hpd(gdev.vars.Rx_active_port, 0, TERM_FOLLOW_HPD);
    _tx_scdc_write(0x20,0x00);
    gdev.vars.hpd_toggle_timeout = timeout;
    it6635_fsm_chg(RX_TOGGLE_HPD);

    return ret;
}
