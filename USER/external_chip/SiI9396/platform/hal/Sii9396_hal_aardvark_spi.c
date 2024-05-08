/*
  2016 (C) Lattice Semiconductor Corporation

  No part of this work may be reproduced, modified, distributed, transmitted,
  transcribed, or translated into any language or computer format, in any form
  or by any means without written permission of Lattice Semiconductor, Inc.,
  111 SW 5th Ave, Suite 700, Portland, OR 97204.

 */
#include "Sii9396_hal.h"
#include "Sii9396_osal.h"
#include "Sii9396_device.h"

#ifdef USE_SPI

#define MAX_WAIT_FOR_DEV_SEMAPHORE	500	// 500 ms

enum isr_pin_state_machine {
	WAIT_FOR_ACTIVE = 0,
	WAIT_FOR_INACTIVE
};

struct thread_info
{
	bool exit_flag; 		// thread exit flag, for safely exit
	HANDLE  h_obj;			//thread handle
	HANDLE  exit_event;		//thread exit event
	LPTHREAD_START_ROUTINE handler;
};

struct sii_device {
	sii_hwid_t hwid;
	bool in_use;
	bool in_reset;
	bool isr_enable;
	BYTE pin_reset;		//SCL
	BYTE pin_int;			//SDA
	Aardvark spidev;
	struct SiiOsSemaphore *sem;
	struct thread_info isr;
};

static struct sii_device sii_dev[SII_DEV_COUNT_MAX] = {0};

