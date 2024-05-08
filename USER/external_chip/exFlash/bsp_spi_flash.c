#include "bsp_spi_flash.h"
#include <stdio.h>
#include <string.h>

#if _ENABLE_SPI_FLASH
extern SemaphoreHandle_t xSemaphore_SPI[NUM_OF_SPI_BUS_MAX];

#define  USE_FLASH_DRIVER         1

u8 g_u8SpiFlashDev      = _SPI_TABLE_INDEX_FLASH;

static void sf_WriteEnable(void);

stFlashType g_stFlashType;

#if USE_FLASH_DRIVER
/*W25Q64 --8M   128Block 每块16个扇区  每个扇区4K*/

void sf_CS_ENABLE(void)
{
    pstSpiDeclare_t pSpi = (pstSpiDeclare_t)&stProjectSpiTable[g_u8SpiFlashDev];
    GPIO_ResetBits(STM32_GPIO_PORT[pSpi->pstSpiCs->u8PortType-PORT_GPIOA], pSpi->pstSpiCs->u16GpioPin);
}

void sf_CS_DISABLE(void)
{
    pstSpiDeclare_t pSpi = (pstSpiDeclare_t)&stProjectSpiTable[g_u8SpiFlashDev];
    GPIO_SetBits(STM32_GPIO_PORT[pSpi->pstSpiCs->u8PortType-PORT_GPIOA], pSpi->pstSpiCs->u16GpioPin);
}

void sf_SCK_LOW(void)
{
    pstSpiDeclare_t pSpi = (pstSpiDeclare_t)&stProjectSpiTable[g_u8SpiFlashDev];
    GPIO_ResetBits(STM32_GPIO_PORT[pSpi->pstSpiClk->u8PortType-PORT_GPIOA], pSpi->pstSpiClk->u16GpioPin);
}

void sf_SCK_HIGH(void)
{
    pstSpiDeclare_t pSpi = (pstSpiDeclare_t)&stProjectSpiTable[g_u8SpiFlashDev];
    GPIO_SetBits(STM32_GPIO_PORT[pSpi->pstSpiClk->u8PortType-PORT_GPIOA], pSpi->pstSpiClk->u16GpioPin);
}

uint16_t sf_MISO_READ(void)
{
    pstSpiDeclare_t pSpi = (pstSpiDeclare_t)&stProjectSpiTable[g_u8SpiFlashDev];
    return GPIO_ReadInputDataBit(STM32_GPIO_PORT[pSpi->pstSpiMiso->u8PortType-PORT_GPIOA], pSpi->pstSpiMiso->u16GpioPin);

}

void sf_MOSI_LOW(void)
{
    pstSpiDeclare_t pSpi = (pstSpiDeclare_t)&stProjectSpiTable[g_u8SpiFlashDev];
    GPIO_ResetBits(STM32_GPIO_PORT[pSpi->pstSpiMosi->u8PortType-PORT_GPIOA], pSpi->pstSpiMosi->u16GpioPin);
}

void sf_MOSI_HIGH(void)
{
    pstSpiDeclare_t pSpi = (pstSpiDeclare_t)&stProjectSpiTable[g_u8SpiFlashDev];
    GPIO_SetBits(STM32_GPIO_PORT[pSpi->pstSpiMosi->u8PortType-PORT_GPIOA], pSpi->pstSpiMosi->u16GpioPin);
}
#endif
void sf_FlashSelect(u8 u8DevID)
{
    g_u8SpiFlashDev = u8DevID;
}

void sf_FlashInit(void)
{
#if 0//USE_FLASH_DRIVER
    sf_CS_DISABLE();
    sf_SCK_HIGH();
    sf_MOSI_HIGH();

#endif
    sf_WriteEnable();

    sf_ReadInfo();
    dbg_printf("FLASH_NAME: %s\r\n",g_stFlashType.m_s8ChipName);
    dbg_printf("FLASH_ID==0x%08x\r\n",g_stFlashType.m_u32ChipId);
}

#if USE_FLASH_DRIVER
void sf_Delay(void)
{
    uint16_t i;
    for (i = 0; i < 2; i++);
}

