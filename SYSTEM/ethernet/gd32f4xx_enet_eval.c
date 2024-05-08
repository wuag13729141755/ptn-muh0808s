/*!
    \file    gd32f4xx_enet_eval.c
    \brief   ethernet hardware configuration

    \version 2022-05-26, V2.0.0, demo for GD32F4xx
*/

/*
    Copyright (c) 2022, GigaDevice Semiconductor Inc

    Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors
       may be used to endorse or promote products derived from this software without
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
OF SUCH DAMAGE.
*/
#include "includes.h"
#include "gd32f4xx_enet.h"
#include "gd32f4xx_enet_eval.h"
#include "netconf.h"
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
#define DBG_ETHNET          dbg_printf
#else
#define DBG_ETHNET(x)
#endif

#ifndef SYSTEMTICK_PERIOD_MS
#define SYSTEMTICK_PERIOD_MS    1
#endif

__IO uint32_t g_localtime = 0; /* for creating a time reference incremented by 10ms */
static __IO uint32_t enet_init_status = 0;
static void enet_gpio_config(void);
static void enet_mac_dma_config(void);
#ifdef USE_ENET_INTERRUPT
static void nvic_configuration(void);
#endif /* USE_ENET_INTERRUPT */
void ethnet_udp_server_init(void);
extern void hello_gigadevice_init(void);

/*!
    \brief      setup ethernet system(GPIOs, clocks, MAC, DMA, systick)
    \param[in]  none
    \param[out] none
    \retval     none
*/
void enet_system_setup(void)
{
//    uint32_t ahb_frequency = 0;

#ifdef USE_ENET_INTERRUPT
    nvic_configuration();
#endif /* USE_ENET_INTERRUPT */

    /* configure the GPIO ports for ethernet pins */
    enet_gpio_config();

    /* configure the ethernet MAC/DMA */
    enet_mac_dma_config();

    if(0 == enet_init_status) {
//        while(1) {
//        }
    }

#ifdef USE_ENET_INTERRUPT
    enet_interrupt_enable(ENET_DMA_INT_NIE);
    enet_interrupt_enable(ENET_DMA_INT_RIE);
#endif /* USE_ENET_INTERRUPT */

#ifdef SELECT_DESCRIPTORS_ENHANCED_MODE
    enet_desc_select_enhanced_mode();
#endif /* SELECT_DESCRIPTORS_ENHANCED_MODE */

#if 0
    /* configure systick clock source as HCLK */
    systick_clksource_set(SYSTICK_CLKSOURCE_HCLK);

    /* an interrupt every 10ms */
    ahb_frequency = rcu_clock_freq_get(CK_AHB);
    SysTick_Config(ahb_frequency / 100);
#endif
}

/*!
    \brief      configures the ethernet interface
    \param[in]  none
    \param[out] none
    \retval     none
*/
static void enet_mac_dma_config(void)
{
    ErrStatus reval_state = ERROR;

    /* enable ethernet clock  */
    rcu_periph_clock_enable(RCU_ENET);
    rcu_periph_clock_enable(RCU_ENETTX);
    rcu_periph_clock_enable(RCU_ENETRX);

    /* reset ethernet on AHB bus */
    enet_deinit();

    reval_state = enet_software_reset();
    if(ERROR == reval_state) {
        while(1) {}
    }

    /* configure the parameters which are usually less cared for enet initialization */
//  enet_initpara_config(HALFDUPLEX_OPTION, ENET_CARRIERSENSE_ENABLE|ENET_RECEIVEOWN_ENABLE|ENET_RETRYTRANSMISSION_DISABLE|ENET_BACKOFFLIMIT_10|ENET_DEFERRALCHECK_DISABLE);
//  enet_initpara_config(DMA_OPTION, ENET_FLUSH_RXFRAME_ENABLE|ENET_SECONDFRAME_OPT_ENABLE|ENET_NORMAL_DESCRIPTOR);

#ifdef CHECKSUM_BY_HARDWARE
    enet_init_status = enet_init(ENET_AUTO_NEGOTIATION, ENET_AUTOCHECKSUM_DROP_FAILFRAMES, ENET_BROADCAST_FRAMES_PASS);
#else
    enet_init_status = enet_init(ENET_AUTO_NEGOTIATION, ENET_NO_AUTOCHECKSUM, ENET_BROADCAST_FRAMES_PASS);
#endif /* CHECKSUM_BY_HARDWARE */

}

