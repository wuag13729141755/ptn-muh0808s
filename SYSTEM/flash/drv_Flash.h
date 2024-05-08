#ifndef __DRV_FLASH_H__
#define __DRV_FLASH_H__

#ifdef STM32F2XX_HD
#include "stm32f2xx_flash.h"
#elif (defined(N32G45X))
#include "n32g45x.h"
#elif (defined(GD32F4XX))
#include "gd32f4xx_fmc.h"
#else
#include "stm32f10x_flash.h"
#endif
#include "Datatype.h"

#ifdef STM32F2XX_HD
#define FLASH_ADDR_INTERNAL_EDID			0x08078000
#define FLASH_ADDR_PROEDUCT_ID			    0x0800BF00

/* 各个扇区首地址 */
#define ADDR_FLASH_SECTOR_0     ((uint32_t)0x08000000) /* Base @ of Sector 0, 16 Kbyte */
#define ADDR_FLASH_SECTOR_1     ((uint32_t)0x08004000) /* Base @ of Sector 1, 16 Kbyte */
#define ADDR_FLASH_SECTOR_2     ((uint32_t)0x08008000) /* Base @ of Sector 2, 16 Kbyte */
#define ADDR_FLASH_SECTOR_3     ((uint32_t)0x0800C000) /* Base @ of Sector 3, 16 Kbyte */
#define ADDR_FLASH_SECTOR_4     ((uint32_t)0x08010000) /* Base @ of Sector 4, 64 Kbyte */
#define ADDR_FLASH_SECTOR_5     ((uint32_t)0x08020000) /* Base @ of Sector 5, 128 Kbyte */
#define ADDR_FLASH_SECTOR_6     ((uint32_t)0x08040000) /* Base @ of Sector 6, 128 Kbyte */
#define ADDR_FLASH_SECTOR_7     ((uint32_t)0x08060000) /* Base @ of Sector 7, 128 Kbyte */
#define ADDR_FLASH_SECTOR_8     ((uint32_t)0x08080000) /* Base @ of Sector 8, 128 Kbyte */
#define ADDR_FLASH_SECTOR_9     ((uint32_t)0x080A0000) /* Base @ of Sector 9, 128 Kbyte */
#define ADDR_FLASH_SECTOR_10    ((uint32_t)0x080C0000) /* Base @ of Sector 10, 128 Kbyte */
#define ADDR_FLASH_SECTOR_11    ((uint32_t)0x080E0000) /* Base @ of Sector 11, 128 Kbyte */

#elif (defined(GD32F4XX))
#define ADDR_FMC_SECTOR_0       ((uint32_t)0x08000000) /*!< base address of sector 0,  16 kbytes */
#define ADDR_FMC_SECTOR_1       ((uint32_t)0x08004000) /*!< base address of sector 1,  16 kbytes */
#define ADDR_FMC_SECTOR_2       ((uint32_t)0x08008000) /*!< base address of sector 2,  16 kbytes */
#define ADDR_FMC_SECTOR_3       ((uint32_t)0x0800C000) /*!< base address of sector 3,  16 kbytes */
#define ADDR_FMC_SECTOR_4       ((uint32_t)0x08010000) /*!< base address of sector 4,  64 kbytes */
#define ADDR_FMC_SECTOR_5       ((uint32_t)0x08020000) /*!< base address of sector 5, 128 kbytes */
#define ADDR_FMC_SECTOR_6       ((uint32_t)0x08040000) /*!< base address of sector 6, 128 kbytes */
#define ADDR_FMC_SECTOR_7       ((uint32_t)0x08060000) /*!< base address of sector 7, 128 kbytes */
#define ADDR_FMC_SECTOR_8       ((uint32_t)0x08080000) /*!< base address of sector 8, 128 kbytes */
#define ADDR_FMC_SECTOR_9       ((uint32_t)0x080A0000) /*!< base address of sector 9, 128 kbytes */
#define ADDR_FMC_SECTOR_10      ((uint32_t)0x080C0000) /*!< base address of sector 10,128 kbytes */
#define ADDR_FMC_SECTOR_11      ((uint32_t)0x080E0000) /*!< base address of sector 11,128 kbytes */
#define ADDR_FMC_SECTOR_12      ((uint32_t)0x08100000) /*!< base address of sector 12, 16 kbytes */
#define ADDR_FMC_SECTOR_13      ((uint32_t)0x08104000) /*!< base address of sector 13, 16 kbytes */
#define ADDR_FMC_SECTOR_14      ((uint32_t)0x08108000) /*!< base address of sector 14, 16 kbytes */
#define ADDR_FMC_SECTOR_15      ((uint32_t)0x0810C000) /*!< base address of sector 15, 16 kbytes */
#define ADDR_FMC_SECTOR_16      ((uint32_t)0x08110000) /*!< base address of sector 16, 64 kbytes */
#define ADDR_FMC_SECTOR_17      ((uint32_t)0x08120000) /*!< base address of sector 17,128 kbytes */
#define ADDR_FMC_SECTOR_18      ((uint32_t)0x08140000) /*!< base address of sector 18,128 kbytes */
#define ADDR_FMC_SECTOR_19      ((uint32_t)0x08160000) /*!< base address of sector 19,128 kbytes */
#define ADDR_FMC_SECTOR_20      ((uint32_t)0x08180000) /*!< base address of sector 20,128 kbytes */
#define ADDR_FMC_SECTOR_21      ((uint32_t)0x081A0000) /*!< base address of sector 21,128 kbytes */
#define ADDR_FMC_SECTOR_22      ((uint32_t)0x081C0000) /*!< base address of sector 22,128 kbytes */
#define ADDR_FMC_SECTOR_23      ((uint32_t)0x081E0000) /*!< base address of sector 23,128 kbytes */
#define ADDR_FMC_SECTOR_24      ((uint32_t)0x08200000) /*!< base address of sector 24,256 kbytes */
#define ADDR_FMC_SECTOR_25      ((uint32_t)0x08240000) /*!< base address of sector 25,256 kbytes */
#define ADDR_FMC_SECTOR_26      ((uint32_t)0x08280000) /*!< base address of sector 26,256 kbytes */
#define ADDR_FMC_SECTOR_27      ((uint32_t)0x082C0000) /*!< base address of sector 27,256 kbytes */

