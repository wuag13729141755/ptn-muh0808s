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
#if (_ENABLE_CHIP_SII9630 == _MACRO_ENABLE)

#include "Sii9630_si_lib_log.h"
#include "Sii9630_si_drv_cp8630.h"

#if 0
#define SII_DRIVER_MINOR_MAX 1

#define ANSI_ESC_RESET_TEXT ""
#define ANSI_ESC_WHITE_BG ""
#define ANSI_ESC_RED_TEXT ""
#define ANSI_ESC_YELLOW_TEXT ""
#define ANSI_ESC_GREEN_TEXT ""
#define ANSI_ESC_BLACK_TEXT ""
#define ANSI_ESC_WHITE_TEXT ""
#define ANSI_ESC_MAGENTA_TEXT ""
#define ANSI_ESC_CYAN_TEXT ""
/* Define SysFs attribute names */
#define SYS_ATTR_NAME_DS_CONNECTION			connection_state
#define SYS_ATTR_NAME_DS_HPD				ds_hpd_state
#define SYS_ATTR_NAME_DS_RSENSE				ds_rsense_state
/*#define SYS_ATTR_NAME_DS_EDID				ds_edid*/
#define SYS_ATTR_NAME_DS_TMDS_MODE			ds_tmds_mode
#define SYS_ATTR_NAME_DS_TMDS_MODE_SET			ds_tmds_mode_set
#define SYS_ATTR_NAME_DS_AVMUTE				ds_avmute
#define SYS_ATTR_NAME_DS_STRM_MSG			ds_strm_msg
#define SYS_ATTR_NAME_DS_HDCP_CTRL			ds_hdcp_ctrl
/*#define SYS_ATTR_NAME_HDCP_AUTH_VERSION			hdcp_version*/
#define SYS_ATTR_NAME_DS_HDCP_STATUS			ds_hdcp_status
#define SYS_ATTR_NAME_DS_HDCP_FAIL_REASON		ds_hdcp_fail_reason
#define SYS_ATTR_NAME_DS_HDCP_VERSION			ds_hdcp_version
#define SYS_ATTR_NAME_DS_BKSV_LIST			ds_bksv_list
#define SYS_ATTR_NAME_DS_HDCP2_CONTENT_TYPE		ds_hdcp2_content_type
#define SYS_ATTR_NAME_DS_HDCP_RPTR_MODE			ds_hdcp_rptr_mode
#define SYS_ATTR_NAME_DS_HDCP_TOPOLOGY			ds_hdcp_topology
#define SYS_ATTR_NAME_HDCP_EVENTS			hdcp_events
#define SYS_ATTR_NAME_DS_SCDC_SCRAMBLE_CTRL		ds_scdc_scramble_ctrl
#define SYS_ATTR_NAME_DS_SCDC_DEBUG_ENABLE		ds_scdc_debug_enable
#ifdef RX_DESCRAMBLE_THROUGH_API
#define SYS_ATTR_NAME_US_SCDC_TMDS_CONFIG		us_scdc_tmds_config
#define SYS_ATTR_NAME_US_SCRAMBLE_STATUS		us_scramble_status
#endif
#define SYS_ATTR_NAME_DS_DSC_CTRL			ds_dsc_ctrl
#define SYS_ATTR_NAME_DS_DSC_COMPRESSION		ds_dsc_compression
#define SYS_ATTR_NAME_DS_OUTPUT_CLRSPC			ds_output_clrspc
#define SYS_ATTR_NAME_DS_HVSYNC_POLARITY		ds_hvsync

#define SYS_ATTR_NAME_US_PLUS_5V			us_plus_5v_state
#define SYS_ATTR_NAME_DEVICE_EDID			device_edid
#define SYS_ATTR_NAME_US_HPD				us_hpd
#define SYS_ATTR_NAME_US_TMDS_MODE			us_tmds_mode
#define SYS_ATTR_NAME_US_HDCP_STATUS			us_hdcp_status
#define SYS_ATTR_NAME_US_BKSV_LIST			us_bksv_list

#define SYS_ATTR_NAME_DEBUG_LEVEL			debug_level
#define SYS_ATTR_NAME_REG_DUMP_DEBUG_LEVEL		debug_reg_dump


#define SYS_ATTR_NAME_GPIO_INDEX			gpio_index
#define SYS_ATTR_NAME_GPIO_VALUE			gpio_value
#define SYS_OBJECT_NAME_REG_ACCESS			reg_access
#define		SYS_ATTR_NAME_REG_ACCESS_PAGE		page
#define		SYS_ATTR_NAME_REG_ACCESS_OFFSET		offset
#define		SYS_ATTR_NAME_REG_ACCESS_LENGTH		length
#define		SYS_ATTR_NAME_REG_ACCESS_DATA		data
#define SYS_ATTR_NAME_RX_CLR_SPC			input_clr_spc
#endif

#if 0
/*
 *  File operations supported by the cp8630 driver
 */
const struct file_operations cp8630_fops = {
	.owner = THIS_MODULE

};
static int rpt_device_dbg_i2c_reg_xfer(
	struct cp8630_driver_context *drv_context, u16 offset, u16 count,
	bool rw_flag, u8 *buffer);
static int si_strtoul(char **str, int base, unsigned long *val);

/*
 * show_ds_connection_state() - Handle read request to the ds_connection_state
 *							attribute file.
 */
ssize_t show_ds_connection_state(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	bool connection_state;
	int return_val;

	struct cp8630_driver_context *drv_context = dev_get_drvdata(dev);
	sii_drv_tx_connection_state_get(drv_context->drv_obj,
		&connection_state);
	if (connection_state)
		return_val = snprintf(buf, PAGE_SIZE, "connected");
	else
		return_val = snprintf(buf, PAGE_SIZE, "not connected");

	return return_val;
}

/*
 * set_ds_connection_state() - Handle write request to the ds_connection_state
 *   attribute file.
 */
ssize_t set_ds_connection_state(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t count)
{
	struct cp8630_driver_context *drv_context = dev_get_drvdata(dev);
	int status = -ENODATA;
	unsigned long new_connection_state = 0x100;

	sii_log_debug("received string: %s\n", buf);

	if (buf != NULL) {
		status = kstrtoul(buf, 0, &new_connection_state);
		if ((status != 0) || (new_connection_state > 0xFF)) {
			sii_log_err("Invalid connection_state: 0x%02lX\n",
				new_connection_state);
			goto err_exit;
		}
	} else {
		sii_log_err("Missing connection_state parameter\n");
		status = 1;
		goto err_exit;
	}

	if ((drv_context->isr_lock))
		return -ERESTARTSYS;

	if (drv_context->dev_flags & DEV_FLAG_SHUTDOWN) {
		status = -ENODEV;
		goto err_exit;
	}
	/*todo: implement api*/
	/*sii_drv_tx_switch_cbus_mode(drv_context->drv_obj,
		CM_NO_CONNECTION);*/
	status = count;

	sii_log_err("%sdisconnecting%s\n", ANSI_ESC_YELLOW_TEXT,
		ANSI_ESC_RESET_TEXT);

err_exit: //up(&drv_context->isr_lock);

	return status;
}

/*
 * show_ds_hpd_state() - Handle read request to the ds_hpd_state attribute
 * file.
 */
ssize_t show_ds_hpd_state(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct cp8630_driver_context *drv_context = dev_get_drvdata(dev);
	bool hpd_status;

	if (!sii_drv_tx_hot_plug_get(drv_context->drv_obj, &hpd_status))
		return scnprintf(buf, PAGE_SIZE, "%d", hpd_status);
	return -ENODATA;
}

/*
 * show_ds_rsense_state() - Handle read request to the ds_rsense_state
 * attribute file.
 */
ssize_t show_ds_rsense_state(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct cp8630_driver_context *drv_context = dev_get_drvdata(dev);
	bool rsense_status;

	if (!sii_drv_tx_rsen_get(drv_context->drv_obj, &rsense_status))
		return scnprintf(buf, PAGE_SIZE, "%d", rsense_status);
	return -ENODATA;
}

/*
 * show_device_edid() - Handle read request to the device_edid attribute file.
 *
 * Reads from this file return the downstream edid, as processed
 * by this driver.
 *
 * The return value is the number of characters written to buf.
 */
ssize_t show_device_edid(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct cp8630_driver_context *drv_context = dev_get_drvdata(dev);
	/*u8 edid_buffer[256] = {0};*/
	struct sii_edid edid_buffer;
	int status = -ENODATA;

	sii_log_info("called\n");

	if ((drv_context->isr_lock))
		return -ERESTARTSYS;

	if (drv_context->dev_flags & DEV_FLAG_SHUTDOWN) {
		status = -ENODEV;
	} else {
		memset(&edid_buffer, 0, sizeof(struct sii_edid));
		sii_drv_tx_edid_get(drv_context->drv_obj, &edid_buffer);
		status = 0;
	}

	//up(&drv_context->isr_lock);

	if (status == 0) {
		int idx, i;

		for (idx = 0, i = 0; i < 16; i++) {
			u8 j;

			for (j = 0; j < 16; ++j, ++idx) {
				status += scnprintf(&buf[status], PAGE_SIZE,
					"0x%02x ", edid_buffer.b[idx]);
			}
			status += scnprintf(&buf[status], PAGE_SIZE, "\n");
		}
	}

	return status;

}

/*
 * show_ds_tmds_mode() - Handle to read request to the ds_tmds_mode file
 *
 * Reads the tmds mode from driver and returns the vale(enum sii_tmds_mode)
 * in decimal string format.
 * */
ssize_t show_ds_tmds_mode(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct cp8630_driver_context *drv_context = dev_get_drvdata(dev);
	enum sii_tmds_mode tmds_mode;
	ssize_t status = -ENODATA;

	if (!sii_drv_tx_tmds_mode_status_get(drv_context->drv_obj, &tmds_mode))
		return scnprintf(buf, PAGE_SIZE, "%d\n", tmds_mode);
	return status;
	/*switch (tmds_mode) {
	case 0:
		return scnprintf(buf, PAGE_SIZE, "off\n");
	case 1:
		return scnprintf(buf, PAGE_SIZE, "dvi\n");
	case 2:
		return scnprintf(buf, PAGE_SIZE, "hdmi1\n");
	case 3:
		return scnprintf(buf, PAGE_SIZE, "hdmi2\n");
	case 4:
		return scnprintf(buf, PAGE_SIZE, "auto\n");
	default:
		sii_log_debug("invalid tmds mode\n");
		return status;
	}*/
}

