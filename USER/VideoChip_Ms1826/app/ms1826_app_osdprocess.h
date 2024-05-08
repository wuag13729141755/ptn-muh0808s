#ifndef __MS1826_APP_OSDPROCESS_H
#define __MS1826_APP_OSDPROCESS_H
#include "ms1826_app.h"


VOID app_osd_init(UINT8 u8dev);
VOID app_osd_trig(UINT8 u8dev,UINT8 u8_module_osd_idx,UINT8 u8_module_tx_idx);
VOID app_osd_trigger(UINT8 u8dev,UINT8 u8_osd_idx);
VOID set_osd_res(UINT8 u8dev,UINT8 u8_rx_id);
VOID sys_show_osd(UINT8 u8dev,BOOL b_show, UINT8 u8_index);
VOID osd_service(UINT8 u8dev);

#endif
