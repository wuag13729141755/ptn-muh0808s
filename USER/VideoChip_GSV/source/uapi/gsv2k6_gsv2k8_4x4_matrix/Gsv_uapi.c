/**
 * @file uapi.c
 *
 * @brief low level ports related universal api functions
 */

#include "includes.h"

#if _ENABLE_CHIP_GSV_CHIP_SERIES
#include <stdarg.h>
#include <stdio.h>
#include "Gsv_uapi.h"

/* firmware static buffer */
static uint8 AvGlobalStaticBuffer[AvGlobalStaticBufferSize];

/* hal functions variables */
AvFpI2cRead        AvHookI2cRead;
AvFpI2cWrite       AvHookI2cWrite;
AvFpUartSendByte   AvHookUartTxByte;
AvFpUartGetByte    AvHookUartRxByte;
AvFpGetMilliSecond AvHookGetMilliSecond;
AvFpGetKey         AvHookGetKey;
AvFpGetIrda        AvHookGetIrda;

/* local macro */
#define SwitchDeviceHeader switch(device->type)
#define SwitchPortHeader   switch(port->device->type)

#if GSV2K6
#define CallDevice0Uapi(uapiName) \
        case Gsv2k6: \
            ret = CallUapi(Gsv2k6, uapiName); \
            break;
#else
#define CallDevice0Uapi(uapiName)
#endif

#if GSV2K8
#define CallDevice1Uapi(uapiName) \
        case Gsv2k8: \
            ret = CallUapi(Gsv2k8, uapiName); \
            break;
#else
#define CallDevice1Uapi(uapiName)
#endif

#if GSV2K2
#define CallDevice2Uapi(uapiName) \
        case Gsv2k2: \
            ret = CallUapi(Gsv2k2, uapiName); \
            break;
#else
#define CallDevice2Uapi(uapiName)
#endif

#if GSV2K1
#define CallDevice3Uapi(uapiName) \
        case Gsv2k1: \
            ret = CallUapi(Gsv2k1, uapiName); \
            break;
#else
#define CallDevice3Uapi(uapiName)
#endif

#if GSV2K11
#define CallDevice4Uapi(uapiName) \
        case Gsv2k11: \
            ret = CallUapi(Gsv2k11, uapiName); \
            break;
#else
#define CallDevice4Uapi(uapiName)
#endif

#if GSV5K1
#define CallDevice5Uapi(uapiName) \
        case Gsv5k1: \
            ret = CallUapi(Gsv5k1, uapiName); \
            break;
#else
#define CallDevice5Uapi(uapiName)
#endif

#define CallDeviceNUapi(uapiName) \
        default: \
            ret = AvNotSupport; \
            break;

#define CallDeviceUapi0(uapiName)  \
        SwitchDeviceHeader  \
        { \
        CallDevice0Uapi(uapiName) \
        CallDevice1Uapi(uapiName) \
        CallDevice2Uapi(uapiName) \
        CallDevice3Uapi(uapiName) \
        CallDevice4Uapi(uapiName) \
        CallDevice5Uapi(uapiName) \
        CallDeviceNUapi(uapiName) \
        }

#define CallDeviceUapi(uapiName)  \
        SwitchPortHeader  \
        { \
        CallDevice0Uapi(uapiName) \
        CallDevice1Uapi(uapiName) \
        CallDevice2Uapi(uapiName) \
        CallDevice3Uapi(uapiName) \
        CallDevice4Uapi(uapiName) \
        CallDevice5Uapi(uapiName) \
        CallDeviceNUapi(uapiName) \
        }

/**
 * @brief  univeral layer inialization function
 * @return AvOk - success
 */
uapi AvRet AvUapiInit()
{
    /* init hardware access function pointer */
    AvHookI2cRead = NULL;
    AvHookI2cWrite = NULL;
    AvHookUartTxByte = NULL;
    AvHookUartRxByte = NULL;
    AvHookGetMilliSecond = NULL;
    AvHookGetKey = NULL;
    AvHookGetIrda = NULL;
    return AvOk;
}

/**
 * @brief  hookup user's hardware access functions
 * @return AvOk - success
 */