#ifdef USE_ENET_INTERRUPT
/*!
    \brief      configures the nested vectored interrupt controller
    \param[in]  none
    \param[out] none
    \retval     none
*/
static void nvic_configuration(void)
{
//    nvic_vector_table_set(NVIC_VECTTAB_FLASH, 0x0);
//    nvic_priority_group_set(NVIC_PRIGROUP_PRE2_SUB2);
    nvic_irq_enable(ENET_IRQn, 0, 0);
}
#endif /* USE_ENET_INTERRUPT */

/*!
    \brief      configures the different GPIO ports
    \param[in]  none
    \param[out] none
    \retval     none
*/
static void enet_gpio_config(void)
{
//    rcu_periph_clock_enable(RCU_GPIOA);
//    rcu_periph_clock_enable(RCU_GPIOB);
//    rcu_periph_clock_enable(RCU_GPIOC);
//    rcu_periph_clock_enable(RCU_GPIOD);
//    rcu_periph_clock_enable(RCU_GPIOG);
//    rcu_periph_clock_enable(RCU_GPIOH);
//    rcu_periph_clock_enable(RCU_GPIOI);

//    gpio_af_set(GPIOA, GPIO_AF_0, GPIO_PIN_8);
//    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_8);
//    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_8);

    /* enable SYSCFG clock */
    rcu_periph_clock_enable(RCU_SYSCFG);

#ifdef MII_MODE

#ifdef PHY_CLOCK_MCO
    /* output HXTAL clock (25MHz) on CKOUT0 pin(PA8) to clock the PHY */
    rcu_ckout0_config(RCU_CKOUT0SRC_HXTAL, RCU_CKOUT0_DIV1);
#endif /* PHY_CLOCK_MCO */

    syscfg_enet_phy_interface_config(SYSCFG_ENET_PHY_MII);

#elif defined RMII_MODE
    /* choose DIV2 to get 50MHz from 200MHz on CKOUT0 pin (PA8) to clock the PHY */
    rcu_ckout0_config(RCU_CKOUT0SRC_PLLP, RCU_CKOUT0_DIV4);
    syscfg_enet_phy_interface_config(SYSCFG_ENET_PHY_RMII);

#endif /* MII_MODE */

#ifdef MII_MODE

    /* PA1: ETH_MII_RX_CLK */
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_1);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_1);

    /* PA2: ETH_MDIO */
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_2);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_2);

    /* PA7: ETH_MII_RX_DV */
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_7);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_7);

    gpio_af_set(GPIOA, GPIO_AF_11, GPIO_PIN_1);
    gpio_af_set(GPIOA, GPIO_AF_11, GPIO_PIN_2);
    gpio_af_set(GPIOA, GPIO_AF_11, GPIO_PIN_7);

    /* PG11: ETH_MII_TX_EN */
    gpio_mode_set(GPIOG, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_11);
    gpio_output_options_set(GPIOG, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_11);

    /* PG13: ETH_MII_TXD0 */
    gpio_mode_set(GPIOG, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_13);
    gpio_output_options_set(GPIOG, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_13);

    /* PG14: ETH_MII_TXD1 */
    gpio_mode_set(GPIOG, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_14);
    gpio_output_options_set(GPIOG, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_14);

    gpio_af_set(GPIOG, GPIO_AF_11, GPIO_PIN_11);
    gpio_af_set(GPIOG, GPIO_AF_11, GPIO_PIN_13);
    gpio_af_set(GPIOG, GPIO_AF_11, GPIO_PIN_14);

    /* PC1: ETH_MDC */
    gpio_mode_set(GPIOC, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_1);
    gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_1);

    /* PC2: ETH_MII_TXD2 */
    gpio_mode_set(GPIOC, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_2);
    gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_2);

    /* PC3: ETH_MII_TX_CLK */
    gpio_mode_set(GPIOC, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_3);
    gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_3);

    /* PC4: ETH_MII_RXD0 */
    gpio_mode_set(GPIOC, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_4);
    gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_4);

    /* PC5: ETH_MII_RXD1 */
    gpio_mode_set(GPIOC, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_5);
    gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_5);

    gpio_af_set(GPIOC, GPIO_AF_11, GPIO_PIN_1);
    gpio_af_set(GPIOC, GPIO_AF_11, GPIO_PIN_2);
    gpio_af_set(GPIOC, GPIO_AF_11, GPIO_PIN_3);
    gpio_af_set(GPIOC, GPIO_AF_11, GPIO_PIN_4);
    gpio_af_set(GPIOC, GPIO_AF_11, GPIO_PIN_5);

    /* PH2: ETH_MII_CRS */
    gpio_mode_set(GPIOH, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_2);
    gpio_output_options_set(GPIOH, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_2);

    /* PH3: ETH_MII_COL */
    gpio_mode_set(GPIOH, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_3);
    gpio_output_options_set(GPIOH, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_3);

    /* PH6: ETH_MII_RXD2 */
    gpio_mode_set(GPIOH, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_6);
    gpio_output_options_set(GPIOH, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_6);

    /* PH7: ETH_MII_RXD3 */
    gpio_mode_set(GPIOH, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_7);
    gpio_output_options_set(GPIOH, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_7);

    gpio_af_set(GPIOH, GPIO_AF_11, GPIO_PIN_2);
    gpio_af_set(GPIOH, GPIO_AF_11, GPIO_PIN_3);
    gpio_af_set(GPIOH, GPIO_AF_11, GPIO_PIN_6);
    gpio_af_set(GPIOH, GPIO_AF_11, GPIO_PIN_7);

    /* PI8: ETH_INT */
    gpio_mode_set(GPIOI, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_8);
    gpio_output_options_set(GPIOI, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_8);

    /* PI10: ETH_MII_RX_ER */
    gpio_mode_set(GPIOI, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_10);
    gpio_output_options_set(GPIOI, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_10);

    gpio_af_set(GPIOI, GPIO_AF_11, GPIO_PIN_8);
    gpio_af_set(GPIOI, GPIO_AF_11, GPIO_PIN_10);

    /* PB8: ETH_MII_TXD3 */
    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_8);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_8);

    gpio_af_set(GPIOB, GPIO_AF_11, GPIO_PIN_8);