/*
 * set_ds_tmds_mode() - Handle write request to the set_ds_tmds_mode
 * attribute file.
 *
 * Write the number charecter to this file to enable/disable the
 * tx TMDS.
 *
 * the return value is number of characters in buf if successful or an
 * error code if not successful.
 */

ssize_t set_ds_tmds_mode(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t count)
{
	struct cp8630_driver_context *drv_context = dev_get_drvdata(dev);
	uint8_t tmds_mode;
	int status = 1;

	status = kstrtou8(buf, 0, &tmds_mode);
	if (-ERANGE == status) {
		sii_log_err("ERANGE %s\n", buf);
	} else if (1 == status) {
		sii_log_err("EINVAL %s\n", buf);
	} else if (status != 0) {
		sii_log_err("status: %d	buf: %c%s%c\n", status, '"', buf, '"');
	} else {
		if ((drv_context->isr_lock))
			return -ERESTARTSYS;
		if (drv_context->dev_flags & DEV_FLAG_SHUTDOWN) {
			status = -ENODEV;
		} else {
			if (sii_drv_tx_tmds_mode_set(drv_context->drv_obj,
				(enum sii_tmds_mode)tmds_mode))
				status = -ENODATA;
			else
				status = count;
		}

		//up(&drv_context->isr_lock);
	}

	return status;
}

/*
 * set_ds_avmute() - Handle write request to ds_avmute file.
 *
 * Writes to this file is to set/unset the avmute
 *
 */

ssize_t set_ds_avmute(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t count)
{
	struct cp8630_driver_context *drv_context = dev_get_drvdata(dev);
	uint8_t avmute;
	int status = 0;

	status = kstrtou8(buf, 0, &avmute);
	if (-ERANGE == status) {
		sii_log_err("ERANGE %s\n", buf);
	} else if (1 == status) {
		sii_log_err("EINVAL %s\n", buf);
	} else if (status != 0) {
		sii_log_err("status: %d	buf: %c%s%c\n", status, '"', buf, '"');
	} else {
		if ((drv_context->isr_lock))
			return -ERESTARTSYS;
		if (drv_context->dev_flags & DEV_FLAG_SHUTDOWN) {
			status = -ENODEV;
		} else {
			if (sii_drv_tx_av_mute_set(drv_context->drv_obj,
				(bool)avmute))
				status = -ENODATA;
			status = count;
		}

		//up(&drv_context->isr_lock);
	}

	return status;
}
/*
 * set_ds_strm_msg() - Handle write request to stream_manage_msg file.
 *
 * Writes to this file is to set stream_manage_msg
 *
 */
ssize_t set_ds_strm_msg(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t count)
{
	struct cp8630_driver_context *drv_context = dev_get_drvdata(dev);
	struct sii_drv_tx_strmng_2x_msg strm_msg;
	unsigned long value;
	u8 data[MAX_DEBUG_TRANSFER_SIZE];
	int i;
	char *str;
	int status = 1;
	char l_s8input = 0;
	char *pinput = &l_s8input;

	sii_log_debug("received string: %s\n", buf);

	if (pinput == 0)
		return 1;
	memcpy(pinput, buf, count);

	str = pinput;
	for (i = 0; (i < MAX_DEBUG_TRANSFER_SIZE) && ('\0' != *str); i++) {

		status = si_strtoul(&str, 0, &value);
		if (-ERANGE == status) {
			sii_log_debug("ERANGE %s%s%s\n", ANSI_ESC_RED_TEXT, str,
				ANSI_ESC_RESET_TEXT);
			goto exit_reg_access_data;
		} else if (1 == status) {
			sii_log_debug("EINVAL %s%s%s\n", ANSI_ESC_RED_TEXT, str,
				ANSI_ESC_RESET_TEXT);
			goto exit_reg_access_data;
		} else if (status != 0) {
			sii_log_debug("status:%d %s%s%s\n", status,
				ANSI_ESC_RED_TEXT, str, ANSI_ESC_RESET_TEXT);
			goto exit_reg_access_data;
		} else if (value > 0xFF) {
			sii_log_debug("value:0x%x str:%s%s%s\n", (uint)value,
				ANSI_ESC_RED_TEXT, str, ANSI_ESC_RESET_TEXT);
			goto exit_reg_access_data;
		} else {
			data[i] = value;
		}
	}

	if ((drv_context->isr_lock)) {
		return -ERESTARTSYS;
	}
	if (drv_context->dev_flags & DEV_FLAG_SHUTDOWN) {
		status = -ENODEV;
	} else {

		strm_msg.seq_num_m = data[3];
		strm_msg.seq_num_m = (strm_msg.seq_num_m << 8) | data[2];
		strm_msg.seq_num_m = (strm_msg.seq_num_m << 8) | data[1];
		strm_msg.seq_num_m = (strm_msg.seq_num_m << 8) | data[0];

		strm_msg.stream_k = data[4];
		strm_msg.stream_i_d = data[5];
		strm_msg.content_type = data[6];

		sii_log_err("seq_num_m: 0x%08x k: 0x%02x strm_id: 0x%02x strm type: %02x\n",
				strm_msg.seq_num_m,strm_msg.stream_k,
				strm_msg.stream_i_d,strm_msg.content_type);

		if (sii_drv_tx_hdcp_stream_manage_msg_set(drv_context->drv_obj,
			(struct sii_drv_tx_strmng_2x_msg*)&strm_msg))
			status = -ENODATA;
		status = count;
	}

	//up(&drv_context->isr_lock);

exit_reg_access_data:
    //kfree(pinput);
	return status;
}
/*
 * set_ds_hdcp_ctrl() - Handle write request to the ds_hdcp_ctrl
 * attribute file.
 *
 * Write the number charecter to this file to enable/disable the
 * tx HDCP.
 *
 * the return value is number fo charescters in buf if successful or an
 * error code if not successful.
 */

ssize_t set_ds_hdcp_ctrl(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t count)
{
	struct cp8630_driver_context *drv_context = dev_get_drvdata(dev);
	uint8_t hdcp_ctrl;
	int status = 1;

	status = kstrtou8(buf, 0, &hdcp_ctrl);
	if (-ERANGE == status) {
		sii_log_err("ERANGE %s\n", buf);
	} else if (1 == status) {
		sii_log_err("EINVAL %s\n", buf);
	} else if (status != 0) {
		sii_log_err("status: %d	buf: %c%s%c\n", status, '"', buf, '"');
	} else {
		if (down_interruptible(&drv_context->isr_lock))
			return -ERESTARTSYS;
		if (drv_context->dev_flags & DEV_FLAG_SHUTDOWN) {
			status = -ENODEV;
		} else {
			if (sii_drv_tx_hdcp_protection_set(drv_context->drv_obj,
				(enum sii_drv_hdcp_link_version)hdcp_ctrl))
				status = -ENODATA;
			else
				status = count;
		}

		up(&drv_context->isr_lock);
	}

	return status;
}
/*
ssize_t set_hdcp_version(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t count)
{
	struct cp8630_driver_context *drv_context = dev_get_drvdata(dev);
	uint8_t hdcp_version;
	int status = 1;

	status = kstrtou8(buf, 0, &hdcp_version);
	if (-ERANGE == status) {
		sii_log_err("ERANGE %s\n", buf);
	} else if (1 == status) {
		sii_log_err("EINVAL %s\n", buf);
	} else if (status != 0) {
		sii_log_err("status: %d	buf: %c%s%c\n", status, '"', buf, '"');
	} else {
		if (down_interruptible(&drv_context->isr_lock))
			return -ERESTARTSYS;
		if (drv_context->dev_flags & DEV_FLAG_SHUTDOWN) {
			status = -ENODEV;
		} else {
			if (sii_drv_tx_hdcp_auth_type_set(drv_context->drv_obj,
				(enum sii_drv_hdcp_link_version)hdcp_version))
				status = -ENODATA;
			status = count;
		}

		up(&drv_context->isr_lock);
	}

	return status;
}
*/
/*
 * show_ds_hdcp_status() - Handle read requst to the attribute file
 * ds_hdcp_status.
 */
ssize_t show_ds_hdcp_status(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct cp8630_driver_context *drv_context = dev_get_drvdata(dev);
	enum sii_drv_hdcp_status hdcp_status;
	int status = -ENODATA;

	if (!sii_drv_tx_hdcp_status_get(drv_context->drv_obj, &hdcp_status))
		return scnprintf(buf, PAGE_SIZE, "%d", hdcp_status);
	/*switch (hdcp_status) {
	case SII_DRV_HDCP_STATUS__OFF:
		status = scnprintf(buf, PAGE_SIZE, "%d", hdcp_status);
		break;
	case SII_DRV_HDCP_STATUS__SUCCESS_1X:
		status = scnprintf(buf, PAGE_SIZE, "%d", hdcp_status);
		break;
	case SII_DRV_HDCP_STATUS__SUCCESS_22:
		status = scnprintf(buf, PAGE_SIZE, "%d", hdcp_status);
		break;
	case SII_DRV_HDCP_STATUS__AUTHENTICATING:
		status = scnprintf(buf, PAGE_SIZE, "%d", hdcp_status);
		break;
	case SII_DRV_HDCP_STATUS__FAILED:
		status = scnprintf(buf, PAGE_SIZE, "%d", hdcp_status);
		break;
	}*/
	return status;
}

/*
 * show_ds_hdcp_fail_reason() - Handle read requst to the attribute file
 * ds_hdcp_fail_reason.
 */
ssize_t show_ds_hdcp_fail_reason(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct cp8630_driver_context *drv_context = dev_get_drvdata(dev);
	enum sii_drv_hdcp_failure hdcp_fail_reason;
	int status = -ENODATA;

	if (!sii_drv_tx_hdcp_failure_reason_get(
		drv_context->drv_obj, &hdcp_fail_reason))
		return scnprintf(buf, PAGE_SIZE, "%d", hdcp_fail_reason);

	return status;
}

/*
 * show_ds_hdcp_version() - Handle read requst to the attribute file
 * ds_hdcp_version.
 */
