/*
  2016 (C) Lattice Semiconductor Corporation

  No part of this work may be reproduced, modified, distributed, transmitted,
  transcribed, or translated into any language or computer format, in any form
  or by any means without written permission of Lattice Semiconductor, Inc.,
  111 SW 5th Ave, Suite 700, Portland, OR 97204.

 */
//#include "aardvark.h"
#include "Sii9396_hal.h"
#include "Sii9396_osal.h"
#include "Sii9396_device.h"

#ifdef USE_I2C

#define MAX_WAIT_FOR_DEV_SEMAPHORE	500	// 500 ms

#if 0
struct thread_info
{
	bool exit_flag; 		// thread exit flag, for safely exit
	HANDLE  h_obj;			//thread handle
	HANDLE  exit_event;		//thread exit event
	LPTHREAD_START_ROUTINE handler;
};
#endif

extern struct sii_device sii_dev[SII_DEV_COUNT_MAX];

enum sii_status sii_hal_platform_init(uint8_t *dev_num,
	sii_hwid_t* hwid, uint32_t *dev_id)
{
	*dev_num=*dev_num;		//∑¿÷π±‡“Î∆˜æØ∏Ê
	*hwid=*hwid;
	*dev_id=*dev_id;
	#if 0
	uint8_t i, j;
	uint16_t device[SII_DEV_COUNT_MAX];
	uint32_t unique_ids[SII_DEV_COUNT_MAX];
	int num = aa_find_devices_ext(SII_DEV_COUNT_MAX, device,
									SII_DEV_COUNT_MAX, unique_ids);

	for (i = 0, j = 0; i < num; i++) {
		if (device[i] & AA_PORT_NOT_FREE) {
			continue;
		}
		else {
			hwid[j] = (sii_hwid_t)device[i];
			dev_id[j] = unique_ids[i];
			j++;
		}
	}
	*dev_num = j;
	if(*dev_num <= 0) {
		err("no free aardvark found\n");
		return SII_STATUS_ERR_FAILED;
	}
	#endif

	return SII_STATUS_SUCCESS;
}

void sii_hal_platform_term(void)
{
	uint8_t i;
	for (i = 0; i < SII_DEV_COUNT_MAX; i++) {
		if (sii_dev[i].in_use)
			sii_hal_term(sii_dev[i].hwid);
	}
}

#if 0
DWORD WINAPI int_pin_irq(LPVOID lpParameter)
{
	struct sii_device *dev = (struct sii_device *)lpParameter;
	struct thread_info *irq_param = &dev->isr;
	uint8_t int_pin_status;

	while(!irq_param->exit_flag) {
		sii_os_sleep_msec(5);
		if (dev->isr_enable) {
			sii_hal_int_pin_status(dev->hwid, &int_pin_status);
			if(!int_pin_status) {
				sii_hal_isr(dev->hwid);
			}
		}
	}
	SetEvent(irq_param->exit_event);

	return 0;
}
#endif

enum sii_status sii_hal_init(sii_hwid_t hwid)
{
	enum sii_status ret_status = SII_STATUS_SUCCESS;
	struct sii_device *dev = &sii_dev[hwid];
	int port_number = hwid;

	#if 0
	if (dev->in_use) {
		err("Device in use\n");
		ret_status = SII_STATUS_ERR_FAILED;
		goto exit;
	}
	#endif

	if (SII_OS_STATUS_SUCCESS != sii_os_semaphore_create("dev_semaphore", 1, 1,
								&(dev->sem))) {
		err("Unable to create device semaphore\n");
		ret_status = SII_STATUS_ERR_FAILED;
		goto exit;
	}

	#if 0
	dev->i2cdev = aa_open(port_number);

	if(dev->i2cdev < 0) {
		err("can't open aardvark\n");
		ret_status = SII_STATUS_ERR_FAILED;
		goto exit;
	}

	aa_configure(dev->i2cdev, AA_CONFIG_GPIO_I2C);
	aa_i2c_pullup(dev->i2cdev, AA_I2C_PULLUP_BOTH);
	aa_i2c_bitrate(dev->i2cdev, 400);
	#endif

	dev->pin_reset =0; 		//AA_GPIO_SS;
	dev->pin_int = 0;			//AA_GPIO_MOSI;
	dev->slave_addr = 0x60;
	//aa_gpio_direction(dev->i2cdev, dev->pin_reset);
	//aa_gpio_pullup(dev->i2cdev, dev->pin_int);

	dev->hwid = hwid;
	dev->isr_enable = false;

	#if 0
	//Create isr thread
	dev->isr.exit_event = CreateEvent(NULL, TRUE, FALSE, NULL);
	dev->isr.exit_flag = false;
	dev->isr.handler = int_pin_irq;
	dev->isr.h_obj = CreateThread(NULL, 0, dev->isr.handler, dev, 0, NULL);
	#endif

	dev->in_use = true;
exit:
	return ret_status;
}

