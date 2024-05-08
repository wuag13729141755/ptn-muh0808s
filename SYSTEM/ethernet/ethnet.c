/******************************************************
**Copyright (c) 2008 PTN Electronics Limited.All Rights Reserved
**FileName:         ethnet.c
**Writer:           wf8421
**Create Date:      2023-04-10
**Rewriter:         wf8421
**Rewrite Date:     2023-04-10
**Description:      网络驱动
**History:
**Version:          V1.0.0
******************************************************/
#include "includes.h"

#ifdef GD32F4XX
#include "gd32f4xx_enet.h"
#elif STM32F2XX_HD
#include "stm32f2x7_eth.h"
#endif
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
#if LWIP_SNMP
#include "lwip/apps/snmp.h"
#include "snmp_function.h"
#endif

#if 1
#include "tcp_function.h"
#include "udp_function.h"
#endif

#if 1
#define DBG_ETHNET          dbg_printf
#else
#define DBG_ETHNET(x)
#endif

#ifndef ETHNET_PRIORITY
#define ETHNET_PRIORITY         0
#endif
#ifndef ETHNET_SUBPRIORITY
#define ETHNET_SUBPRIORITY      0
#endif

#ifndef SYSTEMTICK_PERIOD_MS
#define SYSTEMTICK_PERIOD_MS    1
#endif

__IO uint32_t g_localtime = 0; /* for creating a time reference incremented by 10ms */

uint8_t g_u8EnetLinkSta = 0;        // 1 linked; 0 no ether link
uint8_t g_u8PreEnetLinkSta = 0;     // 1 linked; 0 no ether link

#ifdef GD32F4XX
static void ethnet_init(void)
{
#ifdef CHECKSUM_BY_HARDWARE
    enet_init(ENET_AUTO_NEGOTIATION, ENET_AUTOCHECKSUM_DROP_FAILFRAMES, ENET_BROADCAST_FRAMES_PASS);
#else
    enet_init(ENET_AUTO_NEGOTIATION, ENET_NO_AUTOCHECKSUM, ENET_BROADCAST_FRAMES_PASS);
#endif /* CHECKSUM_BY_HARDWARE */
}
/*!
    \brief      configures the ethernet interface
    \param[in]  none
    \param[out] none
    \retval     none
*/
static void ethnet_mac_dma_config(void)
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
        DBG_ETHNET("Ethernet init error\r\n");
        while(1) {}
    }

    /* configure the parameters which are usually less cared for enet initialization */
//  enet_initpara_config(HALFDUPLEX_OPTION, ENET_CARRIERSENSE_ENABLE|ENET_RECEIVEOWN_ENABLE|ENET_RETRYTRANSMISSION_DISABLE|ENET_BACKOFFLIMIT_10|ENET_DEFERRALCHECK_DISABLE);
//  enet_initpara_config(DMA_OPTION, ENET_FLUSH_RXFRAME_ENABLE|ENET_SECONDFRAME_OPT_ENABLE|ENET_NORMAL_DESCRIPTOR);

    ethnet_init();

}

/*!
    \brief      configures the different GPIO ports
    \param[in]  none
    \param[out] none
    \retval     none
*/
static void ethnet_gpio_config(void)
{

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
    /* 网络引脚设置 RMII接口    ------GD32F427VET6
      * ETH_MDIO -------------------------> PA2
      * ETH_MDC --------------------------> PC1
      * ETH_RMII_REF_CLK------------------> PA1
      * ETH_RMII_CRS_DV ------------------> PA7
      * ETH_RMII_RXD0 --------------------> PC4
      * ETH_RMII_RXD1 --------------------> PC5
      * ETH_RMII_TX_EN -------------------> PB11
      * ETH_RMII_TXD0 --------------------> PB12
      * ETH_RMII_TXD1 --------------------> PB13
      * ETH_RESET-------------------------> PA15 or 外部定
      */

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

    #if 0
    /* PA15: ETH_RESET */
    gpio_mode_set(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_15);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_15);

    gpio_bit_write(GPIOA,GPIO_PIN_15,RESET);     /* 硬件复位 */
    delay_ms(50);
    gpio_bit_write(GPIOA,GPIO_PIN_15,SET);      /* 复位结束 */
    #endif
