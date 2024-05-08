/*
* SiI8630/SiI8632/SiI9630 Linux Driver
*
* 2016 © Lattice Semiconductor Corporation
*
* This program is free software; you can redistribute it and/or
* modify it only under the terms of  version 2 of the GNU General Public License
* as published by the Free Software Foundation, and not any later version.
*
* This program is distributed “AS IS” WITHOUT ANY WARRANTY of any kind,
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

#include "Sii9630_si_datatypes.h"

#if (_ENABLE_CHIP_SIL9630 == _MACRO_ENABLE)

#include "Sii9630_si_platform.h"
#include "Sii9630_si_lib_log.h"
#include "Sii9630_si_timer.h"

static volatile u32 g_siiTimerCounters[eTimerSil_INVALID];
static vu32         sg_uv32SystemTimems = 0;


/* Convert a value specified in milliseconds to nanoseconds */
#define MSEC_TO_NSEC(x)	(x * 1000000UL)
#define MILLI_TO_MAX	(((uint32_t)~0)>>1)	/* maximum milli */

#if SIL9630_USE_TIMER_PROCESS
struct sii_timer_obj {
    uint8_t u8TimerNum;
	uint8_t wake_flag;
	uint8_t stop_flag;
	uint8_t u8work_flag;
	uint32_t timer_cnt;
	void *callback_param;
	void (*timer_callback_handler)(void *callback_param);
	/*unsigned long periodicity;*/
};

static struct sii_timer_obj sg_stnew_timer[eTimerSil_INVALID];
static void s_timer_handler(struct sii_timer_obj *sii_timer);
#endif

int s_timer_thread(void)
{
    #if SIL9630_USE_TIMER_PROCESS
	struct sii_timer_obj *sii_timer = &sg_stnew_timer[0];
    struct sii_timer_obj *sii_start_timer = sii_timer;

	if (!sii_timer) {
		sii_log_err("Invalid Timer Instance\n");
		return 1;
	}

	do {
        s_timer_handler(sii_timer);
        if(sii_timer->stop_flag||(!sii_timer->wake_flag))
        {
            sii_timer++;
            if((sii_timer-sii_start_timer)<eTimerSil_INVALID)
                continue;
            else
                break;
        }
		sii_timer->wake_flag = 0;
		sii_timer->u8work_flag = 0;

		/*handle timer stop condition*/
		if (!sii_timer->stop_flag &&
			sii_timer->timer_callback_handler) {
			sii_timer->timer_callback_handler(
					sii_timer->callback_param);
		}

        sii_timer++;
        if((sii_timer-sii_start_timer)<eTimerSil_INVALID)
            continue;
        else
            break;
	} while (true);
    #endif
	return 0;
}

#if SIL9630_USE_TIMER_PROCESS
static void s_timer_handler(struct sii_timer_obj *sii_timer)
{
	//struct sii_timer_obj *sii_timer = (struct sii_timer_obj *)data_ptr;

	if (!sii_timer) {
		sii_log_err("Invalid Timer Instance\n");
		return;
	}
    if(sii_timer->u8work_flag)
    {
        if(sil_PlatformTimerExpired(sii_timer->u8TimerNum))
        {
            sii_timer->wake_flag = 1;
        }
    }
}
#endif

int sii_timer_start_impl(void **timer_handle, uint32_t expires,
	const char *fn_name)
{
    #if SIL9630_USE_TIMER_PROCESS
	struct sii_timer_obj *sii_timer = (struct sii_timer_obj *)*timer_handle;

	if (!sii_timer) {
		sii_log_err("Invalid Timer Instance. called from %s\n",
			fn_name);
		return 1;
	}

	sii_timer->stop_flag = 0;
    sii_timer->u8work_flag = 1;
    sil_PlatformTimerSet(sii_timer->u8TimerNum,expires);

    #else
    timer_handle = timer_handle;
    expires = expires;
    //fn_name = fn_name;
    #endif
	return 0;
}

int sii_timer_stop(void **timer_handle)
{
    #if SIL9630_USE_TIMER_PROCESS
	struct sii_timer_obj *sii_timer = (struct sii_timer_obj *)*timer_handle;

	if (!sii_timer) {
		sii_log_err("Invalid Timer Instance\n");
		return 1;
	}
	sii_timer->stop_flag = 1;
	/*sii_timer->periodicity = 0;*/
	sii_timer->wake_flag = 0;

    #else
    timer_handle = timer_handle;
    #endif
	return 0;
}

int sii_timer_create(void (*callback_handler)(void *callback_param),
			void *callback_param, void **timer_handle,uint8_t u8TimerNum)
{
    #if SIL9630_USE_TIMER_PROCESS
	struct sii_timer_obj *new_timer = &sg_stnew_timer[u8TimerNum];

	//new_timer = kzalloc(sizeof(*new_timer), GFP_KERNEL);
    memset(&sg_stnew_timer[u8TimerNum],0,sizeof(struct sii_timer_obj));

	if (new_timer == NULL) {
		/*pr_err("Invalid Timer Instance\n");*/
		return 1;
	}
	new_timer->timer_cnt = 0;
	new_timer->timer_callback_handler = callback_handler;
	new_timer->callback_param = callback_param;
	new_timer->wake_flag = 0;
	new_timer->u8work_flag = 0;
	new_timer->stop_flag = 1;
    new_timer->u8TimerNum = u8TimerNum;
	*timer_handle = new_timer;
    #else
    timer_handle = timer_handle;
    callback_param = callback_param;
    timer_handle = timer_handle;
    #endif
	return 0;
}

