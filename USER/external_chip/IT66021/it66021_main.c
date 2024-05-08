///*****************************************
//  Copyright (C) 2009-2014
//  ITE Tech. Inc. All Rights Reserved
//  Proprietary and Confidential
///*****************************************
//   @file   <Main.c>
//   @author Max.Kao@ite.com.tw
//   @date   2014/09/11
//   @fileversion: ITE_MHLRX_SAMPLE_V1.01
//******************************************/
//#include "stm32f4xx.h"
#define __IT66021_MAIN_C__
#include "includes.h"
#include "it6602_config.h"

#include "it6602_mcu_conf.h"
#include "it6602_IO.h"
#include <stdio.h>
#include "it6602_version.h"
#include <string.h>

#ifdef Enable_IR
#include "it6602_Remote.h"
#endif

#include "it6602.h"
#include "it6602_reg.h"

#include "it66021_main.h"	//add by wangjie, 2017.8.2
#include "it6602_mcu.h"

static bool sg_bEnSrcChange[IT66021_MAX];
static u8   sg_u8It66021VidChgSta[IT66021_MAX];
static stIT66021EdidUpdate_t g_stIT66021EdidUpdateVar[IT66021_MAX];
static bool sg_bIt66021InHpdSta[IT66021_MAX];

IT66021_ID it66021_idx;
IT66021_I2C_ADDR it66021_i2c[IT66021_MAX] = {
	{
        #if IT66021_PIN_ADDR_LOW0
		IT6602A0_HDMI_ADDR,     // PCADR pin LOW
		#else
        IT6602B0_HDMI_ADDR,     // PCADR pin HIGH
        #endif
		EDID_ADDR,
		_I2C_TABLE_INDEX_IT660XX_0,
	},

	#if (IT66021_DEV_NUM >= 2)
	{
		#if IT66021_PIN_ADDR_LOW1
		IT6602A0_HDMI_ADDR,     // PCADR pin LOW
		#else
        IT6602B0_HDMI_ADDR,     // PCADR pin HIGH
        #endif
		EDID_ADDR,
		_I2C_TABLE_INDEX_IT660XX_1,
	},
    #endif

    #if (IT66021_DEV_NUM >= 3)
	{
		#if IT66021_PIN_ADDR_LOW2
		IT6602A0_HDMI_ADDR,     // PCADR pin LOW
		#else
        IT6602B0_HDMI_ADDR,     // PCADR pin HIGH
        #endif
		EDID_ADDR,
		_I2C_TABLE_INDEX_IT660XX_2,
	},
    #endif

    #if (IT66021_DEV_NUM >= 4)
	{
		#if IT66021_PIN_ADDR_LOW3
		IT6602A0_HDMI_ADDR,     // PCADR pin LOW
		#else
        IT6602B0_HDMI_ADDR,     // PCADR pin HIGH
        #endif
		EDID_ADDR,
		_I2C_TABLE_INDEX_IT660XX_3,
	},
    #endif

    #if (IT66021_DEV_NUM >= 5)
	{
		#if IT66021_PIN_ADDR_LOW4
		IT6602A0_HDMI_ADDR,     // PCADR pin LOW
		#else
        IT6602B0_HDMI_ADDR,     // PCADR pin HIGH
        #endif
		EDID_ADDR,
		_I2C_TABLE_INDEX_IT660XX_4,
	},
    #endif
    #if (IT66021_DEV_NUM >= 6)
    {
        #if IT66021_PIN_ADDR_LOW5
		IT6602A0_HDMI_ADDR,     // PCADR pin LOW
		#else
        IT6602B0_HDMI_ADDR,     // PCADR pin HIGH
        #endif
        EDID_ADDR,
        _I2C_TABLE_INDEX_IT660XX_5,
    },
    #endif
    #if (IT66021_DEV_NUM >= 7)
    {
        #if IT66021_PIN_ADDR_LOW6
		IT6602A0_HDMI_ADDR,     // PCADR pin LOW
		#else
        IT6602B0_HDMI_ADDR,     // PCADR pin HIGH
        #endif
        EDID_ADDR,
        _I2C_TABLE_INDEX_IT660XX_6,
    },
    #endif
    #if (IT66021_DEV_NUM >= 8)
    {
        #if IT66021_PIN_ADDR_LOW7
		IT6602A0_HDMI_ADDR,     // PCADR pin LOW
		#else
        IT6602B0_HDMI_ADDR,     // PCADR pin HIGH
        #endif
        EDID_ADDR,
        _I2C_TABLE_INDEX_IT660XX_7,
    },
    #endif

};