uapi AvRet AvUapiHookBspFunctions(pin AvFpI2cRead i2cRd, pin AvFpI2cWrite i2cWr, pin AvFpUartSendByte uartTxB, pin AvFpUartGetByte uartRxB, pin AvFpGetMilliSecond getMs, pin AvFpGetKey getKey,pin AvFpGetIrda getIrda)
{
    AvRet ret = AvOk;
    AvHookI2cRead = i2cRd;
    AvHookI2cWrite = i2cWr;
    AvHookUartTxByte = uartTxB;
    AvHookUartRxByte = uartRxB;
    AvHookGetMilliSecond = getMs;
    AvHookGetKey = getKey;
    AvHookGetIrda = getIrda;
    #if AvEnableDebugUapi
    AvUapiOutputDebugMessage(" ");
    AvUapiOutputDebugMessage("-------------------------------------------------------------------");
    AvUapiOutputDebugMessage("    |> Audio/Video Software %s", AvVersion);
    AvUapiOutputDebugMessage("-------------------------------------------------------------------");
    #endif

    delay_ms(10);
    return ret;
}

/**
 * @brief  hookup user's hardware access functions
 * @return AvOk - success
 */
uapi AvRet AvUapiHookBspFunctionsInit(pin AvFpI2cRead i2cRd, pin AvFpI2cWrite i2cWr)
{
    AvRet ret = AvOk;
    AvHookI2cRead = i2cRd;
    AvHookI2cWrite = i2cWr;
    #if AvEnableDebugUapi
    AvUapiOutputDebugMessage(" ");
    AvUapiOutputDebugMessage("-------------------------------------------------------------------");
    AvUapiOutputDebugMessage("    |> Audio/Video Software %s", AvVersion);
    AvUapiOutputDebugMessage("-------------------------------------------------------------------");
    #endif

    delay_ms(10);
    return ret;
}

char g_cGsvDevStrting[20];

void GsvGetDeviceTypeString(u8 type)
{
    switch(type)
    {
        case Gsv1k:
            strcpy(g_cGsvDevStrting,"Gsv1k");
            break;
        case Gsv2k1:
            strcpy(g_cGsvDevStrting,"Gsv2k1");
            break;
        case Gsv2k2:
            strcpy(g_cGsvDevStrting,"Gsv2k2");
            break;
        case Gsv2k6:
            strcpy(g_cGsvDevStrting,"Gsv2k6");
            break;
        case Gsv2k8:
            strcpy(g_cGsvDevStrting,"Gsv2k8");
            break;
        case Gsv5k1:
            strcpy(g_cGsvDevStrting,"Gsv5k1");
            break;
        case Gsv2k11:
            strcpy(g_cGsvDevStrting,"Gsv2k11");
            break;
    }
}
/**
 * @brief  output debug message
 * @return AvOk - success
 */
uapi AvRet AvUapiOuputDbgMsg(pin schar *stringToFormat, ...)
{
    va_list ap;
    uint8 formattedString[256]; /* max 128 chars in a string */
    int32 chars;
    AvRet ret = AvOk;
	pstUartDeclare_t 	pstUart = (pstUartDeclare_t)&stProjectUartWorkTable[0];

    if(!g_bEnableDbgFlag)
        return AvError;

	while(pstUart->u8UartPort != _UART_PORT_NONE)
	{
		if(pstUart->bIsDebugPort)
		{
			break;
		}
		pstUart ++;
	}

	if(pstUart->u8UartPort == _UART_PORT_NONE)
	{
		return AvError;
	}


#if _ENABLE_UART_SEMAPHORE_FUNCTION_BY_PRINT
    if(pstUart->u8UartPort == _UART_PORT_1)
        Uart1_xSemaphoreTask();
    else if(pstUart->u8UartPort == _UART_PORT_2)
        Uart2_xSemaphoreTask();
    else if(pstUart->u8UartPort == _UART_PORT_3)
        Uart3_xSemaphoreTask();
//    #if _ENABLE_UART_4
//    else if(pstUart->u8UartPort == _UART_PORT_4)
//        Uart4_xSemaphoreTask();
//    #endif
//    #if _ENABLE_UART_5
//    else if(pstUart->u8UartPort == _UART_PORT_5)
//        Uart5_xSemaphoreTask();
//    #endif
//    #if _ENABLE_UART_6
//    else if(pstUart->u8UartPort == _UART_PORT_6)
//        Uart6_xSemaphoreTask();
//    #endif
#endif
    GsvGetDeviceTypeString(g_u8GsvChipType[g_u8ChipDev]);

    chars = sprintf((schar*)formattedString,"GSV[%d][%s] ", g_u8ChipDev,g_cGsvDevStrting);
    USARTx_send_bytes(pstUart, (uint8_t *)formattedString, chars);

    va_start(ap, stringToFormat);
    chars = vsprintf((schar*)formattedString,stringToFormat, ap);
    va_end(ap);

    formattedString[chars+0] = 0x0d;
    formattedString[chars+1] = 0x0a;


    USARTx_send_bytes(pstUart, (uint8_t *)formattedString, chars+2);

#if _ENABLE_UART_SEMAPHORE_FUNCTION_BY_PRINT
    if(pstUart->u8UartPort == _UART_PORT_1)
        Uart1_xSemaphoreGive();
    else if(pstUart->u8UartPort == _UART_PORT_2)
        Uart2_xSemaphoreGive();
    else if(pstUart->u8UartPort == _UART_PORT_3)
        Uart3_xSemaphoreGive();
//    #if _ENABLE_UART_4
//    else if(pstUart->u8UartPort == _UART_PORT_4)
//        Uart4_xSemaphoreGive();
//    #endif
//    #if _ENABLE_UART_5
//    else if(pstUart->u8UartPort == _UART_PORT_5)
//        Uart5_xSemaphoreGive();
//    #endif
//    #if _ENABLE_UART_6
//    else if(pstUart->u8UartPort == _UART_PORT_6)
//        Uart6_xSemaphoreGive();
//    #endif
#endif

    return ret;
}

