/*
  2016 (C) Lattice Semiconductor Corporation

  No part of this work may be reproduced, modified, distributed, transmitted,
  transcribed, or translated into any language or computer format, in any form
  or by any means without written permission of Lattice Semiconductor, Inc.,
  111 SW 5th Ave, Suite 700, Portland, OR 97204.

*/
#include "Sii9396_si_datatypes.h"
#include "Sii9396_osal.h"
#include "Sii9396_hal.h"
#include "Sii9396_sii_api.h"
//#define CHIP_ID			0x9396
//#define HOST_APP_NAME	"SiI9396 Rx"
//#define HOST_SW_VER		"00.90.00"
//#define RX_FW_VER		"00.90.00"

static BYTE AppIsTxHdcpRequired[SII9396_DevNum]={0};

const char *copyright_msg = "2016 (C) Lattice Semiconductor Corporation";
const char *build_time = "Build Time: " __DATE__ "-" __TIME__ "";

extern uint8_t g_Init_EdidFlashEdidTable[];         //default internal edid
extern BYTE InPortLinkType[MAX_IN];
extern BYTE OutputSwitchFlag[MAX_OUT];
BYTE OutputEdidData[MAX_OUT][256]={0};

BYTE FirstPwonFlag=0;
BYTE CurTxDsEdidStatus[MAX_OUT]={0};
BYTE OldTxDsEdidStatus[MAX_OUT]={0};
extern WORD OutputSwitchTimeCount[MAX_OUT];

enum sii_user_message {
	SII_USER_MSG__NULL,
	SII_USER_MSG__QUIT,
	SII_USER_MSG__UPDATE,
	SII_USER_MSG__RESET,
};

static enum sii_user_message command_input(void);
static void file_input(char *bin_file_path);