#endif /* MII_MODE */

}

void ethnet_Config(void)
{
//    delay_ms(2000);
    /* configure the GPIO ports for ethernet pins */
#ifdef USE_ENET_INTERRUPT
    nvic_irq_enable(ENET_IRQn, ETHNET_PRIORITY, ETHNET_SUBPRIORITY);
#endif
    ethnet_gpio_config();

    /* configure the ethernet MAC/DMA */
    ethnet_mac_dma_config();

    #ifdef USE_ENET_INTERRUPT
    enet_interrupt_enable(ENET_DMA_INT_NIE);
    enet_interrupt_enable(ENET_DMA_INT_RIE);
    #endif /* USE_ENET_INTERRUPT */

    #ifdef SELECT_DESCRIPTORS_ENHANCED_MODE
    enet_desc_select_enhanced_mode();
    #endif /* SELECT_DESCRIPTORS_ENHANCED_MODE */

}
#elif defined(STM32F2XX_HD)
static void ethnet_init(void)
{
    ETH_InitTypeDef ETH_InitStructure;
    /* ETHERNET Configuration --------------------------------------------------*/
    /* Call ETH_StructInit if you don't like to configure all ETH_InitStructure parameter */
    ETH_StructInit(&ETH_InitStructure);

    /* Fill ETH_InitStructure parametrs */
    /*------------------------   MAC   -----------------------------------*/
    ETH_InitStructure.ETH_AutoNegotiation = ETH_AutoNegotiation_Enable;
    //ETH_InitStructure.ETH_AutoNegotiation = ETH_AutoNegotiation_Disable;
    //  ETH_InitStructure.ETH_Speed = ETH_Speed_10M;
    //  ETH_InitStructure.ETH_Mode = ETH_Mode_FullDuplex;

    ETH_InitStructure.ETH_LoopbackMode = ETH_LoopbackMode_Disable;
    ETH_InitStructure.ETH_RetryTransmission = ETH_RetryTransmission_Disable;
    ETH_InitStructure.ETH_AutomaticPadCRCStrip = ETH_AutomaticPadCRCStrip_Disable;
    ETH_InitStructure.ETH_ReceiveAll = ETH_ReceiveAll_Disable;
    ETH_InitStructure.ETH_BroadcastFramesReception = ETH_BroadcastFramesReception_Enable;
    ETH_InitStructure.ETH_PromiscuousMode = ETH_PromiscuousMode_Disable;
    ETH_InitStructure.ETH_MulticastFramesFilter = ETH_MulticastFramesFilter_Perfect;
    ETH_InitStructure.ETH_UnicastFramesFilter = ETH_UnicastFramesFilter_Perfect;
#ifdef CHECKSUM_BY_HARDWARE
    ETH_InitStructure.ETH_ChecksumOffload = ETH_ChecksumOffload_Enable;
#endif

    /*------------------------   DMA   -----------------------------------*/

    /* When we use the Checksum offload feature, we need to enable the Store and Forward mode:
    the store and forward guarantee that a whole frame is stored in the FIFO, so the MAC can insert/verify the checksum,
    if the checksum is OK the DMA can handle the frame otherwise the frame is dropped */
    ETH_InitStructure.ETH_DropTCPIPChecksumErrorFrame = ETH_DropTCPIPChecksumErrorFrame_Enable;
    ETH_InitStructure.ETH_ReceiveStoreForward = ETH_ReceiveStoreForward_Enable;
    ETH_InitStructure.ETH_TransmitStoreForward = ETH_TransmitStoreForward_Enable;

    ETH_InitStructure.ETH_ForwardErrorFrames = ETH_ForwardErrorFrames_Disable;
    ETH_InitStructure.ETH_ForwardUndersizedGoodFrames = ETH_ForwardUndersizedGoodFrames_Disable;
    ETH_InitStructure.ETH_SecondFrameOperate = ETH_SecondFrameOperate_Enable;
    ETH_InitStructure.ETH_AddressAlignedBeats = ETH_AddressAlignedBeats_Enable;
    ETH_InitStructure.ETH_FixedBurst = ETH_FixedBurst_Enable;
    ETH_InitStructure.ETH_RxDMABurstLength = ETH_RxDMABurstLength_32Beat;
    ETH_InitStructure.ETH_TxDMABurstLength = ETH_TxDMABurstLength_32Beat;
    ETH_InitStructure.ETH_DMAArbitration = ETH_DMAArbitration_RoundRobin_RxTx_2_1;

    /* Configure Ethernet */
    ETH_Init(&ETH_InitStructure, PHY_ADDRESS);
}

