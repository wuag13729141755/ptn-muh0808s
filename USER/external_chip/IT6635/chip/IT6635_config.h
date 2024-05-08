///*****************************************
//  Copyright (C) 2009-2019
//  ITE Tech. Inc. All Rights Reserved
//  Proprietary and Confidential
///*****************************************
//   @file   <config.h>
//   @author Kenneth.Hung@ite.com.tw
//   @date   2019/06/24
//   @fileversion: IT6635_SAMPLE_1.20
//******************************************/
#ifndef _CONFIG_H_
#define _CONFIG_H_

#ifndef _SHOW_PRAGMA_MSG
#define message(ignore)
#endif

#define IT6635_I2C_DEV_ID       _I2C_TABLE_INDEX_IT66351

//
// Switch register i2c address: 0x94(PCADR=0) or 0x96(PCADR=1)
//
#define SWAddr          0x94


//
// RX register i2c address ( programmable )
//
#define RXAddr			0xB2


//
// CEC register i2c address ( programmable )
//
#define CECAddr         0xC0


//
// EDID RAM i2c address ( programmable )
//
#define RXEDIDAddr      0xa8


//
// TX_PN_SWAP
// 1: Enable TX side TMDS P/N swap
// 0: Disable TX side TMDS P/N swap
#define TX_PN_SWAP 1
#ifndef TX_PN_SWAP

    //#pragma message("TX_PN_SWAP is defined to 0")
    //#pragma message("IT6635 EVB should be TX_PN_SWAP==1")
	//#error ("Please define TX_PN_SWAP by your PCB layout.")

#else

    #if TX_PN_SWAP
		#ifdef _SHOW_PRAGMA_MSG
	    //#pragma message("TX_PN_SWAP is pre-defined to 1")
		#endif
    #else
		#ifdef _SHOW_PRAGMA_MSG
	    //#pragma message("TX_PN_SWAP is pre-defined to 0")
		#endif
    #endif

#endif

//
// DEFAULT_RS_IDX
// :The default EQ when power on.
//
#define DEFAULT_RS_IDX 5


//
// DEFAULT_PORT
// :The default active port when power on.
#define DEFAULT_PORT   0


//
// NON_ACTIVE_PORT_REPLY_HPD
// 1 : Non-Active RX port HPD will set HPD high if HDMI 5V in.
// 0 : Always set HPD to low until the port become active.
#define NON_ACTIVE_PORT_REPLY_HPD 0


//
// RX_HPD_OUTPUT_INVERSE
// 1 : RX port HPD output will be inversed,
// 0 : normal
#define RX_HPD_OUTPUT_INVERSE 0


//
// RX_HPD_OUTPUT_INVERSE_MAP
// only when RX_HPD_OUTPUT_INVERSE==1
// 0x01 = port 0 inverse enabled
// 0x02 = port 1 inverse enabled
// 0x04 = port 2 inverse enabled
// 0x08 = port 3 inverse enabled
// 0x05 = port 0/2 inverse enabled
// 0x0C = port 2/3 inverse enabled
// 0x0F = port 0/1/2/3 inverse enabled
#define RX_HPD_OUTPUT_INVERSE_MAP 0xF


//
// DISABLE_PX_EDID_RAM
// 1 : Disable EDID RAM on port X
// 0 : Enable EDID RAM on port X
#define DISABLE_P0_EDID_RAM 0
#define DISABLE_P1_EDID_RAM 0
#define DISABLE_P2_EDID_RAM 0
#define DISABLE_P3_EDID_RAM 0


//
// ENABLE_AUTO_EQ
// 1: Enable Auto EQ adjust on active port
// 0: Use Fixed EQ ( see :DEFAULT_RS_IDX )
#define ENABLE_AUTO_EQ 1


//
// TRY_FIXED_EQ
// 1: FW will try fixed EQ before performing auto EQ adjust
// 0: Always adjust EQ when RX 5V changed
#define TRY_FIXED_EQ 1


//
// COPY_EDID_FROM_SINK
// 1: FW will read sink's EDID data and copy to EDID RAM
// 0: FW will use internal EDID data ( defined in s_default_edid[] )
#define COPY_EDID_FROM_SINK 1


//
// ********* compile options ***********
//
// CHECK_DEV_PRESENT:
// 1: FW will restart when device ID check failed.
//
#define CHECK_DEV_PRESENT 1


//
// FIX_EDID_FOR_ATC_4BLOCK_CTS:
// 1: For ATC 4 blocks EDID test
//
#define FIX_EDID_FOR_ATC_4BLOCK_CTS 1


//
// DEFAULT_RX_AUTO_POWER_DOWN:
// 1: enable RX auto power down
// 0: disable RX auto power down
//
#define DEFAULT_RX_AUTO_POWER_DOWN 0


///////////////////////
#define DEBUG_FSM_CHANGE 0

//
// HDMI Port Switch Timeout Config
//

// constant definition
#define HPD_TOGGLE_TIMEOUT_400MS (27)
#define HPD_TOGGLE_TIMEOUT_1SEC (100)
#define HPD_TOGGLE_TIMEOUT_2SEC (20|(BIT7))
#define HPD_TOGGLE_TIMEOUT_3SEC (30|(BIT7))

// config HDMI Port switch HPD toggle timeout set here
#define HPD_TOGGLE_TIMEOUT HPD_TOGGLE_TIMEOUT_3SEC


//
// IT6635_EN_CEC:
// 1: Enable CEC function
// 0: Disable CEC function
#define IT6635_EN_CEC 0


//
// EN_IT6601_ARC:
// 1: Enable ARC function ( IT6601 is required )
// 0: Disable ARC function
#define EN_IT6601_ARC 0


//
// EN_AUTO_RS: ( compile option )
// 1: Enable Auto EQ code
// 0: Disable Auto EQ code
#define EN_AUTO_RS 1

//
// EN_AUTO_RS: ( compile option )
// 1: Enable Auto EQ code
// 0: Disable Auto EQ code
#define EN_H14_SKEW 0


//
// TXOE_ALIGN:
// 1: TXOE opt 1
// 0: TXOE opt 0
#define TXOE_ALIGN 1

#define CHECK_INT_BEFORE_TXOE 1


//
// POWER_DOWN_WHILE_WAITING_CLOCK:
// 1: Power down to standby mode when active port detected no TMDS clock
// 0: Do not power down when active port detected no TMDS clock
#define POWER_DOWN_WHILE_WAITING_CLOCK 0

//
// NON_ACTIVE_PORT_DETECT_CLOCK
// 1 : Non-Active RX port HPD will turn on clock termination.
// 0 : Non-Active RX port HPD will turn off clock termination.
#define NON_ACTIVE_PORT_DETECT_CLOCK 1

#define DO_TX_POWER_DOWN 1

#define USING_WDOG 0

// add by wf8421 20191026 If Edid data error, Load default
#define DO_TX_EDID_ERROR_TO_LOAD_DEFAULT    1

#endif  // _CONFIG_H_