void sf_WriteByte(uint8_t TxData)//时钟上升沿写数据 下降沿读数据
{
    uint8_t cnt;
    xSemaphoreTake(xSemaphore_SPI[g_u8SpiFlashDev], portMAX_DELAY);

    for(cnt=0;cnt<8;cnt++)
    {
        sf_SCK_LOW();
        sf_Delay();
        if(TxData&0x80)
        {
            sf_MOSI_HIGH();
        }
        else
        {
            sf_MOSI_LOW();
        }
        TxData<<=1;
        sf_Delay();
        sf_SCK_HIGH();
        sf_Delay();
    }
    xSemaphoreGive(xSemaphore_SPI[g_u8SpiFlashDev]);
}

uint8_t sf_ReadByte(void)
{
    uint8_t cnt;
    uint8_t RxData=0;
    xSemaphoreTake(xSemaphore_SPI[g_u8SpiFlashDev], portMAX_DELAY);
    for(cnt=0;cnt<8;cnt++)
    {
        sf_SCK_LOW();
        sf_Delay();

        RxData<<=1;
        if(sf_MISO_READ())
        {
            RxData|=0x01;
        }


        sf_SCK_HIGH();
        sf_Delay();
    }
    xSemaphoreGive(xSemaphore_SPI[g_u8SpiFlashDev]);
    return RxData;
}
#endif

/**********************************************************************************************************
*   函 数 名: sf_WriteEnable
*   功能说明: 向器件发送写使能命令
*   形    参:  无
*   返 回 值: 无
**********************************************************************************************************/
static void sf_WriteEnable(void)
{
#if USE_FLASH_DRIVER
    sf_CS_ENABLE();                                    /* 使能片选 */
    sf_WriteByte(CMD_WREN);                /* 发送命令 */
    sf_CS_DISABLE();                                 /* 禁能片选 */
#else
    drv_spi_bus_write(g_u8SpiFlashDev,CMD_WREN,0,NULL,eSpiU8Addr);
#endif

}
/**********************************************************************************************************
*   函 数 名: sf_WaitForWriteEnd
*   功能说明: 采用循环查询的方式等待器件内部写操作完成
*   形    参:  无
*   返 回 值: 无
**********************************************************************************************************/
static void sf_WaitForWriteEnd(void)
{
#if USE_FLASH_DRIVER
    sf_CS_ENABLE();                                        /* 使能片选 */
    sf_WriteByte(CMD_RDSR);                            /* 发送命令， 读状态寄存器 */
    while((sf_ReadByte() & WIP_FLAG) == SET);  /* 判断状态寄存器的忙标志位 */
    sf_CS_DISABLE();                                       /* 禁能片选 */
#else
    uint8_t l_u8Flag;
    do{
        drv_spi_bus_read(g_u8SpiFlashDev,CMD_RDSR,1,&l_u8Flag,eSpiU8Addr);
    }
    while((l_u8Flag&WIP_FLAG)==WIP_FLAG);
#endif
}
/**********************************************************************************************************
*   函 数 名: sf_EraseSector
*   功能说明: 擦除指定的扇区  至少150ms
*   形    参:  _uiSectorAddr : 扇区地址    0~2047
*   返 回 值: 无
**********************************************************************************************************/
void sf_EraseSector(uint32_t _uiSectorAddr)
{
#if (!USE_FLASH_DRIVER)
    uint32_t l_u32Addr = (CMD_SE<<24)|(_uiSectorAddr&0x00FFFFFF);
#endif
    sf_WriteEnable();                               /* 发送写使能命令 */

    //printf("_uiSectorAddr==0x%08x\r\n",_uiSectorAddr);
    /* 擦除扇区操作 */
#if USE_FLASH_DRIVER
    sf_CS_ENABLE();                                        /* 使能片选 */
    sf_WriteByte(CMD_SE);                              /* 发送擦除命令 */
    sf_WriteByte((_uiSectorAddr & 0xFF0000) >> 16);    /* 发送扇区地址的高8bit */
    sf_WriteByte((_uiSectorAddr & 0xFF00) >> 8);       /* 发送扇区地址中间8bit */
    sf_WriteByte(_uiSectorAddr & 0xFF);                /* 发送扇区地址低8bit */
    sf_CS_DISABLE();                                       /* 禁能片选 */
#else
    drv_spi_bus_write(g_u8SpiFlashDev,l_u32Addr,0,NULL,eSpiU32Addr);
#endif
    sf_WaitForWriteEnd();                           /* 等待串行Flash内部写操作完成 */
}

