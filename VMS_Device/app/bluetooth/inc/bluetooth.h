#ifndef _BLUETOOTH_H_
#define _BLUETOOTH_H_

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/hci.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/gatt.h>
#include <zephyr/logging/log.h>

#include "button.h"
#include "acc.h"
#include "led.h"

/** @brief UUID of the Accelerometer Service. **/
#define BT_UUID_SERV_ACC \
	BT_UUID_128_ENCODE(0xe9ea0001, 0xe19b, 0x482d, 0x9293, 0xc7907585fc48)

/** @brief UUID of the Accelerometer Characteristic for X-Axis measuerement value. **/
#define BT_UUID_CHR_ACC_X_VAL \
	BT_UUID_128_ENCODE(0xe9ea0002, 0xe19b, 0x482d, 0x9293, 0xc7907585fc48)

/** @brief UUID of the Accelerometer Characteristic for Y-Axis measuerement value. **/
#define BT_UUID_CHR_ACC_Y_VAL \
	BT_UUID_128_ENCODE(0xe9ea0003, 0xe19b, 0x482d, 0x9293, 0xc7907585fc48)

/** @brief UUID of the Accelerometer Characteristic for Z-Axis measuerement value. **/
#define BT_UUID_CHR_ACC_Z_VAL \
	BT_UUID_128_ENCODE(0xe9ea0004, 0xe19b, 0x482d, 0x9293, 0xc7907585fc48)

/** @brief UUID of the Accelerometer Characteristic for Z-Axis measuerement value. **/
#define BT_UUID_CHR_PWM_VAL \
	BT_UUID_128_ENCODE(0xe9ea0005, 0xe19b, 0x482d, 0x9293, 0xc7907585fc48)


#define BT_UUID_ACC_SERVICE        BT_UUID_DECLARE_128(BT_UUID_SERV_ACC)
#define BT_UUID_ACC_X_VAL_CHRC 	   BT_UUID_DECLARE_128(BT_UUID_CHR_ACC_X_VAL)
#define BT_UUID_ACC_Y_VAL_CHRC     BT_UUID_DECLARE_128(BT_UUID_CHR_ACC_Y_VAL)
#define BT_UUID_ACC_Z_VAL_CHRC     BT_UUID_DECLARE_128(BT_UUID_CHR_ACC_Z_VAL)
#define BT_UUID_ACC_PWM_VAL_CHRC   BT_UUID_DECLARE_128(BT_UUID_CHR_PWM_VAL)

typedef enum{
	ACC_X_VALUE_CHAR = 0,
	ACC_Y_VALUE_CHAR,
	ACC_Z_VALUE_CHAR,
}char_choice;

int bluetooth_init(void);
int bluetooth_send_acc_val_notif(uint8_t value, uint16_t length, char_choice sel);

#endif /* _BLUETOOTH_H_ */

