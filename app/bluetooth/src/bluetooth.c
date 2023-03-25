#include "bluetooth.h"

/* Create logging module for bluetooth*/
LOG_MODULE_REGISTER(BLUETOOTH_CENTRAL);

/* Create the semaphore for bluetooth */
static K_SEM_DEFINE(ble_init_ok, 1, 1);

/* Bluetooth device name and lenght of device name */
#define DEVICE_NAME CONFIG_BT_DEVICE_NAME
#define DEVICE_NAME_LEN (sizeof(DEVICE_NAME) - 1)

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


/* Declare custom service */
BT_GATT_SERVICE_DEFINE(accelerometer_service,
BT_GATT_PRIMARY_SERVICE(BT_UUID_ACC_SERVICE),
    BT_GATT_CHARACTERISTIC(BT_UUID_ACC_X_VAL_CHRC,
                    BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,
                    BT_GATT_PERM_READ,
                    read_accelerometer_x_val_cb, NULL, NULL),
	BT_GATT_CHARACTERISTIC(BT_UUID_ACC_Y_VAL_CHRC,
                    BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,
                    BT_GATT_PERM_READ,
                    read_accelerometer_y_val_cb, NULL, NULL),
	BT_GATT_CHARACTERISTIC(BT_UUID_ACC_Z_VAL_CHRC,
                    BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,
                    BT_GATT_PERM_READ,
                    read_accelerometer_z_val_cb, NULL, NULL),
    //BT_GATT_CCC(button_chrc_ccc_cfg_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
);

/* Callback for read characteristic event (X-Axis value) */
static ssize_t read_accelerometer_x_val_cb(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, uint16_t len, uint16_t offset)
{
	LOG_INF("Read X-Axis value");
	return bt_gatt_attr_read(conn, attr, buf, len, offset, &button_counter, sizeof(button_counter));
}

/* Callback for read characteristic event (Y-Axis value) */
static ssize_t read_accelerometer_y_val_cb(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, uint16_t len, uint16_t offset)
{
	LOG_INF("Read Y-Axis value");
}

/* Callback for read characteristic event (Z-Axis value) */
static ssize_t read_accelerometer_z_val_cb(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, uint16_t len, uint16_t offset)
{
	LOG_INF("Read Z-Axis value");
}



// static void device_found(const bt_addr_le_t *addr, int8_t rssi, uint8_t type, struct net_buf_simple *ad)
// {
// 	char dev[BT_ADDR_LE_STR_LEN];
// 	struct bt_conn *conn;
// 	int err;

// 	bt_addr_le_to_str(addr, dev, sizeof(dev));
// 	LOG_INF("[DEVICE]: %s, AD evt type %u, AD data len %u, RSSI %i\n", dev, type, ad->len, rssi);

// 	/* We're only interested in connectable events */
// 	if (type != BT_GAP_ADV_TYPE_ADV_IND &&
// 	    type != BT_GAP_ADV_TYPE_ADV_DIRECT_IND) {
// 		return;
// 	}

// 	/* connect only to devices in close proximity */
// 	if (rssi < -70) {
// 		return;
// 	}

// 	err = bt_le_scan_stop();
// 	if (err) {
// 		LOG_ERR("%s: Stop LE scan failed (err %d)\n", __func__, err);
// 		return;
// 	}

// 	err = bt_conn_le_create(addr, BT_CONN_LE_CREATE_CONN, BT_LE_CONN_PARAM_DEFAULT, &conn);
// 	if (err) {
// 		LOG_ERR("%s: Create conn failed (err %d)\n", __func__, err);
// 		start_scan_func();
// 	} else {
// 		bt_conn_unref(conn);
// 	}
// }



/* Connection callback */
static void ble_connected(struct bt_conn *conn, uint8_t err) 
{
	struct bt_conn_info info;
	char addr[BT_ADDR_LE_STR_LEN];

	//BTConnection = conn;

	if (err) {
		LOG_ERR("Connection failed (err %u)\n", err);
		return;
	} else if (bt_conn_get_info(conn, &info)) {
		LOG_ERR("Could not parse connection info\n");
	} else {
		bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));
		LOG_INF(
			"Connection established!		\n\
			Connected to: %s					\n\
			Role: %u							\n\
			Connection interval: %u				\n\
			Slave latency: %u					\n\
			Connection supervisory timeout: %u	\n",
			addr, info.role, info.le.interval, info.le.latency, info.le.timeout);
	}
  //BLEConnected = true;
}


/* Bluetooth connection callbacks for monitoring state of connection*/
static struct bt_conn_cb conn_callbacks = {.connected = ble_connected,
                                           //.disconnected = ble_disconnected,
                                           .le_param_req = NULL,
                                           .le_param_updated = NULL};


/* Callback for notifying that Bluetooth has been enabled. */
static void ble_ready(int32_t err) {
	if (err) {
		LOG_ERR("BLE init failed with error code %d\n", err);
		return;
	}

	/* Start advertising */
	err = bt_le_adv_start(BT_LE_ADV_CONN, ad, ARRAY_SIZE(ad), sd, ARRAY_SIZE(sd));
	if (err) {
		LOG_ERR("Advertising failed to start (err %d)\n", err);
		return;
	}

	k_sem_give(&ble_init_ok);



	/* Configure connection callbacks */
	bt_conn_cb_register(&conn_callbacks);
	/* Initalize custom service */
	// ble_service_init();
	/* Start advertising */
	// err = bt_le_adv_start(BT_LE_ADV_CONN, ad, ARRAY_SIZE(ad), sd, ARRAY_SIZE(sd));
	// if (err) {
	// 	LOG_ERR("Advertising failed to start (err %d)\n", err);
	// 	return;
	// }
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


/* Function start scanning  */
// void bluetooth_start_scan(void)
// {
// 	int err;
// 	err = bt_le_scan_start(BT_LE_SCAN_PASSIVE, device_found);
// 	if (err) {
// 		LOG_ERR("Scanning failed to start (err %d)\n", err);
// 	}
// 	LOG_INF("Scanning successfully started\n");
// }