/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>
#include <zephyr/logging/log.h>
#include "led.h"
#include "button.h"
#include "bluetooth.h"
#include "acc.h"
#include "pwm.h"

/* Name of logging module for this file */
LOG_MODULE_REGISTER(MAIN);


void main(void)
{
	LOG_INF("Hello World! : Board name: %s", CONFIG_BOARD);
	/* Led */
	led_init_all_diodes();
	led_set_state(LED0, ACTIVE);

	/* Button */
	button_init_all_switches();
	button_init_interrupts_for_all_switches();

	/* Bluetooth */
	int err = bluetooth_init();
	if(err){
		LOG_ERR("Bluetooth init failed");
	}

	int x = 0;
	int y = 10;
	int z = 20;

	/* Aceelerometer Init */
	adxl345_init();
	

	/* Infinite loop */
	while (1) {
		//led_toggle(LED3);
		k_msleep(SLEEP_TIME_MS);

		/* Aceelerometer Read */
		int16_t* i2c_buffer_values = adxl345_read();
		printk("Values: X:%d  Y:%d  Z:%d\n", i2c_buffer_values[0], i2c_buffer_values[1], i2c_buffer_values[2]);

		// err = bluetooth_send_acc_val_notif(x, 1, ACC_X_VALUE_CHAR);
		// if(err){
		// 	LOG_ERR("Notification error:%d", ACC_X_VALUE_CHAR);
		// }

		// err = bluetooth_send_acc_val_notif(y, 1, ACC_Y_VALUE_CHAR);
		// if(err){
		// 	LOG_ERR("Notification error:%d", ACC_Y_VALUE_CHAR);
		// }

		// err = bluetooth_send_acc_val_notif(z, 1, ACC_Z_VALUE_CHAR);
		// if(err){
		// 	LOG_ERR("Notification error:%d", ACC_Z_VALUE_CHAR);
		// }

	}
}