#else
#define FLASH_ADDR_INTERNAL_EDID			0x08038000
#define FLASH_ADDR_PROEDUCT_ID			    0x08004F00

//用户根据自己的需要设置
#define STM32_FLASH_SIZE 256//512 	 		//所选STM32的FLASH容量大小(单位为K)
#define STM32_FLASH_WREN 1              //使能FLASH写入(0，不是能;1，使能)
#endif
//////////////////////////////////////////////////////////////////////////////////////////////////////

//FLASH起始地址
#define STM32_FLASH_BASE 0x08000000 	//STM32 FLASH的起始地址
//FLASH解锁键值

#ifdef __DRV_FLASH_C__
#define INTERFACE
#else
#define INTERFACE extern
#endif

#ifdef STM32F2XX_HD
INTERFACE void mdrv_FlashWrite(u32 WriteAddr,u32 *pBuffer,u32 NumToWrite);		//从指定地址开始写入指定长度的数据
INTERFACE void mdrv_FlashRead(u32 ReadAddr,u32 *pBuffer,u32 NumToRead);   		//从指定地址开始读出指定长度的数据
INTERFACE u32 STM32F4FLASH_GetFlashSector(u32 addr);
#elif (defined(N32G45X))
INTERFACE void mdrv_FlashWrite(u32 WriteAddr,u32 *pBuffer,u16 NumToWrite);		//从指定地址开始写入指定长度的数据
INTERFACE void mdrv_FlashRead(u32 ReadAddr,u32 *pBuffer,u16 NumToRead);   		//从指定地址开始读出指定长度的数据
#elif (defined(GD32F4XX))
INTERFACE u32 GD32F4FLASH_GetFlashSector(u32 addr);
INTERFACE void mdrv_FlashWrite(u32 WriteAddr,u32 *pBuffer,u32 NumToWrite);		//从指定地址开始写入指定长度的数据
INTERFACE void mdrv_FlashRead(u32 ReadAddr,u32 *pBuffer,u32 NumToRead);   		//从指定地址开始读出指定长度的数据
#else
//INTERFACE void STMFLASH_Write_NoCheck(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite);
//INTERFACE u16 STMFLASH_ReadHalfWord(u32 faddr);		  //读出半字
//INTERFACE void STMFLASH_WriteLenByte(u32 WriteAddr,u32 DataToWrite,u16 Len);	//指定地址开始写入指定长度的数据
//INTERFACE u32 STMFLASH_ReadLenByte(u32 ReadAddr,u16 Len);						//指定地址开始读取指定长度数据
INTERFACE void mdrv_FlashWrite(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite);		//从指定地址开始写入指定长度的数据
INTERFACE void mdrv_FlashRead(u32 ReadAddr,u16 *pBuffer,u16 NumToRead);   		//从指定地址开始读出指定长度的数据
#endif
INTERFACE uint8_t Flash_Erase(uint32_t u32Address);
INTERFACE uint8_t Flash_WriteWord(uint32_t u32Address, uint32_t u32Data);
INTERFACE void Flash_Unlocked(void);
INTERFACE void Flash_Locked(void);

#undef INTERFACE
#endif //#ifndef __DRV_FLASH_H__

