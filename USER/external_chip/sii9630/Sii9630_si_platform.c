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

#include "Sii9630_si_datatypes.h"
#if (_ENABLE_CHIP_SIL9630 == _MACRO_ENABLE)

#include "Sii9630_si_platform.h"
#include "Sii9630_si_lib_log.h"
#include "Sii9630_si_drv_cp8630_regs.h"
#include "Sii9630_si_drv_cp8630.h"
#if (CONFIG_SOURCE_ONLY == 0)
#include "Sii9630_spiderman.h"
#endif

#define I2C_DEV_INFO(addr) \
	{.dev_addr = addr >> 1, .client = NULL}

#define I2C_ADAPTER			4
#define SI_I2C_RETRY_TIMES		2
#define SPI_BUS_NUM			1
#define SPI_OPCODE__WRITE		0x02
#define SPI_OPCODE__READ		0x0B
#define SPI_READ_NUM_DUMMY_BYTES	3

#define LOCAL_BLK_RCV_BUFFER_SIZE	288
#define MAX_SPI_PAYLOAD_SIZE		LOCAL_BLK_RCV_BUFFER_SIZE
#define MAX_SPI_CMD_SIZE		3
#define EMSC_WRITE_SPI_CMD_SIZE		1
#define EMSC_READ_SPI_CMD_SIZE		1
#define MAX_SPI_DUMMY_XFER_BYTES	20
#define MAX_SPI_XFER_BUFFER_SIZE	(MAX_SPI_CMD_SIZE + \
					MAX_SPI_DUMMY_XFER_BYTES + \
					MAX_SPI_PAYLOAD_SIZE)

#define MAX_SPI_EMSC_BLOCK_SIZE		(MAX_SPI_CMD_SIZE + \
					MAX_SPI_PAYLOAD_SIZE)

#define MAX_I2C_PAYLOAD_SIZE		LOCAL_BLK_RCV_BUFFER_SIZE
#define MAX_I2C_CMD_SIZE		0

#define MAX_I2C_EMSC_BLOCK_SIZE (MAX_I2C_CMD_SIZE + MAX_I2C_PAYLOAD_SIZE)

/*HDCP arbitration masks*/
#define HDCP_ENABLE__APP 0x01
#define HDCP_ENABLE__API 0x02
#define GPIO__DEV_GEN_INT 61
#define GPIO__DEV_EMSC_INT 92
#define GPIO__DEV_RESET 19
#define GPIO__DEV_WAKE_UP 0
#define GPIO__VBUS_CTRL 60

#define NUM_BLOCK_QUEUE_REQUESTS 4
#define HDMI_ANALOG_PHY		0
#define MHL_ANALOG_PHY		1

//static char *buildTime = "Built " __DATE__ "-" __TIME__;
//static char *buildVersion = BUILD_NUM_STRING;

int debug_level = DBG_MSG_DEBUG;
bool debug_reg_dump;
bool use_spi = FALSE;
bool probe_fail = FALSE;
stSii9630_Device_t g_stSii9630_Device;

