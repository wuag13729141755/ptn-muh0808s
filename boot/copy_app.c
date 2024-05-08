/*****************************************************************************
Copyright       : PTN. Co., Ltd.
File name       : copy_app.c
Description     : 实现flash擦除，拷贝等功能
Author          : 靳凯凯
Version         : V1.0.0
Date            : 2021-6-23
History         :
*****************************************************************************/
#include "boot.h"

#if (defined(GD32F4XX))
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
#elif (defined(STM32F2XX_HD))
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

/*****************************************************************************
Function        : void erase_app(uint32_t start_app,uint8_t sector)
Description     : 擦除内部flash
Calls           : null
Called By       : null
Table Accessed  : null
Table Updated   : null
Input           : null
Output          : null
Return          : null
Others          : null
*****************************************************************************/
void erase_app(uint32_t start_app,uint8_t sector)
{
    uint8_t status = 0;
    while(sector--) {
        status = Flash_Erase(start_app);
        if(status) {
            dbg_printf("erase app flash error , palse check mcu!!!\r\n");
            return;
        }
        start_app += 2048;
    }
    dbg_printf("app is a copy!!!\r\n");
}

/*****************************************************************************
Function        : uint8_t check_data(uint32_t src_addr,uint32_t dec_addr)
Description     : 校验flash内容
Calls           : null
Called By       : null
Table Accessed  : null
Table Updated   : null
Input           : null
Output          : null
Return          : null
Others          : null
*****************************************************************************/
uint8_t check_data(uint32_t src_addr,uint32_t dec_addr)
{
    uint8_t flag = 0;
    if((*(uint32_t *)src_addr) != ((*(uint32_t *)dec_addr))) {
        flag = 1;
    }
    return flag;
}

/*****************************************************************************
Function        : void write_app(uint32_t app_addr,uint32_t backup_addr,uint32_t size)
Description     : 写内部flash
Calls           : null
Called By       : null
Table Accessed  : null
Table Updated   : null
Input           : null
Output          : null
Return          : null
Others          : null
*****************************************************************************/
void write_app(uint32_t app_addr,uint32_t backup_addr,uint32_t size)
{
    uint8_t status = 0;
    uint32_t write_addr = 0;
//    uint32_t read_addr = 0;
    uint32_t i = 0;
    write_addr = app_addr;
//    read_addr = backup_addr;
    for(i=0; i<size/4; i++) {
        status = Flash_WriteWord(write_addr,(uint32_t)(*(uint32_t *)backup_addr));
        if(status) {
            dbg_printf("app write error!please check mcu!!!\r\n");
        }
        if(check_data(write_addr,backup_addr) == 1) {
            dbg_printf("check data error!!!\r\n");
            for(;;) {
                ;
            }
        }
        write_addr += 4;
        backup_addr += 4;
    }
}

/*****************************************************************************
Function        : void to_copy_app(uint32_t app_addr,uint32_t backup_addr,uint32_t size)
Description     : 升级程序
Calls           : null
Called By       : null
Table Accessed  : null
Table Updated   : null
Input           : null
Output          : null
Return          : null
Others          : null
*****************************************************************************/
void to_copy_app(uint32_t app_addr,uint32_t backup_addr,uint32_t size)
{
    Flash_Unlocked();
    erase_app(app_addr,size/2048);
    write_app(app_addr,backup_addr,size);
    erase_app(DM_FW_FLAG_ADDR,1);
    Flash_WriteWord(DM_FW_FLAG_ADDR,DM_FW_FLAG_INVALID);
    Flash_Locked();
}











