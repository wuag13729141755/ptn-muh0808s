///*****************************************
//  Copyright (C) 2009-2020
//  ITE Tech. Inc. All Rights Reserved
//  Proprietary and Confidential
///*****************************************
//   @file   <it6635.h>
//   @author Kenneth.Hung@ite.com.tw
//   @date   2020/12/30
//   @fileversion: IT66353_SAMPLE_1.06
//******************************************/
#ifndef _IT6635_H_
#define _IT6635_H_

typedef struct
{
    u8 EnRxDDCBypass;
    u8 EnableAutoEQ;
}IT6635_DEVICE_OPTION;

#ifdef __cplusplus
extern "C" {
#endif
extern vvFuncPoint pvpmappIT6635XUserFunction;
extern u8 g_u8ITEDevice;

//------------------------------
// APIs:
void it6635_DeviceSelect(u8 u8DevID);
void it6635_SetHoldSystem(bool bSta);
bool it6635_GetHoldSystemSta(void);
void it6635_init(void);
void it6635_main(void);
char *it6635_get_lib_version(void);

void it6635_setup_edid_ram_phyaddr(u8 *edid, u8 block);
void it6635_set_internal_EDID( u8 block, u8 const *edid, u8 target_port );
void it6635_get_internal_EDID( u8 block, u8 *edid, u8 target_port );
void it6635_parse_edid_for_phyaddr(u8 *edid);
bool it6635_read_one_block_edid(u8 block, u8 *edid_buffer);

#define SW_HPD_LOW 0
#define SW_HPD_AUTO 1
void it6635_force_rx_hpd( u8 hpd_state );

void it6635_set_option( IT6635_DEVICE_OPTION *Opts );
void it6635_get_option( IT6635_DEVICE_OPTION *Opts );

u8 it6635_get_RS(void);
void it6635_set_RS(u8 rs_idx0, u8 rs_idx1, u8 rs_idx2 );
void it6635_set_ch_RS(u8 ch, u8 rs_index);

void it6635_dump_register_all(void);
void it6635_dump_opts(void);

u8 it6635_get_active_port( void );
bool it6635_set_active_port( u8 port );

void it6635_change_default_RS( u8 port, u8 new_rs_idx0, u8 new_rs_idx1, u8 new_rs_idx2, u8 update_hw );

void it6635_set_rx_hpd(u8 hpd_value);
void it6635_set_tx_5v(u8 output_value);
bool it6635_toggle_hpd(u16 ms_duration);

bool it6635_auto_eq_adjust(void);

void it6635_dev_restart(void);
void it6635_vars_init(void);
void it6635_options_init(void);

//------------------------------
// it6635 event handler:
void it6635_dev_loop(void);
u8 it6635_is_5v_present(u8 port);
u8 it6635_is_clock_detected(u8 port);

//------------------------------
// platform dependent functions: (needs implementation)
u8 it6635_i2c_write(u8 addr, u8 offset, u8 length, u8 *buffer);
u8 it6635_i2c_read(u8 addr, u8 offset, u8 length, u8 *buffer);
void it6635_read_tx_edid(u8 *pu8EdidBuf);

//------------------------------

#ifdef __cplusplus
}
#endif



#endif
