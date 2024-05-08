/*
* SiI8630/SiI8632/SiI9630 Linux Driver
*
* 2016 © Lattice Semiconductor Corporation
*
* This program is free software; you can redistribute it and/or
* modify it only under the terms of  version 2 of the GNU General Public License
* as published by the Free Software Foundation, and not any later version.
*
* This program is distributed “AS ISEWITHOUT ANY WARRANTY of any kind,
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

#include "Sii9630_si_drv_cp8630.h"
#if (CONFIG_SOURCE_ONLY == 0)
#include "Sii9630_spiderman.h"
#include "Sii9630_si_platform.h"
#include "Sii9630_si_lib_log.h"
#include "Sii9630_si_mod_tx_hdcp.h"
#include "Sii9630_si_timer.h"

/*If customer using the following setup they will encounter TV output black screen issue
HDCP 1.4 source ==> SA===> Banner ==>Marantz NR1506(HDCP 2.2 AVR) ==> Samsung 6300 UHD TV
the root cause is Banner source code did not do enough for convert HDCP 1.4 to HDCP 2.2 like spiderman B*/
//#define DEBUG_BANNER_HDCP_CONVERTER_ISSUE			1


/*
   IPC Internal Structure & Context for Host Side
 */

#define __REMOVE_UNNECESSARY_FUNC__

struct sii_ipc_host_context ipc_host_context[SII_INST_COUNT_MAX];
static struct sii_device sii_dev[SII_DEV_COUNT_MAX];
static struct sii_ipc_notification ntf_exec_list[SII_IPC_NOTIFICATION_QUEUE_LENGTH];
static uint8_t ntf_exec_cnt = 0;

static struct sii_rx_hdcp1_data rx_hdcp1_data;
static struct sii_rx_hdcp2_data rx_hdcp2_data;
static struct sii_tx_hdcp2_data tx_hdcp2_data;
static struct sii9396_platform_data sg_stSii9396Platform;
struct sii9396_platform_data *ex_sp_pdata = NULL;

struct sii_api_host_instance {
    bool in_use;
    uint32_t event_mask;
    void (*event_handler)(sii_inst_t inst, void* p_obj, uint8_t event);
};

struct sii_api_host_context {
    uint8_t instance_count;
    struct sii_api_host_instance instance[SII_INST_COUNT_MAX];
};

struct sii_api_host_context api_host_context = { 0 };
static pstdriver_obj_t l_drv_context;
#ifndef __REMOVE_UNNECESSARY_FUNC__
static struct sii_video_info video_info;
#endif

static u8 sg_u8Sil9396Dev[1]=
{
    _I2C_TABLE_INDEX_Sil9396,
};


/*
static uint8_t sink_edid[] = {
    0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00,
    0x4D, 0xD9, 0x03, 0xC8, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x19, 0x01, 0x03, 0x80, 0x90, 0x51, 0x78,
    0x0A, 0x0D, 0xC9, 0xA0, 0x57, 0x47, 0x98, 0x27,
    0x12, 0x48, 0x4C, 0x21, 0x08, 0x00, 0x81, 0x80,
    0xA9, 0xC0, 0x71, 0x4F, 0xB3, 0x00, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x3A,
    0x80, 0x18, 0x71, 0x38, 0x2D, 0x40, 0x58, 0x2C,
    0x45, 0x00, 0x9F, 0x29, 0x53, 0x00, 0x00, 0x1E,
    0x01, 0x1D, 0x00, 0x72, 0x51, 0xD0, 0x1E, 0x20,
    0x6E, 0x28, 0x55, 0x00, 0x9F, 0x29, 0x53, 0x00,
    0x00, 0x1E, 0x00, 0x00, 0x00, 0xFC, 0x00, 0x53,
    0x4F, 0x4E, 0x59, 0x20, 0x54, 0x56, 0x20, 0x20,
    0x2A, 0x30, 0x32, 0x0A, 0x00, 0x00, 0x00, 0xFD,
    0x00, 0x30, 0x3E, 0x0E, 0x46, 0x3C, 0x00, 0x0A,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x03, 0xDD,

    0xF0, 0x02, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C,

    0x02, 0x03, 0x60, 0xF0, 0x5B, 0x61, 0x60, 0x5D,
    0x5E, 0x5F, 0x62, 0x1F, 0x10, 0x14, 0x05, 0x13,
    0x04, 0x20, 0x22, 0x3C, 0x3E, 0x12, 0x16, 0x03,
    0x07, 0x11, 0x15, 0x02, 0x06, 0x01, 0x65, 0x66,
    0x29, 0x0D, 0x7F, 0x07, 0x15, 0x07, 0x50, 0x3D,
    0x07, 0xBC, 0x83, 0x0F, 0x00, 0x00, 0x78, 0x03,
    0x0C, 0x00, 0x10, 0x00, 0xB8, 0x3C, 0x2F, 0xD0,
    0x8A, 0x01, 0x02, 0x03, 0x04, 0x01, 0x40, 0x1F,
    0xC0, 0x80, 0x90, 0xD0, 0xE0, 0xF0, 0xD6, 0x67,
    0xD8, 0x5D, 0xC4, 0x01, 0x78, 0x80, 0x01, 0xE2,
    0x00, 0xF9, 0xE3, 0x05, 0xFF, 0x01, 0xE5, 0x0F,
    0x03, 0x00, 0x00, 0x06, 0xE3, 0x06, 0x05, 0x01,
    0x01, 0x1D, 0x80, 0x18, 0x71, 0x1C, 0x16, 0x20,
    0x58, 0x2C, 0x25, 0x00, 0x9F, 0x29, 0x53, 0x00,
    0x00, 0x9E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x6A,

    0x02, 0x03, 0x04, 0xF0, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07,
};
*/
/*
static void s_dump_buffer(uint8_t *p_data, uint16_t len)
{
    int i = 0;

    sii_log_print("\n");
    while (len--) {
        sii_log_print(" %02X", *p_data);
        p_data++;
        if (++i == 0x10) {
            sii_log_print("\n");
            i = 0;
        }

    }
    sii_log_print("\n");
}
*/
uint32_t sii_get_milli_sec(void)
{
    //struct timeval time1;
    uint32_t new_time = sil_GetSystemTime();
    //do_gettimeofday(&time1);
    //new_time = (time1.tv_sec * 1000) + time1.tv_usec/1000;
    return new_time;
}

enum sii_api_status sii_set_event_mask(sii_inst_t inst, uint32_t mask)
{
    uint8_t index = (uint8_t) inst;
    api_host_context.instance[index].event_mask = mask;
    return SII_API_SUCCESS;
}

enum sii_api_status sii_get_event_mask(sii_inst_t inst, uint32_t *mask)
{
    uint8_t index = (uint8_t) inst;
    *mask = api_host_context.instance[index].event_mask;
    return SII_API_SUCCESS;
}


int sii_device_reg_read_block(
        sii_inst_t inst,
        uint8_t page_num,
        uint8_t offset,
        uint16_t len,
        uint8_t *data_block) {

    int ret=0;
    uint16_t addr =0;
    u8 l_u8state = 0;
    u8 l_u8Dev;

    addr = ((page_num<<8)+offset);
    l_u8Dev = sg_u8Sil9396Dev[inst];
    l_u8state = i2c_read_bytes(l_u8Dev,ex_sp_pdata->i2c_device_id,addr,data_block,len,TRUE);
    if(l_u8state == 0)
        ret = 0;
    else
        ret = -1;

    if (ret < 0)
    {
        pr_err("%s: 9396 I2C block read failed, addr: 0x%02X  ret = %d\n",
        __func__, addr, ret);
    }
    else
        ret = 0;

    return ret;

}

int sii_device_reg_write_block(
        sii_inst_t inst,
        uint8_t page_num,
        uint8_t offset,
        uint16_t len,
        uint8_t *data_block) {

    int ret=0;
    uint16_t addr =0;
    u8 l_u8state = 0;
    u8 l_u8Dev;

    addr = ((page_num<<8)+offset);
    l_u8Dev = sg_u8Sil9396Dev[inst];
    l_u8state = i2c_write_bytes(l_u8Dev,ex_sp_pdata->i2c_device_id,addr,data_block,len,TRUE);
    if(l_u8state == 0)
        ret = 0;
    else
        ret = -1;

    if (ret < 0)
    {
        pr_err("%s: 9396 I2C block writefailed, addr: 0x%02X  ret = %d\n",
            __func__, addr, ret);
    }
    else
        ret = 0;

    return ret;

}


int sii_device_reg_write_byte(
        sii_inst_t inst,
        uint8_t page_num,
        uint8_t offset,
        uint8_t data_byte) {
    return sii_device_reg_write_block(inst, page_num, offset, 1, &data_byte);
}

static enum sii_ipc_status sii_ipc_hal_host_write_byte(
        sii_inst_t inst, uint8_t offset, uint8_t value) {

    enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;
    enum sii_status sii_st = SII_STATUS_SUCCESS;

    sii_st = (enum sii_status)sii_device_reg_write_byte(
            inst, SII_PAGE_IPC_HOST, offset, value);

    if (sii_st != SII_STATUS_SUCCESS) {
        pr_err("HAL failed!");
        status = SII_IPC_STATUS_ERROR_HAL_DEVICE;
        goto exit;
    }

exit:
    return status;
}

static enum sii_ipc_status sii_ipc_hal_host_read_block(
        sii_inst_t inst, uint8_t offset,
        uint8_t *buffer, uint8_t length) {

    enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;
    enum sii_status sii_st = SII_STATUS_SUCCESS;

    sii_st = (enum sii_status)sii_device_reg_read_block(
            inst, SII_PAGE_IPC_HOST, offset, length, buffer);

    if (sii_st != SII_STATUS_SUCCESS) {
        pr_err("HAL failed!");
        status = SII_IPC_STATUS_ERROR_HAL_DEVICE;
        goto exit;
    }

exit:
    return status;
}

static enum sii_ipc_status sii_ipc_hal_host_write_block(
        sii_inst_t inst, uint8_t offset,
        uint8_t *buffer, uint8_t length) {

    enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;
    enum sii_status sii_st = SII_STATUS_SUCCESS;

    sii_st = (enum sii_status)sii_device_reg_write_block(
            inst, SII_PAGE_IPC_HOST, offset, length, buffer);

    if (sii_st != SII_STATUS_SUCCESS) {
        pr_err("HAL failed!");
        status = SII_IPC_STATUS_ERROR_HAL_DEVICE;
        goto exit;
    }

exit:
    return status;
}

enum sii_ipc_status sii_ipc_hal_write_host_buffer_command(
        sii_inst_t inst, uint8_t *buffer, uint8_t length) {

    enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;

    if (length > SII_IPC_HOST_BUF_CMD_LENGTH || 0 == length) {
        status = SII_IPC_STATUS_ERROR_HAL_EXCEED_BUFFER_LIMIT;
        goto exit;
    }

    status = sii_ipc_hal_host_write_block(
            inst, SII_REG_IPC_HOST_BUF_CMD_START, buffer, length);

    if (status != SII_IPC_STATUS_SUCCESS) {
        goto exit;
    }

exit:
    return status;
}


int sii_device_reg_read_byte(
        sii_inst_t inst,
        uint8_t page_num,
        uint8_t offset,
        uint8_t *data_byte) {
    return sii_device_reg_read_block(inst, page_num, offset, 1, data_byte);
}

static enum sii_ipc_status sii_ipc_hal_host_read_byte(
        sii_inst_t inst, uint8_t offset, uint8_t *value) {

    enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;
    enum sii_status sii_st = SII_STATUS_SUCCESS;

    sii_st = (enum sii_status)sii_device_reg_read_byte(
            inst, SII_PAGE_IPC_HOST, offset, value);

    if (sii_st != SII_STATUS_SUCCESS) {
        pr_err("9396 HAL failed!");
        status = SII_IPC_STATUS_ERROR_HAL_DEVICE;
        goto exit;
    }

exit:
    return status;
}

enum sii_ipc_status sii_ipc_hal_read_slave_status1(
        sii_inst_t inst, uint8_t *status1) {

    enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;

    if (!ipc_host_context[inst].slave_status1_cache) {
        status = sii_ipc_hal_host_read_byte(
                inst, SII_REG_IPC_SLAVE_STATUS1, status1);
        ipc_host_context[inst].slave_status1_reg = *status1;
        ipc_host_context[inst].slave_status1_cache = true;
    } else{
        *status1 = ipc_host_context[inst].slave_status1_reg;
   }

    if (status != SII_IPC_STATUS_SUCCESS) {
        goto exit;
    } else {
        *status1 &= SII_BIT_IPC_SLAVE_STATUS1_MASK;
    }

exit:
    return status;
}

enum sii_ipc_status sii_ipc_hal_read_slave_status2(
        sii_inst_t inst, uint8_t *status2) {

    enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;

    if (!ipc_host_context[inst].slave_status2_cache) {
        status = sii_ipc_hal_host_read_byte(
                inst, SII_REG_IPC_SLAVE_STATUS2, status2);
        ipc_host_context[inst].slave_status2_reg = *status2;
        ipc_host_context[inst].slave_status2_cache = true;
    } else{
        *status2 = ipc_host_context[inst].slave_status2_reg;
   }
    if (status != SII_IPC_STATUS_SUCCESS) {
        goto exit;
    } else {
        *status2 &= SII_BIT_IPC_SLAVE_STATUS2_MASK;
    }

exit:
    return status;
}


enum sii_ipc_status sii_ipc_hal_clear_slave_reg_cache(sii_inst_t inst)
{
    ipc_host_context[inst].slave_status1_cache = false;
    ipc_host_context[inst].slave_status2_cache = false;
    return SII_IPC_STATUS_SUCCESS;
}


enum sii_ipc_status sii_ipc_host_check_change(
        sii_inst_t inst, bool *new_reply, bool *new_notification) {
        enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;

    uint8_t status_register1 = 0x00;
    uint8_t status_register2 = 0x00;

    status = sii_ipc_hal_read_slave_status1(inst, &status_register1);
    if (status != SII_IPC_STATUS_SUCCESS) {
        goto exit;
    }

    status = sii_ipc_hal_read_slave_status2(inst, &status_register2);
    if (status != SII_IPC_STATUS_SUCCESS) {
        goto exit;
    }

    *new_reply = (SII_BIT_IPC_SLAVE_STATUS1_RPL_STATE_MASK
            & status_register1) ? true : false;

    *new_notification = (SII_BIT_IPC_SLAVE_STATUS2_NTF_STATE_MASK
            & status_register2) ? true : false;

        if (!(*new_reply) && !(*new_notification)) {
                pr_err("Unknown interrupt source!");
                status = SII_IPC_STATUS_ERROR_PROTOCOL;
                goto exit;
        }

exit:
        return status;
}

enum sii_ipc_status sii_ipc_hal_read_slave_buffer_notification(
        sii_inst_t inst, uint8_t *buffer, uint8_t length) {

    enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;

    if (length > SII_IPC_SLAVE_BUF_NTF_LENGTH || 0 == length) {
        status = SII_IPC_STATUS_ERROR_HAL_EXCEED_BUFFER_LIMIT;
        goto exit;
    }

    status = sii_ipc_hal_host_read_block(
            inst, SII_REG_IPC_SLAVE_BUF_NTF_START, buffer, length);

    if (status != SII_IPC_STATUS_SUCCESS) {
        goto exit;
    }

exit:
    return status;
}


bool verify_checksum_2bit(uint8_t value1, uint8_t value2) {
    uint8_t checksum = 0x00;

    checksum = (value1 & 0x03)
        + ((value1 >> 2) & 0x03)
        + ((value1 >> 4) & 0x03)
        + ((value1 >> 6) & 0x03)
               + (value2 & 0x03)
        + ((value2 >> 2) & 0x03)
        + ((value2 >> 4) & 0x03)
        + ((value2 >> 6) & 0x03);
    return (0 == (checksum & 0x03))? true : false;
}


enum sii_ipc_status sii_ipc_receive_buffer_notification(
        sii_inst_t inst, struct sii_ipc_packet_notification *packet_notification) {
    enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;

    uint8_t status_register1 = 0x00;
    uint8_t status_register2 = 0x00;
    bool verify_checksum = false;
    uint8_t buffer_status = 0x00;

    struct sii_ipc_packet_ntf *packet_ntf =
        packet_notification->packet;

    status = sii_ipc_hal_read_slave_status1(inst, &status_register1);
    if (status != SII_IPC_STATUS_SUCCESS) {
        goto exit;
    }

    status = sii_ipc_hal_read_slave_status2(inst, &status_register2);
    if (status != SII_IPC_STATUS_SUCCESS) {
        goto exit;
    }

    verify_checksum = verify_checksum_2bit(
            status_register1, status_register2);
    if (!verify_checksum) {
        status = SII_IPC_STATUS_ERROR_HAL_CHECKSUM;
        goto exit;
    }

    packet_notification->type = (status_register2
            & SII_BIT_IPC_SLAVE_STATUS2_NOTIFICATION_TYPE_MASK)
        >> SII_BIT_IPC_SLAVE_STATUS2_NOTIFICATION_TYPE_SHIFT;

    if (packet_notification->type !=
            SII_IPC_PHY_NOTIFICATION_TYPE_BUFFER_NOTIFICATION) {
        pr_err("Unsupported Notification Type");

        /* FD: TBU, extension for other Notification Types */
        /*
        switch(packet_notification->type) {
            case :
                break;
            default :
                break;
        }
        */
        goto exit;
    }

    buffer_status = (status_register1
            & SII_BIT_IPC_SLAVE_STATUS1_BUFFER_STATUS_MASK)
        >> SII_BIT_IPC_SLAVE_STATUS1_BUFFER_STATUS_SHIFT;
    if (buffer_status != SII_IPC_PHY_BUFFER_STATUS_NOTIFICATION) {
        status = SII_IPC_STATUS_ERROR_HAL_NO_NOTIFICATION_IN_BUFFER;
        goto exit;
    }

    memset(packet_ntf, 0, SII_IPC_PACKET_NTF_LENGTH);

    status = sii_ipc_hal_read_slave_buffer_notification(
            inst,
            (void *)(packet_ntf), SII_IPC_PACKET_NTF_LENGTH);
    if (status != SII_IPC_STATUS_SUCCESS) {
        goto exit;
    }

exit:
    return status;
}

uint8_t calculate_generic_checksum(
    uint8_t *bytes, uint8_t checksum, uint16_t length) {
    uint16_t i;

    for (i = 0; i < length; i++)
        checksum += bytes[i];

    checksum = 0x100 - checksum;

    return checksum;
}

bool verify_checksum_byte(uint8_t *bytes, uint16_t length) {
    return (0 == calculate_generic_checksum(bytes, 0, length))
        ? true : false;
}