/////////////////////////////////////////////////////////////////////////////////////

void mapp_it66021_SetSrcChangeFlag(u8 u8Id,bool bEn)
{
    sg_bEnSrcChange[u8Id] = bEn;
}

bool mapp_it66021_GetSrcChangeFlag(u8 u8Id)
{
    return sg_bEnSrcChange[u8Id];
}


void mapp_it66021_SetVideoChangeSta(u8 u8Id,u8 u8Sta)
{
    sg_u8It66021VidChgSta[u8Id] = u8Sta;
}

u8 mapp_it66021_GetVideoChangeSta(u8 u8Id)
{
    return sg_u8It66021VidChgSta[u8Id];
}

void mapp_it66021_SetInputHpdSta(u8 u8Id,bool bSta)
{
    sg_bIt66021InHpdSta[u8Id] = bSta;
}

bool mapp_it66021_GetInputHpdSta(u8 u8Id)
{
    return sg_bIt66021InHpdSta[u8Id];
}



/////////////////////////////////////////////////////////////////////////////////////
//extern BYTE IIC_IT66021;
/* select one of IT66021*/
void it66021_id_sel(IT66021_ID id)
{
	if(id < IT66021_MAX)
	{
    	HdmiI2cAddr = it66021_i2c[id].hdmi;
    	EdidI2cAddr = it66021_i2c[id].edid;
        u8IT6602_BusID = it66021_i2c[id].m_u8busId;
	}
	else
	{
		HdmiI2cAddr = it66021_i2c[IT66021_1].hdmi;
		EdidI2cAddr = it66021_i2c[IT66021_1].edid;
        u8IT6602_BusID = it66021_i2c[IT66021_1].m_u8busId;
	}
}

/* init IT66021 */
void it66021_init(void)
{
	//IT66021_N1_RESET(0);
	//IT66021_N2_RESET(0);
	//It6602_delay1ms(10);
	//IT66021_N1_RESET(1);
	//IT66021_N2_RESET(1);
	for(it66021_idx = IT66021_1; it66021_idx < IT66021_MAX; it66021_idx++)
	{
        memset(&CurVTiming[it66021_idx],0,sizeof(It6602_VTiming));
		it66021_id_sel(it66021_idx);

		it6602HPDCtrl(0, 0);	// HDMI port 0, set HPD = 0
		It6602_delay1ms(100);	//for power sequence

		IT6602_fsm_init();
        mapp_it66021_SetSrcChangeFlag((u8)it66021_idx, FALSE);
        mapp_it66021_SetVideoChangeSta((u8)it66021_idx, (u8)emIt66021_VidChgNone);
	}
}

/* IT66021 Process periodically */
//unsigned char check_enable;
void it66021_process(void)
{
	for(it66021_idx = IT66021_1; it66021_idx < IT66021_MAX; it66021_idx++)
	{
		it66021_id_sel(it66021_idx);

		IT6602_fsm();
        it66021_InputTimingChangeProcess();
        it66021EdidManageProcess(it66021_idx);
	}
//	check_enable = 1;
}

/* Calculate accurate PCLK */
unsigned int calc_pclk(IT66021_ID idx)
{
	unsigned int pclk;
	unsigned char fps, tmp;

	if(idx >= IT66021_MAX)
		return 0;

	if(CurVTiming[idx].PCLK * CurVTiming[idx].HTotal * CurVTiming[idx].VTotal == 0)
		return 0;

	tmp = ((CurVTiming[idx].PCLK * 1000000) % (CurVTiming[idx].HTotal * CurVTiming[idx].VTotal) == 0)?0:1;
	fps = (CurVTiming[idx].PCLK * 1000000) / (CurVTiming[idx].HTotal * CurVTiming[idx].VTotal) + tmp;

	IT6602_MSG(("FPS: %d->", fps));


	if((fps >= 110) && (fps < 130))
		fps = 120;
	else if((fps >= 78) && (fps < 90))
		fps = 85;
	else if((fps >= 63) && (fps < 78))
		fps = 75;
	else if((fps >= 54) && (fps < 63))
		fps = 60;
	else if((fps >= 43) && (fps < 54))
		fps = 50;
	else if((fps >= 28) && (fps < 35))
		fps = 30;
	else if((fps >= 20) && (fps < 28))
		fps = 25;
	else if((fps >= 130) || (fps < 20))
		fps = 0;

	IT6602_MSG(("%d\n", fps));


	pclk = CurVTiming[idx].HTotal * CurVTiming[idx].VTotal * fps;

	return pclk;
}