#define I2C_BLOCK_MAX 32
#define min_of(a, b) (((a) < (b)) ? (a) : (b))
#if 0
int platform_read_i2c_block(struct i2c_adapter *i2c_bus,
	uint8_t device_id, uint16_t offset, uint16_t count, uint8_t *values)
{
	uint8_t *buf;
	int status = 0;
	int index = 0;
	uint8_t offset_length = 2;
	uint16_t tmp_cnt = count;
	uint16_t tmp_offset = offset;

	buf = kzalloc(I2C_BLOCK_MAX + offset_length, GFP_KERNEL);

	for (index = 0; tmp_cnt > 0; index++) {
		uint8_t length = (uint8_t)min_of(tmp_cnt, I2C_BLOCK_MAX);
		/*
		 * message 1 - [s]:[page][w]:[offset]:[st/rs]
		 * message 2 - [st/rs]:[data]:[st/rs]
		 */
		struct i2c_msg msg[2] = {
			{
				.addr = device_id >> 1,
				.flags = 0,
				.buf = &buf[0],
				.len = offset_length,
			},
			{
				.addr = device_id >> 1,
				.flags = I2C_M_RD,
				.buf = &buf[2],
				.len = length,
			}
		};
		memset(buf, 0, sizeof(I2C_BLOCK_MAX + offset_length));
		buf[0] = tmp_offset >> 8;
		buf[1] = (uint8_t)tmp_offset;
		/* initiate i2c_transfer for read */
		status = i2c_transfer(i2c_bus, msg, 2);
		memcpy(&values[index * I2C_BLOCK_MAX], &buf[2], length);
		tmp_cnt -= length;
		tmp_offset += length;
	}
#ifdef CONFIG_CAPTURE_REGISTER_DUMP
	sii_lib_write_to_log_file(values, count+2, SII_READ);
#endif
	kfree(buf);
	return status;
}

 int platform_write_i2c_block(struct i2c_adapter *i2c_bus,
	uint8_t device_id, uint16_t offset, uint16_t count,
	const uint8_t *values)
{
	uint8_t *buf;
	int status = 0;
	int index = 0;
	uint8_t retry = 0;
	uint8_t offset_length = 2;
	uint16_t tmp_cnt = count;
	uint16_t tmp_offset = offset;

	buf = kzalloc(I2C_BLOCK_MAX + offset_length, GFP_KERNEL);

	for (index = 0; tmp_cnt > 0; index++) {
		uint8_t length = (uint8_t)min_of(tmp_cnt, I2C_BLOCK_MAX);

		struct i2c_msg msg[] = {
			{
				.addr = device_id >> 1,
				.flags = 0,
				.len = (length + offset_length),
				.buf = buf,
			}
		};
		memset(buf, 0, sizeof(I2C_BLOCK_MAX + offset_length));
		buf[0] = tmp_offset >> 8;
		buf[1] = (uint8_t)(tmp_offset);
		memcpy(&buf[2], &values[index * I2C_BLOCK_MAX], length);

		for (retry = 0; retry < SI_I2C_RETRY_TIMES; retry++) {
			status = i2c_transfer(i2c_bus, msg, 1);

			if (status == 1)
				break;

			sii_log_debug("I2C Transfer failed. retrying\n");
			msleep(20);
		}
		tmp_cnt -= length;
		tmp_offset += length;
	}
#ifdef CONFIG_CAPTURE_REGISTER_DUMP
	sii_lib_write_to_log_file(values, count+2, SII_WRITE);
#endif
	kfree(buf);
	return status;
}
#endif

int sii_platform_block_write8(pstSii9630_Device_t pstDev,
	const uint16_t addr, uint8_t *values, uint16_t size)
{
	int ret = 0;

    #if (SIL9630_CTRL_USE == SIL9630_SPI)
    {
        eSpiResult_t estate = eSpiError;
		//ret = spi_write(pdata->spi_dev, pdata->spi_mem->tx_buf, length);
        estate = drv_spi_bus_write(pstDev->u8DevId, addr, size,(uint8_t *)values,eSpiU16Addr);
        if(estate == eSpiSuccess)
            ret = 0;
        else
            ret = -1;

		if (ret != 0) {
			sii_log_debug(
				"SPI write block failed, addr: 0x%02x	ret = %d\n",
				addr, ret);
			//ret = -EIO;
		} else
			ret = 0;

	}
    #else
    {
    	u8 l_u8state = 0;
		//ret = platform_write_i2c_block(pdata->i2c_bus,
		//	pdata->i2c_device_id, addr, size, values);
        l_u8state = i2c_write_bytes(pstDev->u8DevId,pstDev->u8SlaveAddr,addr,(uint8_t *)values,size,TRUE);
        ret = (l_u8state==0) ? 0 : -1;

		if (ret < 0)
			pr_err("%s: I2C block write failed, addr: 0x%02X ret = %d\n",
				__func__, addr, ret);
		else
			ret = 0;
	}
    #endif
	return ret;
}