enum sii_ipc_status sii_ipc_host_push_queue_notification(
        sii_inst_t inst, struct sii_ipc_notification notification) {
    enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;
    uint8_t i;

    // duplication check
    for( i=0; i<ntf_exec_cnt; i++ )
    {
        if( ntf_exec_list[i].notification_type == notification.notification_type )
        {
                sii_log_print( "[ntf] %02x dup2\n", (int)notification.notification_type);
                return SII_IPC_STATUS_SUCCESS;
        }
    }
    for( i=ipc_host_context[inst].notification_queue_head;
        i!=ipc_host_context[inst].notification_queue_tail;
        i=(i+1)%SII_IPC_NOTIFICATION_QUEUE_LENGTH )
            {
                if(ipc_host_context[inst].notification_queue[i].notification_type
                    == notification.notification_type ){
                    // Same notification exist, no need to duplicate
                    sii_log_print( "[ntf] %02x dup\n", (int)notification.notification_type);
                    return SII_IPC_STATUS_SUCCESS;
            }
    }

    if (ipc_host_context[inst].notification_queue_count
            >= SII_IPC_NOTIFICATION_QUEUE_LENGTH) {
        status = SII_IPC_STATUS_ERROR_NOTIFICATION_QUEUE_FULL;
        goto exit;
    }

    memcpy(&(ipc_host_context[inst].notification_queue
            [ipc_host_context[inst].notification_queue_tail]),
        &notification,
        sizeof(notification));

    ipc_host_context[inst].notification_queue_count++;

    ipc_host_context[inst].notification_queue_tail =
        (ipc_host_context[inst].notification_queue_tail + 1)
        % SII_IPC_NOTIFICATION_QUEUE_LENGTH;

exit:
    //status2 = sii_ipc_host_give_ntf_queue_semaphore(inst);
    //if (status2 != SII_IPC_STATUS_SUCCESS) {
    //    goto exit;
    //}

//exit_last:

    return status;
}

enum sii_ipc_status sii_ipc_extract_notification_from_packet_ntf(
        sii_inst_t inst, struct sii_ipc_packet_ntf *packet_ntf) {
    enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;

    bool verify_checksum = false;
    uint8_t ntf_count = 0;
    uint8_t ntf_index = 0;
    struct sii_ipc_notification notification;

    /* Check Protocol Version */
    if (SII_IPC_PACKET_NTF_PROTOCOL_VERSION_1
            != packet_ntf->protocol_version) {
        /* FD: TBR, support future protocol extension */
        status = SII_IPC_STATUS_ERROR_PROTOCOL_VERSION_UNSUPPORTED;
        goto exit;
    }
     if( packet_ntf->packet_length < SII_IPC_PACKET_NTF_HEADER_LENGTH )
     {
        status = SII_IPC_STATUS_ERROR_INVALID_NOTIFICATION;
        goto exit;
     }
    /* Verify checksum */
    verify_checksum = verify_checksum_byte((uint8_t *)(packet_ntf),
            packet_ntf->packet_length);
    if (!verify_checksum) {
        status = SII_IPC_STATUS_ERROR_PACKET_CHECKSUM;
        goto exit;
    }

    /*
       Extract Notifications, Push into Notification Queue
     */
        ntf_count = packet_ntf->packet_length
            - SII_IPC_PACKET_NTF_HEADER_LENGTH;

    for (; ntf_index < ntf_count; ntf_index++) {

        /* Extract Payload of Notification Packet */
        notification.notification_type =
            packet_ntf->packet_payload[ntf_index];

        /* Push into Notification Queue */
        status = sii_ipc_host_push_queue_notification(inst, notification);
        if (status != SII_IPC_STATUS_SUCCESS) {
            goto exit;
        }
    }

exit:
    return status;
}


enum sii_ipc_status sii_ipc_hal_set_slave_interrupt_mask(
        sii_inst_t inst, bool interrupt_mask) {

    enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;
    uint8_t interrupt_mask_byte = 0;

    interrupt_mask_byte = (interrupt_mask ?
            interrupt_mask_byte | SII_BIT_IPC_SLAVE_INTERRUPT_MASK :
            interrupt_mask_byte & ~SII_BIT_IPC_SLAVE_INTERRUPT_MASK);

    status = sii_ipc_hal_host_write_byte(
            inst, SII_REG_IPC_SLAVE_INTERRUPT_MASK, interrupt_mask_byte);

    if (status != SII_IPC_STATUS_SUCCESS) {
        goto exit;
    }

exit:
    return status;
}


enum sii_ipc_status sii_ipc_host_enable_interrupt(sii_inst_t inst, bool enable) {
    enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;

    status = sii_ipc_hal_set_slave_interrupt_mask(inst, enable);
    return status;
}

enum sii_ipc_status sii_ipc_hal_clear_slave_interrupt(sii_inst_t inst) {
    enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;
    uint8_t interrupt_byte = 0;

    interrupt_byte = interrupt_byte | SII_BIT_IPC_SLAVE_INTERRUPT;

    status = sii_ipc_hal_host_write_byte(
            inst, SII_REG_IPC_SLAVE_INTERRUPT, interrupt_byte);

    if (status != SII_IPC_STATUS_SUCCESS) {
        goto exit;
    }

exit:
    return status;
}


enum sii_ipc_status sii_ipc_host_clear_interrupt(sii_inst_t inst) {
    enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;

    status = sii_ipc_hal_clear_slave_interrupt(inst);
    return status;
}


enum sii_ipc_status sii_ipc_host_clear_and_enable_interrupt(sii_inst_t inst) {
    enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;

    status = sii_ipc_host_clear_interrupt(inst);
    if (status != SII_IPC_STATUS_SUCCESS) {
        goto exit;
    }

    status = sii_ipc_host_enable_interrupt(inst, true);
    if (status != SII_IPC_STATUS_SUCCESS) {
        goto exit;
    }

exit:
    return status;
}

void sii_ipc_display_error(enum sii_ipc_status status) {

    if (status <= SII_IPC_STATUS_ERROR_UNKNOWN) {
/*
    SII_IPC_STATUS_ERROR_UNKNOWN = -99
*/
#ifdef SII_IPC_HOST
        pr_err("UNKNOWN error found on Host side\n");
#endif /* SII_IPC_HOST */
#ifdef SII_IPC_SLAVE
    pr_err(("UNKNOWN error found on Slave side\n"));
#endif /* SII_IPC_SLAVE */
    } else if (status <= SII_IPC_STATUS_ERROR_HAL) {
/*
    SII_IPC_STATUS_ERROR_HAL = -80,
    SII_IPC_STATUS_ERROR_HAL_CHECKSUM = -81,
    SII_IPC_STATUS_ERROR_HAL_EXCEED_BUFFER_LIMIT = -82,
    SII_IPC_STATUS_ERROR_HAL_NO_REPLY_IN_BUFFER = -83,
    SII_IPC_STATUS_ERROR_HAL_DEVICE = -89,
*/
#ifdef SII_IPC_HOST
        pr_err("HAL error found on Host side\n");
#endif /* SII_IPC_HOST */
#ifdef SII_IPC_SLAVE
    pr_err(("HAL error found on Slave side\n"));
#endif /* SII_IPC_SLAVE */
    } else if (status <= SII_IPC_STATUS_ERROR_OSAL) {
/*
    SII_IPC_STATUS_ERROR_OSAL = -70,
    SII_IPC_STATUS_ERROR_OSAL_OUT_OF_MEMORY = -71,
    SII_IPC_STATUS_ERROR_OSAL_SEMAPHORE = -72,
    SII_IPC_STATUS_ERROR_OSAL_SEMAPHORE_TIMEOUT = -73,
*/
#ifdef SII_IPC_HOST
        pr_err("OSAL error found on Host side\n");
#endif /* SII_IPC_HOST */
#ifdef SII_IPC_SLAVE
    pr_err(("OSAL error found on Slave side\n"));
#endif /* SII_IPC_SLAVE */
    } else if (status <= SII_IPC_STATUS_ERROR_PACKET) {
/*
    SII_IPC_STATUS_ERROR_PACKET = -60,
    SII_IPC_STATUS_ERROR_PACKET_CHECKSUM = -61,
*/
#ifdef SII_IPC_HOST
        pr_err("PACKET error found on Host side\n");
#endif /* SII_IPC_HOST */
#ifdef SII_IPC_SLAVE
    pr_err(("PACKET error found on Slave side\n"));
#endif /* SII_IPC_SLAVE */
    } else if (status <= SII_IPC_STATUS_ERROR_MESSAGE) {
/*
    SII_IPC_STATUS_ERROR_MESSAGE = -50,
    SII_IPC_STATUS_ERROR_MESSAGE_CHECKSUM = -51,
*/
#ifdef SII_IPC_HOST
        pr_err("MESSAGE error found on Host side\n");
#endif /* SII_IPC_HOST */
#ifdef SII_IPC_SLAVE
    pr_err(("MESSAGE error found on Slave side\n"));
#endif /* SII_IPC_SLAVE */
    } else if (status <= SII_IPC_STATUS_ERROR_PROTOCOL) {
/*
    SII_IPC_STATUS_ERROR_PROTOCOL = -40,
*/
#ifdef SII_IPC_HOST
        pr_err("PROTOCOL error found on Host side\n");
#endif /* SII_IPC_HOST */
#ifdef SII_IPC_SLAVE
    pr_err(("PROTOCOL error found on Slave side\n"));
#endif /* SII_IPC_SLAVE */
    } else if (status <= SII_IPC_STATUS_ERROR_INVALID_NOTIFICATION) {
/*
    SII_IPC_STATUS_ERROR_INVALID_NOTIFICATION = -30,
    SII_IPC_STATUS_ERROR_NOTIFICATION_QUEUE_FULL = -31,
    SII_IPC_STATUS_ERROR_NOTIFICATION_QUEUE_EMPTY = -32,
*/
#ifdef SII_IPC_HOST
        pr_err("NOTIFICATION error found on Host side\n");
#endif /* SII_IPC_HOST */
#ifdef SII_IPC_SLAVE
    pr_err(("NOTIFICATION error found on Slave side\n"));
#endif /* SII_IPC_SLAVE */
    } else if (status <= SII_IPC_STATUS_ERROR_INVALID_REPLY) {
/*
    SII_IPC_STATUS_ERROR_INVALID_REPLY = -20,
*/
#ifdef SII_IPC_HOST
        pr_err("REPLY error found on Host side\n");
#endif /* SII_IPC_HOST */
#ifdef SII_IPC_SLAVE
    pr_err(("REPLY error found on Slave side\n"));
#endif /* SII_IPC_SLAVE */
    } else if (status <= SII_IPC_STATUS_ERROR_INVALID_COMMAND) {
/*
    SII_IPC_STATUS_ERROR_INVALID_COMMAND = -10,
    SII_IPC_STATUS_ERROR_INVALID_PARAMETER = -11,
*/
#ifdef SII_IPC_HOST
        pr_err("COMMAND error found on Host side\n");
#endif /* SII_IPC_HOST */
#ifdef SII_IPC_SLAVE
    pr_err(("COMMAND error found on Slave side\n"));
#endif /* SII_IPC_SLAVE */
    } else if (status < SII_IPC_STATUS_SUCCESS) {
/*
    SII_IPC_STATUS_SUCCESS = 0,
    SII_IPC_STATUS_FAILURE = -1,
    SII_IPC_STATUS_TIMEOUT = -2,
*/
#ifdef SII_IPC_HOST
        pr_err("TOP LEVEL error found on Host side\n");
#endif /* SII_IPC_HOST */
#ifdef SII_IPC_SLAVE
    pr_err(("TOP LEVEL error found on Slave side\n"));
#endif /* SII_IPC_SLAVE */
    }
}

enum sii_ipc_status sii_ipc_waiting_reply(sii_inst_t inst)
{
    uint8_t status_register1;
    uint8_t status_register2;
    uint32_t tm_start = sii_get_milli_sec();
    enum sii_ipc_status status;
    //sii_log_print("%s:%d start\n", __func__, __LINE__);
        usleep(1000);
    // check if getting notification
    ipc_host_context[inst].slave_status2_cache = false;
    status = sii_ipc_hal_read_slave_status2(inst, &status_register2); // store in cache
    if( status_register2 & SII_BIT_IPC_SLAVE_STATUS2_NTF_STATE_MASK ){
        sii_ipc_host_handler(inst);
        //sii_handle(inst, ex_sp_pdata);
    }
    while( tm_start + 300 > sii_get_milli_sec() ){
        ipc_host_context[inst].slave_status1_cache = false;
        status = sii_ipc_hal_read_slave_status1(inst, &status_register1);
        if (status != SII_IPC_STATUS_SUCCESS) {
            status_register1 = 0x00;
        }
        if(status_register1 & SII_BIT_IPC_SLAVE_STATUS1_RPL_STATE_MASK ){
            ipc_host_context[inst].slave_status2_cache = false;
            status = sii_ipc_hal_read_slave_status2(inst, &status_register2); // store in cache
            break;
        }
        usleep(1000);
    }
    // check if getting notification
    ipc_host_context[inst].slave_status2_cache = false;
    status = sii_ipc_hal_read_slave_status2(inst, &status_register2); // store in cache
    if( status_register2 & SII_BIT_IPC_SLAVE_STATUS2_NTF_STATE_MASK ){
        sii_ipc_host_handler(inst);
        //sii_handle(inst, ex_sp_pdata);
    }
    return ( tm_start + 300 > sii_get_milli_sec() ) ? SII_IPC_STATUS_SUCCESS : SII_IPC_STATUS_TIMEOUT;
}
enum sii_ipc_status sii_ipc_check_notify(sii_inst_t inst, uint8_t *register2)
{
    uint8_t status_register2;
    enum sii_ipc_status status;
    //sii_log_print("%s:%d start\n", __func__, __LINE__);
    ipc_host_context[inst].slave_status2_cache = false;
    status = sii_ipc_hal_read_slave_status2(inst, &status_register2);
    *register2 = (status == SII_IPC_STATUS_SUCCESS) ? status_register2 : 0x00;
    return status;
}

enum sii_ipc_status sii_ipc_host_handler(sii_inst_t inst)
{
    enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;

    bool source_reply = false;
    bool source_notification = false;
    bool process_reply = false;
    bool process_notification = false;

    bool need_to_handle_ntf = false;
    bool need_to_clear_interrupt = false;
//    sii_log_print("%s:%d start\n", __func__, __LINE__);
    status = sii_ipc_host_enable_interrupt(inst, false);

    if (status != SII_IPC_STATUS_SUCCESS) {
        goto exit;
    }

    sii_ipc_hal_clear_slave_reg_cache(inst);

    /* Check Interrupt Source */
    status = sii_ipc_host_check_change(inst,
            &source_reply, &source_notification);
    if (status != SII_IPC_STATUS_SUCCESS) {
        goto exit;
    }

    /* Check State Machine Status */
    switch(ipc_host_context[inst].host_sm) {
        case SII_IPC_HOST_SM_IDLE:
            /* Must be a Notification, discard Reply if any */
            process_reply = false;

            /* Need to Receive Notification */
            process_notification = true;
            break;

        case SII_IPC_HOST_SM_SEND_COMMAND:
            /* Must be a Notification, discard Reply if any */
            process_reply = false;

            /* Need to Receive Notification */
            process_notification = true;
            break;

        case SII_IPC_HOST_SM_WAIT_REPLY:
            /* May have a Reply */
            process_reply = true;

            /* Need to Receive Notification */
            process_notification = true;
            break;

        case SII_IPC_HOST_SM_BUFFER_REPLY_IN_PROGRESS:
            /* May have a Reply */
            process_reply = true;

            /* Need to Receive Notification */
            process_notification = true;
            break;

        default:
            break;
    }

    /* Receive Reply */
    if (process_reply) {
    } else if (source_reply) {
        pr_err("Unwanted Reply.\n");
        /* Discard unwanted Reply, simply
           clear the interrupt per Protocol */
        need_to_clear_interrupt = true;
    }

    /* Receive Notification */
    if (process_notification && source_notification) {

        /* FD, TBC, double check this */

        if (process_reply && source_reply) {
            /* FD: TBU, no support for this case yet */
            /* Host should handle Reply first,
               add Notification in queue */

            need_to_clear_interrupt = false;
        } else if (process_reply) {
            /* Host should wait for Reply back,
               add Notification in queue */
            need_to_handle_ntf = true;

            need_to_clear_interrupt = true;
        } else {
            /* Process Notification right now */
            need_to_handle_ntf = true;

            need_to_clear_interrupt = true;
        }
    } else if (source_notification) {
        pr_err("Unwanted Notification.\n");
        /* Discard unwanted Notification, simply
           clear the interrupt per Protocol */
        need_to_clear_interrupt = true;
    } else {
        /* No Notification now, nothing to do */
    }

    if (need_to_handle_ntf) {
        /* Retrieve the Packet Notification */
        status = sii_ipc_receive_buffer_notification(inst,
            &(ipc_host_context[inst].host_packet_notification));
        if (status != SII_IPC_STATUS_SUCCESS) {
            goto exit_clear_interrupt;
        }

        /* Check Notification Type */
        if (SII_IPC_PHY_NOTIFICATION_TYPE_BUFFER_NOTIFICATION
            == ipc_host_context[inst].host_packet_notification.type) {
            /* Extract Notification per Notification Packet*/
            status =
                sii_ipc_extract_notification_from_packet_ntf(inst,
                ipc_host_context[inst].host_packet_notification.packet);
            if (status != SII_IPC_STATUS_SUCCESS) {
                goto exit_clear_interrupt;
            }
        }
        /* FD: TBU, support of other Notification Types */
    }

exit_clear_interrupt:
    if (need_to_clear_interrupt) {
        status = sii_ipc_host_clear_and_enable_interrupt(inst);
        if (status != SII_IPC_STATUS_SUCCESS) {
            goto exit_give_semaphore;
        }
    }

exit_give_semaphore:

exit:
    sii_ipc_display_error(status);
    return status;
}

uint8_t calculate_checksum_2bit(uint8_t value1, uint8_t value2) {
    uint8_t checksum = 0x00;

    checksum = (value1 & 0x03)
        + ((value1 >> 2) & 0x03)
        + ((value1 >> 4) & 0x03)
        + ((value1 >> 6) & 0x03)
               + (value2 & 0x03)
        + ((value2 >> 2) & 0x03)
        + ((value2 >> 4) & 0x03)
        + ((value2 >> 6) & 0x03);
    return (0 - checksum) & 0x03;
}

uint8_t calculate_checksum_byte(uint8_t *bytes, uint16_t length) {
    return calculate_generic_checksum(bytes, 0, length);
}


enum sii_ipc_status sii_ipc_sync_with_slave_before_send_command(sii_inst_t inst) {
    enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;
    uint8_t status_register1 = 0;
    bool last_reply_active = true;
	uint16_t counter = 0;

    do {
        ipc_host_context[inst].slave_status1_cache = false;
        status = sii_ipc_hal_read_slave_status1(
                inst, &status_register1);
        if (status != SII_IPC_STATUS_SUCCESS)
        {
            goto exit;
        }
        msleep(1);

        last_reply_active = (SII_BIT_IPC_SLAVE_STATUS1_RPL_STATE_MASK
                & status_register1) ? true : false;

        counter++;

        if (counter > 100)
        {
           sii_ipc_host_clear_interrupt(inst);
           counter = 0;
           continue;
        }
    } while (true == last_reply_active);

exit:
    return status;
}


enum sii_ipc_status sii_ipc_hal_write_host_status2(
        sii_inst_t inst, uint8_t status2) {

    enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;
    uint8_t status2_reg = 0;

    status2_reg = status2;
    status = sii_ipc_hal_host_write_byte(
            inst, SII_REG_IPC_HOST_STATUS2, status2_reg);

    if (status != SII_IPC_STATUS_SUCCESS) {
        goto exit;
    }

exit:
    return status;
}


