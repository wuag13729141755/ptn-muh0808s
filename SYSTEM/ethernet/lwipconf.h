/******************************************************
**Copyright (c) 2008 PTN Electronics Limited.All Rights Reserved
**FileName:         lwipconf.h
**Writer:           wf8421
**Create Date:      2023-04-10
**Rewriter:         wf8421
**Rewrite Date:     2023-04-10
**Description:      lwip≈‰÷√
**History:
**Version:          V1.0.0
******************************************************/
#ifndef __LWIPCONF_H_
#define __LWIPCONF_H_

//==============================================================================================
typedef struct _stIPConfig_t_
{
    uint8_t  u8IPAddr[4];
    uint8_t  u8NetMask[4];
    uint8_t  u8GateWay[4];
    uint16_t u16TcpPort;
    uint16_t u16UdpPort;
    uint8_t  u8EnetMac[6];
}stIPAddrConfig_t;

extern stIPAddrConfig_t g_stIPConfig;


void lwip_stack_init(void);
void lwip_pkt_handle(void);
void lwip_periodic_handle(__IO uint32_t localtime);
#endif

