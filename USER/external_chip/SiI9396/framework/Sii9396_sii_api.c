/*
  2016 (C) Lattice Semiconductor Corporation

  No part of this work may be reproduced, modified, distributed, transmitted,
  transcribed, or translated into any language or computer format, in any form
  or by any means without written permission of Lattice Semiconductor, Inc.,
  111 SW 5th Ave, Suite 700, Portland, OR 97204.

*/

#include "Sii9396_osal.h"
#include "Sii9396_hal.h"
#include "Sii9396_device.h"
#include "Sii9396_sii_api.h"
#include "Sii9396_ipc_api.h"
#include "Sii9396_ipc_host.h"
#include "Sii9396_ipc_hal_host.h"
#include "Sii9396_flash.h"


struct sii_api_host_instance {
	bool in_use;
	uint32_t event_mask;
	void (*event_handler)(sii_inst_t inst, uint8_t event);
	struct sii_config config;
};

struct sii_api_host_context {
	uint8_t instance_count;
	struct sii_api_host_instance instance[SII_INST_COUNT_MAX];
};

struct sii_api_host_context api_host_context = { 0 };

static uint8_t get_next_instance_index(void)
{
	uint8_t index;
	for (index = 0; index < SII_INST_COUNT_MAX; index++) {
		if (!api_host_context.instance[index].in_use)
			break;
	}
	if (index == SII_INST_COUNT_MAX)
		return -1;
	return index;
}

sii_inst_t sii_create(sii_hwid_t hwid,
		void (*callback_function)(sii_inst_t inst, uint8_t event),
		struct sii_config *config)
{
	uint8_t index = hwid;		//get_next_instance_index();		//把hwid当作选择控制芯片的索引
	sii_inst_t new_inst = (sii_inst_t) index;
	if (index < 0)
		return SII_INST_NULL ;
	if (sii_device_init(new_inst, hwid) != SII_STATUS_SUCCESS)
		return SII_INST_NULL ;

	dbg("sii_device_init....\n");

	if (sii_ipc_init(new_inst) != SII_IPC_STATUS_SUCCESS)
		return SII_INST_NULL ;

	dbg("sii_ipc_init....\n");

	flash_init_status(new_inst);

	dbg("flash_init_status....\n");

	memcpy(&api_host_context.instance[index].config,
			config,
			sizeof(struct sii_config));
	api_host_context.instance[index].in_use = true;
	api_host_context.instance[index].event_mask = 0x00;
	api_host_context.instance[index].event_handler = callback_function;
	api_host_context.instance_count++;

	if (config->device_reset)
		sii_reset_device(new_inst, config->reset_time);

	dbg("sii_create ok....\n");

	return new_inst;
}

enum sii_api_status sii_delete(sii_inst_t inst)
{
	uint8_t index = (uint8_t) inst;
	if (index >= SII_INST_COUNT_MAX)
		return SII_API_FAILURE;
	sii_device_isr_enable(inst, false);
	sii_device_term(inst);
	if (sii_ipc_exit(inst) != SII_IPC_STATUS_SUCCESS)
		return SII_API_FAILURE;
	api_host_context.instance[index].in_use = false;
	api_host_context.instance[index].event_mask = 0x00;
	api_host_context.instance[index].event_handler = NULL;
	memset(&api_host_context.instance[index].config, 0,
			sizeof(struct sii_config));
	api_host_context.instance_count--;
	return SII_API_SUCCESS;
}
#if 0
enum sii_api_status sii_get_configure(
		sii_inst_t inst, struct sii_config *config)
{
	uint8_t index = (uint8_t)inst;
	memcpy(config, &api_host_context.instance[index].config,
			sizeof(struct sii_config));
	return SII_API_SUCCESS;
}

enum sii_api_status sii_set_configure(
		sii_inst_t inst, struct sii_config *config)
{
	uint8_t index = (uint8_t)inst;

	if (config) {
		memcpy(&api_host_context.instance[index].config,
				config, sizeof(struct sii_config));
	}

	return SII_API_SUCCESS;
}
#endif
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

static bool event_filter(sii_inst_t inst, uint8_t event)
{
	uint8_t index = (uint8_t) inst;
	uint32_t event_flag = 1 << event;
	if (event_flag & api_host_context.instance[index].event_mask)
		return false;
	return true;
}

enum sii_api_status sii_handle(sii_inst_t inst)
{
	struct sii_ipc_notification notification;
	uint8_t event;
	uint8_t index = (uint8_t) inst;
	while (sii_ipc_host_pop_queue_notification(inst, &notification) !=
			SII_IPC_STATUS_ERROR_NOTIFICATION_QUEUE_EMPTY) {
		event = notification.notification_type - SII_EVENT__BASE;
		if (event_filter(inst, event)) {
			if (api_host_context.instance[index].event_handler != NULL)
				api_host_context.instance[index].event_handler(inst, event);
		}
	}
	return SII_API_SUCCESS;
}

enum sii_api_status sii_wait_for_boot_done(sii_inst_t inst)
{
	bool int_pin_status = false;
	uint8_t timeout = 0;
	//sii_set_rx_hpd(0,1);
	sii_device_int_pin_status(inst, &int_pin_status);
	while (!int_pin_status) {
		sii_os_sleep_msec(50);
		sii_device_int_pin_status(inst, &int_pin_status);
		if (++timeout > 60)
			break;
	}
	if (int_pin_status) {
		sii_os_sleep_msec(SII_BOOT_TIME);
		sii_ipc_hal_host_init(inst);
		sii_ipc_hal_set_slave_interrupt_mask(inst, true);
		sii_device_isr_enable(inst, true);
		sii_hal_isr(inst);
		sii_set_host_mode(inst, 1);
		return SII_API_SUCCESS;
	}
	else
		return SII_API_FAILURE;
}

enum sii_api_status sii_get_rx_plus5v(
		sii_inst_t inst, bool *status)
{
	enum sii_api_status api_status = SII_API_SUCCESS;
	enum sii_ipc_api_status ipc_status = SII_IPC_API_SUCCESS;
	struct sii_ipc_register_reply reply;

	ipc_status = sii_host_get_rx_plus5v(inst, &reply);
	if (ipc_status != SII_IPC_API_SUCCESS) {
		*status = 0;
		api_status = SII_API_FAILURE;
	} else {
		*status = (bool) reply.reply_return;
		dbg("get rx +5v: %d\n", *status);
	}

	return api_status;
}

