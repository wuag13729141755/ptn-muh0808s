/******************************************************
**Copyright (c) 2008 PTN Electronics Limited.All Rights Reserved
**FileName: drv_Flash.c
**Writer:       wf8421
**Create Date:  2018-5-16
**Rewriter:     wf8421
**Rewrite Date: 2018-5-16
**Description:  MCU 内部Flash 读写功能
**History:
**Version:  V1.0.0
******************************************************/
#define __DRV_FLASH_C__
#ifdef STM32F2XX_HD
#include "stm32f2xx_flash.h"
#elif (defined(N32G45X))
//#include ""
#elif (defined(GD32F4XX))
#include "gd32f4xx_fmc.h"
#else
#include "stm32f10x_flash.h"
#endif
#include <stdio.h>
#include <string.h>
#include "Datatype.h"
#include "drv_Flash.h"

#define FLASH_PRINT         RS232_printf
#define DBG_FLASH(x)

#ifdef STM32F2XX_HD
//////////////////////////////////////////////////////////////////////////////////////
u32 STM32F4FLASH_ReadWord(u32 FlashAddr)
{
    return *(vu32*)FlashAddr;
}

u32 STM32F4FLASH_GetFlashSector(u32 addr)
{
    if(addr < ADDR_FLASH_SECTOR_1)
        return FLASH_Sector_0;
    else if(addr < ADDR_FLASH_SECTOR_2)
        return FLASH_Sector_1;
    else if(addr < ADDR_FLASH_SECTOR_3)
        return FLASH_Sector_2;
    else if(addr < ADDR_FLASH_SECTOR_4)
        return FLASH_Sector_3;
    else if(addr < ADDR_FLASH_SECTOR_5)
        return FLASH_Sector_4;
    else if(addr < ADDR_FLASH_SECTOR_6)
        return FLASH_Sector_5;
    else if(addr < ADDR_FLASH_SECTOR_7)
        return FLASH_Sector_6;
    else if(addr < ADDR_FLASH_SECTOR_8)
        return FLASH_Sector_7;
    else if(addr < ADDR_FLASH_SECTOR_9)
        return FLASH_Sector_8;
    else if(addr < ADDR_FLASH_SECTOR_10)
        return FLASH_Sector_9;
    else if(addr < ADDR_FLASH_SECTOR_11)
        return FLASH_Sector_10;
    else return FLASH_Sector_11;
}

FLASH_Status STM32F4FLASH_CheckEmpty(u32 StartAddr,u32 EndAddr)
{
    FLASH_Status status = FLASH_COMPLETE;

    if(StartAddr < 0x1fff0000)
    {
        while(StartAddr < EndAddr)
        {
            if(STM32F4FLASH_ReadWord(StartAddr) != 0xffffffff)
            {
                status = FLASH_EraseSector(STM32F4FLASH_GetFlashSector(StartAddr),VoltageRange_3);
                if(status != FLASH_COMPLETE)
                    break;
            }
            else
                StartAddr+=4;
        }
    }
    else
    {
        status = FLASH_ERROR_PGS;
    }
    return status;
}