/**********************************************************************************************************
*   函 数 名: sf_EraseChip
*   功能说明: 擦除整个芯片
*   形    参:  无
*   返 回 值: 无
*********************************************************************************************************/
void sf_EraseChip(void)
{
    sf_WriteEnable();                               /* 发送写使能命令 */

    /* 擦除扇区操作 */
#if USE_FLASH_DRIVER
    sf_CS_ENABLE();                                    /* 使能片选 */
    sf_WriteByte(CMD_BE);                  /* 发送整片擦除命令 */
    sf_CS_DISABLE();                                   /* 禁能片选 */
#else
    drv_spi_bus_write(g_u8SpiFlashDev,CMD_BE,0,NULL,eSpiU8Addr);
#endif

    sf_WaitForWriteEnd();                           /* 等待串行Flash内部写操作完成 */
}

void sf_WriteProtection(void)
{
#if USE_FLASH_DRIVER
    sf_CS_ENABLE();
    sf_WriteByte(CMD_DISWR);
    sf_CS_DISABLE();
#else
    drv_spi_bus_write(g_u8SpiFlashDev,CMD_DISWR,0,NULL,eSpiU8Addr);
#endif
}
/**********************************************************************************************************
*   函 数 名: sf_PageWrite
*   功能说明: 向一个page内写入若干字节。字节个数不能超出页面大小（4K)
*   形    参:   _pBuf : 数据源缓冲区；
*               _uiWriteAddr ：目标区域首地址
*               _usSize ：数据个数，不能超过页面大小
*   返 回 值: 无
**********************************************************************************************************/
void sf_PageWrite(uint8_t * _pBuf, uint32_t _uiWriteAddr, uint16_t _usSize)
{

#if USE_FLASH_DRIVER
    uint32_t i, j;
    /* for MX25L1606E 25Q64BV */
    {
        for (j = 0; j < _usSize / 256; j++)
        {
            sf_WriteEnable();                                       /* 发送写使能命令 */

            sf_CS_ENABLE();                                            /* 使能片选 */
            sf_WriteByte(0x02);                                /* 发送AAI命令(地址自动增加编程) */
            sf_WriteByte((_uiWriteAddr & 0xFF0000) >> 16); /* 发送扇区地址的高8bit */
            sf_WriteByte((_uiWriteAddr & 0xFF00) >> 8);        /* 发送扇区地址中间8bit */
            sf_WriteByte(_uiWriteAddr & 0xFF);             /* 发送扇区地址低8bit */

            for (i = 0; i < 256; i++)
            {
                sf_WriteByte(*_pBuf++);                    /* 发送数据 */
            }

            sf_CS_DISABLE();                               /* 禁止片选 */

            sf_WaitForWriteEnd();                       /* 等待串行Flash内部写操作完成 */

            _uiWriteAddr += 256;
        }

        /* 进入写保护状态 */
        sf_CS_ENABLE();
        sf_WriteByte(CMD_DISWR);
        sf_CS_DISABLE();

        sf_WaitForWriteEnd();                           /* 等待串行Flash内部写操作完成 */
    }
#else
    uint32_t i;
    uint32_t l_u32Addr = (CMD_WRPG<<24)|(_uiWriteAddr&0x00FFFFFF);
    if (g_stFlashType.m_u32ChipId == SST25VF016B_ID)
    {
        l_u32Addr = (uint32_t)((CMD_AAI&0xFF)<<24)|(_uiWriteAddr&0x00FFFFFF);
        /* AAI指令要求传输的数据个数是偶数 */
        if ((_usSize < 2) && (_usSize % 2))
        {
            return ;
        }
        sf_WriteEnable();                                       /* 发送写使能命令 */
        drv_spi_bus_write(g_u8SpiFlashDev,l_u32Addr,2,_pBuf,eSpiU32Addr);
        sf_WaitForWriteEnd();                           /* 等待串行Flash内部写操作完成 */
        _usSize -= 2;                                   /* 计算剩余字节 */
        for (i = 0; i < _usSize / 2; i++)
        {
            drv_spi_bus_write(g_u8SpiFlashDev,CMD_AAI,2,_pBuf,eSpiU8Addr);
            _pBuf += 2;
            sf_WaitForWriteEnd();                           /* 等待串行Flash内部写操作完成 */
        }

        sf_WriteProtection();                           /* 进入写保护状态 */
        sf_WaitForWriteEnd();                           /* 等待串行Flash内部写操作完成 */
    }
    else    /* for MX25L1606E ? W25Q64BV */
    {
        l_u32Addr = (CMD_WRPG<<24)|(_uiWriteAddr&0x00FFFFFF);
        for(i=0;i<_usSize / 256;i++)
        {
            sf_WriteEnable();                                       /* 发送写使能命令 */
            drv_spi_bus_write(g_u8SpiFlashDev,l_u32Addr,256,_pBuf,eSpiU32Addr);
            l_u32Addr += 256;
            _pBuf += 256;
        }
        sf_WriteProtection();                           /* 进入写保护状态 */
        sf_WaitForWriteEnd();                           /* 等待串行Flash内部写操作完成 */
    }
#endif


}
/************************************************
函数名称 ： SFLASH_WriteNoCheck
功    能 ： 无检验写SFLASH
            必须确保所写的地址范围内的数据全部为0xFF,否则在非0xFF处写入的数据将失败!
            具有自动换页功能
            在指定地址开始写入指定长度的数据,但是要确保地址不越界!
参    数 ： pBuffer ----- 写入数据区首地址
            WriteAddr --- 要写入Flash的地址
            nByte ------- 要写入的字节数
*************************************************/
void SFLASH_WriteNoCheck(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t nByte)
{
    uint16_t PageRemain = 256 - WriteAddr%256;     //单页剩余可写的字节数

    if(nByte <= PageRemain)
        PageRemain = nByte;                          //不大于256个字节

    while(1)
    {
        sf_PageWrite(pBuffer, WriteAddr, PageRemain);
        if(nByte == PageRemain)                      //写入结束
            break;
        else                                         //写入未结束
        {
            pBuffer += PageRemain;                     //下一页写入数据
            WriteAddr += PageRemain;                   //下一页写入数据地址
            nByte -= PageRemain;                       //待写入字节数递减

            if(nByte > 256)
                PageRemain = 256;                        //待写入1页(256字节)的数据
            else
                PageRemain = nByte;                      //待写入少于1页(256字节)的数据
        }
    }
}


