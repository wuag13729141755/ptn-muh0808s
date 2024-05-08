#include "includes.h"

#if _ENABLE_CHIP_GSV_CHIP_SERIES

#include "av_key_cmd.h"
#include "av_config.h"
#include "Gsv_hal.h"
#include "Gsv_kapi.h"
#include "av_main.h"

extern AvPort gsv2k8Ports[AvGsv2k8DeviceNumber][AvGsv2k8PortNumber];

#if AvEnableKeyInput /* Enable UART */

void ProcessKey(AvPort *port);
void ARCOnOff(AvPort *port,uint8 TxIndex);
#define FoundKeyPress(p) uChar&p

#endif /* Enable UART */

void ProcessKey(AvPort *port)
{
#if AvEnableKeyInput
    uint8 uChar = 0x00;
    AvRet GetKeyResult = AvNotAvailable;
    uint8 FromPortNum = 0;
    uint8 FromPortDev = 0;
    uint8 ToPortNum = 0;
    uint8 ToPortDev = 0;
    AvPort *TempPort = NULL;

    GetKeyResult = AvHalGetKey(&uChar);
    if(GetKeyResult == AvNotAvailable)
        return;

    /* 2. Tx1 Routing Loop */
    if(FoundKeyPress(AvKeyTx1RoutingLoop))
    {
        ToPortNum = 4;
        ToPortDev = (AvGsv2k8PortNumber*0);
        TempPort  = (AvPort*)(gsv2k8Ports[ToPortDev][ToPortNum].content.RouteVideoFromPort);
        if((TempPort->type == VideoScaler) || (TempPort->type == VideoColor))
            TempPort  = (AvPort*)(TempPort->content.RouteVideoFromPort);
        if((TempPort != NULL) && (TempPort->type == HdmiRx))
        {
            FromPortNum = TempPort->index;
            if(FromPortNum == 3)
                FromPortNum = 0;
            else
                FromPortNum = FromPortNum+1;
            TxInLedOut(1,FromPortNum+1);
            FromPortNum = FromPortNum;
            FromPortDev = (AvGsv2k8PortNumber*0);
            AvApiConnectPort(&gsv2k8Ports[FromPortDev][FromPortNum], &gsv2k8Ports[ToPortDev][ToPortNum], AvConnectAV);
        }
    }
    /* 3. Tx2 Routing Loop */
    if(FoundKeyPress(AvKeyTx2RoutingLoop))
    {
        ToPortNum = 5;
        ToPortDev = (AvGsv2k8PortNumber*0);
        TempPort  = (AvPort*)(gsv2k8Ports[ToPortNum].content.RouteVideoFromPort);
        if((TempPort->type == VideoScaler) || (TempPort->type == VideoColor))
            TempPort  = (AvPort*)(TempPort->content.RouteVideoFromPort);
        if((TempPort != NULL) && (TempPort->type == HdmiRx))
        {
            FromPortNum = TempPort->index;
            if(FromPortNum == 3)
                FromPortNum = 0;
            else
                FromPortNum = FromPortNum+1;
            TxInLedOut(2,FromPortNum+1);
            FromPortNum = FromPortNum;
            FromPortDev = (AvGsv2k8PortNumber*0);
            AvApiConnectPort(&gsv2k8Ports[FromPortDev][FromPortNum], &gsv2k8Ports[ToPortDev][ToPortNum], AvConnectAV);
        }
    }
    /* 4. Tx3 Routing Loop */
    if(FoundKeyPress(AvKeyTx3RoutingLoop))
    {
        ToPortNum = 4;
        ToPortDev = (AvGsv2k8PortNumber*1);
        TempPort  = (AvPort*)(gsv2k8Ports[ToPortDev][ToPortNum].content.RouteVideoFromPort);
        if((TempPort->type == VideoScaler) || (TempPort->type == VideoColor))
            TempPort  = (AvPort*)(TempPort->content.RouteVideoFromPort);
        if((TempPort != NULL) && (TempPort->type == HdmiRx))
        {
            FromPortNum = TempPort->index;
            /* reverse to get logic order */
            if(FromPortNum == 1)
                FromPortNum = 0;
            else if(FromPortNum == 0)
                FromPortNum = 1;
            /* logic order add */
            if(FromPortNum == 3)
                FromPortNum = 0;
            else
                FromPortNum = FromPortNum+1;
            TxInLedOut(3,FromPortNum+1);
            /* map to port order */
            if(FromPortNum == 1)
                FromPortNum = 0;
            else if(FromPortNum == 0)
                FromPortNum = 1;
            FromPortNum = FromPortNum;
            FromPortDev = (AvGsv2k8PortNumber*1);
            AvApiConnectPort(&gsv2k8Ports[FromPortDev][FromPortNum], &gsv2k8Ports[ToPortDev][ToPortNum], AvConnectAV);
        }
    }
    /* 5. Tx4 Routing Loop */
    if(FoundKeyPress(AvKeyTx4RoutingLoop))
    {
        ToPortNum = 5;
        ToPortDev = (AvGsv2k8PortNumber*1);
        TempPort  = (AvPort*)(gsv2k8Ports[ToPortDev][ToPortNum].content.RouteVideoFromPort);
        if((TempPort->type == VideoScaler) || (TempPort->type == VideoColor))
            TempPort  = (AvPort*)(TempPort->content.RouteVideoFromPort);
        if((TempPort != NULL) && (TempPort->type == HdmiRx))
        {
            FromPortNum = TempPort->index;
            /* reverse to get logic order */
            if(FromPortNum == 1)
                FromPortNum = 0;
            else if(FromPortNum == 0)
                FromPortNum = 1;
            /* logic order add */
            if(FromPortNum == 3)
                FromPortNum = 0;
            else
                FromPortNum = FromPortNum+1;
            TxInLedOut(4,FromPortNum+1);
            /* map to port order */
            if(FromPortNum == 1)
                FromPortNum = 0;
            else if(FromPortNum == 0)
                FromPortNum = 1;
            FromPortNum = FromPortNum;
            FromPortDev = (AvGsv2k8PortNumber*1);
            AvApiConnectPort(&gsv2k8Ports[FromPortDev][FromPortNum], &gsv2k8Ports[ToPortDev][ToPortNum], AvConnectAV);
        }
    }

#endif
}
void ListenToKeyCommand(AvPort *port)
{
#if AvEnableKeyInput
    ProcessKey(port);
#endif
}

