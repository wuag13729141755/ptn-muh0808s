/******************************************************
**Copyright (c) 2008 PTN Electronics Limited.All Rights Reserved
**FileName:     Task_CheckChipID.h
**Writer:       wf8421
**Create Date:  2018-12-3
**Rewriter:     wf8421
**Rewrite Date: 2018-12-3
**Description:  У��оƬID�Ƿ�Ϊͬһ��оƬ
**History:
**Version:      V1.0.0
******************************************************/
#ifndef __TASK_CHECKCHIPID_H__
#define __TASK_CHECKCHIPID_H__


void vTaskCheckChipID(void *pvParameters);
void mapp_StartupToMatchingChipID(void);
void mapp_ChipIDErrToDoFunction(void);

#endif
