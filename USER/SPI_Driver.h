#ifndef __SPI_DRIVER_H_
#define __SPI_DRIVER_H_

typedef enum _eSpiDioDirect_t_
{
	eSpiDioDir_Output =0,
	eSpiDioDir_Input,

	eSpiDioDir_Max,
}eSpiDioDirect_t;

typedef enum _eSpiResult_t_
{
	eSpiSuccess=0,
	eSpiError,
	eSpiInvalid,
}eSpiResult_t;

typedef enum _eSpiAdrWide_t_
{
	eSpiU32Addr = 0,
	eSpiU16Addr,
	eSpiU8Addr,
    eSpiU24Addr,
}eSpiAdrWide_t;

eSpiResult_t drv_spi_bus_write(uint8_t u8dev,
								uint32_t u32CmdAddress,
								uint16_t u16Count,
								uint8_t* pu8Buffer,
								eSpiAdrWide_t wFlags);
eSpiResult_t drv_spi_bus_read(uint8_t u8dev,
								uint32_t u32CmdAddress,
								uint16_t u16Count,
								uint8_t* pu8Buffer,
								eSpiAdrWide_t rFlags);
eSpiResult_t drv_spi_direct_write(uint8_t u8dev,
								uint16_t u16Count,
								uint8_t* pu8Buffer);

#endif

