enum sii_ipc_status sii_ipc_hal_write_host_status1(
        sii_inst_t inst, uint8_t status1) {

    enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;
    uint8_t status1_reg = 0;

    status1_reg = status1;
    status = sii_ipc_hal_host_write_byte(
            inst, SII_REG_IPC_HOST_STATUS1, status1_reg);

    if (status != SII_IPC_STATUS_SUCCESS) {
        goto exit;
    }

exit:
    return status;
}


enum sii_ipc_status sii_ipc_send_register_command(
    sii_inst_t inst, struct sii_ipc_register_command command) {
    enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;

    uint8_t status_register1 = 0x00;
    uint8_t status_register2 = 0x00;
    uint8_t checksum = 0x00;

    command.opcode = (command.opcode
            << SII_BIT_IPC_HOST_STATUS1_CMD_OPCODE_SHIFT)
            & SII_BIT_IPC_HOST_STATUS1_CMD_OPCODE_MASK;
    command.parameter = (command.parameter
            << SII_BIT_IPC_HOST_STATUS2_CMD_PARAMETER_SHIFT)
            & SII_BIT_IPC_HOST_STATUS2_CMD_PARAMETER_MASK;

    checksum = calculate_checksum_2bit(command.opcode, command.parameter);

    status_register1 = command.opcode;
    status_register2 = command.parameter | checksum;
    status = sii_ipc_hal_write_host_status2(inst, status_register2);

    if (status != SII_IPC_STATUS_SUCCESS) {
        goto exit;
    }

    status = sii_ipc_hal_write_host_status1(inst, status_register1 | 0x80);
    if (status != SII_IPC_STATUS_SUCCESS) {
        goto exit;
    }

exit:
    return status;
}


enum sii_ipc_status sii_ipc_receive_register_reply(
    sii_inst_t inst, struct sii_ipc_register_reply *reply) {
    enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;

    uint8_t status_register1 = 0x00;
    uint8_t status_register2 = 0x00;
    bool verify_checksum = false;

    status = sii_ipc_hal_read_slave_status1(inst, &status_register1);
    if (status != SII_IPC_STATUS_SUCCESS) {
            pr_err("%s:%d \n", __func__, __LINE__);
        goto exit;
    }

    status = sii_ipc_hal_read_slave_status2(inst, &status_register2);
    if (status != SII_IPC_STATUS_SUCCESS) {
            pr_err("%s:%d \n", __func__, __LINE__);
        goto exit;
    }

    verify_checksum = verify_checksum_2bit(status_register1, status_register2);
    if (!verify_checksum) {
            pr_err("%s:%d \n", __func__, __LINE__);
        status = SII_IPC_STATUS_ERROR_HAL_CHECKSUM;
        goto exit;
    }

    reply->reply_status = (status_register1
            & SII_BIT_IPC_SLAVE_STATUS1_REPLY_STATUS_MASK)
            >> SII_BIT_IPC_SLAVE_STATUS1_REPLY_STATUS_SHIFT;
    reply->reply_return = (status_register1
            & SII_BIT_IPC_SLAVE_STATUS1_REPLY_RETURN_MASK)
            >> SII_BIT_IPC_SLAVE_STATUS1_REPLY_RETURN_SHIFT;

exit:
    return status;
}

enum sii_ipc_status sii_execute_register_command_register_reply(
    sii_inst_t inst, struct sii_ipc_register_command command,
    struct sii_ipc_register_reply *reply) {

    enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;
    enum sii_ipc_status status2 = SII_IPC_STATUS_SUCCESS;
    bool clear_interrupt = false;

    status = sii_ipc_sync_with_slave_before_send_command(inst);
    if (status != SII_IPC_STATUS_SUCCESS) {
        pr_err("%s:%d \n", __func__, __LINE__);
        goto exit;
    }

    /* Send the Command (Register Command) */
    status = sii_ipc_send_register_command(inst, command);
    if (status != SII_IPC_STATUS_SUCCESS) {
        pr_err("%s:%d \n", __func__, __LINE__);
        goto exit;
    }

    ipc_host_context[inst].host_sm = SII_IPC_HOST_SM_WAIT_REPLY;

    status = sii_ipc_waiting_reply(inst);
    if( status == SII_IPC_STATUS_SUCCESS ){
        sii_ipc_host_handler(inst);
    }
    else{
        pr_err("%s:%d IPC POLLING TIMEOUT *** \n", __func__, __LINE__);
    }
    clear_interrupt = true;

    /* Receive the Reply (Register Reply) */
    status = sii_ipc_receive_register_reply(inst, reply);
    if (status != SII_IPC_STATUS_SUCCESS) {
            pr_err("%s:%d \n", __func__, __LINE__);
        goto exit;
    }

    /* FD: TBI, Stop Send Command Timer */

exit:
    ipc_host_context[inst].host_sm = SII_IPC_HOST_SM_IDLE;

    if (true == clear_interrupt) {
        status2 = sii_ipc_host_clear_and_enable_interrupt(inst);
        if (status2 != SII_IPC_STATUS_SUCCESS) {
              pr_err("%s:%d \n", __func__, __LINE__);
            status = status2;
        }
    }

    /* FD: TBI, Stop Send Command Timer if there */

    return status;
}

static enum sii_ipc_api_status sii_api_reg_cmd_reg_reply_process(
        sii_inst_t inst,
        struct sii_ipc_register_command command,
        struct sii_ipc_register_reply *reply)
{
    enum sii_ipc_api_status api_status = SII_IPC_API_SUCCESS;
    enum sii_ipc_status ipc_status = SII_IPC_STATUS_SUCCESS;

    ipc_status = sii_execute_register_command_register_reply(inst,
                                                                command,
                                                                reply);
    if (ipc_status != SII_IPC_STATUS_SUCCESS) {
        api_status = SII_IPC_API_FAILURE;
            pr_err("%s:%d \n", __func__, __LINE__);
        //last_err = SII_IPC_API_ERR__INTERNAL;
        goto exit;
    }

    if (reply->reply_status != SII_IPC_PHY_REPLY_STATUS_SUCCESS) {
        api_status = SII_IPC_API_FAILURE;
            pr_err("%s:%d  %d\n", __func__, __LINE__, SII_IPC_API_FAILURE);
        /* Save api error code */
        //last_err = reply->reply_return;
    }
exit:
    sii_ipc_display_error(ipc_status);
    return api_status;
}


static int8_t get_next_instance_index(void)
{
    int8_t index;
    for (index = 0; index < SII_INST_COUNT_MAX; index++) {
        if (!api_host_context.instance[index].in_use)
            break;
    }
    if (index == SII_INST_COUNT_MAX)
        return -1;
    return index;
}


void sii_ipc_init(sii_inst_t inst)
{

    memset(&ipc_host_context[inst], 0, sizeof(ipc_host_context[inst]));

    /* Explicit init */
    ipc_host_context[inst].notification_queue_count = 0;
    ipc_host_context[inst].notification_queue_head = 0;
    ipc_host_context[inst].notification_queue_tail = 0;
    ipc_host_context[inst].host_sm = SII_IPC_HOST_SM_IDLE;

    ipc_host_context[inst].host_packet_notification.packet =
        &(ipc_host_context[inst].host_packet_ntf);

    ipc_host_context[inst].packet_sequence = SII_IPC_PACKET_SEQUENCE_NONE;
    ipc_host_context[inst].packet_fragment = SII_IPC_PACKET_FRAGMENT_NONE;

    ipc_host_context[inst].slave_status1_cache = false;
    ipc_host_context[inst].slave_status2_cache = false;

}

enum sii_ipc_status sii_ipc_hal_set_host_interrupt(
        sii_inst_t inst, bool interrupt) {

    enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;
    uint8_t interrupt_byte = 0;

    status = sii_ipc_hal_host_read_byte(
            inst, SII_REG_IPC_HOST_INTERRUPT, &interrupt_byte);

    if (status != SII_IPC_STATUS_SUCCESS) {
        goto exit;
    }

    interrupt_byte = (interrupt ?
            interrupt_byte | SII_BIT_IPC_HOST_INTERRUPT :
            interrupt_byte & ~SII_BIT_IPC_HOST_INTERRUPT);

    status = sii_ipc_hal_host_write_byte(
            inst, SII_REG_IPC_HOST_INTERRUPT, interrupt_byte);

    if (status != SII_IPC_STATUS_SUCCESS) {
        goto exit;
    }

exit:
    return status;
}

enum sii_ipc_status sii_ipc_hal_clear_host_buffer(sii_inst_t inst) {
    enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;
    uint8_t buffer_init[SII_IPC_HOST_BUFFER_LENGTH] = {0};

    status = sii_ipc_hal_host_write_block(
            inst, SII_REG_IPC_HOST_BUFFER_START,
            buffer_init, SII_IPC_HOST_BUFFER_LENGTH);

    if (status != SII_IPC_STATUS_SUCCESS) {
        goto exit;
    }

exit:
    return status;
}


enum sii_ipc_status sii_ipc_hal_host_init(sii_inst_t inst) {
    enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;

    /*
       Init host interupt/mask, status, and buffer
     */

    /* clear interrupt if any */
    status = sii_ipc_hal_set_host_interrupt(inst, false);
    if (status != SII_IPC_STATUS_SUCCESS) {
        goto exit;
    }

    /* clear status 1 */
    status = sii_ipc_hal_write_host_status1(inst, 0x00);
    if (status != SII_IPC_STATUS_SUCCESS) {
        goto exit;
    }

    /* clear status 2 */
    status = sii_ipc_hal_write_host_status2(inst, 0x00);
    if (status != SII_IPC_STATUS_SUCCESS) {
        goto exit;
    }

    /* clear buffer */
    status = sii_ipc_hal_clear_host_buffer(inst);
    if (status != SII_IPC_STATUS_SUCCESS) {
        goto exit;
    }

exit:
    return status;
}

enum sii_status sii_hal_isr_enable(sii_hwid_t hwid, bool enable)
{
    struct sii_device *dev = &sii_dev[hwid];
    dev->isr_enable = enable;
    return SII_STATUS_SUCCESS;
}
enum sii_status sii_device_isr_enable(sii_inst_t inst, bool enable)
{
    sii_hwid_t hwid = 0;
    return sii_hal_isr_enable(hwid, enable);
}

enum sii_api_status sii_wait_for_boot_done(sii_inst_t inst)
{
    enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;
    msleep(500);
    status = sii_ipc_hal_host_init(inst);
      if(status != SII_IPC_STATUS_SUCCESS)
          return status==0?SII_API_SUCCESS : SII_API_FAILURE;

    status = (enum sii_ipc_status)sii_device_isr_enable(inst, true);
      if(status != SII_IPC_STATUS_SUCCESS)
          return status==0?SII_API_SUCCESS : SII_API_FAILURE;

     status = sii_ipc_hal_set_slave_interrupt_mask(inst, true);
      if(status != SII_IPC_STATUS_SUCCESS)
          return status==0?SII_API_SUCCESS : SII_API_FAILURE;

    return status==0?SII_API_SUCCESS : SII_API_FAILURE;
}

enum sii_ipc_api_status sii_host_IPC_call(
        sii_inst_t inst,
        uint8_t opcode,
        uint8_t parameter,
        uint8_t fail_val,
        void *reply
        )
{
    struct sii_ipc_register_command command;
    struct sii_ipc_register_reply ipc_reply;
    enum sii_ipc_api_status ipc_status;
    uint8_t *ret = (uint8_t *)reply;

    command.opcode = opcode;
    command.parameter = parameter;

    ipc_status = sii_api_reg_cmd_reg_reply_process(inst, command, &ipc_reply);
    if( ret != NULL )
    {
        *ret = ( ipc_status == SII_IPC_API_SUCCESS ) ? ipc_reply.reply_return : fail_val;
    }
    return ipc_status;
}

// ************* get *************

#define sii_get_boot_status(inst, status) \
    sii_host_IPC_call(inst, SII_IPC_PHY_CMD_OPCODE_GET_BOOT_STATUS, 0x00, (uint8_t)SII_BOOT_STAT__FAILURE, status)

#define sii_get_product_id(inst, status) \
    sii_host_IPC_call(inst, SII_IPC_PHY_CMD_OPCODE_GET_PRODUCT_ID, 0x00, (uint8_t)SII_PRODUCT_ID__NONE, status)

#define sii_get_host_mode(inst, status) \
    sii_host_IPC_call(inst, SII_IPC_PHY_CMD_OPCODE_GET_HOST_MODE, 0x00, (uint8_t)0, status)

#define sii_get_rx_plus5v(inst, status) \
    sii_host_IPC_call(inst, SII_IPC_PHY_CMD_OPCODE_GET_RX_PLUS_5V, 0x00, (uint8_t)0, status)

#define sii_get_rx_link_type(inst, type) \
    sii_host_IPC_call(inst, SII_IPC_PHY_CMD_OPCODE_GET_RX_LINK_TYPE, 0x00, (uint8_t)0, type)

#define sii_get_rx_av_mute(inst, status) \
    sii_host_IPC_call(inst, SII_IPC_PHY_CMD_OPCODE_GET_RX_AV_MUTE, 0x00, (uint8_t)0, status)

#define sii_get_tx_hpd(inst, hpd) \
    sii_host_IPC_call(inst, SII_IPC_PHY_CMD_OPCODE_GET_TX_HPD, 0x00, (uint8_t)0, hpd)

#define sii_get_rx_hdcp_us_version(inst, version) \
    sii_host_IPC_call(inst, SII_IPC_PHY_CMD_OPCODE_GET_RX_HDCP_US_VERSION, 0x00, (uint8_t)0, version)

#define sii_get_tx_rxsense(inst, status) \
    sii_host_IPC_call(inst, SII_IPC_PHY_CMD_OPCODE_GET_TX_RXSENSE, 0x00, (uint8_t)0, status)


// ************* set *************

#define sii_set_host_mode(inst, val) \
    sii_host_IPC_call(inst, SII_IPC_PHY_CMD_OPCODE_SET_HOST_MODE, val, (uint8_t)0, NULL)

#define sii_set_rx_hdcp_repeater_mode(inst, val) \
    sii_host_IPC_call(inst, SII_IPC_PHY_CMD_OPCODE_SET_RX_HDCP_REPEATER_MODE, val, (uint8_t)0, NULL)

#define sii_set_tx_scramble(inst, val) \
    sii_host_IPC_call(inst, SII_IPC_PHY_CMD_OPCODE_SET_TX_SCRAMBLE, val, (uint8_t)0, NULL)

#define sii_trigger_rx_hdcp_rxid_list_to_send(inst) \
    sii_host_IPC_call(inst, SII_IPC_PHY_CMD_OPCODE_TRIGGER_RX_HDCP_RXID_LIST_TO_SEND, 0, (uint8_t)0, NULL)


static enum sii_ipc_status sii_ipc_extract_packet_from_buffer_reply(
    sii_inst_t inst, struct sii_ipc_packet *reply_packet,
    struct sii_ipc_buffer_reply *reply) {
    enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;
    bool verify_checksum = false;

    if (SII_IPC_PACKET_PROTOCOL_VERSION_1 != reply->packet->protocol_version) {
        /* FD: TBR, support future protocol extension */
        status = SII_IPC_STATUS_ERROR_PROTOCOL_VERSION_UNSUPPORTED;
        goto exit;
    } else {
        reply_packet->protocol_version = reply->packet->protocol_version;

        /* Verify checksum */
        verify_checksum = verify_checksum_byte((uint8_t *)(reply->packet),
                            reply->packet->packet_length);

        if (!verify_checksum) {
            status = SII_IPC_STATUS_ERROR_PACKET_CHECKSUM;
            goto exit;
        }

        /* Extract real Packet payload */
        reply_packet->sequence_number = reply->packet->sequence_number;
        reply_packet->fragment_count = reply->packet->fragment_count;
        reply_packet->packet_length = reply->packet->packet_length;
        reply_packet->packet_checksum = reply->packet->packet_checksum;

        memcpy(reply_packet->packet_payload,
                reply->packet->packet_payload,
                reply_packet->packet_length - SII_IPC_PACKET_HEADER_LENGTH);
    }

exit:
    return status;
}


enum sii_ipc_status sii_ipc_hal_read_slave_buffer_reply(
        sii_inst_t inst, uint8_t *buffer, uint8_t length) {

    enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;


    if (length > SII_IPC_SLAVE_BUF_RPL_LENGTH || 0 == length) {
        status = SII_IPC_STATUS_ERROR_HAL_EXCEED_BUFFER_LIMIT;
        goto exit;
    }

    status = sii_ipc_hal_host_read_block(
            inst, SII_REG_IPC_SLAVE_BUF_RPL_START, buffer, length);

    if (status != SII_IPC_STATUS_SUCCESS) {
        goto exit;
    }

exit:
    return status;
}


enum sii_ipc_status sii_ipc_receive_buffer_reply(
    sii_inst_t inst, struct sii_ipc_packet_reply *packet_reply,
    bool *more_packet) {
    enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;

    struct sii_ipc_buffer_reply buffer_reply;
    uint8_t status_register1 = 0x00;
    uint8_t status_register2 = 0x00;
    bool verify_checksum = false;
    uint8_t buffer_status = 0x00;

    /* Initial Return */
    *more_packet = false;

    status = sii_ipc_hal_read_slave_status1(inst, &status_register1);
    if (status != SII_IPC_STATUS_SUCCESS) {
        goto exit;
    }

    status = sii_ipc_hal_read_slave_status2(inst, &status_register2);
    if (status != SII_IPC_STATUS_SUCCESS) {
        goto exit;
    }

    verify_checksum = verify_checksum_2bit(
            status_register1, status_register2);
    if (!verify_checksum) {
        status = SII_IPC_STATUS_ERROR_HAL_CHECKSUM;
        goto exit;
    }

    packet_reply->reply_status = (status_register1
            & SII_BIT_IPC_SLAVE_STATUS1_REPLY_STATUS_MASK)
            >> SII_BIT_IPC_SLAVE_STATUS1_REPLY_STATUS_SHIFT;

    packet_reply->reply_return = (status_register1
            & SII_BIT_IPC_SLAVE_STATUS1_REPLY_RETURN_MASK)
            >> SII_BIT_IPC_SLAVE_STATUS1_REPLY_RETURN_SHIFT;

    if (packet_reply->reply_status != SII_IPC_PHY_REPLY_STATUS_SUCCESS) {
        /* NACK / ACK / FAILURE : no need to check the Buffer */
        goto exit;
    }

    buffer_status = (status_register1
            & SII_BIT_IPC_SLAVE_STATUS1_BUFFER_STATUS_MASK)
            >> SII_BIT_IPC_SLAVE_STATUS1_BUFFER_STATUS_SHIFT;
    if (buffer_status != SII_IPC_PHY_BUFFER_STATUS_REPLY) {
        status = SII_IPC_STATUS_ERROR_HAL_NO_REPLY_IN_BUFFER;
        goto exit;
    }

    buffer_reply.packet = &(ipc_host_context[inst].host_packet);
    memset(buffer_reply.packet, 0, SII_IPC_PACKET_LENGTH);

    status = sii_ipc_hal_read_slave_buffer_reply(
            inst,
            (void *)(buffer_reply.packet), SII_IPC_PACKET_LENGTH);
    if (status != SII_IPC_STATUS_SUCCESS) {
        goto exit;
    }

    /* FD: TBR, double check this to save one copy & process */
    status = sii_ipc_extract_packet_from_buffer_reply(inst,
            packet_reply->packet, &buffer_reply);
    if (status != SII_IPC_STATUS_SUCCESS) {
        goto exit;
    }

    /* FD: TBC, may need more careful check in case out-of-order delivery */
    *more_packet = packet_reply->packet->sequence_number + 1
                    < packet_reply->packet->fragment_count
                    ? true : false;

exit:
    return status;
}


