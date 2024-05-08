/******************************************************
**Copyright (c) 2008 PTN Electronics Limited.All Rights Reserved
**FileName:         drv_LM74LS138.h
**Writer:           wf8421
**Create Date:  2019-11-11
**Rewriter:         wf8421
**Rewrite Date: 2019-11-11
**Description:  138驱动及数据功能处理
**History:
**Version:          V1.0.0
******************************************************/
#ifndef __DRV_LM74LM138_H__
#define __DRV_LM74LM138_H__

void drvSend138Data(u8 u8Dev,u8 u8Dat);
void mapp_Set138Value(u16 u16Data);

#endif

