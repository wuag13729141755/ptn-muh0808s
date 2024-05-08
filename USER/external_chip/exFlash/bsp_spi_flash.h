
#ifndef __BSP_SPI_H__
#define __BSP_SPI_H__

#if _ENABLE_SPI_FLASH

#if 0
//--------------------------------------------------------------------
typedef struct _stSpiPortDefine_
{
    GPIO_TypeDef*           ePortType;
    uint16_t                u16GpioPin;
    GPIOMode_TypeDef        eGpioMode;
#if 1//def STM32F2XX_HD
    GPIOOType_TypeDef       eGpioOType;
    GPIOPuPd_TypeDef        eGpioPuPdType;
#endif
    ITStatus                eInitLevel;
}stSpiPortDefine,pstSpiPortDefine;

typedef enum _emSpiPortType_
{
    eSPI_CS = 0,
    eSPI_CLK,
    eSPI_MISO,
    eSPI_MOSI,
    eSPI_SELA,
    eSPI_SELB,

    eSPI_MAX,
}emSpiPortType;
#endif
//--------------------------------------------------------------------------



#define CMD_AAI       0xAD      /* AAI �������ָ��(FOR SST25VF016B) */
#define CMD_DISWR     0x04      /* ��ֹд, �˳�AAI״̬ */
#define CMD_EWRSR     0x50      /* ����д״̬�Ĵ��������� */
#define CMD_WRSR      0x01      /* д״̬�Ĵ������� */
#define CMD_WREN      0x06      /* дʹ������ */
#define CMD_READ      0x03      /* ������������ */
#define CMD_RDSR      0x05      /* ��״̬�Ĵ������� */
#define CMD_RDID      0x9F      /* ������ID���� */
#define CMD_SE        0x20      /* ������������ */
#define CMD_KE        0xD8      /* ����BLOCK������ */
#define CMD_BE        0xC7      /* ������������ */
#define CMD_WRPG      0x02      /* PageProgram(For winbond MX25L1606E) */
#define DUMMY_BYTE    0x00//0xA5        /* ���������Ϊ����ֵ�����ڶ����� */

#define WIP_FLAG      0x01      /* ״̬�Ĵ����е����ڱ�̱�־��WIP) */
#define W25X_PowerDown          0xB9    //����,�͹���
#define W25X_ReleasePowerDown   0xAB    //�ָ��ϵ�

typedef struct _stFlashType_
{
    u32 m_u32ChipId;
    char m_s8ChipName[16];
    u32 m_u32ChipSize;
    u16 m_u16SectorSize;
}stFlashType,pstFlashType;

typedef enum
{
    SST25VF016B_ID = 0xBF2541,
    MX25L1606E_ID  = 0xC22015,
    W25Q64BV_ID    = 0xEF4017,
    W25Q32BV_ID    = 0xEF4016,
    W25Q16BV_ID    = 0xEF4015,
    W25Q128_ID     = 0xEF4018

}emChipIDType;

extern stFlashType g_stFlashType;

void sf_FlashSelect(u8 u8DevID);
void sf_FlashInit(void);
void sf_EraseChip(void);
void SFLASH_WriteNoCheck(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t nByte);
void sf_ReadBuffer(uint8_t * _pBuf, uint32_t _uiReadAddr, uint32_t _uiSize);
void SFLASH_WriteNByte(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t nByte);
uint32_t sf_ReadID(void);
void sf_ReadInfo(void);
uint8_t sf_WriteBuffer(uint8_t* _pBuf, uint32_t _uiWriteAddr, uint16_t _usWriteSize);

#endif

#endif  // #ifndef __BSP_SPI_H__