static enum sii_ipc_status sii_execute_register_command_buffer_reply(
    sii_inst_t inst, struct sii_ipc_register_command command,
    struct sii_ipc_packet_reply *packet_reply,
    bool *more_packet) {

    enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;
    enum sii_ipc_status status2 = SII_IPC_STATUS_SUCCESS;
    bool clear_interrupt = false;

    /* Initial Return */
    *more_packet = false;

    status = sii_ipc_sync_with_slave_before_send_command(inst);
    if (status != SII_IPC_STATUS_SUCCESS) {
        goto exit;
    }

    /* Send the Command (Register Command) */
    status = sii_ipc_send_register_command(inst, command);
    if (status != SII_IPC_STATUS_SUCCESS) {
        goto exit;
    }

    ipc_host_context[inst].host_sm = SII_IPC_HOST_SM_WAIT_REPLY;
    status = sii_ipc_waiting_reply(inst);
    if( status == SII_IPC_STATUS_SUCCESS ){
        sii_ipc_host_handler(inst);
    }
    else{
        pr_err("%s:%d IPC POLLING TIMEOUT *** \n", __func__, __LINE__);
    }
    clear_interrupt = true;

    /* Receive the Reply (Buffer Reply) */
    status = sii_ipc_receive_buffer_reply(inst, packet_reply, more_packet);
    if (status != SII_IPC_STATUS_SUCCESS) {
        goto exit;
    }

    /* FD: TBI, Stop Send Command Timer */

exit:
    ipc_host_context[inst].host_sm = SII_IPC_HOST_SM_IDLE;

    if (true == clear_interrupt) {
        status2 = sii_ipc_host_clear_and_enable_interrupt(inst);
        if (status2 != SII_IPC_STATUS_SUCCESS) {
            status = status2;
        }
    }

    /* FD: TBI, Stop Send Command Timer if there */

    return status;
}


static void sii_compose_message_reply_from_packet_reply(
    sii_inst_t inst, struct sii_ipc_message_reply *message_reply,
    struct sii_ipc_packet_reply *packet_reply) {

    struct sii_ipc_raw_message *raw_message =
                (struct sii_ipc_raw_message*)(message_reply->message);
    struct sii_ipc_packet *reply_packet = packet_reply->packet;

    memcpy(raw_message->raw_data + reply_packet->sequence_number
                * SII_IPC_PACKET_PAYLOAD_LENGTH_MAX,
            reply_packet->packet_payload,
            reply_packet->packet_length - SII_IPC_PACKET_HEADER_LENGTH);

    /* Save Reply Status & Reply Return */
    message_reply->reply_status = packet_reply->reply_status;
    message_reply->reply_return = packet_reply->reply_return;
}


static enum sii_ipc_status sii_receive_next_piece_of_buffer_reply(
    sii_inst_t inst, struct sii_ipc_packet_reply *packet_reply,
    bool *more_packet) {

    struct sii_ipc_register_command command;
    enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;

    /* Initial Return */
    *more_packet = false;

    /* Prepare the Command */
    command.opcode = SII_IPC_PHY_CMD_OPCODE_ACK_SLAVE_BUFFER_REPLY;
    command.parameter = 0x00;

    status = sii_execute_register_command_buffer_reply(inst,
                command, packet_reply, more_packet);

    return status;
}


/*
   IPC External API for Host Side
 */

enum sii_ipc_status sii_execute_register_command_no_reply(
    sii_inst_t inst, struct sii_ipc_register_command command) {

    enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;

    status = sii_ipc_sync_with_slave_before_send_command(inst);
    if (status != SII_IPC_STATUS_SUCCESS) {
        goto exit;
    }

    /* Send the Command (Register Command) */
    status = sii_ipc_send_register_command(inst, command);
    if (status != SII_IPC_STATUS_SUCCESS) {
        goto exit;
    }

exit:
    ipc_host_context[inst].host_sm = SII_IPC_HOST_SM_IDLE;

    return status;
}


static enum sii_ipc_status sii_refuse_next_piece_of_buffer_reply(sii_inst_t inst) {
    struct sii_ipc_register_command command;
    enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;

    /* Prepare the Command */
    command.opcode = SII_IPC_PHY_CMD_OPCODE_NACK_SLAVE_BUFFER_REPLY;
    command.parameter = 0x00;  // reason of NACK if necessary

    status = sii_execute_register_command_no_reply(inst, command);

    return status;
}


static enum sii_ipc_status sii_accept_last_piece_of_buffer_reply(sii_inst_t inst) {
    struct sii_ipc_register_command command;
    enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;

    /* Prepare the Command */
    command.opcode = SII_IPC_PHY_CMD_OPCODE_ACK_SLAVE_BUFFER_REPLY;
    command.parameter = 0x00;

    status = sii_execute_register_command_no_reply(inst, command);

    return status;
}


enum sii_ipc_status sii_execute_register_command_message_reply(
        sii_inst_t inst, struct sii_ipc_register_command command,
        struct sii_ipc_message_reply *message_reply) {

    enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;
    enum sii_ipc_status status2 = SII_IPC_STATUS_SUCCESS;
    struct sii_ipc_packet_reply packet_reply;
    struct sii_ipc_packet reply_packet;
    bool more_packet = false;
    bool ack_last_piece = false;

    packet_reply.packet = &reply_packet;

    status = sii_execute_register_command_buffer_reply(inst,
            command, &packet_reply, &more_packet);
    if (status != SII_IPC_STATUS_SUCCESS) {
        goto exit;
    }

    if (packet_reply.reply_status != SII_IPC_PHY_REPLY_STATUS_SUCCESS) {
        /* Successful IPC Pair (Command & Reply),
           Unsuccessful Execution */
        goto exit;
    }

    /* Compose Message Reply from Packet Reply */
    sii_compose_message_reply_from_packet_reply(inst,
            message_reply, &packet_reply);

    while (more_packet) {
        status = sii_receive_next_piece_of_buffer_reply(inst,
                &packet_reply, &more_packet);

        /* Refuse next piece of Reply
           if there is any error during parsing */
        if (status != SII_IPC_STATUS_SUCCESS) {
            status2 = sii_refuse_next_piece_of_buffer_reply(inst);
            if (status2 != SII_IPC_STATUS_SUCCESS) {
                status =
                    SII_IPC_STATUS_ERROR_PROTOCOL_CANNOT_NACK;
            } else {
                /* do nothing, use original error code */
            }

            ack_last_piece = false;
            break;  // no need to get next piece
        } else {
            ack_last_piece = true;
        }

        /* Compose Message Reply from Packet Reply */
        sii_compose_message_reply_from_packet_reply(inst,
                message_reply, &packet_reply);
    }

    /* Accept Last Piece of Buffer Reply */
    if (true == ack_last_piece) {
        sii_accept_last_piece_of_buffer_reply(inst);
        if (status != SII_IPC_STATUS_SUCCESS) {
            goto exit;
        }
    }

exit:
    return status;
}



static void sii_compose_packet_command_from_message_command(
        sii_inst_t inst, struct sii_ipc_packet_command *packet_command,
        struct sii_ipc_message_command *message_command,
        uint8_t packet_sequence) {

    struct sii_ipc_packet *command_packet = packet_command->packet;
    struct sii_ipc_raw_message *raw_message = NULL;
    uint16_t message_length = 0;

    raw_message = (struct sii_ipc_raw_message*)(message_command->message);

    memset(command_packet, 0, SII_IPC_PACKET_LENGTH);

    command_packet->protocol_version = SII_IPC_PACKET_PROTOCOL_VERSION_1;
    command_packet->sequence_number = packet_sequence;

    message_length = message_command->message->message_length_l +
        ((uint16_t)(message_command->message->message_length_h) << 8);

    command_packet->fragment_count =
        (message_length + SII_IPC_PACKET_PAYLOAD_LENGTH_MAX - 1)
        / SII_IPC_PACKET_PAYLOAD_LENGTH_MAX;

    if (command_packet->sequence_number < command_packet->fragment_count - 1) {
        command_packet->packet_length = SII_IPC_PACKET_LENGTH;
    } else {
        command_packet->packet_length = message_length -
            (SII_IPC_PACKET_PAYLOAD_LENGTH_MAX *
            (uint16_t)(command_packet->fragment_count - 1))
            + SII_IPC_PACKET_HEADER_LENGTH;
    }

    /* Update packet_fragment */
    ipc_host_context[inst].packet_fragment = command_packet->fragment_count;

    memcpy(command_packet->packet_payload,
        raw_message->raw_data +
            (command_packet->sequence_number
             * SII_IPC_PACKET_PAYLOAD_LENGTH_MAX),
        command_packet->packet_length
            - SII_IPC_PACKET_HEADER_LENGTH);

    command_packet->packet_checksum = calculate_checksum_byte(
            (uint8_t *)command_packet,
            command_packet->packet_length);

    /* Save Command Opcode & Command Parameter */
    packet_command->opcode = message_command->opcode;
    packet_command->parameter = message_command->parameter;
}

static void sii_compose_message(
        uint8_t protocol_version,
        uint8_t message_type,
        uint16_t message_id,
        uint16_t message_length,
        struct sii_ipc_message *message)
{

    message->message_payload.message_type = message_type;
    message->message_payload.message_id_h = message_id >> 8;
    message->message_payload.message_id_l = message_id & 0xFF;
    /* message_payload data is already filled by the function caller */

    message->protocol_version = protocol_version;
    message_length += SII_IPC_MESSAGE_HEADER_LENGTH;
    message->message_length_h = (uint8_t) (message_length >> 8);
    message->message_length_l = (uint8_t) (message_length & 0xFF);
    message->message_checksum = calculate_checksum_byte(
            (uint8_t *) message,
            message_length);
}


enum sii_ipc_status sii_ipc_send_buffer_command(sii_inst_t inst,
        struct sii_ipc_packet_command *packet_command,
        bool need_buffer_reply) {
    enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;

    uint8_t status_register1 = 0x00;
    uint8_t status_register2 = 0x00;
    uint8_t checksum = 0x00;

    packet_command->opcode = need_buffer_reply ?
        SII_IPC_PHY_CMD_OPCODE_BUF_CMD_BUF_RPL_INDICATOR :
        SII_IPC_PHY_CMD_OPCODE_BUFFER_COMMAND_INDICATOR;

    packet_command->opcode = (packet_command->opcode
            << SII_BIT_IPC_HOST_STATUS1_CMD_OPCODE_SHIFT)
        & SII_BIT_IPC_HOST_STATUS1_CMD_OPCODE_MASK;

    packet_command->parameter = (packet_command->parameter
            << SII_BIT_IPC_HOST_STATUS2_CMD_PARAMETER_SHIFT)
        & SII_BIT_IPC_HOST_STATUS2_CMD_PARAMETER_MASK;

    checksum = calculate_checksum_2bit(
            packet_command->opcode, packet_command->parameter);

    /*
       Prepare Status Register 1 & 2
     */
    status_register1 = packet_command->opcode;
    status_register2 = packet_command->parameter | checksum;

    /*
       Update Status Register 1 & 2
     */

    status = sii_ipc_hal_write_host_status2(inst, status_register2);
    if (status != SII_IPC_STATUS_SUCCESS) {
        goto exit;
    }

    /*
       Send the Command Packet
     */

    /* Send Buffer Out */
    status = sii_ipc_hal_write_host_buffer_command(
            inst,
            (uint8_t *)(packet_command->packet),
            packet_command->packet->packet_length);
    if (status != SII_IPC_STATUS_SUCCESS) {
        goto exit;
    }

    status = sii_ipc_hal_write_host_status1(inst, status_register1 | 0x80);
    if (status != SII_IPC_STATUS_SUCCESS) {
        goto exit;
    }
#if 0
    /*
       Trigger the Host Interrupt
     */
    status = sii_ipc_hal_set_host_interrupt(inst, true);
    if (status != SII_IPC_STATUS_SUCCESS) {
        goto exit;
    }
#endif
exit:
    return status;
}


static enum sii_ipc_status sii_execute_buffer_command_register_reply(
    sii_inst_t inst, struct sii_ipc_packet_command *command,
    struct sii_ipc_register_reply *reply) {

    enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;
    enum sii_ipc_status status2 = SII_IPC_STATUS_SUCCESS;
    bool clear_interrupt = false;

    status = sii_ipc_sync_with_slave_before_send_command(inst);
    if (status != SII_IPC_STATUS_SUCCESS) {
        goto exit;
    }
    /* Send the Command (Buffer Command)
       (expect Register Reply)
     */
    status = sii_ipc_send_buffer_command(inst, command, false);
    if (status != SII_IPC_STATUS_SUCCESS) {
        goto exit;
    }

    ipc_host_context[inst].host_sm = SII_IPC_HOST_SM_WAIT_REPLY;

    status = sii_ipc_waiting_reply(inst);
    if( status == SII_IPC_STATUS_SUCCESS ){
        sii_ipc_host_handler(inst);
    }
    else{
        pr_err("%s:%d IPC POLLING TIMEOUT *** \n", __func__, __LINE__);
    }

    clear_interrupt = true;

    /* Receive the Reply (Register Reply) */
    status = sii_ipc_receive_register_reply(inst, reply);
    if (status != SII_IPC_STATUS_SUCCESS) {
        goto exit;
    }

    /* FD: TBI, Stop Send Command Timer */

exit:
    ipc_host_context[inst].host_sm = SII_IPC_HOST_SM_IDLE;

    if (true == clear_interrupt) {
        status2 = sii_ipc_host_clear_and_enable_interrupt(inst);
        if (status2 != SII_IPC_STATUS_SUCCESS) {
            status = status2;
        }
    }

    /* FD: TBI, Stop Send Command Timer if there */

    return status;
}

static enum sii_ipc_status check_message(uint8_t protocol_version,
        uint8_t msg_type, uint16_t msg_id, struct sii_ipc_message *msg)
{
    enum sii_ipc_status ipc_status = SII_IPC_STATUS_SUCCESS;
    if (protocol_version != msg->protocol_version)
        ipc_status = SII_IPC_STATUS_FAILURE;
    else if (msg_type != msg->message_payload.message_type)
        ipc_status = SII_IPC_STATUS_FAILURE;
    else if (msg_id != ((msg->message_payload.message_id_l) |
            (((uint16_t) msg->message_payload.message_id_h) << 8)))
        ipc_status = SII_IPC_STATUS_FAILURE;
    return ipc_status;
}

static enum sii_ipc_status sii_check_message_validity(uint8_t protocol_version,
        uint8_t msg_type, uint16_t msg_id, struct sii_ipc_message *msg)
{
    uint16_t msg_length = msg->message_length_l |
            ((uint16_t) msg->message_length_h) << 8;
    if (!verify_checksum_byte((uint8_t *) msg, msg_length))
        return SII_IPC_STATUS_ERROR_MESSAGE_CHECKSUM;
    if (SII_IPC_STATUS_SUCCESS != check_message(
                                                protocol_version,
                                                msg_type, msg_id, msg))
        return SII_IPC_STATUS_ERROR_MESSAGE;
    return SII_IPC_STATUS_SUCCESS;
}

static enum sii_ipc_api_status sii_api_reg_cmd_msg_reply_process(
        sii_inst_t inst,
        struct sii_ipc_register_command command,
        struct sii_ipc_message_reply *reply)
{
    enum sii_ipc_api_status api_status = SII_IPC_API_SUCCESS;
    enum sii_ipc_status ipc_status = SII_IPC_STATUS_SUCCESS;

    ipc_status = sii_execute_register_command_message_reply(inst,
                                                            command,
                                                            reply);
    if (ipc_status != SII_IPC_STATUS_SUCCESS) {
        api_status = SII_IPC_API_FAILURE;
        //last_err = SII_IPC_API_ERR__INTERNAL;
        goto exit;
    }

    if (reply->reply_status != SII_IPC_PHY_REPLY_STATUS_SUCCESS) {
        api_status = SII_IPC_API_FAILURE;
        /* Save api error code */
        //last_err = reply->reply_return;
    } else {
        ipc_status = sii_check_message_validity(
        SII_IPC_MESSAGE_PROTOCOL_VERSION_1,
                                                SII_IPC_MESSAGE_TYPE_REPLY,
                                                command.opcode,
                                                reply->message);
        if (ipc_status == SII_IPC_STATUS_ERROR_MESSAGE_CHECKSUM) {
            api_status = SII_IPC_API_FAILURE;
            //last_err = SII_IPC_API_ERR__CHECKSUM;
        }
        else if (ipc_status == SII_IPC_STATUS_ERROR_MESSAGE) {
            api_status = SII_IPC_API_FAILURE;
            //last_err = SII_IPC_API_ERR__MSG_FORMAT;
        }
    }

exit:
    sii_ipc_display_error(ipc_status);
    return api_status;
}


enum sii_ipc_api_status sii_host_get_chip_id(
        sii_inst_t inst, struct sii_ipc_message *msg)
{
    struct sii_ipc_register_command command;
    struct sii_ipc_message_reply reply;

    command.opcode = SII_IPC_PHY_CMD_OPCODE_GET_CHIP_ID;
    command.parameter = 0x00;
    reply.message = msg;

    return sii_api_reg_cmd_msg_reply_process(inst, command, &reply);
}

enum sii_api_status sii_get_chip_id(
        sii_inst_t inst, uint16_t *chip_id)
{
    enum sii_api_status api_status = SII_API_SUCCESS;
    enum sii_ipc_api_status ipc_status = SII_IPC_API_SUCCESS;
    struct sii_ipc_message *msg = &ipc_host_context->host_message;
    uint8_t chip_id_l, chip_id_h;

    ipc_status = sii_host_get_chip_id(inst, msg);
    if (ipc_status != SII_IPC_API_SUCCESS) {
        *chip_id = 0;
        api_status = SII_API_FAILURE;
    } else {
        chip_id_l = msg->message_payload.message_data[0];
        chip_id_h = msg->message_payload.message_data[1];
        *chip_id = (((uint16_t) chip_id_h) << 8) | chip_id_l;
    }

    return api_status;
}



enum sii_ipc_api_status sii_host_get_firmware_version(
        sii_inst_t inst,
        struct sii_ipc_message *msg)
{
    struct sii_ipc_register_command command;
    struct sii_ipc_message_reply reply;

    command.opcode = SII_IPC_PHY_CMD_OPCODE_GET_FW_VERSION;
    command.parameter = 0x00;
    reply.message = msg;

    return sii_api_reg_cmd_msg_reply_process(inst, command, &reply);
}


enum sii_api_status sii_get_firmware_version(
        sii_inst_t inst, uint8_t *fw_ver)
{
    enum sii_api_status api_status = SII_API_SUCCESS;
    enum sii_ipc_api_status ipc_status = SII_IPC_API_SUCCESS;
    struct sii_ipc_message *msg = &ipc_host_context->host_message;

    ipc_status = sii_host_get_firmware_version(inst, msg);
    if (ipc_status != SII_IPC_API_SUCCESS) {
        fw_ver[0] = '\0';
        api_status = SII_API_FAILURE;
    } else {
        strcpy((char*)fw_ver,
                (char*)msg->message_payload.message_data);
    }

    return api_status;
}

