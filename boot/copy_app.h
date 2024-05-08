/*****************************************************************************
Copyright       : PTN. Co., Ltd.
File name       : copy_app.h
Description     : 升级功能相关头文件声明
Author          : 靳凯凯

Version         : V1.0.0
Date            : 2021-6-23
History         :
*****************************************************************************/
#ifndef _COPY_APP_H
#define _COPY_APP_H

#include "Datatype.h"
#include <stdint.h>
#include <stdio.h>

#ifdef STM32F2XX_HD
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
#endif

#ifdef N32G45X
#define APP_START_ADDR          0x8004000
#define DM_FW_DATA_ADDR         0x8033800
#define DM_FW_FLAG_ADDR         0x8063000
#define PAGE_SIZE               0x800
#define DM_FW_BYTES_MAX 		(190*1024)	//100KB

#define DM_FW_FLAG_INVALID      0x44332211
#define DM_FW_FLAG_VALID        0x11223344
#define DM_FW_SIZE_ADDR         (DM_FW_FLAG_ADDR+4)
#elif (defined(GD32F4XX))
#define APP_START_ADDR          0x8010000
#define DM_FW_DATA_ADDR         0x8040000
#define DM_FW_FLAG_ADDR         0x8070000
#define PAGE_SIZE               0x800
#define DM_FW_BYTES_MAX 		(192*1024)	//100KB

#define DM_FW_FLAG_INVALID      0x44332211
#define DM_FW_FLAG_VALID        0x11223344
#define DM_FW_SIZE_ADDR         (DM_FW_FLAG_ADDR+4)
#elif (defined(STM32F2XX_HD))
#define APP_START_ADDR          0x8010000
#define DM_FW_DATA_ADDR         0x8040000
#define DM_FW_FLAG_ADDR         0x8060000
#define PAGE_SIZE               0x800
#define DM_FW_BYTES_MAX 		(128*1024)	//100KB

#define DM_FW_FLAG_INVALID      0x44332211
#define DM_FW_FLAG_VALID        0x11223344
#define DM_FW_SIZE_ADDR         (DM_FW_FLAG_ADDR+4)
#else
#define APP_START_ADDR          0x8004000
#define DM_FW_DATA_ADDR         0x8033800
#define DM_FW_FLAG_ADDR         0x8063000
#define PAGE_SIZE               0x800
#define DM_FW_BYTES_MAX 		(100*1024)	//100KB

#define DM_FW_FLAG_INVALID      0x44332211
#define DM_FW_FLAG_VALID        0x11223344
#define DM_FW_SIZE_ADDR         (DM_FW_FLAG_ADDR+4)
#endif

void to_copy_app(uint32_t app_addr,uint32_t backup_addr,uint32_t size);

#endif

