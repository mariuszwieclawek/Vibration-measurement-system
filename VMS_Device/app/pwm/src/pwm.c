#include <zephyr/drivers/pwm.h>
#include <zephyr/logging/log.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>

#include "common_types.h"
#include "pwm.h"

/* Logging module */
LOG_MODULE_REGISTER(PWM);

/* PWM structures indicating port and pin is control by pwm */
static const struct pwm_dt_spec pwm = PWM_DT_SPEC_GET(DT_ALIAS(pwm_led0));

/* Function declaration */
void pwm_init(void);
ret_status pwm_set_motor_value(uint8_t value);

/* Function initialize PWM */
void pwm_init(void){
	if (!device_is_ready(pwm.dev)) {
		LOG_ERR("Error: PWM device %s is not ready\n", pwm.dev->name);
		return;
	}
}

/* Set PWM value for vibration motor */
ret_status pwm_set_motor_value(uint8_t value){
	uint32_t max_period = 1000;
	uint32_t period;
	int err;

	/* value is in range 0-100 so its have to be scaled to 0-1000 */
	period = value * 10;
	err = pwm_set_dt(&pwm, max_period, period);
	if (err) {
		LOG_ERR("Failed to set pulse width. Error %d", err);
		return ERROR;
	}
	LOG_INF("PWM value set to: %d", value);
	return SUCCESS;
}