ssize_t show_ds_hdcp_version(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct cp8630_driver_context *drv_context = dev_get_drvdata(dev);
	enum sii_drv_hdcp_link_version ds_hdcp_ver;
	int status = -ENODATA;

	if (!sii_drv_tx_hdcp_version_get(drv_context->drv_obj, &ds_hdcp_ver))
		return scnprintf(buf, PAGE_SIZE, "%d", ds_hdcp_ver);

	return status;
}

ssize_t show_ds_bksv_list(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct cp8630_driver_context *drv_context = dev_get_drvdata(dev);
	struct sii_drv_hdcp_ksv_list bksv_list;
	enum sii_drv_hdcp_status hdcp_status;
	uint8_t *listptr;
	uint16_t length = 0;
	int status = -ENODATA;

	sii_drv_tx_hdcp_status_get(drv_context->drv_obj, &hdcp_status);

	if (!sii_drv_tx_hdcp_bksv_list_get(drv_context->drv_obj, &bksv_list)) {
		if (bksv_list.p_list_start != NULL) {
			status = 0;
			length = bksv_list.length;
			listptr = bksv_list.p_list_start;
			if (hdcp_status == SII_DRV_HDCP_STATUS__SUCCESS_22)
				sii_log_debug("Sink ReceiverID List: ");
			else
				sii_log_debug("Sink BKSV List: ");
			while (length-- && listptr) {
				status += scnprintf(&buf[status], PAGE_SIZE,
					"0x%02x ", *listptr);
				sii_log_print(" %02X", *listptr);
				listptr++;
			}
			status += scnprintf(&buf[status], PAGE_SIZE, "\n");
			sii_log_print("\n");
		}
	}

	return status;
}

ssize_t show_ds_hdcp2_content_type(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct cp8630_driver_context *drv_context = dev_get_drvdata(dev);
	enum sii_drv_hdcp_content_type content_type;

	if (!sii_drv_tx_hdcp2_content_type_get(drv_context->drv_obj,
		&content_type))
		return scnprintf(buf, PAGE_SIZE, "%d\n", content_type);
	return -ENODATA;
}

ssize_t show_ds_hdcp_rptr_mode(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct cp8630_driver_context *drv_context = dev_get_drvdata(dev);
	enum hdcp_repeater_mode ds_repeater_mode = TX_HDCP_RPTR_NONE;
	int status = -ENODATA;

	if (!sii_drv_tx_hdcp_repeater_info(drv_context->drv_obj,
			&ds_repeater_mode)) {
		status = 0;
		if (ds_repeater_mode == TX_HDCP_RPTR_NONE) {
			sii_log_debug("DS device is not Repeater\n");
		} else {
			if (ds_repeater_mode == TX_HDCP_RPTR_22)
				sii_log_debug("DS device is a HDCP22 Repeater\n");
			else
				sii_log_debug("DS device is a HDCP1X Repeater\n");
			}
				status += scnprintf(&buf[status], PAGE_SIZE,
			"%d\n", ds_repeater_mode);
	}
	return status;
}

ssize_t show_ds_hdcp_topology(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct cp8630_driver_context *drv_context = dev_get_drvdata(dev);
	struct sii_drv_hdcp_topology ds_topology = {0};
	int status = -ENODATA;

	if (!sii_drv_tx_hdcp_topology_get(drv_context->drv_obj,
				&ds_topology)) {
		status = 0;
		status += scnprintf(&buf[status], PAGE_SIZE,
			"%u\n", ds_topology.seq_num_v);
		sii_log_debug("seq num v = 0x%08x\n",ds_topology.seq_num_v);

		status += scnprintf(&buf[status], PAGE_SIZE,
			"%d\n", ds_topology.hdcp1x_device_ds);
		sii_log_debug("HDCP1X Device in topology = %s\n",
			ds_topology.hdcp1x_device_ds?"YES":"NO");

		status += scnprintf(&buf[status], PAGE_SIZE,
			"%d\n", ds_topology.hdcp20_repeater_ds);
		sii_log_debug("HDCP2.2 repeater in topology = %s\n",
			ds_topology.hdcp20_repeater_ds?"YES":"NO");

		status += scnprintf(&buf[status], PAGE_SIZE,
			"%d\n", ds_topology.max_cascade_exceeded);
		sii_log_debug("Max cascade exceeded in topology = %d\n",
			ds_topology.max_cascade_exceeded);

		status += scnprintf(&buf[status], PAGE_SIZE,
			"%d\n", ds_topology.max_devs_exceeded);
		sii_log_debug("Max devs exceeded in topology = %d\n",
			ds_topology.max_devs_exceeded);

		status += scnprintf(&buf[status], PAGE_SIZE,
			"%u\n", ds_topology.device_count);
		sii_log_debug("Device count = %u\n",
			ds_topology.device_count);

		status += scnprintf(&buf[status], PAGE_SIZE,
			"%d\n", ds_topology.depth);
		sii_log_debug("Depth = %d\n", ds_topology.depth);

	}
	return status;
}

ssize_t set_ds_hdcp2_content_type(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t count)
{
	struct cp8630_driver_context *drv_context = dev_get_drvdata(dev);
	enum sii_drv_hdcp_content_type content_type;
	uint8_t input;
	int status = 1;

	status = kstrtou8(buf, 0, &input);
	if (-ERANGE == status) {
		sii_log_err("ERANGE %s\n", buf);
	} else if (1 == status) {
		sii_log_err("EINVAL %s\n", buf);
	} else if (status != 0) {
		sii_log_err("status: %d	buf: %c%s%c\n", status, '"', buf, '"');
	} else {
		if (down_interruptible(&drv_context->isr_lock))
			return -ERESTARTSYS;
		if (drv_context->dev_flags & DEV_FLAG_SHUTDOWN) {
			status = -ENODEV;
		} else {
			status = count;
			if (0 == input) {
				content_type = SII_DRV_HDCP_CONTENT_TYPE__0;
				if (sii_drv_tx_hdcp2_content_type_set(
					drv_context->drv_obj, &content_type))
					status = -ENODATA;
			} else if (1 == input) {
				content_type = SII_DRV_HDCP_CONTENT_TYPE__1;
				if (sii_drv_tx_hdcp2_content_type_set(
					drv_context->drv_obj, &content_type))
					status = -ENODATA;
			} else {
				sii_log_err("invalid content_type: %d\n",
					content_type);
				status = 1;
			}
		}

		up(&drv_context->isr_lock);
	}

	return status;
}

/*
 * show_hdcp_events() - Handle read requst to the attribute file
 * hdcp_events.
 */
ssize_t show_hdcp_events(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct cp8630_driver_context *drv_context = dev_get_drvdata(dev);
	enum sii_drv_hdcp_gen_event hdcp_events;
	int status = -ENODATA;

	if (down_interruptible(&drv_context->isr_lock))
			return -ERESTARTSYS;
	if (!sii_drv_tx_hdcp_events_get(drv_context->drv_obj, &hdcp_events))
		status = scnprintf(buf, PAGE_SIZE, "0x%08x", hdcp_events);
	up(&drv_context->isr_lock);
	return status;
}

ssize_t set_ds_scdc_scramble_ctrl(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t count)
{
	struct cp8630_driver_context *drv_context = dev_get_drvdata(dev);
	uint8_t scramble_enable;
	int status = 1;

	status = kstrtou8(buf, 0, &scramble_enable);
	if (-ERANGE == status) {
		sii_log_err("ERANGE %s\n", buf);
	} else if (1 == status) {
		sii_log_err("EINVAL %s\n", buf);
	} else if (status != 0) {
		sii_log_err("status: %d	buf: %c%s%c\n", status, '"', buf, '"');
	} else {
		if (down_interruptible(&drv_context->isr_lock))
			return -ERESTARTSYS;
		if (drv_context->dev_flags & DEV_FLAG_SHUTDOWN) {
			status = -ENODEV;
		} else {
			if (sii_drv_tx_scdc_scramble_enable
				(drv_context->drv_obj,
				(bool) scramble_enable)) {
				sii_log_print("Scramble enable failed\n");
				status = -ENODATA;
			} else {
				sii_log_print("Scramble enable succeeded\n");
				status = count;
				;
			}
		}

		up(&drv_context->isr_lock);
	}

	return status;
}

ssize_t set_ds_scdc_debug_enable(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t count)
{
	struct cp8630_driver_context *drv_context = dev_get_drvdata(dev);
	uint8_t debug_enable;
	int status = 1;

	status = kstrtou8(buf, 0, &debug_enable);
	if (-ERANGE == status) {
		sii_log_err("ERANGE %s\n", buf);
	} else if (1 == status) {
		sii_log_err("EINVAL %s\n", buf);
	} else if (status != 0) {
		sii_log_err("status: %d	buf: %c%s%c\n", status, '"', buf, '"');
	} else {
		if (down_interruptible(&drv_context->isr_lock))
			return -ERESTARTSYS;
		if (drv_context->dev_flags & DEV_FLAG_SHUTDOWN) {
			status = -ENODEV;
		} else {
			if (sii_drv_tx_scdc_debug_enable
				(drv_context->drv_obj,
				(bool) debug_enable)) {
				sii_log_print("SCDC debug enable failed\n");
				status = -ENODATA;
			} else {
				sii_log_print("SCDC debug enable succeeded");
				status = count;
				;
			}
		}

		up(&drv_context->isr_lock);
	}

	return status;
}