enum sii_api_status sii_get_rx_hpd(
		sii_inst_t inst, bool *hpd)
{
	enum sii_api_status api_status = SII_API_SUCCESS;
	enum sii_ipc_api_status ipc_status = SII_IPC_API_SUCCESS;
	struct sii_ipc_register_reply reply;

	ipc_status = sii_host_get_rx_hpd(inst, &reply);
	if (ipc_status != SII_IPC_API_SUCCESS) {
		*hpd = 0;
		api_status = SII_API_FAILURE;
	} else {
		*hpd = (bool) reply.reply_return;
		dbg("get rx hpd: %d\n", *hpd);
	}

	return api_status;
}

enum sii_api_status sii_get_rx_av_mute(
		sii_inst_t inst, bool *status)
{
	enum sii_api_status api_status = SII_API_SUCCESS;
	enum sii_ipc_api_status ipc_status = SII_IPC_API_SUCCESS;
	struct sii_ipc_register_reply reply;

	ipc_status = sii_host_get_rx_av_mute(inst, &reply);
	if (ipc_status != SII_IPC_API_SUCCESS) {
		*status = 0;
		api_status = SII_API_FAILURE;
	} else {
		*status = (bool) reply.reply_return;
		dbg("get rx av mute: %d\n", *status);
	}

	return api_status;
}

enum sii_api_status sii_get_rx_link_type(
		sii_inst_t inst, enum sii_av_link_type *type)
{
	enum sii_api_status api_status = SII_API_SUCCESS;
	enum sii_ipc_api_status ipc_status = SII_IPC_API_SUCCESS;
	struct sii_ipc_register_reply reply;

	ipc_status = sii_host_get_rx_link_type(inst, &reply);
	if (ipc_status != SII_IPC_API_SUCCESS) {
		*type = 0;
		api_status = SII_API_FAILURE;
	} else {
		*type = (enum sii_av_link_type) reply.reply_return;
		dbg("get rx link type: %d\n", *type);
	}

	return api_status;
}