static void sii_event_handler(sii_inst_t inst, uint8_t event)
{
	info("sii_event_handler_9396######################= %d\n",inst+1);
	#if UART1_DEBUG
	sprintf(USART1_tx_buff, "sii_event_handler_9396#############= %d   event value= %d\r\n", inst+1,event);
	UART1_SendBytes((uint8_t *)USART1_tx_buff, strlen(USART1_tx_buff));
	#endif
	if (event == SII_EVENT__BOOT_DONE) {
		bool host_mode;
		info("enter SII_EVENT__BOOT_DONE..........\n");
		info("EVT: Boot Done\n");
		#if UART1_DEBUG
	    sprintf(USART1_tx_buff, "enter SII_EVENT__BOOT_DONE..........\r\n");
	    UART1_SendBytes((uint8_t *)USART1_tx_buff, strlen(USART1_tx_buff));
	    #endif
		SiiHostModeGet(inst, &host_mode);
		if (host_mode) {
			info("FW Run in Host Mode.\n");
			SiiHostModeSet(inst, false);
		} else
			info("FW Run in Stand-alone Mode.\n");
	}
	#if 1
	else if (event == SII_EVENT__RX_AV_LINK_CHNG) {
		enum sii_av_link_type type;
		info("enter SII_EVENT__RX_AV_LINK_CHNG..........\n");
		info("EVT: Rx Av Link Change\n");
		SiiRxAvLinkGet(inst, &type);
		switch (type) {
			case SII_AV_LINK__NONE:
				info("Rx Link Type is None\n");
				#if UART1_DEBUG
				sprintf(USART1_tx_buff, "Rx Link Type is None\r\n");
	            UART1_SendBytes((uint8_t *)USART1_tx_buff, strlen(USART1_tx_buff));
	            #endif
	            if(inst<MAX_IN)
	            {
	                InPortLinkType[inst] = 0;
	            }
				break;
			case SII_AV_LINK__HDMI1:
				info("Rx Link Type is HDMI 1.4\n");
				#if UART1_DEBUG
				sprintf(USART1_tx_buff, "Rx Link Type is HDMI 1.4\r\n");
	            UART1_SendBytes((uint8_t *)USART1_tx_buff, strlen(USART1_tx_buff));
	            #endif
	            if(inst<MAX_IN)
	            {
	                InPortLinkType[inst] = 1;
	            }
				break;
			case SII_AV_LINK__HDMI2:
				info("Rx Link Type is HDMI 2.0\n");
				#if UART1_DEBUG
				sprintf(USART1_tx_buff, "Rx Link Type is HDMI 2.0\r\n");
	            UART1_SendBytes((uint8_t *)USART1_tx_buff, strlen(USART1_tx_buff));
	            #endif
	            if(inst<MAX_IN)
	            {
	                InPortLinkType[inst] = 2;
	            }
				break;
			case SII_AV_LINK__DVI:
				info("Rx Link Type is DVI\n");
				#if UART1_DEBUG
				sprintf(USART1_tx_buff, "Rx Link Type is DVI\r\n");
	            UART1_SendBytes((uint8_t *)USART1_tx_buff, strlen(USART1_tx_buff));
	            #endif
	            if(inst<MAX_IN)
	            {
	                InPortLinkType[inst] = 3;
	            }
				break;
			default:
			    #if UART1_DEBUG
			    sprintf(USART1_tx_buff, "Rx Link Type is Other\r\n");
	            UART1_SendBytes((uint8_t *)USART1_tx_buff, strlen(USART1_tx_buff));
	            #endif
				break;
		}
	}
	else if (event == SII_EVENT__AV_MUTE_CHNG) {
		bool av_mute;
		info("enter SII_EVENT__AV_MUTE_CHNG..........\n");
		info("EVT: Av Mute Change\n");
		#if UART1_DEBUG
	    sprintf(USART1_tx_buff, "enter SII_EVENT__AV_MUTE_CHNG..........\r\n");
	    UART1_SendBytes((uint8_t *)USART1_tx_buff, strlen(USART1_tx_buff));
	    #endif

		#if 0
		SiiRxAvMuteGet(inst, &av_mute);
		if (av_mute) {
			info("AV Mute ON\n");
		} else {
			info("AV Mute OFF\n");
		}
		#endif
	}
	else if (event == SII_EVENT__SCDT_CHNG) {
		info("EVT: SCDT Change\n");
		info("enter SII_EVENT__SCDT_CHNG..........\n");
		#if UART1_DEBUG
		sprintf(USART1_tx_buff, "EVT: SCDT Change\r\n");
	    UART1_SendBytes((uint8_t *)USART1_tx_buff, strlen(USART1_tx_buff));
	    #endif
		/* Add user callback for SCDT_CHNG event*/
	}
	else if (event == SII_EVENT__DS_CONNECT_STATUS_CHNG) {
		bool tx_hpd;
		info("enter SII_EVENT__DS_CONNECT_STATUS_CHNG..........\n");
		info("EVT: DS Connection Change\n");

        #if UART1_DEBUG
	    sprintf(USART1_tx_buff, "enter SII_EVENT__DS_CONNECT_STATUS_CHNG..........\r\n");
	    UART1_SendBytes((uint8_t *)USART1_tx_buff, strlen(USART1_tx_buff));
        #endif
		SiiTxHpdStatusGet(inst, &tx_hpd);
		if (tx_hpd)
		{
			info("DS HPD HIGH\n");
		}
		else
		{
			info("DS HPD LOW\n");
			#if 0
			if(FirstPwonFlag==1)
			{
                		if((inst>=MAX_OUT)&&(inst<SII9396_DevNum))
                		{
					if(OutputSwitchFlag[inst-MAX_OUT]==0)
						memset(OutputEdidData[inst-MAX_OUT],0,  256);
				}
			}
			#endif
		}
	}
	else if (event == SII_EVENT__DS_EDID_STATUS_CHNG) {
		bool status;
		info("enter SII_EVENT__DS_EDID_STATUS_CHNG..........\n");
		info("EVT: Edid Status Change\n");

        #if UART1_DEBUG
	    sprintf(USART1_tx_buff, "enter SII_EVENT__DS_EDID_STATUS_CHNG..........\r\n");
	    UART1_SendBytes((uint8_t *)USART1_tx_buff, strlen(USART1_tx_buff));
        #endif
		SiiTxDsEdidStatusGet(inst ,&status);
		if(FirstPwonFlag==1)
		{
			if((inst>=MAX_OUT)&&(inst<SII9396_DevNum))
			{
				if(OutputSwitchTimeCount[inst-MAX_OUT]==0)
					CurTxDsEdidStatus[inst-MAX_OUT] = status;
			}
		}
		#if 0
		if (status)
		{
			info("DS EDID Available\n");
			if(FirstPwonFlag==1)
			{
                		if((inst>=MAX_OUT)&&(inst<SII9396_DevNum))
		              {
		                    if ((OutputEdidData[inst-MAX_OUT][0] == 0) && (OutputEdidData[inst-MAX_OUT][1] == 0xff) && (OutputEdidData[inst-MAX_OUT][3] == 0xff)&& (OutputEdidData[inst-MAX_OUT][7] == 0))
				      {
		                    }
		                    else
		                    {
		                        	SiiTxEdidGetEdidData(inst,OutputEdidData[inst-MAX_OUT]);
		                    }
		              }
			}
		}
		else
		{
			info("DS EDID Unavailable\n");
		}
		#endif
#if 0
		SiiRxHpdSet(inst, true);
		info("Set Rx HPD High\n");
#endif
	}
	else if (event == SII_EVENT__RX_HDCP_STATUS_CHNG) {
		enum sii_hdcp_version hdcp_version;
		enum sii_hdcp_status hdcp_status;
		info("enter SII_EVENT__RX_HDCP_STATUS_CHNG..........\n");
		info("EVT: RX HDCP Status Change\n");

        #if UART1_DEBUG
	    sprintf(USART1_tx_buff, "enter SII_EVENT__RX_HDCP_STATUS_CHNG..........\r\n");
	    UART1_SendBytes((uint8_t *)USART1_tx_buff, strlen(USART1_tx_buff));
        #endif
		#if 0
		SiiRxHdcpUsVerGet(inst, &hdcp_version);
		switch(hdcp_version) {
			case SII_HDCP_VERSION__NONE:
				info("RX HDCP Version is None\n");
				break;
			case SII_HDCP_VERSION__1X:
				info("RX HDCP Version is 1.4\n");
				break;
			case SII_HDCP_VERSION__20:
				info("RX HDCP Version is 2.0\n");
				break;
			case SII_HDCP_VERSION__22:
				info("RX HDCP Version is 2.2\n");
				break;
			default:
				break;
		}
		SiiRxHdcpStatusGet(inst, &hdcp_status);
		switch(hdcp_status) {
			case SII_HDCP_STATUS__OFF:
				info("RX HDCP is Off\n");
				break;
			case SII_HDCP_STATUS__SUCCESS_1X:
				info("RX HDCP 1.x is Authenticated\n");
				break;
			case SII_HDCP_STATUS__SUCCESS_22:
				info("RX HDCP 2.2 is Authenticated\n");
				break;
			case SII_HDCP_STATUS__AUTHENTICATING:
				info("RX HDCP is Starting\n");
				break;
			case SII_HDCP_STATUS__FAILED:
				info("RX HDCP is Failed\n");
				break;
			default:
				break;
		}
		#endif
	}
	else if (event == SII_EVENT__HDR_REC) {
		uint8_t infoframe[SII_RX_INFOFRAME_MAX_LENGTH];
		uint8_t length;
		uint8_t i;
		info("enter SII_EVENT__HDR_REC..........\n");
		info("EVT: HDR Infoframe Received\n");

        #if UART1_DEBUG
	    sprintf(USART1_tx_buff, "enter SII_EVENT__HDR_REC..........\r\n");
	    UART1_SendBytes((uint8_t *)USART1_tx_buff, strlen(USART1_tx_buff));
        #endif
		#if 0
		SiiRxInfoFrameHdrGet(inst, infoframe, &length);
		for (i = 0; i < length; i++) {
			info("%02x ", infoframe[i]);
		}
		info("\n");
		#endif
	}
	else if (event == SII_EVENT__VSIF_REC) {
		uint8_t infoframe[SII_RX_INFOFRAME_MAX_LENGTH];
		uint8_t length;
		uint8_t i;
		info("enter SII_EVENT__VSIF_REC..........\n");
		info("EVT: VSIF Infoframe Received\n");

        #if UART1_DEBUG
	    sprintf(USART1_tx_buff, "enter SII_EVENT__VSIF_REC..........\r\n");
	    UART1_SendBytes((uint8_t *)USART1_tx_buff, strlen(USART1_tx_buff));
        #endif
		#if 0
		SiiRxInfoFrameVsifGet(inst, infoframe, &length);
		for (i = 0; i < length; i++) {
			info("%02x ", infoframe[i]);
		}
		info("\n");
		#endif
	}
	else if (event == SII_EVENT__RX_VIDEO_RESOLUTION_CHNG) {
		struct sii_video_info video_info;
		uint8_t frame_rate = 0;
		info("enter SII_EVENT__RX_VIDEO_RESOLUTION_CHNG..........\n");
		info("EVT: Video Resolution Changed\n");
		#if 0
		SiiRxVideoInfoGet(inst, &video_info);
		if (video_info.v_res_total != 0 && video_info.h_res_total != 0)
		{
			frame_rate =
				(video_info.tmds_clock * 1000000 / video_info.color_depth * 8) /
				(video_info.h_res_total * video_info.v_res_total);
			frame_rate = (uint8_t)(frame_rate / 5.0 + 0.5f) * 5;
			if (video_info.color_space == SII_COLOR_SPACE__YCBCR420)
				frame_rate *= 2;
		}
		info("\tTotal Resolution: %d x % d", video_info.h_res_total,video_info.v_res_total);
		if (video_info.interlace)
			info(" I\n");
		else
			info(" P\n");

		info("\tActive Resolution: %d x % d", video_info.h_res_active,video_info.v_res_active);

		if (video_info.interlace)
			info(" I\t");
		else
			info(" P\t");

		info("%dHz\t", frame_rate);

		info("%d bits\t", video_info.color_depth);

		switch (video_info.color_space) {
			case SII_COLOR_SPACE__RGB:
				info("RGB\n");
				break;
			case SII_COLOR_SPACE__YCBCR444:
				info("444\n");
				break;
			case SII_COLOR_SPACE__YCBCR422:
				info("422\n");
				break;
			case SII_COLOR_SPACE__YCBCR420:
				info("420\n");
				break;
			case SII_COLOR_SPACE__UNKNOWN:
			default:
				info("Color Space Unknown\n");
				break;
		}
		info("\tTMDS Clock: %d MHz\t", video_info.tmds_clock);
		if (video_info.ratio)
			info("ratio: 1/40\n");
		else
			info("ratio: 1/10\n");
	   #endif

	}
	else if (event == SII_EVENT__RX_TMDS_CLOCK_CHNG) {
		struct sii_video_info video_info;
		uint8_t frame_rate = 0;
		info("enter SII_EVENT__RX_TMDS_CLOCK_CHNG..........\n");

		#if 0
		SiiRxVideoInfoGet(inst, &video_info);
		if (video_info.v_res_total != 0 && video_info.h_res_total != 0)
		{
			frame_rate =
				(video_info.tmds_clock * 1000000 / video_info.color_depth * 8) /
				(video_info.h_res_total * video_info.v_res_total);
			frame_rate = (uint8_t)(frame_rate / 5.0 + 0.5f) * 5;
			if (video_info.color_space == SII_COLOR_SPACE__YCBCR420)
				frame_rate *= 2;
		}
		info("\tTotal Resolution: %d x % d", video_info.h_res_total,video_info.v_res_total);
		if (video_info.interlace)
			info(" I\n");
		else
			info(" P\n");

		info("\tActive Resolution: %d x % d", video_info.h_res_active,video_info.v_res_active);

		if (video_info.interlace)
			info(" I\t");
		else
			info(" P\t");

		info("%dHz\t", frame_rate);

		info("%d bits\t", video_info.color_depth);

		switch (video_info.color_space) {
			case SII_COLOR_SPACE__RGB:
				info("RGB\n");
				break;
			case SII_COLOR_SPACE__YCBCR444:
				info("444\n");
				break;
			case SII_COLOR_SPACE__YCBCR422:
				info("422\n");
				break;
			case SII_COLOR_SPACE__YCBCR420:
				info("420\n");
				break;
			case SII_COLOR_SPACE__UNKNOWN:
			default:
				info("Color Space Unknown\n");
				break;
		}
		info("\tTMDS Clock: %d MHz\t", video_info.tmds_clock);
		if (video_info.ratio)
			info("ratio: 1/40\n");
		else
			info("ratio: 1/10\n");

	    #endif
	}
	else {
		info("EVT: Unknown\n");
	}
	#endif
	if(FirstPwonFlag==1)
    	{
	    //AppTasKeyLedMsgLoop();
	}
	dbg("");
}

