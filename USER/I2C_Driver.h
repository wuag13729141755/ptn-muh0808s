#ifndef __I2C_DRIVER_H_
#define __I2C_DRIVER_H_

#define I2C_WR	0
#define I2C_RD	1

#if (_EN_UART_DEBUG_PRINTF == _MACRO_ENABLE)
#define Msg_I2c(x)					do{dbg_printf x;}while(0)
#else
#define Msg_I2c(x)
#endif

#if(defined(STM32F2XX_HD))
#define I2C_SPEED_TIME_FOR_50K              150
#define I2C_SPEED_TIME_FOR_75K              90
#define I2C_SPEED_TIME_FOR_100K             70
#define I2C_SPEED_TIME_FOR_150K             40
#define I2C_SPEED_TIME_FOR_200K             30
#define I2C_SPEED_TIME_FOR_250K             22
#define I2C_SPEED_TIME_FOR_300K             16
#define I2C_SPEED_TIME_FOR_400K             10
#elif (defined(GD32F4XX))
#define I2C_SPEED_TIME_FOR_50K              200
#define I2C_SPEED_TIME_FOR_75K              150
#define I2C_SPEED_TIME_FOR_100K             100
#define I2C_SPEED_TIME_FOR_150K             65
#define I2C_SPEED_TIME_FOR_200K             40
#define I2C_SPEED_TIME_FOR_250K             30
#define I2C_SPEED_TIME_FOR_300K             25
#define I2C_SPEED_TIME_FOR_400K             15
#elif (defined(N32G45X))
#define I2C_SPEED_TIME_FOR_50K              150
#define I2C_SPEED_TIME_FOR_75K              90
#define I2C_SPEED_TIME_FOR_100K             70
#define I2C_SPEED_TIME_FOR_150K             40
#define I2C_SPEED_TIME_FOR_200K             30
#define I2C_SPEED_TIME_FOR_250K             22
#define I2C_SPEED_TIME_FOR_300K             11
#define I2C_SPEED_TIME_FOR_400K             8
#else
#define I2C_SPEED_TIME_FOR_50K              45
#define I2C_SPEED_TIME_FOR_75K              25//55
#define I2C_SPEED_TIME_FOR_100K             17//30
#define I2C_SPEED_TIME_FOR_150K             9
#define I2C_SPEED_TIME_FOR_200K             5
#define I2C_SPEED_TIME_FOR_250K             2
#define I2C_SPEED_TIME_FOR_300K             0
#define I2C_SPEED_TIME_FOR_400K             0
#endif

//functions
void bsp_InitI2C(void);
void i2c_Stop(uint8_t dev);
void Init_I2cLineChangePin(void);
void mdrv_SetLineChangePin(pstGpioConfig_t pGpio, ePinLevel_t e_level);
void mdrv_SetI2cLineChange(u8 u8dev,u8 u8index);
#if _ENABLE_SW4052_CS_PIN
void mdrv_SetI2cSw4052CsState(u8 u8dev,ePinLevel_t e_level);
#endif
uint8_t i2c_read_bytes(uint8_t dev, uint8_t dev_addr,\
			uint16_t reg_addr, uint8_t *pData, uint16_t byte_no, bool two_bytes_addr);
uint8_t i2c_write_bytes(uint8_t dev,uint8_t dev_addr,\
			uint16_t reg_addr, uint8_t *pData, uint16_t byte_no, bool two_bytes_addr);

#if 1//((IT663XX_VER != IT663XX_NONE)||(IT666X_VER != IT666X_NONE)||_ENABLE_CHIP_ITE66351||_ENABLE_CHIP_ITE6615)
extern SemaphoreHandle_t xSemaphore_IIC[NUM_OF_I2C_BUS_MAX];
iTE_u1 i2c_read_byte( iTE_u8 address, iTE_u8 offset, iTE_u8 byteno, iTE_u8 *p_data, iTE_u8 device);
iTE_u1 i2c_write_byte( iTE_u8 address, iTE_u8 offset, iTE_u8 byteno, iTE_u8 *p_data, iTE_u8 device);
#endif

#endif

