#ifdef RX_DESCRAMBLE_THROUGH_API
ssize_t set_us_scdc_tmds_config(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t count)
{
	struct cp8630_driver_context *drv_context = dev_get_drvdata(dev);
	int status = 1;
	unsigned long value;
	u8 data[MAX_DEBUG_TRANSFER_SIZE];
	int i;
	char *str;
    char l_s8input = 0;
	char *pinput = &l_s8input;

	sii_log_debug("received string: %s\n", buf);

	if (pinput == 0)
		return 1;
	memcpy(pinput, buf, count);

	str = pinput;
	for (i = 0; (i < MAX_DEBUG_TRANSFER_SIZE) && ('\0' != *str); i++) {

		status = si_strtoul(&str, 0, &value);
		if (-ERANGE == status) {
			sii_log_debug("ERANGE %s%s%s\n", ANSI_ESC_RED_TEXT, str,
				ANSI_ESC_RESET_TEXT);
			goto exit_pinput;
		} else if (1 == status) {
			sii_log_debug("EINVAL %s%s%s\n", ANSI_ESC_RED_TEXT, str,
				ANSI_ESC_RESET_TEXT);
			goto exit_pinput;
		} else if (status != 0) {
			sii_log_debug("status:%d %s%s%s\n", status,
				ANSI_ESC_RED_TEXT, str, ANSI_ESC_RESET_TEXT);
			goto exit_pinput;
		} else if (value > 0xFF) {
			sii_log_debug("value:0x%x str:%s%s%s\n", (uint)value,
				ANSI_ESC_RED_TEXT, str, ANSI_ESC_RESET_TEXT);
			goto exit_pinput;
		} else {
			data[i] = value;
		}
	}

	if (i == 0) {
		sii_log_err("No data specified\n");
		goto exit_pinput;
	}

	if (down_interruptible(&drv_context->isr_lock))
		return -ERESTARTSYS;
	if (drv_context->dev_flags & DEV_FLAG_SHUTDOWN) {
		status = -ENODEV;
	} else {
		if (sii_drv_rx_scdc_tmds_config
			(drv_context->drv_obj,
			(bool) data[0], (bool) data[1])) {
			sii_log_print("Descrambling enable Failed\n");
			status = -ENODATA;
		} else {
			sii_log_print("Descrambling enable succeeded");
			status = count;
		}
	}
	up(&drv_context->isr_lock);
exit_pinput: //kfree(pinput);
	return status;
}

ssize_t show_us_scramble_status(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct cp8630_driver_context *drv_context = dev_get_drvdata(dev);
	uint8_t us_scramble_status = 0;
	int status = -ENODATA;

	if (down_interruptible(&drv_context->isr_lock))
		status = -ERESTARTSYS;

	if (drv_context->dev_flags & DEV_FLAG_SHUTDOWN)
		status = -ENODEV;

	 else {
		if (!sii_drv_rx_scramble_status(
			drv_context->drv_obj,
			&us_scramble_status))
			status = scnprintf(
			buf, PAGE_SIZE, "%d\n", us_scramble_status);
	}

	up(&drv_context->isr_lock);

	return status;
}


#endif

ssize_t show_ds_scdc_source_scramble_status(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct cp8630_driver_context *drv_context = dev_get_drvdata(dev);
	enum sii_drv_scdc_source_scrmble_status source_scramble_status;
	int status = -ENODATA;

	if (down_interruptible(&drv_context->isr_lock))
		status = -ERESTARTSYS;

	if (drv_context->dev_flags & DEV_FLAG_SHUTDOWN)
		status = -ENODEV;

	 else {
		if (!sii_drv_tx_scdc_source_scramble_status(
			drv_context->drv_obj,
			&source_scramble_status))
			status = scnprintf(
				buf, PAGE_SIZE, "%d\n", source_scramble_status);
		/* status = -ENODATA;*/
	}

	up(&drv_context->isr_lock);

	return status;

}

ssize_t show_ds_dsc_compression_status(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct cp8630_driver_context *drv_context = dev_get_drvdata(dev);
	bool compression_status = false;
	int status = -ENODATA;

	if (down_interruptible(&drv_context->isr_lock))
		return -ERESTARTSYS;
	if (drv_context->dev_flags & DEV_FLAG_SHUTDOWN) {
		status = -ENODEV;

	} else {
		if (!sii_drv_tx_dsc_compression_status(
			drv_context->drv_obj, &compression_status))
			status = scnprintf(
				buf, PAGE_SIZE, "%d\n", compression_status);
	}

	up(&drv_context->isr_lock);

	return status;

}

ssize_t show_ds_dsc_compression_ratio_status(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct cp8630_driver_context *drv_context = dev_get_drvdata(dev);
	enum sii_drv_dsc_compression_ratio compression_ratio;
	int status = -ENODATA;

	if (down_interruptible(&drv_context->isr_lock))
		return -ERESTARTSYS;
	if (drv_context->dev_flags & DEV_FLAG_SHUTDOWN)
		status = -ENODEV;

	else {
		if (!sii_drv_tx_dsc_compression_ratio(drv_context->drv_obj,
				&compression_ratio))
			status = scnprintf(
				buf, PAGE_SIZE, "%d\n", compression_ratio);
	}

	up(&drv_context->isr_lock);

	return status;

}

ssize_t set_ds_dsc_ctrl(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t count)
{
	struct cp8630_driver_context *drv_context = dev_get_drvdata(dev);
	uint8_t compression;
	int status = 1;

	status = kstrtou8(buf, 0, &compression);

	if (-ERANGE == status) {
		sii_log_err("ERANGE %s\n", buf);
	} else if (1 == status) {
		sii_log_err("EINVAL %s\n", buf);
	} else if (status != 0) {
		sii_log_err("status: %d	buf: %c%s%c\n", status, '"', buf, '"');
	} else {
		if (down_interruptible(&drv_context->isr_lock)) {
			sii_log_print("isr_lock not released");
			return -ERESTARTSYS;
		}
		if (drv_context->dev_flags & DEV_FLAG_SHUTDOWN) {
			sii_log_print("flags Error\n");
			status = -ENODEV;
		} else {
			switch (compression) {
			case SII_DRV_DSC_COMPRESSION_RATIO__NONE:
			case SII_DRV_DSC_COMPRESSION_RATIO__2X:
			case SII_DRV_DSC_COMPRESSION_RATIO__3X:
				if (sii_drv_tx_dsc_configure(
					drv_context->drv_obj, compression)) {
					sii_log_err("DSC enable/disable failed\n");
					status = -ENODATA;
				} else {
					sii_log_info("DSC enable/disable succeeded\n");
					status = count;
				}
				break;
			default:
				sii_log_err("Invalid Compression Ratio.\n");
				status = 1;
			}
		}

		up(&drv_context->isr_lock);
	}

	return status;
}

ssize_t set_ds_dsc_compression(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t count)
{
	struct cp8630_driver_context *drv_context = dev_get_drvdata(dev);
	uint8_t dsc_compression;
	int status = 1;

	status = kstrtou8(buf, 0, &dsc_compression);
	if (-ERANGE == status) {
		sii_log_err("ERANGE %s\n", buf);
	} else if (1 == status) {
		sii_log_err("EINVAL %s\n", buf);
	} else if (status != 0) {
		sii_log_err("status: %d	buf: %c%s%c\n", status, '"', buf, '"');
	} else {
		if (down_interruptible(&drv_context->isr_lock)) {
			sii_log_print("isr lock not released\n");
			return -ERESTARTSYS;
		}
		if (drv_context->dev_flags & DEV_FLAG_SHUTDOWN) {
			sii_log_print("flags error\n");
			status = -ENODEV;
		} else {
			if (!sii_drv_tx_dsc_configure(drv_context->drv_obj,
				dsc_compression))
				status = count;
			else
				status = -ENODATA;

		}

		up(&drv_context->isr_lock);
	}

	return status;
}

static char *get_clrspc_string(int clrspc)
{
#define DUMMY_PRINT(x)
#define CLRPSC_CASE(clrspc)			\
	do {					\
		/* fall-through */		\
		case clrspc:			\
			DUMMY_PRINT(clrspc);	\
			return #clrspc;		\
	} while (0)
	switch (clrspc) {
		CLRPSC_CASE(SII_DRV_CLRSPC__PASSTHRU);
		CLRPSC_CASE(SII_DRV_CLRSPC__YC444_601);
		CLRPSC_CASE(SII_DRV_CLRSPC__YC422_601);
		CLRPSC_CASE(SII_DRV_CLRSPC__YC420_601);
		CLRPSC_CASE(SII_DRV_CLRSPC__YC444_709);
		CLRPSC_CASE(SII_DRV_CLRSPC__YC422_709);
		CLRPSC_CASE(SII_DRV_CLRSPC__YC420_709);
		CLRPSC_CASE(SII_DRV_CLRSPC__XVYCC444_601);
		CLRPSC_CASE(SII_DRV_CLRSPC__XVYCC422_601);
		CLRPSC_CASE(SII_DRV_CLRSPC__XVYCC420_601);
		CLRPSC_CASE(SII_DRV_CLRSPC__XVYCC444_709);
		CLRPSC_CASE(SII_DRV_CLRSPC__XVYCC422_709);
		CLRPSC_CASE(SII_DRV_CLRSPC__XVYCC420_709);
		CLRPSC_CASE(SII_DRV_CLRSPC__YC444_2020);
		CLRPSC_CASE(SII_DRV_CLRSPC__YC422_2020);
		CLRPSC_CASE(SII_DRV_CLRSPC__YC420_2020);
		CLRPSC_CASE(SII_DRV_CLRSPC__RGB_DEFAULT);
		CLRPSC_CASE(SII_DRV_CLRSPC__RGB_FULL);
		CLRPSC_CASE(SII_DRV_CLRSPC__RGB_LIMITED);
	}
	return "unknown";
}
ssize_t set_ds_output_clrspc(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t count)
{
	struct cp8630_driver_context *drv_context = dev_get_drvdata(dev);
	uint8_t clrspc;
	int status = 1;

	status = kstrtou8(buf, 0, &clrspc);
	if (-ERANGE == status) {
		sii_log_err("ERANGE %s\n", buf);
	} else if (1 == status) {
		sii_log_err("EINVAL %s\n", buf);
	} else if (status != 0) {
		sii_log_err("status: %d	buf: %c%s%c\n", status, '"', buf, '"');
	} else {
		if (down_interruptible(&drv_context->isr_lock))
			return -ERESTARTSYS;
		if (drv_context->dev_flags & DEV_FLAG_SHUTDOWN) {
			status = -ENODEV;
		} else {
			status = count;
			sii_log_print("Set ClrSpc to %s\n",
				get_clrspc_string(clrspc));
			switch (clrspc) {
			case SII_DRV_CLRSPC__PASSTHRU:
			case SII_DRV_CLRSPC__YC444_601:
			case SII_DRV_CLRSPC__YC422_601:
			case SII_DRV_CLRSPC__YC444_709:
			case SII_DRV_CLRSPC__YC422_709:
			case SII_DRV_CLRSPC__XVYCC444_601:
			case SII_DRV_CLRSPC__XVYCC422_601:
			case SII_DRV_CLRSPC__XVYCC444_709:
			case SII_DRV_CLRSPC__XVYCC422_709:
			case SII_DRV_CLRSPC__YC444_2020:
			case SII_DRV_CLRSPC__YC422_2020:
			case SII_DRV_CLRSPC__RGB_DEFAULT:
			case SII_DRV_CLRSPC__RGB_FULL:
			case SII_DRV_CLRSPC__RGB_LIMITED:
				if (!sii_drv_output_color_space_set(
					drv_context->drv_obj, clrspc))
					status = -ENODATA;
				break;
			default:
				sii_log_err("Invalid Color Space : %d\n",
					clrspc);
				status = 1;
				break;
			}
		}

		up(&drv_context->isr_lock);
	}

	return status;
}