static void ethnet_mac_dma_config(void)
{
    ETH_InitTypeDef ETH_InitStructure;

    /* Enable ETHERNET clock  */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_ETH_MAC | RCC_AHB1Periph_ETH_MAC_Tx |
                            RCC_AHB1Periph_ETH_MAC_Rx, ENABLE);

    /* Reset ETHERNET on AHB Bus */
    ETH_DeInit();

    /* Software reset */
    ETH_SoftwareReset();

    /* Wait for software reset */
    while (ETH_GetSoftwareResetStatus() == SET);

    ethnet_init();

    /* Enable the Ethernet Rx Interrupt */
    ETH_DMAITConfig(ETH_DMA_IT_NIS | ETH_DMA_IT_R, ENABLE);
}

static void ethnet_nvic_config(void)
{
    NVIC_InitTypeDef   NVIC_InitStructure;

    /* 2 bit for pre-emption priority, 2 bits for subpriority */
//    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    /* Enable the Ethernet global Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = ETH_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/*!
    \brief      configures the different GPIO ports
    \param[in]  none
    \param[out] none
    \retval     none
*/
static void ethnet_gpio_config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
      volatile int i;
    /* Enable GPIOs clocks */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB |
                           RCC_AHB1Periph_GPIOC, ENABLE);

    /* Enable SYSCFG clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

    SYSCFG_ETH_MediaInterfaceConfig(SYSCFG_ETH_MediaInterface_RMII);

    /* 网络引脚设置 RMII接口    ------STM32F207VET6
      * ETH_MDIO -------------------------> PA2
      * ETH_MDC --------------------------> PC1
      * ETH_RMII_REF_CLK------------------> PA1
      * ETH_RMII_CRS_DV ------------------> PA7
      * ETH_RMII_RXD0 --------------------> PC4
      * ETH_RMII_RXD1 --------------------> PC5
      * ETH_RMII_TX_EN -------------------> PB11
      * ETH_RMII_TXD0 --------------------> PB12
      * ETH_RMII_TXD1 --------------------> PB13
      * ETH_RESET-------------------------> PA15 or 外部定
      */
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_7;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_ETH);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_ETH);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_ETH);


    /* Configure PC1, PC4 and PC5 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource1, GPIO_AF_ETH);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource4, GPIO_AF_ETH);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource5, GPIO_AF_ETH);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_ETH);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource12, GPIO_AF_ETH);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_ETH);
}

void ethnet_Config(void)
{
//    delay_ms(2000);
    /* configure the GPIO ports for ethernet pins */
    ethnet_gpio_config();
    ethnet_nvic_config();

    /* configure the ethernet MAC/DMA */
    ethnet_mac_dma_config();
}
#else //

#define ETH_INTERFACE ETH_INTERFACE_RMII // 0:RMII, 1:MII
#define REMAP_MODE    0

