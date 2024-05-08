/******************************************************
**Copyright (c) 2008 PTN Electronics Limited.All Rights Reserved
**FileName:     Task_CheckChipID.c
**Writer:       wf8421
**Create Date:  2018-12-3
**Rewriter:     wf8421
**Rewrite Date: 2018-12-3
**Description:  校验芯片ID是否为同一个芯片
**History:
**Version:      V1.0.0
******************************************************/

#include "includes.h"
#if _ENABLE_STARTUP_TO_MATCHING_CHIPID
#define DBG_CHIPID(x)       //Uart1_printf x

static bool sg_bChipIDErrFlag = FALSE;

void vTaskCheckChipID(void *pvParameters)
{
    static bool sl_bStartupCheckFlag = TRUE;
    (void)pvParameters;

    sg_bChipIDErrFlag = FALSE;
    for(;;)
    {
        if(sl_bStartupCheckFlag)
        {
            sl_bStartupCheckFlag = FALSE;
            #if _ENABLE_STARTUP_TO_MATCHING_CHIPID
            mapp_StartupToMatchingChipID();
            #endif
        }
        mapp_ChipIDErrToDoFunction();
        delay_ms(10);
    }
}

void mapp_StartupToMatchingChipID(void)
{
    uint8_t l_u8ProductID[256] = {0};
    uint8_t j;
    uint16_t i;
    uint32_t l_u32ChipID[3];
    static bool sl_bSetFlashIdCheckFlag = FALSE;

    Get_ChipID(l_u32ChipID);
    //DBG_CHIPID(("\r\n"));
    //DBG_CHIPID(("l_u32ChipID[0] = 0x%08x\r\n",l_u32ChipID[0]));
    //DBG_CHIPID(("l_u32ChipID[1] = 0x%08x\r\n",l_u32ChipID[1]));
    //DBG_CHIPID(("l_u32ChipID[2] = 0x%08x\r\n",l_u32ChipID[2]));
    // 读取出整块数据块
    #ifdef STM32F2XX_HD
    mdrv_FlashRead(FLASH_ADDR_PROEDUCT_ID,(uint32_t*)l_u8ProductID,256/4);
    #elif (defined(N32G45X))
    mdrv_FlashRead(FLASH_ADDR_PROEDUCT_ID,(uint32_t*)l_u8ProductID,256/4);
    #elif (defined(GD32F4XX))
    mdrv_FlashRead(FLASH_ADDR_PROEDUCT_ID,(uint32_t*)l_u8ProductID,256/4);
    #else
    mdrv_FlashRead(FLASH_ADDR_PROEDUCT_ID,(uint16_t*)l_u8ProductID,256/2);
    #endif
    for(i=0;i<256;i++)
    {
        if(l_u8ProductID[i]!=0x00&&l_u8ProductID[i]!=0xFF)
        {
            sl_bSetFlashIdCheckFlag = TRUE;
            break;
        }
    }
    if(sl_bSetFlashIdCheckFlag == FALSE)
    {
        for(i=0;i<256;i++)
        {
            l_u8ProductID[i]=(i^0xA5);      // 将所有数据异或A5进行存储
        }
        for(j=0;j<3;j++)
        {
            for(i=0;i<4;i++)
            {
                l_u8ProductID[(j<<4)+(i<<2)]/*[j*16+i*4]*/ = (uint8_t)((l_u32ChipID[j]>>/*(i*8)*/(i<<3))&0xFF)^0xA5;     // 将ID值异或A5存放到特定位置
            }
        }
        #ifdef STM32F2XX_HD
        mdrv_FlashWrite(FLASH_ADDR_PROEDUCT_ID,(uint32_t*)l_u8ProductID,256);
        #elif (defined(N32G45X))
        mdrv_FlashWrite(FLASH_ADDR_PROEDUCT_ID,(uint32_t*)l_u8ProductID,256);
        #elif (defined(GD32F4XX))
        mdrv_FlashWrite(FLASH_ADDR_PROEDUCT_ID,(uint32_t*)l_u8ProductID,256);
        #else
        mdrv_FlashWrite(FLASH_ADDR_PROEDUCT_ID,(uint16_t*)l_u8ProductID,256/2);
        #endif
    }
    else
    {
        for(j=0;j<3;j++)
        {
            for(i=0;i<4;i++)
            {
                //DBG_CHIPID(("l_u8ProductID[%d] = 0x%02x\r\n",j*16+i*4,l_u8ProductID[j*16+i*4]));
                // 将特定位置的ID值取出，还原，然后跟当前芯片读出来的ID进行比较
                if((l_u8ProductID[(j<<4)+(i<<2)] /*[j*16+i*4]*/^0xA5) != (uint8_t)((l_u32ChipID[j]>>/*(i*8)*/(i<<3))&0xFF))
                {
                    //DBG_CHIPID(("No match to chip ID\r\n"));

                    sg_bChipIDErrFlag = TRUE;
                    #if 1
                    PlatformTimerSet(etSysChipIDErrFuncTimer,60000);
                    #else
                    SYS_SoftReset();
                    #endif
                }
            }
        }
    }
}

#ifndef mapp_SetPowerDownFunction
#define mapp_SetPowerDownFunction       __NOP
#endif

void mapp_ChipIDErrToDoFunction(void)
{
    if(sg_bChipIDErrFlag)
    {
        if(PlatformTimerExpired(etSysChipIDErrFuncTimer))
        {
            //DBG_CHIPID(("Chip ID Error to ShutDown and SYS_Standby!!!!\r\n"));
            //DBG_CHIPID(("Chip ID Error to ShutDown and SYS_Standby!!!!\r\n"));
            //DBG_CHIPID(("Chip ID Error to ShutDown and SYS_Standby!!!!\r\n"));
            //DBG_CHIPID(("Chip ID Error to ShutDown and SYS_Standby!!!!\r\n"));
            mapp_SetPowerDownFunction();
            SYS_Standby();
            while(1);
        }
    }
}

#endif