static bool sii_is_boot_done(sii_inst_t inst)
{
	enum sii_product_id product_id;
	enum sii_boot_status boot_status;
	uint16_t chip_id = 0;
	uint8_t fw_version[32];
	if (SiiWaitForBootDone(inst) != SII_API_SUCCESS) {
		info("Error: Boot Failed\n");
		return false;
	}
	else {

		SiiBootStatusGet(inst, &boot_status);
		if (SII_BOOT_STAT__SUCCESS != boot_status) {
			info("Error: Boot Failed\n");
			return false;
		}

		#if 0
		SiiChipIdGet(inst, &chip_id);
		if (chip_id != CHIP_ID) {
			info("Error: Chip ID not match, require Chip ID: 0x%x\n", CHIP_ID);
			return false;
		}
		SiiProductIdGet(inst, &product_id);
		if (product_id != SII_PRODUCT_ID__RX) {
			info("Error: Product ID not match, require ID: Rx\n");
			return false;
		}
		SiiFirmwareVersionGet(inst, fw_version);
		if (strcmp(fw_version, RX_FW_VER)) {
			info("Warning: Chip FW Version not match.\n");
			//return false;
		}
		#endif
		if((inst>=MAX_OUT)&&(inst<SII9396_DevNum))
			sii_set_rx_hpd(inst,1);
		#if 0
		sii_os_sleep_msec(50);
		sii_set_rx_hpd(inst,0);
		sii_os_sleep_msec(100);
		sii_set_rx_hpd(inst,1);
		#endif

	}
	return true;
}

bool sii_update(sii_inst_t inst)
{
	char bin_file_path[256];
	uint32_t time_start;
	//file_input(bin_file_path);
	info("Flashing...\n");
	time_start = sii_os_get_current_time_second();
	SiiFlashInit(inst);
	if (SiiFlashUpdate(inst, bin_file_path) != SII_API_SUCCESS) {
		return false;
	}
	info("Time Eclipsed: %ds\n",
			sii_os_get_current_time_second() - time_start);
	return true;
}

static void sii_info_display(void)
{
	info("\n");
	info("%s\n\t%s\n\n", copyright_msg, build_time);
	info("%s Host Software Version: \t%s\n", HOST_APP_NAME, HOST_SW_VER);
	info("%s Require FW Version: \t\t%s\n", HOST_APP_NAME, RX_FW_VER);
	info("\n");
}

