///*****************************************
//  Copyright (C) 2009-2019
//  ITE Tech. Inc. All Rights Reserved
//  Proprietary and Confidential
///*****************************************
//   @file   <IT6635_drv.h>
//   @author Kenneth.Hung@ite.com.tw
//   @date   2019/06/24
//   @fileversion: IT6635_SAMPLE_1.20
//******************************************/
#ifndef _IT6635_DRV_H_
#define _IT6635_DRV_H_

//#define RX_PORT_COUNT 4

#define P0              0
#define P1              1
#define P2              2
#define P3              3

//#define TRUE            1
//#define FALSE           0

#define DDCWAITTIME     5
#define DDCWAITNUM      10

#define RX_PORT_COUNT 4

// for _rx_term_power_down
#define CH0_OFF (0x10)
#define CH1_OFF (0x20)
#define CH2_OFF (0x40)
#define CLK_OFF (0x80)
#define ALLCH_OFF (0xF0)
#define ALLCH_ON (0x00)

//===================================================
// config:
//===================================================

// RCLKFreqSel => 0: 20MHz, 1: 10MHz, 2: 5MHz, 3: 2.5MHz
#define RCLKFreqSel 0

typedef enum
{
    RX_TOGGLE_HPD,      // 0
    RX_PORT_CHANGE,
    TX_OUTPUT,
    TX_OUTPUT_PREPARE,
    RX_CHECK_EQ,
    SETUP_AFE,          // 5
    RX_WAIT_CLOCK,
    RX_HPD,
    TX_GOT_HPD,
    TX_WAIT_HPD,
    TX_UNPLUG,          // 10
	RX_UNPLUG,
	IDLE,
}_SYS_FSM_STATE;

enum
{
    HDMI_MODE_AUTO,
    HDMI_MODE_14,
    HDMI_MODE_20,
};

enum
{
    EQ_MODE_H14,
    EQ_MODE_H20,
};


typedef enum
{
    DEV_DEVICE_LOOP,
    DEV_DEVICE_INIT,
    DEV_WAIT_DEVICE_READY,
    DEV_FW_VAR_INIT,
}_DEV_FSM_STATE;

typedef enum
{
    AEQ_OFF,
    AEQ_START,
    AEQ_CHECK_SAREQ_RESULT,
    AEQ_APPLY_SAREQ,
    AEQ_DONE,
    AEQ_FAIL,
    AEQ_MAX,
}_AEQ_FSM_STATE;

typedef enum
{
    EQRES_UNKNOWN,
	EQRES_BUSY,
    EQRES_SAREQ_DONE,
    EQRES_SAREQ_FAIL,
    EQRES_SAREQ_TIMEOUT,
    EQRES_H14EQ_DONE,
    EQRES_H14EQ_FAIL,
    EQRES_H14EQ_TIMEOUT,
	EQRES_DONE,
}_EQ_RESULT_TYPE;

typedef enum
{
    SysAEQ_OFF,
    SysAEQ_RUN,
    SysAEQ_DONE,
}_SYS_AEQ_TYPE;

enum
{
    EDID_SRC_EXT_SINK,
    EDID_SRC_INTERNAL,
};

enum
{
    TERM_LOW,
    TERM_HIGH,
    TERM_FOLLOW_TX,
    TERM_FOLLOW_HPD,
};

#define EDID_PORT_0 0x01
#define EDID_PORT_1 0x02
#define EDID_PORT_2 0x04
#define EDID_PORT_3 0x08
#define EDID_PORT_ALL (EDID_PORT_0|EDID_PORT_1|EDID_PORT_2|EDID_PORT_3)

//
// for it6635_get_port_info0()
//
#define PI_5V ( BIT0 )
#define PI_HDMI_MODE ( BIT1 )
#define PI_CLK_DET ( BIT2 )
#define PI_CLK_VALID ( BIT3 )
#define PI_CLK_STABLE ( BIT4 )
#define PI_PLL_LOCK ( BIT5 )
//#define PI_XX ( BIT6 )
#define PI_SYM_LOCK ( BIT7 )

//
// for it6635_get_port_info1()
//
#define PI_PLL_HS1G (BIT0)