/**
 * @brief  output debug message
 * @return AvOk - success
 */
uapi AvRet AvUapiUartInput(pin schar *stringToFormat, ...)
{
#if AvEnableUartInput
    va_list ap;
    uint8 formattedString[128]; /* max 128 chars in a string */
    int32 chars;
    AvRet ret = AvOk;

    va_start(ap, stringToFormat);
    chars = vsprintf((schar*)formattedString,stringToFormat, ap);
    va_end(ap);

    formattedString[chars+0] = 0x0a;
    formattedString[chars+1] = 0x0d;

    AvUartTxByte(formattedString, chars+2);

    /*
    if (AvUartTxByte != NULL)
    {
        for(i = 0; i < chars; i ++)
            AvUartTxByte((uint8)formattedString[i]);
    }
    AvUartTxByte(10);
    AvUartTxByte(13);
    */
    return ret;
#else
    return AvOk;
#endif
}

/**
 * @brief  allocate memory from global static buffer
 * @param  bytes memory size in bytes
 * @return AvOk - success
 */
uapi AvRet AvUapiAllocateMemory(pin uint32 bytes, pout uint32 *bufferAddress)
{
    AvRet ret = AvOk;
    static uint32 currentPoint = 0;
    uint32 wordbytes = bytes & 0xfffffffc;
    if ((currentPoint + bytes) < AvGlobalStaticBufferSize)
    {
        *bufferAddress = (uint32)(&AvGlobalStaticBuffer[currentPoint]);
        if(bytes != wordbytes)
            bytes = wordbytes + 4;
        currentPoint = currentPoint + bytes;
    }
    else
    {
        AvUapiOuputDbgMsg("ERROR: global static buffer exhausted.");
        ret = AvNotAvailable;
        while(1) ;
    }

//    AvUapiOuputDbgMsg("currentPoint = 0x%04x, bufferAddress = 0x%04x",currentPoint,bufferAddress);
    return ret;
}

uapi AvRet AvUapiRxReadInfo(pin AvPort *port)
{
    return AvOk;
}

uapi AvRet AvUapiRxReadStdi(pin AvPort *port)
{
    return AvOk;
}

uapi AvRet AvUapiTxDetectMode(pin AvPort *port)
{
    return AvOk;
}