static bool sii_is_boot_done(sii_inst_t inst)
{
    enum sii_product_id product_id   = SII_PRODUCT_ID__NONE;
    enum sii_boot_status boot_status = SII_BOOT_STAT__SUCCESS;
    uint16_t chip_id = 0;
    uint8_t fw_version[32];

    if (sii_wait_for_boot_done(inst) != SII_API_SUCCESS) {
        pr_err("9396 Error: Boot Failed\n");
        return false;
    }
    else {
        sii_get_boot_status(inst, &boot_status);
        if (SII_BOOT_STAT__SUCCESS != boot_status) {
            pr_err("Error: Boot Failed\n");
            return false;
        }
        sii_get_chip_id(inst, &chip_id);
        pr_err("Chip ID:  0x%x\n\n", chip_id);
        if (chip_id != 0x9396)
       {
            pr_err("Error: Chip ID not match, require Chip ID: 0x%x\n", chip_id);
            return false;
        }
        sii_get_product_id(inst, &product_id);
        sii_get_firmware_version(inst, fw_version);
        sii_log_debug("Chip FW Version:  %s .\n", fw_version);
        sii_set_rx_hdcp_repeater_mode(inst, true);
        sii_set_host_mode(inst, false);
        sii_log_debug("Set to hostmode false, set to repeater mode\n");

    }
    return true;
}


enum sii_ipc_status sii_ipc_host_pop_queue_notification(
        sii_inst_t inst, struct sii_ipc_notification *notification) {
    enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;

    if (0 >= ipc_host_context[inst].notification_queue_count) {
        status = SII_IPC_STATUS_ERROR_NOTIFICATION_QUEUE_EMPTY;
        goto exit;
    }

    memcpy(notification,
        &(ipc_host_context[inst].notification_queue
            [ipc_host_context[inst].notification_queue_head]),
        sizeof(*notification));

    ipc_host_context[inst].notification_queue_count--;

    ipc_host_context[inst].notification_queue_head =
        (ipc_host_context[inst].notification_queue_head + 1)
        % SII_IPC_NOTIFICATION_QUEUE_LENGTH;

exit:
    return status;
}


static bool event_filter(sii_inst_t inst, uint8_t event)
{
    uint8_t index = (uint8_t) inst;
    uint32_t event_flag = 1 << event;
    if (event_flag & api_host_context.instance[index].event_mask)
        return false;
    return true;
}



enum sii_ipc_status sii_execute_message_command_register_reply(
        sii_inst_t inst, struct sii_ipc_message_command *message_command,
        struct sii_ipc_register_reply *reply) {

    enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;
    struct sii_ipc_packet_command packet_command;

    packet_command.packet = &(ipc_host_context[inst].host_packet);

    /*
       Prepare the Packet Command for Sending
     */
    ipc_host_context[inst].packet_sequence = 0;
    ipc_host_context[inst].packet_fragment = 0;

    /* Send Packet Command One by One if Necessary */
    do {
        /* Compose Packet Command from Message Command */
        sii_compose_packet_command_from_message_command(inst,
                &packet_command, message_command,
                ipc_host_context[inst].packet_sequence);

        status = sii_execute_buffer_command_register_reply(inst,
                &packet_command, reply);
        if (status != SII_IPC_STATUS_SUCCESS) {
            goto exit;
        }

        if (reply->reply_status != SII_IPC_PHY_REPLY_STATUS_ACK) {
            /* Successful Single-Buffer Command, or
               NACK/FAILURE to Multi-Buffer Command */
            goto exit;
        }

        /* Prepare the Next Packet Command for Sending */
        (ipc_host_context[inst].packet_sequence)++;

        msleep(1);
    } while (ipc_host_context[inst].packet_sequence <
            ipc_host_context[inst].packet_fragment);

exit:
    return status;
}


//RG start API definitions

static enum sii_ipc_api_status sii_api_msg_cmd_reg_reply_process(
        sii_inst_t inst,
        struct sii_ipc_message_command command,
        struct sii_ipc_register_reply *reply)
{
    enum sii_ipc_api_status api_status = SII_IPC_API_SUCCESS;
    enum sii_ipc_status ipc_status = SII_IPC_STATUS_SUCCESS;
    uint16_t raw_msg_length = (command.message->message_length_l) |
            (((uint16_t) (command.message->message_length_h)) << 8);
    sii_compose_message(
    SII_IPC_MESSAGE_PROTOCOL_VERSION_1,
                        SII_IPC_MESSAGE_TYPE_COMMAND,
                        command.opcode,
                        raw_msg_length,
                        command.message);
    ipc_status = sii_execute_message_command_register_reply(inst,
                                                            &command,
                                                            reply);
    if (ipc_status != SII_IPC_STATUS_SUCCESS) {
        api_status = SII_IPC_API_FAILURE;
        //last_err = SII_IPC_API_ERR__INTERNAL;
        goto exit;
    }

    if (reply->reply_status != SII_IPC_PHY_REPLY_STATUS_SUCCESS) {
        api_status = SII_IPC_API_FAILURE;
        /* Save api error code */
        //last_err = reply->reply_return;
    }
exit:
    sii_ipc_display_error(ipc_status);
    return api_status;
}

#ifndef __REMOVE_UNNECESSARY_FUNC__
enum sii_ipc_api_status sii_host_set_rx_edid_content(
        sii_inst_t inst,
        uint8_t blocknum, struct sii_ipc_message *msg)
{
    struct sii_ipc_message_command command;
    struct sii_ipc_register_reply reply;

    command.opcode = SII_IPC_PHY_CMD_OPCODE_SET_RX_EDID_CONTENT;
    command.parameter = blocknum;
    command.message = msg;

    return sii_api_msg_cmd_reg_reply_process(inst, command, &reply);
}


enum sii_api_status sii_set_rx_edid_content(
        sii_inst_t inst,
        uint8_t block_num, uint8_t *edid_buffer)
{
    enum sii_api_status api_status = SII_API_SUCCESS;
    enum sii_ipc_api_status ipc_status = SII_IPC_API_SUCCESS;
    struct sii_ipc_message *msg = &ipc_host_context->host_message;

    memset(msg, 0, sizeof(*msg));
    memcpy(msg->message_payload.message_data, edid_buffer, SII_EDID_BLOCK_SIZE);
    msg->message_length_l = SII_EDID_BLOCK_SIZE;
    msg->message_length_h = 0;

    ipc_status = sii_host_set_rx_edid_content(inst, block_num, msg);
    if (ipc_status != SII_IPC_API_SUCCESS)
        api_status = SII_API_FAILURE;

    return api_status;
}
#endif

enum sii_ipc_api_status sii_host_get_rx_hdcp_status(
        sii_inst_t inst,
        struct sii_ipc_message *msg)
{
    struct sii_ipc_register_command command;
    struct sii_ipc_message_reply reply;

    command.opcode = SII_IPC_PHY_CMD_OPCODE_GET_RX_HDCP_STATUS;
    command.parameter = 0x00;
    reply.message = msg;

    return sii_api_reg_cmd_msg_reply_process(inst, command, &reply);
}


enum sii_api_status sii_get_rx_hdcp_status(
        sii_inst_t inst, enum sii_hdcp_status *status)
{
    enum sii_api_status api_status = SII_API_SUCCESS;
    enum sii_ipc_api_status ipc_status = SII_IPC_API_SUCCESS;
    struct sii_ipc_message *msg = &ipc_host_context->host_message;

    ipc_status = sii_host_get_rx_hdcp_status(inst, msg);
    if (ipc_status != SII_IPC_API_SUCCESS) {
        *status = (enum sii_hdcp_status) 0;
        api_status = SII_API_FAILURE;
    } else {
        *status = (enum sii_hdcp_status) msg->message_payload.message_data[0];
    }

    return api_status;
}
#ifndef __REMOVE_UNNECESSARY_FUNC__
enum sii_ipc_api_status sii_host_get_rx_info_frame(
        sii_inst_t inst,
        uint8_t type, struct sii_ipc_message *msg)
{
    struct sii_ipc_register_command command;
    struct sii_ipc_message_reply reply;

    command.opcode = SII_IPC_PHY_CMD_OPCODE_GET_RX_INFOFRAME;
    command.parameter = type;
    reply.message = msg;

    return sii_api_reg_cmd_msg_reply_process(inst, command, &reply);
}

enum sii_api_status sii_get_rx_info_frame(
        sii_inst_t inst, enum sii_infoframe_type type,
        uint8_t *infoframe, uint8_t *size)
{
    enum sii_api_status api_status = SII_API_SUCCESS;
    enum sii_ipc_api_status ipc_status = SII_IPC_API_SUCCESS;
    struct sii_ipc_message *msg = &ipc_host_context->host_message;

    ipc_status = sii_host_get_rx_info_frame(inst, type, msg);
    if (ipc_status != SII_IPC_API_SUCCESS) {
        api_status = SII_API_FAILURE;
    } else {
        *size = msg->message_payload.message_data[0];
        memcpy(infoframe,
                &(msg->message_payload.message_data[1]),
                *size);
    }
    return api_status;
}

enum sii_api_status sii_get_rx_info_frame_hdr(
        sii_inst_t inst,
        uint8_t *infoframe, uint8_t *size)
{

    return sii_get_rx_info_frame(inst, SII_INFOFRAME__HDR, infoframe, size);
}

enum sii_api_status sii_get_rx_info_frame_vsif(
        sii_inst_t inst,
        uint8_t *infoframe, uint8_t *size)
{

    return sii_get_rx_info_frame(inst, SII_INFOFRAME__VSIF, infoframe, size);
}

enum sii_api_status sii_get_rx_info_frame_avi(
        sii_inst_t inst,
        uint8_t *infoframe, uint8_t *size)
{

    return sii_get_rx_info_frame(inst, SII_INFOFRAME__AVI, infoframe, size);
}

enum sii_api_status sii_get_rx_info_frame_aif(
        sii_inst_t inst,
        uint8_t *infoframe, uint8_t *size)
{

    return sii_get_rx_info_frame(inst, SII_INFOFRAME__AIF, infoframe, size);
}

enum sii_ipc_api_status sii_host_get_rx_video_info(
        sii_inst_t inst,
        struct sii_ipc_message *msg)
{
    struct sii_ipc_register_command command;
    struct sii_ipc_message_reply reply;

    command.opcode = SII_IPC_PHY_CMD_OPCODE_GET_RX_VIDEO_INFO;
    command.parameter = 0x00;
    reply.message = msg;

    return sii_api_reg_cmd_msg_reply_process(inst, command, &reply);
}


enum sii_api_status sii_get_rx_video_info(
        sii_inst_t inst, struct sii_video_info *info)
{
    enum sii_api_status api_status = SII_API_SUCCESS;
    enum sii_ipc_api_status ipc_status = SII_IPC_API_SUCCESS;
    struct sii_ipc_message *msg = &ipc_host_context->host_message;

    ipc_status = sii_host_get_rx_video_info(inst, msg);
    if (ipc_status != SII_IPC_API_SUCCESS) {
        api_status = SII_API_FAILURE;
    } else {
        memcpy(info, msg->message_payload.message_data, sizeof(*info));
    }
    return api_status;
}

static void sii_show_video_info(sii_inst_t inst)
{
    //uint8_t frame_rate = 0;
    sii_get_rx_video_info(inst, &video_info);

#if 0
    if (video_info.v_res_total != 0 && video_info.h_res_total != 0) {
        frame_rate =
            (video_info.tmds_clock * 1000000 / video_info.color_depth * 8) /
            (video_info.h_res_total * video_info.v_res_total);
        //frame_rate = (uint8_t)(frame_rate / 5) * 5;
        if (video_info.color_space == SII_COLOR_SPACE__YCBCR420)
            frame_rate *= 2;
        if (video_info.interlace)
            frame_rate *= 2;
    }
    sii_log_debug("Total Resolution: %d x % d %s\n", video_info.h_res_total,
                                        video_info.v_res_total,
                                        video_info.interlace ? "I" : "P");

    sii_log_debug("Active Resolution: %d x % d %s %dHz %d bits\n", video_info.h_res_active,
                                        video_info.v_res_active,
                                        video_info.interlace ? "I" : "P",
                                        frame_rate,
                                        video_info.color_depth);

//  pr_err("%dHz\t", frame_rate);

//  pr_err("%d bits\t", video_info.color_depth);

    switch (video_info.color_space) {
        case SII_COLOR_SPACE__RGB:
            sii_log_debug("RGB\n");
            break;
        case SII_COLOR_SPACE__YCBCR444:
            sii_log_debug("444\n");
            break;
        case SII_COLOR_SPACE__YCBCR422:
            sii_log_debug("422\n");
            break;
        case SII_COLOR_SPACE__YCBCR420:
            sii_log_debug("420\n");
            break;
        case SII_COLOR_SPACE__UNKNOWN:
        default:
            sii_log_debug("Color Space Unknown\n");
            break;
    }
    sii_log_debug("TMDS Clock: %d MHz\t", video_info.tmds_clock);
    if (video_info.ratio)
        sii_log_debug("ratio: 1/40\n");
    else
        sii_log_debug("ratio: 1/10\n");

#endif
}

#endif
enum sii_ipc_api_status sii_host_get_rx_hdcp_stream_manage_info(
        sii_inst_t inst,
        struct sii_ipc_message *msg)
{
    struct sii_ipc_register_command command;
    struct sii_ipc_message_reply reply;

    command.opcode = SII_IPC_PHY_CMD_OPCODE_GET_RX_HDCP_STREAM_MANAGE_INFO;
    command.parameter = 0x00;
    reply.message = msg;

    return sii_api_reg_cmd_msg_reply_process(inst, command, &reply);
}

enum sii_api_status sii_get_rx_hdcp_stream_manage_info(
        sii_inst_t inst,
        struct sii_hdcp_stream_manage_info *info)
{
    enum sii_api_status api_status = SII_API_SUCCESS;
    enum sii_ipc_api_status ipc_status = SII_IPC_API_SUCCESS;
    struct sii_ipc_message *msg = &ipc_host_context->host_message;

    ipc_status = sii_host_get_rx_hdcp_stream_manage_info(inst, msg);
    if (ipc_status != SII_IPC_API_SUCCESS) {
        api_status = SII_API_FAILURE;
    } else {
        memcpy(info,
                msg->message_payload.message_data,
                sizeof(struct sii_hdcp_stream_manage_info));
    }

    return api_status;
}

enum sii_ipc_api_status sii_host_set_rx_hdcp_topology(
        sii_inst_t inst,
        struct sii_ipc_message *msg)
{
    struct sii_ipc_message_command command;
    struct sii_ipc_register_reply reply;

    command.opcode = SII_IPC_PHY_CMD_OPCODE_SET_RX_HDCP_TOPOLOGY;
    command.parameter = 0x00;
    command.message = msg;

    return sii_api_msg_cmd_reg_reply_process(inst, command, &reply);
}


enum sii_api_status sii_set_rx_hdcp_topology(
        sii_inst_t inst,
        struct sii_hdcp_topology *topology)
{
    enum sii_api_status api_status = SII_API_SUCCESS;
    enum sii_ipc_api_status ipc_status = SII_IPC_API_SUCCESS;
    struct sii_ipc_message *msg = &ipc_host_context->host_message;

    memset(msg, 0, sizeof(*msg));
    memcpy(&msg->message_payload.message_data[0], topology, sizeof(*topology));
    msg->message_length_l = sizeof(*topology);
    msg->message_length_h = 0;

    ipc_status = sii_host_set_rx_hdcp_topology(inst, msg);
    if (ipc_status != SII_IPC_API_SUCCESS)
        api_status = SII_API_FAILURE;

    return api_status;
}

enum sii_ipc_api_status sii_host_set_rx_hdcp_rxid_list(
        sii_inst_t inst,
        struct sii_ipc_message *msg)
{
    struct sii_ipc_message_command command;
    struct sii_ipc_register_reply reply;

    command.opcode = SII_IPC_PHY_CMD_OPCODE_SET_RX_HDCP_RXID_LIST;
    command.parameter = 0x00;
    command.message = msg;

    return sii_api_msg_cmd_reg_reply_process(inst, command, &reply);
}

enum sii_api_status sii_set_rx_hdcp_rxid_list(
        sii_inst_t inst, uint8_t device_count,
        struct sii_hdcp_ksv *rxid_list)
{
    enum sii_api_status api_status = SII_API_SUCCESS;
    enum sii_ipc_api_status ipc_status = SII_IPC_API_SUCCESS;
    struct sii_ipc_message *msg = &ipc_host_context->host_message;
    uint16_t msg_len;

    memset(msg, 0, sizeof(*msg));
    msg->message_payload.message_data[0] = device_count;
    memcpy(&msg->message_payload.message_data[1], rxid_list,
            device_count * sizeof(struct sii_hdcp_ksv));
    msg_len = device_count * sizeof(struct sii_hdcp_ksv) + 1;
    msg->message_length_l = (uint8_t) (msg_len & 0xFF);
    msg->message_length_h = msg_len >> 8;

    ipc_status = sii_host_set_rx_hdcp_rxid_list(inst, msg);
    if (ipc_status != SII_IPC_API_SUCCESS)
        api_status = SII_API_FAILURE;

    return api_status;
}

