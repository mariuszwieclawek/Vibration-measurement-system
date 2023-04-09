#include "bluetooth.h"

/* Create logging module for bluetooth*/
LOG_MODULE_REGISTER(BLUETOOTH);

/* Create the semaphore for bluetooth */
static K_SEM_DEFINE(ble_init_ok, 1, 1);

/* Bluetooth device name and lenght of device name */
#define DEVICE_NAME CONFIG_BT_DEVICE_NAME
#define DEVICE_NAME_LEN (sizeof(DEVICE_NAME) - 1)

/* Current connection struct */
static struct bt_conn *current_conn;

/* Bluetooth advertise data */
static const struct bt_data ad[] = {
    BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
    BT_DATA(BT_DATA_NAME_COMPLETE, DEVICE_NAME, DEVICE_NAME_LEN)
};

/* Bluetooth scan response data */
static const struct bt_data sd[] = {
    BT_DATA_BYTES(BT_DATA_UUID128_ALL, BT_UUID_SERV_ACC),
};

/* Callbacks declaration for characteristics event trigger */
static ssize_t read_accelerometer_x_val_cb(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, uint16_t len, uint16_t offset);
static ssize_t read_accelerometer_y_val_cb(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, uint16_t len, uint16_t offset);
static ssize_t read_accelerometer_z_val_cb(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, uint16_t len, uint16_t offset);
static ssize_t write_pwm_val_cb(struct bt_conn *conn, const struct bt_gatt_attr *attr, const void *buf, uint16_t len, uint16_t offset, uint8_t flags);