ssize_t set_ds_hvsync_pol(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t count)
{
	struct cp8630_driver_context *drv_context = dev_get_drvdata(dev);
	uint8_t hvsyncpol = 0;
	int status = 1;

	status = kstrtou8(buf, 0, &hvsyncpol);
	if (-ERANGE == status) {
		sii_log_err("ERANGE %s\n", buf);
	} else if (1 == status) {
		sii_log_err("EINVAL %s\n", buf);
	} else if (status != 0) {
		sii_log_err("status: %d	buf: %c%s%c\n", status, '"', buf, '"');
	} else {
		if (down_interruptible(&drv_context->isr_lock))
			return -ERESTARTSYS;
		if (drv_context->dev_flags & DEV_FLAG_SHUTDOWN) {
			status = -ENODEV;
		} else {
			status = count;
			switch (hvsyncpol) {
			case SII_DRV_HV_SYNC_POL__HPVP:
			case SII_DRV_HV_SYNC_POL__HPVN:
			case SII_DRV_HV_SYNC_POL__HNVP:
			case SII_DRV_HV_SYNC_POL__HNVN:
				if (!sii_drv_rx_out_h_v_sync_polarity_set(
					drv_context->drv_obj, hvsyncpol))
					status = -ENODATA;
				break;
			default:
				sii_log_err("Invalid HVSYNC Polarity : %d\n",
					hvsyncpol);
				status = 1;
				break;
			}
		}

		up(&drv_context->isr_lock);
	}

	return status;
}

ssize_t show_ds_hvsync_pol(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct cp8630_driver_context *drv_context = dev_get_drvdata(dev);
	enum sii_hv_sync_pol hvsync_pol;

	if (!sii_drv_rx_hv_sync_polarity_get(drv_context->drv_obj, &hvsync_pol))
		return scnprintf(buf, PAGE_SIZE, "%d", hvsync_pol);
	return -ENODATA;
	/*switch (hvsync_pol) {
	case SII_DRV_HV_SYNC_POL__HPVP:
		return scnprintf(buf, PAGE_SIZE, "HPVP");
	case SII_DRV_HV_SYNC_POL__HPVN:
		return scnprintf(buf, PAGE_SIZE, "HPVN");
	case SII_DRV_HV_SYNC_POL__HNVP:
		return scnprintf(buf, PAGE_SIZE, "HNVP");
	case SII_DRV_HV_SYNC_POL__HNVN:
		return scnprintf(buf, PAGE_SIZE, "HNVN");
	}*/
}

ssize_t show_us_5v_status(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct cp8630_driver_context *drv_context = dev_get_drvdata(dev);
	bool plus5v_status;

	if (!sii_drv_rx_plus5v_status_get(drv_context->drv_obj, &plus5v_status))
		return scnprintf(buf, PAGE_SIZE, "%d\n", plus5v_status);
	return -ENODATA;
}

/*
 * set_device_edid() - Handle write request to the us_edid attribute file.
 *
 * This file is used to set the edid data for rx.
 *
 * the format of string in buf must be:
 *	edid_byt_0 edid_byt_1 ... edid_byte_N
 *
 */
ssize_t set_device_edid(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t count)
{
	struct cp8630_driver_context *drv_context = dev_get_drvdata(dev);
	unsigned long value;
	struct sii_edid edid;
	int i;
	char *str;
	int status = 1;
    char l_s8input = 0;
	char *pinput = &l_s8input;

	sii_log_debug("received string: %s\n", buf);

	if (pinput == 0)
		return 1;
	memcpy(pinput, buf, count);

	/*
	 * Parse the input string and extract the edid data
	 */
	str = pinput;
	for (i = 0; (i < SII_EDID_MAX_LEN) && ('\0' != *str); i++) {
		status = si_strtoul(&str, 0, &value);
		if (-ERANGE == status) {
			sii_log_err("ERANGE %s%s%s\n", ANSI_ESC_RED_TEXT, str,
				ANSI_ESC_RESET_TEXT);
			goto exit_edid_data;
		} else if (1 == status) {
			sii_log_err("EINVAL %s%s%s\n", ANSI_ESC_RED_TEXT, str,
				ANSI_ESC_RESET_TEXT);
			goto exit_edid_data;
		} else if (status != 0) {
			sii_log_err("status:%d %s%s%s\n", status,
				ANSI_ESC_RED_TEXT, str, ANSI_ESC_RESET_TEXT);
			goto exit_edid_data;
		} else if (value > 0xFF) {
			sii_log_err("value:0x%x str:%s%s%s\n", (uint)value,
				ANSI_ESC_RED_TEXT, str, ANSI_ESC_RESET_TEXT);
			goto exit_edid_data;
		} else {
			edid.b[i] = value;
		}
	}

	if (i == 0) {
		sii_log_err("No data specified\n");
		goto exit_edid_data;
	}

	if (down_interruptible(&drv_context->isr_lock)) {
		status = -ERESTARTSYS;
		goto exit_edid_data;
	}

	if (drv_context->dev_flags & DEV_FLAG_SHUTDOWN) {
		status = -ENODEV;
	} else {
		if (sii_drv_rx_edid_set(drv_context->drv_obj, &edid))
			status = -ENODATA;
		else
			status = count;
	}

	up(&drv_context->isr_lock);

exit_edid_data: kfree(pinput);

	return status;
}

ssize_t set_us_hpd(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t count)
{
	struct cp8630_driver_context *drv_context = dev_get_drvdata(dev);
	uint8_t upstream_hpd;
	int status = 1;

	status = kstrtou8(buf, 0, &upstream_hpd);
	if (-ERANGE == status) {
		sii_log_err("ERANGE %s\n", buf);
	} else if (1 == status) {
		sii_log_err("EINVAL %s\n", buf);
	} else if (status != 0) {
		sii_log_err("status: %d	buf: %c%s%c\n", status, '"', buf, '"');
	} else {
		if (down_interruptible(&drv_context->isr_lock))
			return -ERESTARTSYS;
		if (drv_context->dev_flags & DEV_FLAG_SHUTDOWN) {
			status = -ENODEV;
		} else {
			if (sii_drv_rx_hot_plug_set(drv_context->drv_obj,
				(bool)upstream_hpd))
				status = -ENODATA;
			else
				status = count;
		}

		up(&drv_context->isr_lock);
	}

	return status;
}

ssize_t show_us_tmds_mode(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct cp8630_driver_context *drv_context = dev_get_drvdata(dev);
	enum sii_tmds_mode tmds_mode;

	if (!sii_drv_rx_tmds_mode_get(drv_context->drv_obj, &tmds_mode))
		return scnprintf(buf, PAGE_SIZE, "%d\n", tmds_mode);
	return -ENODATA;
	/*switch (tmds_mode) {
	case 0:
		return scnprintf(buf, PAGE_SIZE, "off");
	case 1:
		return scnprintf(buf, PAGE_SIZE, "dvi");
	case 2:
		return scnprintf(buf, PAGE_SIZE, "hdmi1");
	case 3:
		return scnprintf(buf, PAGE_SIZE, "hdmi2");
	case 4:
		return scnprintf(buf, PAGE_SIZE, "auto");
	default:
		sii_log_debug("invalid tmds mode\n");
		return status;
	 }*/
}

ssize_t show_rx_clr_spc(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct cp8630_driver_context *drv_context = dev_get_drvdata(dev);
	enum sii_drv_clr_spc clr_spc;

	sii_drv_rx_input_color_space_get(drv_context->drv_obj, &clr_spc);
	switch (clr_spc) {
	case SII_DRV_CLRSPC__YC444_601:
		return scnprintf(buf, PAGE_SIZE, "SII_DRV_CLRSPC__YC444_601\n");
	case SII_DRV_CLRSPC__YC422_601:
		return scnprintf(buf, PAGE_SIZE, "SII_DRV_CLRSPC__YC422_601\n");
	case SII_DRV_CLRSPC__YC420_601:
		return scnprintf(buf, PAGE_SIZE, "SII_DRV_CLRSPC__YC420_601\n");
	case SII_DRV_CLRSPC__YC444_709:
		return scnprintf(buf, PAGE_SIZE, "SII_DRV_CLRSPC__YC444_709\n");
	case SII_DRV_CLRSPC__YC422_709:
		return scnprintf(buf, PAGE_SIZE, "SII_DRV_CLRSPC__YC422_709\n");
	case SII_DRV_CLRSPC__YC420_709:
		return scnprintf(buf, PAGE_SIZE, "SII_DRV_CLRSPC__YC420_709\n");
	case SII_DRV_CLRSPC__XVYCC444_601:
		return scnprintf(buf, PAGE_SIZE, "SII_DRV_CLRSPC__XVYCC444_601\n");
	case SII_DRV_CLRSPC__XVYCC422_601:
		return scnprintf(buf, PAGE_SIZE, "SII_DRV_CLRSPC__XVYCC422_601\n");
	case SII_DRV_CLRSPC__XVYCC420_601:
		return scnprintf(buf, PAGE_SIZE, "SII_DRV_CLRSPC__XVYCC420_601\n");
	case SII_DRV_CLRSPC__XVYCC444_709:
		return scnprintf(buf, PAGE_SIZE, "SII_DRV_CLRSPC__XVYCC444_709\n");
	case SII_DRV_CLRSPC__XVYCC422_709:
		return scnprintf(buf, PAGE_SIZE, "SII_DRV_CLRSPC__XVYCC422_709\n");
	case SII_DRV_CLRSPC__XVYCC420_709:
		return scnprintf(buf, PAGE_SIZE, "SII_DRV_CLRSPC__XVYCC420_709\n");
	case SII_DRV_CLRSPC__YC444_2020:
		return scnprintf(buf, PAGE_SIZE, "SII_DRV_CLRSPC__YC444_2020\n");
	case SII_DRV_CLRSPC__YC422_2020:
		return scnprintf(buf, PAGE_SIZE, "SII_DRV_CLRSPC__YC422_2020\n");
	case SII_DRV_CLRSPC__YC420_2020:
		return scnprintf(buf, PAGE_SIZE, "SII_DRV_CLRSPC__YC420_2020\n");
	case SII_DRV_CLRSPC__RGB_DEFAULT:
		return scnprintf(buf, PAGE_SIZE, "SII_DRV_CLRSPC__RGB_DEFAULT\n");
	case SII_DRV_CLRSPC__RGB_FULL:
		return scnprintf(buf, PAGE_SIZE, "SII_DRV_CLRSPC__RGB_FULL\n");
	case SII_DRV_CLRSPC__RGB_LIMITED:
		return scnprintf(buf, PAGE_SIZE, "SII_DRV_CLRSPC__RGB_LIMITED\n");
	default:
		sii_log_debug("invalid Clr Spc :%d\n",clr_spc);
		return -ENODATA;
	 }
}


