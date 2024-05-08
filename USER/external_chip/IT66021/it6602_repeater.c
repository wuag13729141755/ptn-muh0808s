///*****************************************
//  Copyright (C) 2009-2014
//  ITE Tech. Inc. All Rights Reserved
//  Proprietary and Confidential
///*****************************************
//   @file   <it6602_repeater.c>
//   @author Max.Kao@ite.com.tw
//   @date   2014/09/11
//   @fileversion: ITE_MHLRX_SAMPLE_V1.01
//******************************************/
#ifdef SUPPORT_HDCP_REPEATER
#define SUPPORT_PSEUDO_REPEATER

#include "it6602.h"
#include "sha1.h"




//RXHDCP_State_Type RHDCPState = RXHDCP_Reset ;
// BOOL repeater = FALSE ;


// static BYTE RxHDCPStatusFlag = 0 ;
static BYTE cDownStream = 0 ;
static BYTE KSVOffset=0 ;

static _XDATA BYTE Vr[20] ;
static _XDATA BYTE M0[8] ;
static _XDATA BYTE SHABuff[64] ;

extern BYTE bHDCPMode ;

#if defined(It6602_Debug_message)
#if It6602_Debug_message
char _CODE *RXHDCPStr[] =
{
    "RXHDCP_Reset = 0",
    "RXHDCP_AuthStart",
    "RXHDCP_AuthDone",
    "RXHDCP_UpdateKSVList",
    "RXHDCP_Ready",
    "RXHDCP_FailReady",
    "RXHDCP_Reserved"

};
#endif
#endif
///////////////////////////////////////////////////////////////////////////////
// Function Prototype
///////////////////////////////////////////////////////////////////////////////
BOOL it6602_HDCP_WriteVR(struct it6602_dev_data *it6602,BYTE *pVr);


///////////////////////////////////////////////////////////////////////////////
// Function Body
///////////////////////////////////////////////////////////////////////////////
void it6602_HDCP_SetReceiver(struct it6602_dev_data *it6602)
{
    /*
	PowerDownHDMI();
    it6602->m_HDCPRepeater = 0 ;
	InitHDMIRX(TRUE);
    */
}

void it6602_HDCP_SetRepeater(struct it6602_dev_data *it6602)
{
    /*
    PowerDownHDMI();
    it6602->m_HDCPRepeater = 1 ;
    InitHDMIRX(TRUE);
    */
}

#ifdef REPEATER_DEBUG
// void it6602_HDCP_SetRdyTimeOut(struct it6602_dev_data *it6602)
// {
//     PowerDownHDMI();
//     bHDCPMode = HDCP_REPEATER | HDCP_RDY_TIMEOUT ;
//     InitHDMIRX(TRUE);
// }
//
// void it6602_HDCP_SetInvalidV(struct it6602_dev_data *it6602)
// {
//     PowerDownHDMI();
//     bHDCPMode = HDCP_REPEATER | HDCP_INVALID_V ;
//     InitHDMIRX(FALSE);
// }
//
// void it6602_HDCP_SetOverDownStream(struct it6602_dev_data *it6602)
// {
//     PowerDownHDMI();
//     bHDCPMode = HDCP_REPEATER | HDCP_OVER_DOWNSTREAM ;
//     InitHDMIRX(TRUE);
// }
//
// void it6602_HDCP_SetOverCascade(struct it6602_dev_data *it6602)
// {
//     PowerDownHDMI();
//     bHDCPMode = HDCP_REPEATER | HDCP_OVER_CASCADE ;
//     InitHDMIRX(TRUE);
// }
#endif // REPEATER_DEBUG

void it6602_HDCP_RepeaterCapabilitySet(struct it6602_dev_data *it6602,BYTE uc)
{
    unsigned char reg ;

    hdmirxset(REG_RX_P0_HDCP_CTRL,uc,uc);
    hdmirxset(REG_RX_P1_HDCP_CTRL,uc,uc);
    HDCP_DEBUG_PRINTF(("RxHDCPRepeaterCapabilitySet=%2X\n",(int)uc));
}