int sii_platform_block_read8(pstSii9630_Device_t pstDev,
	const uint16_t addr, uint8_t *values, uint16_t size)
{
	int ret = 0;

    #if (SIL9630_CTRL_USE == SIL9630_SPI)
	{
        eSpiResult_t estate = eSpiError;
        estate = drv_spi_bus_read(pstDev->u8DevId,addr,size,values,eSpiU16Addr);
        if(estate != eSpiSuccess)
            ret = -1;
        else
            ret = 0;

		if (ret != 0) {
			sii_log_debug(
				"SPI Read block failed, addr: 0x%02x	ret = %d\n",
				addr, ret);
			//ret = -EIO;
		} else
			ret = 0;
	}
    #else
    {
    	u8 l_u8state = 0;
		//ret = platform_read_i2c_block(pdata->i2c_bus,
		//	pdata->i2c_device_id, addr, size, values);
        l_u8state = i2c_read_bytes(pstDev->u8DevId,pstDev->u8SlaveAddr,addr,values,size,TRUE);
        ret = (l_u8state==0) ? 0 : -1;
		if (ret != 0)
		{
			pr_err("%s: I2C block read failed, addr: 0x%02X	ret = %d\n",
				__func__, addr, ret);
		}
		else
			ret = 0;
	}
    #endif
	return ret;
}

int sii_platform_wr_reg8(pstSii9630_Device_t pstDev,
	const uint16_t addr, uint8_t val)
{
	return sii_platform_block_write8(pstDev, addr, &val, 1);
}

int sii_platform_rd_reg8(pstSii9630_Device_t pstDev,
	const uint16_t addr)
{
	uint8_t val = 0;
	int status = 0;

	status = sii_platform_block_read8(pstDev, addr, &val, 1);

	return status ? status : val;
}

int sii_platform_set_bit8(pstSii9630_Device_t pstDev,
	const uint16_t addr, const uint8_t mask)
{
	int val;
	int status = 0;

	val = sii_platform_rd_reg8(pstDev, addr);
	if (val < 0)
		return val;

	val = (val & ((uint8_t)~mask)) | mask;
	status = sii_platform_wr_reg8(pstDev, addr, val);

	return status;
}

int sii_platform_clr_bit8(pstSii9630_Device_t pstDev,
	const uint16_t addr, const uint8_t mask)
{
	int val;
	int status = 0;

	val = sii_platform_rd_reg8(pstDev, addr);
	if (val < 0)
		return val;

	val = (val & ((uint8_t)~mask));
	status = sii_platform_wr_reg8(pstDev, addr, val);

	return status;
}

int sii_platform_put_bit8(pstSii9630_Device_t pstDev,
	const uint16_t addr, const uint8_t mask, const uint8_t val)
{
	int temp;
	int status = 0;

	temp = sii_platform_rd_reg8(pstDev, addr);
	if (temp < 0)
		return temp;

	temp &= ((uint8_t)~mask);
	temp |= (mask & val);
	status = sii_platform_wr_reg8(pstDev, addr, temp);

	return status;
}

int sii_platform_fifo_read8(pstSii9630_Device_t pstDev,
	const uint16_t addr, uint8_t *values, uint16_t size)
{
	int temp = 0;
	uint16_t i;

	for (i = 0; i < size; i++) {
		temp = sii_platform_rd_reg8(pstDev, addr);

		if (temp < 0)
			return temp;
		values[i] = temp;
	}
	return 0;
}

int sii_platform_fifo_write8(pstSii9630_Device_t pstDev,
	const uint16_t addr, uint8_t *values, uint16_t size)
{
	uint16_t i;
	int status = 0;

	for (i = 0; i < size; i++) {
		status = sii_platform_wr_reg8(pstDev, addr, *(values+i));

		if (status < 0)
			return status;
	}
	return status;
}

#if 0
bool sii_platform_is_gen_interrupt_rcvd(void)
{
	return gpio_get_value(GPIO__DEV_GEN_INT) ? true : false;
}

bool sii_platform_is_emsc_interrupt_rcvd(void)
{
	return gpio_get_value(GPIO__DEV_EMSC_INT) ? true : false;
}
#endif

void sii_platform_hardware_reset(pstSii9630_Device_t pstDev)
{
	//pr_debug("CP8630:: HW-Reset\n");
	//gpio_set_value_cansleep(
	//	pdata->sk_gpios[DEV_RESET_INDEX].gpio, 0);
	//usleep_range(10000, 40000);
	//gpio_set_value_cansleep(
	//	pdata->sk_gpios[DEV_RESET_INDEX].gpio, 1);
}