unsigned char it66021_GetInputTiming(u8 u8Id, u16 *pu16HActive, u16 *pu16VActive, u32 *pu32Pclk)
{
    *pu16HActive = CurVTiming[u8Id].HActive;
    *pu16VActive = CurVTiming[u8Id].VActive;
    *pu32Pclk = calc_pclk((IT66021_ID)u8Id);
    return CurVTiming[u8Id].ScanMode;
}

void it66021_GetInputVFreq(u8 u8Id,u8 *u8VFreq)
{
	unsigned char fps, tmp = 0;

	if(u8Id >= IT66021_MAX)
		return;

	if(CurVTiming[u8Id].PCLK * CurVTiming[u8Id].HTotal * CurVTiming[u8Id].VTotal == 0)
		return;

	//tmp = ((CurVTiming[u8Id].PCLK * 1000000) % (CurVTiming[u8Id].HTotal * CurVTiming[u8Id].VTotal) == 0)?0:1;
	//fps = (CurVTiming[u8Id].PCLK * 1000000) / (CurVTiming[u8Id].HTotal * CurVTiming[u8Id].VTotal) + tmp;
	fps = calc_pclk((IT66021_ID)u8Id) / (CurVTiming[u8Id].HTotal * CurVTiming[u8Id].VTotal) + tmp;
    *u8VFreq = fps;
}

void it66021_InputTimingChangeProcess(void)
{
    static u16 sl_u16StartCnt = 300;
    struct it6602_dev_data *it6602data = get_it6602_dev_data();

    if(sl_u16StartCnt)
    {
        sl_u16StartCnt--;
        return;
    }

    if(mapp_it66021_GetSrcChangeFlag(it66021_idx))
    {
        mapp_it66021_SetSrcChangeFlag(it66021_idx,FALSE);
        IT6602_MSG(("\r\n==================================================\r\n"));
        IT6602_MSG(("IT66021[%d] m_VState == %d\r\n",it66021_idx,it6602data->m_VState));
        IT6602_MSG(("==================================================\r\n"));

        if(it6602data->m_VState == VSTATE_VideoOn)
        {
            mapp_it66021_SetVideoChangeSta((u8)it66021_idx, (u8)emIt66021_VidChgeVidOn);
            mapp_it66021_SetInputHpdSta((u8)it66021_idx,TRUE);
        }
        else
        {
            memset(&CurVTiming[it66021_idx],0,sizeof(It6602_VTiming));
            mapp_it66021_SetVideoChangeSta((u8)it66021_idx, (u8)emIt66021_VidChgeVidoff);
            mapp_it66021_SetInputHpdSta((u8)it66021_idx,FALSE);
        }
    }
}

/* Update IT66021 EDID RAM */
void it66021_update_edid(unsigned char id,unsigned char *u8EdidBuf)
{
    it66021_id_sel((IT66021_ID)id);

	it6602HPDCtrl(0, 0);	// HDMI port 0, set HPD = 0
    EDIDRAMInitial(u8EdidBuf);
    hdmirxset(REG_RX_0C0, 0x20, 0x20);  //EDID reset
    It6602_delay1ms(1);
    hdmirxset(REG_RX_0C0, 0x20, 0x00);
    It6602_delay1ms(300);
    IT6602_fsm();

    it6602HPDCtrl(0, 1);    // HDMI port 0, set HPD = 1
    It6602_delay1ms(500);
}

/* Check HDMI/DVI mode */
unsigned char it66021_hdmi_mode(IT66021_ID id)
{
    it66021_id_sel(id);
    return IsHDMIMode();
}

void it66021EdidManageProcess(u8 u8ID)
{
    if(g_stIT66021EdidUpdateVar[u8ID].u8EdidChgFlag)
    {
        g_stIT66021EdidUpdateVar[u8ID].u8EdidChgFlag = 0;
//        mapp_EdidLimitTo1080p60(g_stIT66021EdidUpdateVar[u8ID].u8Edid,g_stIT66021EdidUpdateVar[u8ID].u8Edid,256);
        it66021_update_edid(u8ID,g_stIT66021EdidUpdateVar[u8ID].u8Edid);
    }
}

void it66021EdidChange(u8 u8ID,u8 *pu8EdidBuf)
{
    g_stIT66021EdidUpdateVar[u8ID].u8EdidChgFlag = 1;
    memcpy(g_stIT66021EdidUpdateVar[u8ID].u8Edid,pu8EdidBuf,256);
}