void it6602_HDCP_RepeaterCapabilityClear(struct it6602_dev_data *it6602,BYTE uc)
{
    unsigned char reg ;
    if( 0 == it6602->m_ucCurrentHDMIPort )
    {
        reg = REG_RX_P0_HDCP_CTRL ;
    }
    else
    {
        reg = REG_RX_P1_HDCP_CTRL ;
    }
	hdmirxset(reg,uc,0);
    HDCP_DEBUG_PRINTF(("RxHDCPRepeaterCapabilityClear=%2X\n",(int)uc));
}

BOOL it6602_HDCP_GenVR(BYTE Vr[])
{
	// extern void SHA_Simple(void *p, LONG len, BYTE *output);

    int i, n ;

    n = cDownStream*5+10 ;
    for(i = n ; i < 64 ; i++)
    {
        SHABuff[i] = 0 ;
    }
	for(i = 0 ; i < 64 ; i++)
	{
		HDCP_DEBUG_PRINTF(("%02X ",(int)SHABuff[i]));
		if(0==((i+1)%8))HDCP_DEBUG_PRINTF(("\n"));
	}
	HDCP_DEBUG_PRINTF(("n=%2X\n",(int)n));
    SHA_Simple(SHABuff, (LONG)n, Vr);
	HDCP_DEBUG_PRINTF(("SHA[]: "));
	for(i = 0 ; i < 20 ; i++)
	{
		HDCP_DEBUG_PRINTF(("%02X ",(int)Vr[i]));
	}
	HDCP_DEBUG_PRINTF(("\n"));
    return TRUE ;
}


BOOL set_it6602_HDCP_KSVList(struct it6602_dev_data *it6602,BYTE offset, BYTE *pKSVList, BYTE count)
{
	BYTE i=0;
	BYTE idx = 0 ;
	BYTE c ;

    if(!pKSVList)
    {
        return FALSE ;
    }


    chgbank(1) ;
    if((offset+count)> HDMIRX_MAX_KSV)
    {
        return FALSE ;
    }

    for(KSVOffset = offset ; (KSVOffset < HDMIRX_MAX_KSV)&& (idx < count); KSVOffset ++, idx++)
    {
    	for(i = 0 ;i<5;i++)
    	{
    	    c = pKSVList[idx*5+i] ;
    		hdmirxwr(REG_RX_KSV_00+i+KSVOffset*5, c) ;
    		SHABuff[KSVOffset*5+i] = c ;
    	}
    }
    chgbank(0);
	return TRUE;
}

BOOL it6602_HDCP_GetM0(struct it6602_dev_data *it6602)
{
	BYTE i;
	BYTE reg ;
    if(cDownStream > HDMIRX_MAX_KSV)
    {
        return FALSE ;
    }
    chgbank(2) ;
    if(0==it6602->m_ucCurrentHDMIPort)
    {
        reg = REG_RX_P0_M0 ;
    }
    else
    {
        reg = REG_RX_P1_M0 ;
    }

	for(i =0 ;i<8;i++)
	{
		SHABuff[cDownStream*5+2+i]=hdmirxrd(reg+i);
		HDCP_DEBUG_PRINTF(("M[%d] = %02X\n",i,(int)SHABuff[cDownStream*5+2+i]));
	}

    chgbank(0) ;
    HDCP_DEBUG_PRINTF(("RxHDCPGetM0\n"));
	return TRUE;
}

BOOL it6602_HDCP_GetBstatus(struct it6602_dev_data *it6602,USHORT *pBstatus)
{
    if(!pBstatus)
    {
        return FALSE ;
    }
    chgbank(1);
    if(it6602->m_ucCurrentHDMIPort==1)
    {
        *pBstatus=hdmirxrd(REG_RX_P1_BSTATUS_L);
        *pBstatus <<= 8 ;
        *pBstatus |= hdmirxrd(REG_RX_P1_BSTATUS_L);
    }
    else
    {
        *pBstatus=hdmirxrd(REG_RX_P0_BSTATUS_L);
        *pBstatus <<= 8 ;
        *pBstatus |= hdmirxrd(REG_RX_P0_BSTATUS_L);
    }
    chgbank(0);
    HDCP_DEBUG_PRINTF(("RxHDCPGetBstatus\n"));
	return TRUE;
}