uapi AvRet AvUapiConnectPort(pin AvPort *FromPort, pin AvPort *ToPort,AvConnectType type)
{
    AvRet ret = AvNotSupport;
    AvPort *port = ToPort;
    CallDeviceUapi(AvUapiConnectPort(FromPort, ToPort,type));
    return ret;
}
uapi AvRet AvUapiDisconnectPort(pin AvPort *port)
{
    AvRet ret = AvNotSupport;
    CallDeviceUapi(AvUapiDisconnectPort(port));
    return ret;
}
uapi AvRet AvUapiInitDevice(pio AvDevice *device)
{
    AvRet ret = AvNotSupport;
    CallDeviceUapi0(AvUapiInitDevice(device));
    return ret;
}
uapi AvRet AvUapiResetPort(pio AvPort *port)
{
    AvRet ret = AvNotSupport;
    CallDeviceUapi(AvUapiResetPort(port));
    return ret;
}
uapi AvRet AvUapiEnablePort(pio AvPort *port)
{
    AvRet ret = AvNotSupport;
    CallDeviceUapi(AvUapiEnablePort(port));
    return ret;
}
uapi AvRet AvUapiRxPortInit(pio AvPort *port)
{
    AvRet ret = AvNotSupport;
    CallDeviceUapi(AvUapiRxPortInit(port));
    return ret;
}
uapi AvRet AvUapiRxGetStatus(pio AvPort *port)
{
    AvRet ret = AvNotSupport;
    CallDeviceUapi(AvUapiRxGetStatus(port));
    return ret;
}
uapi AvRet AvUapiRxEnableFreeRun(pio AvPort *port, bool enable)
{
    AvRet ret = AvNotSupport;
    CallDeviceUapi(AvUapiRxEnableFreeRun(port, enable));
    return ret;
}
uapi AvRet AvUapiRxGetHdcpStatus(pio AvPort *port, HdcpInterrupt* Intpt)
{
    AvRet ret = AvNotSupport;
    CallDeviceUapi(AvUapiRxGetHdcpStatus(port, Intpt));
    return ret;
}
uapi AvRet AvUapiRxGetVideoPacketStatus(pio AvPort *port, VideoInterrupt* Intpt)
{
    AvRet ret = AvNotSupport;
    CallDeviceUapi(AvUapiRxGetVideoPacketStatus(port, Intpt));
    return ret;
}
uapi AvRet AvUapiRxGetAudioPacketStatus(pio AvPort *port, AudioInterrupt* Intpt)
{
    AvRet ret = AvNotSupport;
    CallDeviceUapi(AvUapiRxGetAudioPacketStatus(port, Intpt));
    return ret;
}
uapi AvRet AvUapiRxGet5VStatus(pio AvPort *port)
{
    AvRet ret = AvNotSupport;
    CallDeviceUapi(AvUapiRxGet5VStatus(port));
    return ret;
}
uapi AvRet AvUapiRxSetHpdDown(pio AvPort *port)
{
    AvRet ret = AvNotSupport;
    CallDeviceUapi(AvUapiRxSetHpdDown(port));
    return ret;
}

uapi AvRet AvUapiRxSetHpdUp(pio AvPort *port)
{
    AvRet ret = AvNotSupport;
    CallDeviceUapi(AvUapiRxSetHpdUp(port));
    return ret;
}
uapi AvRet AvUapiRxClearFlags(pio AvPort *port)
{
    AvRet ret = AvNotSupport;
    CallDeviceUapi(AvUapiRxClearFlags(port));
    return ret;
}
uapi AvRet AvUapiRxGetPacketContent(pin AvPort *port, PacketType Pkt, uint8 *Content)
{
    AvRet ret = AvNotSupport;
    CallDeviceUapi(AvUapiRxGetPacketContent(port, Pkt,Content));
    return ret;
}
uapi AvRet AvUapiRxGetHdmiAcrInfo(pio AvPort *port)
{
    AvRet ret = AvNotSupport;
    CallDeviceUapi(AvUapiRxGetHdmiAcrInfo(port));
    return ret;
}
uapi AvRet AvUapiRxClearAudioInterrupt(pio AvPort *port, AudioInterrupt* Intpt)
{
    AvRet ret = AvNotSupport;
    CallDeviceUapi(AvUapiRxClearAudioInterrupt(port, Intpt));
    return ret;
}
uapi AvRet AvUapiRxClearVideoInterrupt(pio AvPort *port, VideoInterrupt* Intpt)
{
    AvRet ret = AvNotSupport;
    CallDeviceUapi(AvUapiRxClearVideoInterrupt(port, Intpt));
    return ret;
}
uapi AvRet AvUapiRxClearHdcpInterrupt(pio AvPort *port,  HdcpInterrupt* Intpt)
{
    AvRet ret = AvNotSupport;
    CallDeviceUapi(AvUapiRxClearHdcpInterrupt(port, Intpt));
    return ret;
}

