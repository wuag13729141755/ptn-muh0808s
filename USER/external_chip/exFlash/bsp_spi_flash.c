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
/*W25Q64 --8M   128Block ÿ��16������  ÿ������4K*/

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

void sf_WriteByte(uint8_t TxData)//ʱ��������д���� �½��ض�����
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
*   �� �� ��: sf_WriteEnable
*   ����˵��: ����������дʹ������
*   ��    ��:  ��
*   �� �� ֵ: ��
**********************************************************************************************************/
static void sf_WriteEnable(void)
{
#if USE_FLASH_DRIVER
    sf_CS_ENABLE();                                    /* ʹ��Ƭѡ */
    sf_WriteByte(CMD_WREN);                /* �������� */
    sf_CS_DISABLE();                                 /* ����Ƭѡ */
#else
    drv_spi_bus_write(g_u8SpiFlashDev,CMD_WREN,0,NULL,eSpiU8Addr);
#endif

}
/**********************************************************************************************************
*   �� �� ��: sf_WaitForWriteEnd
*   ����˵��: ����ѭ����ѯ�ķ�ʽ�ȴ������ڲ�д�������
*   ��    ��:  ��
*   �� �� ֵ: ��
**********************************************************************************************************/
static void sf_WaitForWriteEnd(void)
{
#if USE_FLASH_DRIVER
    sf_CS_ENABLE();                                        /* ʹ��Ƭѡ */
    sf_WriteByte(CMD_RDSR);                            /* ������� ��״̬�Ĵ��� */
    while((sf_ReadByte() & WIP_FLAG) == SET);  /* �ж�״̬�Ĵ�����æ��־λ */
    sf_CS_DISABLE();                                       /* ����Ƭѡ */
#else
    uint8_t l_u8Flag;
    do{
        drv_spi_bus_read(g_u8SpiFlashDev,CMD_RDSR,1,&l_u8Flag,eSpiU8Addr);
    }
    while((l_u8Flag&WIP_FLAG)==WIP_FLAG);
#endif
}
/**********************************************************************************************************
*   �� �� ��: sf_EraseSector
*   ����˵��: ����ָ��������  ����150ms
*   ��    ��:  _uiSectorAddr : ������ַ    0~2047
*   �� �� ֵ: ��
**********************************************************************************************************/
void sf_EraseSector(uint32_t _uiSectorAddr)
{
#if (!USE_FLASH_DRIVER)
    uint32_t l_u32Addr = (CMD_SE<<24)|(_uiSectorAddr&0x00FFFFFF);
#endif
    sf_WriteEnable();                               /* ����дʹ������ */

    //printf("_uiSectorAddr==0x%08x\r\n",_uiSectorAddr);
    /* ������������ */
#if USE_FLASH_DRIVER
    sf_CS_ENABLE();                                        /* ʹ��Ƭѡ */
    sf_WriteByte(CMD_SE);                              /* ���Ͳ������� */
    sf_WriteByte((_uiSectorAddr & 0xFF0000) >> 16);    /* ����������ַ�ĸ�8bit */
    sf_WriteByte((_uiSectorAddr & 0xFF00) >> 8);       /* ����������ַ�м�8bit */
    sf_WriteByte(_uiSectorAddr & 0xFF);                /* ����������ַ��8bit */
    sf_CS_DISABLE();                                       /* ����Ƭѡ */
#else
    drv_spi_bus_write(g_u8SpiFlashDev,l_u32Addr,0,NULL,eSpiU32Addr);
#endif
    sf_WaitForWriteEnd();                           /* �ȴ�����Flash�ڲ�д������� */
}