typedef struct
{
    // TxSwap
    u8 EnTxPNSwap;  //TRUE; // IT6635A0: TRUE,  IT66351A0: FALSE
    u8 EnTxChSwap;  //TRUE; // IT6635A0: TRUE,  IT66351A0: FALSE
    u8 EnTxVCLKInv;  //TRUE;
    u8 EnTxOutD1t;  //TRUE; // Tx Output delay 1T option

    u8 EnRxDDCBypass;
    u8 EnRxPWR5VBypass;
    u8 EnRxHPDBypass;

    u8 EnCEC;

    u8 EnableAutoEQ;
    u8 ParseEDIDFromSink;
    u8 NonActivePortReplyHPD;
    u8 DisableEdidRam;
    u8 TryFixedEQFirst;
    u8 TurnOffTx5VWhenSwitchPort;
    u8 FixIncorrectHdmiEnc;

}IT6635_DEVICE_OPTION_INT;

typedef struct
{
    IT6635_DEVICE_OPTION_INT opts;

    struct
    {
        u8 Rev;
        u32 RCLK;
        u8 RxHPDFlag[4];

        u8 VSDBOffset;  //0xFF;

        u8 PhyAdr[4];
        u8 EdidChkSum[2];

        _SYS_FSM_STATE state_sys_fsm;
        u8 state_dev_init;
        u8 state_dev;
        u8 fsm_return;
        u8 Rx_active_port;
		u8 Rx_new_port;
        u32 vclk;

        u16 RxCEDErr[3];
        u8 RxCEDErrValid;
		u16 RxCEDErrRec[3][3];

        u8 count_unlock;
        u8 count_symlock;
        u8 count_symlock_lost;
        u8 count_symlock_fail;
        u8 count_fsm_err;
        u8 count_eq_check;
        u8 count_try_force_hdmi_mode;
        u8 count_auto_eq_fail;
        u8 count_wait_clock;
        u8 clock_ratio;
		u8 edid_ready;
        u8 prev_hpd_state;

        u8 try_fixed_EQ;
        u8 current_hdmi_mode;
        u8 current_txoe;
        u8 check_for_hpd_toggle;
        u8 sdi_stable_count;
        u8 check_for_sdi;
        u8 force_hpd_state;
        u8 txoe_alignment;
        u8 hpd_toggle_timeout;
        u8 spmon;
        u8 is_hdmi20_sink;
    }vars;

    struct
    {
        u8 ForceRxOn;
    }debug;

    struct
    {
        _SYS_AEQ_TYPE sys_aEQ;
        u8 AutoEQ_state;
        u8 AutoEQ_WaitTime;
        u8 AutoEQ_Result;
        u8 DFE_Valid;
        u8 RS_Valid;
        u16 RS_ValidMap[3];
        u8 EqHDMIMode;
        u8 ManuEQ_state;
        u8 DFE[14][3][3]; // [RS_value][channel012][NumABC]  -> 0x34B...0x353
        u8 CalcRS[3];

        u8 EQ_flag_14;
        u8 EQ_flag_20;
        u8 txoe_ready14;
        u8 txoe_ready20;
        u8 stored_RS_14[3];
        u8 stored_RS_20[3];
        u8 current_eq_mode;

        u8 FixedRsIndex[4];

        u8 meq_cur_idx;

		u8 meq_adj_idx[3];
        u32 ced_err_avg[3];
        u32 ced_err_avg_prev[3];
        u8  ced_acc_count;
        u8  manu_eq_fine_tune_count[3];
        u8  manu_eq_fine_tune_best_rs[3];

    }EQ;



}IT6635_DEVICE_DATA;

extern IT6635_DEVICE_DATA gdev;
extern const u8 RS_Value[];