void mdrv_FlashWrite(u32 WriteAddr,u32 *pBuffer,u32 NumToWrite)
{
    FLASH_Status status = FLASH_COMPLETE;
    //static u8 f = 1;
    u32 addrx=0;
    u32 endaddr=0;
  //char temp[20];
    if(WriteAddr<STM32_FLASH_BASE||WriteAddr%4) return; //非法地址
    FLASH_Unlock();
    FLASH_DataCacheCmd(DISABLE);//FLASH擦除期间,必须禁止数据缓存
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR |
                  FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR);
    //FLASH_OB_Unlock();
    //解锁

    addrx=WriteAddr;                //写入的起始地址
    endaddr=WriteAddr+NumToWrite;   //写入的结束地址

    if(addrx<0X1FFF0000)            //只有主存储区,才需要执行擦除操作!!
    {
        while(addrx<endaddr)        //扫清一切障碍.(对非FFFFFFFF的地方,先擦除)
        {
            if(STM32F4FLASH_ReadWord(addrx)!=0XFFFFFFFF)//有非0XFFFFFFFF的地方,要擦除这个扇区
            {
                status=FLASH_EraseSector(STM32F4FLASH_GetFlashSector(addrx),VoltageRange_3);//VCC=2.7~3.6V之间!!
                if(status!=FLASH_COMPLETE)
                {
                    break;  //发生错误了
                }
            }
            else
            {
                    addrx+=4;
            }
        }
    }
    if(status==FLASH_COMPLETE)
    {
        while(WriteAddr<endaddr)//写数据
        {
            if(FLASH_ProgramWord(WriteAddr,*pBuffer)!=FLASH_COMPLETE)//写入数据
            {
                break;  //写入异常
            }
            WriteAddr+=4;
            pBuffer++;
        }
    }
    FLASH_DataCacheCmd(ENABLE); //FLASH擦除结束,开启数据缓存
    //FLASH_OB_Lock();
    FLASH_Lock();//上锁
}
//从指定地址开始读出指定长度的数据
//ReadAddr:起始地址
//pBuffer:数据指针
//NumToRead:字(4位)数
void mdrv_FlashRead(u32 ReadAddr,u32 *pBuffer,u32 NumToRead)
{
    u32 i;
    for(i=0;i<NumToRead;i++)
    {
        pBuffer[i]=STM32F4FLASH_ReadWord(ReadAddr);//读取4个字节.
        ReadAddr+=4;//偏移4个字节.
    }
}
#elif (defined(N32G45X))
//读取指定地址的半字(16位数据)
//faddr:读地址(此地址必须为2的倍数!!)
//返回值:对应数据.
u32 STMFLASH_ReadWord(u32 faddr)
{
    return *(vu32*)faddr;
}

//不检查的写入
//WriteAddr:起始地址
//pBuffer:数据指针
//NumToWrite:半字(16位)数
void STMFLASH_Write_NoCheck(u32 WriteAddr,u32 *pBuffer,u16 NumToWrite)
{
    u16 i;
    for(i=0;i<NumToWrite;i=i+1)
    {
        FLASH_ProgramWord(WriteAddr,pBuffer[i]);
        WriteAddr+=4;//地址增加=4.
    }
}
//从指定地址开始写入指定长度的数据
//WriteAddr:起始地址(此地址必须为2的倍数!!)
//pBuffer:数据指针
//NumToWrite:字(32位)数(就是要写入的32位数据的个数.)
#if STM32_FLASH_SIZE<256
#define STM_SECTOR_SIZE 1024 //字节
#else
#define STM_SECTOR_SIZE 2048
#endif
u32 STMFLASH_BUF[STM_SECTOR_SIZE/4];//最多是2K字节

void mdrv_FlashWrite(u32 WriteAddr,u32 *pBuffer,u16 NumToWrite)
{
    u32 secpos;    //扇区地址
    u16 secoff;    //扇区内偏移地址(16位字计算)
    u16 secremain; //扇区内剩余地址(16位字计算)
    u16 i;
    u32 offaddr;   //去掉0X08000000后的地址

    FLASH_Unlock();                     //解锁
    offaddr=WriteAddr-STM32_FLASH_BASE;     //实际偏移地址.
    secpos=offaddr/STM_SECTOR_SIZE;         //扇区地址  0~127 for STM32F103RBT6
    secoff=(offaddr%STM_SECTOR_SIZE)/2;     //在扇区内的偏移(2个字节为基本单位.)
    secremain=STM_SECTOR_SIZE/2-secoff;     //扇区剩余空间大小
    if(NumToWrite<=secremain)
        secremain=NumToWrite;//不大于该扇区范围

    while(1)
    {
        mdrv_FlashRead(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE/4);//读出整个扇区的内容
        for(i=0;i<secremain;i++)//校验数据
        {
            if(STMFLASH_BUF[secoff+i]!=0XFFFF)break;//需要擦除
        }
        if(i<secremain)//需要擦除
        {
            FLASH_EraseOnePage(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE);//擦除这个扇区
            for(i=0;i<secremain;i++)//复制
            {
                STMFLASH_BUF[i+secoff]=pBuffer[i];
            }
            STMFLASH_Write_NoCheck(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE/4);//写入整个扇区
        }
        else
            STMFLASH_Write_NoCheck(WriteAddr,pBuffer,secremain);//写已经擦除了的,直接写入扇区剩余区间.

        if(NumToWrite==secremain)
            break;//写入结束了
        else//写入未结束
        {
            secpos++;               //扇区地址增1
            secoff=0;               //偏移位置为0
            pBuffer+=secremain;     //指针偏移
            WriteAddr+=secremain;   //写地址偏移
            NumToWrite-=secremain;  //字节(16位)数递减
            if(NumToWrite>(STM_SECTOR_SIZE/4))secremain=STM_SECTOR_SIZE/4;//下一个扇区还是写不完
            else secremain=NumToWrite;//下一个扇区可以写完了
        }
    };
    FLASH_Lock();//上锁
}