ssize_t show_us_hdcp_status(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct cp8630_driver_context *drv_context = dev_get_drvdata(dev);
	enum sii_drv_hdcp_status hdcp_status;

	if (!sii_drv_rx_hdcp_status_get(drv_context->drv_obj, &hdcp_status))
		return scnprintf(buf, PAGE_SIZE, "%d", hdcp_status);
	return -ENODATA;
	/*switch (hdcp_status) {
	case SII_DRV_HDCP_STATUS__OFF:
		return scnprintf(buf, PAGE_SIZE, "off");
	case SII_DRV_HDCP_STATUS__SUCCESS_1X:
		return scnprintf(buf, PAGE_SIZE, "success_hdcp14");
	case SII_DRV_HDCP_STATUS__SUCCESS_22:
		return scnprintf(buf, PAGE_SIZE, "success_hdcp22");
	case SII_DRV_HDCP_STATUS__AUTHENTICATING:
		return scnprintf(buf, PAGE_SIZE, "authenticating");
	case SII_DRV_HDCP_STATUS__FAILED:
		return scnprintf(buf, PAGE_SIZE, "failed");
	}*/
}

/*
 * show_debug_level() - Handle read request to the debug_level attribute file.
 *
 * The return value is the number characters written to buf, or EAGAIN
 * if the driver is busy and cannot service the read request immediately.
 * If EAGAIN is returned the caller should wait a little and retry the
 * read.
 */
ssize_t show_debug_level(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct cp8630_driver_context *drv_context = dev_get_drvdata(dev);
	int status = 1;

	if (down_interruptible(&drv_context->isr_lock)) {
		sii_log_err("-ERESTARTSYS\n");
		return -ERESTARTSYS;
	}

	if (drv_context->dev_flags & DEV_FLAG_SHUTDOWN) {
		sii_log_err("-ENODEV\n");
		status = -ENODEV;
		goto err_exit;
	}

	status = scnprintf(buf, PAGE_SIZE, "level=%d %s\n", debug_level,
		debug_reg_dump ? "(includes reg dump)" : "");
	sii_log_info("buf:%c%s%c\n", '"', buf, '"');

err_exit: up(&drv_context->isr_lock);
	/*this should be a separate sysfs!!!
	 si_dump_important_regs(
	 (struct drv_hw_context *)drv_context->drv_obj);*/

	return status;
}

/*
 * set_debug_level() - Handle write request to the debug_level attribute file.
 *
 * Writes to this file cause a RAP message with the specified action code
 * to be sent to the downstream device.
 */
ssize_t set_debug_level(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t count)
{
	struct cp8630_driver_context *drv_context = dev_get_drvdata(dev);
	int status;
	long new_debug_level = 0x100;

	sii_log_debug("received string: %s\n", buf);

	if (buf != NULL) {
		/* BUGZILLA 27012 no prefix here, only on module parameter. */
		status = kstrtol(buf, 0, &new_debug_level);
		if (status != 0) {
			sii_log_err("Invalid debug_level: 0x%02lX\n",
				new_debug_level);
			goto err_exit;
		}
	} else {
		sii_log_err("Missing debug_level parameter\n");
		status = 1;
		goto err_exit;
	}

	if (down_interruptible(&drv_context->isr_lock))
		return -ERESTARTSYS;

	if (drv_context->dev_flags & DEV_FLAG_SHUTDOWN) {
		status = -ENODEV;
		goto err_exit;
	}
	debug_level = new_debug_level;
	status = count;

	sii_log_debug("new debug level = %d\n", debug_level);

err_exit: up(&drv_context->isr_lock);

	return status;
}

/*
 * show_debug_reg_dump()
 *
 * Handle read request to the debug_reg_dump attribute file.
 *
 * The return value is the number characters written to buf, or EAGAIN
 * if the driver is busy and cannot service the read request immediately.
 * If EAGAIN is returned the caller should wait a little and retry the
 * read.
 */
ssize_t show_debug_reg_dump(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct cp8630_driver_context *drv_context = dev_get_drvdata(dev);
	int status = 1;

	if (down_interruptible(&drv_context->isr_lock)) {
		sii_log_err("-ERESTARTSYS\n");
		return -ERESTARTSYS;
	}

	if (drv_context->dev_flags & DEV_FLAG_SHUTDOWN) {
		sii_log_err("-ENODEV\n");
		status = -ENODEV;
		goto err_exit;
	}

	status = scnprintf(buf, PAGE_SIZE, "%s\n",
		debug_reg_dump ? "(includes reg dump)" : "");
	sii_log_info("buf:%c%s%c\n", '"', buf, '"');

err_exit: up(&drv_context->isr_lock);

	return status;
}

/*
 * set_debug_reg_dump()
 *
 * Handle write request to the debug_reg_dump attribute file.
 *
 * Writes to this file cause a RAP message with the specified action code
 * to be sent to the downstream device.
 */
ssize_t set_debug_reg_dump(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t count)
{
	struct cp8630_driver_context *drv_context = dev_get_drvdata(dev);
	int status;
	unsigned long new_debug_reg_dump = 0x100;

	/* Assume success */
	status = count;

	sii_log_debug("received string: %s\n", buf);

	status = kstrtoul(buf, 0, &new_debug_reg_dump);
	if (status != 0)
		goto err_exit2;

	if (down_interruptible(&drv_context->isr_lock))
		return -ERESTARTSYS;

	if (drv_context->dev_flags & DEV_FLAG_SHUTDOWN) {
		status = -ENODEV;
		goto err_exit;
	}
	debug_reg_dump = (new_debug_reg_dump > 0) ? true : false;
	sii_log_info("debug dump %s\n", debug_reg_dump ? "ON" : "OFF");

	status = count;
err_exit:
	up(&drv_context->isr_lock);
err_exit2:
	return status;
}
//#endif

/*
 * Sysfs attribute files supported by this driver.
 */
struct device_attribute driver_attribs[] = {
	__ATTR(SYS_ATTR_NAME_DS_CONNECTION, 0666,
		show_ds_connection_state, set_ds_connection_state),
	__ATTR(SYS_ATTR_NAME_DS_HPD, 0444,
		show_ds_hpd_state, NULL),
	__ATTR(SYS_ATTR_NAME_DS_RSENSE, 0444,
		show_ds_rsense_state, NULL),
	/*__ATTR(SYS_ATTR_NAME_DS_EDID, 0444,
		show_ds_edid, NULL),*/
	__ATTR(SYS_ATTR_NAME_DS_TMDS_MODE, 0444,
		show_ds_tmds_mode, NULL),
	__ATTR(SYS_ATTR_NAME_DS_TMDS_MODE_SET, 0222,
		NULL, set_ds_tmds_mode),
	__ATTR(SYS_ATTR_NAME_DS_AVMUTE, 0222,
		NULL, set_ds_avmute),
	__ATTR(SYS_ATTR_NAME_DS_STRM_MSG, 0222,
		NULL, set_ds_strm_msg),
	__ATTR(SYS_ATTR_NAME_DS_HDCP_CTRL, 0222,
		NULL, set_ds_hdcp_ctrl),
	/*__ATTR(SYS_ATTR_NAME_HDCP_AUTH_VERSION, 0222,
		NULL, set_hdcp_version),*/
	__ATTR(SYS_ATTR_NAME_DS_HDCP_STATUS, 0444,
		show_ds_hdcp_status, NULL),
	__ATTR(SYS_ATTR_NAME_DS_HDCP_FAIL_REASON, 0444,
		show_ds_hdcp_fail_reason, NULL),
	__ATTR(SYS_ATTR_NAME_DS_HDCP_VERSION, 0444,
		show_ds_hdcp_version, NULL),
	__ATTR(SYS_ATTR_NAME_DS_BKSV_LIST, 0444,
		show_ds_bksv_list, NULL),
	__ATTR(SYS_ATTR_NAME_DS_HDCP2_CONTENT_TYPE, 0666,
		show_ds_hdcp2_content_type, set_ds_hdcp2_content_type),
	__ATTR(SYS_ATTR_NAME_DS_HDCP_RPTR_MODE, 0444,
		show_ds_hdcp_rptr_mode, NULL),
	__ATTR(SYS_ATTR_NAME_DS_HDCP_TOPOLOGY, 0444,
		show_ds_hdcp_topology, NULL),
	__ATTR(SYS_ATTR_NAME_HDCP_EVENTS, 0444,
		show_hdcp_events, NULL),
	__ATTR(SYS_ATTR_NAME_DS_SCDC_SCRAMBLE_CTRL, 0666,
	       show_ds_scdc_source_scramble_status, set_ds_scdc_scramble_ctrl),
	__ATTR(SYS_ATTR_NAME_DS_SCDC_DEBUG_ENABLE, 0222,
	       NULL, set_ds_scdc_debug_enable),
#ifdef RX_DESCRAMBLE_THROUGH_API
	__ATTR(SYS_ATTR_NAME_US_SCDC_TMDS_CONFIG, 0222,
	       NULL, set_us_scdc_tmds_config),
	__ATTR(SYS_ATTR_NAME_US_SCRAMBLE_STATUS, 0444,
	       show_us_scramble_status, NULL),
#endif
	__ATTR(SYS_ATTR_NAME_DS_DSC_CTRL, 0666,
		show_ds_dsc_compression_status, set_ds_dsc_ctrl),
	__ATTR(SYS_ATTR_NAME_DS_DSC_COMPRESSION, 0444,
	       show_ds_dsc_compression_ratio_status, NULL),
	__ATTR(SYS_ATTR_NAME_DS_OUTPUT_CLRSPC, 0222,
		NULL, set_ds_output_clrspc),
	__ATTR(SYS_ATTR_NAME_DS_HVSYNC_POLARITY, 0666,
		show_ds_hvsync_pol, set_ds_hvsync_pol),
	__ATTR(SYS_ATTR_NAME_US_PLUS_5V, 0444,
		show_us_5v_status, NULL),
	__ATTR(SYS_ATTR_NAME_DEVICE_EDID, 0666,
		show_device_edid, set_device_edid),
	__ATTR(SYS_ATTR_NAME_US_HPD, 0222,
		NULL, set_us_hpd),
	__ATTR(SYS_ATTR_NAME_US_TMDS_MODE, 0444,
		show_us_tmds_mode, NULL),
	__ATTR(SYS_ATTR_NAME_US_HDCP_STATUS, 0444,
		show_us_hdcp_status, NULL),
	__ATTR(SYS_ATTR_NAME_DEBUG_LEVEL, 0666,
		show_debug_level, set_debug_level),
	__ATTR(SYS_ATTR_NAME_REG_DUMP_DEBUG_LEVEL, 0666,
		show_debug_reg_dump, set_debug_reg_dump),
	__ATTR(SYS_ATTR_NAME_RX_CLR_SPC, 0444,
		show_rx_clr_spc, NULL),
	__ATTR_NULL
};
#endif