void set_it6602_HDCP_BStatus(struct it6602_dev_data *it6602,WORD bstatus)
{
    BYTE HDCP ;
    chgbank(1);
    if(it6602->m_ucCurrentHDMIPort==1)
    {
        hdmirxwr(REG_RX_P1_BSTATUS_H,(BYTE)((bstatus>>8)& 0x0F));
        hdmirxwr(REG_RX_P1_BSTATUS_L,(BYTE)(bstatus & 0xFF));
    }
    else
    {
        hdmirxwr(REG_RX_P0_BSTATUS_H,(BYTE)((bstatus>>8)& 0x0F));
        hdmirxwr(REG_RX_P0_BSTATUS_L,(BYTE)(bstatus & 0xFF));
    }

    chgbank(0);

    cDownStream = (BYTE)(bstatus &0x7F);
    if((0==(bstatus & 0x880))&& cDownStream <= HDMIRX_MAX_KSV)
    {
        SHABuff[cDownStream*5] = (BYTE)(bstatus &0xFF);
        SHABuff[cDownStream*5+1] = (BYTE)((bstatus>>8)&0x0F)| (IsHDMIMode()?0x10:0);
    }

}

BOOL it6602_HDCP_WriteVR(struct it6602_dev_data *it6602,BYTE *pVr)
{
	BYTE i;
    if(!pVr)
    {
        return FALSE ;
    }
    chgbank(1);

	for(i = 0 ; i < 20 ; i++ )
	{
		hdmirxwr(REG_RX_SHA1_00 +i,*(pVr+i));
		if(!(i%5))HDCP_DEBUG_PRINTF(("\n"));
		HDCP_DEBUG_PRINTF(("SHA1[%2X]=%2X ,",(int)i,(int)*(pVr+i))) ;
	}
    chgbank(0);
	Dump_ITEHDMIReg();
	return TRUE;
}

void set_it6602_HDCP_Event(struct it6602_dev_data *it6602,BYTE event)
{
    it6602->m_HDCPStatusFlag |= event ;
}


BOOL get_it6602_HDCP_Event(struct it6602_dev_data *it6602,BYTE event)
{
    BOOL retv ;
    retv = (it6602->m_HDCPStatusFlag & event)?TRUE:FALSE ;
    it6602->m_HDCPStatusFlag &= ~event ;
    return retv ;
}


void set_it6602_HDCP_CalcSHA(struct it6602_dev_data *it6602)
{
#if 1
    IT6602_SwitchRxHDCPState(it6602,RXHDCP_UpdateKSVList);
#else
    it6602_HDCP_GetM0(it6602);
    it6602_HDCP_GenVR(Vr);
    it6602_HDCP_WriteVR(it6602,Vr);
    IT6602_SwitchRxHDCPState(it6602,RXHDCP_Ready);
#endif
}


void IT6602_SwitchRxHDCPState(struct it6602_dev_data *it6602, RxHDCP_State_Type state)
{
    if(it6602->m_RxHDCPState == state)
    {
        return ;
    }

    HDCP_DEBUG_PRINTF(("IT6602_SwitchRxHDCPState():%s->%s\n",RXHDCPStr[it6602->m_RxHDCPState ],RXHDCPStr[it6602->m_RxHDCPState ])) ;

    switch(state)
    {
    case RXHDCP_Reset:
    	it6602_HDCP_RepeaterCapabilityClear(it6602,B_KSV_READY);

        break ;

    case RXHDCP_AuthStart:
    	it6602_HDCP_RepeaterCapabilityClear(it6602,B_KSV_READY);
	    set_it6602_HDCP_Event(it6602,EVENT_RXHDCP_AUTH_START);
        downstream_ActivateHDCP(it6602) ; // for activate downstream HDCP
		break ;

    case RXHDCP_AuthDone:
	    set_it6602_HDCP_Event(it6602,EVENT_RXHDCP_AUTH_DONE);
	    KSVOffset=0 ;
	    cDownStream = 0 ;
        break ;

    case RXHDCP_UpdateKSVList:
        break ;

    case RXHDCP_Ready:
        it6602_HDCP_RepeaterCapabilitySet(it6602,B_KSV_READY);
        break ;

    case RXHDCP_FailReady:
        it6602_HDCP_RepeaterCapabilitySet(it6602,B_KSV_READY);
        break ;
    }

    it6602->m_RxHDCPState = state ;
}