//从指定地址开始读出指定长度的数据
//ReadAddr:起始地址
//pBuffer:数据指针
//NumToWrite:半字(16位)数
void mdrv_FlashRead(u32 ReadAddr,u32 *pBuffer,u16 NumToRead)
{
    uint32_t addr = 0x08016000 + 400*1024;
    u16 i;

    if(ReadAddr >= addr)
    {
        for(i=0;i<NumToRead;i++)
        {
            pBuffer[i]=0x00000000;
        }

        return;
    }

    for(i=0;i<NumToRead;i++)
    {
        pBuffer[i]=STMFLASH_ReadWord(ReadAddr);//读取4个字节.
        ReadAddr+=4;//偏移2个字节.
    }
}
#elif (defined(GD32F4XX))
u32 GD32F4FLASH_ReadWord(u32 FlashAddr)
{
    return *(vu32*)FlashAddr;
}

u32 GD32F4FLASH_GetFlashSector(u32 addr)
{
    u32 sector = 0U;

    if ((addr < ADDR_FMC_SECTOR_1) && (addr >= ADDR_FMC_SECTOR_0)) {
        sector = CTL_SECTOR_NUMBER_0;
    } else if ((addr < ADDR_FMC_SECTOR_2) && (addr >= ADDR_FMC_SECTOR_1)) {
        sector = CTL_SECTOR_NUMBER_1;
    } else if ((addr < ADDR_FMC_SECTOR_3) && (addr >= ADDR_FMC_SECTOR_2)) {
        sector = CTL_SECTOR_NUMBER_2;
    } else if ((addr < ADDR_FMC_SECTOR_4) && (addr >= ADDR_FMC_SECTOR_3)) {
        sector = CTL_SECTOR_NUMBER_3;
    } else if ((addr < ADDR_FMC_SECTOR_5) && (addr >= ADDR_FMC_SECTOR_4)) {
        sector = CTL_SECTOR_NUMBER_4;
    } else if ((addr < ADDR_FMC_SECTOR_6) && (addr >= ADDR_FMC_SECTOR_5)) {
        sector = CTL_SECTOR_NUMBER_5;
    } else if ((addr < ADDR_FMC_SECTOR_7) && (addr >= ADDR_FMC_SECTOR_6)) {
        sector = CTL_SECTOR_NUMBER_6;
    } else if ((addr < ADDR_FMC_SECTOR_8) && (addr >= ADDR_FMC_SECTOR_7)) {
        sector = CTL_SECTOR_NUMBER_7;
    } else if ((addr < ADDR_FMC_SECTOR_9) && (addr >= ADDR_FMC_SECTOR_8)) {
        sector = CTL_SECTOR_NUMBER_8;
    } else if ((addr < ADDR_FMC_SECTOR_10) && (addr >= ADDR_FMC_SECTOR_9)) {
        sector = CTL_SECTOR_NUMBER_9;
    } else if ((addr < ADDR_FMC_SECTOR_11) && (addr >= ADDR_FMC_SECTOR_10)) {
        sector = CTL_SECTOR_NUMBER_10;
    } else if ((addr < ADDR_FMC_SECTOR_12) && (addr >= ADDR_FMC_SECTOR_11)) {
        sector = CTL_SECTOR_NUMBER_11;
    } else if ((addr < ADDR_FMC_SECTOR_13) && (addr >= ADDR_FMC_SECTOR_12)) {
        sector = CTL_SECTOR_NUMBER_12;
    } else if ((addr < ADDR_FMC_SECTOR_14) && (addr >= ADDR_FMC_SECTOR_13)) {
        sector = CTL_SECTOR_NUMBER_13;
    } else if ((addr < ADDR_FMC_SECTOR_15) && (addr >= ADDR_FMC_SECTOR_14)) {
        sector = CTL_SECTOR_NUMBER_14;
    } else if ((addr < ADDR_FMC_SECTOR_16) && (addr >= ADDR_FMC_SECTOR_15)) {
        sector = CTL_SECTOR_NUMBER_15;
    } else if ((addr < ADDR_FMC_SECTOR_17) && (addr >= ADDR_FMC_SECTOR_16)) {
        sector = CTL_SECTOR_NUMBER_16;
    } else if ((addr < ADDR_FMC_SECTOR_18) && (addr >= ADDR_FMC_SECTOR_17)) {
        sector = CTL_SECTOR_NUMBER_17;
    } else if ((addr < ADDR_FMC_SECTOR_19) && (addr >= ADDR_FMC_SECTOR_18)) {
        sector = CTL_SECTOR_NUMBER_18;
    } else if ((addr < ADDR_FMC_SECTOR_20) && (addr >= ADDR_FMC_SECTOR_19)) {
        sector = CTL_SECTOR_NUMBER_19;
    } else if ((addr < ADDR_FMC_SECTOR_21) && (addr >= ADDR_FMC_SECTOR_20)) {
        sector = CTL_SECTOR_NUMBER_20;
    } else if ((addr < ADDR_FMC_SECTOR_22) && (addr >= ADDR_FMC_SECTOR_21)) {
        sector = CTL_SECTOR_NUMBER_21;
    } else if ((addr < ADDR_FMC_SECTOR_23) && (addr >= ADDR_FMC_SECTOR_22)) {
        sector = CTL_SECTOR_NUMBER_22;
    } else if ((addr < ADDR_FMC_SECTOR_24) && (addr >= ADDR_FMC_SECTOR_23)) {
        sector = CTL_SECTOR_NUMBER_23;
    } else if ((addr < ADDR_FMC_SECTOR_25) && (addr >= ADDR_FMC_SECTOR_24)) {
        sector = CTL_SECTOR_NUMBER_24;
    } else if ((addr < ADDR_FMC_SECTOR_26) && (addr >= ADDR_FMC_SECTOR_25)) {
        sector = CTL_SECTOR_NUMBER_25;
    } else if ((addr < ADDR_FMC_SECTOR_27) && (addr >= ADDR_FMC_SECTOR_26)) {
        sector = CTL_SECTOR_NUMBER_26;
    } else {
    }

    return sector;
}

