/******************************************************
**Copyright (c) 2008 PTN Electronics Limited.All Rights Reserved
**FileName:         tcp_function.c
**Writer:           wf8421
**Create Date:      2023-04-20
**Rewriter:         wf8421
**Rewrite Date:     2023-04-20
**Description:      TCP server/client function
**History:
**Version:          V1.0.0
******************************************************/
#include "lwip/opt.h"

#if LWIP_TCP
#include "tcp_function.h"
#include "lwip/tcp.h"
#include "lwip/memp.h"
#include <string.h>
#include <stdio.h>
#ifdef GD32F4XX
#include "gd32f4xx.h"
#elif STM32F2XX_HD
#include "stm32f2xx.h"
#endif
#include "lwipconf.h"

vvFuncPoint pvvFuncTcpServerCallBack = NULL;

#if 1
#define DBG_TCP          dbg_printf
#else
#define DBG_TCP(x)
#endif

#define MAX_BUF_SIZE    50

struct recev_packet {
    int length;
    char bytes[MAX_BUF_SIZE];
};

static err_t tcp_client_connected(void *arg, struct tcp_pcb *pcb, err_t err);
static err_t tcp_client_recv(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err);

/*!
    \brief      called when a data is received on the tcp connection
    \param[in]  arg: the user argument
    \param[in]  pcb: the tcp_pcb that has received the data
    \param[in]  p: the packet buffer
    \param[in]  err: the error value linked with the received data
    \param[out] none
    \retval     err_t: error value
*/
static err_t tcp_client_recv(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err)
{
    struct pbuf *q;
    struct recev_packet *recev_packet = (struct recev_packet *)arg;
    int buf_full;
    char *c;
    int i;

    /* we perform here any necessary processing on the pbuf */
    if(p != NULL) {
        /* we call this function to tell the LwIp that we have processed the data */
        /* this lets the stack advertise a larger window, so more data can be received*/
        tcp_recved(pcb, p->tot_len);

        /* check the name if NULL, no data passed, return withh illegal argument error */
        if(!recev_packet) {
            pbuf_free(p);
            return ERR_ARG;
        }

        buf_full = 0;

        for(q = p; q != NULL; q = q->next) {

            c = q->payload;
            for(i = 0; i < q->len && !buf_full; i++) {
                /* if the received data size is larger than the size we want to get */
                if(recev_packet->length < MAX_BUF_SIZE) {
                    recev_packet->bytes[recev_packet->length] = c[i];
                    recev_packet->length++;
                } else {
                    buf_full = 1;
                }
            }
        }

        /* send out the message */
        tcp_write(pcb, recev_packet->bytes, recev_packet->length, 1);
        recev_packet->length = 0;

        pbuf_free(p);

    } else if(ERR_OK == err) {

        mem_free(recev_packet);
        return tcp_close(pcb);
    }

    return ERR_OK;
}

/*!
    \brief      this function when the connection is established
    \param[in]  arg: user supplied argument
    \param[in]  pcb: the tcp_pcb which accepted the connection
    \param[in]  err: error value
    \param[out] none
    \retval     err_t: error value
*/
static err_t tcp_client_connected(void *arg, struct tcp_pcb *pcb, err_t err)
{
    tcp_arg(pcb, mem_calloc(sizeof(struct recev_packet), 1));
    /* configure LwIP to use our call back functions */
    tcp_recv(pcb, tcp_client_recv);

    return ERR_OK;
}

/*!
    \brief      initialize the tcp_client application
    \param[in]  none
    \param[out] none
    \retval     none
*/

void tcp_client_init(void)
{
    struct tcp_pcb *pcb;
    ip_addr_t ipaddr;

    IP4_ADDR(&ipaddr, 192, 168, 0, 100);

    /* create a new TCP control block  */
    pcb = tcp_new();

    /* assign to the new pcb a local IP address and a port number */
    /* using IP_ADDR_ANY allow the pcb to be used by any local interface */
    if(ERR_USE != tcp_bind(pcb, IP_ADDR_ANY, 10260)) {
        /* Specify the function to be called when a connection is established */
        tcp_connect(pcb, &ipaddr, 10260, tcp_client_connected);
    } else {
        //printf("connect is still alive \r\n ");
        memp_free(MEMP_TCP_PCB, pcb);
    }
}

static err_t tcp_server_recv(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err)
{
    if(0)
    {}
    else
    {
        if (p != NULL) {
            /* Inform TCP that we have taken the data. */
            tcp_recved(pcb, p->tot_len);
            DBG_TCP("acpet data len: %d\r\n",p->tot_len);
            tcp_write(pcb, p->payload, p->tot_len, 1);

            if(pvvFuncTcpServerCallBack != NULL)
            {
                pvvFuncTcpServerCallBack();
            }
            pbuf_free(p);
        }
        else
        {
            tcp_close(pcb);
        }
    }
    return ERR_OK;
}

static err_t tcp_server_accept(void *arg, struct tcp_pcb *pcb, err_t err)
{
    /* Set priority */
    tcp_setprio(pcb, TCP_PRIO_MIN);
    tcp_recv(pcb, tcp_server_recv);
    return ERR_OK;
}

void tcp_server_init(void)
{
    struct tcp_pcb *pcb;

    pcb = tcp_new();

    if(pcb == NULL)
    {
        DBG_TCP("tcp_server_init error\r\n");
        return;
    }
    tcp_bind(pcb, IP_ADDR_ANY, g_stIPConfig.u16TcpPort);
    pcb = tcp_listen(pcb);
    /* initialize callback arg and accept callback */
//    tcp_arg(pcb, pcb);
    tcp_accept(pcb, tcp_server_accept);
}
#endif