/**********************************************************************************************************
*   �� �� ��: sf_EraseChip
*   ����˵��: ��������оƬ
*   ��    ��:  ��
*   �� �� ֵ: ��
*********************************************************************************************************/
void sf_EraseChip(void)
{
    sf_WriteEnable();                               /* ����дʹ������ */

    /* ������������ */
#if USE_FLASH_DRIVER
    sf_CS_ENABLE();                                    /* ʹ��Ƭѡ */
    sf_WriteByte(CMD_BE);                  /* ������Ƭ�������� */
    sf_CS_DISABLE();                                   /* ����Ƭѡ */
#else
    drv_spi_bus_write(g_u8SpiFlashDev,CMD_BE,0,NULL,eSpiU8Addr);
#endif

    sf_WaitForWriteEnd();                           /* �ȴ�����Flash�ڲ�д������� */
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
*   �� �� ��: sf_PageWrite
*   ����˵��: ��һ��page��д�������ֽڡ��ֽڸ������ܳ���ҳ���С��4K)
*   ��    ��:   _pBuf : ����Դ��������
*               _uiWriteAddr ��Ŀ�������׵�ַ
*               _usSize �����ݸ��������ܳ���ҳ���С
*   �� �� ֵ: ��
**********************************************************************************************************/
void sf_PageWrite(uint8_t * _pBuf, uint32_t _uiWriteAddr, uint16_t _usSize)
{

#if USE_FLASH_DRIVER
    uint32_t i, j;
    /* for MX25L1606E 25Q64BV */
    {
        for (j = 0; j < _usSize / 256; j++)
        {
            sf_WriteEnable();                                       /* ����дʹ������ */

            sf_CS_ENABLE();                                            /* ʹ��Ƭѡ */
            sf_WriteByte(0x02);                                /* ����AAI����(��ַ�Զ����ӱ��) */
            sf_WriteByte((_uiWriteAddr & 0xFF0000) >> 16); /* ����������ַ�ĸ�8bit */
            sf_WriteByte((_uiWriteAddr & 0xFF00) >> 8);        /* ����������ַ�м�8bit */
            sf_WriteByte(_uiWriteAddr & 0xFF);             /* ����������ַ��8bit */

            for (i = 0; i < 256; i++)
            {
                sf_WriteByte(*_pBuf++);                    /* �������� */
            }

            sf_CS_DISABLE();                               /* ��ֹƬѡ */

            sf_WaitForWriteEnd();                       /* �ȴ�����Flash�ڲ�д������� */

            _uiWriteAddr += 256;
        }

        /* ����д����״̬ */
        sf_CS_ENABLE();
        sf_WriteByte(CMD_DISWR);
        sf_CS_DISABLE();

        sf_WaitForWriteEnd();                           /* �ȴ�����Flash�ڲ�д������� */
    }
#else
    uint32_t i;
    uint32_t l_u32Addr = (CMD_WRPG<<24)|(_uiWriteAddr&0x00FFFFFF);
    if (g_stFlashType.m_u32ChipId == SST25VF016B_ID)
    {
        l_u32Addr = (uint32_t)((CMD_AAI&0xFF)<<24)|(_uiWriteAddr&0x00FFFFFF);
        /* AAIָ��Ҫ��������ݸ�����ż�� */
        if ((_usSize < 2) && (_usSize % 2))
        {
            return ;
        }
        sf_WriteEnable();                                       /* ����дʹ������ */
        drv_spi_bus_write(g_u8SpiFlashDev,l_u32Addr,2,_pBuf,eSpiU32Addr);
        sf_WaitForWriteEnd();                           /* �ȴ�����Flash�ڲ�д������� */
        _usSize -= 2;                                   /* ����ʣ���ֽ� */
        for (i = 0; i < _usSize / 2; i++)
        {
            drv_spi_bus_write(g_u8SpiFlashDev,CMD_AAI,2,_pBuf,eSpiU8Addr);
            _pBuf += 2;
            sf_WaitForWriteEnd();                           /* �ȴ�����Flash�ڲ�д������� */
        }

        sf_WriteProtection();                           /* ����д����״̬ */
        sf_WaitForWriteEnd();                           /* �ȴ�����Flash�ڲ�д������� */
    }
    else    /* for MX25L1606E ? W25Q64BV */
    {
        l_u32Addr = (CMD_WRPG<<24)|(_uiWriteAddr&0x00FFFFFF);
        for(i=0;i<_usSize / 256;i++)
        {
            sf_WriteEnable();                                       /* ����дʹ������ */
            drv_spi_bus_write(g_u8SpiFlashDev,l_u32Addr,256,_pBuf,eSpiU32Addr);
            l_u32Addr += 256;
            _pBuf += 256;
        }
        sf_WriteProtection();                           /* ����д����״̬ */
        sf_WaitForWriteEnd();                           /* �ȴ�����Flash�ڲ�д������� */
    }
#endif


}
/************************************************
�������� �� SFLASH_WriteNoCheck
��    �� �� �޼���дSFLASH
            ����ȷ����д�ĵ�ַ��Χ�ڵ�����ȫ��Ϊ0xFF,�����ڷ�0xFF��д������ݽ�ʧ��!
            �����Զ���ҳ����
            ��ָ����ַ��ʼд��ָ�����ȵ�����,����Ҫȷ����ַ��Խ��!
��    �� �� pBuffer ----- д���������׵�ַ
            WriteAddr --- Ҫд��Flash�ĵ�ַ
            nByte ------- Ҫд����ֽ���
*************************************************/
void SFLASH_WriteNoCheck(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t nByte)
{
    uint16_t PageRemain = 256 - WriteAddr%256;     //��ҳʣ���д���ֽ���

    if(nByte <= PageRemain)
        PageRemain = nByte;                          //������256���ֽ�

    while(1)
    {
        sf_PageWrite(pBuffer, WriteAddr, PageRemain);
        if(nByte == PageRemain)                      //д�����
            break;
        else                                         //д��δ����
        {
            pBuffer += PageRemain;                     //��һҳд������
            WriteAddr += PageRemain;                   //��һҳд�����ݵ�ַ
            nByte -= PageRemain;                       //��д���ֽ����ݼ�

            if(nByte > 256)
                PageRemain = 256;                        //��д��1ҳ(256�ֽ�)������
            else
                PageRemain = nByte;                      //��д������1ҳ(256�ֽ�)������
        }
    }
}