uint32_t InitPHY(ETH_InitType* ETH_InitStruct)
{
    unsigned short RegValue = 0;
    uint32_t timeout;
    unsigned short PHY_Address;
    uint16_t phy_status_reg, phy_speed_bit, phy_duplex_bit;

    // search PHY address
    for (PHY_Address = 0; PHY_Address < 32; ++PHY_Address)
    {
        uint16_t val = ETH_ReadPhyRegister(PHY_Address, 3); // PHY ID register
        if (val != 0x0000 && val != 0xFFFF)
        {
            DBG_ETHNET("PHY addr: %d\n", PHY_Address);
            break;
        }
    }
    if (PHY_Address > 31)
    {
        DBG_ETHNET("cannot comm with PHY!\n");
        while (1);
    }

    // SetReset
    if (!(ETH_WritePhyRegister(PHY_Address, PHY_BCR, (PHY_RESET|PHY_AUTONEGO))))
    {
        return ETH_ERROR;
    }
    ETH_ReadPhyRegister(PHY_Address, PHY_BCR);

    switch (PHY_TYPE)
    {
    case PHY_TYPE_LAN8710:
        phy_status_reg = 0x1f;
        phy_speed_bit  = 0x0004;
        phy_duplex_bit = 0x0010;
        break;
    case PHY_TYPE_DP83848:
        phy_status_reg = 0x10;
        phy_speed_bit  = 0x0002;
        phy_duplex_bit = 0x0004;
        // we can configure RMII mode through PHY register for DP83848
        if (ETH_INTERFACE == ETH_INTERFACE_RMII)
        {
            ETH_WritePhyRegister(PHY_Address, 0x17, 0x21);
        }
        break;
    case PHY_TYPE_KSZ8041:
        phy_status_reg = 0x1f;
        phy_speed_bit  = 0x0004;
        phy_duplex_bit = 0x0010;
        break;
    case PHY_TYPE_DM9161:
        phy_status_reg = 0x11;
        phy_speed_bit  = 0x00;
        phy_duplex_bit = 0x00;
        break;
    default:
        DBG_ETHNET("unsupport with PHY!\n");
        while(1);
    }

    if (ETH_InitStruct->AutoNegotiation != ETH_AUTONEG_DISABLE)
    {
#if 1
        // We wait for linked satus
        timeout = 0;
        do
        {
            timeout++;
        } while (!(ETH_ReadPhyRegister(PHY_Address, PHY_BSR) & PHY_LINKED_STATUS) && (timeout < PHY_READ_TO));
#endif
        if (!(ETH_WritePhyRegister(PHY_Address, PHY_BCR, PHY_AUTONEGO)))
        {
            return ETH_ERROR;
        }
        ETH_ReadPhyRegister(PHY_Address, PHY_BCR);
        // Enable Auto-Negotiation
        timeout = 0;
        do
        {
            timeout++;
        } while (!(ETH_ReadPhyRegister(PHY_Address, PHY_BSR) & PHY_AUTONEGO_COMPLETE)
                 && (timeout < (uint32_t)PHY_READ_TO));

        // Read Phy Status that is the result of the autonegotiation//786d
        RegValue = ETH_ReadPhyRegister(PHY_Address, phy_status_reg);
        if (PHY_TYPE != PHY_TYPE_DM9161)
        {
            if ((RegValue & phy_duplex_bit) != (uint32_t)RESET)
            {
                ETH_InitStruct->DuplexMode = ETH_DUPLEX_MODE_FULL;
            }
            else
            {
                ETH_InitStruct->DuplexMode = ETH_DUPLEX_MODE_HALF;
            }

            if (RegValue & phy_speed_bit)
            {
                ETH_InitStruct->SpeedMode = ETH_SPEED_MODE_10M;
            }

            else
            {
                ETH_InitStruct->SpeedMode = ETH_SPEED_MODE_100M;
            }
        }
        else
        {
            if (RegValue & 0x8000) // bit15
            {
                ETH_InitStruct->DuplexMode = ETH_DUPLEX_MODE_FULL;
                ETH_InitStruct->SpeedMode  = ETH_SPEED_MODE_100M;
            }
            else if (RegValue & 0x4000) // bit14
            {
                ETH_InitStruct->DuplexMode = ETH_DUPLEX_MODE_HALF;
                ETH_InitStruct->SpeedMode  = ETH_SPEED_MODE_100M;
            }
            else if (RegValue & 0x2000) // bit13
            {
                ETH_InitStruct->DuplexMode = ETH_DUPLEX_MODE_FULL;
                ETH_InitStruct->SpeedMode  = ETH_SPEED_MODE_10M;
            }
            else if (RegValue & 0x1000) // bit12
            {
                ETH_InitStruct->DuplexMode = ETH_DUPLEX_MODE_HALF;
                ETH_InitStruct->SpeedMode  = ETH_SPEED_MODE_10M;
            }
            else
            {
                while (1)
                    ;
            }
        }
    }
    else
    {
        RegValue = 0;
        if (ETH_InitStruct->DuplexMode == ETH_DUPLEX_MODE_FULL)
        {
            RegValue |= 0x100;
        }
        if (ETH_InitStruct->SpeedMode == ETH_SPEED_MODE_100M)
        {
            RegValue |= 0x2000;
        }
        if (!ETH_WritePhyRegister(PHY_Address, PHY_BCR, RegValue))
        {
            return ETH_ERROR;
        }

        // We wait for linked satus
        timeout = 0;
        do
        {
            timeout++;
        } while (!(ETH_ReadPhyRegister(PHY_Address, PHY_BSR) & PHY_LINKED_STATUS) && (timeout < PHY_READ_TO));
    }

    return ETH_SUCCESS;
}

