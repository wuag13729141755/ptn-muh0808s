/******************************************************
**Copyright (c) 2008 PTN Electronics Limited.All Rights Reserved
**FileName: drv_Flash.c
**Writer:       wf8421
**Create Date:  2018-5-16
**Rewriter:     wf8421
**Rewrite Date: 2018-5-16
**Description:  MCU �ڲ�Flash ��д����
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
    if(WriteAddr<STM32_FLASH_BASE||WriteAddr%4) return; //�Ƿ���ַ
    FLASH_Unlock();
    FLASH_DataCacheCmd(DISABLE);//FLASH�����ڼ�,�����ֹ���ݻ���
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR |
                  FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR);
    //FLASH_OB_Unlock();
    //����

    addrx=WriteAddr;                //д�����ʼ��ַ
    endaddr=WriteAddr+NumToWrite;   //д��Ľ�����ַ

    if(addrx<0X1FFF0000)            //ֻ�����洢��,����Ҫִ�в�������!!
    {
        while(addrx<endaddr)        //ɨ��һ���ϰ�.(�Է�FFFFFFFF�ĵط�,�Ȳ���)
        {
            if(STM32F4FLASH_ReadWord(addrx)!=0XFFFFFFFF)//�з�0XFFFFFFFF�ĵط�,Ҫ�����������
            {
                status=FLASH_EraseSector(STM32F4FLASH_GetFlashSector(addrx),VoltageRange_3);//VCC=2.7~3.6V֮��!!
                if(status!=FLASH_COMPLETE)
                {
                    break;  //����������
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
        while(WriteAddr<endaddr)//д����
        {
            if(FLASH_ProgramWord(WriteAddr,*pBuffer)!=FLASH_COMPLETE)//д������
            {
                break;  //д���쳣
            }
            WriteAddr+=4;
            pBuffer++;
        }
    }
    FLASH_DataCacheCmd(ENABLE); //FLASH��������,�������ݻ���
    //FLASH_OB_Lock();
    FLASH_Lock();//����
}
//��ָ����ַ��ʼ����ָ�����ȵ�����
//ReadAddr:��ʼ��ַ
//pBuffer:����ָ��
//NumToRead:��(4λ)��
void mdrv_FlashRead(u32 ReadAddr,u32 *pBuffer,u32 NumToRead)
{
    u32 i;
    for(i=0;i<NumToRead;i++)
    {
        pBuffer[i]=STM32F4FLASH_ReadWord(ReadAddr);//��ȡ4���ֽ�.
        ReadAddr+=4;//ƫ��4���ֽ�.
    }
}
#elif (defined(N32G45X))
//��ȡָ����ַ�İ���(16λ����)
//faddr:����ַ(�˵�ַ����Ϊ2�ı���!!)
//����ֵ:��Ӧ����.
u32 STMFLASH_ReadWord(u32 faddr)
{
    return *(vu32*)faddr;
}

//������д��
//WriteAddr:��ʼ��ַ
//pBuffer:����ָ��
//NumToWrite:����(16λ)��
void STMFLASH_Write_NoCheck(u32 WriteAddr,u32 *pBuffer,u16 NumToWrite)
{
    u16 i;
    for(i=0;i<NumToWrite;i=i+1)
    {
        FLASH_ProgramWord(WriteAddr,pBuffer[i]);
        WriteAddr+=4;//��ַ����=4.
    }
}
//��ָ����ַ��ʼд��ָ�����ȵ�����
//WriteAddr:��ʼ��ַ(�˵�ַ����Ϊ2�ı���!!)
//pBuffer:����ָ��
//NumToWrite:��(32λ)��(����Ҫд���32λ���ݵĸ���.)
#if STM32_FLASH_SIZE<256
#define STM_SECTOR_SIZE 1024 //�ֽ�
#else
#define STM_SECTOR_SIZE 2048
#endif
u32 STMFLASH_BUF[STM_SECTOR_SIZE/4];//�����2K�ֽ�

void mdrv_FlashWrite(u32 WriteAddr,u32 *pBuffer,u16 NumToWrite)
{
    u32 secpos;    //������ַ
    u16 secoff;    //������ƫ�Ƶ�ַ(16λ�ּ���)
    u16 secremain; //������ʣ���ַ(16λ�ּ���)
    u16 i;
    u32 offaddr;   //ȥ��0X08000000��ĵ�ַ

    FLASH_Unlock();                     //����
    offaddr=WriteAddr-STM32_FLASH_BASE;     //ʵ��ƫ�Ƶ�ַ.
    secpos=offaddr/STM_SECTOR_SIZE;         //������ַ  0~127 for STM32F103RBT6
    secoff=(offaddr%STM_SECTOR_SIZE)/2;     //�������ڵ�ƫ��(2���ֽ�Ϊ������λ.)
    secremain=STM_SECTOR_SIZE/2-secoff;     //����ʣ��ռ��С
    if(NumToWrite<=secremain)
        secremain=NumToWrite;//�����ڸ�������Χ

    while(1)
    {
        mdrv_FlashRead(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE/4);//������������������
        for(i=0;i<secremain;i++)//У������
        {
            if(STMFLASH_BUF[secoff+i]!=0XFFFF)break;//��Ҫ����
        }
        if(i<secremain)//��Ҫ����
        {
            FLASH_EraseOnePage(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE);//�����������
            for(i=0;i<secremain;i++)//����
            {
                STMFLASH_BUF[i+secoff]=pBuffer[i];
            }
            STMFLASH_Write_NoCheck(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE/4);//д����������
        }
        else
            STMFLASH_Write_NoCheck(WriteAddr,pBuffer,secremain);//д�Ѿ������˵�,ֱ��д������ʣ������.

        if(NumToWrite==secremain)
            break;//д�������
        else//д��δ����
        {
            secpos++;               //������ַ��1
            secoff=0;               //ƫ��λ��Ϊ0
            pBuffer+=secremain;     //ָ��ƫ��
            WriteAddr+=secremain;   //д��ַƫ��
            NumToWrite-=secremain;  //�ֽ�(16λ)���ݼ�
            if(NumToWrite>(STM_SECTOR_SIZE/4))secremain=STM_SECTOR_SIZE/4;//��һ����������д����
            else secremain=NumToWrite;//��һ����������д����
        }
    };
    FLASH_Lock();//����
}

//��ָ����ַ��ʼ����ָ�����ȵ�����
//ReadAddr:��ʼ��ַ
//pBuffer:����ָ��
//NumToWrite:����(16λ)��
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
        pBuffer[i]=STMFLASH_ReadWord(ReadAddr);//��ȡ4���ֽ�.
        ReadAddr+=4;//ƫ��2���ֽ�.
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
    if(WriteAddr<STM32_FLASH_BASE||WriteAddr%4) return; //�Ƿ���ַ
    fmc_unlock();
    fmc_flag_clear(FMC_FLAG_END | FMC_FLAG_OPERR | FMC_FLAG_WPERR | FMC_FLAG_PGMERR | FMC_FLAG_PGSERR);
    //����

    addrx=WriteAddr;                //д�����ʼ��ַ
    endaddr=WriteAddr+NumToWrite;   //д��Ľ�����ַ

    if(addrx<0X1FFF0000)            //ֻ�����洢��,����Ҫִ�в�������!!
    {
        while(addrx<endaddr)        //ɨ��һ���ϰ�.(�Է�FFFFFFFF�ĵط�,�Ȳ���)
        {
            if(GD32F4FLASH_ReadWord(addrx)!=0XFFFFFFFF)//�з�0XFFFFFFFF�ĵط�,Ҫ�����������
            {
                status=fmc_sector_erase(GD32F4FLASH_GetFlashSector(addrx));//VCC=2.7~3.6V֮��!!
                if(status!=FMC_READY)
                {
                    break;  //����������
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
        while(WriteAddr<endaddr)//д����
        {
            if(fmc_word_program(WriteAddr,*pBuffer)!=FMC_READY)//д������
            {
                break;  //д���쳣
            }
            WriteAddr+=4;
            pBuffer++;
        }
    }
    fmc_lock();//����
}
//��ָ����ַ��ʼ����ָ�����ȵ�����
//ReadAddr:��ʼ��ַ
//pBuffer:����ָ��
//NumToRead:��(4λ)��
void mdrv_FlashRead(u32 ReadAddr,u32 *pBuffer,u32 NumToRead)
{
    u32 i;
    for(i=0;i<NumToRead;i++)
    {
        pBuffer[i]=GD32F4FLASH_ReadWord(ReadAddr);//��ȡ4���ֽ�.
        ReadAddr+=4;//ƫ��4���ֽ�.
    }
}

#else
//��ȡָ����ַ�İ���(16λ����)
//faddr:����ַ(�˵�ַ����Ϊ2�ı���!!)
//����ֵ:��Ӧ����.
u16 STMFLASH_ReadHalfWord(u32 faddr)
{
    return *(vu16*)faddr;
}

//������д��
//WriteAddr:��ʼ��ַ
//pBuffer:����ָ��
//NumToWrite:����(16λ)��
void STMFLASH_Write_NoCheck(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)
{
    u16 i;
    for(i=0;i<NumToWrite;i++)
    {
        FLASH_ProgramHalfWord(WriteAddr,pBuffer[i]);
        WriteAddr+=2;//��ַ����2.
    }
}
//��ָ����ַ��ʼд��ָ�����ȵ�����
//WriteAddr:��ʼ��ַ(�˵�ַ����Ϊ2�ı���!!)
//pBuffer:����ָ��
//NumToWrite:����(16λ)��(����Ҫд���16λ���ݵĸ���.)
#if STM32_FLASH_SIZE<256
#define STM_SECTOR_SIZE 1024 //�ֽ�
#else
#define STM_SECTOR_SIZE 2048
#endif
u16 STMFLASH_BUF[STM_SECTOR_SIZE/2];//�����2K�ֽ�

extern uint32_t flash_flag;

void mdrv_FlashWrite(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)
{
    u32 secpos;    //������ַ
    u16 secoff;    //������ƫ�Ƶ�ַ(16λ�ּ���)
    u16 secremain; //������ʣ���ַ(16λ�ּ���)
    u16 i;
    u32 offaddr;   //ȥ��0X08000000��ĵ�ַ

    FLASH_Unlock();                     //����
    offaddr=WriteAddr-STM32_FLASH_BASE;     //ʵ��ƫ�Ƶ�ַ.
    secpos=offaddr/STM_SECTOR_SIZE;         //������ַ  0~127 for STM32F103RBT6
    secoff=(offaddr%STM_SECTOR_SIZE)/2;     //�������ڵ�ƫ��(2���ֽ�Ϊ������λ.)
    secremain=STM_SECTOR_SIZE/2-secoff;     //����ʣ��ռ��С
    if(NumToWrite<=secremain)
        secremain=NumToWrite;//�����ڸ�������Χ

    while(1)
    {
        mdrv_FlashRead(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE/2);//������������������
        for(i=0;i<secremain;i++)//У������
        {
            if(STMFLASH_BUF[secoff+i]!=0XFFFF)break;//��Ҫ����
        }
        if(i<secremain)//��Ҫ����
        {
            FLASH_ErasePage(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE);//�����������
            for(i=0;i<secremain;i++)//����
            {
                STMFLASH_BUF[i+secoff]=pBuffer[i];
            }
            STMFLASH_Write_NoCheck(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE/2);//д����������
        }
        else
            STMFLASH_Write_NoCheck(WriteAddr,pBuffer,secremain);//д�Ѿ������˵�,ֱ��д������ʣ������.

        if(NumToWrite==secremain)
            break;//д�������
        else//д��δ����
        {
            secpos++;               //������ַ��1
            secoff=0;               //ƫ��λ��Ϊ0
            pBuffer+=secremain;     //ָ��ƫ��
            WriteAddr+=secremain;   //д��ַƫ��
            NumToWrite-=secremain;  //�ֽ�(16λ)���ݼ�
            if(NumToWrite>(STM_SECTOR_SIZE/2))secremain=STM_SECTOR_SIZE/2;//��һ����������д����
            else secremain=NumToWrite;//��һ����������д����
        }
    };
    FLASH_Lock();//����
}

//��ָ����ַ��ʼ����ָ�����ȵ�����
//ReadAddr:��ʼ��ַ
//pBuffer:����ָ��
//NumToWrite:����(16λ)��
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
        pBuffer[i]=STMFLASH_ReadHalfWord(ReadAddr);//��ȡ2���ֽ�.
        ReadAddr+=2;//ƫ��2���ֽ�.
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

