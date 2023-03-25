#ifndef _BLUETOOTH_CENTRAL_H_
#define _BLUETOOTH_CENTRAL_H_

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/hci.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/gatt.h>
#include <zephyr/logging/log.h>

#include "button.h"

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


#define BT_UUID_ACC_SERVICE        BT_UUID_DECLARE_128(BT_UUID_SERV_ACC)
#define BT_UUID_ACC_X_VAL_CHRC 	   BT_UUID_DECLARE_128(BT_UUID_CHR_ACC_X_VAL)
#define BT_UUID_ACC_Y_VAL_CHRC     BT_UUID_DECLARE_128(BT_UUID_CHR_ACC_Y_VAL)
#define BT_UUID_ACC_Z_VAL_CHRC     BT_UUID_DECLARE_128(BT_UUID_CHR_ACC_Z_VAL)

int bluetooth_init(void);

#endif /* _BLUETOOTH_CENTRAL_H_ */