/************************************************
函数名称 ： SFLASH_WriteNByte
功    能 ： 从ReadAddr地址开始连续写入nByte到SFLASH中
参    数 ： pBuffer ----- 写入数据区首地址
            WriteAddr --- 要写入Flash的地址
            nByte ------- 要写入的字节数(最大65535B = 64K 块)
*************************************************/
void SFLASH_WriteNByte(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t nByte)
{
    static uint8_t SectorBuf[4096];                //扇区buf
    uint32_t SecPos;                               //扇区位置
    uint16_t SecOff;                               //扇区偏移
    uint16_t SecRemain;                            //剩余扇区
    uint16_t i;

    SecPos = WriteAddr/g_stFlashType.m_u16SectorSize;                       //地址所在扇区(0~511)
    SecOff = WriteAddr%g_stFlashType.m_u16SectorSize;                       //地址所在扇区的偏移
    SecRemain = g_stFlashType.m_u16SectorSize-SecOff;                       //地址所在扇区剩余字节数(扇区大小4096B=4KB)

    if(nByte <= SecRemain)
    SecRemain = nByte;                           //写入数据大小 < 剩余空间大小 (即剩余空间够保存这些数据)

    while(1)
    {
        /* 第1步·校验 */
        sf_ReadBuffer(SectorBuf, SecPos*g_stFlashType.m_u16SectorSize, g_stFlashType.m_u16SectorSize);        //读出整个扇区的内容
        for(i=0; i<SecRemain; i++)                             //校验数据,是否需要擦除
        {
            if(SectorBuf[SecOff + i] != 0xFF)                    //存储数据不为0xFF 则需要擦除
                break;
        }
        if(i < SecRemain)                                      //需要擦除
        {
            sf_EraseSector(SecPos*g_stFlashType.m_u16SectorSize);                          //擦除该扇区
            for(i=0; i<SecRemain; i++)                           //保存写入的数据(第1次时，是写入那扇区后面剩余的空间)
            {
                SectorBuf[SecOff + i] = pBuffer[i];
            }
            SFLASH_WriteNoCheck(SectorBuf, SecPos*g_stFlashType.m_u16SectorSize, g_stFlashType.m_u16SectorSize);   //写入整个扇区（扇区 = 老数据 + 新写入数据）
        }
        else
            SFLASH_WriteNoCheck(pBuffer, WriteAddr, SecRemain);  //不需要擦除,直接写入扇区剩余空间

        if(nByte == SecRemain)                       //写入结束
        {
            sf_WriteProtection();                           /* 进入写保护状态 */

            break;
        }
        else                                         //写入未结束
        {
            SecPos++;                                  //扇区地址增1
            SecOff = 0;                                //偏移位置归零
            pBuffer += SecRemain;                      //指针偏移
            WriteAddr += SecRemain;                    //写地址偏移
            nByte -= SecRemain;                        //待写入字节数递减
            if(nByte > g_stFlashType.m_u16SectorSize)
                SecRemain = g_stFlashType.m_u16SectorSize;                        //待写入1扇区(4096字节)的数据
            else
                SecRemain = nByte;                       //待写入少于1扇区(4096字节)的数据
        }
    }
}
/**********************************************************************************************************
*   函 数 名: sf_ReadBuffer
*   功能说明: 连续读取若干字节。字节个数不能超出芯片容量。
*   形    参:   _pBuf : 数据源缓冲区；
*               _uiReadAddr ：首地址
*               _usSize ：数据个数, 可以大于PAGE_SIZE,但是不能超出芯片总容量
*   返 回 值: 无
**********************************************************************************************************/
void sf_ReadBuffer(uint8_t * _pBuf, uint32_t _uiReadAddr, uint32_t _uiSize)
{
#if (!USE_FLASH_DRIVER)
    uint32_t l_u32Addr = (CMD_READ<<24)|(_uiReadAddr&0x00FFFFFF);
#endif
    /* 如果读取的数据长度为0或者超出串行Flash地址空间，则直接返回 */
    if ((_uiSize == 0) ||(_uiReadAddr + _uiSize) > g_stFlashType.m_u32ChipSize)
    {
        return;
    }

#if USE_FLASH_DRIVER
    /* 擦除扇区操作 */
    sf_CS_ENABLE();                                        /* 使能片选 */
    sf_WriteByte(CMD_READ);                            /* 发送读命令 */
    sf_WriteByte((_uiReadAddr & 0xFF0000) >> 16);  /* 发送扇区地址的高8bit */
    sf_WriteByte((_uiReadAddr & 0xFF00) >> 8);     /* 发送扇区地址中间8bit */
    sf_WriteByte(_uiReadAddr & 0xFF);              /* 发送扇区地址低8bit */
    while (_uiSize--)
    {
        *_pBuf++ = sf_ReadByte();          /* 读一个字节并存储到pBuf，读完后指针自加1 */
    }
    sf_CS_DISABLE();                                               /* 禁能片选 */
#else
    drv_spi_bus_read(g_u8SpiFlashDev,l_u32Addr,_uiSize,_pBuf,eSpiU32Addr);
#endif

}