#ifdef __cplusplus
extern "C" {
#endif

//--------------------------------
u8 h2swwr( u8 offset, u8 wdata );
u8 h2swrd( u8 offset );
u8 h2swset( u8 offset, u8 mask, u8 wdata );
void h2swbrd( u8 offset, u8 length, u8 *rddata );
void h2swbwr( u8 offset, u8 length, u8 *rddata );

u8 h2rxwr(u8 offset, u8 wdata );
u8 h2rxrd( u8 offset );
u8 h2rxset( u8 offset, u8 mask, u8 dwata );
void h2rxbrd( u8 offset, u8 length, u8 *rddata );
void h2rxbwr( u8 offset, u8 length, u8 *rddata );

u8 cecwr( u8 offset, u8 wdata );
u8 cecrd( u8 offset );
u8 cecset( u8 offset, u8 mask, u8 wdata );
void cecbrd(u8 offset, u8 length, u8 *rddata);
void cecbwr(u8 offset, u8 length, u8 *rddata);

u8 h2rxedidwr( u8 offset, u8 *wrdata, u8 length );

void chgrxbank( u8 bankno );
void chgswbank( u8 bankno );

void _rx_update_ced_err_from_hw(void);
void _rx_get_ced_err(void);
void _rx_clear_ced_err(void);
u8 _rx_monitor_ced_err(void);
void _rx_DFE_enable( u8 enable );
void _rx_set_rs_3ch( u8 *rs_value );
void _rx_set_rs( u8 ch, u8 rs_value );

u8 _rx_is_all_ch_symlock(void);
u8 _rx_is_ch_symlock(u8 ch);
u8 _rx_is_clock_stable(void);

void _rx_ovwr_hdmi_clk(u8 port, u8 ratio);
void _rx_ovwr_h20_scrb(u8 port, u8 scrb);

void _rx_auto_power_down_enable(u8 port, u8 enable);
void _rx_term_power_down(u8 port, u8 channel);
void _rx_handle_output_err(void);

void _sw_enable_timer0(void);
void _sw_disable_timer0(void);
u8 _sw_get_timer0_interrupt(void);

u8 _sw_is_hdcp_enabled(void);
void _sw_clear_hdcp_status(void);
u8 _sw_is_hdcp_auth_done(void);

//--------------------------------
void it6635_txoe(u8 enable);
void it6635_auto_detect_hdmi_encoding(void);
void it6635_fix_incorrect_hdmi_encoding(void);

u8 it6635_get_port_info1( u8 port, u8 info );
u8 it6635_get_port_info0(u8 port, u8 info);

void it6635_init_rclk(void);
void it6635_enable_tx_port(u8 enable);

// --------------
void it6635_sys_state(u8 new_state);
void _rx_reset( void );
void _rx_caof_init(u8 port);



void _eq_save_h20(void);
void _eq_load_h20(void);
void _eq_save_h14(void);
void _eq_load_h14(void);
void _eq_load_previous(void);
void _eq_load_default(void);


void _eq_reset_state(void);
void _eq_set_state(u8 state);
u8 _eq_get_state(void);
void _eq_reset_txoe_ready(void);
void _eq_set_txoe_ready(u8 ready);
u8 _eq_get_txoe_ready(void);

void _aeq_set_DFE2(u8 EQ0, u8 EQ1, u8 EQ2);

u8 _rx_is_hdmi20(void);

void _aeq_diable_eq_trigger(void);
u8 _aeq_check_sareq_result(void);
void _aeq_chg( u8 new_state );
u8 _aeq_fsm(void);


#if DEBUG_FSM_CHANGE
#define it6635_fsm_chg( new_state )  __it6635_fsm_chg( new_state, __LINE__ )
#define it6635_fsm_chg_delayed( new_state )  __it6635_fsm_chg2( new_state, __LINE__ )
#else
void it6635_fsm_chg( u8 new_state );
void it6635_fsm_chg_delayed( u8 new_state );
#endif

void __it6635_fsm_chg(u8 new_state, int caller);
void __it6635_fsm_chg2(u8 new_state, int caller);
void it6635_vars_init(void);
bool it6635_device_init(void);
bool it6635_device_init2(void);

bool it6635_read_edid(u8 block, u8 offset, int length, u8 *edid_buffer);
bool it6635_write_one_block_edid(u8 block, u8 *edid_buffer);
bool it6635_parse_edid(u8 edid_source);

void it6635_force_hdmi20(void);
#ifdef __cplusplus
}
#endif

void _rx_skew_adj(u8 ch);
#define _rx_edid_address_enable(port) { h2swset(0x55 + port, 0x24, 0x00); }
#define _rx_edid_address_disable(port) { h2swset(0x55 + port, 0x24, 0x04); }
#define _rx_edid_ram_enable(port) { h2swset(0x55 + port, 0x01, 0x00); }
#define _rx_edid_ram_disable(port) { h2swset(0x55 + port, 0x01, 0x01); }
#define _rx_edid_set_chksum(port, sum) { h2swwr(0xe1 + port * 2, sum);  }
#define _rx_edid_set_cec_phyaddr(port, phyAB, phyCD) { h2swwr(0xd9 + port*2, phyAB); h2swwr(0xda + port*2, phyCD);  }


#endif