/* Declare custom service */
BT_GATT_SERVICE_DEFINE(accelerometer_service,
BT_GATT_PRIMARY_SERVICE(BT_UUID_ACC_SERVICE),
    BT_GATT_CHARACTERISTIC(BT_UUID_ACC_X_VAL_CHRC,
                    BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,
                    BT_GATT_PERM_READ,
                    read_accelerometer_x_val_cb, NULL, NULL),
		BT_GATT_CCC(NULL, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	BT_GATT_CHARACTERISTIC(BT_UUID_ACC_Y_VAL_CHRC,
                    BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,
                    BT_GATT_PERM_READ,
                    read_accelerometer_y_val_cb, NULL, NULL),
		BT_GATT_CCC(NULL, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	BT_GATT_CHARACTERISTIC(BT_UUID_ACC_Z_VAL_CHRC,
                    BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,
                    BT_GATT_PERM_READ,
                    read_accelerometer_z_val_cb, NULL, NULL),
    	BT_GATT_CCC(NULL, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	BT_GATT_CHARACTERISTIC(BT_UUID_ACC_PWM_VAL_CHRC,
			       BT_GATT_CHRC_WRITE,
			       BT_GATT_PERM_WRITE,
			       NULL, write_pwm_val_cb, NULL),
);

/* Callback for read characteristic event (X-Axis value) */
static ssize_t read_accelerometer_x_val_cb(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, uint16_t len, uint16_t offset)
{
	LOG_INF("Read X-Axis value");
	return bt_gatt_attr_read(conn, attr, buf, len, offset, &acc_meas_values.x_axis_val, sizeof(acc_meas_values.x_axis_val));
}

/* Callback for read characteristic event (Y-Axis value) */
static ssize_t read_accelerometer_y_val_cb(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, uint16_t len, uint16_t offset)
{
	LOG_INF("Read Y-Axis value");
	return bt_gatt_attr_read(conn, attr, buf, len, offset, &acc_meas_values.y_axis_val, sizeof(acc_meas_values.y_axis_val));
}

/* Callback for read characteristic event (Z-Axis value) */
static ssize_t read_accelerometer_z_val_cb(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, uint16_t len, uint16_t offset)
{
	LOG_INF("Read Z-Axis value");
	return bt_gatt_attr_read(conn, attr, buf, len, offset, &acc_meas_values.z_axis_val, sizeof(acc_meas_values.z_axis_val));
}

static ssize_t write_pwm_val_cb(struct bt_conn *conn, const struct bt_gatt_attr *attr, const void *buf, uint16_t len, uint16_t offset, uint8_t flags)
{
	LOG_DBG("Attribute write, handle: %u, conn: %p", attr->handle,
		(void *)conn);

	if (len != 1U) {
		LOG_DBG("Write led: Incorrect data length");
		return BT_GATT_ERR(BT_ATT_ERR_INVALID_ATTRIBUTE_LEN);
	}

	if (offset != 0) {
		LOG_DBG("Write led: Incorrect data offset");
		return BT_GATT_ERR(BT_ATT_ERR_INVALID_OFFSET);
	}

	//TO DO
	uint8_t val = *((uint8_t *)buf);
	LOG_INF("Write PWM value: %d", val);

	return len;
}


static void sent_notif_cb(struct bt_conn *conn, void *user_data)
{
	ARG_UNUSED(user_data);
    LOG_INF("Notification sent on connection %p", (void *)conn);
}

int bluetooth_send_acc_val_notif(uint8_t value, uint16_t length, char_choice sel)
{
    int err = 0;
	/* Attribute table: 0 = Service, 1 = Primary service, 2 = X_VAL, 3 = CCC, 4 = Y_VAL, 5 = CCC, 6 = Z_VAL, 7 = CCC, 8 = PWM_VAL*/
	const struct bt_gatt_attr *attr;
    struct bt_gatt_notify_params params;
	params.data = &value;
	params.len = length;
	params.func = sent_notif_cb;
	
	switch (sel)
	{
	case ACC_X_VALUE_CHAR:
		attr = &accelerometer_service.attrs[2];
		params.uuid = BT_UUID_ACC_X_VAL_CHRC;
		params.attr = attr;
		break;
	case ACC_Y_VALUE_CHAR:
		attr = &accelerometer_service.attrs[5];
		params.uuid = BT_UUID_ACC_Y_VAL_CHRC;
		params.attr = attr;
		break;
	case ACC_Z_VALUE_CHAR:
		attr = &accelerometer_service.attrs[7];
		params.uuid = BT_UUID_ACC_Z_VAL_CHRC;
		params.attr = attr;
		break;
	default:
		LOG_ERR("Wrong char selected: %d", sel);
		return -1;
	}

	/* Check whether notifications are enabled or not */
  	if (bt_gatt_is_subscribed(current_conn, attr, BT_GATT_CCC_NOTIFY)) {
		/* Send the notification */
		err = bt_gatt_notify_cb(current_conn, &params);
		if(err < 0){
			LOG_ERR("Error, unable to send notification\n");
		}
	} 
	else {
		LOG_WRN("Notification not enabled on the selected attribute\n");
	}
    return err;
}

/* Connection callback */
static void ble_connected(struct bt_conn *conn, uint8_t err) 
{
	struct bt_conn_info info;
	char addr[BT_ADDR_LE_STR_LEN];

	if (err) {
		LOG_ERR("Connection failed (err %u)\n", err);
		return;
	} else if (bt_conn_get_info(conn, &info)) {
		LOG_ERR("Could not parse connection info\n");
	} else {
		/* Active LED1 */
		led_set_state(LED1, ACTIVE);
		/* Increment reference connection count */
		current_conn = bt_conn_ref(conn);
		/* Parse connection data */
		bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));
		LOG_INF(
			"Connection established!		\n\r\
			Connected to: %s					\n\r\
			Role: %u							\n\r\
			Connection interval: %u				\n\r\
			Slave latency: %u					\n\r\
			Connection supervisory timeout: %u	\n",
			addr, info.role, info.le.interval, info.le.latency, info.le.timeout);
	}
}

/* Disconnect callback */
static void ble_disconnected(struct bt_conn *conn, uint8_t reason)
{
	LOG_INF("Disconnected (reason: %d)", reason);
	if(current_conn) {
		bt_conn_unref(current_conn);
		current_conn = NULL;
	}
	/* Inactive LED1 */
	led_set_state(LED1, INACTIVE);
}


/* Bluetooth connection callbacks for monitoring state of connection*/
static struct bt_conn_cb conn_callbacks = {.connected = ble_connected,
                                           .disconnected = ble_disconnected,
                                           .le_param_req = NULL,
                                           .le_param_updated = NULL};


/* Callback for notifying that Bluetooth has been enabled. */
static void ble_ready(int32_t err) {
	if (err) {
		LOG_ERR("BLE init failed with error code %d\n", err);
		return;
	}
	/* Configure connection callbacks */
	bt_conn_cb_register(&conn_callbacks);
	/* Start advertising */
	err = bt_le_adv_start(BT_LE_ADV_CONN, ad, ARRAY_SIZE(ad), sd, ARRAY_SIZE(sd));
	if (err) {
		LOG_ERR("Advertising failed to start (err %d)\n", err);
		return;
	}
	/* Give semaphore when ble is initialized */
	k_sem_give(&ble_init_ok);
}

/* Function initialize bluetooth module */
int bluetooth_init(void)
{
	int err;
	/* Enable BLE */
	err = bt_enable(ble_ready);
	if (err) {
		LOG_ERR("Bluetooth init failed (err %d)", err);
		return err;
	}
	else{
		LOG_INF("Bluetooth initialized");
	}
	/* Wait until ble initialized */
	err = k_sem_take(&ble_init_ok, K_MSEC(500));
	if (err != 0) {
    	LOG_INF("BLE initialization did not complete in time");
		return err;
	}
	return err;
}