#if 0
/*
 * set_reg_access_page() - Handle write request to set the
 *		reg access page value.
 *
 * The format of the string in buf must be:
 *	<pageaddr>
 * Where: <pageaddr> specifies the reg page of the register(s)
 *			to be written/read
 */
ssize_t set_reg_access_page(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t count)
{
	struct cp8630_driver_context *drv_context = dev_get_drvdata(dev);
	unsigned long address = 0x100;
	int status = 1;
	char my_buf[20];
	unsigned int i;

	sii_log_debug("received string: %s\n", buf);
	if (count >= sizeof(my_buf)) {
		sii_log_debug("string too long %c%s%c\n", '"', buf, '"');
		return status;
	}
	for (i = 0; i < count; ++i) {
		if ('\n' == buf[i]) {
			my_buf[i] = '\0';
			break;
		}
		if ('\t' == buf[i]) {
			my_buf[i] = '\0';
			break;
		}
		if (' ' == buf[i]) {
			my_buf[i] = '\0';
			break;
		}
		my_buf[i] = buf[i];
	}

	status = kstrtoul(my_buf, 0, &address);
	if (-ERANGE == status)
		sii_log_debug("ERANGE %s\n", my_buf);
	else if (1 == status)
		sii_log_debug("EINVAL %s\n", my_buf);
	else if (status != 0)
		sii_log_debug("status:%d buf:%s\n", status, my_buf);
	else if (address > 0xFF)
		sii_log_debug("address:0x%x buf:%s\n", address, my_buf);
	else {
		if (down_interruptible(&drv_context->isr_lock)) {
			sii_log_debug("could not get mutex\n");
			return -ERESTARTSYS;
		}
		if (drv_context->dev_flags & DEV_FLAG_SHUTDOWN) {
			sii_log_debug("DEV_FLAG_SHUTDOWN\n");
			status = -ENODEV;
		} else {
			drv_context->debug_i2c_address = address;
			status = count;
		}
		up(&drv_context->isr_lock);
	}

	return status;
}

/*
 * show_reg_access_page() - Show the current page number to be used when
 *	reg_access/data is accessed.
 *
 */
ssize_t show_reg_access_page(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct cp8630_driver_context *drv_context = dev_get_drvdata(dev);
	int status = 1;

	sii_log_debug("called\n");

	status = scnprintf(buf, PAGE_SIZE, "0x%02x",
		drv_context->debug_i2c_address);

	return status;
}

/*
 * set_reg_access_offset() - Handle write request to set the
 *		reg access page value.
 *
 * The format of the string in buf must be:
 *	<pageaddr>
 * Where: <pageaddr> specifies the reg page of the register(s)
 *			to be written/read
 */
ssize_t set_reg_access_offset(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t count)
{
	struct cp8630_driver_context *drv_context = dev_get_drvdata(dev);
	unsigned long offset = 0x100;
	int status = 1;

	sii_log_debug("received string: %s\n", buf);

	status = kstrtoul(buf, 0, &offset);
	if (-ERANGE == status) {
		sii_log_debug("ERANGE %s%s%s\n", ANSI_ESC_RED_TEXT, buf,
			ANSI_ESC_RESET_TEXT);
	} else if (1 == status) {
		sii_log_debug("EINVAL %s%s%s\n", ANSI_ESC_RED_TEXT, buf,
			ANSI_ESC_RESET_TEXT);
	} else if (status != 0) {
		sii_log_debug("status:%d buf:%c%s%c\n", status, '"', buf, '"');
	} else if (offset > 0xFFFF) {
		sii_log_debug("offset:0x%x buf:%c%s%c\n", (uint)offset, '"',
			buf, '"');
	} else {

		if (down_interruptible(&drv_context->isr_lock))
			return -ERESTARTSYS;
		if (drv_context->dev_flags & DEV_FLAG_SHUTDOWN) {
			status = -ENODEV;
		} else {
			drv_context->debug_i2c_offset = offset;
			status = count;
		}
		up(&drv_context->isr_lock);
	}

	return status;
}

/*
 * show_reg_access_offset()	- Show the current page number to be used when
 *	reg_access/data is accessed.
 *
 */
ssize_t show_reg_access_offset(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct cp8630_driver_context *drv_context = dev_get_drvdata(dev);
	int status = 1;

	sii_log_info("called\n");

	status = scnprintf(buf, PAGE_SIZE, "0x%02x\n",
		drv_context->debug_i2c_offset);

	return status;
}

/*
 * set_reg_access_length() - Handle write request to set the
 *		reg access page value.
 *
 * The format of the string in buf must be:
 *	<pageaddr>
 * Where: <pageaddr> specifies the reg page of the register(s)
 *			to be written/read
 */
ssize_t set_reg_access_length(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t count)
{
	struct cp8630_driver_context *drv_context = dev_get_drvdata(dev);
	unsigned long length = 0x100;
	int status = 1;

	sii_log_debug("received string: %s\n", buf);

	status = kstrtoul(buf, 0, &length);
	if (-ERANGE == status) {
		sii_log_debug("ERANGE %s%s%s\n", ANSI_ESC_RED_TEXT, buf,
			ANSI_ESC_RESET_TEXT);
	} else if (1 == status) {
		sii_log_debug("EINVAL %s'%s'%s\n", ANSI_ESC_RED_TEXT, buf,
			ANSI_ESC_RESET_TEXT);
	} else if (status != 0) {
		sii_log_debug("status:%d buf:%c%s%c\n", status, '"', buf, '"');
	} else if (length > 0xFF) {
		sii_log_debug("length:0x%x buf:%c%s%c\n", (uint)length, '"',
			buf, '"');
	} else {

		if (down_interruptible(&drv_context->isr_lock))
			return -ERESTARTSYS;
		if (drv_context->dev_flags & DEV_FLAG_SHUTDOWN) {
			status = -ENODEV;
		} else {
			drv_context->debug_i2c_xfer_length = length;
			status = count;
		}
		up(&drv_context->isr_lock);
	}

	return status;
}

/*
 * show_reg_access_length()	- Show the current page number to be used when
 *	reg_access/data is accessed.
 *
 */
ssize_t show_reg_access_length(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct cp8630_driver_context *drv_context = dev_get_drvdata(dev);
	int status = 1;

	sii_log_info("called\n");

	status = scnprintf(buf, PAGE_SIZE, "0x%02x",
		drv_context->debug_i2c_xfer_length);

	return status;
}

/*
 * set_reg_access_data() - Handle write request to the
 *	reg_access_data attribute file.
 *
 * This file is used to either perform a write to registers of the transmitter
 * or to set the address, offset and byte count for a subsequent from the
 * register(s) of the transmitter.
 *
 * The format of the string in buf must be:
 *	data_byte_0 ... data_byte_length-1
 * Where:	data_byte is a space separated list of <length_value> data
 *		bytes to be written.  If no data bytes are present then
 *		the write to this file will only be used to set
 *		the  page address, offset and length for a
 *		subsequent read from this file.
 */
ssize_t set_reg_access_data(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t count)
{
	struct cp8630_driver_context *drv_context = dev_get_drvdata(dev);
	unsigned long value;
	u8 data[MAX_DEBUG_TRANSFER_SIZE];
	int i;
	char *str;
	int status = 1;
    char l_s8input = 0;
	char *pinput = &l_s8input;

	sii_log_debug("received string: %s\n", buf);

	if (pinput == 0)
		return 1;
	memcpy(pinput, buf, count);

	/*
	 * Parse the input string and extract the scratch pad register
	 * selection parameters
	 */
	str = pinput;
	for (i = 0; (i < MAX_DEBUG_TRANSFER_SIZE) && ('\0' != *str); i++) {

		status = si_strtoul(&str, 0, &value);
		if (-ERANGE == status) {
			sii_log_debug("ERANGE %s%s%s\n", ANSI_ESC_RED_TEXT, str,
				ANSI_ESC_RESET_TEXT);
			goto exit_reg_access_data;
		} else if (1 == status) {
			sii_log_debug("EINVAL %s%s%s\n", ANSI_ESC_RED_TEXT, str,
				ANSI_ESC_RESET_TEXT);
			goto exit_reg_access_data;
		} else if (status != 0) {
			sii_log_debug("status:%d %s%s%s\n", status,
				ANSI_ESC_RED_TEXT, str, ANSI_ESC_RESET_TEXT);
			goto exit_reg_access_data;
		} else if (value > 0xFF) {
			sii_log_debug("value:0x%x str:%s%s%s\n", (uint)value,
				ANSI_ESC_RED_TEXT, str, ANSI_ESC_RESET_TEXT);
			goto exit_reg_access_data;
		} else {
			data[i] = value;
		}
	}

	if (i == 0) {
		sii_log_info("No data specified\n");
		goto exit_reg_access_data;
	}

	if (down_interruptible(&drv_context->isr_lock)) {
		status = -ERESTARTSYS;
		goto exit_reg_access_data;
	}

	if (drv_context->dev_flags & DEV_FLAG_SHUTDOWN) {
		status = -ENODEV;
	} else {

		/*status = drv_context->drv_info->rpt_device_dbg_i2c_reg_xfer(
		 drv_context->drv_obj,
		 drv_context->debug_i2c_address,
		 drv_context->debug_i2c_offset,
		 i, DEBUG_I2C_WRITE, data);*/
		status = rpt_device_dbg_i2c_reg_xfer(drv_context,
			drv_context->debug_i2c_offset, i, DEBUG_I2C_WRITE,
			data);
		if (status == 0)
			status = count;
	}

	up(&drv_context->isr_lock);

exit_reg_access_data: //kfree(pinput);
	return status;
}