void SiiSetRXHpdLow2High(BYTE cInstance)
{
	sii_set_rx_hpd(cInstance,0);				//拉低热拔插
	sii_os_sleep_msec(200);
	sii_set_rx_hpd(cInstance,1);				//拉高热拔插
}

bool SiiTxHpdGet(BYTE cInstance)
{
	bool hpd=FALSE;
	sii_get_tx_hpd(cInstance,&hpd);
	return hpd;
}

// 得到9396  输入端是否有接信号?// return TRUE: connected
BOOL Sii9396RxConnectGet(BYTE cInstance)
{
	BOOL  bRetVal = TRUE;
	enum sii_av_link_type type;

	SiiRxAvLinkGet(cInstance, &type);
	switch (type) {
	case SII_AV_LINK__NONE:
		info("Rx Link Type is None\n");
		break;
	case SII_AV_LINK__HDMI1:
		info("Rx Link Type is HDMI 1.4\n");
		break;
	case SII_AV_LINK__HDMI2:
		info("Rx Link Type is HDMI 2.0\n");
		break;
	case SII_AV_LINK__DVI:
		info("Rx Link Type is DVI\n");
		break;
	default:
		break;
	}
	if(type == SII_AV_LINK__NONE)
	{
		bRetVal = FALSE;
	}
	return bRetVal;

}

void SiiTxEdidGetEdidData(BYTE cInstance,PBYTE cpEdid)
{
	sii_get_tx_ds_edid_content(cInstance,0, cpEdid);
	sii_os_sleep_msec(1);
	sii_get_tx_ds_edid_content(cInstance,1, &cpEdid[128]);
}

void SiiSetRxEdidSramWrite(BYTE cInstance, uint8_t  *pData, uint16_t length, eEDIDINPORTSELECT cINPORTSELECT)
{
	Event_t   bEvent;
	BYTE i=0;
	memset(&bEvent, 0, sizeof(Event_t));

	bEvent.Head.opcode=eInputHpdProc;
    	bEvent.Head.DestAddr=mqINPUT;

	if(cINPORTSELECT==SINGLE_INPORT)
	{
		bEvent.args[cInstance]=1;			//记录要进行热插拔脚拉低再拉高的输入端口
		bEvent.args[MAX_IN]=0;
    		bEvent.Head.argCount=MAX_IN;
		sii_set_rx_edid_content(cInstance,0, pData);
		sii_set_rx_edid_content(cInstance,1, &pData[128]);
	}
	else if(cINPORTSELECT==ALL_INPORT)
	{
		bEvent.args[MAX_IN]=1;			//记录要进行热插拔脚拉低再拉高的输入端口
    		bEvent.Head.argCount=MAX_IN+1;
		for(i=0;i<MAX_IN;i++)
		{
			sii_set_rx_edid_content(i,0, pData);
			sii_set_rx_edid_content(i,1, &pData[128]);
		}
	}
    	utilExecOlnyMsgSend(mqINPUT, bEvent);
	SiiPlatformTimerSet(etEDIDPROC_HPD,mS_TO_TICKS(2000) ,1);   // 只执行一次
}

// return : true have HDCP
//             false no HDCP
BOOL Sii9396RxHDCPGet(BYTE cInstance)
{
	BOOL  bRetVal = TRUE;
	enum sii_hdcp_version hdcp_version;
	enum sii_hdcp_status hdcp_status;

	SiiRxHdcpUsVerGet(cInstance, &hdcp_version);
	switch(hdcp_version) {
	case SII_HDCP_VERSION__NONE:
		info("RX HDCP Version is None\n");
		break;
	case SII_HDCP_VERSION__1X:
		info("RX HDCP Version is 1.4\n");
		break;
	case SII_HDCP_VERSION__20:
		info("RX HDCP Version is 2.0\n");
		break;
	case SII_HDCP_VERSION__22:
		info("RX HDCP Version is 2.2\n");
		break;
	default:
		break;
	}

	SiiRxHdcpStatusGet(cInstance, &hdcp_status);

	switch(hdcp_status) {
	case SII_HDCP_STATUS__OFF:
		info("RX HDCP is Off\n");
		break;
	case SII_HDCP_STATUS__SUCCESS_1X:
		info("RX HDCP 1.x is Authenticated\n");
		break;
	case SII_HDCP_STATUS__SUCCESS_22:
		info("RX HDCP 2.2 is Authenticated\n");
		break;
	case SII_HDCP_STATUS__AUTHENTICATING:
		info("RX HDCP is Starting\n");
		break;
	case SII_HDCP_STATUS__FAILED:
		info("RX HDCP is Failed\n");
		break;
	default:
		break;
	}

	if((hdcp_version==SII_HDCP_VERSION__NONE)&&((hdcp_status==SII_HDCP_STATUS__OFF)||(hdcp_status==SII_HDCP_STATUS__FAILED)))
	{
		bRetVal = FALSE;
	}

	return bRetVal;
}

// 得到当前9396 TX HDCP 是否打开
BOOL Sii9396TxHDCPGet(BYTE cInstance)
{
	return AppIsTxHdcpRequired[cInstance];
}

// 使能9396 的TX HDCP
void Sii9396TXHDCPSetStart(BYTE cInstance,BOOL bEnable)
{
	AppIsTxHdcpRequired[cInstance]= bEnable;
	if(bEnable==TRUE)
	{
		SiiTxHdcpProtectionSet(cInstance, TRUE);
	}
	else if(bEnable==FALSE)
	{
		SiiTxHdcpProtectionSet(cInstance, FALSE);
	}
}