#elif defined RMII_MODE

    /* PA1: ETH_RMII_REF_CLK */
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_1);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_1);

    /* PA2: ETH_MDIO */
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_2);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_2);

    /* PA7: ETH_RMII_CRS_DV */
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_7);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_7);

    gpio_af_set(GPIOA, GPIO_AF_11, GPIO_PIN_1);
    gpio_af_set(GPIOA, GPIO_AF_11, GPIO_PIN_2);
    gpio_af_set(GPIOA, GPIO_AF_11, GPIO_PIN_7);

    /* PB11: ETH_RMII_TX_EN */
    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_11);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_11);

    /* PB12: ETH_RMII_TXD0 */
    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_12);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_12);

    /* PB13: ETH_RMII_TXD1 */
    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_13);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_13);

    gpio_af_set(GPIOB, GPIO_AF_11, GPIO_PIN_11);
    gpio_af_set(GPIOB, GPIO_AF_11, GPIO_PIN_12);
    gpio_af_set(GPIOB, GPIO_AF_11, GPIO_PIN_13);

    /* PC1: ETH_MDC */
    gpio_mode_set(GPIOC, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_1);
    gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_1);

    /* PC4: ETH_RMII_RXD0 */
    gpio_mode_set(GPIOC, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_4);
    gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_4);

    /* PC5: ETH_RMII_RXD1 */
    gpio_mode_set(GPIOC, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_5);
    gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_5);

    gpio_af_set(GPIOC, GPIO_AF_11, GPIO_PIN_1);
    gpio_af_set(GPIOC, GPIO_AF_11, GPIO_PIN_4);
    gpio_af_set(GPIOC, GPIO_AF_11, GPIO_PIN_5);