uapi AvRet AvUapiTxPortInit(pio AvPort *port)
{
    AvRet ret = AvNotSupport;
    CallDeviceUapi(AvUapiTxPortInit(port));
    return ret;
}
uapi AvRet AvUapiTxEnableCore(pin AvPort *port)
{
    AvRet ret = AvNotSupport;
    CallDeviceUapi(AvUapiTxEnableCore(port));
    return ret;
}
uapi AvRet AvUapiTxDisableCore(pin AvPort *port)
{
    AvRet ret = AvNotSupport;
    CallDeviceUapi(AvUapiTxDisableCore(port));
    return ret;
}
uapi AvRet AvUapiTxGetStatus(pio AvPort *port)
{
    AvRet ret = AvNotSupport;
    CallDeviceUapi(AvUapiTxGetStatus(port));
    return ret;
}
uapi AvRet AvUapiTxMuteTmds(pio AvPort *port, bool mute)
{
    AvRet ret = AvNotSupport;
    CallDeviceUapi(AvUapiTxMuteTmds(port, mute));
    return ret;
}
uapi AvRet AvUapiTxGetSinkHdcpCapability(pio AvPort *port)
{
    AvRet ret = AvNotSupport;
    CallDeviceUapi(AvUapiTxGetSinkHdcpCapability(port));
    return ret;
}
uapi AvRet AvUapiTxSetAudioPackets(pin AvPort *port)
{
    AvRet ret = AvNotSupport;
    CallDeviceUapi(AvUapiTxSetAudioPackets(port));
    return ret;
}
uapi AvRet AvUapiTxVideoManage(pio AvPort *port)
{
    AvRet ret = AvNotSupport;
    CallDeviceUapi(AvUapiTxVideoManage(port));
    return ret;
}
uapi AvRet AvUapiTxAudioManage(pio AvPort *port)
{
    AvRet ret = AvNotSupport;
    CallDeviceUapi(AvUapiTxAudioManage(port));
    return ret;
}
uapi AvRet AvUapiTxSetPacketContent(pin AvPort *port, PacketType Pkt, uint8 *Content, uint8 PktEn)
{
    AvRet ret = AvNotSupport;
    CallDeviceUapi(AvUapiTxSetPacketContent(port, Pkt, Content, PktEn));
    return ret;
}
uapi AvRet AvUapiTxEnableInfoFrames(pin AvPort* port, uint16 InfoFrames, bool Enable)
{
    AvRet ret = AvNotSupport;
    CallDeviceUapi(AvUapiTxEnableInfoFrames(port, InfoFrames, Enable));
    return ret;
}
uapi AvRet AvUapiTxGetHdcpStatus(pin AvPort *port)
{
    AvRet ret = AvNotSupport;
    CallDeviceUapi(AvUapiTxGetHdcpStatus(port));
    return ret;
}
uapi AvRet AvUapiTxGetBksvReady(pin AvPort *port)
{
    AvRet ret = AvNotSupport;
    CallDeviceUapi(AvUapiTxGetBksvReady(port));
    return ret;
}

