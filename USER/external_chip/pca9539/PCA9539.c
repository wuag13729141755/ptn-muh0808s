/**
 *****************************************************************************
  * Update
  * @Version : V1.0
  * @By      : dengzhiqin
  * @Date    : 2017 / 12 / 05
  * @Brief   : driver of pca9539
  *
  *****************************************************************************
**/
#include "includes.h"

StartupPCA9539Init pPCA9539InitFunc = NULL;

#if (_EN_CHIP_PCA9539_USE == _MACRO_ENABLE)

extern SemaphoreHandle_t xSemaphore_IIC[NUM_OF_I2C_BUS_MAX];
uint16_t pca_output_val=0, pca_output_val_keep=0;
uint16_t g_u16PcaPortConfig = 0;

uint16_t pca9539_i2c_read(uint8_t reg_addr)
{
	uint16_t input_val = 0;

#if _SYSTEM_SUPPORT_RTOS
	xSemaphoreTake(xSemaphore_IIC[_I2C_TABLE_INDEX_PCA9539], portMAX_DELAY);
#endif
	i2c_read_bytes(_I2C_TABLE_INDEX_PCA9539, DEVICE_ADDR_PCA9539,\
					reg_addr, (uint8_t *)&input_val, 2, FALSE);
#if _SYSTEM_SUPPORT_RTOS
	xSemaphoreGive(xSemaphore_IIC[_I2C_TABLE_INDEX_PCA9539]);
#endif

	return input_val;
}

void pca9539_i2c_write(uint8_t reg_addr, uint16_t wr_val)
{
#if _SYSTEM_SUPPORT_RTOS
	xSemaphoreTake(xSemaphore_IIC[_I2C_TABLE_INDEX_PCA9539], portMAX_DELAY);
#endif
	i2c_write_bytes(_I2C_TABLE_INDEX_PCA9539, DEVICE_ADDR_PCA9539,\
					reg_addr, (uint8_t *)&wr_val, 2, FALSE);
#if _SYSTEM_SUPPORT_RTOS
	xSemaphoreGive(xSemaphore_IIC[_I2C_TABLE_INDEX_PCA9539]);
#endif
}

uint16_t pca9539_inputport_read(void)
{
	uint16_t input_val = 0;

	input_val = pca9539_i2c_read(PCA_REG_INPUT_PORT0);

	return input_val;
}

void pca9539_outputport_write(uint16_t port_val)
{
	pca9539_i2c_write(PCA_REG_OUTPUT_PORT0, port_val);
}

uint16_t pca9539_outputport_read(void)
{
	uint16_t  port_val;

	port_val = pca9539_i2c_read(PCA_REG_OUTPUT_PORT0);

	return port_val;
}

void pca9539_pol_inv_write(uint16_t inv_val)
{
	pca9539_i2c_write(PCA_REG_POL_INV_PORT0, inv_val);
}

void pca9539_port_set_output(uint16_t cfg_val)
{
	uint16_t cfg_pre;

	cfg_pre = pca9539_i2c_read(PCA_REG_CONFIG_PORT0);
	cfg_pre &= (~cfg_val);
	pca9539_i2c_write(PCA_REG_CONFIG_PORT0, cfg_pre);
}

void pca9539_port_set_input(uint16_t cfg_val)
{
	uint16_t cfg_pre;

	cfg_pre = pca9539_i2c_read(PCA_REG_CONFIG_PORT0);
	cfg_pre |= cfg_val;
	pca9539_i2c_write(PCA_REG_CONFIG_PORT0, cfg_pre);
}

#if (_EN_GPIO_PCA9539_RESET==_MACRO_ENABLE)
void pca9539_rst(void)
{
	#if ((_PCA_9539_RST_PORT>=PORT_GPIOA)&&(_PCA_9539_RST_PORT<=PORT_GPIOG))
		#if (_PCA_9539_RST_INIT_LEVEL == _GPIO_PIN_LEVEL_HIGH)
			GPIO_ResetBits(STM32_GPIO_PORT[_PCA_9539_RST_PORT-PORT_GPIOA], _PCA_9539_RST_PIN);
			delay_ms(20);
			GPIO_SetBits(STM32_GPIO_PORT[_PCA_9539_RST_PORT-PORT_GPIOA], _PCA_9539_RST_PIN);
			delay_ms(100);
		#else
			GPIO_SetBits(STM32_GPIO_PORT[_PCA_9539_RST_PORT-PORT_GPIOA], _PCA_9539_RST_PIN);
			delay_ms(20);
			GPIO_ResetBits(STM32_GPIO_PORT[_PCA_9539_RST_PORT-PORT_GPIOA], _PCA_9539_RST_PIN);
			delay_ms(100);
		#endif
	#endif
}
#endif

void pca9539_init(void)
{
	uint16_t config_port = 0x00;

	#if (_EN_GPIO_PCA9539_RESET==_MACRO_ENABLE)
	pca9539_rst();
	#endif

    config_port = g_u16PcaPortConfig;

	pca9539_port_set_output(config_port);

	pca_output_val_keep = pca_output_val;
	pca9539_outputport_write(pca_output_val);
    if(pPCA9539InitFunc != NULL)
    {
        pPCA9539InitFunc();
    }

}

#endif//#if (_EN_CHIP_PCA9539_USE == _MACRO_ENABLE)