#if 0
void set_pin_impl(struct sii8630_platform_data *pdata,
	int pin_idx, int value,
	const char *function_name, int line_num)
{
	sii_log_info("set_pin(%s, %d)\n",
		pdata->sk_gpios[pin_idx].label,
		value);
	if (pin_idx < ARRAY_SIZE(pdata->sk_gpios))
		gpio_set_value(pdata->sk_gpios[pin_idx].gpio, value);
}
#endif
#if 0
static int starter_kit_init(pstSii9630_Device_t pstDev)
{
	int ret = 0;

	/* Acquire the GPIO pins needed to control the starter kit. */
	//ret = gpio_request_array(pdata->sk_gpios, ARRAY_SIZE(pdata->sk_gpios));
	//if (ret < 0) {
	//	pr_err("%s(: gpio_request_array failed, error code %d\n",
	//		__func__, ret);
	//}
	return ret;
}
#endif
#if 0
static int si_drv_8630_parse_dt(struct device *dev,
	pstSii9630_Device_t pstDev)
{
	struct device_node *np = dev->of_node;
	int value;
	int rc = 0;
	pdata->lvs2 = NULL;

#ifdef BOARD_MSM_8994
	pdata->pinctrl = devm_pinctrl_get(dev);
	if (IS_ERR_OR_NULL(pdata->pinctrl)) {
		sii_log_err("%s: failed to get pinctrl\n", __func__);
		rc = -ENODEV;
		goto dt_exit;
	}

	pdata->active_state =
		pinctrl_lookup_state(pdata->pinctrl, "simg_active");
	if (IS_ERR_OR_NULL(pdata->active_state)) {
		sii_log_err("%s: failed to find simg_active state\n",
				__func__);
		rc = -ENODEV;
		goto dt_exit;
	}

	if (pinctrl_select_state(pdata->pinctrl, pdata->active_state)) {
		sii_log_err("%s: failed to select simg active state\n",
				__func__);
	}
#endif
	pdata->lvs2 = devm_regulator_get(dev, "lvs2");
	if (IS_ERR_OR_NULL(pdata->lvs2)) {
		sii_log_err("%s: Unable to get regulator\n", __func__);
		rc = -ENODEV;
		goto dt_exit;
	}

	/*rc = regulator_set_voltage(pdata->lvs2, 1800000, 1800000);
	if (rc) {
		sii_log_err("%s: Unable to set regulator voltage\n",
				__func__);
		goto dt_exit;
	}*/

	rc = regulator_enable(pdata->lvs2);
	if (rc) {
		sii_log_err("%s: Regulator enable failed\n", __func__);
		goto dt_exit;
	}

	pdata->sk_gpios[DEV_GEN_INT_INDEX].flags = GPIOF_IN;
	pdata->sk_gpios[DEV_GEN_INT_INDEX].label = "gen_intr";

	pdata->sk_gpios[DEV_RESET_INDEX].flags = GPIOF_OUT_INIT_HIGH;
	pdata->sk_gpios[DEV_RESET_INDEX].label = "dev_reset";

	pdata->sk_gpios[DEV_FW_WAKE_INDEX].flags = GPIOF_OUT_INIT_LOW;
	pdata->sk_gpios[DEV_FW_WAKE_INDEX].label = "tx_wakeup";

	value = of_get_named_gpio_flags(np, "simg,irq-gpio", 0, NULL);
	if (value >= 0) {
		pdata->sk_gpios[DEV_GEN_INT_INDEX].gpio = value;
	} else {
		sii_log_err("%s: unable to get irq gpio\n", __func__);
		rc = -ENODEV;
		goto dt_exit2;
	}

	value = of_get_named_gpio_flags(np, "simg,reset-gpio", 0, NULL);
	if (value >= 0) {
		pdata->sk_gpios[DEV_RESET_INDEX].gpio = value;
	} else {
		sii_log_err("%s: unable to get reset gpio\n", __func__);
		rc = -ENODEV;
		goto dt_exit2;
	}

	value = of_get_named_gpio_flags(np, "simg,fw_wake-gpio", 0, NULL);
	if (value >= 0) {
		pdata->sk_gpios[DEV_FW_WAKE_INDEX].gpio = value;
	} else {
		sii_log_err("%s: unable to get fw_wake gpio\n", __func__);
		rc = -ENODEV;
		goto dt_exit2;
	}
/*	value = of_get_named_gpio_flags(np, "simg, emsc-irq-gpio", 0, NULL);
	if (value >= 0)
		pdata->sk_gpios[DEV_EMSC_INT_INDEX].gpio = value;

	value = of_get_named_gpio_flags(np, "simg, vbus-ctrl-gpio", 0, NULL);
	if (value >= 0)
		pdata->sk_gpios[DEV_VBUS_CTRL_INDEX].gpio = value;
*/
	if (!of_property_read_u32(np, "simg,device_id", &value))
		pdata->i2c_device_id = (u8)value;

	/*
	 * Need this for I/O expander in case we're using SPI as
	 * the register I/O.
	 */
	if (use_spi) {
		if (!of_property_read_u32(np, "simg,i2c_port#", &value))
			pdata->i2c_adapter_num = value;
	}
	return rc;
dt_exit2:
	if (pdata->lvs2) {
		regulator_disable(pdata->lvs2);
		devm_regulator_put(pdata->lvs2);
	}
dt_exit:
	return rc;
}