uapi AvRet AvUapiRxGetAvMute(pio AvPort *port)
{
    AvRet ret = AvNotSupport;
    CallDeviceUapi(AvUapiRxGetAvMute(port));
    return ret;
}
uapi AvRet AvUapiTxSetAvMute(pio AvPort *port)
{
    AvRet ret = AvNotSupport;
    CallDeviceUapi(AvUapiTxSetAvMute(port));
    return ret;
}
uapi AvRet AvUapiTxSetBlackMute(pio AvPort *port)
{
    AvRet ret = AvNotSupport;
    CallDeviceUapi(AvUapiTxSetBlackMute(port));
    return ret;
}
uapi AvRet AvUapiRxGetHdmiModeSupport(pio AvPort *port)
{
    AvRet ret = AvNotSupport;
    CallDeviceUapi(AvUapiRxGetHdmiModeSupport(port));
    return ret;
}
uapi AvRet AvUapiTxSetHdmiModeSupport(pio AvPort *port)
{
    AvRet ret = AvNotSupport;
    CallDeviceUapi(AvUapiTxSetHdmiModeSupport(port));
    return ret;
}
uapi AvRet AvUapiTxSetFeatureSupport(pio AvPort *port)
{
    AvRet ret = AvNotSupport;
    CallDeviceUapi(AvUapiTxSetFeatureSupport(port));
    return ret;
}
uapi AvRet AvUapiRxGetVideoLock(pio AvPort *port)
{
    AvRet ret = AvNotSupport;
    CallDeviceUapi(AvUapiRxGetVideoLock(port));
    return ret;
}
uapi AvRet AvUapiRxGetVideoTiming(pio AvPort *port)
{
    AvRet ret = AvNotSupport;
    CallDeviceUapi(AvUapiRxGetVideoTiming(port));
    return ret;
}
uapi AvRet AvUapiTxSetVideoTiming(pio AvPort *port)
{
    AvRet ret = AvNotSupport;
    CallDeviceUapi(AvUapiTxSetVideoTiming(port));
    return ret;
}
uapi AvRet AvUapiRxGetHdmiDeepColor(pio AvPort *port)
{
    AvRet ret = AvNotSupport;
    CallDeviceUapi(AvUapiRxGetHdmiDeepColor(port));
    return ret;
}
uapi AvRet AvUapiTxSetHdmiDeepColor(pio AvPort *port)
{
    AvRet ret = AvNotSupport;
    CallDeviceUapi(AvUapiTxSetHdmiDeepColor(port));
    return ret;
}
uapi AvRet AvUapiRxGetAudioInternalMute(pio AvPort *port)
{
    AvRet ret = AvNotSupport;
    CallDeviceUapi(AvUapiRxGetAudioInternalMute(port));
    return ret;
}
uapi AvRet AvUapiRxSetAudioInternalMute(pio AvPort *port)
{
    AvRet ret = AvNotSupport;
    CallDeviceUapi(AvUapiRxSetAudioInternalMute(port));
    return ret;
}
uapi AvRet AvUapiTxSetAudNValue(pio AvPort *port)
{
    AvRet ret = AvNotSupport;
    CallDeviceUapi(AvUapiTxSetAudNValue(port));
    return ret;
}
uapi AvRet AvUapiRxGetPacketType(pio AvPort *port)
{
    AvRet ret = AvNotSupport;
    CallDeviceUapi(AvUapiRxGetPacketType(port));
    return ret;
}
uapi AvRet AvUapiTxReadBksv(pio AvPort *port, uint8 *Value, uint8 Count)
{
    AvRet ret = AvNotSupport;
    CallDeviceUapi(AvUapiTxReadBksv(port,Value,Count));
    return ret;
}
uapi AvRet AvUapiRxAddBksv(pio AvPort *port, uint8 *Value, uint8 Position)
{
    AvRet ret = AvNotSupport;
    CallDeviceUapi(AvUapiRxAddBksv(port,Value,Position));
    return ret;
}
uapi AvRet AvUapiTxGetBksvTotal(pio AvPort *port, uint8 *Value)
{
    AvRet ret = AvNotSupport;
    CallDeviceUapi(AvUapiTxGetBksvTotal(port,Value));
    return ret;
}
uapi AvRet AvUapiTxEncryptSink(pin AvPort *port)
{
    AvRet ret = AvNotSupport;
    CallDeviceUapi(AvUapiTxEncryptSink(port));
    return ret;
}
uapi AvRet AvUapiTxDecryptSink(pin AvPort *port)
{
    AvRet ret = AvNotSupport;
    CallDeviceUapi(AvUapiTxDecryptSink(port));
    return ret;
}
uapi AvRet AvUapiRxSetHdcpEnable(pin AvPort *port)
{
    AvRet ret = AvNotSupport;
    CallDeviceUapi(AvUapiRxSetHdcpEnable(port));
    return ret;
}
uapi AvRet AvUapiRxSetBksvListReady(pin AvPort *port)
{
    AvRet ret = AvNotSupport;
    CallDeviceUapi(AvUapiRxSetBksvListReady(port));
    return ret;
}
uapi AvRet AvUapiRxSetHdcpMode(pin AvPort *port)
{
    AvRet ret = AvNotSupport;
    CallDeviceUapi(AvUapiRxSetHdcpMode(port));
    return ret;
}
uapi AvRet AvUapiHdcp2p2Mode(pin AvPort *port)
{
    AvRet ret = AvNotSupport;
    CallDeviceUapi(AvUapiHdcp2p2Mode(port));
    return ret;
}
uapi AvRet AvUapiTxClearBksvReady(pin AvPort *port)
{
    AvRet ret = AvNotSupport;
    CallDeviceUapi(AvUapiTxClearBksvReady(port));
    return ret;
}
uapi AvRet AvUapiTxClearRxidReady(pin AvPort *port)
{
    AvRet ret = AvNotSupport;
    CallDeviceUapi(AvUapiTxClearRxidReady(port));
    return ret;
}
uapi AvRet AvUapiTxClearHdcpError(pin AvPort *port)
{
    AvRet ret = AvNotSupport;
    CallDeviceUapi(AvUapiTxClearHdcpError(port));
    return ret;
}
uapi AvRet AvUapiRxCheckBksvExisted(pin AvPort *port, uint8 *Bksv)
{
    AvRet ret = AvNotSupport;
    CallDeviceUapi(AvUapiRxCheckBksvExisted(port,Bksv));
    return ret;
}
uapi AvRet AvUapiCecSendMessage(pin AvPort *port)
{
    AvRet ret = AvNotSupport;
    CallDeviceUapi(AvUapiCecSendMessage(port));
    return ret;
}
uapi AvRet AvUapiGetNackCount(pin AvPort *port)
{
    AvRet ret = AvNotSupport;
    CallDeviceUapi(AvUapiGetNackCount(port));
    return ret;
}
uapi AvRet AvUapiTxCecInit(pin AvPort *port)
{
    AvRet ret = AvNotSupport;
    CallDeviceUapi(AvUapiTxCecInit(port));
    return ret;
}
uapi AvRet AvUapiCecRxGetStatus(pin AvPort *port)
{
    AvRet ret = AvNotSupport;
    CallDeviceUapi(AvUapiCecRxGetStatus(port));
    return ret;
}
uapi AvRet AvUapiCecTxGetStatus(pin AvPort *port)
{
    AvRet ret = AvNotSupport;
    CallDeviceUapi(AvUapiCecTxGetStatus(port));
    return ret;
}
uapi AvRet AvUapiTxCecSetPhysicalAddr(AvPort *port)
{
    AvRet ret = AvNotSupport;
    CallDeviceUapi(AvUapiTxCecSetPhysicalAddr(port));
    return ret;
}
uapi AvRet AvUapiTxCecSetLogicalAddr(AvPort *port)
{
    AvRet ret = AvNotSupport;
    CallDeviceUapi(AvUapiTxCecSetLogicalAddr(port));
    return ret;
}
uapi AvRet AvUapiRxReadEdid(pio AvPort *port, uint8 *Value, uint16 Count)
{
    AvRet ret = AvNotSupport;
    CallDeviceUapi(AvUapiRxReadEdid(port,Value,Count));
    return ret;
}
uapi AvRet AvUapiRxWriteEdid(pio AvPort *port, uint8 *Value, uint16 Count)
{
    AvRet ret = AvNotSupport;
    CallDeviceUapi(AvUapiRxWriteEdid(port,Value,Count));
    return ret;
}
uapi AvRet AvUapiRxSetSpa(pio AvPort *port, uint8 SpaLocation, uint8 *SpaValue)
{
    AvRet ret = AvNotSupport;
    CallDeviceUapi(AvUapiRxSetSpa(port,SpaLocation,SpaValue));
    return ret;
}
uapi AvRet AvUapiTxReadEdid(pio AvPort *port, uint8 *Value, uint16 Count)
{
    AvRet ret = AvNotSupport;
    CallDeviceUapi(AvUapiTxReadEdid(port,Value,Count));
    return ret;
}
uapi AvRet AvUapiRxEnableInternalEdid(pio AvPort *port)
{
    AvRet ret = AvNotSupport;
    CallDeviceUapi(AvUapiRxEnableInternalEdid(port));
    return ret;
}
uapi AvRet AvUapiTxArcEnable(pio AvPort *port, uint8 value)
{
    AvRet ret = AvNotSupport;
    CallDeviceUapi(AvUapiTxArcEnable(port,value));
    return ret;
}
uapi AvRet AvUapiRxHdcp2p2Manage(pin AvPort *port)
{
    AvRet ret = AvNotSupport;
    CallDeviceUapi(AvUapiRxHdcp2p2Manage(port));
    return ret;
}
uapi AvRet AvUapiTxHdcp2p2Manage(pin AvPort *port)
{
    AvRet ret = AvNotSupport;
    CallDeviceUapi(AvUapiTxHdcp2p2Manage(port));
    return ret;
}
uapi AvRet AvUapiCheckLogicAudioTx(pio AvPort *port)
{
    AvRet ret = AvNotSupport;
    CallDeviceUapi(AvUapiCheckLogicAudioTx(port));
    return ret;
}
uapi AvRet AvUapiCheckLogicAudioRx(pio AvPort *port)
{
    AvRet ret = AvNotSupport;
    CallDeviceUapi(AvUapiCheckLogicAudioRx(port));
    return ret;
}
uapi AvRet AvUapiCheckVideoScaler(pio AvPort *port)
{
    AvRet ret = AvNotSupport;
    CallDeviceUapi(AvUapiCheckVideoScaler(port));
    return ret;
}
uapi AvRet AvUapiCheckVideoColor(pio AvPort *port)
{
    AvRet ret = AvNotSupport;
    CallDeviceUapi(AvUapiCheckVideoColor(port));
    return ret;
}
uapi AvRet AvUapiCheckVideoGen(pio AvPort *port)
{
    AvRet ret = AvNotSupport;
    CallDeviceUapi(AvUapiCheckVideoGen(port));
    return ret;
}
uapi AvRet AvUapiRxVideoManage(pin AvPort *port, VideoInterrupt* Intpt)
{
    AvRet ret = AvNotSupport;
    CallDeviceUapi(AvUapiRxVideoManage(port,Intpt));
    return ret;
}
uapi AvRet AvUapiRxAudioManage(pin AvPort *port, AudioInterrupt* Intpt)
{
    AvRet ret = AvNotSupport;
    CallDeviceUapi(AvUapiRxAudioManage(port,Intpt));
    return ret;
}