// 得到分辨率
BOOL  Sii9396RXResolutionGet(BYTE cInstance,INT* pHorizPixel, INT *pVertLine,INT *pTmdsClock,bool_t* pbInterlace)
{
	POUTPUTPORT_MANAGER_DATA     psOutputPortMgrData;
    	BYTE cInput=0;
	struct sii_video_info video_info;
	uint8_t frame_rate = 0;
	uint8_t Rx5V=0,TxHpd=0;
	enum sii_av_link_type type;

	psOutputPortMgrData = (POUTPUTPORT_MANAGER_DATA)utilDataMgrReadLockData(nnOUTPUTPORT_MANAGER_DATA);

	utilDataMgrUnlock(nnOUTPUTPORT_MANAGER_DATA,
	                      FALSE);
    cInput = psOutputPortMgrData->acInPortSelect[utilInstanceToOutputPort(cInstance)];
    if(cInput<MAX_IN)
	    SiiRxVideoInfoGet(cInput, &video_info);
	if (video_info.v_res_total != 0 && video_info.h_res_total != 0)
	{
		frame_rate =
			(video_info.tmds_clock * 1000000 / video_info.color_depth * 8) /
			(video_info.h_res_total * video_info.v_res_total);
		frame_rate = (uint8_t)(frame_rate / 5.0 + 0.5f) * 5;
		if (video_info.color_space == SII_COLOR_SPACE__YCBCR420)
			frame_rate *= 2;
	}
	info("\tTotal Resolution: %d x % d", video_info.h_res_total,video_info.v_res_total);
	if (video_info.interlace)
		info(" I\n");
	else
		info(" P\n");

	info("\tActive Resolution: %d x % d", video_info.h_res_active,video_info.v_res_active);

	if (video_info.interlace)
		info(" I\t");
	else
		info(" P\t");

	info("%dHz\t", frame_rate);

	info("%d bits\t", video_info.color_depth);

	switch (video_info.color_space) {
		case SII_COLOR_SPACE__RGB:
			info("RGB\n");
			break;
		case SII_COLOR_SPACE__YCBCR444:
			info("444\n");
			break;
		case SII_COLOR_SPACE__YCBCR422:
			info("422\n");
			break;
		case SII_COLOR_SPACE__YCBCR420:
			info("420\n");
			break;
		case SII_COLOR_SPACE__UNKNOWN:
		default:
			info("Color Space Unknown\n");
			break;
	}
	info("\tTMDS Clock: %d MHz\t", video_info.tmds_clock);
	if (video_info.ratio)
		info("ratio: 1/40\n");
	else
		info("ratio: 1/10\n");

	sii_get_tx_hpd(cInstance,&TxHpd);
	#if 0
    SiiRxAvLinkGet(cInstance, &type);
	switch (type) {
	case SII_AV_LINK__NONE:
		TxHpd=0;
		break;
	case SII_AV_LINK__HDMI1:
		TxHpd=1;
		break;
	case SII_AV_LINK__HDMI2:
		TxHpd=1;
		break;
	case SII_AV_LINK__DVI:
		TxHpd=1;
		break;
	default:
		break;
	}
	#endif

	if(cInput<MAX_IN)
		sii_get_rx_plus5v(cInput,&Rx5V);
	else
		Rx5V=0;

	if((Rx5V==1)&&(TxHpd==1))
	{
		*pHorizPixel = video_info.h_res_active;
	 	*pVertLine = video_info.v_res_active;
	 	*pTmdsClock= video_info.tmds_clock;
	   	*pbInterlace = video_info.interlace;
	}
	else
	{
		*pHorizPixel = 0;
	 	*pVertLine = 0;
	 	*pTmdsClock= 0;
	   	*pbInterlace = 0;
	}

	 return TRUE;

}

void Sii9396TxSpdifAudioOnOff(BYTE cInstance,enum sii_audio_mode mode)
{
    BYTE cPort;

    POUTPUTPORT_MANAGER_DATA psOutputPortMgrData;
    cPort =  utilInstanceToOutputPort(cInstance);
    psOutputPortMgrData=(POUTPUTPORT_MANAGER_DATA)utilDataMgrWriteLockData(nnOUTPUTPORT_MANAGER_DATA);
    utilDataMgrUnlock(nnOUTPUTPORT_MANAGER_DATA, TRUE);
    if(psOutputPortMgrData->abHDMIAudio_EN[cPort ] != mode)			//相同状态下不处理
    {
		psOutputPortMgrData->abHDMIAudio_EN[cPort ] = mode;
		sii_set_rx_audio_extraction(cInstance, mode);
    }
}

void get_EDID(ePOWER_STATUS POWER_STATUS,unsigned char *p_data)				//获取第一个接有显示器的输出端口的EDID
{
	 BYTE cOut=0;
	 BYTE cCNT = 0;
	 if((POWER_STATUS==eALREADY_POWERON)||(POWER_STATUS==eFIRST_POWERON))
	 //if(POWER_STATUS==eALREADY_POWERON)
	 {
		 #if 0
		 for (cOut = 0; cOut < MAX_OUT; cOut++)
	     {
		  	if(cCNT==0)
		  	{
				if (TRUE==SiiTxHpdGet(utilOutputPortToInstance(cOut)))
			    {
					SiiTxEdidGetEdidData(utilOutputPortToInstance(cOut), p_data);
					if ((p_data[0] == 0) && (p_data[1] == 0xff) && (p_data[3] == 0xff)&& (p_data[7] == 0))
					{
						cCNT++;
					}
			    }
				else if(cOut==MAX_OUT-1)
				{

				}
		  	}
	     }
	     if(cCNT==0)
		 {
            memcpy(p_data,g_Init_EdidFlashEdidTable, 256);
		 }
		 #endif

		 #if 1
		 for (cOut = 0; cOut < MAX_OUT; cOut++)
	     {
		  	if(cCNT==0)
		  	{
				if ((OutputEdidData[cOut][0] == 0) && (OutputEdidData[cOut][1] == 0xff) && (OutputEdidData[cOut][3] == 0xff)&& (OutputEdidData[cOut][7] == 0))
				{
					cCNT++;
					memcpy(p_data,OutputEdidData[cOut], 256);
				}
		  	}
	     }
		 if(cCNT==0)
		 {
			memcpy(OutputEdidData[0],g_Init_EdidFlashEdidTable, 256);
                     memcpy(p_data,g_Init_EdidFlashEdidTable, 256);
		 }
		 #endif
	 }

	 #if 0
	 else if(POWER_STATUS==eFIRST_POWERON)
	 {
	        for (cOut = 0; cOut < MAX_OUT; cOut++)
	        {
		  	if(cCNT==0)
		  	{
				if ((OutputEdidData[cOut][0] == 0) && (OutputEdidData[cOut][1] == 0xff) && (OutputEdidData[cOut][3] == 0xff)&& (OutputEdidData[cOut][7] == 0))
				{
					cCNT++;
					memcpy(p_data,OutputEdidData[cOut], 256);
				}
		  	}
	        }
		 if(cCNT==0)
		 {
			memcpy(OutputEdidData[0],g_Init_EdidFlashEdidTable, 256);
                        memcpy(p_data,g_Init_EdidFlashEdidTable, 256);
		 }
	 }
	 #endif
}


void Uart1_process(void);