void sii_drv_spiderman_init(pstdriver_obj_t drv_obj)
{
    l_drv_context = drv_obj;
}
static void sii_hdcp_enable(bool enable)
{
    bool last_enable;
    last_enable = rx_hdcp1_data.auth_req_rcvd_frm_spiderman;

#ifdef DEBUG_BANNER_HDCP_CONVERTER_ISSUE
	enum sii_hdcp_version source_hdcp_version = SII_HDCP_VERSION__NONE;

	sii_get_rx_hdcp_us_version(ex_sp_pdata->rx_inst, &source_hdcp_version);
#endif

    if( enable == false )
    {
        /*
        * Onkyo reported connectivity issue with the connection below
        *   K8500 - Onkyo AVR - HDBT - TV
        * And change input connection of AVR from K8500 to no input, then switch back to K8500.
        * Looking at the DDC output of Banner, HDCP2.2 auth is canceled in the middle of the CertRx
        * receiving condition.
        * after that, all of DDC accesses, such as SCDC for EMC, master DDC for HDCP2 capability check
        * and HDCP1 auth, are failed due to wrong value read from the DDC FIFO.
        * Below is the trial of ignoring of disabling HDCP2 when it is not receiving certRx.
        * certRx must be received within 100ms from HDCP starts, but not sure when the HDCP starts from
        * Spiderman driver side.
        *
        * The immediate disabling of HDCP right after enabling will happen only when the video is changed
        * right after HDCP starts. So, even HDCP reauth will happen later by the driver regardless of the
        * API command sent from Spiderman side or not. Ignoring the disabling shouldn't introduce
        * other regressions. Let's see if it is true...
        */
        enum sii_drv_hdcp_link_version     ds_hdcp_ver;
        sii_drv_tx_hdcp_version_get((void *)l_drv_context, &ds_hdcp_ver);
        if( (ds_hdcp_ver != SII_DRV_HDCP_LINK_VER__HDCP22 )
            || (tx_hdcp2_data.bHdcp2_CertRx_done == 1)
            )
        {
            sii_drv_tx_hdcp_protection_set((void *)l_drv_context, SII_DRV_HDCP_LINK_VER__NONE);
            rx_hdcp1_data.ds_hdcp_start_time = 0;
            tx_hdcp2_data.bHdcp2_CertRx_done = 0;
            tx_hdcp2_data.StreamManageInfoDs.seq_num_m = 0;
            tx_hdcp2_data.StreamManageInfoDs.content_type = 0;
            sii_log_debug("HDCP DISABLED ***\n");
        }
    }
    else if( last_enable != enable )
    {
#ifdef DEBUG_BANNER_HDCP_CONVERTER_ISSUE
    	if(source_hdcp_version == SII_HDCP_VERSION__1X)
            sii_drv_tx_hdcp_protection_set((void *)l_drv_context, SII_DRV_HDCP_LINK_VER__HDCP1X);
		else
		    sii_drv_tx_hdcp_protection_set((void *)l_drv_context, SII_DRV_HDCP_LINK_VER__AUTO);
#else
		sii_drv_tx_hdcp_protection_set((void *)l_drv_context, SII_DRV_HDCP_LINK_VER__AUTO);
#endif
        rx_hdcp1_data.ds_hdcp_start_time = sii_get_milli_sec();
        sii_log_debug("HDCP STARTED ***\n");
    }
    else
    {
        uint32_t t = sii_get_milli_sec();
        // Toggle HDCP auth if auth request is new (time between this and last is > 500ms).
        // Addresses HDCP2.2 3C-1-03, 3C-1-04
        if( t >  rx_hdcp1_data.ds_hdcp_start_time + 500 )
        {


            sii_drv_tx_hdcp_protection_set((void *)l_drv_context, SII_DRV_HDCP_LINK_VER__NONE);
#ifdef DEBUG_BANNER_HDCP_CONVERTER_ISSUE
			if(source_hdcp_version == SII_HDCP_VERSION__1X)
				sii_drv_tx_hdcp_protection_set((void *)l_drv_context, SII_DRV_HDCP_LINK_VER__HDCP1X);
			else
				sii_drv_tx_hdcp_protection_set((void *)l_drv_context, SII_DRV_HDCP_LINK_VER__AUTO);
#else
			sii_drv_tx_hdcp_protection_set((void *)l_drv_context, SII_DRV_HDCP_LINK_VER__AUTO);
#endif
            rx_hdcp1_data.ds_hdcp_start_time = sii_get_milli_sec();
            sii_log_debug("HDCP STARTED AGAIN***\n");
        }
        else
        {
            sii_log_debug("***HDCP auth requested but canceled: it's already started ***\n");
            rx_hdcp1_data.ds_hdcp_start_time = sii_get_milli_sec();
        }
    }
	rx_hdcp1_data.auth_req_rcvd_frm_spiderman = enable;
}

static void sii_us_send_topology(enum hdcp_repeater_mode  ds_repeater_mode)
{
	if (
		(rx_hdcp1_data.spiderman_rdy == true)
		&& (rx_hdcp1_data.hdcp_data_rdy == true)
		)
	{
        if( rx_hdcp1_data.hdcp_data_sent == false )
        {
    		rx_hdcp1_data.hdcp_data_sent = true;
    		sii_log_debug(" S+B Ready\n");
    		sii_set_rx_hdcp_topology(0, &(rx_hdcp1_data.topology_us));
    		sii_set_rx_hdcp_rxid_list(0, rx_hdcp1_data.topology_us.device_count, rx_hdcp1_data.ksv_list);
    		sii_trigger_rx_hdcp_rxid_list_to_send(0);
    		sii_log_debug(" HDCP data Sent\n");
        }
		if (
			(ds_repeater_mode == TX_HDCP_RPTR_22)
			&& (
			(rx_hdcp1_data.us_hdcp_ver == SII_HDCP_VERSION__1X)
				|| (tx_hdcp2_data.bForceStreamManage == true)
				|| (tx_hdcp2_data.StreamManageInfoDs.seq_num_m != 0)
				)
			)
		{
			tx_hdcp2_data.bForceStreamManage = false;
			sii_log_debug(" Send SMNG\n");
			sii_drv_tx_hdcp_stream_manage_msg_set((void *)l_drv_context, &(tx_hdcp2_data.StreamManageInfoDs));
			tx_hdcp2_data.StreamManageInfoDs.seq_num_m++;
		}
	}
}

static void sii_rx_hdcp_event_handler(sii_inst_t inst, void* p_obj, uint8_t event)
{
    struct sii_drv_hdcp_ksv_list ksv_list;
    uint8_t l_u8list_start[(6 * 32)];
    ksv_list.p_list_start = l_u8list_start;//(uint8_t *)vmalloc(6 * 32);

    memset(l_u8list_start,0, (6*32));
    ksv_list.p_list = ksv_list.p_list_start;
    if (ksv_list.p_list_start == NULL) {
        pr_err("Banner: Failed to allocate memory for Bksv/RcvId list\n");
    }

    if (event == SII_EVENT__RX_HDCP_AUTH_START) {
        enum sii_hdcp_version source_hdcp_version = SII_HDCP_VERSION__NONE;

		sii_get_rx_hdcp_us_version(inst, &source_hdcp_version);
		/*Sometimes SA is not reciving AUTH_DONE interrupt even though first stage
		*authentication is done. So considering this event as first stage of
		*authentication. So make spiderman_rdy flag true for US HDCP1.4 case*/
		if (SII_HDCP_VERSION__1X == source_hdcp_version)
			rx_hdcp1_data.spiderman_rdy = true;
		else
			rx_hdcp1_data.spiderman_rdy = false;

        sii_hdcp_enable(true);

		tx_hdcp2_data.bForceStreamManage = false;
        sii_log_debug(" >> HDCP_RX_STATUS_AUTH_START\n");
        rx_hdcp2_data.topology_us.seq_num_v = 0;
        rx_hdcp2_data.info_us.seq_num_m = 0;
        rx_hdcp2_data.info_us.stream_id_type[0] = 0;
        rx_hdcp1_data.hdcp_data_rdy = false;
        rx_hdcp1_data.hdcp_data_sent = false;

#ifndef __REMOVE_UNNECESSARY_FUNC__
	/*Calling if sii_get_rx_hdcp_us_version is just
	 *to print the status, So commenting as of now*/
        sii_get_rx_hdcp_us_version(inst, &source_hdcp_version);
        if (SII_HDCP_VERSION__1X == source_hdcp_version)
            sii_log_debug(" >> HDCP1_RX_STATUS_AUTH_DONE\n");
#endif

    }  // RX_AUTH_START

    if (event == SII_EVENT__RX_HDCP_AUTH_DONE) {
#ifndef __REMOVE_UNNECESSARY_FUNC__
        enum sii_hdcp_version source_hdcp_version = SII_HDCP_VERSION__NONE;
        // enum sii_hdcp_version sink_hdcp_version = SII_HDCP_VERSION__NONE;
        // get upstream device hdcp version
        sii_get_rx_hdcp_us_version(inst, &source_hdcp_version);
#endif
		enum hdcp_repeater_mode            ds_repeater_mode;
		enum sii_drv_hdcp_link_version     ds_hdcp_ver;
		enum sii_drv_hdcp_status           ds_hdcp_status;
		/*Make spiderman ready flag true,
		*saying that SA is done with first stage of authentication*/
		rx_hdcp1_data.spiderman_rdy = true;;

		sii_drv_tx_hdcp_status_get((void *)l_drv_context, &ds_hdcp_status);
		sii_drv_tx_hdcp_version_get((void *)l_drv_context, &ds_hdcp_ver);
		sii_drv_tx_hdcp_repeater_info((void *)l_drv_context, &ds_repeater_mode);
		if (
			(ds_hdcp_status == SII_DRV_HDCP_STATUS__SUCCESS_1X)
			|| (ds_hdcp_status == SII_DRV_HDCP_STATUS__SUCCESS_22)
			|| ((ds_hdcp_status == SII_DRV_HDCP_STATUS__AUTHENTICATING) && (ds_hdcp_ver == SII_DRV_HDCP_LINK_VER__HDCP22))
			)
		{
			sii_us_send_topology(ds_repeater_mode);
		}
        rx_hdcp1_data.ds_hdcp_start_time = 0;
    }  // RX_AUTH_DONE
    if (event == SII_EVENT__RX_HDCP_RCVID_DONE) {
        enum sii_hdcp_version source_hdcp_version = SII_HDCP_VERSION__NONE;

        sii_get_rx_hdcp_us_version(inst, &source_hdcp_version);

        if (SII_HDCP_VERSION__1X == source_hdcp_version) {
            sii_log_debug(">>ERROR<< [US] HDCP1 NEVER COMES RCVID_DONE!\n");
        } else if (SII_HDCP_VERSION__22 == source_hdcp_version) {
            rx_hdcp2_data.topology_us.seq_num_v++;
            sii_log_debug("     Rx HDCP2 Seq Num : %d\n",
                    rx_hdcp2_data.topology_us.seq_num_v);
            sii_log_debug(" >> HDCP2_RX_STATUS_RCVID_DONE\n");
        }
    }  // RX_RCVID_DONE

    if (event == SII_EVENT__RX_HDCP_SMNG_CHNG) {
        enum sii_hdcp_version SourceHdcpVersion = SII_HDCP_VERSION__NONE;
        enum sii_drv_hdcp_link_version SinkHdcpVersion = SII_DRV_HDCP_LINK_VER__NONE;
        sii_get_rx_hdcp_stream_manage_info(inst, &(rx_hdcp2_data.info_us));

        tx_hdcp2_data.StreamManageInfoDs.stream_k = (uint8_t)rx_hdcp2_data.info_us.k;
        //tx_hdcp2_data.StreamManageInfoDs.seq_num_m = rx_hdcp2_data.info_us.seq_num_m;
        tx_hdcp2_data.StreamManageInfoDs.stream_i_d = (uint8_t)((rx_hdcp2_data.info_us.stream_id_type[0]&0xFF00)>>8);
        tx_hdcp2_data.StreamManageInfoDs.content_type = (uint8_t)(rx_hdcp2_data.info_us.stream_id_type[0]&0x00FF);
        // HDCP spec allows to send SMNG at anytime after SKE
        // However, our HDCP Tx doesn't allow to send SMNG before RxID list
        // So, it is possible to fail sending SMNG here
        // SMNG will be sent at the time of RxID complete in that case.
        tx_hdcp2_data.bForceStreamManage = rx_hdcp1_data.hdcp_data_sent ? false : true;

        // get upstream device hdcp version
        sii_get_rx_hdcp_us_version(inst, &SourceHdcpVersion);

        sii_drv_tx_hdcp_version_get((void *)l_drv_context, &SinkHdcpVersion);

        if ( SII_HDCP_VERSION__1X == SourceHdcpVersion ) {
            sii_log_debug(">>ERROR<< [US] HDCP1 NEVER COMES SMNG_CHG!\n");
        }
        else if ( SII_HDCP_VERSION__22 == SourceHdcpVersion ) {
            sii_log_debug("    >> HDCP2_RX_STATUS_SMNG_CHG\n");

            if ( SII_DRV_HDCP_LINK_VER__HDCP1X == SinkHdcpVersion )
            {
                // UpStream HDCP2.2, DownStream HDCP1.x.
                sii_log_debug("        (2.2 to 1.x Converter)\n");

                // read RepeaterAuth_Stream_Manage Message
                if ((rx_hdcp2_data.info_us.stream_id_type[0]&0x00FF) > 0)
                {
                    sii_log_debug("        content type =1, video should not output to HDCP1.x sink\n");
                    sii_drv_tx_av_mute_set((void *)l_drv_context, true);
                }
                else
                {
                    sii_drv_tx_av_mute_set((void *)l_drv_context, false);
                }
            }
            else if ( SII_DRV_HDCP_LINK_VER__HDCP22 == SinkHdcpVersion )
            {
                // UpStream HDCP2.2, DownStream HDCP2.2.
                sii_log_debug("        (2.2 Repeater)\n");
                sii_log_debug("        DS HDCP2: %s\n", (tx_hdcp2_data.bIsDsRepeater == TX_HDCP_RPTR_NONE) ? "Receiver" : "Repeater");

                if(tx_hdcp2_data.bIsDsRepeater == TX_HDCP_RPTR_22)
                {
                    // read RepeaterAuth_Stream_Manage Message and send out
                    sii_log_debug("        About to send stream mgmt message\n");

                    if( rx_hdcp1_data.hdcp_data_sent == true )
                    {
                        sii_drv_tx_hdcp_stream_manage_msg_set((void *)l_drv_context,&(tx_hdcp2_data.StreamManageInfoDs));
                        tx_hdcp2_data.StreamManageInfoDs.seq_num_m++;
                    }
               }

            }

        }
    }  // RX_SMNG_CHG

    if (event == SII_EVENT__RX_HDCP_AUTH_FAIL) {
        enum sii_hdcp_status HdcpStatus;
        sii_get_rx_hdcp_status(inst, &HdcpStatus);
        switch (HdcpStatus) {
        case SII_HDCP_STATUS__OFF:
            sii_log_debug(" >> SII_DRV_ADAPTER_HDCP_RX_STATUS__OFF\n");
            //SiiDrvAdaptTxHdcpProtectionSet(instCurrentTx, SII_DRV_ADAPTER_HDCP_PROTECT__OFF);
            break;
        case SII_HDCP_STATUS__FAILED:
            sii_log_debug(" >> SII_DRV_ADAPTER_HDCP_RX_STATUS__FAIL\n");
            break;
        default:
            sii_log_debug(" >> HdcpStatus Impossible\n");
            break;
        }
    }  // RX_AUTH_FAIL

    //vfree(ksv_list.p_list_start);
}

/* sii_tx_hdcp_event_handler() is called from Banner thread
 * So no need to consider isr_lock condition within this function
 * BUT, Should take sa_drviver_lock before doing calling any SA APIs.
 */