enum sii_status sii_hal_reset(sii_hwid_t hwid, bool reset)
{
	hwid=hwid;
	reset=reset;
	#if 0
	struct sii_device *dev = &sii_dev[hwid];
	dev->in_reset = reset;
	sii_os_semaphore_take(dev->sem, MAX_WAIT_FOR_DEV_SEMAPHORE);
	aa_gpio_set(dev->i2cdev, reset ? 0 : 0xff);
	sii_os_semaphore_give(dev->sem);
	#endif
	return SII_STATUS_SUCCESS;
}

enum sii_status sii_hal_int_pin_status(sii_hwid_t hwid, bool *status)
{
	uint8_t int_pin_status;
	struct sii_device *dev = &sii_dev[hwid];

	#if 0
	sii_os_semaphore_take(dev->sem, MAX_WAIT_FOR_DEV_SEMAPHORE);
	int_pin_status = (uint8_t) aa_gpio_get(dev->i2cdev);
	sii_os_semaphore_give(dev->sem);
	#endif

	//*status = (int_pin_status & dev->pin_int) ? true : false;
	*status = true ;
	return SII_STATUS_SUCCESS;
}

enum sii_status sii_hal_isr_enable(sii_hwid_t hwid, bool enable)
{
	struct sii_device *dev = &sii_dev[hwid];
	dev->isr_enable = enable;
	return SII_STATUS_SUCCESS;
}

void sii_hal_isr_done(sii_hwid_t hwid)
{
	struct sii_device *dev = &sii_dev[hwid];
}

void sii_hal_isr(sii_hwid_t hwid)
{
	sii_device_isr(hwid);
	sii_hal_isr_done(hwid);
}

void sii_hal_term(sii_hwid_t hwid)
{
	sii_hal_isr(hwid);
	#if 0
	struct sii_device *dev = &sii_dev[hwid];
	dev->isr.exit_flag = true;
	//Wait isr thread end
	if (WAIT_TIMEOUT == WaitForSingleObject(dev->isr.exit_event, 3000)) {
		err("thread can't exit");
	}
	aa_close(dev->i2cdev);
	if (NULL != dev->sem) {
		if (SII_OS_STATUS_SUCCESS !=
				sii_os_semaphore_delete(dev->sem))
			err("Failed to delete device semaphore\n");
		dev->sem = NULL;
	}
	memset(dev, 0, sizeof(struct sii_device));
	#endif
}

enum sii_status sii_hal_op(sii_hwid_t hwid, uint8_t *pTxData,
	uint16_t Txsize, uint8_t *pRxData, uint16_t Rxsize)
{
	enum sii_status ret_status = SII_STATUS_SUCCESS;

	#if 0
	enum sii_os_status sii_os_rv = SII_OS_STATUS_SUCCESS;
	struct sii_device *dev = &sii_dev[hwid];
	uint16_t num_written = 0;
	uint16_t num_read = 0;

	int i = 0;
	if (NULL != pTxData) {
		dbg("data out = \n");
		for (i=0; i<Txsize; i++) {
			dbg("0x%02X\n", pTxData[i]);
		}
	}

	if (dev->i2cdev < 0) {
		err("I2C not initialized\n");
		ret_status = SII_STATUS_ERR_FAILED;
		goto done;
	}

	if (dev->in_reset) {
		dbg("I2C Op does not work in_reset\n");
		ret_status = SII_STATUS_ERR_FAILED;
		goto done;
	}

	if (NULL == pTxData || 0 == Txsize) {
		err("no input I2C data\n");
		ret_status = SII_STATUS_ERR_FAILED;
		goto done;
	}

	if (pRxData == NULL)
		Rxsize = 0;

	sii_os_semaphore_take(dev->sem, MAX_WAIT_FOR_DEV_SEMAPHORE);

	if (Rxsize == 0) {
		num_written = aa_i2c_write(dev->i2cdev,
			dev->slave_addr>>1,
			AA_I2C_NO_FLAGS, Txsize, pTxData);
	}else {
		aa_i2c_write_read(dev->i2cdev,
			dev->slave_addr>>1,
			AA_I2C_NO_FLAGS, Txsize, pTxData, &num_written,
			Rxsize, pRxData, &num_read);
	}

	if (num_written < Txsize || num_read < Rxsize) {
		err("sii9396_spi_write_read failed\n");
		ret_status = SII_STATUS_ERR_FAILED;
		goto done_give_sem;
	}

	if (NULL != pRxData) {
		dbg("data in  = \n");
		for (i=0; i<Rxsize; i++) {
			dbg("0x%02X\n", pRxData[i]);
		}
	}

done_give_sem:
	sii_os_semaphore_give(dev->sem);

done:
	#endif
	return ret_status;
}



static char *i2c_status[] =
{
	"OK",
	"BUS_ERROR",
	"SLA_ACK",
	"SLA_NACK",
	"DATA_NACK",
	"ARB_LOST",
	"BUS_LOCKED",
	"LAST_DATA_ACK"
};

static char* i2c_status_string(int status)
{
	char *status_string = "Unknown";

	if (status >= 0 && status < 8)
	{
		status_string = i2c_status[status];
	}

	return status_string;
}

#endif