enum sii_status sii_hal_platform_init(uint8_t *dev_num,
	sii_hwid_t* hwid, uint32_t *dev_id)
{
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

DWORD WINAPI int_pin_irq(LPVOID lpParameter)
{
	struct sii_device *dev = (struct sii_device *)lpParameter;
	struct thread_info *irq_param = &dev->isr;
	bool int_pin_status;
//	enum isr_pin_state_machine isr_sm = WAIT_FOR_ACTIVE;
//	uint16_t int_pin_timeout = 0;
//	bool isr_flag = false;

	while(!irq_param->exit_flag) {
		sii_os_sleep_msec(5);
		if (dev->isr_enable) {
			sii_hal_int_pin_status(dev->hwid, &int_pin_status);
	/*
			switch(isr_sm) {
				case WAIT_FOR_ACTIVE:
					if (!int_pin_status) {
						isr_flag = true;
						isr_sm = WAIT_FOR_INACTIVE;
					}
					int_pin_timeout = 0;
					break;
				case WAIT_FOR_INACTIVE:
					if (int_pin_status)
						isr_sm = WAIT_FOR_ACTIVE;
					if (++int_pin_timeout > 120)
						isr_sm = WAIT_FOR_ACTIVE;
					break;
			}
	*/
			if(!int_pin_status) {
				sii_hal_isr(dev->hwid);
	//			isr_flag = false;
			}
		}
	}
	SetEvent(irq_param->exit_event);

	return 0;
}

enum sii_status sii_hal_init(sii_hwid_t hwid)
{
	enum sii_status ret_status = SII_STATUS_SUCCESS;
	struct sii_device *dev = &sii_dev[hwid];
	int port_number = hwid;
	if (dev->in_use) {
		err("Device in use\n");
		ret_status = SII_STATUS_ERR_FAILED;
		goto exit;
	}

	if (SII_OS_STATUS_SUCCESS != sii_os_semaphore_create("dev_semaphore", 1, 1,
								&(dev->sem))) {
		err("Unable to create device semaphore\n");
		ret_status = SII_STATUS_ERR_FAILED;
		goto exit;
	}

	dev->spidev = aa_open(port_number);

	if(dev->spidev < 0) {
		err("can't open aardvark\n");
		ret_status = SII_STATUS_ERR_FAILED;
		goto exit;
	}

	aa_configure(dev->spidev, AA_CONFIG_SPI_GPIO);
	aa_spi_configure(dev->spidev,
					AA_SPI_POL_FALLING_RISING,
					AA_SPI_PHASE_SETUP_SAMPLE,
					AA_SPI_BITORDER_MSB);
	aa_spi_bitrate(dev->spidev, 4000);
	aa_spi_master_ss_polarity(dev->spidev, AA_SPI_SS_ACTIVE_LOW);

	dev->pin_reset = AA_GPIO_SCL;
	dev->pin_int = AA_GPIO_SDA;
	aa_gpio_direction(dev->spidev, dev->pin_reset);
	aa_gpio_pullup(dev->spidev, dev->pin_int);

	dev->hwid = hwid;
	dev->isr_enable = false;
	//Create isr thread
	dev->isr.exit_event = CreateEvent(NULL, TRUE, FALSE, NULL);
	dev->isr.exit_flag = false;
	dev->isr.handler = int_pin_irq;
	dev->isr.h_obj = CreateThread(NULL, 0, dev->isr.handler, dev, 0, NULL);
	dev->in_use = true;
exit:
	return ret_status;
}

void sii_hal_term(sii_hwid_t hwid)
{
	struct sii_device *dev = &sii_dev[hwid];
	dev->isr.exit_flag = true;
	//Wait isr thread end
	if (WAIT_TIMEOUT == WaitForSingleObject(dev->isr.exit_event, 3000)) {
		err("thread can't exit");
	}
	aa_close(dev->spidev);
	if (NULL != dev->sem) {
		if (SII_OS_STATUS_SUCCESS !=
				sii_os_semaphore_delete(dev->sem))
			err("Failed to delete device semaphore\n");
		dev->sem = NULL;
	}
	memset(dev, 0, sizeof(struct sii_device));
}

enum sii_status sii_hal_reset(sii_hwid_t hwid, bool reset)
{
	struct sii_device *dev = &sii_dev[hwid];
	dev->in_reset = reset;
	sii_os_semaphore_take(dev->sem, MAX_WAIT_FOR_DEV_SEMAPHORE);
	aa_gpio_set(dev->spidev, reset ? 0 : 0xff);
	sii_os_semaphore_give(dev->sem);
	return SII_STATUS_SUCCESS;
}

enum sii_status sii_hal_int_pin_status(sii_hwid_t hwid, bool *status)
{
	uint8_t int_pin_status;
	struct sii_device *dev = &sii_dev[hwid];
	sii_os_semaphore_take(dev->sem, MAX_WAIT_FOR_DEV_SEMAPHORE);
	int_pin_status = (uint8_t) aa_gpio_get(dev->spidev);
	sii_os_semaphore_give(dev->sem);
	*status = (int_pin_status & dev->pin_int) ? true : false;
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

enum sii_status sii_hal_op(sii_hwid_t hwid, uint8_t *pTxData,
	uint16_t Txsize, uint8_t *pRxData, uint16_t Rxsize)
{
	enum sii_status ret_status = SII_STATUS_SUCCESS;
	enum sii_os_status sii_os_rv = SII_OS_STATUS_SUCCESS;
	struct sii_device *dev = &sii_dev[hwid];
	int rv = 0;

	int i = 0;
	if (NULL != pTxData) {
		dbg("data out = \n");
		for (i=0; i<Txsize; i++) {
			dbg("0x%02X\n", pTxData[i]);
		}
	}

	if (dev->spidev < 0) {
		err("SPI not initialized\n");
		ret_status = SII_STATUS_ERR_FAILED;
		goto done;
	}

	if (dev->in_reset) {
		dbg("SPI Op does not work in_reset\n");
		ret_status = SII_STATUS_ERR_FAILED;
		goto done;
	}

	if (NULL == pTxData || 0 == Txsize) {
		err("no input SPI data\n");
		ret_status = SII_STATUS_ERR_FAILED;
		goto done;
	}

	sii_os_semaphore_take(dev->sem, MAX_WAIT_FOR_DEV_SEMAPHORE);

	rv = aa_spi_write(dev->spidev,
				Txsize, pTxData,
				Rxsize, pRxData);

	if (rv < Rxsize) {
		err("sii9396_spi_write_read failed: %d\n", rv);
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
	return ret_status;
}

#endif
