/******************************************************
**Copyright (c) 2008 PTN Electronics Limited.All Rights Reserved
**FileName:     Task_INV767X.c
**Writer:       wf8421
**Create Date:  2021-02-19
**Rewriter:     wf8421
**Rewrite Date: 2021-02-19
**Description:  ADIÏµÁÐÐ¾Æ¬task³ÌÐò
**History:
**Version:      V1.0.0
******************************************************/
#include "includes.h"

#if _ENABLE_CHIP_INV479x_SERIES
#include "inv479x_main.h"
vvFuncPoint pvvFuncInvUserFunctionInit = NULL;
vvFuncPoint pvvFuncInvUserFunctionProcess = NULL;


void vTask_InvSeriers(void *pvParameters)
{
	(void)pvParameters;

    mapp_Inv479xInit();
    if(pvvFuncInvUserFunctionInit != NULL)
    {
        pvvFuncInvUserFunctionInit();
    }

  	while (1)
  	{
        mapp_Inv479xMain();
        if(pvvFuncInvUserFunctionProcess != NULL)
        {
            pvvFuncInvUserFunctionProcess();
        }
        vTaskDelay(10);
  	}
}

#endif