void set_it6602_HDCP_FailReady(struct it6602_dev_data *it6602)
{
    IT6602_SwitchRxHDCPState(it6602,RXHDCP_FailReady);
}

void it6602_HDCP_Handler(struct it6602_dev_data *it6602)
{
    if(!(it6602->m_HDCPRepeater))
    {
        return ;
    }
    switch(it6602->m_RxHDCPState)
    {
    case RXHDCP_Reset:
/*
        if(hdmirxrd(REG_P0_HDCP_STATUS)& B_R0_READY)
        {
            IT6602_SwitchRxHDCPState(it6602,RXHDCP_AuthDone);
        }
        break ;
*/
    case RXHDCP_AuthStart:


        if(0==it6602->m_ucCurrentHDMIPort)
        {
            if(hdmirxrd(REG_P0_HDCP_STATUS)& B_R0_READY)
            {
                IT6602_SwitchRxHDCPState(it6602,RXHDCP_AuthDone);
            }
        }
        else
        {
            if(hdmirxrd(REG_P1_HDCP_STATUS)& B_R0_READY)
            {
                IT6602_SwitchRxHDCPState(it6602,RXHDCP_AuthDone);
            }
        }
        break ;

    case RXHDCP_AuthDone:
        break ;

    case RXHDCP_UpdateKSVList:
        it6602_HDCP_GetM0(it6602);
        it6602_HDCP_GenVR(Vr);
        it6602_HDCP_WriteVR(it6602,Vr);
        IT6602_SwitchRxHDCPState(it6602,RXHDCP_Ready);
        break ;
    }
}

void downstream_ActivateHDCP(struct it6602_dev_data *it6602) // for activate downstream HDCP
{
	// it should drive true sink HDCP behavior. Here is a dummy funnciton.
}

#ifdef SUPPORT_PSEUDO_REPEATER

static _CODE BYTE  SampleKSVList[] = {
	0x2E,0x17,0x6A,0x79,0x35,
	0x0F,0xE2,0x71,0x8E,0x47,
	0xA6,0x97,0x53,0xE8,0x74,
	0x0F,0xF5,0x26,0x36,0x54
};

#endif


void downstream_HDCPComplete(struct it6602_dev_data *it6602) // for activate downstream HDCP
{
	// it should drive true sink HDCP behavior. Here is a pseudo funnciton.
#ifdef SUPPORT_PSEUDO_REPEATER

	USHORT bStatus = 0x0103 ;
	BYTE KSVList[5] ;
    int i,j ;
    int DownStreamCount = 3;

    for(i = 0 ; i < 5*DownStreamCount ; i+=5)
    {
        for( j = 0 ;j< 5 ; j++)
        {
            KSVList[j] = SampleKSVList[i+j] ;
        }
        set_it6602_HDCP_KSVList(it6602,i+5,KSVList,1);
    }

    if( DownStreamCount > 0 )
    {
        bStatus=DownStreamCount|0x0100 ;
    }
    else
    {
        bStatus=0;
    }

    set_it6602_HDCP_BStatus(it6602,bStatus);
    set_it6602_HDCP_CalcSHA(it6602);
#endif// SUPPORT_PSEUDO_REPEATER
    IT6602_SwitchRxHDCPState(it6602,RXHDCP_UpdateKSVList) ;

}

#endif // SUPPORT_HDCP_REPEATER