static int si_drv_8630_spi_probe(struct spi_device *spi)
{
	int ret;
	struct i2c_adapter *i2c_bus_adapter;
    struct spi_xfer_mem l_spi_mem;
    uint8_t l_TxBuf[MAX_SPI_XFER_BUFFER_SIZE];
    uint8_t l_RxBuf[MAX_SPI_XFER_BUFFER_SIZE];
    uint8_t l_u8block_tx_buffers[MAX_SPI_EMSC_BLOCK_SIZE*NUM_BLOCK_QUEUE_REQUESTS];
	struct sii8630_platform_data stdata;

	struct sii8630_platform_data *pdata = &stdata;
		//kzalloc(sizeof(struct sii8630_platform_data), GFP_KERNEL);

    memset(l_spi_mem,0,sizeof(struct spi_xfer_mem));
    memset(l_TxBuf,0,MAX_SPI_XFER_BUFFER_SIZE);
    memset(l_RxBuf,0,MAX_SPI_XFER_BUFFER_SIZE);
    memset(stdata,0,sizeof(struct sii8630_platform_data));

	if (NULL == pdata) {
		ret = -ENOMEM;
		goto failed;
	}

	spi->dev.platform_data = pdata;
	spi->bits_per_word = 8;

	pdata->spi_dev = spi;
	pdata->spi_bus_num = spi->master->bus_num;

	/*
	 * Modify default driver platform parameters with those
	 * specified in the device tree.
	 */
	if (spi->dev.of_node) {
		ret = si_drv_8630_parse_dt(&spi->dev, pdata);
		if (ret)
			goto failed;
	}

	/*
	 * Because we're using SPI for register access, we need to
	 * obtain separate i2c access for the I/O expander, so we
	 * use the I2C adapter number we got from the device tree.
	 */
	i2c_bus_adapter = i2c_get_adapter(pdata->i2c_adapter_num);
	if (i2c_bus_adapter == NULL) {
		pr_err("%s( failed to get i2c adapter %d\n", __func__,
			pdata->i2c_adapter_num);
		ret = -EFAULT;
		goto failed;
	}

	if (!i2c_check_functionality(i2c_bus_adapter,
		I2C_FUNC_SMBUS_BYTE_DATA)) {
		pr_debug("[ERROR] i2c function check failed\n");
		ret = -EIO;
		goto failed;
	}

	pdata->spi_mem = &l_spi_mem;//kzalloc(sizeof(struct spi_xfer_mem), GFP_KERNEL);
	pdata->spi_mem->tx_buf = &l_TxBuf;//kzalloc(MAX_SPI_XFER_BUFFER_SIZE, GFP_KERNEL);
	pdata->spi_mem->rx_buf = &l_RxBuf;//kzalloc(MAX_SPI_XFER_BUFFER_SIZE, GFP_KERNEL);
	pdata->spi_mem->block_tx_buffers = &l_u8block_tx_buffers;//kzalloc(MAX_SPI_EMSC_BLOCK_SIZE *
		//NUM_BLOCK_QUEUE_REQUESTS, GFP_KERNEL);
	if (!pdata->spi_mem || !pdata->spi_mem->tx_buf ||
		!pdata->spi_mem->rx_buf || !pdata->spi_mem->block_tx_buffers) {
		ret = -ENOMEM;
		goto mem_cleanup;
	}
	pdata->i2c_bus = i2c_bus_adapter;
	ret = starter_kit_init(pdata);
	if (ret >= 0) {
		ret = sii_drv_init((struct device *)&pdata->spi_dev->dev);
		if (ret < 0) {
			pr_debug("Driver create failed, error code %d\n", ret);
			gpio_free_array(pdata->sk_gpios,
				ARRAY_SIZE(pdata->sk_gpios));
			goto mem_cleanup;
		} else
			goto done;
	}

mem_cleanup:
	if (pdata->spi_mem) {
		//kfree(pdata->spi_mem->block_tx_buffers);
		pdata->spi_mem->block_tx_buffers = NULL;

		//kfree(pdata->spi_mem->rx_buf);
		pdata->spi_mem->rx_buf = NULL;

		//kfree(pdata->spi_mem->tx_buf);
		pdata->spi_mem->tx_buf = NULL;

		//kfree(pdata->spi_mem);
		pdata->spi_mem = NULL;
	}
failed:
	//kfree(pdata);
	pdata = NULL;

	probe_fail = true;
done:
	return ret;
}

