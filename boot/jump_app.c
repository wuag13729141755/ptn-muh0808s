/*****************************************************************************
Copyright       : PTN. Co., Ltd.
File name       : jump_app.c
Description     : 实现boot跳转到APP区
Author          : wf8421
Version         : V1.0.1
Date            : 2023-5-23
History         :
*****************************************************************************/
#include "boot.h"


typedef void (*pfun)(void);
pfun jump_fun;

/*****************************************************************************
Function        : void to_jump_app(uint32_t addr)
Description     : 跳转到app区
Calls           : null
Called By       : null
Table Accessed  : null
Table Updated   : null
Input           : null
Output          : null
Return          : null
Others          : null
*****************************************************************************/
void to_jump_app(uint32_t addr)
{
    uint32_t jump_addr= 0;
    uint32_t app_addr = 0;
    app_addr = addr;
    if(((*(uint32_t *)app_addr) & 0x2f000000) != 0x20000000)
//    if((*(uint32_t *)app_addr) < 0x20000000)
    {
        dbg_printf("please check soft filed 0x%x!\r\n",(*(uint32_t *)app_addr));
        for(;;) {
            ;
        }
    } else {
        dbg_printf("jump to app!!!\r\n");
        jump_addr = *(uint32_t *)(addr+4);
        jump_fun = (pfun)jump_addr;
        __set_MSP(*(uint32_t *)addr);
        jump_fun();
    }
}