/*
 * show_reg_access_data()	- Handle read request to the
 reg_access_data attribute file.
 *
 * Reads from this file return one or more transmitter register values in
 * hexadecimal string format.  The registers returned are specified by the
 * address, offset and length values previously written to this file.
 *
 * The return value is the number characters written to buf, or an error
 * code if the I2C read fails.
 *
 * The format of the string returned in buf is:
 * "address:<pageaddr> offset:<offset> length:<lenvalue> data:<datavalues>
 * where:	<pageaddr>  is the last I2C register page address written
 *				to this file
 *		<offset>    is the last register offset written to this file
 *		<lenvalue>  is the last register transfer length written
 *				to this file
 *		<datavalue> space separated list of <lenvalue> register
 *				values in OxXX format
 */
ssize_t show_reg_access_data(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct cp8630_driver_context *drv_context = dev_get_drvdata(dev);
	u8 data[MAX_DEBUG_TRANSFER_SIZE] = {0};
	u8 idx;
	int status = 1;

	sii_log_info("called\n");

	if (down_interruptible(&drv_context->isr_lock))
		return -ERESTARTSYS;

	if (drv_context->dev_flags & DEV_FLAG_SHUTDOWN) {
		status = -ENODEV;
		goto no_dev;
	}

	status = rpt_device_dbg_i2c_reg_xfer(drv_context,
		drv_context->debug_i2c_offset,
		drv_context->debug_i2c_xfer_length, DEBUG_I2C_READ, data);
no_dev: up(&drv_context->isr_lock);

	if (status == 0) {
		status = scnprintf(buf, PAGE_SIZE, "0x%02x:",
			drv_context->debug_i2c_offset);

		for (idx = 0; idx < drv_context->debug_i2c_xfer_length; idx++) {
			status += scnprintf(&buf[status], PAGE_SIZE, " 0x%02x",
				data[idx]);
		}
		status += scnprintf(&buf[status], PAGE_SIZE, "\n");
	}

	return status;
}

static struct device_attribute reg_access_page_attr =
	__ATTR(SYS_ATTR_NAME_REG_ACCESS_PAGE, 0666,
	show_reg_access_page, set_reg_access_page);

static struct device_attribute reg_access_offset_attr =
	__ATTR(SYS_ATTR_NAME_REG_ACCESS_OFFSET, 0666,
	show_reg_access_offset, set_reg_access_offset);

static struct device_attribute reg_access_length_attr =
	__ATTR(SYS_ATTR_NAME_REG_ACCESS_LENGTH, 0666,
	show_reg_access_length, set_reg_access_length);

static struct device_attribute reg_access_data_attr =
	__ATTR(SYS_ATTR_NAME_REG_ACCESS_DATA, 0666,
	show_reg_access_data, set_reg_access_data);

static struct attribute *reg_access_attrs[] = {
	&reg_access_page_attr.attr, &reg_access_offset_attr.attr,
	&reg_access_length_attr.attr, &reg_access_data_attr.attr, NULL };

static struct attribute_group reg_access_attribute_group = {
	.name = __stringify(SYS_OBJECT_NAME_REG_ACCESS),
	.attrs = reg_access_attrs };

static int rpt_device_dbg_i2c_reg_xfer(
	struct cp8630_driver_context *drv_context, u16 offset, u16 count,
	bool rw_flag, u8 *buffer)
{
	if (rw_flag == DEBUG_I2C_WRITE)
		sii_platform_block_write8(drv_context->pdata,
			offset, buffer, count);
	else
		sii_platform_block_read8(drv_context->pdata,
			offset, buffer, count);
	return 0;
}

/*
 * Wrapper for kstrtoul() that nul-terminates the input string at
 * the first non-digit character instead of returning an error.
 *
 * This function is destructive to the input string.
 *
 */
static int si_strtoul(char **str, int base, unsigned long *val)
{
	int tok_length, status, nul_offset;
	char *tstr = *str;

	nul_offset = 1;
	status = 1;
	if ((base == 0) && (tstr[0] == '0') && (tolower(tstr[1]) == 'x')) {
		tstr += 2;
		base = 16;
	}

	tok_length = strspn(tstr, "0123456789ABCDEFabcdef");
	if (tok_length) {
		if ((tstr[tok_length] == '\n') || (tstr[tok_length] == 0))
			nul_offset = 0;

		tstr[tok_length] = 0;
		status = kstrtoul(tstr, base, val);
		if (status == 0) {
			tstr = (tstr + tok_length) + nul_offset;
			*str = tstr;
		}
	}
	return status;
}

#define MAX_EVENT_STRING_LEN 128
void sii_drv_tx_uevent_notify(struct cp8630_driver_context *drv_context,
	u32 event, u32 event_param, void *data)
{
	char event_string[MAX_EVENT_STRING_LEN];
	char *envp[] = { event_string, NULL };

	switch (event) {
	case HDMI_TX_UEVENT_CONNECTION:
		sysfs_notify(&drv_context->cp8630_dev->kobj, NULL,
			__stringify(SYS_ATTR_NAME_DS_CONNECTION));

		strlcpy(event_string, "HDMIEVENT=connected",
			MAX_EVENT_STRING_LEN);
		kobject_uevent_env(&drv_context->cp8630_dev->kobj, KOBJ_CHANGE,
			envp);
		break;
	case HDMI_TX_UEVENT_DISCONNECTION:
		sysfs_notify(&drv_context->cp8630_dev->kobj, NULL,
			__stringify(SYS_ATTR_NAME_DS_CONNECTION));

		strlcpy(event_string, "HDMIEVENT=disconnected",
			MAX_EVENT_STRING_LEN);
		kobject_uevent_env(&drv_context->cp8630_dev->kobj, KOBJ_CHANGE,
			envp);
		break;
	default:
		sii_log_err("called with unrecognized event code!\n");
		break;
	}
}
#endif

#if 0
int sii_drv_sysfs_init(struct cp8630_driver_context *drv_context,
			struct device *parent_dev)
{
	int ret = 0;

	if (drv_context->cp8630_class == NULL) {
		drv_context->cp8630_class = class_create(THIS_MODULE,
						"hdmi_mhl");
		if (IS_ERR(drv_context->cp8630_class)) {
			ret = PTR_ERR(drv_context->cp8630_class);
			sii_log_debug("class_create failed\n");
			goto exit;
		}

		drv_context->cp8630_class->dev_attrs = driver_attribs;

		ret = alloc_chrdev_region(&drv_context->dev_num, 0,
				SII_DRIVER_MINOR_MAX, SII_DRV_NAME);

		if (ret) {
			sii_log_debug("register_chrdev %s failed, error code: %d\n",
				SII_DRV_NAME, ret);
			goto free_class;
		}

		cdev_init(&drv_context->cp8630_cdev, &cp8630_fops);
		drv_context->cp8630_cdev.owner = THIS_MODULE;

		ret = cdev_add(&drv_context->cp8630_cdev,
				MINOR(drv_context->dev_num),
				SII_DRIVER_MINOR_MAX);
		if (ret) {
			sii_log_debug("cdev_add %s failed %d\n", SII_DRV_NAME,
				ret);
			goto free_chrdev;
		}
	}

	drv_context->cp8630_dev = device_create(drv_context->cp8630_class,
				parent_dev, drv_context->dev_num,
				drv_context, "%s", SII_DEVICE_NAME);
	if (IS_ERR(drv_context->cp8630_dev)) {
		ret = PTR_ERR(drv_context->cp8630_dev);
		sii_log_debug("device_create failed\n");
		goto free_cdev;
	}

	ret = sysfs_create_group(&drv_context->cp8630_dev->kobj,
		&reg_access_attribute_group);
	if (ret)
		sii_log_debug("sysfs_create_group failed:%d\n", ret);

	/*ret = sysfs_create_group(&drv_context->cp8630_dev->kobj,
			&hdcp_attribute_group);
	if(ret)
		sii_log_debug("sysfs_create_group failed:%d\n", ret);

	ret = sysfs_create_group(&drv_context->cp8630_dev->kobj,
			&vc_attribute_group);
	 if(ret)
		 sii_log_debug("sysfs_create_group failed:%d\n", ret);*/
	return 0;
free_cdev:
	cdev_del(&drv_context->cp8630_cdev);
free_chrdev:
	unregister_chrdev_region(drv_context->dev_num, SII_DRIVER_MINOR_MAX);
	drv_context->dev_num = 0;
free_class:
	class_destroy(drv_context->cp8630_class);
	drv_context->cp8630_class = NULL;
exit:
	return ret;
}

int sii_drv_sysfs_exit(struct cp8630_driver_context *drv_context)
{
	sysfs_remove_group(&drv_context->cp8630_dev->kobj,
		&reg_access_attribute_group);

	device_destroy(drv_context->cp8630_class, drv_context->dev_num);
	cdev_del(&drv_context->cp8630_cdev);
	unregister_chrdev_region(drv_context->dev_num, SII_DRIVER_MINOR_MAX);
	drv_context->dev_num = 0;
	class_destroy(drv_context->cp8630_class);
	drv_context->cp8630_class = NULL;

	return 0;
}
#endif
#endif

