/******************************************************
**Copyright (c) 2008 PTN Electronics Limited.All Rights Reserved
**FileName:         udp_function.c
**Writer:           wf8421
**Create Date:      2023-04-20
**Rewriter:         wf8421
**Rewrite Date:     2023-04-20
**Description:      udp function
**History:
**Version:          V1.0.0
******************************************************/
#include "lwip/opt.h"
#if LWIP_UDP
#include "udp_function.h"
#include "lwip/udp.h"
#include <string.h>
#include <stdio.h>
#ifdef GD32F4XX
#include "gd32f4xx.h"
#elif STM32F2XX_HD
#include "stm32f2xx.h"
#endif
#include "lwipconf.h"

#if 1
#define DBG_UDP          dbg_printf
#else
#define DBG_UDP(x)
#endif

vvFuncPoint pvvFuncUdpServerCallBack = NULL;

static void udp_server_recv(void *arg, struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *addr, u16_t port);

/*!
    \brief      called when a data is received on the udp connection
    \param[in]  args: the user argument
    \param[in]  pcb: the udp_pcb that has received the data
    \param[in]  p: the packet buffer
    \param[in]  addr: pointer on the receive IP address
    \param[in]  port: receive port number
    \param[out] none
    \retval     none
*/
static void udp_server_recv(void *arg, struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *addr, u16_t port)
{
    ip_addr_t destaddr = *addr;

    if(p != NULL) {
        udp_sendto(pcb, p, &destaddr, port);

        if(pvvFuncUdpServerCallBack != NULL)
        {
            pvvFuncUdpServerCallBack();
        }
        /* end of processing, we free the pbuf */
        pbuf_free(p);
    }
}

/*!
    \brief      initialize the udp_client application
    \param[in]  none
    \param[out] none
    \retval     none
*/
void udp_server_init(void)
{
    struct udp_pcb *udppcb;

    /* create a new UDP control block */
    udppcb = udp_new();
    if(udppcb == NULL)
    {
        DBG_UDP("udp_server_init error\r\n");
        return;
    }
    /* assign to the new pcb a local IP address and a port number */
    udp_bind(udppcb, IP_ADDR_ANY, g_stIPConfig.u16UdpPort);
    udp_recv(udppcb, udp_server_recv, NULL);
}
#endif