static void ethnet_init(void)
{
    ETH_InitType ETH_InitStructure;

    ETH_InitStruct(&ETH_InitStructure);

    // set ETH MAC parameters
    ETH_InitStructure.AutoNegotiation          = ETH_AUTONEG_ENABLE;
    ETH_InitStructure.LoopbackMode             = ETH_LOOPBACK_MODE_DISABLE;
    ETH_InitStructure.RetryTransmission        = ETH_RETRY_TRANSMISSION_DISABLE;
    ETH_InitStructure.AutomaticPadCRCStrip     = ETH_AUTO_PAD_CRC_STRIP_DISABLE;
    ETH_InitStructure.RxAll                    = ETH_RX_ALL_DISABLE;
    ETH_InitStructure.BroadcastFramesReception = ETH_BROADCAST_FRAMES_RECEPTION_ENABLE;
    ETH_InitStructure.PromiscuousMode          = ETH_PROMISCUOUS_MODE_DISABLE;
    ETH_InitStructure.MulticastFramesFilter    = ETH_MULTICAST_FRAMES_FILTER_NONE;
    ETH_InitStructure.UnicastFramesFilter      = ETH_UNICAST_FRAMES_FILTER_PERFECT;
#ifdef CHECKSUM_BY_HARDWARE
    ETH_InitStructure.ChecksumOffload = ETH_CHECKSUM_OFFLOAD_ENABLE;
#endif
    ETH_InitStructure.DropTCPIPChecksumErrorFrame = ETH_DROP_TCPIP_CHECKSUM_ERROR_FRAME_ENABLE;
    ETH_InitStructure.RxStoreForward              = ETH_RX_STORE_FORWARD_ENABLE;
    ETH_InitStructure.TxStoreForward              = ETH_TX_STORE_FORWARD_ENABLE;

    ETH_InitStructure.ForwardErrorFrames          = ETH_FORWARD_ERROR_FRAMES_DISABLE;
    ETH_InitStructure.ForwardUndersizedGoodFrames = ETH_FORWARD_UNDERSIZED_GOOD_FRAMES_DISABLE;
    ETH_InitStructure.SecondFrameOperate          = ETH_SECOND_FRAME_OPERATE_ENABLE;
    ETH_InitStructure.AddrAlignedBeats            = ETH_ADDR_ALIGNED_BEATS_ENABLE;
    ETH_InitStructure.FixedBurst                  = ETH_FIXED_BURST_ENABLE;
    ETH_InitStructure.RxDMABurstLen               = ETH_RX_DMA_BURST_LEN_32BEAT;
    ETH_InitStructure.TxDMABurstLen               = ETH_TX_DMA_BURST_LEN_32BEAT;
    ETH_InitStructure.DMAArbitration              = ETH_DMA_ARBITRATION_ROUND_ROBIN_RXTX_2_1;

    ETH_Init(&ETH_InitStructure, InitPHY);
}