fmc_state_enum GD32F4FLASH_CheckEmpty(u32 StartAddr,u32 EndAddr)
{
    fmc_state_enum status = FMC_READY;

    if(StartAddr < 0x1fff0000)
    {
        while(StartAddr < EndAddr)
        {
            if(GD32F4FLASH_ReadWord(StartAddr) != 0xffffffff)
            {
                status = fmc_sector_erase(GD32F4FLASH_GetFlashSector(StartAddr));
                if(status != FMC_READY)
                    break;
            }
            else
                StartAddr+=4;
        }
    }
    else
    {
        status = FMC_PGSERR;
    }
    return status;
}

void mdrv_FlashWrite(u32 WriteAddr,u32 *pBuffer,u32 NumToWrite)
{
    fmc_state_enum status = FMC_READY;
    //static u8 f = 1;
    u32 addrx=0;
    u32 endaddr=0;
  //char temp[20];
    if(WriteAddr<STM32_FLASH_BASE||WriteAddr%4) return; //非法地址
    fmc_unlock();
    fmc_flag_clear(FMC_FLAG_END | FMC_FLAG_OPERR | FMC_FLAG_WPERR | FMC_FLAG_PGMERR | FMC_FLAG_PGSERR);
    //解锁

    addrx=WriteAddr;                //写入的起始地址
    endaddr=WriteAddr+NumToWrite;   //写入的结束地址

    if(addrx<0X1FFF0000)            //只有主存储区,才需要执行擦除操作!!
    {
        while(addrx<endaddr)        //扫清一切障碍.(对非FFFFFFFF的地方,先擦除)
        {
            if(GD32F4FLASH_ReadWord(addrx)!=0XFFFFFFFF)//有非0XFFFFFFFF的地方,要擦除这个扇区
            {
                status=fmc_sector_erase(GD32F4FLASH_GetFlashSector(addrx));//VCC=2.7~3.6V之间!!
                if(status!=FMC_READY)
                {
                    break;  //发生错误了
                }
            }
            else
            {
                    addrx+=4;
            }
        }
    }
    if(status==FMC_READY)
    {
        while(WriteAddr<endaddr)//写数据
        {
            if(fmc_word_program(WriteAddr,*pBuffer)!=FMC_READY)//写入数据
            {
                break;  //写入异常
            }
            WriteAddr+=4;
            pBuffer++;
        }
    }
    fmc_lock();//上锁
}
//从指定地址开始读出指定长度的数据
//ReadAddr:起始地址
//pBuffer:数据指针
//NumToRead:字(4位)数
void mdrv_FlashRead(u32 ReadAddr,u32 *pBuffer,u32 NumToRead)
{
    u32 i;
    for(i=0;i<NumToRead;i++)
    {
        pBuffer[i]=GD32F4FLASH_ReadWord(ReadAddr);//读取4个字节.
        ReadAddr+=4;//偏移4个字节.
    }
}

