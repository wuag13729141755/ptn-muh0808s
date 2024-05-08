///*****************************************
//  Copyright (C) 2009-2020
//  ITE Tech. Inc. All Rights Reserved
//  Proprietary and Confidential
///*****************************************
//   @file   <it6635.c>
//   @author Kenneth.Hung@ite.com.tw
//   @date   2020/12/30
//   @fileversion: IT66353_SAMPLE_1.06
//******************************************/
#define _SHOW_PRAGMA_MSG
#include "IT66353/chip/IT66353_config.h"
#include "IT66353/chip/IT66353_platform.h"
#include "IT66353/chip/IT66353_debug.h"
#include "IT66353/chip/it66353_drv.h"
#include "IT66353/chip/it66353_EQ.h"
#include "IT66353/chip/it66353.h"

//
// ********* compile options ***********
//
// CHECK_DEV_PRESENT:
// 1: FW will restart when device ID check failed.
//
#define CHECK_DEV_PRESENT 0

// ********* compile options end *******

#if DEBUG_FSM_CHANGE
#define __init_str_SYS_FSM_STATE
#include "IT66353/chip/IT66353_drv_h_str.h"
#endif

//for CEC
#if IT6635_EN_CEC
#include "IT66353/chip/it66353_cec.h"
#include "IT66353/Customer/IT66353_CecSys.h"
#endif
//for CEC

static u8 g_is_set_io[IT6635_I2C_DEV_NUM] = {0};
u8 dev_state = DEV_FW_VAR_INIT;
IT6635_DEVICE_DATA gdev[IT6635_I2C_DEV_NUM];
u8 g_u8ITEDevice;
//////////////////////add by wf8421 20211230///////////////////////////////////////
IT6635_DEVICE_DATA* g_pstDev = NULL;
static u8* sg_pIsSetIO = NULL;
IT6635_DEV_ADDR *g_pstDevAddr = NULL;
vvFuncPoint pvpmappIT6635XUserFunction = NULL;
static bool sg_bIT6635HoldSystemFlag = FALSE;

extern unsigned char g_i2c_dev_no;
u8 g_u8IT6635XDeviceTable[IT6635_I2C_DEV_NUM]=
{
    _I2C_TABLE_INDEX_IT6635X_0,
#if (IT6635_I2C_DEV_NUM >= 2)
    _I2C_TABLE_INDEX_IT6635X_1,
#endif
#if (IT6635_I2C_DEV_NUM >= 3)
    _I2C_TABLE_INDEX_IT6635X_2,
#endif
#if (IT6635_I2C_DEV_NUM >= 4)
    _I2C_TABLE_INDEX_IT6635X_3,
#endif
};

IT6635_DEV_ADDR g_stIT6635DevAddr[IT6635_I2C_DEV_NUM] =
{
    {
        _IT6635X_SWADDR_0,
        _IT6635X_RXADDR_0,
        _IT6635X_CECADDR_0,
        _IT6635X_EDIDADDR_0
    },
#if (IT6635_I2C_DEV_NUM >= 2)
    {
        _IT6635X_SWADDR_1,
        _IT6635X_RXADDR_1,
        _IT6635X_CECADDR_1,
        _IT6635X_EDIDADDR_1
    },
#endif
#if (IT6635_I2C_DEV_NUM >= 3)
    {
        _IT6635X_SWADDR_2,
        _IT6635X_RXADDR_2,
        _IT6635X_CECADDR_2,
        _IT6635X_EDIDADDR_2
    },
#endif
#if (IT6635_I2C_DEV_NUM >= 4)
    {
        _IT6635X_SWADDR_3,
        _IT6635X_RXADDR_3,
        _IT6635X_CECADDR_3,
        _IT6635X_EDIDADDR_3
    },
#endif
};

////////////////////////////////////////////////////////////////////////////////

u8 g_enable_io_log = 0;
#define PR_IO(x)  { if (g_enable_io_log)  pr_info x ; }
//#define PR_IO(x)

void it6635_DeviceSelect(u8 u8DevID)
{
    g_u8ITEDevice = u8DevID;
    g_i2c_dev_no = g_u8IT6635XDeviceTable[u8DevID];
    g_pstDev = &gdev[u8DevID];
    sg_pIsSetIO = &g_is_set_io[u8DevID];
    g_pstDevAddr = &g_stIT6635DevAddr[u8DevID];
}

void it6635_init(void)
{
    u8 i;
    for(i=0;i<IT6635_I2C_DEV_NUM;i++)
    {
        it6635_DeviceSelect(i);
        it6635_options_init();
    }
}

void it6635_SetHoldSystem(bool bSta)
{
    sg_bIT6635HoldSystemFlag = bSta;
}

bool it6635_GetHoldSystemSta(void)
{
    return sg_bIT6635HoldSystemFlag;
}

int hold_system(void)
{
    if(it6635_GetHoldSystemSta())
        return 1;
    else
        return 0;
}

u32 it6635_DiffrentTime(u32 u32NowTime,u32 u32OldTime)
{
    u32 l_u32DiffTime = 0;
    if(u32NowTime > u32OldTime)
        l_u32DiffTime = u32NowTime-u32OldTime;
    else
        l_u32DiffTime = (~u32OldTime)+u32NowTime+1;

    return l_u32DiffTime;
}

void it6635_main(void)
{
    static u8 sl_u8DumpFlag = 1;
    static u32 sl_u32Rutime = 0;
    static bool sl_bStartupFlag = TRUE;

    if(sl_bStartupFlag)
    {
        sl_bStartupFlag = FALSE;
        sl_u32Rutime = get_tick_count();
    }

    if ( 0 == hold_system() )
    {
        sl_u8DumpFlag = 1;

        if ( it6635_DiffrentTime(get_tick_count(),sl_u32Rutime) >= 50) // handling event every 50ms
        {
            sl_u32Rutime = get_tick_count();
            it6635_dev_loop();
        }
    }
    else
    {
        if ( sl_u8DumpFlag )
        {
            u8 l_u8DevID;

            sl_u8DumpFlag = 0;
            for(l_u8DevID = 0;l_u8DevID<IT6635_I2C_DEV_NUM;l_u8DevID++)
            {
                it6635_DeviceSelect(l_u8DevID);
                it6635_dump_register_all();
            }
        }
    }
}

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
            PR_DATA(("0x%02x ",*buf));
            buf++;
        }
        PR_DATA(("\r\n"));
        length -= pr_len;
    }
}

u8 h2swwr(u8 offset, u8 wdata)
{
    if ( (*sg_pIsSetIO) == 0 ) {
        //PR_IO(("    ---> h2swwr(0x%02x, 0x%02x);\r\n", offset, wdata));
        //PR51_IO(("    ---> h2swwr(0x%02Bx, 0x%02Bx);\n", offset, wdata));
        PR_IO(("    w %02x %02x %02x\r\n", offset, wdata, g_pstDev->opts.dev_opt->SwAddr));
    }
    else{
        (*sg_pIsSetIO) = 0;
    }

    if ( (g_pstDev->opts.active_rx_opt->EnRxDDCBypass) && (offset == 0x3C) )
    {
    	PR_ERR(("****h2swwr 0x3C 0x%02x\n", wdata));
    }

    return it6635_i2c_write(g_pstDev->opts.dev_opt->SwAddr, offset, 1, &wdata);
}

u8 h2swrd(u8 offset)
{
    u8 rddata;

    it6635_i2c_read(g_pstDev->opts.dev_opt->SwAddr, offset, 1, &rddata);

    return rddata;
}

u8 h2swset(u8 offset, u8 mask, u8 wdata)
{
    u8 temp;

    (*sg_pIsSetIO) = 1;
    //PR_IO(("    ---> h2swset(0x%02x, 0x%02x, 0x%02x);\r\n", offset, mask, wdata));
    //PR51_IO(("    ---> h2swset(0x%02Bx, 0x%02Bx, 0x%02Bx);\n", offset, mask, wdata));
    PR_IO(("    s %02x %02x %02x %02x\n", offset, mask, wdata, g_pstDev->opts.dev_opt->SwAddr));

    temp = h2swrd(offset);
    temp = (temp & ((~mask) & 0xFF)) + (mask & wdata);

    return h2swwr(offset, temp);
}

void h2swbrd(u8 offset, u8 length, u8 *rddata)
{
    if ( length > 0 )
    {
        it6635_i2c_read(g_pstDev->opts.dev_opt->SwAddr, offset, length, rddata);
    }
}

void h2swbwr(u8 offset, u8 length, u8 *rddata)
{
    PR_IO(("    ---> h2swbwr(0x%02x, 0x%02x);\r\n", offset,*rddata));
    if ( length > 0 )
    {
        it6635_i2c_write(g_pstDev->opts.dev_opt->SwAddr, offset, length, rddata);
    }
}

////////////////////////////////////////////////////////////////////////////////
u8 h2rxedidwr(u8 offset, u8 *wrdata, u8 length)
{
    //PR_IO(("    ---> h2rxedidwr(0x%02x, 0x%02x);\r\n", offset, length));
    //PR51_IO(("    ---> h2rxedidwr(0x%02Bx, 0x%02Bx);\n", offset, length));
	#if 0
    u8 i;
    for (i=0 ; i<length ; i++)
    {
        PR_IO(("w %02x %02x %02x\r\n", offset+i, wrdata[i], RXEDIDAddr));
    }
	#endif

    return it6635_i2c_write(g_pstDev->opts.dev_opt->EdidAddr, offset, length, wrdata);
}

u8 h2rxedidrd(u8 offset, u8 *wrdata, u8 length)
{
    return it6635_i2c_read(g_pstDev->opts.dev_opt->EdidAddr, offset, length, wrdata);
}

u8 h2rxwr(u8 offset, u8 rdata)
{
    if ( (*sg_pIsSetIO) == 0 ) {
        //PR_IO(("    ---> h2swwr(0x%02x, 0x%02x);\r\n", offset, rdata));
        //PR51_IO(("    ---> h2swwr(0x%02Bx, 0x%02Bx);\n", offset, rdata));
        PR_IO(("w %02x %02x %02x\r\n", offset, rdata, g_pstDev->opts.dev_opt->RxAddr));
    }
    else{
        (*sg_pIsSetIO) = 0;
    }

    return it6635_i2c_write(g_pstDev->opts.dev_opt->RxAddr, offset, 1, &rdata);
}

u8 h2rxrd(u8 offset)
{
    u8 rddata;

    it6635_i2c_read(g_pstDev->opts.dev_opt->RxAddr, offset, 1, &rddata);

    return rddata;
}

u8 h2rxset(u8 offset, u8 mask, u8 wdata)
{
    u8 temp;

    (*sg_pIsSetIO) = 1;
    //PR_IO(("    ---> h2swset(0x%02x, 0x%02x, 0x%02x);\r\n", offset, mask, wdata));
    //PR51_IO(("    ---> h2swset(0x%02Bx, 0x%02Bx, 0x%02Bx);\n", offset, mask, wdata));
    PR_IO(("    s %02x %02x %02x %02x\r\n", offset, mask, wdata, g_pstDev->opts.dev_opt->RxAddr));

    temp = h2rxrd(offset);
    temp = (temp & ((~mask) & 0xFF)) + (mask & wdata);

    return h2rxwr(offset, temp);
}

void h2rxbrd(u8 offset, u8 length, u8 *rddata)
{
    if ( length > 0 )
    {
        it6635_i2c_read(g_pstDev->opts.dev_opt->RxAddr, offset, length, rddata);
    }
}

void h2rxbwr(u8 offset, u8 length, u8 *rddata)
{
    PR_IO(("    ---> h2rxbwr(0x%02x, 0x%02x);\r\n", offset,*rddata));
    if ( length > 0 )
    {
        it6635_i2c_write(g_pstDev->opts.dev_opt->RxAddr, offset, length, rddata);
    }
}

#if IT6635_EN_CEC

u8 cecwr(u8 offset, u8 wdata)
{
    PR_IO(("    ---> cecwr(0x%02x, 0x%02x);\r\n", offset, wdata));
    return it6635_i2c_write(g_pstDev->opts.dev_opt->CecAddr, offset, 1, &wdata);
}

u8 cecrd(u8 offset)
{
    u8 rddata;

    it6635_i2c_read(g_pstDev->opts.dev_opt->CecAddr, offset, 1, &rddata);

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
        it6635_i2c_read(g_pstDev->opts.dev_opt->CecAddr, offset, length, rddata);
    }
}

void cecbwr(u8 offset, u8 length, u8 *rddata)
{
    PR_IO(("    ---> cecbwr(0x%02x, 0x%02x);\r\n", offset,*rddata));
    PR51_IO(("    ---> cecbwr(0x%02Bx, 0x%02Bx);\n", offset,*rddata));
    if ( length > 0 )
    {
        it6635_i2c_write(g_pstDev->opts.dev_opt->CecAddr, offset, length, rddata);
    }
}

#endif

