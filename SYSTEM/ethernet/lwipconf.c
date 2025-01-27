/******************************************************
**Copyright (c) 2008 PTN Electronics Limited.All Rights Reserved
**FileName:         lwipconf.c
**Writer:           wf8421
**Create Date:      2023-04-10
**Rewriter:         wf8421
**Rewrite Date:     2023-04-10
**Description:      lwip����
**History:
**Version:          V1.0.0
******************************************************/
#include "includes.h"

#include "lwip/mem.h"
#include "lwip/memp.h"
#include "lwip/tcp.h"
#include "lwip/udp.h"
#include "netif/etharp.h"
#include "lwip/dhcp.h"
#include "ethernetif.h"
#include "stdint.h"
#include "lwipconf.h"
#include "ethnet.h"
#include <stdio.h>
#include "lwip/priv/tcp_priv.h"
#include "lwip/timeouts.h"

#if 1
#define LWIP_DBG          dbg_printf
#else
#define LWIP_DBG(x)
#endif

#define MAX_DHCP_TRIES        4


typedef enum {
    DHCP_START = 0,
    DHCP_WAIT_ADDRESS,
    DHCP_ADDRESS_ASSIGNED,
    DHCP_TIMEOUT
} dhcp_state_enum;

#ifdef USE_DHCP
uint32_t dhcp_fine_timer = 0;
uint32_t dhcp_coarse_timer = 0;
dhcp_state_enum dhcp_state = DHCP_START;
#endif /* USE_DHCP */

struct netif g_mynetif;
uint32_t tcp_timer = 0;
uint32_t arp_timer = 0;
ip_addr_t ip_address = {0};


stIPAddrConfig_t g_stIPConfig =
{
    {192,168,0,178},
    {255,255,255,0},
    {192,168,0,1},
    4001,
    1000,
    {0x00,0x00,0x00,0x00,0x00,0x00},
};

void lwip_dhcp_process_handle(void);
extern void lwip_netif_status_callback(struct netif *netif);

/*!
    \brief      initializes the LwIP stack
    \param[in]  none
    \param[out] none
    \retval     none
*/
void lwip_stack_init(void)
{
    ip_addr_t ipaddr;
    ip_addr_t netmask;
    ip_addr_t gw;

    /* initializes the dynamic memory heap defined by MEM_SIZE */
    mem_init();

    /* initializes the memory pools defined by MEMP_NUM_x */
    memp_init();
#ifdef TIMEOUT_CHECK_USE_LWIP
    sys_timeouts_init();
#endif /* TIMEOUT_CHECK_USE_LWIP */
    #if 1
    if(g_stIPConfig.u8EnetMac[0] == 0x00
        &&g_stIPConfig.u8EnetMac[1] == 0x00
        &&g_stIPConfig.u8EnetMac[2] == 0x00
        &&g_stIPConfig.u8EnetMac[3] == 0x00
        &&g_stIPConfig.u8EnetMac[4] == 0x00
        &&g_stIPConfig.u8EnetMac[5] == 0x00
        )
    {
        uint32_t l_u32ChipID[3];
        Get_ChipID(l_u32ChipID);
        LWIP_DBG("\r\n");
        LWIP_DBG("l_u32ChipID[0] = 0x%08x\r\n",l_u32ChipID[0]);
        LWIP_DBG("l_u32ChipID[1] = 0x%08x\r\n",l_u32ChipID[1]);
        LWIP_DBG("l_u32ChipID[2] = 0x%08x\r\n",l_u32ChipID[2]);
        g_stIPConfig.u8EnetMac[0] = (u8)((l_u32ChipID[1]>>8)&0xff)^0xA5;
        g_stIPConfig.u8EnetMac[1] = (u8)((l_u32ChipID[1])&0xff)^0xA5;
        g_stIPConfig.u8EnetMac[2] = (u8)((l_u32ChipID[2]>>24)&0xff)^0xA5;
        g_stIPConfig.u8EnetMac[3] = (u8)((l_u32ChipID[2]>>16)&0xff)^0xA5;
        g_stIPConfig.u8EnetMac[4] = (u8)((l_u32ChipID[2]>>8)&0xff)^0xA5;
        g_stIPConfig.u8EnetMac[5] = (u8)((l_u32ChipID[2])&0xff)^0xA5;
    }
    #endif
#ifdef USE_DHCP
    ipaddr.addr = 0;
    netmask.addr = 0;
    gw.addr = 0;
#else
    IP4_ADDR(&ipaddr, g_stIPConfig.u8IPAddr[0], g_stIPConfig.u8IPAddr[1], g_stIPConfig.u8IPAddr[2], g_stIPConfig.u8IPAddr[3]);
    IP4_ADDR(&netmask, g_stIPConfig.u8NetMask[0], g_stIPConfig.u8NetMask[1], g_stIPConfig.u8NetMask[2], g_stIPConfig.u8NetMask[3]);
    IP4_ADDR(&gw, g_stIPConfig.u8GateWay[0], g_stIPConfig.u8GateWay[1], g_stIPConfig.u8GateWay[2], g_stIPConfig.u8GateWay[3]);

    LWIP_DBG("IP: %d.%d.%d.%d\r\n",g_stIPConfig.u8IPAddr[0],g_stIPConfig.u8IPAddr[1],g_stIPConfig.u8IPAddr[2],g_stIPConfig.u8IPAddr[3]);
    LWIP_DBG("MASK: %d.%d.%d.%d\r\n",g_stIPConfig.u8NetMask[0],g_stIPConfig.u8NetMask[1],g_stIPConfig.u8NetMask[2],g_stIPConfig.u8NetMask[3]);
    LWIP_DBG("GW: %d.%d.%d.%d\r\n",g_stIPConfig.u8GateWay[0],g_stIPConfig.u8GateWay[1],g_stIPConfig.u8GateWay[2],g_stIPConfig.u8GateWay[3]);
    LWIP_DBG("MAC: %02x:%02x:%02x:%02x:%02x:%02x\r\n",g_stIPConfig.u8EnetMac[0],
                                    g_stIPConfig.u8EnetMac[1],
                                    g_stIPConfig.u8EnetMac[2],
                                    g_stIPConfig.u8EnetMac[3],
                                    g_stIPConfig.u8EnetMac[4],
                                    g_stIPConfig.u8EnetMac[5]);
#endif /* USE_DHCP */

    /* - netif_add(struct netif *netif, ip_addr_t *ipaddr,
              ip_addr_t *netmask, ip_addr_t *gw,
              void *state, err_t (* init)(struct netif *netif),
              err_t (* input)(struct pbuf *p, struct netif *netif))

     Adds your network interface to the netif_list. Allocate a struct
    netif and pass a pointer to this structure as the first argument.
    Give pointers to cleared ip_addr structures when using DHCP,
    or fill them with sane numbers otherwise. The state pointer may be NULL.

    The init function pointer must point to a initialization function for
    your ethernet netif interface. The following code illustrates it's use.*/

    netif_add(&g_mynetif, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, &ethernet_input);
    /* registers the default network interface */
    netif_set_default(&g_mynetif);
    netif_set_status_callback(&g_mynetif, lwip_netif_status_callback);
#ifdef USE_DHCP
    dhcp_start(&g_mynetif);
#else
    /* when the netif is fully configured this function must be called */
    netif_set_up(&g_mynetif);
#endif /* USE_DHCP */
}

