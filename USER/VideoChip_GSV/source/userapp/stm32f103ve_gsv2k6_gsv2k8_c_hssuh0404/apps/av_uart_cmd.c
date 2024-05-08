#include "includes.h"

#if _ENABLE_CHIP_GSV_CHIP_SERIES

#include "av_config.h"
#include "Gsv_hal.h"
#include "Gsv_kapi.h"
#include "av_uart_cmd.h"

extern AvPort gsv2k8Ports[AvGsv2k8DeviceNumber][AvGsv2k8PortNumber];

#define CommandNameIs(data) (!AvStrcmp((const char*) cCommand, data))
#define UartCommandLength 8

#if AvEnableUartInput /* Enable UART */
void ProcessChar(AvPort *port);
void DoUartCommand(AvPort *port);
void ReturnIdle(void);

typedef enum
{
  P_IDLE=0,
  P_CMD,
  P_ARG1,
  P_ARG2,
  P_ARG3,
  P_REP
}ParseState;

#if (GSV2K8||AvMatrix4x4Mode)
extern uchar  AudioStatus[AvGsv2k8DeviceNumber][AudioNumber];
#elif GSV2K6
extern uchar  AudioStatus[AvGsv2k6DeviceNumber][AudioNumber];
#elif GSV2K2
extern uchar  AudioStatus[AvGsv2k2DeviceNumber][AudioNumber];
#endif

static ParseState eParser = P_IDLE;
static uint8 cCommand[UartCommandLength];
static uint8 uCmdIndex;
static uint8 cArg1[4];
static uint8 uArg1Index;
static uint8 cArg2[4];
static uint8 uArg2Index;
static uint8 cArg3[4];
static uint8 uArg3Index;
static uint32 uArg1;
static uint32 uArg2;
static uint32 uArg3;

static uint8 RegPause   = 0;

#endif /* Enable UART */

void ProcessChar(AvPort *port)
{
#if AvEnableUartInput
    uint8 uChar = 0x00;
    uint8 uReturn = 0x0a;
    /* Get Uart Data, if no data, return */
    if(AvHalUartGetByte(&uChar) == AvError)
        return;

    AvHalUartSendByte(&uChar, 1);
    if(uChar == 0x0d)
        AvHalUartSendByte(&uReturn, 1);

    switch (eParser)
    {
        case P_IDLE:
            //AvKapiOutputDebugMessage("\n\rKeyIn List:\n\r's'-Current Connection");
            //AvKapiOutputDebugMessage("'c * *'-Connect Rx* to Tx*, e.g. 'c 4 1'");
            uChar = IsLetter(uChar);
            if (uChar)
            {
                cCommand[uCmdIndex] = uChar;
                uCmdIndex++;
                eParser = P_CMD;
            }
            break;

        case P_CMD:
            if (IsCR(uChar)) //receive end char 0x0D
            {
                cCommand[uCmdIndex] = 0;
                uCmdIndex++;
                DoUartCommand(port); //process data
                ReturnIdle();
            }
            else if (IsSpace(uChar)) //receive space key
            {
                cCommand[uCmdIndex] = 0;
                uCmdIndex++;
                if CommandNameIs("REP")
                    eParser = P_REP;
                else
                    eParser = P_ARG1;
            }
            else if (IsBackSpace(uChar))
            {
                if (uCmdIndex>0)
                    uCmdIndex--;
            }
            else
            {
                uChar = IsLetter(uChar);
                if (uChar)
                {
                    cCommand[uCmdIndex] = uChar;
                    uCmdIndex++;
                }
            }
            break;

        case P_ARG1:
            if (IsCR(uChar))
            {
                cArg1[uArg1Index] = 0;
                uArg1 = AsciiToNumber(cArg1, uArg1Index);
                DoUartCommand(port);
                ReturnIdle();
            }
            else if (IsSpace(uChar))
            {
                cArg1[uArg1Index] = 0;
                uArg1 = AsciiToNumber(cArg1, uArg1Index);
                eParser = P_ARG2;
            }
            else if (IsBackSpace(uChar))
            {
                if (uArg1Index>0) uArg1Index--;
                else eParser = P_CMD;
            }
            else
            {
                uChar = IsLetter(uChar);
                if (uChar)
                {
                    cArg1[uArg1Index] = uChar;
                    uArg1Index++;
                }
            }
            break;

        case P_ARG2:
            if (IsCR(uChar))
            {
                cArg2[uArg2Index] = 0;
                uArg2 = AsciiToNumber(cArg2, uArg2Index);
                DoUartCommand(port);
                ReturnIdle();
            }
            else if (IsSpace(uChar))
            {
                cArg2[uArg2Index] = 0;
                uArg2 = AsciiToNumber(cArg2, uArg2Index);
                eParser = P_ARG3;
            }
            else if (IsBackSpace(uChar))
            {
                if (uArg2Index>0) uArg2Index--;
                else eParser = P_ARG1;
            }
            else
            {
                uChar = IsLetter(uChar);
                if (uChar)
                {
                    cArg2[uArg2Index] = uChar;
                    uArg2Index++;
                }
            }
            break;

        case P_ARG3:
            if (IsCR(uChar) || IsSpace(uChar))
            {
                cArg3[uArg3Index] = 0;
                uArg3 = AsciiToNumber(cArg3, uArg3Index);
                DoUartCommand(port);
                ReturnIdle();
            }
            else if (IsBackSpace(uChar))
            {
                if (uArg3Index>0) uArg3Index--;
                else eParser = P_ARG2;
            }
            else
            {
                uChar = IsLetter(uChar);
                if (uChar)
                {
                    cArg3[uArg3Index] = uChar;
                    uArg3Index++;
                }
            }
            break;

        case P_REP:
            if (IsCR(uChar))
            {
                cArg1[uArg1Index++] = '\r';
                cArg1[uArg1Index] = 0;
                DoUartCommand(port);
            }
            else if (IsSpace(uChar))
            {
                cArg1[uArg1Index] = ' ';
                uArg1Index++;

            }
            else if (IsBackSpace(uChar))
            {
                if (uArg1Index>0) uArg1Index--;
                else eParser = P_CMD;
            }
            else
            {
                uChar = IsLetter(uChar);
                if (uChar)
                {
                    cArg1[uArg1Index] = uChar;
                    uArg1Index++;
                }
            }
            break;
    }
#endif
}