uint32_t sii_tx_hdcp_event_handler(uint32_t event_flags)
{
    uint32_t return_val = 0;
    struct sii_drv_hdcp_topology       ds_topology;
    struct sii_drv_hdcp_ksv_list       bksv_list;
    enum hdcp_repeater_mode            ds_repeater_mode;
    enum sii_drv_hdcp_link_version     ds_hdcp_ver;
    enum sii_drv_hdcp_status           ds_hdcp_status;

    //sii_log_debug(" Spiderman tx event handler [0x%x]\n", event_flags);
    // gather info first
    sii_drv_tx_hdcp_status_get((void *)l_drv_context, &ds_hdcp_status);
    sii_drv_tx_hdcp_version_get((void *)l_drv_context, &ds_hdcp_ver);
    sii_drv_tx_hdcp_repeater_info((void *)l_drv_context, &ds_repeater_mode);
    sii_drv_tx_hdcp_topology_get((void *)l_drv_context,&ds_topology);
    sii_drv_tx_hdcp_bksv_list_get((void *)l_drv_context, &bksv_list);
    if(event_flags & SII_DRV_HDCP_EVENT__HDCP_STATUS_CHNG){
        sii_log_debug(" SII_DRV_HDCP_EVENT__HDCP_STATUS_CHNG Status [%d]\n",ds_hdcp_status);
#if 0
        //sii_mod_tx_hdcp_get(p_hdcp_obj,SII_MOD_HDCP_OPCODE__VERSION, &version);

        if((p_hdcp_obj->auth_state == SII_MOD_HDCP_EVENT__AUTHENTICATED))
        {
            //rx_hdcp1_data.banner_rdy= true;

            if((rx_hdcp1_data.spiderman_rdy == true) && (rx_hdcp1_data.hdcp_data_sent == false) && (rx_hdcp1_data.hdcp_data_rdy == true))
            {
                rx_hdcp1_data.hdcp_data_sent = true;
                sii_log_debug(" S+B Ready\n");
                sii_set_rx_hdcp_topology(0, &(rx_hdcp1_data.topology_us));
                sii_set_rx_hdcp_rxid_list(0, rx_hdcp1_data.topology_us.device_count, rx_hdcp1_data.ksv_list);
                sii_trigger_rx_hdcp_rxid_list_to_send(0);
                sii_log_debug(" HDCP data Sent\n");
            }
        }
#endif
        //if((p_hdcp_obj->hdcp_status == SII_DRV_HDCP_STATUS__AUTHENTICATING) && (version == SII_DRV_HDCP_LINK_VER__HDCP22))
#if 0
        // sii_us_send_topology() should not be handled here
        // Tx will trigger interrupt after smng xfer done
        // this could become infinity loop in new sii_us_send_topology()
        // Just need to add sii_us_send_topology() in
        // BKSV change, SKE done
        // RCVID change, and BSTATUS change event
        if(
            (ds_hdcp_status == SII_DRV_HDCP_STATUS__SUCCESS_1X)
            || (ds_hdcp_status == SII_DRV_HDCP_STATUS__SUCCESS_22)
            || ((ds_hdcp_status == SII_DRV_HDCP_STATUS__AUTHENTICATING) && (ds_hdcp_ver == SII_DRV_HDCP_LINK_VER__HDCP22))
            )
        {
            //rx_hdcp1_data.banner_rdy= true;
            sii_us_send_topology(ds_repeater_mode);

        }
#endif
        if( (ds_hdcp_status == SII_DRV_HDCP_STATUS__OFF       ) ||
            (ds_hdcp_status == SII_DRV_HDCP_STATUS__FAILED    )
            )
        {
            tx_hdcp2_data.bHdcp2_CertRx_done = 0;
        }
        return_val |= SII_DRV_HDCP_EVENT__HDCP_STATUS_CHNG;
    }

    if(event_flags & SII_DRV_HDCP_EVENT__HDCP_BKSV_CHANGED){
        sii_log_debug(" SII_DRV_HDCP_EVENT__HDCP_BKSV_CHANGED\n");
        if( ds_topology.device_count == 0 )
        {
            ds_repeater_mode = TX_HDCP_RPTR_NONE;
        }
        sii_log_debug(" REPEATER MODE = [%d]\n",(int) ds_repeater_mode);

        //s_print_hdcp_status(p_hdcp_obj);

        //sii_log_debug(" ERROR START = [%d]\n",(int) p_hdcp_obj->error_state);
        //sii_log_debug(" AUTH STATE = [%d]\n",(int) p_hdcp_obj->auth_state);

        if(((ds_repeater_mode == TX_HDCP_RPTR_1X) && (ds_topology.device_count > 0)) || (ds_repeater_mode == TX_HDCP_RPTR_NONE)){
            sii_log_debug(" DS is REPEATER DEV CNT = %d\n",ds_topology.device_count);
            //convert topology info to Spiderman topology
            rx_hdcp1_data.topology_us.seq_num_v            =        ds_topology.seq_num_v;
            rx_hdcp1_data.topology_us.hdcp1_device_ds      = (bool) ds_topology.hdcp1x_device_ds;
            rx_hdcp1_data.topology_us.hdcp2_device_ds      = (bool) ds_topology.hdcp20_repeater_ds;
            rx_hdcp1_data.topology_us.max_cascade_exceeded = (bool) ds_topology.max_cascade_exceeded;
            rx_hdcp1_data.topology_us.max_device_exceeded  = (bool) ds_topology.max_devs_exceeded;
            rx_hdcp1_data.topology_us.device_count         =        ds_topology.device_count+1;
            rx_hdcp1_data.topology_us.depth                =        ds_topology.depth+1;

            rx_hdcp1_data.ksv_list = (struct sii_hdcp_ksv *)(bksv_list.p_list_start);

            if(rx_hdcp1_data.topology_us.device_count > HDCP1_MAX_DEVICE_COUNT)
            {
                sii_log_debug(" HDCP1_MAX_DEVICE_COUNT\n");
                rx_hdcp1_data.topology_us.max_device_exceeded = TRUE;
                //rx_hdcp1_data.hdcp_data_sent = false;
                //rx_hdcp1_data.banner_rdy = true;
                rx_hdcp1_data.hdcp_data_rdy = true;
                //rx_hdcp1_data.spiderman_rdy = true;
            }
            else if(rx_hdcp1_data.topology_us.depth > HDCP1_MAX_DEPTH)
            {
                sii_log_debug(" HDCP1_MAX_DEPTH\n");
                rx_hdcp1_data.topology_us.max_cascade_exceeded = TRUE;
                //rx_hdcp1_data.hdcp_data_sent = false;
                //rx_hdcp1_data.banner_rdy = true;
                rx_hdcp1_data.hdcp_data_rdy = true;
            }
            else
            {
                rx_hdcp1_data.hdcp_data_rdy = true;
                //rx_hdcp1_data.hdcp_data_sent = false;
            }

            /*sii_log_debug(" Banner TOP = Seq [%d] hdcp1 [%d] hdcp2 [%d] max_c [%d] max_d [%d] dev_cnt [%d] depth [%d] \n",rx_hdcp1_data.topology_us.seq_num_v,
            rx_hdcp1_data.topology_us.hdcp1_device_ds,
            rx_hdcp1_data.topology_us.hdcp2_device_ds,
            rx_hdcp1_data.topology_us.max_cascade_exceeded,
            rx_hdcp1_data.topology_us.max_device_exceeded,
            rx_hdcp1_data.topology_us.device_count,
            rx_hdcp1_data.topology_us.depth);*/

        }
        else
            sii_log_debug(" DS is REPEATER but DEV CNT = 0\n");

        sii_us_send_topology(ds_repeater_mode);
        return_val |= SII_DRV_HDCP_EVENT__HDCP_BKSV_CHANGED;
    }

    if(event_flags == SII_DRV_HDCP_EVENT__SKE_SEND_EKS){
        sii_log_debug(" SII_DRV_HDCP_EVENT__HDCP_SKE_EVENT\n");

        sii_log_debug(" REPEATER MODE = [%d]\n",(int) ds_repeater_mode);
	tx_hdcp2_data.bIsDsRepeater = ds_repeater_mode;

        if (tx_hdcp2_data.bIsDsRepeater != TX_HDCP_RPTR_22)
        {
            rx_hdcp1_data.topology_us.seq_num_v            =        ds_topology.seq_num_v;
            rx_hdcp1_data.topology_us.hdcp1_device_ds      = (bool) ds_topology.hdcp1x_device_ds;
            rx_hdcp1_data.topology_us.hdcp2_device_ds      = (bool) ds_topology.hdcp20_repeater_ds;
            rx_hdcp1_data.topology_us.max_cascade_exceeded =        false;
            rx_hdcp1_data.topology_us.max_device_exceeded  =        false;
            rx_hdcp1_data.topology_us.device_count         =        1;
            rx_hdcp1_data.topology_us.depth                =        1;

            rx_hdcp1_data.ksv_list = (struct sii_hdcp_ksv *)(bksv_list.p_list_start);

            //rx_hdcp1_data.hdcp_data_sent = false;
            //rx_hdcp1_data.banner_rdy = true;
            rx_hdcp1_data.hdcp_data_rdy = true;

        }
        else
            sii_log_debug(" received SKE but DS is repeater waiting for RCVID CHG\n",(int) ds_repeater_mode);

        sii_us_send_topology(ds_repeater_mode);
        return_val |= SII_DRV_HDCP_EVENT__SKE_SEND_EKS;
    }

    if(event_flags == SII_DRV_HDCP_EVENT__HDCP_RCVID_CHANGED){
        sii_log_debug(" SII_DRV_HDCP_EVENT__HDCP_RCVID_CHANGED\n");
        rx_hdcp1_data.topology_us.seq_num_v            =        ds_topology.seq_num_v;
        rx_hdcp1_data.topology_us.hdcp1_device_ds      = (bool) ds_topology.hdcp1x_device_ds;
        rx_hdcp1_data.topology_us.hdcp2_device_ds      = (bool) ds_topology.hdcp20_repeater_ds;
        rx_hdcp1_data.topology_us.max_cascade_exceeded = (bool) ds_topology.max_cascade_exceeded;
        rx_hdcp1_data.topology_us.max_device_exceeded  = (bool) ds_topology.max_devs_exceeded;
        rx_hdcp1_data.topology_us.device_count         =        ds_topology.device_count+1;
        rx_hdcp1_data.topology_us.depth                =        ds_topology.depth+1;

        rx_hdcp1_data.ksv_list = (struct sii_hdcp_ksv *)(bksv_list.p_list_start);

        sii_log_debug(" REPEATER MODE = [%d]\n",(int) ds_repeater_mode);

        tx_hdcp2_data.bIsDsRepeater = ds_repeater_mode;

        if(rx_hdcp1_data.topology_us.device_count > HDCP2_MAX_DEVICE_COUNT)
        {
            sii_log_debug(" HDCP2_MAX_DEVICE_COUNT\n");
            rx_hdcp1_data.topology_us.max_device_exceeded = TRUE;
            //rx_hdcp1_data.hdcp_data_sent = false;
            //rx_hdcp1_data.banner_rdy = true;
            rx_hdcp1_data.hdcp_data_rdy = true;
        }
        else if(rx_hdcp1_data.topology_us.depth > HDCP2_MAX_DEPTH)
        {
            sii_log_debug(" HDCP2_MAX_DEPTH\n");
            rx_hdcp1_data.topology_us.max_cascade_exceeded = TRUE;
            //rx_hdcp1_data.hdcp_data_sent = false;
            //rx_hdcp1_data.banner_rdy = true;
            rx_hdcp1_data.hdcp_data_rdy = true;
        }
        else
        {
            //rx_hdcp1_data.hdcp_data_sent = false;
            //rx_hdcp1_data.banner_rdy = true;
            rx_hdcp1_data.hdcp_data_rdy = true;
        }

        /*sii_log_debug(" Banner TOP = Seq [%d] hdcp1 [%d] hdcp2 [%d] max_c [%d] max_d [%d] dev_cnt [%d] depth [%d] \n",rx_hdcp1_data.topology_us.seq_num_v,
        rx_hdcp1_data.topology_us.hdcp1_device_ds,
        rx_hdcp1_data.topology_us.hdcp2_device_ds,
        rx_hdcp1_data.topology_us.max_cascade_exceeded,
        rx_hdcp1_data.topology_us.max_device_exceeded,
        rx_hdcp1_data.topology_us.device_count,
        rx_hdcp1_data.topology_us.depth);*/
        sii_us_send_topology(ds_repeater_mode);
        return_val |= SII_DRV_HDCP_EVENT__HDCP_RCVID_CHANGED;
    }
    if(event_flags & SII_DRV_HDCP_EVENT__HDCP_BSTATUS_CHANGED){
        sii_log_debug(" SII_DRV_HDCP_EVENT__HDCP_BSTATUS_CHANGED - ERROR OCCURED\n");
        rx_hdcp1_data.topology_us.max_cascade_exceeded = (bool)ds_topology.max_cascade_exceeded;
        rx_hdcp1_data.topology_us.max_device_exceeded  = (bool)ds_topology.max_devs_exceeded;
        rx_hdcp1_data.topology_us.device_count         = ds_topology.device_count;
        rx_hdcp1_data.topology_us.depth                = ds_topology.depth;

        rx_hdcp1_data.ksv_list = (struct sii_hdcp_ksv *)(bksv_list.p_list_start);

        if(rx_hdcp1_data.hdcp_data_sent == false)
        {
            sii_set_rx_hdcp_topology(0, &(rx_hdcp1_data.topology_us));
            sii_set_rx_hdcp_rxid_list(0, rx_hdcp1_data.topology_us.device_count, rx_hdcp1_data.ksv_list);
            sii_trigger_rx_hdcp_rxid_list_to_send(0);
            rx_hdcp1_data.hdcp_data_sent = true;
        }
        //rx_hdcp1_data.banner_rdy = true;
        rx_hdcp1_data.hdcp_data_rdy = true;
        sii_us_send_topology(ds_repeater_mode);
        return_val |= SII_DRV_HDCP_EVENT__HDCP_BSTATUS_CHANGED;
    }

    if (event_flags & SII_DRV_HDCP_EVENT__HDCP_HASH_FAIL_EVENT) {
	    /*Resend Stream_manage_message*/
	    sii_log_debug("Resending the Stream Manage message\n");
	    sii_drv_tx_hdcp_stream_manage_msg_set((void *)l_drv_context,
		    &(tx_hdcp2_data.StreamManageInfoDs));
        tx_hdcp2_data.StreamManageInfoDs.seq_num_m++;
    }
    if( event_flags == SII_DRV_HDCP_EVENT__HDCP_CERT_RCVD){
        // allow disabling HDCP2
        tx_hdcp2_data.bHdcp2_CertRx_done = 1;
    }
    if (event_flags == SII_DRV_HDCP_EVENT__HDCP_REAUTH_REQ){
        // When reauth_req happen,
        // set ForceStreamManage = 1 and
        // must indicate data are not sent yet
        tx_hdcp2_data.bForceStreamManage = true;
        sii_log_debug("bForceStreamManage=%d(%d)\n",
            tx_hdcp2_data.bForceStreamManage,__LINE__);
        rx_hdcp1_data.hdcp_data_sent = false;
        rx_hdcp1_data.hdcp_data_rdy  = false;
    }
    return return_val;
}


static void sii_rx_event_handler(sii_inst_t inst, void* p_obj, uint8_t event)
{
    if (event == SII_EVENT__BOOT_DONE) {
        bool host_mode;
        sii_log_debug("[EVT]: Boot Done\n");
        sii_get_host_mode(inst, &host_mode);
        if (host_mode)
            sii_log_debug("\tRx FW Run in Host Mode.\n");
        else
            sii_log_debug("\tRx FW Run in Stand-alone Mode.\n");
        sii_set_host_mode(inst, false);
        sii_set_rx_hdcp_repeater_mode(inst, true);
/*      sii_set_rx_edid_content(inst, 0, sink_edid);
        sii_set_rx_edid_content(inst, 1, sink_edid + SII_EDID_BLOCK_SIZE);
        sii_set_rx_edid_content(inst, 2, sink_edid + SII_EDID_BLOCK_SIZE*2);
        sii_set_rx_edid_content(inst, 3, sink_edid + SII_EDID_BLOCK_SIZE*3);
*/  } else
    if (event == SII_EVENT__RX_AV_LINK_CHNG) {
        enum sii_av_link_type type = SII_AV_LINK__NONE;
        bool plus_5v_status;
#ifdef CONFIG_TX_TMDS_MODE_CTRL
        static bool plus_5v_status_prev = false;
#endif
        sii_get_rx_link_type(inst, &type);
        switch (type) {
            case SII_AV_LINK__NONE:
                sii_log_debug("\tRx Link Type is None\n");
                break;
            case SII_AV_LINK__HDMI1:
                sii_log_debug("\tRx Link Type is HDMI 1.4\n");
                break;
            case SII_AV_LINK__HDMI2:
                sii_log_debug("\tRx Link Type is HDMI 2.0\n");
                break;
            case SII_AV_LINK__DVI:
                sii_log_debug("\tRx Link Type is DVI\n");
                break;
            default:
                break;
        }
        sii_get_rx_plus5v(inst, &plus_5v_status);
        sii_log_debug("\tRX PRW 5V Detect: %s\n", plus_5v_status ? "ON" : "OFF");
#ifdef CONFIG_TX_TMDS_MODE_CTRL
        if( plus_5v_status != plus_5v_status_prev)
        {
            plus_5v_status_prev = plus_5v_status;
            if( plus_5v_status )
            {
                sii_drv_tx_tmds_mode_set((void *)l_drv_context, SII_TMDS_MODE__AUTO);
            }
            else
            {
                memset(&tx_hdcp2_data, 0, sizeof(tx_hdcp2_data));
                sii_hdcp_enable(false);
                sii_drv_tx_tmds_mode_set((void *)l_drv_context, SII_TMDS_MODE__NONE);
            }
        }
#endif //CONFIG_TX_TMDS_MODE_CTRL
    }

#ifndef __REMOVE_UNNECESSARY_FUNC__
    else if (event == SII_EVENT__AV_MUTE_CHNG) {
        bool av_mute;
        sii_log_debug("[EVT]: Av Mute Change\n");
        sii_get_rx_av_mute(inst, &av_mute);
        if (av_mute) {
            sii_log_debug("\tAV Mute ON\n");
        } else {
            sii_log_debug("\tAV Mute OFF\n");
        }
    }
#endif
    else if (event == SII_EVENT__SCDT_CHNG) {
        //memset(&rx_hdcp1_data, 0, sizeof(rx_hdcp1_data));
        memset(&rx_hdcp2_data, 0, sizeof(rx_hdcp2_data));
        memset(&tx_hdcp2_data, 0, sizeof(tx_hdcp2_data));
        //mute tx av and disable tx hdcp
        //...

        sii_log_debug("[EVT]: SCDT Change\n");

    }
#ifndef __REMOVE_UNNECESSARY_FUNC__
    else if (event == SII_EVENT__DS_CONNECT_STATUS_CHNG) {
        bool tx_hpd;
        sii_log_debug("DS Connection Change\n");
        /* Add user callback for DS_CONNECT_STATUS_CHNG event*/
        sii_get_tx_hpd(inst, &tx_hpd);
        if (tx_hpd) {
            sii_log_debug(" DS HPD HIGH\n");
            if (video_info.tmds_clock > 340)
                sii_set_tx_scramble(inst, true);
        } else {
            sii_log_debug(" DS HPD LOW\n");
        }
    } else if (event == SII_EVENT__DS_EDID_STATUS_CHNG) {
        sii_log_debug("[EVT]: DS EDID Status Change\n");
    }
#endif
    else if (event == SII_EVENT__RX_HDCP_STATUS_CHNG) {
        //enum sii_hdcp_version hdcp_version;
#ifndef __REMOVE_UNNECESSARY_FUNC__
        enum sii_hdcp_status hdcp_status;
#endif
        sii_log_debug("[EVT]: RX HDCP Status Change\n");
        rx_hdcp1_data.us_hdcp_ver = SII_HDCP_VERSION__NONE;
        sii_get_rx_hdcp_us_version(inst, &rx_hdcp1_data.us_hdcp_ver);
        switch(rx_hdcp1_data.us_hdcp_ver) {
            case SII_HDCP_VERSION__NONE:
                sii_log_debug("\tRX HDCP Version is None\n");
                rx_hdcp1_data.spiderman_rdy = false;
				rx_hdcp1_data.auth_req_rcvd_frm_spiderman = false;
                //rx_hdcp1_data.banner_rdy = false;
                rx_hdcp1_data.hdcp_data_rdy = false;
                rx_hdcp1_data.hdcp_data_sent = false;
                sii_hdcp_enable(false);
                break;
            case SII_HDCP_VERSION__1X:
                sii_log_debug("\tRX HDCP Version is 1.4\n");
                //Should not enable HDCP here.
                //SA may notify at later events.
                //sii_hdcp_enable(true);
                rx_hdcp2_data.info_us.seq_num_m = 0;
                rx_hdcp2_data.info_us.stream_id_type[0] = 0;
                break;
            case SII_HDCP_VERSION__20:
                sii_log_debug("\tRX HDCP Version is 2.0\n");
                break;
            case SII_HDCP_VERSION__22:
                sii_log_debug("\tRX HDCP Version is 2.2\n");
                break;
            default:
                break;
        }
#ifndef __REMOVE_UNNECESSARY_FUNC__
	/*Calling if sii_get_rx_hdcp_status is just
	 *to print the status, So commenting as of now*/
        sii_get_rx_hdcp_status(inst, &hdcp_status);
        switch(hdcp_status) {
            case SII_HDCP_STATUS__OFF:
                sii_log_debug("\tRX HDCP is Off\n");
                break;
            case SII_HDCP_STATUS__SUCCESS_1X:
                sii_log_debug("\tRX HDCP 1.x is Authenticated\n");
                break;
            case SII_HDCP_STATUS__SUCCESS_22:
                sii_log_debug("\tRX HDCP 2.2 is Authenticated\n");
                break;
            case SII_HDCP_STATUS__AUTHENTICATING:
                sii_log_debug("\tRX HDCP is Starting\n");
                break;
            case SII_HDCP_STATUS__FAILED:
                sii_log_debug("\tRX HDCP is Failed\n");
                break;
            default:
                break;
        }
#endif

    }
    else if (event == SII_EVENT__HDR_REC) {
        sii_log_debug("EVT: HDR Infoframe Received\n");
        //sii_get_rx_info_frame_hdr(inst, infoframe, &length);
    } else if (event == SII_EVENT__VSIF_REC) {
        sii_log_debug("EVT: VSIF Infoframe Received\n");

    }
#ifndef __REMOVE_UNNECESSARY_FUNC__
        else if (event == SII_EVENT__AVI_REC) {
        uint8_t infoframe[SII_RX_INFOFRAME_MAX_LENGTH];
        uint8_t length;
        sii_log_debug("EVT: AVI Infoframe Received\n");
        sii_get_rx_info_frame_avi(inst, infoframe, &length);

    } else if (event == SII_EVENT__AIF_REC) {
        uint8_t infoframe[SII_RX_INFOFRAME_MAX_LENGTH];
        uint8_t length;
        sii_log_debug("EVT: AIF Infoframe Received\n");
        sii_get_rx_info_frame_aif(inst, infoframe, &length);
    } else if (event == SII_EVENT__RX_VIDEO_RESOLUTION_CHNG) {
        sii_log_debug("[EVT]: Video Resolution Changed\n");
        sii_show_video_info(inst);
    } else if (event == SII_EVENT__RX_TMDS_CLOCK_CHNG) {
        sii_log_debug("[EVT]: TMDS Clock Changed\n");
        sii_get_rx_video_info(inst, &video_info);
    } else if (event == SII_EVENT__TX_RXSENSE_STATUS_CHNG) {
        bool rsen;
        sii_log_debug("[EVT]: TX RxSense Status Change\n");
        /* Add user callback for TX_RXSENSE_STATUS_CHNG event*/
        sii_get_tx_rxsense(inst, &rsen);
        if (rsen) {
            sii_log_debug(" TX RxSense ON\n");
        } else {
            sii_log_debug(" TX RxSense OFF\n");
        }
    }
#endif
    else if (event >= SII_EVENT__RX_HDCP_AUTH_START &&
            event <= SII_EVENT__RX_HDCP_AUTH_FAIL) {
            sii_rx_hdcp_event_handler(inst, p_obj, event);
            sii_log_debug(" HDCP EVENT [0x%x]\n",event);
    } else {
        sii_log_debug("[EVT]: Unknown: %d\n", event);
    }
}


