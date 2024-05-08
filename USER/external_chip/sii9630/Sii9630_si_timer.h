/*
* SiI8630/SiI8632/SiI9630 Linux Driver
*
* 2016 © Lattice Semiconductor Corporation
*
* This program is free software; you can redistribute it and/or
* modify it only under the terms of  version 2 of the GNU General Public License
* as published by the Free Software Foundation, and not any later version.
*
* This program is distributed "AS IS" WITHOUT ANY WARRANTY of any kind,
* whether express or implied, including without limitation the implied warranty
* of MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE or NON-INFRINGEMENT.
* See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software Foundation, Inc.,
* 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*
* You may contact Lattice Semiconductor Corporation at 111 SW 5th Ave,
* Suite 700, Portland, OR 97204
*/

#ifndef _SI_TIMER_API_H_
#define _SI_TIMER_API_H_

#include"Sii9630_si_drv_cp8630.h"

int s_timer_thread(void);
int sii_timer_stop(void **timer_handle);
int sii_timer_create(void (*callback_handler)(void *callback_param),
		void *callback_param, void **timer_handle,uint8_t u8TimerNum);
int sii_timer_delete(void **timer_handle);

int sii_timer_start_impl(void **timer_handle, uint32_t expires,
	const char *function_name);
#define sii_timer_start(timer_handle, expires)	\
	sii_timer_start_impl(timer_handle, expires, __func__);

typedef enum
{
    eTimerSil_temp_tdms_handler,
    eTimerSil_hdcp_auth_deglitch,
    eTimerSil_tx_tmds,
    #ifdef CONFIG_HANDLE_BOARD_RESET
    eTimerSil_chip_reset,
    #endif
    eTimerSil_rx_hdmi_hpd_timer,
    eTimerSil_rx_hdmi_video_state,
    eTimerSil_rx_hdmi_stable_video_timing,
    eTimerSil_rx_hdmi_clk_sync_timeout,

    #ifdef CONFIG_HDCP_REPEATER_EN
    eTimerSil_tx_hdcp_tx2x_rpt_rcv_id_timeout,
    eTimerSil_tx_hdcp_tx2x_repeater_re_auth,
    #endif
    eTimerSil_tx_hdcp_hdcp_ddc_ack_poll,
    eTimerSil_tx_hdcp_hdcp_restart_1xauth,
    #ifdef CONFIG_POWER_DOWN_EN
    eTimerSil_tx_hdcp_hdcp2x_pwr_up_monitor,
    #endif
    eTimerSil_tx_scdc_scramble_timeout,
    #ifdef CONFIG_ENHANCED_MODE_PROT
    eTimerSil_tx_scdc_scdc_rcv_status,
    #endif
    eTimerSil_INVALID,  // not remove

}emSiiTIMER;
void sil_PlatformTimerSet( uint8_t index, u32 m_sec );
u32 sil_PlatformTimerGet(uint8_t eTimer);
void sil_PlatformTimerInit (void);
void sil_PlatformTimerTask(void);
uint8_t sil_PlatformTimerExpired ( uint8_t timer );
u32 sil_GetSystemTime(void);

#endif /* _SI_TIMER_API_H_ */