/************************************************
�������� �� SFLASH_WriteNByte
��    �� �� ��ReadAddr��ַ��ʼ����д��nByte��SFLASH��
��    �� �� pBuffer ----- д���������׵�ַ
            WriteAddr --- Ҫд��Flash�ĵ�ַ
            nByte ------- Ҫд����ֽ���(���65535B = 64K ��)
*************************************************/
void SFLASH_WriteNByte(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t nByte)
{
    static uint8_t SectorBuf[4096];                //����buf
    uint32_t SecPos;                               //����λ��
    uint16_t SecOff;                               //����ƫ��
    uint16_t SecRemain;                            //ʣ������
    uint16_t i;

    SecPos = WriteAddr/g_stFlashType.m_u16SectorSize;                       //��ַ��������(0~511)
    SecOff = WriteAddr%g_stFlashType.m_u16SectorSize;                       //��ַ����������ƫ��
    SecRemain = g_stFlashType.m_u16SectorSize-SecOff;                       //��ַ��������ʣ���ֽ���(������С4096B=4KB)

    if(nByte <= SecRemain)
    SecRemain = nByte;                           //д�����ݴ�С < ʣ��ռ��С (��ʣ��ռ乻������Щ����)

    while(1)
    {
        /* ��1����У�� */
        sf_ReadBuffer(SectorBuf, SecPos*g_stFlashType.m_u16SectorSize, g_stFlashType.m_u16SectorSize);        //������������������
        for(i=0; i<SecRemain; i++)                             //У������,�Ƿ���Ҫ����
        {
            if(SectorBuf[SecOff + i] != 0xFF)                    //�洢���ݲ�Ϊ0xFF ����Ҫ����
                break;
        }
        if(i < SecRemain)                                      //��Ҫ����
        {
            sf_EraseSector(SecPos*g_stFlashType.m_u16SectorSize);                          //����������
            for(i=0; i<SecRemain; i++)                           //����д�������(��1��ʱ����д������������ʣ��Ŀռ�)
            {
                SectorBuf[SecOff + i] = pBuffer[i];
            }
            SFLASH_WriteNoCheck(SectorBuf, SecPos*g_stFlashType.m_u16SectorSize, g_stFlashType.m_u16SectorSize);   //д���������������� = ������ + ��д�����ݣ�
        }
        else
            SFLASH_WriteNoCheck(pBuffer, WriteAddr, SecRemain);  //����Ҫ����,ֱ��д������ʣ��ռ�

        if(nByte == SecRemain)                       //д�����
        {
            sf_WriteProtection();                           /* ����д����״̬ */

            break;
        }
        else                                         //д��δ����
        {
            SecPos++;                                  //������ַ��1
            SecOff = 0;                                //ƫ��λ�ù���
            pBuffer += SecRemain;                      //ָ��ƫ��
            WriteAddr += SecRemain;                    //д��ַƫ��
            nByte -= SecRemain;                        //��д���ֽ����ݼ�
            if(nByte > g_stFlashType.m_u16SectorSize)
                SecRemain = g_stFlashType.m_u16SectorSize;                        //��д��1����(4096�ֽ�)������
            else
                SecRemain = nByte;                       //��д������1����(4096�ֽ�)������
        }
    }
}
/**********************************************************************************************************
*   �� �� ��: sf_ReadBuffer
*   ����˵��: ������ȡ�����ֽڡ��ֽڸ������ܳ���оƬ������
*   ��    ��:   _pBuf : ����Դ��������
*               _uiReadAddr ���׵�ַ
*               _usSize �����ݸ���, ���Դ���PAGE_SIZE,���ǲ��ܳ���оƬ������
*   �� �� ֵ: ��
**********************************************************************************************************/
void sf_ReadBuffer(uint8_t * _pBuf, uint32_t _uiReadAddr, uint32_t _uiSize)
{
#if (!USE_FLASH_DRIVER)
    uint32_t l_u32Addr = (CMD_READ<<24)|(_uiReadAddr&0x00FFFFFF);
#endif
    /* �����ȡ�����ݳ���Ϊ0���߳�������Flash��ַ�ռ䣬��ֱ�ӷ��� */
    if ((_uiSize == 0) ||(_uiReadAddr + _uiSize) > g_stFlashType.m_u32ChipSize)
    {
        return;
    }

#if USE_FLASH_DRIVER
    /* ������������ */
    sf_CS_ENABLE();                                        /* ʹ��Ƭѡ */
    sf_WriteByte(CMD_READ);                            /* ���Ͷ����� */
    sf_WriteByte((_uiReadAddr & 0xFF0000) >> 16);  /* ����������ַ�ĸ�8bit */
    sf_WriteByte((_uiReadAddr & 0xFF00) >> 8);     /* ����������ַ�м�8bit */
    sf_WriteByte(_uiReadAddr & 0xFF);              /* ����������ַ��8bit */
    while (_uiSize--)
    {
        *_pBuf++ = sf_ReadByte();          /* ��һ���ֽڲ��洢��pBuf�������ָ���Լ�1 */
    }
    sf_CS_DISABLE();                                               /* ����Ƭѡ */
#else
    drv_spi_bus_read(g_u8SpiFlashDev,l_u32Addr,_uiSize,_pBuf,eSpiU32Addr);
#endif

}