void chgrxbank(u8 bankno)
{
    h2rxset(0x0f, 0x07, bankno & 0x07);
}


//void chgswbank(u8 bankno)
//{
//    h2swset(0x0f, 0x03, bankno & 0x03);
//}

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
        PR_ERR(("** TX DDC Bus Sta=%02x\r\n", ddc_status));
        PR_ERR(("** TX DDC Bus Wait TimeOut => "));

        if ( h2swrd(0x27) & 0x80 )
        {
            PR_ERR(("** DDC Bus Hang\r\n"));
            //Do not handle the DDC Bus Hang here
            //h2txwr(port, 0x2E, 0x0F);  // Abort DDC Command
            //h2txwr(port, 0x16, 0x08);  // Clear Interrupt
        }
        else if ( ddc_status & 0x20 )
        {
            PR_ERR(("** DDC NoACK\r\n"));
        }
        else if ( ddc_status & 0x10 )
        {
            PR_ERR(("** DDC WaitBus\r\n"));
        }
        else if ( ddc_status & 0x08 )
        {
            PR_ERR(("** DDC ArbiLose\r\n"));
        }
        else
        {
            PR_ERR(("** UnKnown Issue\r\n"));
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

    if ( g_pstDev->opts.active_rx_opt->EnRxDDCBypass )
    {
        PR_ERR(("** EnRxDDCBypass:Abort SCDC write\r\n"));
        return FALSE;
    }

    if ( (h2swrd(0x11) & 0x20) == 0x00 )
    {
        PR_ERR(("** HPD-Low:Abort SCDC write\r\n"));
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

    if ( g_pstDev->opts.active_rx_opt->EnRxDDCBypass )
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

    if ( g_pstDev->opts.active_rx_opt->EnRxDDCBypass )
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

    if ( g_pstDev->opts.active_rx_opt->EnRxDDCBypass )
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
    if ( g_pstDev->opts.dev_opt->DoTxPowerDown )
    {
        h2swset(0xD3, 0x80, 0x00);
        h2swset(0xD1, 0x60, 0x60);
    }
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


u8 _rx_is_ch_symlock(u8 ch)
{
    if ( (h2rxrd(0x14) & (0x08 << ch)) )
    {
        return 1;
    }
    return 0;
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


void _sw_config_timer0(u8 count)
{
    // init timer = count[6:0] * 10 ms
    h2swwr(0x1C, count);
}

void _sw_enable_timer0(void)
{
    h2swset(0x38, 0x02, 0x02);
}

void _sw_clear_timer0_interrupt(void)
{
    h2swset(0x28, 0x02, 0x02);
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


void _sw_show_hdcp_status(void)
{
    u8 hdcp_sts;
    if ( g_pstDev->vars.Rev >= 0xC0 )
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
    u8 DRV_PISW, DRV_PISWC, DRV_HS;
    //vclk = 340000UL;
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
        //h2swset(0xD4, 0x04, 0x04);
        DRV_HS=1;
    }
    else
    {
    	DRV_HS=0;
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
        DRV_PISW = 1;
        DRV_PISWC = 1;
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
        DRV_PISW = 1;
        DRV_PISWC = 1;
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
        DRV_PISW = 1;
        DRV_PISWC = 1;
    }

    h2swset(0xD0, 0x08, (H2ON_PLL << 3));
    h2swset(0xD3, 0x1F, DRV_ISW);
    h2swset(0xD4, 0xF4, (DRV_PISWC<<6)+(DRV_PISW<<4)+(DRV_HS<<2));
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

    h2swset(0x05, 0x01, 0x01);
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
            if ( failcnt >= 10 )
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

    PR_INFO(("CAOF_Int=%02x, Status=%02x\r\n\r\n",(h2rxrd(0x59) & 0xC0),((h2rxrd(0x5A) << 4) + (h2rxrd(0x59) & 0x0F))));
    chgrxbank(0);

    h2swset(0x59+port, 0x20, 0x00);
    h2swset(0x05+port, 0x01, 0x00);
    h2swset(0x05, 0x01, 0x00);

    h2rxset(0x08, 0x30, 0x30);
    h2rxset(0x3C, 0x10, 0x10);

    chgrxbank(3);
    h2rxset(0x3A, 0x80, 0x00); // Reg_CAOFTrg low
    h2rxset(0xA0, 0x80, 0x00);
    h2rxset(0xA1, 0x80, 0x00);
    h2rxset(0xA2, 0x80, 0x00);
    chgrxbank(0);

    _rx_auto_power_down_enable(port, g_pstDev->opts.dev_opt->RxAutoPowerDown);
}

void _rx_show_ced_info(void)
{
    u8 symlock = (h2rxrd(0x14) & 0x38) >> 3;
    u8 ch;

    if ( 0x38 != symlock )
    {
        PR_INFO(("symlock = %02x\r\n", symlock));
    }
    else
    {
        for (ch=0 ; ch<3 ; ch++)
        {
            if ( g_pstDev->vars.RxCEDErrValid & (0x01<<ch) )
            {
                PR_INFO(("ch_%d CED=0x%04x\r\n", ch, g_pstDev->vars.RxCEDErr[ch]));
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

bool _rx_is_5v_active(void)
{
    return (h2rxrd(0x13) & 0x01)?TRUE:FALSE;
}

u8 _rx_is_clock_stable(void)
{
    if ( it6635_get_port_info0(g_pstDev->vars.Rx_active_port, (PI_CLK_STABLE | PI_CLK_VALID | PI_5V) ) )
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
    if ( g_pstDev->vars.Rev >= 0xC0 )
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
    //h2swwr(0x20 + port * 2, 0x7C); // clear clock related interrupt
    h2swset(0x2b, 0x01, 0x01);
    //h2swwr(0x20 + port * 2, 0x04);
    #endif
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

        //if (g_pstDev->vars.RxHPDFlag[port] != value)
        {
            g_pstDev->vars.RxHPDFlag[port] = hpd_value;
            if ( hpd_value )
            {
                if ( g_pstDev->vars.Rx_active_port == port )
                {
                    DBG_TM(RX_HPD_HIGH);
                }

                if ( g_pstDev->opts.rx_opt[port]->EnRxDDCBypass == 0 )
                {
                    h2swset(0x3C,0x01,0x01);
                    delay1ms(1);
                    h2swset(0x3C,0x01,0x00);
                }

                if ( g_pstDev->opts.rx_opt[port]->DisableEdidRam == 0 )
                {
                    _rx_edid_ram_enable(port);
                }

                if ( g_pstDev->opts.rx_opt[port]->HPDOutputInverse )
                {
                    h2swset(0x4C + port, 0xC0, 0x40);   // RXHPD=0
                }
                else
                {
                    h2swset(0x4C + port, 0xC0, 0xC0);   // RXHPD=1
                }


				#if 0
                if ( g_pstDev->vars.Rx_active_port == port )
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
                if ( g_pstDev->vars.Rx_active_port == port )
                {
                    DBG_TM(RX_HPD_LOW);
                }

                _rx_edid_ram_disable(port);

                _rx_term_power_down(port, term_value);

                if ( g_pstDev->opts.rx_opt[port]->HPDOutputInverse )
                {
                    h2swset(0x4C + port, 0xC0, 0xC0);   // RXHPD=1
                }
                else
                {
                    h2swset(0x4C + port, 0xC0, 0x40);   // RXHPD=0
                }

                if ( port == g_pstDev->vars.Rx_active_port )
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
            if ( g_pstDev->vars.Rx_active_port == i )
            {
                continue;
            }
        }

        if ( g_pstDev->opts.rx_opt[i]->NonActivePortReplyHPD )
        {
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
    if ( g_pstDev->opts.active_rx_opt->EnableAutoEQ )
    {
        if ( g_pstDev->vars.try_fixed_EQ )
        {
            PR_INFO(("*** fixed EQ fail\r\n"));
            g_pstDev->vars.try_fixed_EQ = 0;
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
        //
        // h2swset(0x90 + port, 0x3D, 0x3D); <-- will auto power down D0~D2 3.3V
        // h2swset(0x90 + port, 0x3D, 0x1D); <-- will not power down D0~D2 3.3V
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
    // to detect clock,
    // 0x88[7][0] must be '0','0';
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
        PR_INFO(("## src SCDC wr %02x\r\n", reg));

        if ( g_pstDev->vars.current_hdmi_mode == HDMI_MODE_20 )
        {
            if ( (reg&0x03) != 0x03 )
            {
                _tx_scdc_write(0x20, 0x03);
            }
        }
        else if ( g_pstDev->vars.current_hdmi_mode == HDMI_MODE_14 )
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
    h2swwr(0xB0, 0xC0);
}

void _sw_sdi_check(void)
{
    u8 port;
    u8 reg6C, reg70;
    port = g_pstDev->vars.Rx_active_port;

    if ( g_pstDev->vars.sdi_stable_count < 8 )
    {
        if (  it6635_get_port_info0(port, PI_CLK_STABLE) )
        {
            g_pstDev->vars.sdi_stable_count++;
        }
        else
        {
            g_pstDev->vars.sdi_stable_count = 0;
        }
    }
    else
    {
        // perform check
        g_pstDev->vars.sdi_stable_count = 0;
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
                g_pstDev->vars.check_for_sdi = 0;
                PR_INFO(("check_for_sdi disabled ...%02x\r\n", h2rxrd(0x13)));
            }
        }
        else
        {
            g_pstDev->vars.check_for_sdi = 0;
        }
    }
}

void _sw_hdcp_access_enable(u8 enable)
{
	if ( g_pstDev->vars.spmon == 2 )
	{
        PR_INFO(("  >> skip HDCP acc %d\r\n", enable));
		return;
	}

    PR_INFO(("  >> HDCP acc %d\r\n", enable));

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
    if ( g_pstDev->opts.dev_opt->ForceRxOn )
    {
        // for ATC electrical test
        h2swwr(0xFF, 0xC3);
        h2swwr(0xFF, 0xA5);
        h2swset(0xF4, 0x80, g_pstDev->opts.dev_opt->ForceRxOn << 7);
        h2swwr(0xFF, 0xFF);
    }

    h2swset(0x50, 0x0B, 0x08);

    h2swset(0x3A, 0xC0, (1 << 7) + (0 << 6));
    h2swset(0x3B, 0x03, 0); // DDC 75K
    h2swset(0x43, 0xFC, (0 << 7) + (0 << 5) + (0 << 4) + (2 << 2));
    h2swset(0xA9, 0xC0, (g_pstDev->opts.tx_opt->EnTxChSwap << 7) + (g_pstDev->opts.tx_opt->EnTxPNSwap << 6));

    // Enable HPD and RxSen Interrupt
    h2swwr(0x27, 0xff);
    h2swset(0x37, 0x78, 0x78);

    _tx_power_down();

    h2swset(0xBD, 0x01, g_pstDev->opts.tx_opt->EnTxVCLKInv);
    h2swset(0xA9, 0x20, g_pstDev->opts.tx_opt->EnTxOutD1t << 5);

    h2swset(0x50, 0x03, g_pstDev->vars.Rx_active_port);
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
    h2rxset(0x3C, 0x01, 0x00);

    //h2rxset(0x3d, 0x02, 0x02); // Reg_deskewdown = 1
}

void _rx_reset(void)
{
    PR_INFO(("RX Reset\r\n"));

    h2rxset(0x29, 0x40, 0x00);
    h2swset(0x44, 0x08, 0x08);
    h2rxwr(0x23, 0x01);   // SWRst=1
    //h2rxwr(0x22, 0x08);   // RegRst=1

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
    h2swwr(0xEF, g_pstDev->opts.dev_opt->RxAddr | 0x01);

    #if IT6635_EN_CEC
    if ( g_pstDev->opts.EnCEC )
    {
        // if CEC is enabled, we should have a accurate RCLK.
        u16 cec_timer_unit;

        h2swwr(0xEE, (g_pstDev->opts.dev_opt->CecAddr | 0x01));
        cecset(0x08, 0x01, 0x01);

        cec_timer_unit = g_pstDev->vars.RCLK / (16*10);
        Cec_Init( 0xff&cec_timer_unit );
    }
    else
    #endif
    {
		u8 tmp;
        h2swwr(0xEE, (g_pstDev->opts.dev_opt->CecAddr | 0x01));
        //cecset(0x0d, 0x10, 0x00); // Disable CEC_IOPU
		tmp = 0x40;
		it6635_i2c_write(g_pstDev->opts.dev_opt->CecAddr, 0x10, 1, &tmp); // Disable CEC_IOPU
        h2swwr(0xEE, (g_pstDev->opts.dev_opt->CecAddr & 0xFE));
    }

    h2swset(0x44, 0x40, 0x00);  // EnRxPort2Pwd=0
    delay1ms(10);

    _rx_caof_init(g_pstDev->vars.Rx_active_port);

    // Setup INT Pin: Active Low & Open-Drain
    h2swset(0x11, 0x07, 0x03);

    // Enable SW Interrupt
    h2swset(0x37, 0xE0, 0xE0);
    h2swset(0x38, 0xF9, 0xF9);

    // enable non main port to power down
    h2swset(0x15, 0x08, 0 << 3);

    h2swset(0x2B, 0x02, 0x00);
    h2swset(0x2C, 0xC0, 0xC0);

    h2swset(0x50, 0xf0, 0x00);

    h2swset(0xC4, 0x08, 0x08);
    h2swset(0xC5, 0x08, 0x08);
    h2swset(0xC6, 0x08, 0x08);

    // P0~P3 auto power downs
#if 0
    _rx_auto_power_down_enable_all(1);
#else
    _rx_auto_power_down_enable_all(g_pstDev->opts.dev_opt->RxAutoPowerDown);

    _rx_term_power_down(RX_PORT_0, 0);
    _rx_term_power_down(RX_PORT_1, 0);
    _rx_term_power_down(RX_PORT_2, 0);
    _rx_term_power_down(RX_PORT_3, 0);

#endif

    h2swset(0xF5, 0xE0, (g_pstDev->opts.active_rx_opt->EnRxDDCBypass << 7) + (g_pstDev->opts.active_rx_opt->EnRxPWR5VBypass << 6) + (g_pstDev->opts.active_rx_opt->EnRxHPDBypass << 5));
    if ( g_pstDev->opts.active_rx_opt->EnRxDDCBypass == 1 )
    {
        h2swset(0x3C, 0x01, 0x01); // disable DDCRegen by set RegTxMastersel=1
        h2swset(0xb3, 0x20, 0x20);
        _rx_edid_ram_disable(RX_PORT_0);
        _rx_edid_ram_disable(RX_PORT_1);
        _rx_edid_ram_disable(RX_PORT_2);
        _rx_edid_ram_disable(RX_PORT_3);
    }
    else
    {
        // config EDID RAM
        for (port=0 ; port<RX_PORT_COUNT ; port++)
        {
            if ( g_pstDev->opts.rx_opt[port]->DisableEdidRam )
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

    if ( g_pstDev->opts.active_rx_opt->EnRxHPDBypass )
    {
        h2swset(0x4c, 0x40, 0x00);
        h2swset(0x4d, 0x40, 0x00);
        h2swset(0x4e, 0x40, 0x00);
    }

    // disable EDID read/write to clear P0AutoH2Mode and AutoScrbEn
    h2swset(0xB2, 0x60, 0x00);
    //h2swset(0xB2, 0x40, 0x00);

    // enable TX port latch ERROR count
    h2swset(0xAC, 0x11, 0x11);

    // ddc monitor
    h2swwr(0xB0, 0x80);

}

void _sw_reset(void)
{
    PR_INFO(("Switch Reset\r\n"));

    h2swwr(0xEF, g_pstDev->opts.dev_opt->RxAddr | 0x01);
    h2swset(0x0A, 0x01, 0x01);  // SoftRstAll=1
    if(h2swrd(0xEF) == (g_pstDev->opts.dev_opt->RxAddr | 0x01))
    {
        h2swset(0x44, 0xA0, 0x80); // ForceWrUpd = 1 and SWGateRCLK = 0
    }
    //h2swset(0x0A, 0x02, 0x02);  // SoftSWRRst=1

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

    g_pstDev->vars.RCLK = rclk;
}
#endif


void it6635_init_rclk(void)
{
#if IT6635_EN_CEC
    if ( g_pstDev->opts.EnCEC )
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
            g_pstDev->vars.RCLK = 20000;
        #else
            #if 0
            // RCLK=19569 kHz
            h2swwr(0x1e, 0x13);
            h2swwr(0x1f, 0x91);
            h2swset(0x2c, 0x3f, 0x19);
            h2swwr(0x2d, 0x7d);
            h2swwr(0x2e, 0x50);
            h2swwr(0x2f, 0x47);
            g_pstDev->vars.RCLK = 19569;
            #endif

            // RCLK=18562 kHz
            h2swwr(0x1e, 0x12);
            h2swwr(0x1f, 0x90);
            h2swset(0x2c, 0x3f, 0x18);
            h2swwr(0x2d, 0x77);
            h2swwr(0x2e, 0x10);
            h2swwr(0x2f, 0xc6);
            g_pstDev->vars.RCLK = 18562;

        #endif
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
    if ( g_pstDev->vars.current_txoe == enable )
    {
        return;
    }

    PR_INFO(("TXOE=%d align=%d\r\n", enable, g_pstDev->opts.active_rx_opt->TxOEAlignment));

    if ( enable )
    {
        if ( g_pstDev->vars.current_hdmi_mode == HDMI_MODE_20 )
        {
            _tx_ovwr_hdmi_mode(HDMI_MODE_20);
            _tx_scdc_write(0x20, 0x03);
        }
        else if ( g_pstDev->vars.current_hdmi_mode == HDMI_MODE_14 )
        {
            _tx_ovwr_hdmi_mode(HDMI_MODE_14);
            _tx_scdc_write(0x20, 0x00); // todo: ? check if safe to send this?
        }

        h2swset(0xD4, 0x03, 0x00);      // Set DRV_RST='0'

        REPORT_TXOE_1();
    }
    else
    {
        REPORT_TXOE_0();

        h2swset(0xD4, 0x03, 0x01);      // Set DRV_RST='1'
    }

    g_pstDev->vars.current_txoe = enable;
}

void it6635_auto_txoe(u8 enable)
{
    PR_INFO(("A_TXOE=%d align=%d\r\n", enable, g_pstDev->opts.active_rx_opt->TxOEAlignment));

    if ( enable )
    {
        h2swset(0xEB, 0x07, 0x02);  // output when data ready
        //h2swset(0xEB, 0x07, 0x07);  // output when clock ready

        h2swset(0xEA, 0xA2, 0x00); //[7]Reg_GateTxOut, [5]Disoutdeskew, [1]Reg_EnTxDly
        h2swset(0xEB, 0x10, 0x00); //[4]RegEnTxDODeskew_doneDly
    }
    else
    {
        h2swset(0xEB, 0x03, 0x01);
    }
}

void it6635_set_tx_5v(u8 output_value)
{
    if ( g_pstDev->vars.Tx_current_5v != output_value )
    {
        g_pstDev->vars.Tx_current_5v = output_value;
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

    _rx_wdog_rst(g_pstDev->vars.Rx_active_port);

__RETRY_VCLK:

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
        tmds_clk = g_pstDev->vars.RCLK * 256 * wdog_clk_div / tmds_clk_speed;

        PR_ERR(("RXP%d WatchDog detect TMDSCLK = %lu kHz (div=%d, 6C=%02x)\r\n", port, tmds_clk, wdog_clk_div, tmds_clk_speed ));

        tmds_clk_speed = 0;

        h2swwr(0x20 + port * 2, sw_reg20);

        goto __RETRY_VCLK;
    }

    if ( tmds_clk_speed )
    {
        tmds_clk_speed = ((tmds_clk_speed * 2) >> (RCLKFreqSel));
        tmds_clk = g_pstDev->vars.RCLK * 256 * wdog_clk_div / tmds_clk_speed;

        PR_INFO(("RXP%d WatchDog detect TMDSCLK = %lu kHz (div=%d, 6C=%02x)\r\n", port, tmds_clk, wdog_clk_div, tmds_clk_speed ));
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
            if ( g_pstDev->vars.Rx_active_port != port )
            {
                if ( (rddata & 0x01) )
                {
                    // 5V presents
                    if ( g_pstDev->opts.rx_opt[port]->NonActivePortReplyHPD )
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
            PR_INFO(("--RXP-%d RX Clock Valid Chg => RxCLK_Valid = %d\r\n", port,(rddata & 0x08) >> 3));
        }

        if ( sw_reg20 & 0x04 )
        {
            PR_INFO(("--RXP-%d RX Clock Stable Chg => RxCLK_Stb = %d\r\n\r\n", port,(rddata & 0x10) >> 4));
        }

        if ( sw_reg20 & 0x08 )
        {
            PR_INFO(("--RXP-%d RX Clock Frequency Change ...\r\n", port));
        }

        sts_off0 = 0x61 + port * 3;
        rddata = h2swrd(sts_off0);

        if ( sw_reg20 & 0x10 )
        {
            PR_INFO(("--RXP-%d RX Clock Ratio Chg => Clk_Ratio = %d \r\n", port,(rddata & 0x40) >> 6));
        }

        if ( sw_reg20 & 0x20 )
        {
            PR_INFO(("--RXP%d RX Scrambling Enable Chg => Scr_En = %d \r\n",port,(rddata & 0x80) >> 7));
        }

        sts_off0 = 0x62 + port * 3;
        rddata = h2swrd(sts_off0);

        if ( sw_reg20 & 0x40 )
        {
            PR_INFO(("--RXP%d RX Scrambling Status Chg => ScrbSts = %d \r\n",port,(rddata & 0x02) >> 1));
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
            PR_INFO(("--RXP%d EDID Bus Hang\r\n", port));
        }
#endif
    }
}

void it6635_detect_ports(void)
{
    u8 i;

    for ( i = 0; i < 4; i++ )
    {
        if ( g_pstDev->vars.Rx_active_port != i )
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
            PR_INFO(("..RX5V change\r\n"));

            _eq_reset_txoe_ready();
            _eq_reset_state();
            it6635_auto_detect_hdmi_encoding();

            if ( g_pstDev->opts.active_rx_opt->TryFixedEQFirst )
            {
                g_pstDev->vars.try_fixed_EQ = 1;
            }

            if ( 0 == _rx_is_5v_active() )
            {
                it6635_fsm_chg_delayed(RX_UNPLUG);
            }
        }

        if ( rx_reg05 & 0x10 )
        {
            PR_INFO(("..RX HDMIMode chg => HDMIMode = %d\r\n",(h2rxrd(0x13) & 0x02) >> 1));
        }

        if ( rx_reg05 & 0x40 )
        {
            PR_INFO(("..RX DeSkew Err\r\n"));
            g_pstDev->vars.rx_deskew_err++;
            if ( g_pstDev->vars.rx_deskew_err > 50 )
            {
                g_pstDev->vars.rx_deskew_err = 0;
                it6635_toggle_hpd(1000);
            }
        }

        if ( rx_reg05 & 0x80 )
        {
            PR_INFO(("..RXP H2V FIFO Skew Fail\r\n"));
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if ( rx_reg06 & 0x01 )
        {
            u8 symlock = ((h2rxrd(0x13) & 0x80) >> 7);
            PR_INFO(("..RX CHx SymLock Chg => RxSymLock = %d\r\n", symlock));
            if ( symlock )
            {
                //g_pstDev->vars.count_fsm_err = 0;
            }
        }

        if ( rx_reg06 & 0x02 )
        {
            PR_INFO(("..RX CH0 SymFIFORst\r\n"));
        }

        if ( rx_reg06 & 0x04 )
        {
            PR_INFO(("..RX CH1 SymFIFORst\r\n"));
        }

        if ( rx_reg06 & 0x08 )
        {
            PR_INFO(("..RX CH2 SymFIFORst\r\n"));
        }

        if ( rx_reg06 & 0x10 )
        {
            PR_INFO(("..RX CH0 SymLockRst\r\n"));
        }

        if ( rx_reg06 & 0x20 )
        {
            PR_INFO(("..RX CH1 SymLockRst\r\n"));
        }

        if ( rx_reg06 & 0x40 )
        {
            PR_INFO(("..RX CH2 SymLockRst\r\n"));
        }

        if ( rx_reg06 & 0x80 )
        {
            PR_INFO(("..RX FSM Fail\r\n"));
            g_pstDev->vars.count_fsm_err++;
            if ( g_pstDev->vars.count_fsm_err > 20 )
            {
                if ( g_pstDev->opts.active_rx_opt->FixIncorrectHdmiEnc )
                {
                    it6635_fix_incorrect_hdmi_encoding();
                }
                _eq_reset_txoe_ready();
                _eq_reset_state();
                it6635_fsm_chg(RX_WAIT_CLOCK);
                g_pstDev->vars.count_fsm_err = 0;
            }
        }
        else
        {
            if ( g_pstDev->vars.count_fsm_err > 0 )
            {
                g_pstDev->vars.count_fsm_err--;
            }
        }

        #if EN_H14_SKEW
		{
		    u8 rx_reg07;
	        rx_reg07 = h2rxrd(0x07);
	        h2rxwr(0x07, rx_reg07);

	        if ( rx_reg07 & 0x01 )
	        {
	            PR_INFO(("..RX CH0 Lag Err\r\n"));
	            _rx_skew_adj(0);
	        }
	        if ( rx_reg07 & 0x02 )
	        {
	            PR_INFO(("..RX CH1 Lag Err\r\n"));
	            _rx_skew_adj(1);
	        }
	        if ( rx_reg07 & 0x04 )
	        {
	            PR_INFO(("..RX CH2 Lag Err\r\n"));
	            _rx_skew_adj(2);
	        }
		}
        #endif

        if ( rx_reg10 & 0x08 )
        {
            h2rxwr(0x10, 0x08);
            PR_INFO(("..RX FW Timer Interrupt ...\r\n"));
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
            PR_INFO(("..RX-P%d PWR5V Chg => PWR5V = %d\r\n", port,(rddata & 0x01)));
            //_rx_wdog_rst(port);
            if ( 0 == (rddata & 0x01) )
            {
                it6635_fsm_chg_delayed(RX_UNPLUG);
            }
        }

        if ( sw_reg20 & 0x02 )
        {
            rddata = h2swrd(sts_off0);
            PR_INFO(("..RXP%d RX Clock Valid Chg => RxCLK_Valid = %d\r\n", port,(rddata & 0x08) >> 3));
            if ( port == g_pstDev->vars.Rx_active_port )
            {
                if ( 0 == (rddata & 0x08) ) // clock not valid
                {
                    DBG_TM(CLK_UNSTABLE);
                    if ( g_pstDev->vars.RxHPDFlag[g_pstDev->vars.Rx_active_port] > 0 )
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
            PR_INFO(("..RXP%d RX Clock Stable Chg => RxCLK_Stb = %d\r\n\r\n", port,(rddata & 0x10) >> 4));
            if ( 0 == (rddata & 0x10) )
            {
                DBG_CLKSTABLE_0();
                DBG_SYMLOCK_0();
                if ( g_pstDev->vars.RxHPDFlag[port] )
                {
                    it6635_fsm_chg_delayed(RX_WAIT_CLOCK);
                }
            }
            else
            {
                g_pstDev->vars.vclk = it6635_get_rx_vclk(g_pstDev->vars.Rx_active_port);
                if ( (g_pstDev->vars.vclk != g_pstDev->vars.vclk_prev) )
                {
                    if ( g_pstDev->vars.RxHPDFlag[port] )
                    {
                        it6635_fsm_chg_delayed(RX_WAIT_CLOCK);
                    }
                }
            }
        }


        if ( sw_reg20 & 0x08 )
        {
            PR_INFO(("..RXP%d RX Clock Frequency Chg ...\r\n", port));
        }

        if ( sw_reg20 & 0x10 )
        {
            u8 new_ratio = (h2swrd(0x61 + port * 3) & 0x40) >> 6;

            PR_INFO(("..RXP%d RX Clock Ratio Chg => Clk_Ratio = %d \r\n", port, new_ratio));

            if ( g_pstDev->vars.Rx_active_port == port )
            {
                if ( new_ratio > 0 )
                {
                    it6635_auto_txoe( g_pstDev->opts.active_rx_opt->TxOEAlignment );
                }
                else
                {
                    it6635_auto_txoe(0);
                }
                it6635_txoe(1);

                if ( new_ratio != g_pstDev->vars.clock_ratio )
                {
                    //it6635_auto_detect_hdmi_encoding();
                    //it6635_fsm_chg_delayed(RX_WAIT_CLOCK);
                }
            }
        }

        if ( sw_reg20 & 0x20 )
        {
            PR_INFO(("..RXP%d RX Scrambling Enable Chg => Scr_En = %d \r\n",port,(h2swrd(0x61 + port * 3) & 0x80) >> 7));
        }

        if ( sw_reg20 & 0x40 )
        {
            u8 new_scramble = (h2swrd(0x62 + port * 3) & 0x02) >> 1;

            PR_INFO(("..RXP%d RX Scrambling Status Chg => ScrbSts = %d \r\n",port, new_scramble));
            if ( g_pstDev->vars.Rx_active_port == port )
            {
                if ( new_scramble != g_pstDev->vars.h2_scramble )
                {
                    //it6635_fsm_chg_delayed(RX_WAIT_CLOCK);
                }
            }
        }

        if ( sw_reg20 & 0x80 )
        {
            PR_INFO(("..RXP%d RX HDMI2 Detected Interrupt => HDMI2DetSts = %d \r\n",port,(h2swrd(0x62 + port * 3) & 0x3C) >> 2));
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if ( sw_reg21 & 0x01 )
        {
            PR_INFO(("..RXP%d EDID Bus Hang\r\n", port));
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

    if ( sw_reg27 & 0x08 )
    {
        //PR_INFO((" => HDCP 0x74 is detected\r\n"));
    }

    if ( sw_reg27 & 0x10 )
    {
        PR_INFO(("  => HDCP 0x74 NOACK\r\n"));
    }


    if ( sw_reg27 & 0x20 )
    {
        rddata = h2swrd(0x11);

        if ( (rddata & 0x20) )
        {
            PR_INFO(("  => HPD High\r\n"));
        }
        else
        {
            PR_INFO(("  => HPD Low\r\n"));

            if ( g_pstDev->vars.state_sys_fsm != RX_TOGGLE_HPD &&
                 g_pstDev->vars.state_sys_fsm != RX_UNPLUG )
            {
                it6635_fsm_chg_delayed(TX_UNPLUG);
            }
        }
    }

    if ( sw_reg27 & 0x40 )
    {
        PR_INFO(("  TX RxSen chg\r\n"));

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
        PR_INFO(("  TX DDC Bus Hang\r\n"));

		if ( g_pstDev->opts.active_rx_opt->EnRxDDCBypass == false )
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
        PR_INFO(("  TX DDC Command Fail\r\n"));
		if ( g_pstDev->opts.active_rx_opt->EnRxDDCBypass == false )
		{
			reg3C = h2swrd(0x3C);
	        h2swset(0x3C, 0x01, 0x01);
	        h2swwr(0x3D, 0x0F);
	        h2swwr(0x3C, reg3C);
		}

    }

    if ( sw_reg28 & 0x80 )
    {
        PR_INFO(("  TX DDC FIFO Error\r\n"));
		if ( g_pstDev->opts.active_rx_opt->EnRxDDCBypass == false )
		{
			reg3C = h2swrd(0x3C);
	        h2swset(0x3C, 0x01, 0x01);
	        h2swwr(0x3D, 0x09);
	        h2swwr(0x3C, reg3C);
		}
    }
}

void it6635_wait_for_ddc_idle(void)
{
    u8 ddc_sts;
    u8 idle_cnt = 0;
    u8 busy_cnt = 0;
    u8 chk_dly = 3;

    while(1)
    {
        ddc_sts = h2swrd(0xB3);
        if ( (ddc_sts & 0x10) )
        {
            busy_cnt=0;
            idle_cnt++;
            chk_dly++;
            if (idle_cnt>=5)
            {
                break;
            }
        }
        else
        {
            busy_cnt++;
            idle_cnt = 0;
            chk_dly = 3;

            delay1ms(100);
            if ( busy_cnt > 10 )
            {
                PR_ERR(("**Wait DDC idle timeout\n"));
                break;
            }
        }

        delay1ms(chk_dly);
    }
}

 //#if DEBUG_FSM_CHANGE
 //void __it6635_fsm_chg(u8 new_state, int caller)
 //#else
void it6635_fsm_chg( u8 new_state )
 //#endif
{

#if DEBUG_FSM_CHANGE
    if ( new_state <= IDLE && g_pstDev->vars.state_sys_fsm <= IDLE )
    {
        PR_INFO2(("state_fsm %s -> %s (%d)\r\n", s__SYS_FSM_STATE[g_pstDev->vars.state_sys_fsm], s__SYS_FSM_STATE[new_state], caller));
    }
    else
    {
        PR_ERR(("state_fsm %d, new %d -> %d (%d)\r\n", g_pstDev->vars.state_sys_fsm, new_state, caller));
    }
#else
    PR_INFO2(("state_fsm %d -> %d\r\n", g_pstDev->vars.state_sys_fsm, new_state));
#endif

    if ( RX_PORT_CHANGE != new_state )
    {
        if ( g_pstDev->vars.state_sys_fsm == new_state )
        {
            PR_INFO2(("skip fsm chg 1\r\n"));
            return;
        }
    }


    if ( new_state == RX_WAIT_CLOCK )
    {
        if ( g_pstDev->vars.RxHPDFlag[g_pstDev->vars.Rx_active_port] == 0 )
        {
            // don't change before HPD High
            PR_INFO2(("skip fsm chg 2\r\n"));
            return;
        }
    }


    g_pstDev->vars.state_sys_fsm = (_SYS_FSM_STATE)new_state;
    g_pstDev->vars.fsm_return = 0;

    switch ( g_pstDev->vars.state_sys_fsm )
    {
        case RX_TOGGLE_HPD:
            _sw_enable_hpd_toggle_timer(g_pstDev->vars.hpd_toggle_timeout);
            break;

        case RX_PORT_CHANGE:

            it6635_txoe(0);

            DBG_TM(RX_SWITCH_PORT);
            PR_INFO(("Active port change from P%d to P%d\r\n", g_pstDev->vars.Rx_active_port, g_pstDev->vars.Rx_new_port));

            if ( g_pstDev->vars.clock_ratio > 0 && g_pstDev->opts.active_rx_opt->EnRxDDCBypass == false )
            {
                _tx_scdc_write(0x20,0x00);
            }

            if ( g_pstDev->opts.tx_opt->TurnOffTx5VWhenSwitchPort )
            {
                it6635_set_tx_5v(0);
            }

            //_rx_int_enable(g_pstDev->vars.Rx_active_port, 1);
            //_rx_set_hpd(g_pstDev->vars.Rx_active_port, 0);
            //_rx_wdog_rst(gdev.vars.Rx_prev_port);

            // make HPD low to stop DDC traffic
            _rx_set_hpd(g_pstDev->vars.Rx_active_port, 0, TERM_FOLLOW_HPD);
            // wait 200ms for DDC traffic stopped
            delay1ms(200);

            // set g_pstDev->vars.force_hpd_state to SW_HPD_AUTO
            // this to reset the force hpd low in previous active port
            // remove this line if you want to keep HPD low after port changing
            g_pstDev->vars.force_hpd_state = SW_HPD_AUTO;

            g_pstDev->vars.Rx_active_port = g_pstDev->vars.Rx_new_port;

            it6635_wait_for_ddc_idle();

            h2swset(0x50, 0x03, g_pstDev->vars.Rx_active_port);

            it6635_set_RS(g_pstDev->opts.active_rx_opt->DefaultEQ[0],
                          g_pstDev->opts.active_rx_opt->DefaultEQ[1],
                          g_pstDev->opts.active_rx_opt->DefaultEQ[2]);

            g_pstDev->EQ.sys_aEQ = SysAEQ_RUN;
            it6635_auto_detect_hdmi_encoding();
            _eq_reset_state();
            _eq_reset_txoe_ready();
            break;

        case TX_OUTPUT:
            g_pstDev->vars.count_symlock_lost = 0;
            g_pstDev->vars.count_symlock_unstable = 0;
            _sw_disable_hpd_toggle_timer();

            if ( (g_pstDev->opts.active_rx_opt->FixIncorrectHdmiEnc) && (g_pstDev->opts.active_rx_opt->EnRxDDCBypass == FALSE) )
            {
                if ( g_pstDev->vars.current_hdmi_mode == HDMI_MODE_20 )
                {
                    _tx_scdc_write(0x20, 0x03);
                }
                else if ( g_pstDev->vars.current_hdmi_mode == HDMI_MODE_14 )
                {
                    // todo: to check sink support SCDC ?
                    _tx_scdc_write(0x20, 0x00);
                }
                it6635_auto_detect_hdmi_encoding();
            }

            _sw_hdcp_access_enable(1);
            it6635_txoe(1);

            if ( g_pstDev->vars.spmon == 1 )
            {
                if ( (g_pstDev->opts.active_rx_opt->DisableEdidRam & (1<<g_pstDev->vars.Rx_active_port)) == 0 )
                {
                    _rx_edid_ram_enable(g_pstDev->vars.Rx_active_port);
                }
            }
            break;

        case TX_OUTPUT_PREPARE:
            g_pstDev->vars.count_try_force_hdmi_mode = 0;
//            it6635_auto_txoe(1);
            h2rxwr(0x05, 0xFF);
            h2rxwr(0x06, 0xFF);
            h2rxwr(0x07, 0xFF);
            break;

		#if EN_AUTO_RS
        case RX_CHECK_EQ:
            g_pstDev->vars.count_symlock_fail=0;
            //_sw_hdcp_access_enable(0);
            break;
		#endif

        case SETUP_AFE:
            //g_pstDev->vars.en_count_hdcp = 1;
            g_pstDev->vars.tick_set_afe = get_tick_count();
            _rx_term_power_down(g_pstDev->vars.Rx_active_port,0x00);
            g_pstDev->vars.vclk = it6635_get_rx_vclk(g_pstDev->vars.Rx_active_port);

            if ( g_pstDev->vars.vclk )
            {
                g_pstDev->vars.clock_ratio = ((h2swrd(0x61 + g_pstDev->vars.Rx_active_port * 3) >> 6) & 1);
                PR_INFO2(("Clk Ratio = %d\r\n", g_pstDev->vars.clock_ratio));

                if ( g_pstDev->vars.clock_ratio > 0 )
                {
                    if ( g_pstDev->vars.vclk < 300000UL )
                    {
                        g_pstDev->vars.vclk = 300001UL;
                    }
                    h2rxset(0x3B, 0x10, 0x00); // CED opt for HDBaseT disabled
                }
                else
                {
                    if ( g_pstDev->vars.vclk >= 300000UL )
                    {
                        g_pstDev->vars.vclk = 297000UL;
                    }
                    h2rxset(0x3B, 0x10, 0x10); // CED opt for HDBaseT enabled
                }

                #if 0 // for 8-7 480p
                if ( g_pstDev->vars.vclk < 35000UL ) {
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
                _rx_setup_afe(g_pstDev->vars.vclk);
                _tx_setup_afe(g_pstDev->vars.vclk);


                if ( g_pstDev->vars.clock_ratio == 0 )
                {
                    it6635_auto_txoe(0);
                    PR_INFO2(("Clk Ratio==0, align=0\n"));
                }
                else
                {
                    it6635_auto_txoe(g_pstDev->opts.active_rx_opt->TxOEAlignment);
                    PR_INFO2(("Clk Ratio==1, align=%d\n", g_pstDev->opts.active_rx_opt->TxOEAlignment));
                }

                it6635_txoe(1);


            }
            break;

        case RX_WAIT_CLOCK:
            it6635_txoe(0);
            if ( g_pstDev->opts.dev_opt->TxPowerDownWhileWaitingClock )
            {
                _tx_power_down();
            }

            _sw_disable_timer0();
            _sw_clear_hdcp_status();
            //_rx_wdog_rst(g_pstDev->vars.Rx_active_port);

            #if EN_AUTO_RS
            g_pstDev->vars.RxCEDErrRec[1][0]=0xffff;
            g_pstDev->vars.RxCEDErrRec[1][1]=0xffff;
            g_pstDev->vars.RxCEDErrRec[1][2]=0xffff;
            g_pstDev->EQ.manu_eq_fine_tune_count[0]=0;
            g_pstDev->EQ.manu_eq_fine_tune_count[1]=0;
            g_pstDev->EQ.manu_eq_fine_tune_count[2]=0;
            g_pstDev->EQ.ced_err_avg_prev[0] = 0x8888;
            g_pstDev->EQ.ced_err_avg_prev[1] = 0x8888;
            g_pstDev->EQ.ced_err_avg_prev[2] = 0x8888;
            g_pstDev->EQ.ced_acc_count = 0;
            #endif
            g_pstDev->vars.count_symlock = 0;
            g_pstDev->vars.count_unlock = 0;
            g_pstDev->vars.check_for_hpd_toggle = 0;
            g_pstDev->vars.sdi_stable_count = 0;
            g_pstDev->vars.check_for_sdi = 1;
            g_pstDev->vars.rx_deskew_err = 0;
            break;

        case RX_HPD:
            _rx_int_enable();

            #if 1
            //_rx_ovwr_hdmi_clk(g_pstDev->vars.Rx_active_port, HDMI_MODE_14);
            //_rx_ovwr_h20_scrb(g_pstDev->vars.Rx_active_port, 0);
            #else
            _rx_ovwr_hdmi_clk(g_pstDev->vars.Rx_active_port, RX_CLK_H20);
            _rx_ovwr_h20_scrb(g_pstDev->vars.Rx_active_port, 1);
            #endif


            //g_pstDev->vars.Rx_prev_port = g_pstDev->vars.Rx_active_port;
            //_rx_int_enable_all(0);
            //_rx_set_hpd_all(0);
            _sw_hdcp_access_enable(1);
            _sw_int_enable(g_pstDev->vars.Rx_active_port, 1);
            //_rx_set_hpd(g_pstDev->vars.Rx_active_port, 1);
            _rx_wdog_rst(g_pstDev->vars.Rx_active_port);
            if ( g_pstDev->vars.spmon == 1 )
            {
                if ( (g_pstDev->opts.active_rx_opt->DisableEdidRam & (1<<g_pstDev->vars.Rx_active_port)) == 0 )
                {
                    _rx_edid_ram_disable(g_pstDev->vars.Rx_active_port);
                }
            }

            _rx_set_hpd(g_pstDev->vars.Rx_active_port, 1, TERM_FOLLOW_HPD);

            if ( g_pstDev->vars.is_hdmi20_sink == 0 )
            {
                it6635_auto_txoe(0);
            }
            else
            {
                it6635_auto_txoe( g_pstDev->opts.active_rx_opt->TxOEAlignment );
            }
            it6635_txoe(1);

            break;

        case TX_GOT_HPD:
            it6635_txoe(0);

			//_tx_power_on();

            if ( g_pstDev->opts.tx_opt->CopyEDIDFromSink )
            {
            	if (g_pstDev->opts.active_rx_opt->EnRxDDCBypass == false )
            	{
            		it6635_setup_edid_ram(0);
            	}
                g_pstDev->vars.edid_ready = 1;
            }

            #if IT6635_EN_CEC
            if(g_pstDev->opts.EnCEC)
            {
                u8 u8phyAB = (g_pstDev->vars.PhyAdr[0] << 4) | (g_pstDev->vars.PhyAdr[1] & 0xF);
                u8 u8phyCD = (g_pstDev->vars.PhyAdr[2] << 4) | (g_pstDev->vars.PhyAdr[3] & 0xF);

                CecSys_Init(u8phyAB, u8phyCD, g_pstDev->vars.Rx_active_port);
            }
            #endif

            if ( g_pstDev->opts.active_rx_opt->NonActivePortReplyHPD )
            {
                _rx_set_hpd_with_5v_all(true);
            }

            break;

        case TX_WAIT_HPD:
            it6635_txoe(0);
            it6635_auto_txoe(0);
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
            it6635_auto_txoe(0);
            g_pstDev->vars.edid_ready = 0;
            //_rx_int_enable(g_pstDev->vars.Rx_active_port, 0);
            _rx_set_hpd(g_pstDev->vars.Rx_active_port, 0, TERM_FOLLOW_TX);
            //_rx_set_hpd_all(0);
            h2swset(0xB2, 0x0A, 0x0A);  // W1C AutoH2Mode and AutoScrbEn
            _tx_power_down();
            break;

        case RX_UNPLUG:
            it6635_txoe(0);
            it6635_auto_txoe(0);
//            _rx_int_enable();
            if ( g_pstDev->vars.force_hpd_state == SW_HPD_LOW )
            {
                _rx_set_hpd(g_pstDev->vars.Rx_active_port, 0, TERM_FOLLOW_HPD);
            }
            else
            {
                _rx_set_hpd(g_pstDev->vars.Rx_active_port, 0, TERM_FOLLOW_TX);
            }
            it6635_set_tx_5v(_rx_is_5v_active());
            //g_pstDev->EQ.sys_aEQ = SysAEQ_RUN;
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
    if ( new_state <= IDLE && g_pstDev->vars.state_sys_fsm <= IDLE )
    {
        PR_INFO2(("#state_fsm %s -> %s (%d)\r\n", s__SYS_FSM_STATE[g_pstDev->vars.state_sys_fsm], s__SYS_FSM_STATE[new_state], caller));
    }
    else
    {
        PR_ERR(("#state_fsm %d, new %d -> %d (%d)\r\n", g_pstDev->vars.state_sys_fsm, new_state, caller));
    }
#else
    PR_INFO2(("#state_fsm %d -> %d\r\n", g_pstDev->vars.state_sys_fsm, new_state));
#endif

    it6635_fsm_chg(new_state);
    g_pstDev->vars.fsm_return = 1;
}

void _rx_pll_reset(void)
{
    h2swset( 0x06+g_pstDev->vars.Rx_active_port, 0x01, 0x01 );
    delay1ms(2);
    h2swset( 0x06+g_pstDev->vars.Rx_active_port, 0x01, 0x00 );
}

void it6635_auto_detect_hdmi_encoding(void)
{
    _rx_ovwr_hdmi_mode(g_pstDev->vars.Rx_active_port, HDMI_MODE_AUTO);
    _tx_ovwr_hdmi_mode(HDMI_MODE_AUTO);
    g_pstDev->vars.current_hdmi_mode = HDMI_MODE_AUTO;
    //_rx_pll_reset();
    PR_INFO(("HDMI_MODE=AUTO \r\n"));
}

void it6635_force_hdmi20(void)
{
    _rx_ovwr_hdmi_mode(g_pstDev->vars.Rx_active_port, HDMI_MODE_20);
    _tx_ovwr_hdmi_mode(HDMI_MODE_20);
    g_pstDev->vars.current_hdmi_mode = HDMI_MODE_20;
    //_rx_pll_reset();
    PR_INFO(("HDMI_MODE=F20\r\n"));
}

void it6635_force_hdmi14(void)
{
    _rx_ovwr_hdmi_mode(g_pstDev->vars.Rx_active_port, HDMI_MODE_14);
    _tx_ovwr_hdmi_mode(HDMI_MODE_14);
    g_pstDev->vars.current_hdmi_mode = HDMI_MODE_14;
    //_rx_pll_reset();
    PR_INFO(("HDMI_MODE=F14\r\n"));
}


void it6635_fix_incorrect_hdmi_encoding(void)
{

    switch ( g_pstDev->vars.current_hdmi_mode )
    {
        case HDMI_MODE_AUTO:
            // try HDMI 2.0
            it6635_force_hdmi20();
            break;
        case HDMI_MODE_20:
            // try HDMI 1.4
            it6635_force_hdmi14();
            break;
        case HDMI_MODE_14:
            // try HDMI 2.0
            it6635_auto_detect_hdmi_encoding();
            break;
        default:
            // try HDMI 2.0
            it6635_auto_detect_hdmi_encoding();
            break;
    }

    _rx_pll_reset();
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
                    g_pstDev->vars.check_for_hpd_toggle = 1;
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
                        g_pstDev->vars.check_for_hpd_toggle = 1;
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
//LOOP_FSM:

    switch ( g_pstDev->vars.state_sys_fsm )
    {
        case RX_TOGGLE_HPD:
            if ( (g_pstDev->opts.active_rx_opt->NonActivePortReplyHPD == 0) && (g_pstDev->opts.tx_opt->TurnOffTx5VWhenSwitchPort == 0) )
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
            _rx_set_hpd(g_pstDev->vars.Rx_active_port, 0, TERM_FOLLOW_HPD);
            _rx_reset();
            _rx_caof_init(g_pstDev->vars.Rx_active_port);
            _rx_pll_reset();
            g_pstDev->vars.hpd_toggle_timeout = g_pstDev->opts.active_rx_opt->HPDTogglePeriod;
            it6635_fsm_chg(RX_TOGGLE_HPD);
            break;

        case TX_OUTPUT:

            //if ( g_pstDev->opts.WaitSymlockBeforeTXOE )
            {
                if ( 0 == _rx_is_all_ch_symlock() )
                {
                    if ( 0 == _rx_is_any_ch_symlock() )
                    {
                        g_pstDev->vars.count_symlock_lost++;
                        PR_ERR(("RX Symlock lost %d\r\n", g_pstDev->vars.count_symlock_lost));
                        if ( g_pstDev->vars.count_symlock_lost == 100 )
                        {
							_rx_pll_reset();
                            it6635_toggle_hpd(1000);
                            //it6635_set_tx_5v(0);
                            //g_pstDev->vars.count_symlock_lost = 0;
                            //_rx_handle_output_err();
                        }
                    }
                    else
                    {
                        g_pstDev->vars.count_symlock_unstable++;
                        PR_ERR(("RX Symlock unstable %d\r\n", g_pstDev->vars.count_symlock_unstable));
                        if ( g_pstDev->vars.count_symlock_unstable > 8 )
                        {
                            _rx_pll_reset();
                            //it6635_fsm_chg(RX_WAIT_CLOCK);
							it6635_toggle_hpd(1000);
                        }
                    }
                }
                else
                {
                    g_pstDev->vars.count_symlock_lost = 0;
                    g_pstDev->vars.count_symlock_unstable = 0;
                }
            }

            if ( _rx_monitor_ced_err() )
            {
                _rx_handle_output_err();
            }

            //_sw_show_hdcp_status();

            if ( g_pstDev->opts.active_rx_opt->FixIncorrectHdmiEnc )
            {
                //check if source send incorrect SCDC clock ratio after 6635 sent.
                if ( g_pstDev->vars.current_hdmi_mode != HDMI_MODE_AUTO && g_pstDev->opts.active_rx_opt->EnRxDDCBypass == false )
                {
                    _sw_monitor_and_fix_scdc_write();
                }
            }

            if ( g_pstDev->vars.check_for_sdi )
            {
                _sw_sdi_check();
            }

            //_tx_show_sink_ced();

            //_pr_port_info(g_pstDev->vars.Rx_active_port);

            break;

        case TX_OUTPUT_PREPARE:
		#if EN_AUTO_RS
            // check symbol lock before tx output
            if ( 0 == _rx_is_any_ch_symlock() )
            {
                PR_ERR(("RxChk-SymUnlock\r\n"));
                g_pstDev->vars.count_symlock_fail++;
                if ( g_pstDev->vars.count_symlock_fail > 3 )
                {
                    g_pstDev->vars.count_symlock_fail = 0;

                    // can not get any channel symbol lock,
                    // the HDMI encoding may be incorrect
                    if ( g_pstDev->opts.active_rx_opt->FixIncorrectHdmiEnc )
                    {
                        if ( g_pstDev->vars.count_try_force_hdmi_mode < 6 )
                        {
                            g_pstDev->vars.count_try_force_hdmi_mode++;
                            it6635_fix_incorrect_hdmi_encoding();
                        }
                        else
                        {
                            g_pstDev->vars.count_try_force_hdmi_mode = 0;
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

                if ( (g_pstDev->vars.check_for_hpd_toggle == 1) && (g_pstDev->vars.current_txoe == 0) && (_rx_need_hpd_toggle()) )
                {
                    DBG_TM(AEQ_TOGGLE_HPD);
                    it6635_set_tx_5v(0);
                    _rx_set_hpd(g_pstDev->vars.Rx_active_port, 0, TERM_FOLLOW_HPD);
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
                        PR_ERR(("RX reg: 05=%02x, 06=%02x 07=%02x\r\n", reg05, reg06, reg07));
                        h2rxwr(0x05, reg05);
                        h2rxwr(0x06, reg06);
                        h2rxwr(0x07, reg07);
                        prep_count = 0;
                        prep_fail_count++;
                    }

                    if ( prep_count == 1 )
                    {
                        _sw_hdcp_access_enable(0);
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
            if ( g_pstDev->vars.vclk == 0 )
            {
                it6635_fsm_chg(RX_WAIT_CLOCK);
            }
            else
            {
			#if EN_AUTO_RS
                if ( g_pstDev->vars.try_fixed_EQ )
                {
                    _eq_set_txoe_ready(1);
                    //it6635_fsm_chg(TX_OUTPUT);
                    it6635_fsm_chg(TX_OUTPUT_PREPARE);
                }
                else
                {
                    if ( g_pstDev->opts.active_rx_opt->EnableAutoEQ )
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
                if ( g_pstDev->vars.RxHPDFlag[g_pstDev->vars.Rx_active_port] == 0 )
                {
                    it6635_fsm_chg(RX_UNPLUG);
                }

                if ( g_pstDev->vars.current_hdmi_mode != HDMI_MODE_AUTO )
                {
                    g_pstDev->vars.count_wait_clock++;
                    if ( g_pstDev->vars.count_wait_clock > 100 )
                    {
                        g_pstDev->vars.count_wait_clock = 0;
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
                if ( g_pstDev->vars.force_hpd_state == SW_HPD_LOW )
                {
                    break;
                }

                if ( g_pstDev->vars.state_sys_fsm != RX_TOGGLE_HPD )
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

    if ( g_pstDev->vars.fsm_return == 0 )
    {
        g_pstDev->vars.fsm_return = 1;
        //goto LOOP_FSM;
    }
    UNUSED(prep_count);
    UNUSED(prep_fail_count);
}


void it6635_irq(void)
{
    u8 sys_int_sts;

    it6635_detect_ports();

    if ( g_pstDev->vars.state_sys_fsm == RX_TOGGLE_HPD )
    {
        return;
    }

    {
        //static u8 prevBD =1;
        u8 currBD = h2swrd(0xBD);
        //if ( currBD != prevBD ) {
        if ( currBD & 0xe0 ) {
            g_pstDev->vars.tick_hdcp = get_tick_count();
            PR_INFO(("---HDCP BD=%02x (%lu)\r\n", currBD, (u32)(g_pstDev->vars.tick_hdcp-g_pstDev->vars.tick_set_afe)));
            //prevBD = currBD;
            _sw_clear_hdcp_status();

            /*
            if ( currBD & 0x10 ) {
                if ( prevBD ) {
                    prevBD = 0;
                    _sw_hdcp_access_enable(0);
                }
                else
                {
                    _sw_hdcp_access_enable(1);
                }
            }
            */

        }
    }

    sys_int_sts = h2swrd(0x0C);

    if ( sys_int_sts == 0x00 )
    {
        return;
    }

    if ( sys_int_sts & 0x01 )
    {
        it6635_rx_irq();
    }

    if ( sys_int_sts & 0x10 )
    {
        it6635_sw_irq(g_pstDev->vars.Rx_active_port);
        it6635_tx_irq();
    }

    #if IT6635_EN_CEC
    if ( g_pstDev->opts.EnCEC && (sys_int_sts & 0x80)  )
    {
        Cec_Irq();
    }
    if(g_pstDev->opts.EnCEC)
    {
		CecSys_TxHandler();
    	CecSys_RxHandler();
    }
    #endif
}

bool it6635_device_init(void)
{
    u8 i;
    bool init_done = FALSE;

    switch ( g_pstDev->vars.state_dev_init )
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
            it6635_set_RS(g_pstDev->opts.active_rx_opt->DefaultEQ[0],
                          g_pstDev->opts.active_rx_opt->DefaultEQ[1],
                          g_pstDev->opts.active_rx_opt->DefaultEQ[2]);


            if ( g_pstDev->opts.tx_opt->CopyEDIDFromSink )
            {
                it6635_set_tx_5v(1);
                g_pstDev->vars.state_dev_init = 1;
                g_pstDev->vars.hpd_wait_count = 0;
            }
            else
            {
                g_pstDev->vars.state_dev_init = 2;
            }
            break;

        case 1:
            if ( _tx_is_sink_hpd_high() )
            {
            	if ( g_pstDev->opts.active_rx_opt->EnRxDDCBypass == false )
            	{
	                if ( it6635_setup_edid_ram(0) )
	                {
	                    g_pstDev->vars.edid_ready = 1;
	                }
	                g_pstDev->vars.state_dev_init = 3;
            	}
            	else
            	{
            		g_pstDev->vars.edid_ready = 1;
            		g_pstDev->vars.state_dev_init = 3;
            	}
            }
            else
            {
                g_pstDev->vars.hpd_wait_count++;
                if ( g_pstDev->vars.hpd_wait_count > 200 )
                {
                    //g_pstDev->vars.state_dev_init = 2;
                    g_pstDev->vars.hpd_wait_count = 0;
                    //PR_INFO(("waiting HPD...\r\n"));
                }
                //it6635_set_tx_5v(_rx_is_5v_active());
            }
            break;

        case 2:
            // load FW default EDID
            PR_INFO(("Using internal EDID...\r\n"));
            if ( g_pstDev->opts.active_rx_opt->EnRxDDCBypass == false )
            {
                g_pstDev->vars.default_edid[0] = (u8 *)s_default_edid_port0;
                g_pstDev->vars.default_edid[1] = (u8 *)s_default_edid_port0;
                g_pstDev->vars.default_edid[2] = (u8 *)s_default_edid_port0;
                g_pstDev->vars.default_edid[3] = (u8 *)s_default_edid_port0;

                // note:
                // The EDID can be different from each port.
                // please set default_edid[?] pointer to a desired EDID array
                // if you need different EDID.
                //
                // for example:
                // g_pstDev->vars.default_edid[1] = s_default_edid_port1;
                // g_pstDev->vars.default_edid[2] = s_default_edid_port2;
                // g_pstDev->vars.default_edid[3] = s_default_edid_port3;

                for ( i = 0; i < RX_PORT_COUNT; i++ )
                {
                    it6635_setup_edid_ram(i);
                }
            }

            g_pstDev->vars.edid_ready = 1;
            g_pstDev->vars.state_dev_init = 3;

            break;

        case 3:
            _sw_int_enable_all(1);
            _rx_set_hpd_with_5v_all(1);

            PR_INFO(("Active port = P%d\r\n", g_pstDev->vars.Rx_active_port));

            it6635_set_tx_5v(_rx_is_5v_active());

            init_done = TRUE;
            break;

        default:
            g_pstDev->vars.state_dev_init = 0;
            break;
    }

    return init_done;
}

void it6635_vars_init(void)
{
    // FW Variables init:
    g_pstDev->vars.state_dev_init = 0;
    g_pstDev->vars.VSDBOffset = 0xFF;
    g_pstDev->vars.PhyAdr[0] = 0;
    g_pstDev->vars.PhyAdr[1] = 0;
    g_pstDev->vars.PhyAdr[2] = 0;
    g_pstDev->vars.PhyAdr[3] = 0;

    g_pstDev->vars.RxHPDFlag[0] = 1;
    g_pstDev->vars.RxHPDFlag[1] = 1;
    g_pstDev->vars.RxHPDFlag[2] = 1;
    g_pstDev->vars.RxHPDFlag[3] = 1;
    g_pstDev->vars.Tx_current_5v = 1;

    g_pstDev->vars.count_eq_check = 0;
    g_pstDev->vars.count_fsm_err = 0;
    g_pstDev->vars.count_unlock = 0;
    g_pstDev->vars.state_sys_fsm = RX_UNPLUG;
    g_pstDev->EQ.AutoEQ_state = AEQ_OFF;
    g_pstDev->EQ.sys_aEQ = SysAEQ_RUN;
    g_pstDev->vars.edid_ready = 0;
    g_pstDev->vars.current_txoe = 0xFF;
    g_pstDev->vars.check_for_hpd_toggle = 0;
    g_pstDev->vars.sdi_stable_count = 0;
    g_pstDev->vars.check_for_sdi = 1;
    g_pstDev->vars.force_hpd_state = SW_HPD_AUTO; // 1 : auto, don't modify here

    if ( g_pstDev->opts.active_rx_opt->TryFixedEQFirst )
    {
        g_pstDev->vars.try_fixed_EQ = 1;
    }

    g_pstDev->vars.current_hdmi_mode = HDMI_MODE_AUTO;
	g_pstDev->vars.rx_deskew_err = 0;
    _eq_reset_state();
    _eq_reset_txoe_ready();

    it6635_dump_opts();
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
                g_pstDev->vars.Rev = h2swrd(0x04);
                PR_INFO(("Find 6635 %02x !! \r\n",(int)g_pstDev->vars.Rev));
                return TRUE;
            }
        }
        else if ( vendor_id[1] == 0x35 )
        {
            vendor_id[0] = h2swrd(0x04);
            if ( vendor_id[0] == 0x66 )
            {
                g_pstDev->vars.Rev = h2swrd(0x05);
                PR_INFO(("Find 6635x %02x !! \r\n",(int)g_pstDev->vars.Rev));
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
    h2swwr(0x41, block/2);           // EDID Segment

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
                    g_pstDev->vars.is_hdmi20_sink = 1;
                    break;
                }
            }

            if ( len == 0 )
                break;
            off += len;
        }
    }

    PR_INFO(("HDMI2 sink=%d\n", g_pstDev->vars.is_hdmi20_sink));
}

/**
 * it6635_parse_edid_for_phyaddr - parse necessary data for RX
 * EDID
 *
 * @edid: 128 byte EDID data buffer that contains HDMI CEA ext
 *
 * Before set RX EDID, must call it6635_parse_edid_cea_ext to
 * initialize some variables.
 */
void it6635_parse_edid_for_phyaddr(u8 *edid)
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
                    g_pstDev->vars.VSDBOffset = ((u8)off) + 0x80 + 4;
                    g_pstDev->vars.PhyAdr[0] = (edid[off + 4] >> 4) & 0xF;
                    g_pstDev->vars.PhyAdr[1] = edid[off + 4] & 0xF;
                    g_pstDev->vars.PhyAdr[2] = (edid[off + 5] >> 4) & 0xF;
                    g_pstDev->vars.PhyAdr[3] = edid[off + 5] & 0xF;
                    g_pstDev->vars.EdidChkSum[1] = (0x100 - edid[0x7F] - edid[off + 4] - edid[off + 5]) & 0xFF;
                    break;
                }
            }

            if ( len == 0 )
                break;
            off += len;
        }
    }
}

void it6635_setup_edid_ram_step2(u8 block)
{
    int i;
    u8 wcount = 16;
    u8 phyAB, phyCD;
    u8 mask;
    u16 sum;

    PR_INFO(("Set RX EDID step2...\r\n"));

    if ( block == 0 )
    {
        for ( i = 0; i < 4; i++ )
        {
            _rx_edid_set_chksum(i, g_pstDev->vars.EdidChkSum[0]);
        }
    }

    if ( block == 1 )
    {
        h2rxwr(0x4B, g_pstDev->opts.dev_opt->EdidAddr | 0x01);
        h2rxset(0x4C, 0x0F, 0x0F);

        h2swwr(0xe9, g_pstDev->vars.VSDBOffset);    // VSDB_Offset
        PR_INFO(("VSDB=%02x\r\n", g_pstDev->vars.VSDBOffset));

        phyAB = (g_pstDev->vars.PhyAdr[0] << 4) | (g_pstDev->vars.PhyAdr[1] & 0xF);
        phyCD = (g_pstDev->vars.PhyAdr[2] << 4) | (g_pstDev->vars.PhyAdr[3] & 0xF);

        for ( i = 0; i < 4; i++ )
        {
            h2swwr(0xd9 + i * 2, phyAB);    // Port0 VSDB_AB
            h2swwr(0xda + i * 2, phyCD);    // Port0 VSDB_CD
        }

        for ( i = 0; i < 4; i++ )
        {
            if ( g_pstDev->vars.PhyAdr[i] == 0 )
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
            sum = (0x100 - g_pstDev->vars.EdidChkSum[1] - phyAB - phyCD) & 0xFF;
            h2swwr(0xe2 + i * 2, (u8)sum);

            //if ( g_pstDev->vars.Rev >= 0xC )
            {
				#if 0
                switch(i)
                {
                    case 0: mask = 1<<1; break;
                    case 1: mask = 1<<2; break;
                    case 2: mask = 1<<0; break;
                    case 3: mask = 1<<3; break;
                    default:mask = 1<<3; break;
                }
                #endif

				mask = 1<<i;

                h2rxset(0x4C, 0x0F, mask);

                h2rxedidwr(g_pstDev->vars.VSDBOffset, &phyAB, 1);
                h2rxedidwr(g_pstDev->vars.VSDBOffset+1, &phyCD, 1);
                phyAB = (u8)sum;
                h2rxedidwr(128+127, &phyAB, 1);
            }

        }

        h2rxwr(0x4B, g_pstDev->opts.dev_opt->EdidAddr);
    }
}

void it6635_ddc_abort( void )
{
    u8 reg3C = h2swrd(0x3C);
    u8 i,j, uc ;

    h2swset(0x3C, 0x01, 0x01);
    for( i = 0 ;i < 2 ; i++ )
    {
        h2swwr(0x3D, 0x0F);
        for(j = 0 ; j < 50 ; j++ )
        {
            uc = h2swrd(0x1B);
            if( uc &0x80)
            {
                // DDC_FW_Stus_DONE
                break ;
            }
            if( uc & 0x38)
            {
                // DDC has something error
                PR_ERR(("ERROR: DDC 0x1B=%02X\r\n",uc));
                break ;
            }
            delay1ms(1) ;
        }
    }
    h2swwr(0x3D, 0x0A);
    h2swwr(0x3D, 0x09);
    h2swwr(0x3C, reg3C);
}

bool it6635_update_edid(u8 block, u8 *edid_buf, u8 flag)
{
    u8 i;
    bool ret;
    u8 retry=0;

__RETRY_EDID_READ:
    if ( g_pstDev->opts.tx_opt->CopyEDIDFromSink )
    {
        ret = it6635_read_one_block_edid(block, edid_buf);
        if ( false == ret )
        {
            PR_ERR(("ERROR: read edid block 0\r\n"));
            if ( retry <3 )
            {
                retry++;
                it6635_ddc_abort();
                goto __RETRY_EDID_READ;
            }
        }
    }
    else
    {
        u8 *def_edid = g_pstDev->vars.default_edid[flag];

        if ( def_edid )
        {
            for (i=0 ; i<128 ; i++)
            {
                edid_buf[i] = def_edid[i+block*128];
            }
            ret = true;
        }
        else
        {
            ret = false;
        }
    }

    return ret;
}

bool it6635_setup_edid_ram(u8 flag)
{
    u8 edid_tmp[128];
    u8 extblock;
    u8 i;

    g_pstDev->vars.spmon = 0;
    g_pstDev->vars.is_hdmi20_sink = 0;

    if ( false == it6635_update_edid(0, edid_tmp, flag) )
    {
        goto __err_exit;
    }

    if ( (edid_tmp[0x08] == 0x5A) &&
         (edid_tmp[0x09] == 0x63) &&
         (edid_tmp[0x0a] == 0x32) &&
         (edid_tmp[0x0b] == 0x0e) )
    {
        g_pstDev->vars.spmon = 1;
    }

    if ( (edid_tmp[0x71] == 0x4C) &&
         (edid_tmp[0x72] == 0x47) &&
         (edid_tmp[0x74] == 0x54) &&
         (edid_tmp[0x75] == 0x56) &&
         (edid_tmp[0x7F] == 0x63) )
    {
        g_pstDev->vars.spmon = 2;
    }

    if ( (edid_tmp[0x60] == 0x48) &&
         (edid_tmp[0x61] == 0x4C) &&
         (edid_tmp[0x63] == 0x32) &&
         (edid_tmp[0x64] == 0x37) &&
         (edid_tmp[0x65] == 0x36) &&
         (edid_tmp[0x66] == 0x45) &&
         (edid_tmp[0x67] == 0x38) &&
         (edid_tmp[0x68] == 0x56) )
    {
        g_pstDev->vars.spmon = 3;
    }

    // read Ext block no
    extblock = edid_tmp[0x7E];
    g_pstDev->vars.EdidChkSum[0] = edid_tmp[0x7F];

    #if FIX_EDID_FOR_ATC_4BLOCK_CTS
    if ( extblock > 1 )
    {
        edid_tmp[0x7E] = 1;
        g_pstDev->vars.EdidChkSum[0] = _rx_calc_edid_sum(edid_tmp);
    }
    #endif

    _pr_buf(edid_tmp, 128);

    if ( g_pstDev->opts.tx_opt->CopyEDIDFromSink )
    {
        // update EDID block 0 for all port
        it6635_set_internal_EDID(0, edid_tmp, EDID_PORT_ALL);
    }
    else
    {
        // update EDID block 0 for assigned port
        it6635_set_internal_EDID(0, edid_tmp, (1<<flag));
    }
    it6635_setup_edid_ram_step2(0);

    if ( extblock > 3 )
    {
        PR_ERR(("Warning: Extblock = %d\r\n", extblock));
        extblock = 3;
    }

    for ( i = 1; i <= extblock; i++ )
    {
        if ( false == it6635_update_edid(i, edid_tmp, flag) )
        {
            goto __err_exit;
        }

        g_pstDev->vars.VSDBOffset = 0;
        it6635_parse_edid_for_vsdb(edid_tmp);

        if ( i==1 ) // assume our sink has only 2 block EDID
        {
		    if ( g_pstDev->vars.spmon == 2 )
		    {
		    	if ( edid_tmp[0x7F] != 0x6A)
		    	{
		    		g_pstDev->vars.spmon = 0;
		    	}
		    }

            _pr_buf(edid_tmp, 128);

            if ( g_pstDev->opts.tx_opt->CopyEDIDFromSink )
            {
                // update EDID block 0 for all port
                it6635_set_internal_EDID(1, edid_tmp, EDID_PORT_ALL);
            }
            else
            {
                // update EDID block 0 for assigned port
                it6635_set_internal_EDID(1, edid_tmp, (1<<flag));
            }

            if ( g_pstDev->opts.tx_opt->ParsePhysicalAddr )
            {
                it6635_parse_edid_for_phyaddr(edid_tmp);

                if ( g_pstDev->vars.VSDBOffset )
                {
                    it6635_setup_edid_ram_step2(1);

                    // break;
                    // break parsing here will make the 4 block EDID CTS fail.
                }
            }
        }
    } // for i

    return true;

__err_exit:
    return false;
}

void it6635_dev_loop(void)
{
    u8 l_u8DevID;
    #if CHECK_DEV_PRESENT
    for(l_u8DevID = 0;l_u8DevID<IT6635_I2C_DEV_NUM;l_u8DevID++)
    {
        it6635_DeviceSelect(l_u8DevID);
        if ( dev_state < DEV_WAIT_DEVICE_READY ) {
            if ( it6635_is_device_lost() )
            {
                dev_state = DEV_FW_VAR_INIT;
            }
        }
    }
    #endif

    switch ( dev_state )
    {
        case DEV_DEVICE_LOOP:
            {
                for(l_u8DevID = 0;l_u8DevID<IT6635_I2C_DEV_NUM;l_u8DevID++)
                {
                    it6635_DeviceSelect(l_u8DevID);
                    it6635_fsm();
                    it6635_irq();
                    _rx_update_ced_err_from_hw();
                    if(pvpmappIT6635XUserFunction != NULL)
                    {
                        pvpmappIT6635XUserFunction();
                    }
                }
            }
            break;

        case DEV_DEVICE_INIT:
            {
                u8 l_u8Sta[IT6635_I2C_DEV_NUM];
                bool l_bInitOk = TRUE;
                for(l_u8DevID = 0;l_u8DevID<IT6635_I2C_DEV_NUM;l_u8DevID++)
                {
                    l_u8Sta[l_u8DevID] = 0;
                    it6635_DeviceSelect(l_u8DevID);
                    l_u8Sta[l_u8DevID] = it6635_device_init();
                }
                for(l_u8DevID = 0;l_u8DevID<IT6635_I2C_DEV_NUM;l_u8DevID++)
                {
                    l_u8Sta[0] += l_u8Sta[l_u8DevID];
                }
                if(l_u8Sta[0]<IT6635_I2C_DEV_NUM)
                    l_bInitOk = FALSE;
                if(l_bInitOk)
                {
                    dev_state = DEV_DEVICE_LOOP;
                }
            }
            break;

        case DEV_WAIT_DEVICE_READY:
            {
                u8 l_u8Sta[IT6635_I2C_DEV_NUM];
                bool l_bInitOk = TRUE;
                for(l_u8DevID = 0;l_u8DevID<IT6635_I2C_DEV_NUM;l_u8DevID++)
                {
                    l_u8Sta[l_u8DevID] = 0;
                    it6635_DeviceSelect(l_u8DevID);
                    l_u8Sta[l_u8DevID] = it6635_is_device_ready();
                }
                for(l_u8DevID = 0;l_u8DevID<IT6635_I2C_DEV_NUM;l_u8DevID++)
                {
                    l_u8Sta[0] += l_u8Sta[l_u8DevID];
                }
                if(l_u8Sta[0]<IT6635_I2C_DEV_NUM)
                    l_bInitOk = FALSE;
                if(l_bInitOk)
                {
                    dev_state = DEV_DEVICE_INIT;
                }
            }
            break;

        case DEV_FW_VAR_INIT:
            {
                for(l_u8DevID = 0;l_u8DevID<IT6635_I2C_DEV_NUM;l_u8DevID++)
                {
                    it6635_DeviceSelect(l_u8DevID);
                    it6635_vars_init();
                }
                dev_state = DEV_WAIT_DEVICE_READY;
            }
            break;

        default:
            break;
    }

}

///////

//
// APIs:
//

void it6635_dump_opts(void)
{
    PR_INFO((".rx_opt->tag1=%d\r\n", g_pstDev->opts.active_rx_opt->tag1));
    PR_INFO((".rx_opt->EnRxDDCBypass=%d\r\n", g_pstDev->opts.active_rx_opt->EnRxDDCBypass));
    PR_INFO((".rx_opt->EnRxPWR5VBypass=%d\r\n", g_pstDev->opts.active_rx_opt->EnRxPWR5VBypass));
    PR_INFO((".rx_opt->EnRxHPDBypass=%d\r\n", g_pstDev->opts.active_rx_opt->EnRxHPDBypass));
    PR_INFO((".rx_opt->TryFixedEQFirst=%d\r\n", g_pstDev->opts.active_rx_opt->TryFixedEQFirst));
    PR_INFO((".rx_opt->EnableAutoEQ=%d\r\n", g_pstDev->opts.active_rx_opt->EnableAutoEQ));
    PR_INFO((".rx_opt->NonActivePortReplyHPD=%d\r\n", g_pstDev->opts.active_rx_opt->NonActivePortReplyHPD));
    PR_INFO((".rx_opt->DisableEdidRam=%d\r\n", g_pstDev->opts.active_rx_opt->DisableEdidRam));
    PR_INFO((".rx_opt->DefaultEQ=%x %x %x\r\n", g_pstDev->opts.active_rx_opt->DefaultEQ[0], g_pstDev->opts.active_rx_opt->DefaultEQ[1], g_pstDev->opts.active_rx_opt->DefaultEQ[2]));
    PR_INFO((".rx_opt->FixIncorrectHdmiEnc=%d\r\n", g_pstDev->opts.active_rx_opt->FixIncorrectHdmiEnc));
    PR_INFO((".rx_opt->HPDOutputInverse=%d\r\n", g_pstDev->opts.active_rx_opt->HPDOutputInverse));
    PR_INFO((".rx_opt->HPDTogglePeriod=%d\r\n", g_pstDev->opts.active_rx_opt->HPDTogglePeriod));
    PR_INFO((".rx_opt->TxOEAlignment=%d\r\n", g_pstDev->opts.active_rx_opt->TxOEAlignment));
    PR_INFO((".rx_opt->str_size=%d\r\n", g_pstDev->opts.active_rx_opt->str_size));

    PR_INFO((".tx_opt->tag1=%d\r\n", g_pstDev->opts.tx_opt->tag1));
    PR_INFO((".tx_opt->EnTxPNSwap=%d\r\n", g_pstDev->opts.tx_opt->EnTxPNSwap));
    PR_INFO((".tx_opt->EnTxChSwap=%d\r\n", g_pstDev->opts.tx_opt->EnTxChSwap));
    PR_INFO((".tx_opt->EnTxVCLKInv=%d\r\n", g_pstDev->opts.tx_opt->EnTxVCLKInv));
    PR_INFO((".tx_opt->EnTxOutD1t=%d\r\n", g_pstDev->opts.tx_opt->EnTxOutD1t));
    PR_INFO((".tx_opt->CopyEDIDFromSink=%d\r\n", g_pstDev->opts.tx_opt->CopyEDIDFromSink));
    PR_INFO((".tx_opt->ParsePhysicalAddr=%d\r\n", g_pstDev->opts.tx_opt->ParsePhysicalAddr));
    PR_INFO((".tx_opt->TurnOffTx5VWhenSwitchPort=%d\r\n", g_pstDev->opts.tx_opt->TurnOffTx5VWhenSwitchPort));
    PR_INFO((".tx_opt->str_size=%d\r\n", g_pstDev->opts.tx_opt->str_size));

    PR_INFO((".dev_opt->tag1=0x%02x\r\n", g_pstDev->opts.dev_opt->tag1));
    PR_INFO((".dev_opt->SwAddr=0x%02x\r\n", g_pstDev->opts.dev_opt->SwAddr));
    PR_INFO((".dev_opt->RxAddr=0x%02x\r\n", g_pstDev->opts.dev_opt->RxAddr));
    PR_INFO((".dev_opt->CecAddr=0x%02x\r\n", g_pstDev->opts.dev_opt->CecAddr));
    PR_INFO((".dev_opt->EdidAddr=0x%02x\r\n", g_pstDev->opts.dev_opt->EdidAddr));
    PR_INFO((".dev_opt->ForceRxOn=%d\r\n", g_pstDev->opts.dev_opt->ForceRxOn));
    PR_INFO((".dev_opt->RxAutoPowerDown=%d\r\n", g_pstDev->opts.dev_opt->RxAutoPowerDown));
    PR_INFO((".dev_opt->DoTxPowerDown=%d\r\n", g_pstDev->opts.dev_opt->DoTxPowerDown));
    PR_INFO((".dev_opt->TxPowerDownWhileWaitingClock=%d\r\n", g_pstDev->opts.dev_opt->TxPowerDownWhileWaitingClock));
    PR_INFO((".dev_opt->str_size=%d\r\n", g_pstDev->opts.dev_opt->str_size));
}

#define BUF_LEN 16
void it6635_dump_register( u8 addr, char *reg_desc)
{
    u8 regbuf[BUF_LEN];
    int i,j;

    // print reg description
    PR_INFO((reg_desc));

    // print table
    PR_DATA(("   | "));
    for (j=0;j<BUF_LEN;j++)
    {
        if ( j==(BUF_LEN/2) )
        {
            PR_DATA(("- "));
        }
        PR_DATA(("%02X ", j));
    }
    PR_DATA(("\n"));

    // print split line
    for (j=0;j<BUF_LEN+2;j++)
    {
        PR_DATA(("---"));
    }
    PR_DATA(("\n"));

    // print register values
    for (i=0;i<256;i+=BUF_LEN)
    {
        PR_DATA(("%02X | ", i));
        it6635_i2c_read(addr, i, BUF_LEN, regbuf);
        for (j=0;j<BUF_LEN;j++)
        {
            if ( j==(BUF_LEN/2) )
            {
                PR_DATA(("- "));
            }
            PR_DATA(("%02x ", regbuf[j]));
        }
        PR_DATA(("\n"));
    }
    PR_DATA(("\n"));
}

void it6635_dump_register_all(void)
{
    it6635_dump_register(g_pstDev->opts.dev_opt->SwAddr, "\n*** Switch Register:\n");

    it6635_dump_register(g_pstDev->opts.dev_opt->RxAddr, "\n*** RX Register(0):\n");
    chgrxbank(3);
    it6635_dump_register(g_pstDev->opts.dev_opt->RxAddr, "\n*** RX Register(3):\n");
    chgrxbank(5);
    it6635_dump_register(g_pstDev->opts.dev_opt->RxAddr, "\n*** RX Register(5):\n");
    chgrxbank(0);

    // dump EDID RAM
    h2rxwr(0x4B, g_pstDev->opts.dev_opt->EdidAddr | 0x01); // enable EDID RAM i2c address
    h2rxset(0x4C, 0x30, 0x00);
    it6635_dump_register(g_pstDev->opts.dev_opt->EdidAddr, "\n*** EDID Port 0:\n");
    h2rxset(0x4C, 0x30, 0x10);
    it6635_dump_register(g_pstDev->opts.dev_opt->EdidAddr, "\n*** EDID Port 1:\n");
    h2rxset(0x4C, 0x30, 0x20);
    it6635_dump_register(g_pstDev->opts.dev_opt->EdidAddr, "\n*** EDID Port 2:\n");
    h2rxwr(0x4B, g_pstDev->opts.dev_opt->EdidAddr); // disable EDID RAM i2c address

    #if EN_CEC
    it6635_dump_register(g_pstDev->opts.dev_opt->CecAddr, "\n*** CEC Register:\n");
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

    h2swset(0xF5, 0x80, (g_pstDev->opts.active_rx_opt->EnRxDDCBypass << 7));
    if ( g_pstDev->opts.active_rx_opt->EnRxDDCBypass == 0 )
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
    if ( port < RX_PORT_COUNT )
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
    if ( port < RX_PORT_COUNT )
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
    if ( port < RX_PORT_COUNT )
    {
        if ( g_pstDev->vars.Rx_active_port != port )
        {
            g_pstDev->vars.Rx_new_port = port;
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
    return g_pstDev->vars.Rx_active_port;
}

void it6635_set_option(IT6635_DEVICE_OPTION *Opts)
{
    if ( Opts )
    {
        //g_pstDev->opts.EnableAutoEQ = Opts->EnableAutoEQ;
        //g_pstDev->opts.rx_opt->EnRxDDCBypass = Opts->EnRxDDCBypass;
    }
}

void it6635_get_option(IT6635_DEVICE_OPTION *Opts)
{
    if ( Opts )
    {
        //Opts->EnableAutoEQ = g_pstDev->opts.EnableAutoEQ;
        //Opts->EnRxDDCBypass = g_pstDev->opts.rx_opt->EnRxDDCBypass;
    }
}

void it6635_dev_restart(void)
{
    //g_pstDev->vars.Rx_prev_port = -1;
    g_pstDev->vars.state_sys_fsm = RX_UNPLUG;

    dev_state = DEV_WAIT_DEVICE_READY;
}

u8 it6635_get_RS(void)
{
    //return g_pstDev->EQ.FixedRsIndex[g_pstDev->vars.Rx_active_port];
    return 0;
}

void it6635_set_RS(u8 rs_idx0, u8 rs_idx1, u8 rs_idx2 )
    {
    u8 rs[3];
    if ( (rs_idx0 < 14) && ( rs_idx1 < 14 ) && ( rs_idx2 < 14 ) )
    {
        //g_pstDev->EQ.FixedRsIndex[    g_pstDev->vars.Rx_active_port] = rs_index;

        rs[0] = RS_Value[rs_idx0] | 0x80;
        rs[1] = RS_Value[rs_idx1] | 0x80;
        rs[2] = RS_Value[rs_idx2] | 0x80;
        _rx_set_rs_3ch(rs);

        chgrxbank(3);
        h2rxbrd(0x27, 3, rs);
        //h2rxset(0x22, 0x40, 0x00);
        chgrxbank(0);
        PR_INFO(("==> RS set to %02x %02x %02x\r\n", rs[0], rs[1], rs[2]));

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
    _rx_set_hpd(g_pstDev->vars.Rx_active_port, hpd_value, TERM_FOLLOW_HPD);
}

/**
 * it6635_set_internal_EDID - write data to EDID RAM
 *
 * @edid: 128 byte EDID data buffer.
 *
 * @block: EDID block number (0,1 or 2)
 *
 * target_port is a bitmap from 0x1 to 0xF
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

    h2rxwr(0x4B, g_pstDev->opts.dev_opt->EdidAddr | 0x01); // enable EDID RAM i2c address

    h2rxset(0x4C, 0x0F, target_port);  // for block 1, select port to be written

    for ( i = 0; i < 128; i += wcount )
    {
        h2rxedidwr(i + 128 * block, (u8 *)edid, wcount);
        edid += wcount;
    }

    h2rxwr(0x4B, g_pstDev->opts.dev_opt->EdidAddr);  // disable EDID RAM i2c address
}

/**
 * it6635_get_internal_EDID - read data from EDID RAM
 *
 * @edid: 128 byte EDID data buffer.
 *
 * @block: EDID block number (0,1,2 or 3)
 *
 */
void it6635_get_internal_EDID( u8 block, u8 *edid, u8 target_port )
{
    int i;
    u8 wcount = 16;

    if ( block > 1 )
    {
        PR_ERR(("Invalid block %d\r\n", block));
        return;
    }
    if ( target_port > 2 )
    {
        PR_ERR(("Invalid port %d\r\n", target_port));
        return;
    }

    h2rxwr(0x4B, g_pstDev->opts.dev_opt->EdidAddr | 0x01); // enable EDID RAM i2c address
    h2rxset(0x4C, 0x30, target_port<<4);

    for ( i = 0; i < 128; i += wcount )
    {
        h2rxedidrd(i + 128 * block, edid, wcount);
        edid += wcount;
    }

    h2rxwr(0x4B, g_pstDev->opts.dev_opt->EdidAddr); // disable EDID RAM i2c address
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
void it6635_change_default_RS( u8 port, u8 new_rs_idx0, u8 new_rs_idx1, u8 new_rs_idx2, u8 update_hw )
{
    if ( port <= RX_PORT_3 )
    {
        g_pstDev->opts.rx_opt[port]->DefaultEQ[0] = new_rs_idx0;
        g_pstDev->opts.rx_opt[port]->DefaultEQ[1] = new_rs_idx1;
        g_pstDev->opts.rx_opt[port]->DefaultEQ[2] = new_rs_idx2;
        if ( update_hw && (port == g_pstDev->vars.Rx_active_port) )
        {
            it6635_set_RS( new_rs_idx0, new_rs_idx1, new_rs_idx2 );
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
 * g_pstDev->vars.force_hpd_state will reset to SW_HPD_AUTO when
 * active port changed by it6635_fsm_chg(RX_PORT_CHANGE)
 *
 */
void it6635_force_rx_hpd( u8 hpd_state )
{
    g_pstDev->vars.force_hpd_state = hpd_state;

    if ( hpd_state ) // hpd 0 --> hpd auto
    {
        // nothing to do here:
        // hpd will be controlled by it6635_fsm()
    }
    else // hpd auto --> hpd 0
    {
        _rx_set_hpd(g_pstDev->vars.Rx_active_port, hpd_state, TERM_FOLLOW_HPD);
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

    _rx_set_hpd(g_pstDev->vars.Rx_active_port, 0, TERM_FOLLOW_HPD);
    _tx_scdc_write(0x20,0x00);
    g_pstDev->vars.hpd_toggle_timeout = timeout;
    it6635_fsm_chg(RX_TOGGLE_HPD);

    return ret;
}

void it6635_read_tx_edid(u8 *pu8EdidBuf)
{
    u8 i;
    bool ret;
    u8 retry=0;
    u8 block = 0;

__RETRY_EDID_READ:
    ret = it6635_read_one_block_edid(block, pu8EdidBuf);
    if ( false == ret )
    {
        PR_ERR(("ERROR: read edid block 0\r\n"));
        if ( retry <3 )
        {
            retry++;
            it6635_ddc_abort();
            goto __RETRY_EDID_READ;
        }
    }

    if(pu8EdidBuf[0x7E]<=0)
    {
        memset(pu8EdidBuf+0x80,0,128);
        return;
    }

    for(i=0;i<pu8EdidBuf[0x7E];i++)
    {
        block = i+1;
        ret = it6635_read_one_block_edid(block, &pu8EdidBuf[0x80*(i+1)]);
        if ( false == ret )
        {
            PR_ERR(("ERROR: read edid block 0\r\n"));
            if ( retry <3 )
            {
                retry++;
                it6635_ddc_abort();
            }
        }
    }
}

