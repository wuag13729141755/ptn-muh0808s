/******************************************************
**Copyright (c) 2008 PTN Electronics Limited.All Rights Reserved
**FileName:     Task_INV767X.h
**Writer:       wf8421
**Create Date:  2021-02-19
**Rewriter:     wf8421
**Rewrite Date: 2021-02-19
**Description:  ADIÏµÁÐÐ¾Æ¬task³ÌÐò
**History:
**Version:      V1.0.0
******************************************************/
#ifndef __TASK_INV767X_H__
#define __TASK_INV767X_H__

#include "Inv479x_main.h"

extern vvFuncPoint pvvFuncInvUserFunctionInit;
extern vvFuncPoint pvvFuncInvUserFunctionProcess;

void vTask_InvSeriers(void *pvParameters);


#endif  // #ifndef __TASK_GSV2KX_H__