#if (CONFIG_SOURCE_ONLY == 0)
struct sii9396_platform_data *ex_sp_pdata;

static int si_drv_9396_i2c_probe(struct i2c_client *client,
		const struct i2c_device_id *id)
{
	int ret;
	struct i2c_adapter *i2c_bus_adapter;
	struct sii9396_platform_data l_stdata;
	struct sii9396_platform_data *pdata = &l_stdata;//NULL;
    //pr_err(" si_drv_9396_i2c_probe !!\n");
	i2c_bus_adapter = to_i2c_adapter(client->dev.parent);

	if (!i2c_bus_adapter ||
		!i2c_check_functionality(i2c_bus_adapter,
			I2C_FUNC_SMBUS_BYTE_DATA)) {
		pr_err(" [ERROR] i2c function check failed\n");
		ret = -EIO;
		goto done;
	}

	//pdata = kzalloc(sizeof(struct sii9396_platform_data), GFP_KERNEL);
    memset(l_stdata,0,sizeof(struct sii9396_platform_data));
	if (NULL == pdata) {
		ret = -ENOMEM;
   		pr_err(" [ERROR] ENOMEM \n");
		goto done;
	}
	client->dev.platform_data = pdata;

	pdata->i2c_bus = i2c_bus_adapter;
	pdata->i2c_dev_client = client;
	pdata->i2c_device_id = SII_DRV_SPIDERMAN_I2C_ADDR;

	/*
	* Modify default driver platform parameters with those
	* specified in the device tree.
	*/
  	i2c_set_clientdata(client,pdata);
  	ret = spiderman_init(pdata);
  	if(ret){
     	pr_err("'spiderman_init error\n");
    	goto done;
   }

  ex_sp_pdata = pdata;

done:
	if (ret < 0) {
		//kfree(pdata);
		pdata = NULL;
	}
    else{
   		//pr_err(" 9396 schedule_work call..... \n");
    }
	return ret;
}
#endif // (CONFIG_SOURCE_ONLY == 0)


static int si_drv_8630_i2c_probe(struct i2c_client *client,
		const struct i2c_device_id *id)
{
	int ret;
	struct i2c_adapter *i2c_bus_adapter;
	struct sii8630_platform_data l_stdata;
	struct sii8630_platform_data *pdata = &l_stdata;

	i2c_bus_adapter = to_i2c_adapter(client->dev.parent);

	if (!i2c_bus_adapter ||
		!i2c_check_functionality(i2c_bus_adapter,
			I2C_FUNC_SMBUS_BYTE_DATA)) {
		pr_debug("[ERROR] i2c function check failed\n");
		ret = -EIO;
		goto done;
	}

	//pdata = kzalloc(sizeof(struct sii8630_platform_data), GFP_KERNEL);
    memset(l_stdata,0,sizeof(struct sii8630_platform_data));

	if (NULL == pdata) {
		ret = -ENOMEM;
		goto done;
	}
	client->dev.platform_data = pdata;

	pdata->i2c_bus = i2c_bus_adapter;
	pdata->i2c_dev_client = client;
	pdata->i2c_device_id = SII_DRV_DEVICE_I2C_ADDR;
	/*
	* Modify default driver platform parameters with those
	* specified in the device tree.
	*/
	if (client->dev.of_node) {
		ret = si_drv_8630_parse_dt(&client->dev, pdata);
		if (ret)
			goto done;
	}

	ret = starter_kit_init(pdata);
	if (ret >= 0) {
		ret = sii_drv_init(&client->dev);
		if (ret < 0) {
			pr_debug("Driver create failed, error code %d\n", ret);
			gpio_free_array(pdata->sk_gpios,
				ARRAY_SIZE(pdata->sk_gpios));
		}
	}
done:
	if (ret < 0) {
		//kfree(pdata);
		pdata = NULL;
		probe_fail = true;
	}
	return ret;
}