#if 0
void Init_9396(void)
{
    POUTPUTPORT_MANAGER_DATA     psOutputPortMgrData;
	uint8_t trycount=0,crc=0;
	bool exit = false;
	struct sii_config config[SII9396_DevNum];
	BYTE acEdid[256] = {0},i=0;
	uint16_t conut=0;
	BYTE loopCnt=0;
	sii_inst_t inst=SII9396_DevNum-1;

	sii_info_display();

	for(i=0;i<MAX_OUT;i++)      		//从最后一个输出的sil9396开始初始化, 先让输出的sil9396启动
	{
		trycount=0;
		config[inst].device_reset = true;
		config[inst].reset_time = SII_RESET_TIME;
		inst = SiiCreate(inst, sii_event_handler, &config[inst]);
		if (inst == SII_INST_NULL)
	       {
	            info("SiiCreate fail...\n");
	       }

		info("Init_9396###################= %d\n",inst+1);

		info("Wait for Boot Up...\n");
		if (!sii_is_boot_done(inst) != SII_API_SUCCESS) {
			//info("Trying to Update Firmware...\n");
			#if 0
			if (!sii_update(inst)) {
				info("Update Failed!\n");
				goto exit;
			}
			else {
				info("Update Success.\nResetting...\n");
				SiiDeviceReset(inst, SII_RESET_TIME);
				goto wait_for_boot_done;
			}
			#endif
			info("boot_fail...\n");
		}

		while((trycount<6)&&(inst<SII9396_DevNum))
		{
			process_9396(inst);
			trycount++;
		}

		inst--;
		dbg("");
	}

	//sii_os_sleep_msec(20);
	inst=SII9396_DevNum-1;

	for(i=0;i<SII9396_DevNum;i++)      		//Intialize 9396    从最后一个输出的sil9396开始初始化
  	{
		trycount=0;

		if(inst<MAX_IN)				//初始化输入端口
		{
			config[inst].device_reset = true;
			config[inst].reset_time = SII_RESET_TIME;
			inst = SiiCreate(inst, sii_event_handler, &config[inst]);
			if (inst == SII_INST_NULL)
		       {
		            info("SiiCreate fail...\n");
		       }

			info("Init_9396###################= %d\n",inst+1);

			info("Wait for Boot Up...\n");
			if (!sii_is_boot_done(inst) != SII_API_SUCCESS) {
				//info("Trying to Update Firmware...\n");
				#if 0
				if (!sii_update(inst)) {
					info("Update Failed!\n");
					goto exit;
				}
				else {
					info("Update Success.\nResetting...\n");
					SiiDeviceReset(inst, SII_RESET_TIME);
					goto wait_for_boot_done;
				}
				#endif
				info("boot_fail...\n");
			}

			loopCnt=5;          //输入口的循环次数
			while((trycount<loopCnt)&&(inst<SII9396_DevNum))
			{
				process_9396(inst);
				trycount++;
				if(trycount==loopCnt)
				{
					sii_set_rx_hpd(inst,0);				//拉低热拔插
					sii_os_sleep_msec(100);
					sii_set_rx_hpd(inst,1);				//拉高热拔插
				}
				else if(trycount==loopCnt-1)
				{
					dvNVRAMUserEdidRead(edsEDID_IN1 + inst, acEdid);			//恢复每个输入端口断电前保存的EDID数据
					sii_os_sleep_msec(10);
					sii_set_rx_edid_content(inst,0, acEdid);
				 	sii_set_rx_edid_content(inst,1, &acEdid[128]);
					sii_os_sleep_msec(10);
				}
			}
		}
		else
		{
                dbg("");
                info("Init_9396###################= %d\n",inst+1);
                     #if 1
            		psOutputPortMgrData = (POUTPUTPORT_MANAGER_DATA)utilDataMgrReadLockData(nnOUTPUTPORT_MANAGER_DATA);
            		utilDataMgrUnlock(nnOUTPUTPORT_MANAGER_DATA,FALSE);
            		if(psOutputPortMgrData->abHDMIAudio_EN[utilInstanceToOutputPort(inst)]==SII_AUDIO__SPDIF)
		    	{
			    	sii_set_rx_audio_extraction(inst, SII_AUDIO__SPDIF);		//打开输出端的音频分离
		    	}
		       //sii_os_sleep_msec(10);
			    crc=0;
        		if(SiiTxHpdGet(inst))		//获取有接显示器的输出端口的EDID数据
		    	{
				sii_os_sleep_msec(2);
                		SiiTxEdidGetEdidData(inst,OutputEdidData[inst-MAX_OUT]);
				if ((OutputEdidData[inst-MAX_OUT][0] == 0) && (OutputEdidData[inst-MAX_OUT][1] == 0xff) && (OutputEdidData[inst-MAX_OUT][3] == 0xff)&& (OutputEdidData[inst-MAX_OUT][7] == 0))
			       {
				     if(OutputEdidData[inst-MAX_OUT][126] != 1)			// DVI
				     {
						for (conut = 0; conut < 128; conut++)  crc += OutputEdidData[inst-MAX_OUT][conut];
						for (conut = 128; conut < 256; conut++)    OutputEdidData[inst-MAX_OUT][conut] = 0xff;
				     }
				     else
				     {
						for (conut = 0; conut < 256; conut++)  crc += OutputEdidData[inst-MAX_OUT][conut];
				     }
				     if(crc !=0)
					 	memcpy(OutputEdidData[inst-MAX_OUT],g_Init_EdidFlashEdidTable,  256); //否则就用默认的EDID数据
			       }
				else
				{
					memcpy(OutputEdidData[inst-MAX_OUT], g_Init_EdidFlashEdidTable, 256);
				}
		    	}
                    #endif
		}

		if(inst==MAX_OUT)			//获取完第一个输出端口的edid时，进行edid拨码开关的判断和输入端edid的处理
		{
			appInputPortDialSwitchEdidDataDetect();
		}

		inst--;
		dbg("");
	}
	//FirstPwonFlag=1;
}
#endif