#endif /* MII_MODE */

}

void ethnet_Init(void)
{
    enet_system_setup();
    lwip_stack_init();

//    ethnet_link_init();
}

void ethnet_Process(void)
{
    #ifndef USE_ENET_INTERRUPT
    /* check if any packet received */
    if(enet_rxframe_size_get()) {
        /* process received ethernet packet */
        lwip_pkt_handle();
    }
    #endif /* USE_ENET_INTERRUPT */

    /* handle periodic timers for LwIP */
    #ifdef TIMEOUT_CHECK_USE_LWIP
    sys_check_timeouts();

    #ifdef USE_DHCP
    lwip_dhcp_process_handle();
    #endif /* USE_DHCP */

    #else
    lwip_periodic_handle(g_localtime);
    #endif /* TIMEOUT_CHECK_USE_LWIP */

//    ethnet_link_process();

}

/*!
    \brief      updates the system local time
    \param[in]  none
    \param[out] none
    \retval     none
*/
void ethnet_time_update(void)
{
    g_localtime += SYSTEMTICK_PERIOD_MS;
}

/*!
    \brief      after the netif is fully configured, it will be called to initialize the function of telnet, client and udp
    \param[in]  netif: the struct used for lwIP network interface
    \param[out] none
    \retval     none
*/
void lwip_netif_status_callback(struct netif *netif)
{
    if((netif->flags & NETIF_FLAG_UP) != 0) {
        hello_gigadevice_init();
//        ethnet_tcp_server_init();
        ethnet_udp_server_init();

        DBG_ETHNET("lwip_netif_status_callback\r\n");
    }
}

static void ethnet_udp_server_recv(void *arg, struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *addr, u16_t port)
{
    ip_addr_t destaddr = *addr;

    if(p != NULL)
    {
        udp_sendto(pcb, p, &destaddr, port);
        /* end of processing, we free the pbuf */
        pbuf_free(p);
    }
}

void ethnet_udp_server_init(void)
{
  struct udp_pcb *pcb;

    /* create a new UDP control block */
    pcb = udp_new();

    if (pcb == NULL) {
        return;
    }

    /* assign to the new pcb a local IP address and a port number */
    udp_bind(pcb, IP_ADDR_ANY, g_stIPConfig.u16UdpPort);
    udp_recv(pcb, ethnet_udp_server_recv, NULL);
}

#ifdef USE_ENET_INTERRUPT
/*!
    \brief      this function handles ethernet interrupt request
    \param[in]  none
    \param[out] none
    \retval     none
*/
void ENET_IRQHandler(void)
{
    uint32_t reval;

    /* clear the enet DMA Rx interrupt pending bits */
    enet_interrupt_flag_clear(ENET_DMA_INT_FLAG_RS_CLR);
    enet_interrupt_flag_clear(ENET_DMA_INT_FLAG_NI_CLR);
    /* handles all the received frames */
    do {
        reval = enet_rxframe_size_get();

        if(reval > 1) {
            lwip_pkt_handle();
        }
    } while(reval != 0);

}
#endif /* USE_ENET_INTERRUPT */