int si_8630_pm_suspend(struct device *dev)
{
	int status = 1;

	if (dev == 0)
		goto done;

	status = sii_drv_handle_power_change_request(dev, false);
done:
	return status;
}

int si_8630_pm_resume(struct device *dev)
{
	int status = 1;

	if (dev == 0)
		goto done;

	status = sii_drv_handle_power_change_request(dev, true);
done:
	return status;
}

#if (CONFIG_SOURCE_ONLY == 0)
int si_9636_pm_suspend(struct device *dev)
{
	int status = 0;
	return status;
}

int si_9636_pm_resume(struct device *dev)
{
	int status = 0;
	return status;
}
#endif

static const struct dev_pm_ops si_drv_8630_tx_pm_ops = {
	.runtime_suspend = si_8630_pm_suspend,
	.runtime_resume = si_8630_pm_resume,
};

#if (CONFIG_SOURCE_ONLY == 0)
static const struct dev_pm_ops si_drv_9636_tx_pm_ops = {
	.runtime_suspend = si_9636_pm_suspend,
	.runtime_resume = si_9636_pm_resume,
};
#endif

static struct of_device_id si_drv_8630_of_match_table[] = {
	{
		.compatible = "simg,sii-8630",
	},
	{},
};

#if (CONFIG_SOURCE_ONLY == 0)
static struct of_device_id si_drv_9396_of_match_table[] = {
	{
		.compatible = "simg,sii-9396",
	},
	{},
};
#endif

static const struct i2c_device_id si_drv_8630_id[] = {
	{SII_DEVICE_NAME, 0},
	{}
};

#if (CONFIG_SOURCE_ONLY == 0)
static const struct i2c_device_id si_drv_9396_id[] = {
	{"spiderman_host", 0},
	{}
};
#endif
static int si_drv_8630_spi_remove(struct spi_device *spi_dev)
{
	struct sii8630_platform_data *pdata;
	pdata = spi_dev->dev.platform_data;
	sii_drv_exit(&spi_dev->dev);

	kfree(pdata->spi_mem->tx_buf);
	kfree(pdata->spi_mem->rx_buf);
	kfree(pdata->spi_mem->block_tx_buffers);
	kfree(pdata->spi_mem);

	gpio_free_array(pdata->sk_gpios,
		ARRAY_SIZE(pdata->sk_gpios));
	if (pdata->lvs2) {
		regulator_disable(pdata->lvs2);
		devm_regulator_put(pdata->lvs2);
	}
	kfree(pdata);
	pdata = NULL;
	return 0;
}

static int si_drv_8630_i2c_remove(struct i2c_client *client)
{
	struct sii8630_platform_data *pdata;
	pdata = client->dev.platform_data;
	sii_drv_exit(&client->dev);

	gpio_free_array(pdata->sk_gpios,
		ARRAY_SIZE(pdata->sk_gpios));
	if (pdata->lvs2) {
		regulator_disable(pdata->lvs2);
		devm_regulator_put(pdata->lvs2);
	}

	kfree(pdata);
	pdata = NULL;
	return 0;
}

#if (CONFIG_SOURCE_ONLY == 0)
static int si_drv_9636_i2c_remove(struct i2c_client *client)
{
	struct sii9396_platform_data *pdata;
	pdata = client->dev.platform_data;
	kfree(pdata);
	pdata = NULL;
	return 0;
}
#endif

static struct i2c_driver si_drv_8630_i2c = {
	.driver = {
		.owner = THIS_MODULE,
		.name = SII_DEVICE_NAME,
		.of_match_table = si_drv_8630_of_match_table,
		.pm = &si_drv_8630_tx_pm_ops,
	},
	.id_table = si_drv_8630_id,
	.probe = si_drv_8630_i2c_probe,
	.remove = si_drv_8630_i2c_remove,
	.command = NULL,
};