#if 1
void Init_9396(void)
{
    POUTPUTPORT_MANAGER_DATA     psOutputPortMgrData;
	uint8_t trycount=0,crc=0;
	bool exit = false;
	struct sii_config config[SII9396_DevNum];
	BYTE acEdid[256] = {0},i=0;
	uint16_t conut=0;
	BYTE loopCnt=0;
	sii_inst_t inst=SII9396_DevNum-1;

	sii_info_display();
	dvInSii9396HardWareReset(); 	// earaly reset
       dvOutSii9396HardWareReset();

	for(i=0;i<SII9396_DevNum;i++)      		//Intialize 9396    从最后一个输出的sil9396开始初始化
  	{
		trycount=0;
		config[inst].device_reset = true;
		config[inst].reset_time = SII_RESET_TIME;
		inst = SiiCreate(inst, sii_event_handler, &config[inst]);
		if (inst == SII_INST_NULL)
	        {
	            info("SiiCreate fail...\n");
	        }

		 info("Init_9396###################= %d\n",inst+1);

		#if 1

	wait_for_boot_done:
		info("Wait for Boot Up...\n");
		if (!sii_is_boot_done(inst) != SII_API_SUCCESS) {
			//info("Trying to Update Firmware...\n");
			#if 0
			if (!sii_update(inst)) {
				info("Update Failed!\n");
				goto exit;
			}
			else {
				info("Update Success.\nResetting...\n");
				SiiDeviceReset(inst, SII_RESET_TIME);
				goto wait_for_boot_done;
			}
			#endif
			info("boot_fail...\n");
		}
		#endif
		#if 0
		if(inst==4)
		{
			dvDeviceBusRead(  eI2CBusreadEDID, 0xa0, 0 , 256,DATA , FLG_WADDR);
			#if 1
			for (i = 0; i < SII_EDID_BLOCK_SIZE; i++)
			{
				if (i % 8 == 0)
					edid_info("\r\n");
				edid_info("%02x ", DATA[i]);
			}
			for (i = 0; i < SII_EDID_BLOCK_SIZE; i++)
			{
				if (i % 8 == 0)
					edid_info("\r\n");
				edid_info("%02x ", DATA[i+128]);
			}
			#endif
		}
		#endif
		#if 1
        if(inst<MAX_IN)
		{
		    loopCnt=5;          //输入口的循环次数
		}
		else
		{
            loopCnt=8;          //输出口的循环次数
		}
		while((trycount<loopCnt)&&(inst<SII9396_DevNum))
		{
			process_9396(inst);
			trycount++;
			if(trycount==loopCnt)
			{
				if(inst<MAX_IN)
				{
					sii_set_rx_hpd(inst,0);				//拉低热拔插
					sii_os_sleep_msec(100);
					sii_set_rx_hpd(inst,1);				//拉高热拔插
				}
				else
				{
				    //	sii_set_rx_hpd(inst,1);
                    #if 0
            		psOutputPortMgrData = (POUTPUTPORT_MANAGER_DATA)utilDataMgrReadLockData(nnOUTPUTPORT_MANAGER_DATA);
            		utilDataMgrUnlock(nnOUTPUTPORT_MANAGER_DATA,FALSE);
            		if(psOutputPortMgrData->abHDMIAudio_EN[utilInstanceToOutputPort(inst)]==SII_AUDIO__SPDIF)
				    {
					    sii_set_rx_audio_extraction(inst, SII_AUDIO__SPDIF);		//打开输出端的音频分离
				    }
				    if ((OutputEdidData[inst-MAX_OUT][0] == 0) && (OutputEdidData[inst-MAX_OUT][1] == 0xff) && (OutputEdidData[inst-MAX_OUT][3] == 0xff)&& (OutputEdidData[inst-MAX_OUT][7] == 0))
			        {
			        }
			        else
			        {
					    sii_os_sleep_msec(2);
                        if(SiiTxHpdGet(inst))								//获取有接显示器的输出端口的EDID数据
    					{
    						sii_os_sleep_msec(2);
                            SiiTxEdidGetEdidData(inst,OutputEdidData[inst-MAX_OUT]);
    						if ((OutputEdidData[inst-MAX_OUT][0] == 0) && (OutputEdidData[inst-MAX_OUT][1] == 0xff) && (OutputEdidData[inst-MAX_OUT][3] == 0xff)&& (OutputEdidData[inst-MAX_OUT][7] == 0))
    					       {
    						     if(OutputEdidData[inst-MAX_OUT][126] != 1)			// DVI
    						     {
    								for (conut = 0; conut < 128; conut++)  crc += OutputEdidData[inst-MAX_OUT][conut];
    								for (conut = 128; conut < 256; conut++)    OutputEdidData[inst-MAX_OUT][conut] = 0xff;
    						     }
    						     else
    						     {
    								for (conut = 0; conut < 256; conut++)  crc += OutputEdidData[inst-MAX_OUT][conut];
    						     }
    						     if(crc !=0)
    							 	memcpy(OutputEdidData[inst-MAX_OUT],g_Init_EdidFlashEdidTable,  256); //否则就用默认的EDID数据
    					       }
    						else
    						{
    							memcpy(OutputEdidData[inst-MAX_OUT], g_Init_EdidFlashEdidTable, 256);
    						}
    					}
    				}
                    #endif
				}
			}

			if(trycount==loopCnt-1)
			{
				if(inst<MAX_IN)				//恢复每个输入端口断电前保存的EDID数据
				{
					#if 1
					dvNVRAMUserEdidRead(edsEDID_IN1 + inst, acEdid);
					sii_os_sleep_msec(10);
					sii_set_rx_edid_content(inst,0, acEdid);
				 	sii_set_rx_edid_content(inst,1, &acEdid[128]);
					sii_os_sleep_msec(10);
					#endif
				}
				else
				{
                    #if 1
            		psOutputPortMgrData = (POUTPUTPORT_MANAGER_DATA)utilDataMgrReadLockData(nnOUTPUTPORT_MANAGER_DATA);
            		utilDataMgrUnlock(nnOUTPUTPORT_MANAGER_DATA,FALSE);
            		if(psOutputPortMgrData->abHDMIAudio_EN[utilInstanceToOutputPort(inst)]==SII_AUDIO__SPDIF)
				    {
					    sii_set_rx_audio_extraction(inst, SII_AUDIO__SPDIF);		//打开输出端的音频分离
				    }
					sii_os_sleep_msec(1);
                    if(SiiTxHpdGet(inst))								//获取有接显示器的输出端口的EDID数据
					{
						sii_os_sleep_msec(1);
                        SiiTxEdidGetEdidData(inst,OutputEdidData[inst-MAX_OUT]);
						if ((OutputEdidData[inst-MAX_OUT][0] == 0) && (OutputEdidData[inst-MAX_OUT][1] == 0xff) && (OutputEdidData[inst-MAX_OUT][3] == 0xff)&& (OutputEdidData[inst-MAX_OUT][7] == 0))
					       {
						     if(OutputEdidData[inst-MAX_OUT][126] != 1)			// DVI
						     {
								for (conut = 0; conut < 128; conut++)  crc += OutputEdidData[inst-MAX_OUT][conut];
								for (conut = 128; conut < 256; conut++)    OutputEdidData[inst-MAX_OUT][conut] = 0xff;
						     }
						     else
						     {
								for (conut = 0; conut < 256; conut++)  crc += OutputEdidData[inst-MAX_OUT][conut];
						     }
						     if(crc !=0)
							 	memcpy(OutputEdidData[inst-MAX_OUT],g_Init_EdidFlashEdidTable,  256); //否则就用默认的EDID数据
					       }
						else
						{
							memcpy(OutputEdidData[inst-MAX_OUT], g_Init_EdidFlashEdidTable, 256);
						}
					}
					#endif
				}
			}
			else if(trycount==loopCnt-2)
			{
                #if 0
                if((inst>=MAX_OUT)&&(inst<SII9396_DevNum))
                {
					sii_os_sleep_msec(1);
                    if(SiiTxHpdGet(inst))								//获取有接显示器的输出端口的EDID数据
					{
						sii_os_sleep_msec(1);
                        SiiTxEdidGetEdidData(inst,OutputEdidData[inst-MAX_OUT]);
						if ((OutputEdidData[inst-MAX_OUT][0] == 0) && (OutputEdidData[inst-MAX_OUT][1] == 0xff) && (OutputEdidData[inst-MAX_OUT][3] == 0xff)&& (OutputEdidData[inst-MAX_OUT][7] == 0))
					       {
						     if(OutputEdidData[inst-MAX_OUT][126] != 1)			// DVI
						     {
								for (conut = 0; conut < 128; conut++)  crc += OutputEdidData[inst-MAX_OUT][conut];
								for (conut = 128; conut < 256; conut++)    OutputEdidData[inst-MAX_OUT][conut] = 0xff;
						     }
						     else
						     {
								for (conut = 0; conut < 256; conut++)  crc += OutputEdidData[inst-MAX_OUT][conut];
						     }
						     if(crc !=0)
							 	memcpy(OutputEdidData[inst-MAX_OUT],g_Init_EdidFlashEdidTable,  256); //否则就用默认的EDID数据
					       }
						else
						{
							memcpy(OutputEdidData[inst-MAX_OUT], g_Init_EdidFlashEdidTable, 256);
						}
					}
				}
				#endif
			}
		}

		if(inst==MAX_OUT)			//获取完第一个输出端口的edid时，进行edid拨码开关的判断和输入端edid的处理
		{
			appInputPortDialSwitchEdidDataDetect();
		}

		inst--;
		#endif
		dbg("");
	}
	//FirstPwonFlag=1;
}
#endif