static void ethnet_mac_dma_config(void)
{
//    uint8_t rval;
    ETH_InitType ETH_InitStructure;

    ETH_DeInit();        // Reset AHB ETH clock
    ETH_SoftwareReset(); // software reset ETH module
    while (ETH_GetSoftwareResetStatus() == SET); // wait for reset over

    ethnet_init();


//    if (rval) // init succeed
    {
        ETH_EnableDmaInt(ETH_DMA_INT_NIS | ETH_DMA_INT_RX, ENABLE); // Enable rx interrupt
    }

#if 1
    ETH_SetMacAddr(ETH_MAC_ADDR0, (uint8_t*)mac_address);
    ETH_ConfigDmaTxDescInChainMode(g_TxDesc, g_TxBuf, ETH_TX_BUF_SIZE, ETH_TXBUFNB);
    ETH_ConfigDmaRxDescInChainMode(g_RxDesc, g_RxBuf, ETH_TX_BUF_SIZE, ETH_TXBUFNB);

#ifdef CHECKSUM_BY_HARDWARE
    for (int i = 0; i < ETH_TXBUFNB; i++)
    {
        ETH_ConfigDmaTxDescChecksumInsertion(&g_TxDesc[i], ETH_DMA_TX_DESC_CHECKSUM_TCPUDPICMP_FULL);
    }
#endif
#endif

//    return !rval;
}

static void ethnet_nvic_config(void)
{
    /* Enable the Ethernet global Interrupt */
    NVIC_InitType NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel                   = ETH_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/*!
    \brief      configures the different GPIO ports
    \param[in]  none
    \param[out] none
    \retval     none
*/
static void ethnet_gpio_config(void)
{
    ETH_ConfigGpio(ETH_INTERFACE,REMAP_MODE);
}

void ethnet_Config(void)
{
    RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_AFIO | RCC_APB2_PERIPH_GPIOA | RCC_APB2_PERIPH_GPIOB | RCC_APB2_PERIPH_GPIOC
                                | RCC_APB2_PERIPH_GPIOD | RCC_APB2_PERIPH_GPIOE | RCC_APB2_PERIPH_GPIOF
                                | RCC_APB2_PERIPH_GPIOG,
                            ENABLE);

    // MII/RMII should be configured before enabling ETH clock
    GPIO_ETH_ConfigMediaInterface(ETH_INTERFACE == ETH_INTERFACE_MII ? GPIO_ETH_MII_CFG : GPIO_ETH_RMII_CFG);

    RCC_EnableAHBPeriphClk(RCC_AHB_PERIPH_ETHMAC, ENABLE);

    /* configure the GPIO ports for ethernet pins */
    ethnet_gpio_config();
    ethnet_nvic_config();

    /* configure the ethernet MAC/DMA */
    ethnet_mac_dma_config();
}

#endif