/*
*********************************************************************************************************
*   函 数 名: sf_ReadID
*   功能说明: 读取器件ID
*   形    参:  无
*   返 回 值: 32bit的器件ID (最高8bit填0，有效ID位数为24bit）
*********************************************************************************************************
*/
uint32_t sf_ReadID(void)
{
#if USE_FLASH_DRIVER
    uint32_t    uiID    ;
    uint8_t id1, id2, id3;

    sf_CS_ENABLE();                                                    /* 使能片选 */
    sf_WriteByte(CMD_RDID);                                /* 发送读ID命令 */

    id1=sf_ReadByte();
    id2=sf_ReadByte();
    id3=sf_ReadByte();

    sf_CS_DISABLE();                                                   /* 禁能片选 */

    uiID = ((uint32_t)id1 << 16) | ((uint32_t)id2 << 8) | id3;
#else
    uint32_t l_u32Addr = CMD_RDID;
    uint8_t l_u8Uid[3];
    uint32_t    uiID    ;
    drv_spi_bus_read(g_u8SpiFlashDev,l_u32Addr,3,l_u8Uid,eSpiU32Addr);

    uiID = ((uint32_t)l_u8Uid[0] << 16) | ((uint32_t)l_u8Uid[1] << 8) | l_u8Uid[2];
#endif

    return uiID;
}