void ReturnIdle(void)
{
#if AvEnableUartInput
    uint8 i;
    eParser = P_IDLE;
    uCmdIndex = 0;
    uArg1Index = 0;
    uArg2Index = 0;
    uArg3Index = 0;
    uArg1 = 0;
    uArg2 = 0;
    uArg3 = 0;
    for(i=0;i<UartCommandLength;i++)
        cArg1[i]=0;
    for(i=0;i<4;i++)
        cArg2[i]=0;
    for(i=0;i<4;i++)
        cArg3[i]=0;

#endif
}

void DoUartCommand(AvPort *port)
{
#if AvEnableUartInput
    uint8 FromPortNum = 0;
    uint8 ToPortNum = 0;
    uint8 FromPortDev = 0;
    uint8 ToPortDev = 0;
    uint8 DevNum = 0;

    if CommandNameIs("S") /* STATUS */
    {
        AvKapiOutputDebugMessage("Current Routing Rx%x=>Tx1, Rx%x=>Tx2", RegTx1From, RegTx2From);
        return;
    }
    else if CommandNameIs("P") /* PAUSE */
    {
        RegPause = 1;
        AvKapiOutputDebugMessage("Software Paused");
        return;
    }
    else if CommandNameIs("G") /* GO ON */
    {
        RegPause = 0;
        AvKapiOutputDebugMessage("Software Continued");
        return;
    }
    else if CommandNameIs("C") /* CONNECT HDMI PORT */
    {
        if((uArg1 > 4) || (uArg1 < 1))
            AvKapiOutputDebugMessage("Invalid From Port! ReTry!");
        else if((uArg2 > 4) || (uArg2 < 1))
            AvKapiOutputDebugMessage("Invalid To Port! ReTry!");
        else if(uArg3 != 0)
            AvKapiOutputDebugMessage("Don't input 3rd argument!");
        else
        {
            if(uArg2 > 2)
                DevNum = 1;
            else
                DevNum = 0;
            FromPortNum   = (uArg1-1);
            FromPortDev   = (AvGsv2k8PortNumber*DevNum);
            if(DevNum == 1)
            {
                ToPortNum = (uArg2+1);
                ToPortDev = (AvGsv2k8PortNumber*DevNum);
            }
            else
            {
                ToPortNum = (uArg2+3);
                ToPortDev = (AvGsv2k8PortNumber*DevNum);
            }
            AvApiConnectPort(&gsv2k8Ports[FromPortDev][FromPortNum], &gsv2k8Ports[ToPortDev][ToPortNum], AvConnectAV);
            AvKapiOutputDebugMessage("Current Routing Rx%x=>Tx1, Rx%x=>Tx2", uArg1, uArg2);
        }
    }
    else if CommandNameIs("H")
    {
      /* Sets the HDCP type for a port.
       * The first arg is the port number (1-6, where 1-4 are INPUTS and where 5-6 are OUTPUTS)
       * The second arg is the HDCP mode : 1 --> HDCP 1.4, 2 --> HDCP 2.2, 3 --> AUTO, other --> No encryption */

      if(port[uArg1-1].type == HdmiRx)
      {
          if(uArg2 == 1)
          {
              port[uArg1-1].content.hdcp->HdcpNeeded = AV_HDCP_RX_1P4_ONLY;
          }
          else if(uArg2 == 2)
          {
              port[uArg1-1].content.hdcp->HdcpNeeded = AV_HDCP_RX_2P2_ONLY;
          }
          else if(uArg2 == 3)
          {
              port[uArg1-1].content.hdcp->HdcpNeeded = AV_HDCP_RX_AUTO;
          }
          else
          {
              port[uArg1-1].content.hdcp->HdcpNeeded = AV_HDCP_RX_NOT_SUPPORT;
          }
          /* Force Rx DDC Rerun */
          port[uArg1-1].content.rx->Hpd = AV_HPD_TOGGLE;
      }
      else if(port[uArg1-1].type == HdmiTx)
      {
          if(uArg2 == 1)
          {
              port[uArg1-1].content.hdcptx->HdmiStyle = AV_HDCP_TX_1P4_ONLY;
          }
          else if(uArg2 == 2)
          {
              port[uArg1-1].content.hdcptx->HdmiStyle = AV_HDCP_TX_2P2_ONLY;
          }
          else if(uArg2 == 3)
          {
              port[uArg1-1].content.hdcptx->HdmiStyle = AV_HDCP_TX_AUTO;
          }
          else
          {
              port[uArg1-1].content.hdcptx->HdmiStyle = AV_HDCP_TX_ILLEGAL_NO_HDCP;
          }
          /* Force Tx HPD rerun */
          port[uArg1-1].content.hdcptx->HdcpModeUpdate = 1;
      }
      else
      {
        AvKapiOutputDebugMessage("Physical port #%u does not exist!", uArg1);
      }
    }
#endif
}

void ListenToUartCommand(AvPort *port)
{
#if AvEnableUartInput
    ProcessChar(port);
    while(RegPause == 1)
        ProcessChar(port);

#endif
}

#endif

