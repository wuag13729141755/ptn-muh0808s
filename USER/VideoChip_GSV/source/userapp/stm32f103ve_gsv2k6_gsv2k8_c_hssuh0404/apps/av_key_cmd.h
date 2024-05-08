#ifndef __av_key_cmd_h
#define __av_key_cmd_h

#include "av_config.h"
#if 0//def GSV_BASE_PROJECT
#else
//#include "stm32f1xx_hal.h"
#endif

#if AvEnableKeyInput
enum
{
    AvKeyTx1RoutingLoop   = 0x01,
    AvKeyTx2RoutingLoop   = 0x02,
    AvKeyTx3RoutingLoop   = 0x04,
    AvKeyTx4RoutingLoop   = 0x08,
};
#endif

void ListenToKeyCommand(AvPort *port);
void TxInLedOut(uint8 TxIndex, uint8 RxIndex);
void Edid2p0LedOut(uint8 enable);

#endif