/*!
    \brief      after the netif is fully configured, it will be called to initialize the function of telnet, client and udp
    \param[in]  netif: the struct used for lwIP network interface
    \param[out] none
    \retval     none
*/
void lwip_netif_status_callback(struct netif *netif)
{
    if((netif->flags & NETIF_FLAG_UP) != 0) {
//        hello_gigadevice_init();
        tcp_server_init();
//        tcp_client_init();

        udp_server_init();

        DBG_ETHNET("==========================\r\n");
        DBG_ETHNET("lwip_netif_status_callback\r\n");
        DBG_ETHNET("==========================\r\n");
    }
}

#ifdef USE_ENET_INTERRUPT
/*!
    \brief      this function handles ethernet interrupt request
    \param[in]  none
    \param[out] none
    \retval     none
*/
#if GD32F4XX
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
#elif defined(STM32F2XX_HD)
void ETH_IRQHandler(void)
{
  /* Handles all the received frames */
    /* check if any packet received */
    while(ETH_CheckFrameReceived())
    {
      /* process received ethernet packet */
      lwip_pkt_handle();
    }
	/* Clear the Eth DMA Rx IT pending bits */
	ETH_DMAClearITPendingBit(ETH_DMA_IT_R);
	ETH_DMAClearITPendingBit(ETH_DMA_IT_NIS);
}
#else //n32g45x
void ETH_IRQHandler(void)
{
  /* Handles all the received frames */
    /* check if any packet received */
    while(ETH_GetRxPacketSize())
    {
      /* process received ethernet packet */
      lwip_pkt_handle();
    }
	/* Clear the Eth DMA Rx IT pending bits */
	ETH_ClrDmaIntPendingBit(ETH_DMA_INT_RX);
	ETH_ClrDmaIntPendingBit(ETH_DMA_INT_NIS);
}
#endif
#endif /* USE_ENET_INTERRUPT */

void snmp_sys_init(void)
{
#if LWIP_SNMP
    snmp_function_init();
#endif
}

void ethnet_Init(void)
{
    ethnet_Config();
#if (defined(GD32F4XX)||defined(STM32F2XX_HD))
    lwip_stack_init();
#endif

#if LWIP_SNMP
    snmp_sys_init();
#endif
    ethnet_link_init();
}

void ethnet_Process(void)
{
#ifndef USE_ENET_INTERRUPT
    #ifdef GD32F4XX
    /* check if any packet received */
    if(enet_rxframe_size_get()) {
        /* process received ethernet packet */
        lwip_pkt_handle();
    }
    #endif
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

    ethnet_link_process();

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

uint8_t ethnet_link_status(void)
{
#if GD32F4XX
    uint16_t l_u16PhyVal;
    enet_phy_write_read(ENET_PHY_READ, PHY_ADDRESS, PHY_REG_BSR, &l_u16PhyVal);
    return (l_u16PhyVal&PHY_LINKED_STATUS)?1:0;
#else
	return ((ETH_ReadPHYRegister(PHY_ADDRESS, PHY_BSR) & PHY_Linked_Status)==0)?0:1;
#endif
}

void ethnet_link_init(void)
{
	g_u8EnetLinkSta = ethnet_link_status();
}

void ethnet_link_process(void)
{
    static u16 sl_u16LoopCnt = 0;

    if(++sl_u16LoopCnt>50)
    {
        sl_u16LoopCnt = 0;
        g_u8EnetLinkSta = ethnet_link_status();
        if(g_u8PreEnetLinkSta!=g_u8EnetLinkSta)
        {
            if(g_u8EnetLinkSta)
            {
                DBG_ETHNET("Ethernet link\r\n");
                DBG_ETHNET("enet_init\r\n");
                ethnet_init();
//                SYS_SoftReset();
            }
            else
            {
                DBG_ETHNET("Ethernet unlink\r\n");
            }
            g_u8PreEnetLinkSta=g_u8EnetLinkSta;
        }
//        snmp_trap_test();
    }
}

