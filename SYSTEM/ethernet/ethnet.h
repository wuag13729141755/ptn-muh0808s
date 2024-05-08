/******************************************************
**Copyright (c) 2008 PTN Electronics Limited.All Rights Reserved
**FileName:         ethnet.h
**Writer:           wf8421
**Create Date:      2023-04-10
**Rewriter:         wf8421
**Rewrite Date:     2023-04-10
**Description:      ÍøÂçÇý¶¯
**History:
**Version:          V1.0.0
******************************************************/

#ifndef __ETHNET_H_
#define __ETHNET_H_

#define USE_ENET_INTERRUPT

/* MII and RMII mode selection */
#define RMII_MODE  // user have to provide the 50 MHz clock by soldering a 50 MHz oscillator
//#define MII_MODE

/* clock the PHY from external 25MHz crystal (only for MII mode) */
#ifdef  MII_MODE
#define PHY_CLOCK_MCO
#endif

#define DP83848                          0
#define LAN8700                          1
#define PHY_TYPE                         DP83848

#define PHY_ADDRESS                      ((uint16_t)1U)                         /*!< phy address determined by the hardware */

#if(PHY_TYPE == LAN8700)
#define PHY_SR                           31U                                    /*!< tranceiver status register */
#define PHY_SPEED_STATUS                 ((uint16_t)0x0004)                     /*!< configured information of speed: 10Mbit/s */
#define PHY_DUPLEX_STATUS                ((uint16_t)0x0010)                     /*!< configured information of duplex: full-duplex */
#elif(PHY_TYPE == DP83848)
#define PHY_SR                           16U                                    /*!< tranceiver status register */
#define PHY_SPEED_STATUS                 ((uint16_t)0x0002)                     /*!< configured information of speed: 10Mbit/s */
#define PHY_DUPLEX_STATUS                ((uint16_t)0x0004)                     /*!< configured information of duplex: full-duplex */
#endif /* PHY_TYPE */

extern __IO uint32_t g_localtime; /* for creating a time reference incremented by 10ms */

void ethnet_time_update(void);
void ethnet_Init(void);
void ethnet_Process(void);
void ethnet_tcp_server_init(void);
void ethnet_udp_server_init(void);
void ethnet_link_init(void);
void ethnet_link_process(void);
void snmp_sys_init(void);

#endif