void process_9396(BYTE cInstance)
{
	SiiHandle(cInstance);
	sii_hal_isr(cInstance);
	//sii_os_sleep_msec(10);
}

#if 0
#ifdef WIN32
#include <stdlib.h>
#include <conio.h>

static enum sii_user_message command_input(void)
{
	enum sii_user_message msg = SII_USER_MSG__NULL;
	if (_kbhit()) {
		switch(_getch()) {
			case 'Q':
			case 'q':
				msg = SII_USER_MSG__QUIT;
				break;
			case 'U':
			case 'u':
				msg = SII_USER_MSG__UPDATE;
				break;
			case 'R':
			case 'r':
				msg = SII_USER_MSG__RESET;
				break;
			default:
				break;
		}
	}
	return msg;
}

static void file_input(char *bin_file_path)
{
	printf("Please input the bin file path: \n");
	scanf("%s", bin_file_path);
}

static void select_hwid(uint8_t dev_num, sii_hwid_t *hwid_list,
		uint32_t *dev_id_list, sii_hwid_t *rx_hwid)
{
	int input;
	int i;
	printf("Find %d Aardvark:\n", dev_num);
	for (i = 0; i < dev_num; i++)
		printf("[%d] %lu\n", hwid_list[i], dev_id_list[i]);
	printf("Please select one for Rx (0 ~ %d): ", hwid_list[dev_num - 1]);
	scanf("%d", &input);
	*rx_hwid = input;
}

int sii_app_rx(sii_hwid_t rx_hwid)
{
	sii_inst_t inst;
	bool exit = false;
	struct sii_config config;

	sii_info_display();

	config.device_reset = true;
	config.reset_time = SII_RESET_TIME;
	inst = SiiCreate(rx_hwid, sii_event_handler, &config);
	if (inst == SII_INST_NULL)
		goto exit;

wait_for_boot_done:
	info("Wait for Boot Up...\n");
	if (!sii_is_boot_done(inst) != SII_API_SUCCESS) {
		info("Trying to Update Firmware...\n");
		if (!sii_update(inst)) {
			info("Update Failed!\n");
			goto exit;
		}
		else {
			info("Update Success.\nResetting...\n");
			SiiDeviceReset(inst, SII_RESET_TIME);
			goto wait_for_boot_done;
		}
	}

	while(!exit) {

		SiiHandle(inst);

		switch(command_input()) {
			case SII_USER_MSG__QUIT:
				exit = true;
				break;
			case SII_USER_MSG__UPDATE:
				if (!sii_update(inst)) {
					info("Update Failed!\n");
				} else {
					info("Update Success.\nResetting...\n");
					SiiDeviceReset(inst, SII_RESET_TIME);
					goto wait_for_boot_done;
				}
				break;
			case SII_USER_MSG__RESET:
				info("Resetting...\n");
				SiiDeviceReset(inst, SII_RESET_TIME);
				goto wait_for_boot_done;
				break;
			default:
				break;
		}
	}
	SiiDelete(inst);
exit:
	return 0;
}


int main(int argc, char *argv[])
{
	sii_hwid_t rx_hwid;
	sii_hwid_t hwid_list[SII_DEV_COUNT_MAX];
	uint32_t dev_id_list[SII_DEV_COUNT_MAX];
	uint8_t dev_num;

	if (SiiPlatformInit(&dev_num, hwid_list, dev_id_list) != SII_API_SUCCESS)
		goto exit;
	if (dev_num > 1) {
		select_hwid(dev_num, hwid_list, dev_id_list, &rx_hwid);
	} else
		rx_hwid = hwid_list[0];

	sii_app_rx(rx_hwid);

	SiiPlatformTerm();
exit:
	printf("Program Terminated\n");
	system("pause");
	return 0;
}
#endif
#endif