#if (CONFIG_SOURCE_ONLY == 0)
static struct i2c_driver si_drv_9396_i2c = {
	.driver = {
		.owner = THIS_MODULE,
		.name = "spiderman_host",
		.of_match_table = si_drv_9396_of_match_table,
		.pm = &si_drv_9636_tx_pm_ops,
	},
	.id_table = si_drv_9396_id,
	.probe = si_drv_9396_i2c_probe,
	.remove = si_drv_9636_i2c_remove,
	.command = NULL,
};
#endif

static struct spi_driver si_86x0_mhl_tx_spi_driver = {
	.driver = {
		.owner = THIS_MODULE,
		.name = SII_DEVICE_NAME,
		.of_match_table = si_drv_8630_of_match_table,
		.pm = &si_drv_8630_tx_pm_ops,
	},
	.probe = si_drv_8630_spi_probe,
	.remove = si_drv_8630_spi_remove,
};

static int __init spi_init(void)
{
	int status;

	status = spi_register_driver(&si_86x0_mhl_tx_spi_driver);
	if (status < 0) {
		pr_err("[ERROR] %s(:%d failed !\n", __func__, __LINE__);
		goto done;
	}

	if (probe_fail || status < 0) {
		spi_unregister_driver(&si_86x0_mhl_tx_spi_driver);
		status = -ENODEV;
	}
done:
	return status;
}

#if (CONFIG_SOURCE_ONLY == 0)
static int __init spiderman_drv_init(void)
{
  int ret=0;
	ret = i2c_add_driver(&si_drv_9396_i2c);
	if (ret) {
			i2c_del_driver(&si_drv_9396_i2c);
		pr_err(" si_drv_9396_i2c failed !\n\nCHECK POWER AND CONNECTION ");
		pr_err("TO si_drv_9396_i2c  Starter Kit.\n\n");
		goto err_exit;
	}
  err_exit:
  	return ret;
}
#endif

static int __init i2c_init(void)
{
	int ret = -ENODEV;

	ret = i2c_add_driver(&si_drv_8630_i2c);
	if (ret < 0 || probe_fail) {
		if (ret == 0)
			i2c_del_driver(&si_drv_8630_i2c);
		pr_debug("failed !\n\nCHECK POWER AND CONNECTION ");
		pr_debug("TO CP8630 Starter Kit.\n\n");
		goto err_exit;
	}

	goto done;

err_exit:
done:
	if (probe_fail)
		ret = -ENODEV;

	return ret;
}

static int __init si_drv_8630_init(void)
{
	int ret;

	pr_info("cp8630_init: Starting SiI%d Driver v%s\n", SII_PRODUCT_NUM,
		buildVersion);
	pr_info("cp8630_init: %s\n", buildTime);

	/*
	 * NOTE: Even if the user selects to communicate with the MHL
	 * transmitter via SPI we still need I2C to communicate with
	 * other devices on the starter kit board.
	 */
#if (CONFIG_SOURCE_ONLY == 0)
     ret = spiderman_drv_init();
#endif
	if (use_spi)
		ret = spi_init();
	else
		ret = i2c_init();

	return ret;
}

static void __exit si_drv_8630_exit(void)
{
	if (use_spi) {
		/*sii_drv_exit(&spi_dev->dev);*/
		spi_unregister_driver(&si_86x0_mhl_tx_spi_driver);
	} else {
		/*sii_drv_exit(&i2c_dev_client->dev);*/
		i2c_del_driver(&si_drv_8630_i2c);
#if (CONFIG_SOURCE_ONLY == 0)
   		i2c_del_driver(&si_drv_9396_i2c);
#endif
	}
}
#endif
//module_init(si_drv_8630_init);
//module_exit(si_drv_8630_exit);


//MODULE_DESCRIPTION("Silicon Image cp8630 driver");
//MODULE_AUTHOR("Silicon Image <http://www.siliconimage.com>");
//MODULE_LICENSE("GPL");

#endif  // #if (_ENABLE_CHIP_SIL9630 == _MACRO_ENABLE)
/** END of file *********************************************************/

