/******************************************************
**Copyright (c) 2008 PTN Electronics Limited.All Rights Reserved
**FileName:     Task_Gsv2kx.h
**Writer:       wf8421
**Create Date:  2019-12-10
**Rewriter:     wf8421
**Rewrite Date: 2019-12-10
**Description:  GSVÏµÁÐÐ¾Æ¬task³ÌÐò
**History:
**Version:      V1.0.0
******************************************************/
#ifndef __TASK_GSV2KX_H__
#define __TASK_GSV2KX_H__

#include "av_main.h"

extern vvFuncPoint pvvFuncGsvUserFunctionInit;
extern vvFuncPoint pvvFuncGsvUserFunctionProcess;

void vTask_GsvSeriers(void *pvParameters);


#endif  // #ifndef __TASK_GSV2KX_H__

