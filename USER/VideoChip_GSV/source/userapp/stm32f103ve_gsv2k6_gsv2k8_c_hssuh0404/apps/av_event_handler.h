/**
 * @file av_event_handler.h
 *
 * @brief av event handler to handle the events for customers use
 */

#ifndef __av_event_handler_h
#define __av_event_handler_h

#include "av_config.h"
#include "uapi_function_mapper.h"
#include "Gsv_kapi.h"
#include "av_edid_manage.h"
//#include "av_key_cmd.h"

typedef void (*vpstAvPortFuncPoint)(AvPort *);
typedef void (*vpstu8AvPortTxEdidPoint)(AvPort *,u8*,u16);

/*
  Connect Style:
  1 : bypass, Rx -> Tx
  2 : Rx -> Color -> Tx
  3 : Rx -> Scaler -> Tx

  4 : Rx -> Scaler -> Dsc -> Tx
  5 : Rx -> Color -> Dsc -> Tx
  6 : Rx -> Dsc -> Tx
  7 : Rx -> Dsc -> Color -> Tx
  8 : Rx -> Dsc -> Scaler -> Tx
 */
typedef enum{
    ROUTE_NO_CONNECT  = 0,
    ROUTE_1_R_T       = 1,
    ROUTE_2_R_C_T     = 2,
    ROUTE_3_R_S_T     = 3,
    // gsv201x gsv101x
    ROUTE_4_R_V       = 4,
    ROUTE_5_R_C_V     = 5,
    ROUTE_6_R_S_V     = 6,
    ROUTE_7_V_T       = 7,
    ROUTE_8_V_C_T     = 8,
    ROUTE_9_V_S_T     = 9,

    // gsv5100 gsv5010
    ROUTE_4_R_S_D_T   = 10,
    ROUTE_5_R_C_D_T   = 11,
    ROUTE_6_R_D_T     = 12,
    ROUTE_7_R_D_C_T   = 13,
    ROUTE_8_R_D_S_T   = 14

} RouteStat;
extern uint8 *g_pu8GsvSysEdid;
extern vpstAvPortFuncPoint pvpstFuncGsvUserEdidFunction;
extern vpstu8AvPortTxEdidPoint pvpstu8FuncGsvUserEdidSaveFunction;
#if AvGsv2k11CopyTxEdid
extern uint8 g_u8DisplayEdid[AvGsv2k11DeviceNumber][256];
#endif

AvRet AvHandleEvent(AvPort *port, AvEvent event, uint8 *wparam, uint8 *pparam);
AvRet AvPortConnectUpdate(AvDevice *Device);
#if AvGsv2k8VideoRgb_422_444To420
void AvUartCmdToSendInputStatus(uint8 u8Dev,
                                        uint8 u8PortId,
                                        uint8 u8ColorData,
                                        uint8 u8ColorSpace,
                                        uint8 u8Vic);
#endif

#endif