#if GSV2K11
uapi AvRet AvUapiCheckLogicVideoTx(pio AvPort *port)
{
    AvRet ret = AvNotSupport;
    //CallDeviceUapi(AvUapiCheckLogicVideoTx(port));
    CallUapi(Gsv2k11, AvUapiCheckLogicVideoTx(port));
    return ret;
}
uapi AvRet AvUapiCheckLogicVideoRx(pio AvPort *port)
{
    AvRet ret = AvNotSupport;
    //CallDeviceUapi(AvUapiCheckLogicVideoRx(port));
    CallUapi(Gsv2k11, AvUapiCheckLogicVideoRx(port));
    return ret;
}
uapi AvRet AvUapiCheckAudioGen(pio AvPort *port)
{
    AvRet ret = AvNotSupport;
    CallDeviceUapi(AvUapiCheckAudioGen(port));
    return ret;
}
uapi AvRet AvUapiCheckClockGen(pio AvPort *port)
{
    AvRet ret = AvNotSupport;
    CallDeviceUapi(AvUapiCheckClockGen(port));
    return ret;
}
#endif

#if GSV5K1
uapi AvRet AvUapiCheckVideoDsc(pio AvPort *port)
{
    AvRet ret = AvNotSupport;
    //CallDeviceUapi(AvUapiCheckVideoDsc(port));
    CallUapi(Gsv5k1, AvUapiCheckVideoDsc(port));
    return ret;
}