int sii_timer_delete(void **timer_handle)
{
    #if SIL9630_USE_TIMER_PROCESS
	struct sii_timer_obj *sii_timer = (struct sii_timer_obj *)*timer_handle;

	if (!sii_timer)
		return 1;

	sii_timer->wake_flag = 1;
    #else
    timer_handle = timer_handle;
    #endif
	//kfree(sii_timer);
	return 0;
}
#if 0//SIL9630_USE_TIMER_PROCESS
uint32_t sii_time_milli_get(void)
{
	return (uint32_t)(jiffies * 1000 / HZ);
}

static void s_time_out_milli_set(uint32_t *p_milli_t_o, uint32_t time_out)
{
	/*SII_PLATFORM_DEBUG_ASSERT(MILLI_TO_MAX > time_out);*/
	*p_milli_t_o = sii_time_milli_get() + time_out;
}

void sii_time_out_milli_set(uint32_t *p_milli_t_o, uint32_t time_out)
{
	s_time_out_milli_set(p_milli_t_o, time_out);
}

static bool s_time_out_milli_is(const uint32_t *p_milli_t_o)
{
	uint32_t milli_new = sii_time_milli_get();
	uint32_t milli_dif =
		(*p_milli_t_o > milli_new) ? (*p_milli_t_o - milli_new) :
						(milli_new - *p_milli_t_o);
	bool ret_val = false;

	if (MILLI_TO_MAX < milli_dif) {
		ret_val = (*p_milli_t_o > milli_new) ? (true) : (false);
		;
	} else {
		ret_val = (*p_milli_t_o <= milli_new) ? (true) : (false);
	}
	return ret_val;
}

bool sii_time_out_milli_is(const uint32_t *p_milli_t_o)
{
	return s_time_out_milli_is(p_milli_t_o);
}

static bool s_time_out_is_expired(const uint32_t *p_milli_t_o,
					uint32_t timeout)
{
	uint32_t milli_new = sii_time_milli_get();
	uint32_t milli_dif =
		(*p_milli_t_o > milli_new) ? (*p_milli_t_o - milli_new) :
						(milli_new - *p_milli_t_o);

	/* todo. check if it is working fine.*/
	/*if (MILLI_TO_MAX < milli_dif)*/
	return (milli_dif < timeout) ? (false) : (true);
}
bool sii_time_out_is_expired(const uint32_t *p_milli_t_o, uint32_t timeout)
{
	return s_time_out_is_expired(p_milli_t_o, timeout);
}
#endif

//========================================================================================
// add by wf8421 20180426
//------------------------------------------------------------------------------
// Function:    PlatformTimerSet
// Description: ÉèÖÃÏà¹Ø¹¦ÄÜ¶¨Ê±Æ÷Ê±¼ä
//------------------------------------------------------------------------------
void sil_PlatformTimerSet( uint8_t index, u32 m_sec )
{
    switch ( index )
    {
        default:
            if (index<eTimerSil_INVALID)
            {
                g_siiTimerCounters[index] = m_sec;
            }
            break;
    }
}

//------------------------------------------------------------------------------
// Function:    sii_PlatformTimerSet
// Description: »ñÈ¡Ïà¹Ø¹¦ÄÜ¶¨Ê±Æ÷Ê±¼ä
//------------------------------------------------------------------------------
u32 sil_PlatformTimerGet(uint8_t eTimer)
{
    return(g_siiTimerCounters[eTimer]);
}

//------------------------------------------------------------------------------
// Function:    sii_PlatformTimerSet
// Description: ¶¨Ê±Æ÷³õÊ¼»¯
//------------------------------------------------------------------------------
void sil_PlatformTimerInit (void)
{
    int i;
    //initializer timer counters in array
    for ( i = 0; i < eTimerSil_INVALID; i++ )
    {
        g_siiTimerCounters[ i ] = 0;
    }

    //PlatformTimerSet(etKEY_Detect,5000);
}

//------------------------------------------------------------------------------
// Function:    sii_PlatformTimerSet
// Description: ¶¨Ê±Æ÷µÝ¼õº¯Êý
//------------------------------------------------------------------------------
void sil_PlatformTimerTask(void)
{
    int i;
    for(i=0;i<eTimerSil_INVALID;i++)
    {
        if(g_siiTimerCounters[i] > 0)
        {
            g_siiTimerCounters[i]--;
        }
    }
    sg_uv32SystemTimems++;
}

//------------------------------------------------------------------------------
// Function:    sii_PlatformTimerSet
// Description: Ïà¹Ø¹¦ÄÜ¶¨Ê±Æ÷Ê±¼äÊÇ·ñÎª0
//------------------------------------------------------------------------------
uint8_t sil_PlatformTimerExpired ( uint8_t timer )		   //¶¨Ê±Ê±¼äµ½º¯Êý
{
    if ( timer < eTimerSil_INVALID )
    {
        return( g_siiTimerCounters[ timer ] == 0);			   //¶¨Ê±Ê±¼äµ½Ê±·µ»Ø1
    }

    return( 0 );										   //¶¨Ê±Ê±¼ä»¹Ã»µ½Ê±·µ»Ø0
}

u32 sil_GetSystemTime(void)
{
    return sg_uv32SystemTimems;
}
//========================================================================================

#endif  //#if (_ENABLE_CHIP_SIL9630 == _MACRO_ENABLE)