#else
//读取指定地址的半字(16位数据)
//faddr:读地址(此地址必须为2的倍数!!)
//返回值:对应数据.
u16 STMFLASH_ReadHalfWord(u32 faddr)
{
    return *(vu16*)faddr;
}

//不检查的写入
//WriteAddr:起始地址
//pBuffer:数据指针
//NumToWrite:半字(16位)数
void STMFLASH_Write_NoCheck(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)
{
    u16 i;
    for(i=0;i<NumToWrite;i++)
    {
        FLASH_ProgramHalfWord(WriteAddr,pBuffer[i]);
        WriteAddr+=2;//地址增加2.
    }
}
//从指定地址开始写入指定长度的数据
//WriteAddr:起始地址(此地址必须为2的倍数!!)
//pBuffer:数据指针
//NumToWrite:半字(16位)数(就是要写入的16位数据的个数.)
#if STM32_FLASH_SIZE<256
#define STM_SECTOR_SIZE 1024 //字节
#else
#define STM_SECTOR_SIZE 2048
#endif
u16 STMFLASH_BUF[STM_SECTOR_SIZE/2];//最多是2K字节

extern uint32_t flash_flag;

void mdrv_FlashWrite(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)
{
    u32 secpos;    //扇区地址
    u16 secoff;    //扇区内偏移地址(16位字计算)
    u16 secremain; //扇区内剩余地址(16位字计算)
    u16 i;
    u32 offaddr;   //去掉0X08000000后的地址

    FLASH_Unlock();                     //解锁
    offaddr=WriteAddr-STM32_FLASH_BASE;     //实际偏移地址.
    secpos=offaddr/STM_SECTOR_SIZE;         //扇区地址  0~127 for STM32F103RBT6
    secoff=(offaddr%STM_SECTOR_SIZE)/2;     //在扇区内的偏移(2个字节为基本单位.)
    secremain=STM_SECTOR_SIZE/2-secoff;     //扇区剩余空间大小
    if(NumToWrite<=secremain)
        secremain=NumToWrite;//不大于该扇区范围

    while(1)
    {
        mdrv_FlashRead(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE/2);//读出整个扇区的内容
        for(i=0;i<secremain;i++)//校验数据
        {
            if(STMFLASH_BUF[secoff+i]!=0XFFFF)break;//需要擦除
        }
        if(i<secremain)//需要擦除
        {
            FLASH_ErasePage(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE);//擦除这个扇区
            for(i=0;i<secremain;i++)//复制
            {
                STMFLASH_BUF[i+secoff]=pBuffer[i];
            }
            STMFLASH_Write_NoCheck(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE/2);//写入整个扇区
        }
        else
            STMFLASH_Write_NoCheck(WriteAddr,pBuffer,secremain);//写已经擦除了的,直接写入扇区剩余区间.

        if(NumToWrite==secremain)
            break;//写入结束了
        else//写入未结束
        {
            secpos++;               //扇区地址增1
            secoff=0;               //偏移位置为0
            pBuffer+=secremain;     //指针偏移
            WriteAddr+=secremain;   //写地址偏移
            NumToWrite-=secremain;  //字节(16位)数递减
            if(NumToWrite>(STM_SECTOR_SIZE/2))secremain=STM_SECTOR_SIZE/2;//下一个扇区还是写不完
            else secremain=NumToWrite;//下一个扇区可以写完了
        }
    };
    FLASH_Lock();//上锁
}