/*
*********************************************************************************************************
*   函 数 名: sf_ReadInfo
*   功能说明: 读取器件ID,并填充器件参数
*   形    参:  无
*   返 回 值: 无
*********************************************************************************************************
*/
void sf_ReadInfo(void)
{
    /* 自动识别串行Flash型号 */
    {
        g_stFlashType.m_u32ChipId = sf_ReadID();    /* 芯片ID */

        switch (g_stFlashType.m_u32ChipId)
        {
            case SST25VF016B_ID:
                strcpy(g_stFlashType.m_s8ChipName, "SST25VF016B");
                g_stFlashType.m_u32ChipSize = 2 * 1024 * 1024;  /* 总容量 = 2M */
                g_stFlashType.m_u16SectorSize = 4 * 1024;           /* 页面大小 = 4K */
                break;

            case MX25L1606E_ID:
                strcpy(g_stFlashType.m_s8ChipName, "MX25L1606E");
                g_stFlashType.m_u32ChipSize  = 2 * 1024 * 1024; /* 总容量 = 2M */
                g_stFlashType.m_u16SectorSize = 4 * 1024;           /* 页面大小 = 4K */
                break;

            case W25Q64BV_ID:
                strcpy(g_stFlashType.m_s8ChipName, "W25Q64BV");
                g_stFlashType.m_u32ChipSize = 8 * 1024 * 1024;  /* 总容量 = 8M */
                g_stFlashType.m_u16SectorSize = 4 * 1024;           /* 页面大小 = 4K */
                break;

            case W25Q32BV_ID:
                strcpy(g_stFlashType.m_s8ChipName, "W25Q32BV");
                g_stFlashType.m_u32ChipSize = 4 * 1024 * 1024;  /* 总容量 = 4M */
                g_stFlashType.m_u16SectorSize = 4 * 1024;           /* 页面大小 = 4K */
                break;
            case W25Q16BV_ID:
                strcpy(g_stFlashType.m_s8ChipName, "W25Q16BV");
                g_stFlashType.m_u32ChipSize = 2 * 1024 * 1024;  /* 总容量 = 2M */
                g_stFlashType.m_u16SectorSize = 4 * 1024;           /* 页面大小 = 4K */
                break;

            case W25Q128_ID:
                strcpy(g_stFlashType.m_s8ChipName, "W25Q128");
                g_stFlashType.m_u32ChipSize = 16 * 1024 * 1024;  /* 总容量 = 16M */
                g_stFlashType.m_u16SectorSize = 4 * 1024;           /* 页面大小 = 4K */
                break;

            default:
                strcpy(g_stFlashType.m_s8ChipName, "Unknow Flash");
                g_stFlashType.m_u32ChipSize = 4 * 1024 * 1024;
                g_stFlashType.m_u16SectorSize = 4 * 1024;
                break;
        }
    }
}

/*
*********************************************************************************************************
*   函 数 名: sf_NeedErase
*   功能说明: 判断写PAGE前是否需要先擦除。
*   形    参:   _ucpOldBuf ： 旧数据
*              _ucpNewBuf ： 新数据
*              _uiLen ：数据个数，不能超过页面大小
*   返 回 值: 0 : 不需要擦除， 1 ：需要擦除
*********************************************************************************************************
*/
static uint8_t sf_NeedErase(uint8_t * _ucpOldBuf, uint8_t *_ucpNewBuf, uint16_t _usLen)
{
    uint16_t i;
    uint8_t ucOld;

    /*
    算法第1步：old 求反, new 不变
          old    new
          1101   0101
    ~     1111
        = 0010   0101

    算法第2步: old 求反的结果与 new 位与
          0010   old
    &     0101   new
         =0000

    算法第3步: 结果为0,则表示无需擦除. 否则表示需要擦除
    */

    for (i = 0; i < _usLen; i++)
    {
        ucOld = *_ucpOldBuf++;
        ucOld = ~ucOld;

        /* 注意错误的写法: if (ucOld & (*_ucpNewBuf++) != 0) */
        if ((ucOld & (*_ucpNewBuf++)) != 0)
        {
                return 1;
        }
    }
    return 0;
}