void TxInLedOut(uint8 TxIndex, uint8 RxIndex)
{
#if AvEnableKeyInput
    switch(TxIndex)
    {
        case 1:
            HAL_GPIO_WritePin(GPIOA, LED0_Pin | LED1_Pin | LED2_Pin | LED3_Pin, GPIO_PIN_SET);
            switch(RxIndex)
            {
                case 1:
                    HAL_GPIO_WritePin(GPIOA, LED0_Pin, GPIO_PIN_RESET);
                    break;
                case 2:
                    HAL_GPIO_WritePin(GPIOA, LED1_Pin, GPIO_PIN_RESET);
                    break;
                case 3:
                    HAL_GPIO_WritePin(GPIOA, LED2_Pin, GPIO_PIN_RESET);
                    break;
                case 4:
                    HAL_GPIO_WritePin(GPIOA, LED3_Pin, GPIO_PIN_RESET);
                    break;
            }
            break;
        case 2:
            HAL_GPIO_WritePin(GPIOB, LED5_Pin | LED6_Pin | LED7_Pin | LED8_Pin, GPIO_PIN_SET);
            switch(RxIndex)
            {
                case 1:
                    HAL_GPIO_WritePin(GPIOB, LED5_Pin, GPIO_PIN_RESET);
                    break;
                case 2:
                    HAL_GPIO_WritePin(GPIOB, LED6_Pin, GPIO_PIN_RESET);
                    break;
                case 3:
                    HAL_GPIO_WritePin(GPIOB, LED7_Pin, GPIO_PIN_RESET);
                    break;
                case 4:
                    HAL_GPIO_WritePin(GPIOB, LED8_Pin, GPIO_PIN_RESET);
                    break;
            }
            break;
        case 3:
            HAL_GPIO_WritePin(GPIOC, LED13_Pin| LED14_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOB, LED15_Pin| LED16_Pin, GPIO_PIN_SET);
            switch(RxIndex)
            {
                case 1:
                    HAL_GPIO_WritePin(GPIOC, LED13_Pin,GPIO_PIN_RESET);
                    break;
                case 2:
                    HAL_GPIO_WritePin(GPIOC, LED14_Pin,GPIO_PIN_RESET);
                    break;
                case 3:
                    HAL_GPIO_WritePin(GPIOB, LED15_Pin,GPIO_PIN_RESET);
                    break;
                case 4:
                    HAL_GPIO_WritePin(GPIOB, LED16_Pin,GPIO_PIN_RESET);
                    break;
            }
            break;
        case 4:
            HAL_GPIO_WritePin(GPIOA, LED10_Pin | LED11_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOC, LED9_Pin  | LED12_Pin, GPIO_PIN_SET);
            switch(RxIndex)
            {
                case 1:
                    HAL_GPIO_WritePin(GPIOC, LED9_Pin,GPIO_PIN_RESET);
                    break;
                case 2:
                    HAL_GPIO_WritePin(GPIOA, LED10_Pin,GPIO_PIN_RESET);
                    break;
                case 3:
                    HAL_GPIO_WritePin(GPIOA, LED11_Pin,GPIO_PIN_RESET);
                    break;
                case 4:
                    HAL_GPIO_WritePin(GPIOC, LED12_Pin,GPIO_PIN_RESET);
                    break;
            }
            break;
    }
    return;
#endif
}

#endif