//从指定地址开始读出指定长度的数据
//ReadAddr:起始地址
//pBuffer:数据指针
//NumToWrite:半字(16位)数
void mdrv_FlashRead(u32 ReadAddr,u16 *pBuffer,u16 NumToRead)
{
    uint32_t addr = 0x08016000 + 400*1024;
    u16 i;

    if(ReadAddr >= addr)
    {
        for(i=0;i<NumToRead;i++)
        {
            pBuffer[i]=0x0000;
        }

        return;
    }

    for(i=0;i<NumToRead;i++)
    {
        pBuffer[i]=STMFLASH_ReadHalfWord(ReadAddr);//读取2个字节.
        ReadAddr+=2;//偏移2个字节.
    }
}
#endif

uint8_t Flash_Erase(uint32_t u32Address)
{
    uint8_t u8Ret = 0;

#ifdef N32G45X
    if(FLASH_EraseOnePage(u32Address)!=FLASH_COMPL)
        u8Ret = 1;
#elif (defined(GD32F4XX))
    if(fmc_sector_erase(GD32F4FLASH_GetFlashSector(u32Address))!=FMC_READY)
        u8Ret = 1;
#elif (defined(STM32F2XX_HD))
    if(FLASH_EraseSector(STM32F4FLASH_GetFlashSector(u32Address),VoltageRange_3)!=FLASH_COMPLETE)
        u8Ret = 1;
#else
    if(FLASH_ErasePage(u32Address)!=FLASH_COMPLETE)
        u8Ret = 1;
#endif
    return u8Ret;
}

uint8_t Flash_WriteWord(uint32_t u32Address, uint32_t u32Data)
{
    uint8_t u8Ret = 0;

#ifdef N32G45X
    if(FLASH_ProgramWord(u32Address,u32Data)!=FLASH_COMPL)
        u8Ret = 1;
#elif (defined(GD32F4XX))
    if(fmc_word_program(u32Address,u32Data)!=FMC_READY)
        u8Ret = 1;
#elif (defined(STM32F2XX_HD))
    if(FLASH_ProgramWord(u32Address,u32Data)!=FLASH_COMPLETE)
        u8Ret = 1;
#else
    if(FLASH_ProgramWord(u32Address,u32Data)!=FLASH_COMPLETE)
        u8Ret = 1;
#endif
    return u8Ret;
}

void Flash_Unlocked(void)
{
#ifdef N32G45X
    FLASH_Unlock();
#elif (defined(GD32F4XX))
    fmc_unlock();
#elif (defined(STM32F2XX_HD))
    FLASH_Unlock();
#else
    FLASH_Unlock();
#endif
}

void Flash_Locked(void)
{
#ifdef N32G45X
    FLASH_Lock();
#elif (defined(GD32F4XX))
    fmc_lock();
#elif (defined(STM32F2XX_HD))
    FLASH_Lock();
#else
    FLASH_Lock();
#endif
}

