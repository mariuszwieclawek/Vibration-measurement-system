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

/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS   3000

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

	/* Aceelerometer */
	// acc_meas_values.x_axis_val = 1;
	// acc_meas_values.y_axis_val = 2;
	// acc_meas_values.z_axis_val = 3;
	int x = 0;
	int y = 10;
	int z = 20;

	/* Infinite loop */
	while (1) {
		led_toggle(LED3);
		k_msleep(SLEEP_TIME_MS);
		err = bluetooth_send_acc_val_notif(x, 1, ACC_X_VALUE_CHAR);
		if(err){
			LOG_ERR("Notification error:%d", ACC_X_VALUE_CHAR);
		}

		err = bluetooth_send_acc_val_notif(y, 1, ACC_Y_VALUE_CHAR);
		if(err){
			LOG_ERR("Notification error:%d", ACC_Y_VALUE_CHAR);
		}

		err = bluetooth_send_acc_val_notif(z, 1, ACC_Z_VALUE_CHAR);
		if(err){
			LOG_ERR("Notification error:%d", ACC_Z_VALUE_CHAR);
		}

		x++;
		y++;
		z++;
	}
}