#if AvEnableCcaFeature /* CCA Related */
uapi AvRet AvUapiCcaTxGetInfo(pio AvPort *port)
{
    AvRet ret = AvNotSupport;
    CallDeviceUapi(AvUapiCcaTxGetInfo(port));
    return ret;
}

uapi AvRet AvUapiCcaRxGetInfo(pio AvPort *port)
{
    AvRet ret = AvNotSupport;
    CallDeviceUapi(AvUapiCcaRxGetInfo(port));
    return ret;
}
uapi AvRet AvUapiCcaTxInit(pio AvPort *port)
{
    AvRet ret = AvNotSupport;
    CallDeviceUapi(AvUapiCcaTxInit(port));
    return ret;
}

uapi AvRet AvUapiCcaRxInit(pio AvPort *port)
{
    AvRet ret = AvNotSupport;
    CallDeviceUapi(AvUapiCcaRxInit(port));
    return ret;
}

uapi AvRet AvUapiCcaTxChainReset(pio AvPort *port)
{
    AvRet ret = AvNotSupport;
    CallDeviceUapi(AvUapiCcaTxChainReset(port));
    return ret;
}

uapi AvRet AvUapiCcaRxChainReset(pio AvPort *port)
{
    AvRet ret = AvNotSupport;
    CallDeviceUapi(AvUapiCcaRxChainReset(port));
    return ret;
}

uapi AvRet AvUapiCcaTxCalibManage(pio AvPort *port)
{
    AvRet ret = AvNotSupport;
    CallDeviceUapi(AvUapiCcaTxCalibManage(port));
    return ret;
}

uapi AvRet AvUapiCcaRxCalibManage(pio AvPort *port)
{
    AvRet ret = AvNotSupport;
    CallDeviceUapi(AvUapiCcaRxCalibManage(port));
    return ret;
}

uapi AvRet AvUapiCcaTxSetWorkMode(pio AvPort *port)
{
    AvRet ret = AvNotSupport;
    CallDeviceUapi(AvUapiCcaTxSetWorkMode(port));
    return ret;
}

uapi AvRet AvUapiCcaTxEnablePin(pio AvPort *port, uint8 SlotName)
{
    AvRet ret = AvNotSupport;
    CallDeviceUapi(AvUapiCcaTxEnablePin(port,SlotName));
    return ret;
}

uapi AvRet AvUapiCcaRxEnablePin(pio AvPort *port, uint8 SlotName)
{
    AvRet ret = AvNotSupport;
    CallDeviceUapi(AvUapiCcaRxEnablePin(port,SlotName));
    return ret;
}
#endif /* CCA Related */

#endif
#endif

