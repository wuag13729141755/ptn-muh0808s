/**
 * @file kapi.h
 *
 * @brief kernel api functions list of kernel lib \n
 *        this file will also be only header file of this software package when user do porting
 */

#ifndef __kapi_h
#define __kapi_h

#include "av_config.h"
#include "Gsv_hal.h"
#include "uapi_function_mapper.h"

/* define kapi as internal identification key word */
#define kapi

/* exported functions list */
#ifdef __cplusplus
extern "C" {
#endif

typedef void (*AvFpKeyCommand) (AvPort *port);
typedef void (*AvFpUartCommand) (AvPort *port);
typedef void (*AvFpIrdaCommand) (AvPort *port);
#define AvUserUartCmd       AvHookUartCmd
#define AvUserKeyCmd        AvHookKeyCmd
#define AvUserIrdaCmd       AvHookIrdaCmd
/* #define AvApiInitDevice(device) AvUapiInitDevice(device) */

#define AvApiHookBspFunctions   AvUapiHookBspFunctions
#define AvApiHookUserFunctions  AvKapiHookUserFunctions
#define AvApiHookBspFunctionsInit   AvUapiHookBspFunctionsInit
extern AvRet AvUapiHookBspFunctions(pin AvFpI2cRead i2cRd,
                                    pin AvFpI2cWrite i2cWr,
                                    pin AvFpUartSendByte uartTxB,
                                    pin AvFpUartGetByte uartRxB,
                                    pin AvFpGetMilliSecond getMs,
                                    pin AvFpGetKey getKey,
                                    pin AvFpGetIrda getIrda);
extern AvRet AvUapiHookBspFunctionsInit(pin AvFpI2cRead i2cRd, pin AvFpI2cWrite i2cWr);
kapi AvRet AvKapiHookUserFunctions(pin AvFpKeyCommand keyCmd, pin AvFpUartCommand uartCmd,pin AvFpIrdaCommand IrdaCmd);
kapi AvRet AvApiInit(void);
kapi AvRet AvApiAddDevice(AvDevice *device, AvDeviceType type, uint8 index,
                           void *specific, void *port,  void *extension);
kapi AvRet AvApiAddPort(AvDevice *device, pin AvPort *port, uint16 index, AvPortType type);
kapi AvRet AvApiInitDevice(AvDevice *device);
kapi AvRet AvApiInitFsm(pin AvPort *port);
kapi AvRet AvApiUpdate(void);
kapi AvRet AvApiTriggerEvent(pout AvEvent event);
kapi AvRet AvApiConnectPort(pin AvPort *FromPort, pin AvPort *ToPort, AvConnectType type);
kapi AvRet AvApiPortMemAllocate(pin AvPort *port);
kapi AvRet AvApiPortStart(void);

kapi AvRet AvKapiCecSendMessage(AvPort *port);
kapi AvRet AvKapiCecSetPhysicalAddr(AvPort *port);
kapi AvRet AvKapiCecSetLogicalAddr(AvPort *port);
kapi AvRet AvKapiArcEnable(AvPort *port, uint8 value);
#if  AvEnableDebugMessage
#define AvKapiOutputDebugMessage AvUapiOuputDbgMsg
#else
#define AvKapiOutputDebugMessage(...)
#endif
extern AvRet AvUapiOuputDbgMsg(pin schar *stringToFormat, ...);

/* 1. Report FSM State Related */
#define ReportHdcpFsm         PrintHdcpFsm
#define ReportTxRoutingFsm    PrintTxRoutingFsm
#define ReportPlugTxFsm       PrintPlugTxFsm
#define ReportPlugRxFsm       PrintPlugRxFsm
#define ReportReceiverFsm     PrintReceiverFsm
#define ReportCecFsm          PrintCecFsm
#define ReportCcaTxFsm        PrintCcaTxFsm
#define ReportCcaRxFsm        PrintCcaRxFsm

void PrintHdcpFsm(AvPort *Port, uint8 OldState);
void PrintTxRoutingFsm(AvPort *Port, uint8 OldState);
void PrintPlugTxFsm(AvPort *Port, uint8 OldState);
void PrintPlugRxFsm(AvPort *Port, uint8 OldState);
void PrintReceiverFsm(AvPort *Port, uint8 OldState);
void PrintCecFsm(AvPort *Port, uint8 OldState);
void PrintCcaTxFsm(AvPort *Port, uint8 OldState);
void PrintCcaRxFsm(AvPort *Port, uint8 OldState);

/* 2. FSM State Definition */
#define AvKapiFsmFunTxRoutingFsm      AvFsmFunTxRoutingFsm
#define AvKapiFsmFunPlugTxFsm         AvFsmFunPlugTxFsm
#define AvKapiFsmFunPlugRxFsm         AvFsmFunPlugRxFsm
#define AvKapiFsmFunReceiverFsm       AvFsmFunReceiverFsm
#define AvKapiFsmEnterFunTxRoutingFsm AvFsmEnterFunTxRoutingFsm
#define AvKapiFsmEnterFunPlugTxFsm    AvFsmEnterFunPlugTxFsm
#define AvKapiFsmEnterFunPlugRxFsm    AvFsmEnterFunPlugRxFsm
#define AvKapiFsmEnterFunReceiverFsm  AvFsmEnterFunReceiverFsm

#if AvEnableCecFeature
#define AvKapiFsmEnterFunCecFsm       AvFsmEnterFunCecFsm
#define AvKapiFsmFunCecFsm            AvFsmFunCecFsm
#else
    #ifdef COMPILER_C51_MODE
    #define AvKapiFsmEnterFunCecFsm   AvUselessFunction
    #define AvKapiFsmFunCecFsm        AvUselessFunction
    #else
    #define AvKapiFsmEnterFunCecFsm(...)
    #define AvKapiFsmFunCecFsm(...)
    #endif
#endif

#if AvEnableSimplifyHdcp
    #ifdef COMPILER_C51_MODE
    #define AvKapiFsmFunHdcpFsm       AvUselessFunction
    #define AvKapiFsmEnterFunHdcpFsm  AvUselessFunction
    #else
    #define AvKapiFsmFunHdcpFsm(...)
    #define AvKapiFsmEnterFunHdcpFsm(...)
    #endif
#else
#define AvKapiFsmFunHdcpFsm           AvFsmFunHdcpFsm
#define AvKapiFsmEnterFunHdcpFsm      AvFsmEnterFunHdcpFsm
#endif

#if AvEnableCcaFeature
#define AvKapiFsmFunCcaTxFsm          AvFsmFunCcaTxFsm
#define AvKapiFsmFunCcaRxFsm          AvFsmFunCcaRxFsm
#define AvKapiFsmEnterFunCcaTxFsm     AvFsmEnterFunCcaTxFsm
#define AvKapiFsmEnterFunCcaRxFsm     AvFsmEnterFunCcaRxFsm
#endif

#ifdef __cplusplus
}
#endif
#endif
