#ifndef __TASK_LED_H_
#define __TASK_LED_H_

typedef void (*led_work_status_fun)(uint8_t);

typedef enum _led_work_status_t_
{
	led_work_none = 0,

	led_work_always_off,
	led_work_always_on,
	led_work_moment_on,
	led_work_moment_off,
	led_work_moment_toggle,
	led_work_norm_blink,
	led_work_fast_blink,
	led_work_slow_blink,
    // add by wf8421 20190218
	led_work_moment_on_3s,
	led_work_moment_off_3s,

	led_work_status_num
}led_work_status_t;

typedef struct _led_work_run_t_
{
	led_work_status_t 	status;
	led_work_status_t 	pre_status;
	uint16_t 			blink_toggle_time;
}led_work_run_t, *pLed_work_run_t;

typedef struct _led_location_t_
{
	uint8_t 			l_port;
	uint16_t			l_pin;
	uint8_t 			l_on_level;
}led_location_t, *pled_location_t;

#define led_on(led_name)					led_work_status_assign(led_name, led_work_always_on)
#define led_off(led_name)					led_work_status_assign(led_name, led_work_always_off)
#define led_moment_on(led_name)				led_work_status_assign(led_name, led_work_moment_on)
#define led_moment_off(led_name)			led_work_status_assign(led_name, led_work_moment_off)
#define led_moment_toggle(led_name)			led_work_status_assign(led_name, led_work_moment_toggle)
#define led_norm_blink(led_name)			led_work_status_assign(led_name, led_work_norm_blink)
#define led_fast_blink(led_name)			led_work_status_assign(led_name, led_work_fast_blink)
#define led_slow_blink(led_name)			led_work_status_assign(led_name, led_work_slow_blink)
#if _ENABLE_LED_WORK_MOMENT_3S
#define led_moment_on_3s(led_name)          led_work_status_assign(led_name, led_work_moment_on_3s)
#define led_moment_off_3s(led_name)			led_work_status_assign(led_name, led_work_moment_off_3s)
#endif

//function
void vTaskLED(void *pvParameters);
void led_init(void);
void led_work_status_assign(uint8_t led_name, led_work_status_t led_status);
void led_cur_state_set(uint8_t led_name, led_work_status_t led_status);
led_work_status_t led_cur_state_get(uint8_t led_name);
#if (_SYSTEM_SUPPORT_RTOS==0)
void TaskLedInit(void);
#endif

#endif


