/*
*********************************************************************************************************
*   函 数 名: sf_CmpData
*   功能说明: 比较Flash的数据.
*   形    参:   _ucpTar : 数据缓冲区
*               _uiSrcAddr ：Flash地址
*               _uiSize ：数据个数, 可以大于PAGE_SIZE,但是不能超出芯片总容量
*   返 回 值: 0 = 相等, 1 = 不等
*********************************************************************************************************
*/
static uint8_t sf_CmpData(uint32_t _uiSrcAddr, uint8_t *_ucpTar, uint32_t _uiSize)
{
#if (!USE_FLASH_DRIVER)
    uint32_t l_u32Addr = (CMD_READ<<24)|(_uiSrcAddr&0x00FFFFFF);
    uint8_t l_u8Value;
    uint32_t i;
#else
    uint8_t ucValue;
#endif

    /* 如果读取的数据长度为0或者超出串行Flash地址空间，则直接返回 */
    if ((_uiSrcAddr + _uiSize) > g_stFlashType.m_u32ChipSize)
    {
        return 1;
    }

#if USE_FLASH_DRIVER
    if (_uiSize == 0)
    {
        return 0;
    }

    sf_CS_ENABLE();                                    /* 使能片选 */
    sf_WriteByte(CMD_READ);                            /* 发送读命令 */
    sf_WriteByte((_uiSrcAddr & 0xFF0000) >> 16);       /* 发送扇区地址的高8bit */
    sf_WriteByte((_uiSrcAddr & 0xFF00) >> 8);      /* 发送扇区地址中间8bit */
    sf_WriteByte(_uiSrcAddr & 0xFF);                   /* 发送扇区地址低8bit */
    while (_uiSize--)
    {
        /* 读一个字节 */
        ucValue = sf_ReadByte();
        if (*_ucpTar++ != ucValue)
        {
            sf_CS_DISABLE();
            return 1;
        }
    }
    sf_CS_DISABLE();
#else
    for(i=0;i<_uiSize;i++)
    {
        drv_spi_bus_read(g_u8SpiFlashDev,l_u32Addr+i,1,&l_u8Value,eSpiU32Addr);
        if (*_ucpTar++ != l_u8Value)
        {
            return 1;
        }
    }
#endif

    return 0;
}

/*
*********************************************************************************************************
*   函 数 名: sf_AutoWritePage
*   功能说明: 写1个PAGE并校验,如果不正确则再重写两次。本函数自动完成擦除操作。
*   形    参:   _pBuf : 数据源缓冲区；
*               _uiWriteAddr ：目标区域首地址
*               _usSize ：数据个数，不能超过页面大小
*   返 回 值: 0 : 错误， 1 ： 成功
*********************************************************************************************************
*/
static uint8_t sf_AutoWritePage(uint8_t *_ucpSrc, uint32_t _uiWrAddr, uint16_t _usWrLen)
{
    uint16_t i;
    uint16_t j;                 /* 用于延时 */
    uint32_t uiFirstAddr;       /* 扇区首址 */
    uint8_t ucNeedErase;        /* 1表示需要擦除 */
    uint8_t cRet;
    static uint8_t sl_u8spiBuf[4096];

    /* 长度为0时不继续操作,直接认为成功 */
    if (_usWrLen == 0)
    {
        return 1;
    }

    /* 如果偏移地址超过芯片容量则退出 */
    if (_uiWrAddr >= g_stFlashType.m_u32ChipSize)
    {
        return 0;
    }

    /* 如果数据长度大于扇区容量，则退出 */
    if (_usWrLen > g_stFlashType.m_u16SectorSize)
    {
        return 0;
    }

    /* 如果FLASH中的数据没有变化,则不写FLASH */
    sf_ReadBuffer(sl_u8spiBuf, _uiWrAddr, _usWrLen);
    if (memcmp(sl_u8spiBuf, _ucpSrc, _usWrLen) == 0)
    {
        return 1;
    }

    /* 判断是否需要先擦除扇区 */
    /* 如果旧数据修改为新数据，所有位均是 1->0 或者 0->0, 则无需擦除,提高Flash寿命 */
    ucNeedErase = 0;
    if (sf_NeedErase(sl_u8spiBuf, _ucpSrc, _usWrLen))
    {
        ucNeedErase = 1;
    }

    uiFirstAddr = _uiWrAddr & (~(g_stFlashType.m_u16SectorSize - 1));

    if (_usWrLen == g_stFlashType.m_u16SectorSize)      /* 整个扇区都改写 */
    {
        for (i = 0; i < g_stFlashType.m_u16SectorSize; i++)
        {
            sl_u8spiBuf[i] = _ucpSrc[i];
        }
    }
    else                        /* 改写部分数据 */
    {
        /* 先将整个扇区的数据读出 */
        sf_ReadBuffer(sl_u8spiBuf, uiFirstAddr, g_stFlashType.m_u16SectorSize);

        /* 再用新数据覆盖 */
        i = _uiWrAddr & (g_stFlashType.m_u16SectorSize - 1);
        memcpy(&sl_u8spiBuf[i], _ucpSrc, _usWrLen);
    }

    /* 写完之后进行校验，如果不正确则重写，最多3次 */
    cRet = 0;
    for (i = 0; i < 3; i++)
    {

        /* 如果旧数据修改为新数据，所有位均是 1->0 或者 0->0, 则无需擦除,提高Flash寿命 */
        if (ucNeedErase == 1)
        {
            sf_EraseSector(uiFirstAddr);        /* 擦除1个扇区 */
        }

        /* 编程一个PAGE */
        sf_PageWrite(sl_u8spiBuf, uiFirstAddr, g_stFlashType.m_u16SectorSize);

        if (sf_CmpData(_uiWrAddr, _ucpSrc, _usWrLen) == 0)
        {
            cRet = 1;
            break;
        }
        else
        {
            if (sf_CmpData(_uiWrAddr, _ucpSrc, _usWrLen) == 0)
            {
                cRet = 1;
                break;
            }

            /* 失败后延迟一段时间再重试 */
            for (j = 0; j < 10000; j++);
        }
    }

    return cRet;
}