/*
*********************************************************************************************************
*   �� �� ��: sf_ReadID
*   ����˵��: ��ȡ����ID
*   ��    ��:  ��
*   �� �� ֵ: 32bit������ID (���8bit��0����ЧIDλ��Ϊ24bit��
*********************************************************************************************************
*/
uint32_t sf_ReadID(void)
{
#if USE_FLASH_DRIVER
    uint32_t    uiID    ;
    uint8_t id1, id2, id3;

    sf_CS_ENABLE();                                                    /* ʹ��Ƭѡ */
    sf_WriteByte(CMD_RDID);                                /* ���Ͷ�ID���� */

    id1=sf_ReadByte();
    id2=sf_ReadByte();
    id3=sf_ReadByte();

    sf_CS_DISABLE();                                                   /* ����Ƭѡ */

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
*   �� �� ��: sf_ReadInfo
*   ����˵��: ��ȡ����ID,�������������
*   ��    ��:  ��
*   �� �� ֵ: ��
*********************************************************************************************************
*/
void sf_ReadInfo(void)
{
    /* �Զ�ʶ����Flash�ͺ� */
    {
        g_stFlashType.m_u32ChipId = sf_ReadID();    /* оƬID */

        switch (g_stFlashType.m_u32ChipId)
        {
            case SST25VF016B_ID:
                strcpy(g_stFlashType.m_s8ChipName, "SST25VF016B");
                g_stFlashType.m_u32ChipSize = 2 * 1024 * 1024;  /* ������ = 2M */
                g_stFlashType.m_u16SectorSize = 4 * 1024;           /* ҳ���С = 4K */
                break;

            case MX25L1606E_ID:
                strcpy(g_stFlashType.m_s8ChipName, "MX25L1606E");
                g_stFlashType.m_u32ChipSize  = 2 * 1024 * 1024; /* ������ = 2M */
                g_stFlashType.m_u16SectorSize = 4 * 1024;           /* ҳ���С = 4K */
                break;

            case W25Q64BV_ID:
                strcpy(g_stFlashType.m_s8ChipName, "W25Q64BV");
                g_stFlashType.m_u32ChipSize = 8 * 1024 * 1024;  /* ������ = 8M */
                g_stFlashType.m_u16SectorSize = 4 * 1024;           /* ҳ���С = 4K */
                break;

            case W25Q32BV_ID:
                strcpy(g_stFlashType.m_s8ChipName, "W25Q32BV");
                g_stFlashType.m_u32ChipSize = 4 * 1024 * 1024;  /* ������ = 4M */
                g_stFlashType.m_u16SectorSize = 4 * 1024;           /* ҳ���С = 4K */
                break;
            case W25Q16BV_ID:
                strcpy(g_stFlashType.m_s8ChipName, "W25Q16BV");
                g_stFlashType.m_u32ChipSize = 2 * 1024 * 1024;  /* ������ = 2M */
                g_stFlashType.m_u16SectorSize = 4 * 1024;           /* ҳ���С = 4K */
                break;

            case W25Q128_ID:
                strcpy(g_stFlashType.m_s8ChipName, "W25Q128");
                g_stFlashType.m_u32ChipSize = 16 * 1024 * 1024;  /* ������ = 16M */
                g_stFlashType.m_u16SectorSize = 4 * 1024;           /* ҳ���С = 4K */
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
*   �� �� ��: sf_NeedErase
*   ����˵��: �ж�дPAGEǰ�Ƿ���Ҫ�Ȳ�����
*   ��    ��:   _ucpOldBuf �� ������
*              _ucpNewBuf �� ������
*              _uiLen �����ݸ��������ܳ���ҳ���С
*   �� �� ֵ: 0 : ����Ҫ������ 1 ����Ҫ����
*********************************************************************************************************
*/
static uint8_t sf_NeedErase(uint8_t * _ucpOldBuf, uint8_t *_ucpNewBuf, uint16_t _usLen)
{
    uint16_t i;
    uint8_t ucOld;

    /*
    �㷨��1����old ��, new ����
          old    new
          1101   0101
    ~     1111
        = 0010   0101

    �㷨��2��: old �󷴵Ľ���� new λ��
          0010   old
    &     0101   new
         =0000

    �㷨��3��: ���Ϊ0,���ʾ�������. �����ʾ��Ҫ����
    */

    for (i = 0; i < _usLen; i++)
    {
        ucOld = *_ucpOldBuf++;
        ucOld = ~ucOld;

        /* ע������д��: if (ucOld & (*_ucpNewBuf++) != 0) */
        if ((ucOld & (*_ucpNewBuf++)) != 0)
        {
                return 1;
        }
    }
    return 0;
}

/*
*********************************************************************************************************
*   �� �� ��: sf_CmpData
*   ����˵��: �Ƚ�Flash������.
*   ��    ��:   _ucpTar : ���ݻ�����
*               _uiSrcAddr ��Flash��ַ
*               _uiSize �����ݸ���, ���Դ���PAGE_SIZE,���ǲ��ܳ���оƬ������
*   �� �� ֵ: 0 = ���, 1 = ����
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

    /* �����ȡ�����ݳ���Ϊ0���߳�������Flash��ַ�ռ䣬��ֱ�ӷ��� */
    if ((_uiSrcAddr + _uiSize) > g_stFlashType.m_u32ChipSize)
    {
        return 1;
    }

#if USE_FLASH_DRIVER
    if (_uiSize == 0)
    {
        return 0;
    }

    sf_CS_ENABLE();                                    /* ʹ��Ƭѡ */
    sf_WriteByte(CMD_READ);                            /* ���Ͷ����� */
    sf_WriteByte((_uiSrcAddr & 0xFF0000) >> 16);       /* ����������ַ�ĸ�8bit */
    sf_WriteByte((_uiSrcAddr & 0xFF00) >> 8);      /* ����������ַ�м�8bit */
    sf_WriteByte(_uiSrcAddr & 0xFF);                   /* ����������ַ��8bit */
    while (_uiSize--)
    {
        /* ��һ���ֽ� */
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
*   �� �� ��: sf_AutoWritePage
*   ����˵��: д1��PAGE��У��,�������ȷ������д���Ρ��������Զ���ɲ���������
*   ��    ��:   _pBuf : ����Դ��������
*               _uiWriteAddr ��Ŀ�������׵�ַ
*               _usSize �����ݸ��������ܳ���ҳ���С
*   �� �� ֵ: 0 : ���� 1 �� �ɹ�
*********************************************************************************************************
*/
static uint8_t sf_AutoWritePage(uint8_t *_ucpSrc, uint32_t _uiWrAddr, uint16_t _usWrLen)
{
    uint16_t i;
    uint16_t j;                 /* ������ʱ */
    uint32_t uiFirstAddr;       /* ������ַ */
    uint8_t ucNeedErase;        /* 1��ʾ��Ҫ���� */
    uint8_t cRet;
    static uint8_t sl_u8spiBuf[4096];

    /* ����Ϊ0ʱ����������,ֱ����Ϊ�ɹ� */
    if (_usWrLen == 0)
    {
        return 1;
    }

    /* ���ƫ�Ƶ�ַ����оƬ�������˳� */
    if (_uiWrAddr >= g_stFlashType.m_u32ChipSize)
    {
        return 0;
    }

    /* ������ݳ��ȴ����������������˳� */
    if (_usWrLen > g_stFlashType.m_u16SectorSize)
    {
        return 0;
    }

    /* ���FLASH�е�����û�б仯,��дFLASH */
    sf_ReadBuffer(sl_u8spiBuf, _uiWrAddr, _usWrLen);
    if (memcmp(sl_u8spiBuf, _ucpSrc, _usWrLen) == 0)
    {
        return 1;
    }

    /* �ж��Ƿ���Ҫ�Ȳ������� */
    /* ����������޸�Ϊ�����ݣ�����λ���� 1->0 ���� 0->0, ���������,���Flash���� */
    ucNeedErase = 0;
    if (sf_NeedErase(sl_u8spiBuf, _ucpSrc, _usWrLen))
    {
        ucNeedErase = 1;
    }

    uiFirstAddr = _uiWrAddr & (~(g_stFlashType.m_u16SectorSize - 1));

    if (_usWrLen == g_stFlashType.m_u16SectorSize)      /* ������������д */
    {
        for (i = 0; i < g_stFlashType.m_u16SectorSize; i++)
        {
            sl_u8spiBuf[i] = _ucpSrc[i];
        }
    }
    else                        /* ��д�������� */
    {
        /* �Ƚ��������������ݶ��� */
        sf_ReadBuffer(sl_u8spiBuf, uiFirstAddr, g_stFlashType.m_u16SectorSize);

        /* ���������ݸ��� */
        i = _uiWrAddr & (g_stFlashType.m_u16SectorSize - 1);
        memcpy(&sl_u8spiBuf[i], _ucpSrc, _usWrLen);
    }

    /* д��֮�����У�飬�������ȷ����д�����3�� */
    cRet = 0;
    for (i = 0; i < 3; i++)
    {

        /* ����������޸�Ϊ�����ݣ�����λ���� 1->0 ���� 0->0, ���������,���Flash���� */
        if (ucNeedErase == 1)
        {
            sf_EraseSector(uiFirstAddr);        /* ����1������ */
        }

        /* ���һ��PAGE */
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

            /* ʧ�ܺ��ӳ�һ��ʱ�������� */
            for (j = 0; j < 10000; j++);
        }
    }

    return cRet;
}

/*
*********************************************************************************************************
*   �� �� ��: sf_WriteBuffer
*   ����˵��: д1��������У��,�������ȷ������д���Ρ��������Զ���ɲ���������
*   ��    ��:   _pBuf : ����Դ��������
*               _uiWrAddr ��Ŀ�������׵�ַ
*               _usSize �����ݸ��������ܳ���ҳ���С
*   �� �� ֵ: 1 : �ɹ��� 0 �� ʧ��
*********************************************************************************************************
*/
uint8_t sf_WriteBuffer(uint8_t* _pBuf, uint32_t _uiWriteAddr, uint16_t _usWriteSize)
{
    uint16_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;

    Addr = _uiWriteAddr % g_stFlashType.m_u16SectorSize;
    count = g_stFlashType.m_u16SectorSize - Addr;
    NumOfPage =  _usWriteSize / g_stFlashType.m_u16SectorSize;
    NumOfSingle = _usWriteSize % g_stFlashType.m_u16SectorSize;

    if (Addr == 0) /* ��ʼ��ַ��ҳ���׵�ַ  */
    {
        if (NumOfPage == 0) /* ���ݳ���С��ҳ���С */
        {
            if (sf_AutoWritePage(_pBuf, _uiWriteAddr, _usWriteSize) == 0)
            {
                return 0;
            }
        }
        else    /* ���ݳ��ȴ��ڵ���ҳ���С */
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
    else  /* ��ʼ��ַ����ҳ���׵�ַ  */
    {
        if (NumOfPage == 0) /* ���ݳ���С��ҳ���С */
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
        else    /* ���ݳ��ȴ��ڵ���ҳ���С */
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
    return 1;   /* �ɹ� */
}

#endif