uint8_t sii_handle(sii_inst_t inst, void *p_obj)
{
    static bool boot_done_flag = false;
    struct sii_ipc_notification notification;
    enum sii_ipc_status ntf_queue_status;
    uint8_t event;

    if( boot_done_flag == false ){
        if( sii_is_boot_done(inst) == true ){
            boot_done_flag = true;
        }
        else{
            pr_err("*** SA boot failure *** \n");
            return SII_API_FAILURE;
        }
    }
    /* sii_device_isr_enable is a platform-independent function.
       It must be implemented on the hardware-triggered ISR platform. */
    sii_device_isr_enable(inst, false);
    ntf_queue_status = sii_ipc_host_pop_queue_notification(inst, &notification);
    sii_device_isr_enable(inst, true);
    ntf_exec_list[ntf_exec_cnt].notification_type = notification.notification_type;
    ntf_exec_cnt = (ntf_exec_cnt + 1) % SII_IPC_NOTIFICATION_QUEUE_LENGTH;

    if (ntf_queue_status != SII_IPC_STATUS_ERROR_NOTIFICATION_QUEUE_EMPTY){
        sii_log_debug("[debug notify]: %d(%02x) start\n", (int)ntf_exec_cnt,(int)notification.notification_type);
        event = notification.notification_type - SII_EVENT__BASE;
        if (event_filter(inst, event)) {
            sii_rx_event_handler(inst, p_obj, event);
            sii_log_debug("[debug notify]: %d(%02x) done\n", (int)ntf_exec_cnt,(int)notification.notification_type);
        }
    }
    else{
        ntf_exec_cnt = 0;
    }
    //Sii9396RxHDCPGet(inst);
    // sii_log_debug("%s:%d done\n", __func__, __LINE__);
    return ntf_exec_cnt;
}


uint8_t sii_create(void)
{
    int8_t index = get_next_instance_index();
    uint8_t new_inst = (uint8_t) index;

    if(index <0){
      pr_err(" sii create error '\n");
    }

    sii_ipc_init(new_inst);

    api_host_context.instance[index].in_use = true;
    api_host_context.instance[index].event_mask = 0x00;
    api_host_context.instance[index].event_handler = NULL;
    api_host_context.instance_count++;

    return new_inst;

}

int spiderman_init(void)
{
    int ret=0;
    struct sii9396_platform_data *pdata = &sg_stSii9396Platform;

    memset(&sg_stSii9396Platform,0,sizeof(struct sii9396_platform_data));
    ex_sp_pdata = pdata;

	pdata->i2c_device_id = 0x60;//SII_DRV_SPIDERMAN_I2C_ADDR;
    pdata->rx_inst = sii_create();

    if (pdata->rx_inst == SII_INST_NULL){
		pr_err(" if (rx_inst == SII_INST_NULL) \n");
		ret = SII_INST_NULL;
		return  ret;
    }

    return ret;
}

/*This function is to handle the case of AVI change without SCDT change
*when this AVI change happens, 9630 driver toggles it TX TMDS, to redo
*the CSC and DSC, So it will do HDCP again, BUT SA doesn't anything about it,
*so it deosn't send a new Stream mange messag. so using this flag we are
*sending stream mange message in tx_hdcp_handler(by reading the stream mange
*info from SA) */
void set_need_to_send_strm_mng_msg_flag(void)
{
	tx_hdcp2_data.bForceStreamManage = true;
    rx_hdcp1_data.hdcp_data_rdy      = false;
}


static enum sii_ipc_status sii_execute_buffer_command_buffer_reply(
	sii_inst_t inst, struct sii_ipc_packet_command *command,
	struct sii_ipc_packet_reply *packet_reply,
	bool *more_packet) {

	enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;
	enum sii_ipc_status status2 = SII_IPC_STATUS_SUCCESS;
	bool clear_interrupt = false;

	/* Initial Return */
	*more_packet = false;

	status = sii_ipc_sync_with_slave_before_send_command(inst);
	if (status != SII_IPC_STATUS_SUCCESS) {
		goto exit;
	}

	/* Send the Command (Buffer Command)
	   (expect Buffer Reply)
	 */
	status = sii_ipc_send_buffer_command(inst, command, true);
	if (status != SII_IPC_STATUS_SUCCESS) {
		goto exit;
	}

	ipc_host_context[inst].host_sm = SII_IPC_HOST_SM_WAIT_REPLY;
	status = sii_ipc_waiting_reply(inst);
	if( status == SII_IPC_STATUS_SUCCESS ){
		sii_ipc_host_handler(inst);
	}
//	else{
//		pr_err("%s:%d IPC POLLING TIMEOUT *** \n", __func__, __LINE__);
//	}
	clear_interrupt = true;

	/* Receive the Reply (Buffer Reply) */
	status = sii_ipc_receive_buffer_reply(inst, packet_reply, more_packet);
	if (status != SII_IPC_STATUS_SUCCESS) {
		goto exit;
	}

	/* FD: TBI, Stop Send Command Timer */

exit:
	ipc_host_context[inst].host_sm = SII_IPC_HOST_SM_IDLE;

	if (true == clear_interrupt) {
		status2 = sii_ipc_host_clear_and_enable_interrupt(inst);
		if (status2 != SII_IPC_STATUS_SUCCESS) {
			status = status2;
		}
	}

	/* FD: TBI, Stop Send Command Timer if there */

	return status;
}

enum sii_ipc_status sii_execute_message_command_message_reply(
		sii_inst_t inst, struct sii_ipc_message_command *message_command,
		struct sii_ipc_message_reply *message_reply) {

	enum sii_ipc_status status = SII_IPC_STATUS_SUCCESS;
	enum sii_ipc_status status2 = SII_IPC_STATUS_SUCCESS;

	struct sii_ipc_packet_command packet_command;
	struct sii_ipc_packet_reply packet_reply;
	struct sii_ipc_register_reply register_reply;

	struct sii_ipc_packet reply_packet;
	bool more_packet = false;
	bool ack_last_piece = false;

	packet_command.packet = &(ipc_host_context[inst].host_packet);
	packet_reply.packet = &reply_packet; /* FD: TBR, re-use host_packet */

	/*
	   Send Single-Packet Command or Multi-Packet Command
	   (receive the first Packet Reply)
	 */
	ipc_host_context[inst].packet_sequence = 0;
	ipc_host_context[inst].packet_fragment = 0;

	/* Compose First Packet Command from Message Command */
	sii_compose_packet_command_from_message_command(inst,
			&packet_command, message_command,
			ipc_host_context[inst].packet_sequence);

	if (1 == ipc_host_context[inst].packet_fragment) {

		/* Single-Packet Command */

		/* Need to Send the Only Packet Command */
	} else {

		/* Multi-Packet Command */

		/* Send Packet Command One by One */
		/* (expect to receive ACK) */
		do {
			status = sii_execute_buffer_command_register_reply(inst,
					&packet_command, &register_reply);
			if (status != SII_IPC_STATUS_SUCCESS) {
				goto exit;
			}

			if (register_reply.reply_status
					!= SII_IPC_PHY_REPLY_STATUS_ACK) {
				/* NACK/FAILURE to Multi-Buffer Command */
				goto exit;
			}

			/* Prepare the Next Packet Command for Sending */
			(ipc_host_context[inst].packet_sequence)++;

			/* Compose Packet Command from Message Command */
			sii_compose_packet_command_from_message_command(inst,
					&packet_command, message_command,
					ipc_host_context[inst].packet_sequence);

		} while (ipc_host_context[inst].packet_sequence + 1 <
				ipc_host_context[inst].packet_fragment);

		/* Need to Send Last Packet of Multi-Packet Command */
	}

	/*
	   Send the Only Packet Command of Single-Packet Command, or
	   Last Packet of Multi-Packet Command
	   (expect to receive the first Packet Reply)
	 */
	status = sii_execute_buffer_command_buffer_reply(inst,
			&packet_command,
			&packet_reply,
			&more_packet);
	if (status != SII_IPC_STATUS_SUCCESS) {
		goto exit;
	}

	if (packet_reply.reply_status != SII_IPC_PHY_REPLY_STATUS_SUCCESS) {
		/* Successful IPC Pair (Command & Reply),
		   Unsuccessful Execution */
		goto exit;
	}

	/* Compose Message Reply from Packet Reply */
	sii_compose_message_reply_from_packet_reply(inst,
			message_reply, &packet_reply);

	/*
	   Receive Packet Reply(s)
	   (except for the first Packet Reply)
	 */
	while (more_packet) {
		status = sii_receive_next_piece_of_buffer_reply(inst,
				&packet_reply, &more_packet);

		/* Refuse next piece of Reply
		   if there is any error during parsing */
		if (status != SII_IPC_STATUS_SUCCESS) {
			status2 = sii_refuse_next_piece_of_buffer_reply(inst);
			if (status2 != SII_IPC_STATUS_SUCCESS) {
				status =
					SII_IPC_STATUS_ERROR_PROTOCOL_CANNOT_NACK;
			} else {
				/* do nothing, use original error code */
			}

			ack_last_piece = false;
			break;  // no need to get next piece
		} else {
			ack_last_piece = true;
		}

		/* Compose Message Reply from Packet Reply */
		sii_compose_message_reply_from_packet_reply(inst,
				message_reply, &packet_reply);
	}

	/* Accept Last Piece of Buffer Reply */
	if (true == ack_last_piece) {
		sii_accept_last_piece_of_buffer_reply(inst);
		if (status != SII_IPC_STATUS_SUCCESS) {
			goto exit;
		}
	}

exit:
	return status;
}


static enum sii_ipc_api_status sii_api_msg_cmd_msg_reply_process(
		sii_inst_t inst,
		struct sii_ipc_message_command command,
		struct sii_ipc_message_reply *reply)
{
	enum sii_ipc_api_status api_status = SII_IPC_API_SUCCESS;
	enum sii_ipc_status ipc_status = SII_IPC_STATUS_SUCCESS;
	uint16_t raw_msg_length = (command.message->message_length_l) |
			(((uint16_t) (command.message->message_length_h)) << 8);
	sii_compose_message(
	SII_IPC_MESSAGE_PROTOCOL_VERSION_1,
						SII_IPC_MESSAGE_TYPE_COMMAND,
						command.opcode,
						raw_msg_length,
						command.message);
	ipc_status = sii_execute_message_command_message_reply(inst,
															&command,
															reply);
	if (ipc_status != SII_IPC_STATUS_SUCCESS) {
		api_status = SII_IPC_API_FAILURE;
		//last_err = SII_IPC_API_ERR__INTERNAL;
		goto exit;
	}

	if (reply->reply_status != SII_IPC_PHY_REPLY_STATUS_SUCCESS) {
		api_status = SII_IPC_API_FAILURE;
		/* Save api error code */
		//last_err = reply->reply_return;
	} else {
		ipc_status = sii_check_message_validity(
		SII_IPC_MESSAGE_PROTOCOL_VERSION_1,
												SII_IPC_MESSAGE_TYPE_REPLY,
												command.opcode,
												reply->message);
		if (ipc_status == SII_IPC_STATUS_ERROR_MESSAGE_CHECKSUM) {
			api_status = SII_IPC_API_FAILURE;
			//last_err = SII_IPC_API_ERR__CHECKSUM;
		}
		else if (ipc_status == SII_IPC_STATUS_ERROR_MESSAGE) {
			api_status = SII_IPC_API_FAILURE;
			//last_err = SII_IPC_API_ERR__MSG_FORMAT;
		}
	}

exit:
	//sii_ipc_display_error(ipc_status);
	return api_status;
}


enum sii_ipc_api_status sii_host_set_tx_audio_insertion(
		sii_inst_t inst,
		struct sii_ipc_message *msg)
{
	struct sii_ipc_message_command command;
	struct sii_ipc_register_reply reply;

	command.opcode = SII_IPC_PHY_CMD_OPCODE_SET_TX_AUDIO_INSERTION;
	command.parameter = 0;
	command.message = msg;

	return sii_api_msg_cmd_reg_reply_process(inst, command, &reply);
}

enum sii_ipc_api_status sii_host_set_rx_audio_extraction(
		sii_inst_t inst, uint8_t value)
{
	struct sii_ipc_register_command command;
	struct sii_ipc_register_reply reply;

	command.opcode = SII_IPC_PHY_CMD_OPCODE_SET_RX_AUDIO_EXTRACTION;
	command.parameter = value;

	return sii_api_reg_cmd_reg_reply_process(inst, command, &reply);
}

enum sii_ipc_api_status sii_host_set_rx_audio_mclk_ratio(
		sii_inst_t inst, uint8_t value)
{
	struct sii_ipc_register_command command;
	struct sii_ipc_register_reply reply;

	command.opcode = SII_IPC_PHY_CMD_OPCODE_SET_RX_AUDIO_MCLK_RATIO;
	command.parameter = value;

	return sii_api_reg_cmd_reg_reply_process(inst, command, &reply);
}

enum sii_ipc_api_status sii_host_set_reg(
		sii_inst_t inst, struct sii_ipc_message *msg)
{
	struct sii_ipc_message_command command;
	struct sii_ipc_register_reply reply;

	command.opcode = SII_IPC_PHY_CMD_OPCODE_SET_REG;
	command.parameter = 0;
	command.message = msg;

	return sii_api_msg_cmd_reg_reply_process(inst, command, &reply);
}

enum sii_ipc_api_status sii_host_get_reg(
		sii_inst_t inst, struct sii_ipc_message *msg)
{
	struct sii_ipc_message_command msg_command;
	struct sii_ipc_message_reply msg_reply;

	msg_command.opcode = SII_IPC_PHY_CMD_OPCODE_GET_REG;
	msg_command.parameter = 0;
	msg_command.message = msg;
	msg_reply.message = msg;

	return sii_api_msg_cmd_msg_reply_process(inst, msg_command, &msg_reply);
}


enum sii_api_status sii_set_cec_reg(
		sii_inst_t inst, uint8_t offset, uint8_t length, uint8_t *value)
{
	enum sii_api_status api_status = SII_API_SUCCESS;
	enum sii_ipc_api_status ipc_status = SII_IPC_API_SUCCESS;
	struct sii_ipc_message *msg = &ipc_host_context->host_message;
	uint16_t msg_len = length + 3;

	memset(msg, 0, sizeof(*msg));
	msg->message_payload.message_data[0] = SII_CEC_PAGE;
	msg->message_payload.message_data[1] = offset;
	msg->message_payload.message_data[2] = length;
	memcpy(&(msg->message_payload.message_data[3]), value, length);
	msg->message_length_l = msg_len & 0xFF;
	msg->message_length_h = msg_len >> 8;

	ipc_status = sii_host_set_reg(inst, msg);
	if (ipc_status != SII_IPC_API_SUCCESS)
		api_status = SII_API_FAILURE;

	return api_status;
}

enum sii_api_status sii_get_cec_reg(
		sii_inst_t inst, uint8_t offset, uint8_t length, uint8_t *value)
{
	enum sii_api_status api_status = SII_API_SUCCESS;
	enum sii_ipc_api_status ipc_status = SII_IPC_API_SUCCESS;
	struct sii_ipc_message *msg = &ipc_host_context->host_message;

	memset(msg, 0, sizeof(*msg));
	msg->message_payload.message_data[0] = SII_CEC_PAGE;
	msg->message_payload.message_data[1] = offset;
	msg->message_payload.message_data[2] = length;
	msg->message_length_l = 3;
	msg->message_length_h = 0;

	ipc_status = sii_host_get_reg(inst, msg);
	if (ipc_status != SII_IPC_API_SUCCESS)
		api_status = SII_API_FAILURE;
	else
		memcpy(value, msg->message_payload.message_data, length);
	return api_status;
}

enum sii_ipc_api_status sii_host_set_rx_hdcp_ddc(
		sii_inst_t inst, uint8_t value)
{
	struct sii_ipc_register_command command;
	struct sii_ipc_register_reply reply;

	command.opcode = SII_IPC_PHY_CMD_OPCODE_SET_RX_HDCP_DDC;
	command.parameter = value;

	return sii_api_reg_cmd_reg_reply_process(inst, command, &reply);
}

enum sii_ipc_api_status sii_host_get_tx_hdcp_ds_version(
        sii_inst_t inst,
        struct sii_ipc_message *msg)
{
    struct sii_ipc_register_command command;
    struct sii_ipc_message_reply reply;

    command.opcode = SII_IPC_PHY_CMD_OPCODE_GET_TX_HDCP_DS_VERSION;
    command.parameter = 0x00;
    reply.message = msg;

    return sii_api_reg_cmd_msg_reply_process(inst, command, &reply);
}


enum sii_api_status sii_get_tx_hdcp_ds_version(
        sii_inst_t inst,
        enum sii_hdcp_version *version)
{
    enum sii_api_status api_status = SII_API_SUCCESS;
    enum sii_ipc_api_status ipc_status = SII_IPC_API_SUCCESS;
    struct sii_ipc_message *msg = &ipc_host_context->host_message;

    ipc_status = sii_host_get_tx_hdcp_ds_version(inst, msg);
    if (ipc_status != SII_IPC_API_SUCCESS) {
        *version = SII_HDCP_VERSION__NONE;
        api_status = SII_API_FAILURE;
    } else {
        *version = (enum sii_hdcp_version) msg->message_payload.message_data[0];
        //dbg("get tx hdcp version: %x\n", *version);
    }

    return api_status;
}

bool Sii9396RxHDCPGet(sii_inst_t inst)
{
	BOOL  bRetVal = TRUE;
	enum sii_hdcp_version hdcp_version;
	enum sii_hdcp_status hdcp_status;

	sii_get_rx_hdcp_us_version(inst, &hdcp_version);
	switch(hdcp_version) {
	case SII_HDCP_VERSION__NONE:
		sii_log_debug("RX HDCP Version is None\n");
		break;
	case SII_HDCP_VERSION__1X:
		sii_log_debug("RX HDCP Version is 1.4\n");
		break;
	case SII_HDCP_VERSION__20:
		sii_log_debug("RX HDCP Version is 2.0\n");
		break;
	case SII_HDCP_VERSION__22:
		sii_log_debug("RX HDCP Version is 2.2\n");
		break;
	default:
		break;
	}

	sii_get_rx_hdcp_status(inst, &hdcp_status);

	switch(hdcp_status) {
	case SII_HDCP_STATUS__OFF:
		sii_log_debug("RX HDCP is Off\n");
		break;
	case SII_HDCP_STATUS__SUCCESS_1X:
		sii_log_debug("RX HDCP 1.x is Authenticated\n");
		break;
	case SII_HDCP_STATUS__SUCCESS_22:
		sii_log_debug("RX HDCP 2.2 is Authenticated\n");
		break;
	case SII_HDCP_STATUS__AUTHENTICATING:
		sii_log_debug("RX HDCP is Starting\n");
		break;
	case SII_HDCP_STATUS__FAILED:
		sii_log_debug("RX HDCP is Failed\n");
		break;
	default:
		break;
	}

	if((hdcp_version==SII_HDCP_VERSION__NONE)&&((hdcp_status==SII_HDCP_STATUS__OFF)||(hdcp_status==SII_HDCP_STATUS__FAILED)))
	{
		bRetVal = FALSE;
	}

	return bRetVal;
}

#endif // (CONFIG_SOURCE_ONLY == 0)

#endif  // #if (_ENABLE_CHIP_SIL9630 == _MACRO_ENABLE)