/*
*********************************************************************************************************
*   函 数 名: sf_WriteBuffer
*   功能说明: 写1个扇区并校验,如果不正确则再重写两次。本函数自动完成擦除操作。
*   形    参:   _pBuf : 数据源缓冲区；
*               _uiWrAddr ：目标区域首地址
*               _usSize ：数据个数，不能超过页面大小
*   返 回 值: 1 : 成功， 0 ： 失败
*********************************************************************************************************
*/
uint8_t sf_WriteBuffer(uint8_t* _pBuf, uint32_t _uiWriteAddr, uint16_t _usWriteSize)
{
    uint16_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;

    Addr = _uiWriteAddr % g_stFlashType.m_u16SectorSize;
    count = g_stFlashType.m_u16SectorSize - Addr;
    NumOfPage =  _usWriteSize / g_stFlashType.m_u16SectorSize;
    NumOfSingle = _usWriteSize % g_stFlashType.m_u16SectorSize;

    if (Addr == 0) /* 起始地址是页面首地址  */
    {
        if (NumOfPage == 0) /* 数据长度小于页面大小 */
        {
            if (sf_AutoWritePage(_pBuf, _uiWriteAddr, _usWriteSize) == 0)
            {
                return 0;
            }
        }
        else    /* 数据长度大于等于页面大小 */
        {
            while (NumOfPage--)
            {
                if (sf_AutoWritePage(_pBuf, _uiWriteAddr, g_stFlashType.m_u16SectorSize) == 0)
                {
                    return 0;
                }
                _uiWriteAddr +=  g_stFlashType.m_u16SectorSize;
                _pBuf += g_stFlashType.m_u16SectorSize;
            }
            if (sf_AutoWritePage(_pBuf, _uiWriteAddr, NumOfSingle) == 0)
            {
                return 0;
            }
        }
    }
    else  /* 起始地址不是页面首地址  */
    {
        if (NumOfPage == 0) /* 数据长度小于页面大小 */
        {
            if (NumOfSingle > count) /* (_usWriteSize + _uiWriteAddr) > SPI_FLASH_PAGESIZE */
            {
                temp = NumOfSingle - count;

                if (sf_AutoWritePage(_pBuf, _uiWriteAddr, count) == 0)
                {
                    return 0;
                }

                _uiWriteAddr +=  count;
                _pBuf += count;

                if (sf_AutoWritePage(_pBuf, _uiWriteAddr, temp) == 0)
                {
                    return 0;
                }
            }
            else
            {
                if (sf_AutoWritePage(_pBuf, _uiWriteAddr, _usWriteSize) == 0)
                {
                    return 0;
                }
            }
        }
        else    /* 数据长度大于等于页面大小 */
        {
            _usWriteSize -= count;
            NumOfPage =  _usWriteSize / g_stFlashType.m_u16SectorSize;
            NumOfSingle = _usWriteSize % g_stFlashType.m_u16SectorSize;

            if (sf_AutoWritePage(_pBuf, _uiWriteAddr, count) == 0)
            {
                return 0;
            }

            _uiWriteAddr +=  count;
            _pBuf += count;

            while (NumOfPage--)
            {
                if (sf_AutoWritePage(_pBuf, _uiWriteAddr, g_stFlashType.m_u16SectorSize) == 0)
                {
                    return 0;
                }
                _uiWriteAddr +=  g_stFlashType.m_u16SectorSize;
                _pBuf += g_stFlashType.m_u16SectorSize;
            }

            if (NumOfSingle != 0)
            {
                if (sf_AutoWritePage(_pBuf, _uiWriteAddr, NumOfSingle) == 0)
                {
                    return 0;
                }
            }
        }
    }
    return 1;   /* 成功 */
}

#endif

