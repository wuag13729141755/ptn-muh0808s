/******************************************************
**Copyright (c) 2008 PTN Electronics Limited.All Rights Reserved
**FileName:     drv_LM74HC595.h
**Writer:       wf8421
**Create Date:  2018-12-3
**Rewriter:     wf8421
**Rewrite Date: 2018-12-3
**Description:  595驱动及数据功能处理
**History:
**Version:      V1.0.0
******************************************************/
#ifndef __DRV_LM74HC595_H__
#define __DRV_LM74HC595_H__


#ifdef __DRV_LM74HC595_C__
#define INTERFACE
#else
#define INTERFACE   extern
#endif

INTERFACE void mapp_Set595Value(u32 u32Value);
INTERFACE u32 mapp_Get595Value(void);

INTERFACE void mdrv_595Init(void);
INTERFACE void mdrv_595SendData(u8 u8Id,u32 u32data);
INTERFACE bool mapp_Change595ValueProcess(void);
INTERFACE eSpiResult_t drv_595_bus_direct_write(uint8_t u8Dev,
                                uint16_t u16Count,
                                uint8_t* pu8Buffer);

#undef INTERFACE
#endif  // #ifndef __DRV_LM74HC595_H__