enum sii_api_status sii_get_rx_edid_content(
		sii_inst_t inst,
		uint8_t block_num, uint8_t *edid_buffer)
{
	enum sii_api_status api_status = SII_API_SUCCESS;
	enum sii_ipc_api_status ipc_status = SII_IPC_API_SUCCESS;
	struct sii_ipc_message *msg = &ipc_host_context->host_message;
#if 1
	uint8_t i;
#endif


	ipc_status = sii_host_get_rx_edid_content(inst, block_num, msg);
	if (ipc_status != SII_IPC_API_SUCCESS) {
		api_status = SII_API_FAILURE;
	} else {
		memcpy(edid_buffer,
				msg->message_payload.message_data,
				SII_EDID_BLOCK_SIZE);
#if 1
		for (i = 0; i < SII_EDID_BLOCK_SIZE; i++)
		{
			if (i % 8 == 0)
			{
				edid_info("\r\n");
                UART1_SendBytes("\r\n", 2);
			}
			edid_info("%02x ", edid_buffer[i]);
			#if UART1_DEBUG
			sprintf(USART1_tx_buff, "%02x ", edid_buffer[i]);
            UART1_SendBytes((uint8_t *)USART1_tx_buff, strlen(USART1_tx_buff));
            #endif
		}
#endif

	}

	return api_status;
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

enum sii_api_status sii_get_rx_char_error_count(
		sii_inst_t inst,
		struct sii_char_error_count *count)
{
	enum sii_api_status api_status = SII_API_SUCCESS;
	enum sii_ipc_api_status ipc_status = SII_IPC_API_SUCCESS;
	struct sii_ipc_message *msg = &ipc_host_context->host_message;

	ipc_status = sii_host_get_rx_char_error_count(inst, msg);
	if (ipc_status != SII_IPC_API_SUCCESS) {
		api_status = SII_API_FAILURE;
	} else {
		memcpy(count, msg->message_payload.message_data, sizeof(*count));
	}
	return api_status;
}

enum sii_api_status sii_get_rx_tmds_lock_status(
		sii_inst_t inst,
		struct sii_tmds_lock_status *status)
{
	enum sii_api_status api_status = SII_API_SUCCESS;
	enum sii_ipc_api_status ipc_status = SII_IPC_API_SUCCESS;
	struct sii_ipc_message *msg = &ipc_host_context->host_message;

	ipc_status = sii_host_get_rx_tmds_lock_status(inst, msg);
	if (ipc_status != SII_IPC_API_SUCCESS) {
		api_status = SII_API_FAILURE;
	} else {
		memcpy(status, msg->message_payload.message_data, sizeof(*status));
	}
	return api_status;
}

enum sii_api_status sii_get_rx_device_id_string(
		sii_inst_t inst,
		struct sii_device_id_string *string)
{
	enum sii_api_status api_status = SII_API_SUCCESS;
	enum sii_ipc_api_status ipc_status = SII_IPC_API_SUCCESS;
	struct sii_ipc_message *msg = &ipc_host_context->host_message;

	ipc_status = sii_host_get_rx_device_id_string(inst, msg);
	if (ipc_status != SII_IPC_API_SUCCESS) {
		api_status = SII_API_FAILURE;
	} else {
		memcpy(string, msg->message_payload.message_data, sizeof(*string));
	}
	return api_status;
}

enum sii_api_status sii_get_rx_cec_reg(
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

	ipc_status = sii_host_get_rx_reg(inst, msg);
	if (ipc_status != SII_IPC_API_SUCCESS)
		api_status = SII_API_FAILURE;
	else
		memcpy(value, msg->message_payload.message_data, length);
	return api_status;
}

enum sii_api_status sii_get_rx_scdc_reg(
		sii_inst_t inst, uint8_t offset, uint8_t length, uint8_t *value)
{
	enum sii_api_status api_status = SII_API_SUCCESS;
	enum sii_ipc_api_status ipc_status = SII_IPC_API_SUCCESS;
	struct sii_ipc_message *msg = &ipc_host_context->host_message;

	memset(msg, 0, sizeof(*msg));
	msg->message_payload.message_data[0] = SII_SCDC_PAGE;
	msg->message_payload.message_data[1] = offset;
	msg->message_payload.message_data[2] = length;
	msg->message_length_l = 3;
	msg->message_length_h = 0;

	ipc_status = sii_host_get_rx_reg(inst, msg);
	if (ipc_status != SII_IPC_API_SUCCESS)
		api_status = SII_API_FAILURE;
	else
		memcpy(value, msg->message_payload.message_data, length);
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

enum sii_api_status sii_get_rx_audio_info(
		sii_inst_t inst, struct sii_audio_info *info)
{
	enum sii_api_status api_status = SII_API_SUCCESS;
	enum sii_ipc_api_status ipc_status = SII_IPC_API_SUCCESS;
	struct sii_ipc_message *msg = &ipc_host_context->host_message;

	ipc_status = sii_host_get_rx_audio_info(inst, msg);
	if (ipc_status != SII_IPC_API_SUCCESS) {
		api_status = SII_API_FAILURE;
	} else {
		memcpy(info, msg->message_payload.message_data, sizeof(*info));
	}
	return api_status;
}

enum sii_api_status sii_set_rx_eq(
		sii_inst_t inst, struct sii_eq_setting *setting)
{
	enum sii_api_status api_status = SII_API_SUCCESS;
	enum sii_ipc_api_status ipc_status = SII_IPC_API_SUCCESS;
	struct sii_ipc_message *msg = &ipc_host_context->host_message;

	memset(msg, 0, sizeof(*msg));
	memcpy(msg->message_payload.message_data, setting, sizeof(*setting));
	msg->message_length_l = sizeof(*setting);
	msg->message_length_h = 0;

	ipc_status = sii_host_set_rx_eq(inst, msg);
	if (ipc_status != SII_IPC_API_SUCCESS)
		api_status = SII_API_FAILURE;

	return api_status;
}

enum sii_api_status sii_set_rx_cec_reg(
		sii_inst_t inst, uint8_t offset, uint8_t value)
{
	enum sii_api_status api_status = SII_API_SUCCESS;
	enum sii_ipc_api_status ipc_status = SII_IPC_API_SUCCESS;
	struct sii_ipc_message *msg = &ipc_host_context->host_message;

	memset(msg, 0, sizeof(*msg));
	msg->message_payload.message_data[0] = SII_CEC_PAGE;
	msg->message_payload.message_data[1] = offset;
	msg->message_payload.message_data[2] = value;
	msg->message_length_l = 3;
	msg->message_length_h = 0;

	ipc_status = sii_host_set_rx_reg(inst, msg);
	if (ipc_status != SII_IPC_API_SUCCESS)
		api_status = SII_API_FAILURE;

	return api_status;
}

enum sii_api_status sii_set_rx_scdc_reg(
		sii_inst_t inst, uint8_t offset, uint8_t value)
{
	enum sii_api_status api_status = SII_API_SUCCESS;
	enum sii_ipc_api_status ipc_status = SII_IPC_API_SUCCESS;
	struct sii_ipc_message *msg = &ipc_host_context->host_message;

	memset(msg, 0, sizeof(*msg));
	msg->message_payload.message_data[0] = SII_SCDC_PAGE;
	msg->message_payload.message_data[1] = offset;
	msg->message_payload.message_data[2] = value;
	msg->message_length_l = 3;
	msg->message_length_h = 0;

	ipc_status = sii_host_set_rx_reg(inst, msg);
	if (ipc_status != SII_IPC_API_SUCCESS)
		api_status = SII_API_FAILURE;

	return api_status;
}

enum sii_api_status sii_set_rx_hpd(
		sii_inst_t inst, bool hpd)
{
	enum sii_api_status api_status = SII_API_SUCCESS;
	enum sii_ipc_api_status ipc_status = SII_IPC_API_SUCCESS;

	ipc_status = sii_host_set_rx_hpd(inst, hpd);
	if (ipc_status != SII_IPC_API_SUCCESS)
		api_status = SII_API_FAILURE;

	return api_status;
}

enum sii_api_status sii_set_rx_term(
		sii_inst_t inst, bool term)
{
	enum sii_api_status api_status = SII_API_SUCCESS;
	enum sii_ipc_api_status ipc_status = SII_IPC_API_SUCCESS;

	ipc_status = sii_host_set_rx_term(inst, term);
	if (ipc_status != SII_IPC_API_SUCCESS)
		api_status = SII_API_FAILURE;

	return api_status;
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

enum sii_api_status sii_set_rx_audio_mclk_ratio(
	sii_inst_t inst, enum sii_audio_mclk_ratio ratio)
{
	enum sii_api_status api_status = SII_API_SUCCESS;
	enum sii_ipc_api_status ipc_status = SII_IPC_API_SUCCESS;

	ipc_status = sii_host_set_rx_audio_mclk_ratio(inst, ratio);
	if (ipc_status != SII_IPC_API_SUCCESS)
		api_status = SII_API_FAILURE;

	return api_status;
}

enum sii_api_status sii_set_rx_audio_extraction(
		sii_inst_t inst, enum sii_audio_mode mode)
{
	enum sii_api_status api_status = SII_API_SUCCESS;
	enum sii_ipc_api_status ipc_status = SII_IPC_API_SUCCESS;

	ipc_status = sii_host_set_rx_audio_extraction(inst, mode);
	if (ipc_status != SII_IPC_API_SUCCESS)
		api_status = SII_API_FAILURE;

	return api_status;
}

enum sii_api_status sii_get_rx_scramble_full(
	sii_inst_t inst, enum sii_scramble_state *state)
{
	enum sii_api_status api_status = SII_API_SUCCESS;
	enum sii_ipc_api_status ipc_status = SII_IPC_API_SUCCESS;
	struct sii_ipc_register_reply reply;

	ipc_status = sii_host_get_rx_scramble_full(inst, &reply);
	if (ipc_status != SII_IPC_API_SUCCESS) {
		*state = 0;
		api_status = SII_API_FAILURE;
	} else
		*state = (enum sii_scramble_state) reply.reply_return;

	return api_status;
}

enum sii_api_status sii_get_tx_tmds_status(
		sii_inst_t inst, bool *status)
{
	enum sii_api_status api_status = SII_API_SUCCESS;
	enum sii_ipc_api_status ipc_status = SII_IPC_API_SUCCESS;
	struct sii_ipc_register_reply reply;

	ipc_status = sii_host_get_tx_tmds_status(inst, &reply);
	if (ipc_status != SII_IPC_API_SUCCESS) {
		*status = 0;
		api_status = SII_API_FAILURE;
	} else {
		*status = (bool) reply.reply_return;
		dbg("get tx tmds status: %d\n", *status);
	}

	return api_status;
}

enum sii_api_status sii_get_tx_av_mute(
		sii_inst_t inst, bool *status)
{
	enum sii_api_status api_status = SII_API_SUCCESS;
	enum sii_ipc_api_status ipc_status = SII_IPC_API_SUCCESS;
	struct sii_ipc_register_reply reply;

	ipc_status = sii_host_get_tx_av_mute(inst, &reply);
	if (ipc_status != SII_IPC_API_SUCCESS) {
		*status = 0;
		api_status = SII_API_FAILURE;
	} else {
		*status = (bool) reply.reply_return;
		dbg("get tx av mute: %d\n", *status);
	}

	return api_status;
}

enum sii_api_status sii_get_tx_tmds_pin_swap(
		sii_inst_t inst, bool *pin_swap)
{
	// enum sii_tmds_pin_swap *pin_swap) {
	enum sii_api_status api_status = SII_API_SUCCESS;
	enum sii_ipc_api_status ipc_status = SII_IPC_API_SUCCESS;
	struct sii_ipc_register_reply reply;

	ipc_status = sii_host_get_tx_tmds_pin_swap(inst, &reply);
	if (ipc_status != SII_IPC_API_SUCCESS) {
		*pin_swap = 0;
		api_status = SII_API_FAILURE;
	} else {
		*pin_swap = (bool) reply.reply_return;
		dbg("get tx tmds pin swap: %d\n", *pin_swap);
	}

	return api_status;
}

enum sii_api_status sii_get_tx_hpd(
		sii_inst_t inst, bool *hpd)
{
	enum sii_api_status api_status = SII_API_SUCCESS;
	enum sii_ipc_api_status ipc_status = SII_IPC_API_SUCCESS;
	struct sii_ipc_register_reply reply;

	ipc_status = sii_host_get_tx_hpd(inst, &reply);
	if (ipc_status != SII_IPC_API_SUCCESS) {
		*hpd = 0;
		api_status = SII_API_FAILURE;
	} else {
		*hpd = (bool) reply.reply_return;
		edid_info("get tx hpd: %d\n", *hpd);
	}

	return api_status;
}

enum sii_api_status sii_get_tx_rxsense(
		sii_inst_t inst, bool *status)
{
	enum sii_api_status api_status = SII_API_SUCCESS;
	enum sii_ipc_api_status ipc_status = SII_IPC_API_SUCCESS;
	struct sii_ipc_register_reply reply;

	ipc_status = sii_host_get_tx_rxsense(inst, &reply);
	if (ipc_status != SII_IPC_API_SUCCESS) {
		*status = 0;
		api_status = SII_API_FAILURE;
	} else {
		*status = (bool) reply.reply_return;
		dbg("get tx rxsense: %d\n", *status);
	}

	return api_status;
}

enum sii_api_status sii_get_tx_ds_edid_status(
		sii_inst_t inst, bool *status)
{
	enum sii_api_status api_status = SII_API_SUCCESS;
	enum sii_ipc_api_status ipc_status = SII_IPC_API_SUCCESS;
	struct sii_ipc_register_reply reply;

	ipc_status = sii_host_get_tx_ds_edid_status(inst, &reply);
	if (ipc_status != SII_IPC_API_SUCCESS) {
		*status = 0;
		api_status = SII_API_FAILURE;
	} else {
		*status = (bool) reply.reply_return;
		dbg("get tx ds edid status: %d\n", *status);
	}

	return api_status;
}

enum sii_api_status sii_get_tx_ds_edid_content(
		sii_inst_t inst,
		uint8_t block_num, uint8_t *edid_buffer)
{
	enum sii_api_status api_status = SII_API_SUCCESS;
	enum sii_ipc_api_status ipc_status = SII_IPC_API_SUCCESS;
	struct sii_ipc_message *msg = &ipc_host_context->host_message;
#if 1
		uint8_t i;
#endif


	ipc_status = sii_host_get_tx_ds_edid_content(inst, block_num, msg);
	if (ipc_status != SII_IPC_API_SUCCESS) {
		api_status = SII_API_FAILURE;
	} else {
		memcpy(edid_buffer,
				msg->message_payload.message_data,
				SII_EDID_BLOCK_SIZE);
	}

#if 1
			for (i = 0; i < SII_EDID_BLOCK_SIZE; i++)
			{
				if (i % 8 == 0)
					edid_info("\r\n");
				edid_info("%02x ", edid_buffer[i]);
			}
#endif


	return api_status;
}

enum sii_api_status sii_get_tx_tmds_swing_level(
		sii_inst_t inst,
		uint8_t *clk_level, uint8_t *data_level)
{
	enum sii_api_status api_status = SII_API_SUCCESS;
	enum sii_ipc_api_status ipc_status = SII_IPC_API_SUCCESS;
	struct sii_ipc_message *msg = &ipc_host_context->host_message;

	ipc_status = sii_host_get_tx_tmds_swing_level(inst, msg);
	if (ipc_status != SII_IPC_API_SUCCESS) {
		*clk_level = 0;
		*data_level = 0;
		api_status = SII_API_FAILURE;
	} else {
		*clk_level = msg->message_payload.message_data[0];
		*data_level = msg->message_payload.message_data[1];
		dbg("get tx tmds swing clk level: %d\tdata level: %d\n",
				*clk_level, *data_level);
	}

	return api_status;
}

enum sii_api_status sii_set_tx_tmds_status(
		sii_inst_t inst, bool status)
{
	enum sii_api_status api_status = SII_API_SUCCESS;
	enum sii_ipc_api_status ipc_status = SII_IPC_API_SUCCESS;

	ipc_status = sii_host_set_tx_tmds_status(inst, status);
	if (ipc_status != SII_IPC_API_SUCCESS)
		api_status = SII_API_FAILURE;

	return api_status;
}

enum sii_api_status sii_set_tx_av_mute(
		sii_inst_t inst, bool status)
{
	enum sii_api_status api_status = SII_API_SUCCESS;
	enum sii_ipc_api_status ipc_status = SII_IPC_API_SUCCESS;

	ipc_status = sii_host_set_tx_av_mute(inst, status);
	if (ipc_status != SII_IPC_API_SUCCESS)
		api_status = SII_API_FAILURE;

	return api_status;
}

enum sii_api_status sii_set_tx_tmds_swing_level(
		sii_inst_t inst,
		struct sii_tmds_swing_level *swing_level)
{
	enum sii_api_status api_status = SII_API_SUCCESS;
	enum sii_ipc_api_status ipc_status = SII_IPC_API_SUCCESS;
	struct sii_ipc_message *msg = &ipc_host_context->host_message;

	memset(msg, 0, sizeof(*msg));
	memcpy(msg->message_payload.message_data, swing_level,
			sizeof(struct sii_tmds_swing_level));
	msg->message_length_l = sizeof(struct sii_tmds_swing_level);
	msg->message_length_h = 0;

	ipc_status = sii_host_set_tx_tmds_swing_level(inst, msg);
	if (ipc_status != SII_IPC_API_SUCCESS)
		api_status = SII_API_FAILURE;

	return api_status;
}

enum sii_api_status sii_set_tx_tmds_pin_swap(
		sii_inst_t inst, enum sii_tmds_pin_swap pin_swap)
{
	enum sii_api_status api_status = SII_API_SUCCESS;
	enum sii_ipc_api_status ipc_status = SII_IPC_API_SUCCESS;

	ipc_status = sii_host_set_tx_tmds_pin_swap(inst, pin_swap);
	if (ipc_status != SII_IPC_API_SUCCESS)
		api_status = SII_API_FAILURE;

	return api_status;
}

enum sii_api_status sii_get_rx_hdcp_ddc(
		sii_inst_t inst, bool *status)
{
	enum sii_api_status api_status = SII_API_SUCCESS;
	enum sii_ipc_api_status ipc_status = SII_IPC_API_SUCCESS;
	struct sii_ipc_register_reply reply;

	ipc_status = sii_host_get_rx_hdcp_ddc(inst, &reply);
	if (ipc_status != SII_IPC_API_SUCCESS) {
		*status = 0;
		api_status = SII_API_FAILURE;
	} else {
		*status = (bool) reply.reply_return;
		dbg("get rx hdcp ddc status: %d\n", *status);
	}

	return api_status;
}

enum sii_api_status sii_get_rx_hdcp_status(
		sii_inst_t inst, enum sii_hdcp_status *status)
{
	enum sii_api_status api_status = SII_API_SUCCESS;
	enum sii_ipc_api_status ipc_status = SII_IPC_API_SUCCESS;
	struct sii_ipc_message *msg = &ipc_host_context->host_message;

	ipc_status = sii_host_get_rx_hdcp_status(inst, msg);
	if (ipc_status != SII_IPC_API_SUCCESS) {
		*status = 0;
		api_status = SII_API_FAILURE;
	} else {
		*status = (enum sii_hdcp_status) msg->message_payload.message_data[0];
		dbg("get rx hdcp status: %d\n", *status);
	}

	return api_status;
}

enum sii_api_status sii_get_rx_hdcp_us_version(
		sii_inst_t inst,
		enum sii_hdcp_version *version)
{
	enum sii_api_status api_status = SII_API_SUCCESS;
	enum sii_ipc_api_status ipc_status = SII_IPC_API_SUCCESS;
	struct sii_ipc_register_reply reply;

	ipc_status = sii_host_get_rx_hdcp_us_version(inst, &reply);
	if (ipc_status != SII_IPC_API_SUCCESS) {
		*version = 0;
		api_status = SII_API_FAILURE;
	} else {
		*version = (enum sii_hdcp_version) reply.reply_return;
		dbg("get rx hdcp version: %d\n", *version);
	}

	return api_status;
}

enum sii_api_status sii_get_rx_hdcp_rxid(
		sii_inst_t inst, struct sii_hdcp_ksv *rxid)
{
	enum sii_api_status api_status = SII_API_SUCCESS;
	enum sii_ipc_api_status ipc_status = SII_IPC_API_SUCCESS;
	struct sii_ipc_message *msg = &ipc_host_context->host_message;
#if 0
	uint8_t i;
#endif

	ipc_status = sii_host_get_rx_hdcp_rxid(inst, msg);
	if (ipc_status != SII_IPC_API_SUCCESS) {
		api_status = SII_API_FAILURE;
	} else {
		memcpy(rxid->key,
				msg->message_payload.message_data,
				sizeof(struct sii_hdcp_ksv));
#if 0
		for (i = 0; i < sizeof(struct sii_hdcp_ksv); i++)
		{
			dbg("%d:\t%02x\n", i, rxid->key[i]);
		}
#endif
	}

	return api_status;
}

enum sii_api_status sii_get_rx_hdcp_bksv(
		sii_inst_t inst, struct sii_hdcp_ksv *bksv)
{
	enum sii_api_status api_status = SII_API_SUCCESS;
	enum sii_ipc_api_status ipc_status = SII_IPC_API_SUCCESS;
	struct sii_ipc_message *msg = &ipc_host_context->host_message;
#if 0
	uint8_t i;
#endif

	ipc_status = sii_host_get_rx_hdcp_bksv(inst, msg);
	if (ipc_status != SII_IPC_API_SUCCESS) {
		api_status = SII_API_FAILURE;
	} else {
		memcpy(bksv->key,
				msg->message_payload.message_data,
				sizeof(struct sii_hdcp_ksv));
#if 0
		for (i = 0; i < sizeof(struct sii_hdcp_ksv); i++)
		{
			dbg("%d:\t%02x\n", i, bksv->key[i]);
		}
#endif
	}

	return api_status;
}

enum sii_api_status sii_set_rx_hdcp_ddc(
		sii_inst_t inst, bool status)
{
	enum sii_api_status api_status = SII_API_SUCCESS;
	enum sii_ipc_api_status ipc_status = SII_IPC_API_SUCCESS;

	ipc_status = sii_host_set_rx_hdcp_ddc(inst, status);
	if (ipc_status != SII_IPC_API_SUCCESS)
		api_status = SII_API_FAILURE;

	return api_status;
}

enum sii_api_status sii_get_rx_hdcp_repeater_mode(
		sii_inst_t inst, bool *enable)
{
	enum sii_api_status api_status = SII_API_SUCCESS;
	enum sii_ipc_api_status ipc_status = SII_IPC_API_SUCCESS;
	struct sii_ipc_register_reply reply;

	ipc_status = sii_host_get_rx_hdcp_repeater_mode(inst, &reply);
	if (ipc_status != SII_IPC_API_SUCCESS) {
		*enable = 0;
		api_status = SII_API_FAILURE;
	} else {
		*enable = (bool) reply.reply_return;
	}

	return api_status;
}

enum sii_api_status sii_set_rx_hdcp_repeater_mode(
		sii_inst_t inst, bool enable)
{
	enum sii_api_status api_status = SII_API_SUCCESS;
	enum sii_ipc_api_status ipc_status = SII_IPC_API_SUCCESS;

	ipc_status = sii_host_set_rx_hdcp_repeater_mode(inst, enable);
	if (ipc_status != SII_IPC_API_SUCCESS)
		api_status = SII_API_FAILURE;

	return api_status;
}

enum sii_api_status sii_trigger_rx_hdcp_rxid_list_to_send(
		sii_inst_t inst)
{
	enum sii_api_status api_status = SII_API_SUCCESS;
	enum sii_ipc_api_status ipc_status = SII_IPC_API_SUCCESS;

	ipc_status = sii_host_trigger_rx_hdcp_rxid_list_to_send(inst);
	if (ipc_status != SII_IPC_API_SUCCESS)
		api_status = SII_API_FAILURE;

	return api_status;
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

enum sii_api_status sii_get_product_id(
		sii_inst_t inst, enum sii_product_id *id)
{
	enum sii_api_status api_status = SII_API_SUCCESS;
	enum sii_ipc_api_status ipc_status = SII_IPC_API_SUCCESS;
	struct sii_ipc_register_reply reply;

	ipc_status = sii_host_get_product_id(inst, &reply);
	if (ipc_status != SII_IPC_API_SUCCESS) {
		*id = SII_PRODUCT_ID__NONE;
		api_status = SII_API_FAILURE;
	} else {
		*id = (enum sii_product_id) reply.reply_return;
		dbg("get product id: %d\n", *id);
	}

	return api_status;
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
		dbg("get chip id: %x\n", *chip_id);
	}

	return api_status;
}

enum sii_api_status sii_get_chip_revision(
		sii_inst_t inst, uint8_t *revison)
{
	enum sii_api_status api_status = SII_API_SUCCESS;
	enum sii_ipc_api_status ipc_status = SII_IPC_API_SUCCESS;
	struct sii_ipc_register_reply reply;

	ipc_status = sii_host_get_chip_revision(inst, &reply);
	if (ipc_status != SII_IPC_API_SUCCESS) {
		*revison = 0;
		api_status = SII_API_FAILURE;
	} else {
		*revison = (uint8_t) reply.reply_return;
		dbg("get rx chip revision: %d\n", *revison);
	}

	return api_status;
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
		strcpy(fw_ver,
				msg->message_payload.message_data);
		dbg("get firmware version: %s\n", fw_ver);
	}

	return api_status;
}

enum sii_api_status sii_get_release_timestamp(
		sii_inst_t inst, uint8_t *timestamp)
{
	enum sii_api_status api_status = SII_API_SUCCESS;
	enum sii_ipc_api_status ipc_status = SII_IPC_API_SUCCESS;
	struct sii_ipc_message *msg = &ipc_host_context->host_message;

	ipc_status = sii_host_get_release_timestamp(inst, msg);
	if (ipc_status != SII_IPC_API_SUCCESS) {
		timestamp[0] = '\0';
		api_status = SII_API_FAILURE;
	} else {
		strcpy(timestamp,
				msg->message_payload.message_data);
		dbg("get firmware release time stamp: %s\n", timestamp);
	}

	return api_status;
}

enum sii_api_status sii_get_boot_status(
		sii_inst_t inst, enum sii_boot_status *status)
{
	enum sii_api_status api_status = SII_API_SUCCESS;
	enum sii_ipc_api_status ipc_status = SII_IPC_API_SUCCESS;
	struct sii_ipc_register_reply reply;

	ipc_status = sii_host_get_boot_status(inst, &reply);
	if (ipc_status != SII_IPC_API_SUCCESS) {
		*status = SII_BOOT_STAT__FAILURE;
		api_status = SII_API_FAILURE;
	} else {
		*status = (enum sii_boot_status) reply.reply_return;
		dbg("get boot status: %d\n", *status);
	}

	return api_status;
}

enum sii_api_status sii_get_host_mode(sii_inst_t inst, bool *status)
{
	enum sii_api_status api_status = SII_API_SUCCESS;
	enum sii_ipc_api_status ipc_status = SII_IPC_API_SUCCESS;
	struct sii_ipc_register_reply reply;

	ipc_status = sii_host_get_host_mode(inst, &reply);
	if (ipc_status != SII_IPC_API_SUCCESS) {
		*status = 0;
		api_status = SII_API_FAILURE;
	} else {
		*status = (bool) reply.reply_return;
	}

	return api_status;
}

enum sii_api_status sii_set_host_mode(sii_inst_t inst, bool status)
{
	enum sii_api_status api_status = SII_API_SUCCESS;
	enum sii_ipc_api_status ipc_status = SII_IPC_API_SUCCESS;

	ipc_status = sii_host_set_host_mode(inst, status);
	if (ipc_status != SII_IPC_API_SUCCESS)
		api_status = SII_API_FAILURE;

	return api_status;
}

enum sii_api_status sii_flash_init(sii_inst_t inst)
{
	enum sii_api_status api_status = SII_API_SUCCESS;
	if (flash_init(inst) != SII_STATUS_SUCCESS)
		api_status = SII_API_FAILURE;
	return api_status;
}

enum sii_api_status sii_get_flash_status(
		sii_inst_t inst, enum sii_flash_status *status)
{
	enum sii_api_status api_status = SII_API_SUCCESS;
	*status = flash_get_status(inst);
	if (*status > SII_FLASH_STATUS__FAILURE) {
		*status = SII_FLASH_STATUS__FAILURE;
	}
	return api_status;
}

enum sii_api_status sii_flash_update(
		sii_inst_t inst, const char* file_name)
{
	enum sii_api_status api_status = SII_API_SUCCESS;
	sii_device_isr_enable(inst, false);
	if (flash_update(inst, file_name) != SII_STATUS_SUCCESS)
		api_status = SII_API_FAILURE;
	return api_status;
}

void sii_reset_device(sii_inst_t inst, uint32_t time_msec)
{
	sii_device_reset(inst, time_msec);
}

enum sii_api_status sii_platform_init(uint8_t *dev_num,
		sii_hwid_t* hwid, uint32_t *dev_id)
{
	enum sii_api_status api_status = SII_API_SUCCESS;
	if (sii_os_init(1) != SII_OS_STATUS_SUCCESS) {
		api_status = SII_API_FAILURE;
		goto exit;
	}
	if (SII_OS_STATUS_SUCCESS !=
			sii_hal_platform_init(dev_num, hwid, dev_id)) {
		api_status = SII_API_FAILURE;
		goto exit;
	}
exit:
	return api_status;
}

enum sii_api_status sii_platform_term(void)
{
	enum sii_api_status api_status = SII_API_SUCCESS;
	sii_hal_platform_term();
	if (sii_os_term() != SII_OS_STATUS_SUCCESS)
		api_status = SII_API_FAILURE;
	return api_status;
}

enum sii_api_status sii_set_tx_scramble(sii_inst_t inst, bool enable)
{
	enum sii_api_status api_status = SII_API_SUCCESS;
	enum sii_ipc_api_status ipc_status = SII_IPC_API_SUCCESS;

	ipc_status = sii_host_set_tx_scramble(inst, enable);
	if (ipc_status != SII_IPC_API_SUCCESS)
		api_status = SII_API_FAILURE;

	return api_status;
}

enum sii_api_status sii_set_tx_scramble_full(
	sii_inst_t inst, enum sii_scramble_state state)
{
	enum sii_api_status api_status = SII_API_SUCCESS;
	enum sii_ipc_api_status ipc_status = SII_IPC_API_SUCCESS;

	ipc_status = sii_host_set_tx_scramble_full(inst, state);
	if (ipc_status != SII_IPC_API_SUCCESS)
		api_status = SII_API_FAILURE;

	return api_status;
}

enum sii_api_status sii_get_tx_char_error_count(
		sii_inst_t inst,
		struct sii_char_error_count *count)
{
	enum sii_api_status api_status = SII_API_SUCCESS;
	enum sii_ipc_api_status ipc_status = SII_IPC_API_SUCCESS;
	struct sii_ipc_message *msg = &ipc_host_context->host_message;

	ipc_status = sii_host_get_tx_char_error_count(inst, msg);
	if (ipc_status != SII_IPC_API_SUCCESS) {
		api_status = SII_API_FAILURE;
	} else {
		memcpy(count, msg->message_payload.message_data, sizeof(*count));
	}
	return api_status;
}

enum sii_api_status sii_get_tx_device_id_string(
		sii_inst_t inst,
		struct sii_device_id_string *string)
{
	enum sii_api_status api_status = SII_API_SUCCESS;
	enum sii_ipc_api_status ipc_status = SII_IPC_API_SUCCESS;
	struct sii_ipc_message *msg = &ipc_host_context->host_message;

	ipc_status = sii_host_get_tx_device_id_string(inst, msg);
	if (ipc_status != SII_IPC_API_SUCCESS) {
		api_status = SII_API_FAILURE;
	} else {
		memcpy(string, msg->message_payload.message_data, sizeof(*string));
	}
	return api_status;
}

enum sii_api_status sii_set_tx_color_space(
		sii_inst_t inst, enum sii_color_space type)
{
	enum sii_api_status api_status = SII_API_SUCCESS;
	enum sii_ipc_api_status ipc_status = SII_IPC_API_SUCCESS;

	ipc_status = sii_host_set_tx_color_space(inst, type);
	if (ipc_status != SII_IPC_API_SUCCESS)
		api_status = SII_API_FAILURE;

	return api_status;
}

enum sii_api_status sii_set_tx_hdcp_protection(
		sii_inst_t inst, bool enable)
{
	enum sii_api_status api_status = SII_API_SUCCESS;
	enum sii_ipc_api_status ipc_status = SII_IPC_API_SUCCESS;

	ipc_status = sii_host_set_tx_hdcp_protection(inst, enable);
	if (ipc_status != SII_IPC_API_SUCCESS)
		api_status = SII_API_FAILURE;

	return api_status;
}

enum sii_api_status sii_set_tx_hdcp_content_type(
		sii_inst_t inst,
		enum sii_hdcp_content_type type)
{
	enum sii_api_status api_status = SII_API_SUCCESS;
	enum sii_ipc_api_status ipc_status = SII_IPC_API_SUCCESS;

	ipc_status = sii_host_set_tx_hdcp_content_type(inst, type);
	if (ipc_status != SII_IPC_API_SUCCESS)
		api_status = SII_API_FAILURE;

	return api_status;
}

enum sii_api_status sii_get_tx_hdcp_ds_version(
		sii_inst_t inst,
		enum sii_hdcp_version *version)
{
	enum sii_api_status api_status = SII_API_SUCCESS;
	enum sii_ipc_api_status ipc_status = SII_IPC_API_SUCCESS;
	struct sii_ipc_register_reply reply;

	ipc_status = sii_host_get_tx_hdcp_ds_version(inst, &reply);
	if (ipc_status != SII_IPC_API_SUCCESS) {
		*version = 0;
		api_status = SII_API_FAILURE;
	} else {
		*version = (enum sii_hdcp_version) reply.reply_return;
		dbg("get tx hdcp version: %d\n", *version);
	}

	return api_status;
}

enum sii_api_status sii_get_tx_hdcp_status(
		sii_inst_t inst, enum sii_hdcp_status *status)
{
	enum sii_api_status api_status = SII_API_SUCCESS;
	enum sii_ipc_api_status ipc_status = SII_IPC_API_SUCCESS;
	struct sii_ipc_message *msg = &ipc_host_context->host_message;

	ipc_status = sii_host_get_tx_hdcp_status(inst, msg);
	if (ipc_status != SII_IPC_API_SUCCESS) {
		*status = 0;
		api_status = SII_API_FAILURE;
	} else {
		*status = (enum sii_hdcp_status) msg->message_payload.message_data[0];
		dbg("get tx hdcp status: %d\n", *status);
	}

	return api_status;
}

enum sii_api_status sii_set_tx_audio_insertion(
		sii_inst_t inst, struct sii_audio_format *format)
{
	enum sii_api_status api_status = SII_API_SUCCESS;
	enum sii_ipc_api_status ipc_status = SII_IPC_API_SUCCESS;
	struct sii_ipc_message *msg = &ipc_host_context->host_message;

	memset(msg, 0, sizeof(*msg));
	memcpy(msg->message_payload.message_data, format, sizeof(*format));
	msg->message_length_l = sizeof(*format);
	msg->message_length_h = 0;

	ipc_status = sii_host_set_tx_audio_insertion(inst, msg);
	if (ipc_status != SII_IPC_API_SUCCESS)
		api_status = SII_API_FAILURE;

	return api_status;
}

enum sii_api_status sii_get_tx_hdcp_rcvid(
		sii_inst_t inst, struct sii_hdcp_ksv *rcvid)
{
	enum sii_api_status api_status = SII_API_SUCCESS;
	enum sii_ipc_api_status ipc_status = SII_IPC_API_SUCCESS;
	struct sii_ipc_message *msg = &ipc_host_context->host_message;
#if 0
	uint8_t i;
#endif

	ipc_status = sii_host_get_tx_hdcp_rcvid(inst, msg);
	if (ipc_status != SII_IPC_API_SUCCESS) {
		api_status = SII_API_FAILURE;
	} else {
		memcpy(rcvid->key,
				msg->message_payload.message_data,
				sizeof(struct sii_hdcp_ksv));
#if 0
		for (i = 0; i < sizeof(struct sii_hdcp_ksv); i++)
		{
			dbg("%d:\t%02x\n", i, rcvid->key[i]);
		}
#endif
	}

	return api_status;
}

enum sii_api_status sii_get_tx_hdcp_ds_rxid_list(
		sii_inst_t inst, struct sii_hdcp_ksv *ds_rxid_list, uint8_t length)
{
	enum sii_api_status api_status = SII_API_SUCCESS;
	enum sii_ipc_api_status ipc_status = SII_IPC_API_SUCCESS;
	struct sii_ipc_message *msg = &ipc_host_context->host_message;

	memset(msg, 0, sizeof(*msg));
	msg->message_payload.message_data[0] = length;
	msg->message_length_l = 1;
	msg->message_length_h = 0;

	ipc_status = sii_host_get_tx_hdcp_ds_rxid_list(inst, msg);
	if (ipc_status != SII_IPC_API_SUCCESS) {
		api_status = SII_API_FAILURE;
	} else {
		memcpy(ds_rxid_list, msg->message_payload.message_data, length * 5);
	}

	return api_status;
}

enum sii_api_status sii_get_tx_hdcp_ds_repeater_bit(
		sii_inst_t inst, bool *repeaterBit)
{
	enum sii_api_status api_status = SII_API_SUCCESS;
	enum sii_ipc_api_status ipc_status = SII_IPC_API_SUCCESS;
	struct sii_ipc_register_reply reply;

	ipc_status = sii_host_get_tx_hdcp_ds_repeater_bit(inst, &reply);
	if (ipc_status != SII_IPC_API_SUCCESS) {
		*repeaterBit = 0;
		api_status = SII_API_FAILURE;
	} else {
		*repeaterBit = (bool)reply.reply_return;
		dbg("get tx ds repeater bit: %d\n", *repeaterBit);
	}

	return api_status;
}

enum sii_api_status sii_get_tx_hdcp_ds_topology(
		sii_inst_t inst, struct sii_hdcp_topology *topology)
{
	enum sii_api_status api_status = SII_API_SUCCESS;
	enum sii_ipc_api_status ipc_status = SII_IPC_API_SUCCESS;
	struct sii_ipc_message *msg = &ipc_host_context->host_message;

	ipc_status = sii_host_get_tx_hdcp_ds_topology(inst, msg);
	if (ipc_status != SII_IPC_API_SUCCESS) {
		api_status = SII_API_FAILURE;
	} else {
		memcpy(topology,
				msg->message_payload.message_data,
				sizeof(struct sii_hdcp_topology));
	}

	return api_status;
}

enum sii_api_status sii_set_tx_hdcp_stream_manage_info(
		sii_inst_t inst, struct sii_hdcp_stream_manage_info *info)
{
	enum sii_api_status api_status = SII_API_SUCCESS;
	enum sii_ipc_api_status ipc_status = SII_IPC_API_SUCCESS;
	struct sii_ipc_message *msg = &ipc_host_context->host_message;

	memset(msg, 0, sizeof(*msg));
	memcpy(msg->message_payload.message_data, info, sizeof(*info));
	msg->message_length_l = sizeof(*info);
	msg->message_length_h = 0;

	ipc_status = sii_host_set_tx_hdcp_stream_manage_info(inst, msg);
	if (ipc_status != SII_IPC_API_SUCCESS)
		api_status = SII_API_FAILURE;

	return api_status;
}