/*!
    \brief      called when a frame is received
    \param[in]  none
    \param[out] none
    \retval     none
*/
void lwip_pkt_handle(void)
{
    /* read a received packet from the Ethernet buffers and send it to the lwIP for handling */
    ethernetif_input(&g_mynetif);
}

/*!
    \brief      LwIP periodic tasks
    \param[in]  localtime the current LocalTime value
    \param[out] none
    \retval     none
*/
void lwip_periodic_handle(__IO uint32_t localtime)
{
#if LWIP_TCP
    /* TCP periodic process every 250 ms */
    if(localtime - tcp_timer >= TCP_TMR_INTERVAL) {
        tcp_timer =  localtime;
        tcp_tmr();
    }

#endif /* LWIP_TCP */

    /* ARP periodic process every 5s */
    if((localtime - arp_timer) >= ARP_TMR_INTERVAL) {
        arp_timer = localtime;
        etharp_tmr();
    }

#ifdef USE_DHCP
    /* fine DHCP periodic process every 500ms */
    if(localtime - dhcp_fine_timer >= DHCP_FINE_TIMER_MSECS) {
        dhcp_fine_timer =  localtime;
        dhcp_fine_tmr();
        if((DHCP_ADDRESS_ASSIGNED != dhcp_state) && (DHCP_TIMEOUT != dhcp_state)) {
            /* process DHCP state machine */
            lwip_dhcp_process_handle();
        }
    }

    /* DHCP coarse periodic process every 60s */
    if(localtime - dhcp_coarse_timer >= DHCP_COARSE_TIMER_MSECS) {
        dhcp_coarse_timer =  localtime;
        dhcp_coarse_tmr();
    }

#endif /* USE_DHCP */
}

#ifdef USE_DHCP
/*!
    \brief      lwip_dhcp_process_handle
    \param[in]  none
    \param[out] none
    \retval     none
*/
void lwip_dhcp_process_handle(void)
{
    ip_addr_t ipaddr;
    ip_addr_t netmask;
    ip_addr_t gw;
    struct dhcp *dhcp_client;

    switch(dhcp_state) {
    case DHCP_START:
        dhcp_start(&g_mynetif);

        dhcp_state = DHCP_WAIT_ADDRESS;
        break;

    case DHCP_WAIT_ADDRESS:
        /* read the new IP address */
        ip_address.addr = g_mynetif.ip_addr.addr;

        if(0 != ip_address.addr) {
            dhcp_state = DHCP_ADDRESS_ASSIGNED;

            LWIP_DBG("\r\nDHCP -- eval board ip address: %d.%d.%d.%d \r\n", ip4_addr1_16(&ip_address), \
                   ip4_addr2_16(&ip_address), ip4_addr3_16(&ip_address), ip4_addr4_16(&ip_address));
        } else {
            /* DHCP timeout */
            dhcp_client = netif_dhcp_data(&g_mynetif);
            if(dhcp_client->tries > MAX_DHCP_TRIES) {
                dhcp_state = DHCP_TIMEOUT;
                /* stop DHCP */
                dhcp_stop(&g_mynetif);

                /* static address used */
                IP4_ADDR(&ipaddr, g_stIPConfig.u8IPAddr[0], g_stIPConfig.u8IPAddr[1], g_stIPConfig.u8IPAddr[2], g_stIPConfig.u8IPAddr[3]);
                IP4_ADDR(&netmask, g_stIPConfig.u8NetMask[0], g_stIPConfig.u8NetMask[1], g_stIPConfig.u8NetMask[2], g_stIPConfig.u8NetMask[3]);
                IP4_ADDR(&gw, g_stIPConfig.u8GateWay[0], g_stIPConfig.u8GateWay[1], g_stIPConfig.u8GateWay[2], g_stIPConfig.u8GateWay[3]);
                netif_set_addr(&g_mynetif, &ipaddr, &netmask, &gw);
            }
        }
        break;

    default:
        break;
    }
}
#endif /* USE_DHCP */

#if (NO_SYS)
unsigned long sys_now(void)
{
    extern volatile unsigned int g_localtime;
    return g_localtime;
}
#endif

