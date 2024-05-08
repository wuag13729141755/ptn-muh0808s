/*
* SiI8630/SiI8632/SiI9630 Linux Driver
*
* 2016 © Lattice Semiconductor Corporation
*
* This program is free software; you can redistribute it and/or
* modify it only under the terms of  version 2 of the GNU General Public License
* as published by the Free Software Foundation, and not any later version.
*
* This program is distributed “AS IS” WITHOUT ANY WARRANTY of any kind,
* whether express or implied, including without limitation the implied warranty
* of MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE or NON-INFRINGEMENT.
* See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software Foundation, Inc.,
* 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*
* You may contact Lattice Semiconductor Corporation at 111 SW 5th Ave,
* Suite 700, Portland, OR 97204
*/

#ifndef _SI_PLATFORM_H_
#define _SI_PLATFORM_H_


#define DEV_GEN_INT_INDEX		0
#define DEV_RESET_INDEX			1
#define DEV_FW_WAKE_INDEX		2
#define GPIO_MAX			3
/*#define DEV_EMSC_INT_INDEX	3*/
/*#define DEV_VBUS_CTRL_INDEX	4*/


#define SII_DRV_DEVICE_I2C_ADDR 0x60
#define SII_DRV_DEVICE_I2C_ADDR1 0x62
#define SII_DRV_SPIDERMAN_I2C_ADDR 0x64

#define DEBUG_I2C_WRITE                 1
#define DEBUG_I2C_READ                  0
#define MAX_DEBUG_TRANSFER_SIZE         32

#define SIL9630_I2C         0
#define SIL9630_SPI         1
#define SIL9630_CTRL_USE    SIL9630_I2C

extern bool use_spi;
extern int debug_level;
extern bool debug_reg_dump;

#if 0
struct spi_xfer_mem {
	uint8_t *tx_buf;
	uint8_t *rx_buf;
	/* block commands are asynchronous to normal cbus traffic
	* and CANNOT share a buffer.
	*/
	uint8_t *block_tx_buffers;
	struct spi_transfer spi_xfer[2];
	struct spi_message spi_cmd;
};

struct sii8630_platform_data {
	struct spi_xfer_mem *spi_mem;
	struct gpio sk_gpios[GPIO_MAX];
	struct i2c_adapter *i2c_bus;
	struct i2c_client *i2c_dev_client;
	uint8_t i2c_device_id;
	struct spi_device *spi_dev;
	struct semaphore drv_lock;

	uint32_t i2c_adapter_num;
	uint32_t spi_bus_num;
#ifdef BOARD_MSM_8994
	struct pinctrl *pinctrl;
	struct pinctrl_state *active_state;
#endif
	struct regulator *lvs2;
};
#endif

typedef struct _stSii9630_Device_t_ {
    uint8_t u8DevId;
	uint8_t u8SlaveAddr;
}stSii9630_Device_t,*pstSii9630_Device_t;

extern stSii9630_Device_t g_stSii9630_Device;

//void set_pin_impl(struct sii8630_platform_data *pdata,
//	int pin_idx, int value,
//	const char *function_name, int line_num);
//#define set_pin(pin_idx, value) \
//	set_pin_impl(pdata, pin_idx, value, __func__, __LINE__)

int sii_platform_wr_reg8(pstSii9630_Device_t pstDev,
	uint16_t addr, uint8_t val);
int sii_platform_rd_reg8(pstSii9630_Device_t pstDev,
	uint16_t addr);
int sii_platform_set_bit8(pstSii9630_Device_t pstDev,
	uint16_t addr, uint8_t mask);
int sii_platform_clr_bit8(pstSii9630_Device_t pstDev,
	uint16_t addr, uint8_t mask);
int sii_platform_put_bit8(pstSii9630_Device_t pstDev,
	uint16_t addr, uint8_t mask, uint8_t val);
int sii_platform_fifo_write8(pstSii9630_Device_t pstDev,
	uint16_t addr, uint8_t *values, uint16_t size);
int sii_platform_fifo_read8(pstSii9630_Device_t pstDev,
	uint16_t addr, uint8_t *values, uint16_t size);
int sii_platform_block_write8(pstSii9630_Device_t pstDev,
	uint16_t addr, uint8_t *values, uint16_t size);
int sii_platform_block_read8(pstSii9630_Device_t pstDev,
	uint16_t addr, uint8_t *values, uint16_t size);

void sii_platform_hardware_reset(pstSii9630_Device_t pstDev);

bool sii_platform_is_gen_interrupt_rcvd(void);
bool sii_platform_is_emsc_interrupt_rcvd(void);

bool sii_time_out_milli_is(const uint32_t *p_milli_t_o);
bool sii_time_out_is_expired(const uint32_t *p_milli_t_o,
		uint32_t timeout);
void sii_time_out_milli_set(uint32_t *p_milli_t_o,
		uint32_t time_out);
uint32_t sii_time_milli_get(void);

//int platform_read_i2c_block(struct i2c_adapter *i2c_bus,
//	uint8_t device_id, uint16_t offset, uint16_t count, uint8_t *values);

// int platform_write_i2c_block(struct i2c_adapter *i2c_bus,
//	uint8_t device_id, uint16_t offset, uint16_t count,
//	const uint8_t *values);

#endif /* _SI_PLATFORM_H_ */
