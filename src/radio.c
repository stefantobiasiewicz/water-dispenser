#include "radio.h"


#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/hci.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/gatt.h>
#include <zephyr/logging/log.h>

LOG_MODULE_DECLARE(radio, LOG_LEVEL_DBG);

static struct api_water_dispenser api_callbacks;


static uint16_t imps = 0;

ssize_t bt_impset_read(struct bt_conn *conn,
					     const struct bt_gatt_attr *attr,
					     const void *buf, uint16_t len,
					     uint16_t offset, uint8_t flags) 
{
	LOG_INF("bt_impset_read");
	
	uint16_t *value = attr->user_data;
	
	return bt_gatt_attr_read(conn, attr, buf, len, offset, value, sizeof(*value));
}	

ssize_t bt_impset_write(struct bt_conn *conn,
					     const struct bt_gatt_attr *attr,
					     const void *buf, uint16_t len,
					     uint16_t offset, uint8_t flags) 
{
	LOG_INF("bt_impset_write");
	LOG_DBG("data len: %d, offset: %d, flags: %d", len, offset, flags);

	uint16_t val = *((uint16_t *)buf);

	LOG_INF("value: %d", val);

	imps = val;

	if(api_callbacks.api_set_count) {
		api_callbacks.api_set_count(imps);
	};

	return len;
}	

ssize_t bt_run(struct bt_conn *conn,
					     const struct bt_gatt_attr *attr,
					     const void *buf, uint16_t len,
					     uint16_t offset, uint8_t flags) 
{
	LOG_INF("bt_run");
	LOG_DBG("data len: %d, offset: %d, flags: %d", len, offset, flags);

	LOG_DBG("Attribute write, handle: %u, conn: %p", attr->handle, (void *)conn);

	if (len != 1U) {
		LOG_DBG("Write led: Incorrect data length");
		return BT_GATT_ERR(BT_ATT_ERR_INVALID_ATTRIBUTE_LEN);
	}

	if (offset != 0) {
		LOG_DBG("Write led: Incorrect data offset");
		return BT_GATT_ERR(BT_ATT_ERR_INVALID_OFFSET);
	}

	if (api_callbacks.api_run) {
		uint8_t val = *((uint8_t *)buf);

		if (val == 0x01) {
			api_callbacks.api_run();
			imps = 0;
		} else if (val == 0x00) {
			api_callbacks.api_run_stop();
			imps = 0;
		}
		else {
			LOG_DBG("Write led: Incorrect value");
			return BT_GATT_ERR(BT_ATT_ERR_VALUE_NOT_ALLOWED);
		}
	}

	return len;
}	

ssize_t bt_identify(struct bt_conn *conn,
					     const struct bt_gatt_attr *attr,
					     const void *buf, uint16_t len,
					     uint16_t offset, uint8_t flags) 
{
	LOG_INF("bt_identify");

	LOG_DBG("data len: %d, offset: %d, flags: %d", len, offset, flags);

	if (len != 1U) {
		LOG_DBG("Write led: Incorrect data length");
		return BT_GATT_ERR(BT_ATT_ERR_INVALID_ATTRIBUTE_LEN);
	}

	if (offset != 0) {
		LOG_DBG("Write led: Incorrect data offset");
		return BT_GATT_ERR(BT_ATT_ERR_INVALID_OFFSET);
	}

	if (api_callbacks.api_run) {
		uint8_t val = *((uint8_t *)buf);

		if (val == 0x01) {
			api_callbacks.api_identify_on();
		} else if (val == 0x00) {
			api_callbacks.api_identify_off();
		}
		else {
			LOG_DBG("Write led: Incorrect value");
			return BT_GATT_ERR(BT_ATT_ERR_VALUE_NOT_ALLOWED);
		}
	}

	return len;
}	

ssize_t bt_on_off(struct bt_conn *conn,
					     const struct bt_gatt_attr *attr,
					     const void *buf, uint16_t len,
					     uint16_t offset, uint8_t flags) 
{
	LOG_INF("bt_on_off");
	LOG_DBG("data len: %d, offset: %d, flags: %d", len, offset, flags);

	LOG_DBG("Attribute write, handle: %u, conn: %p", attr->handle, (void *)conn);

	if (len != 1U) {
		LOG_DBG("Write led: Incorrect data length");
		return BT_GATT_ERR(BT_ATT_ERR_INVALID_ATTRIBUTE_LEN);
	}

	if (offset != 0) {
		LOG_DBG("Write led: Incorrect data offset");
		return BT_GATT_ERR(BT_ATT_ERR_INVALID_OFFSET);
	}

	if (api_callbacks.api_run) {
		uint8_t val = *((uint8_t *)buf);

		if (val == 0x01) {
			api_callbacks.api_pump_on();
		} else if (val == 0x00) {
			api_callbacks.api_pump_off();
		}
		else {
			LOG_DBG("Write led: Incorrect value");
			return BT_GATT_ERR(BT_ATT_ERR_VALUE_NOT_ALLOWED);
		}
	}

	return len;
}	

BT_GATT_SERVICE_DEFINE(my_lbs_svc,
	BT_GATT_PRIMARY_SERVICE(BT_UUID_WATER_DISPENSER),
	BT_GATT_CHARACTERISTIC(BT_UUID_IMPUSET,
					(BT_GATT_CHRC_WRITE | BT_GATT_CHRC_READ),
					(BT_GATT_PERM_WRITE | BT_GATT_PERM_READ), 
					bt_impset_read, 
					bt_impset_write,
					&imps),
	BT_GATT_CHARACTERISTIC(BT_UUID_RUN,
					BT_GATT_CHRC_WRITE,
					BT_GATT_PERM_WRITE,
					NULL,
					bt_run,
					NULL),
	BT_GATT_CHARACTERISTIC(BT_UUID_IDENTIFY,
					BT_GATT_CHRC_WRITE,
					BT_GATT_PERM_WRITE,
					NULL,
					bt_identify,
					NULL),
	BT_GATT_CHARACTERISTIC(BT_UUID_ON_OFF,
					BT_GATT_CHRC_WRITE,
					BT_GATT_PERM_WRITE,
					NULL,
					bt_on_off,
					NULL),
);


#define COMPANY_ID_CODE            0x0059

typedef struct adv_mfg_data {
	uint16_t company_code;	    /* Company Identifier Code. */
	uint16_t number_press;      /* Number of times Button 1 is pressed*/
} adv_mfg_data_type;


static adv_mfg_data_type adv_mfg_data = {COMPANY_ID_CODE,0x00};

/*advertising packet*/
static const struct bt_data ad[] = {
	/* STEP 4.1.2 - Set the advertising flags */    
    BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
	/* STEP 4.1.3 - Set the advertising packet data  */
    BT_DATA(BT_DATA_NAME_COMPLETE, CONFIG_BT_DEVICE_NAME, sizeof(CONFIG_BT_DEVICE_NAME) - 1),
    BT_DATA(BT_DATA_MANUFACTURER_DATA,(unsigned char *)&adv_mfg_data, sizeof(adv_mfg_data)),
};


/*scan response*/
static const struct bt_data sd[] = {
        /* 4.2.3 Include the URL data in the scan response packet*/
        BT_DATA_BYTES(BT_DATA_UUID128_ALL, BT_UUID_WATER_DISPENSER_ENCODED),
};



static struct bt_le_adv_param *adv_param =
	BT_LE_ADV_PARAM(BT_LE_ADV_OPT_CONNECTABLE,
	800,
	801,
	NULL);


/* ---BLE CONNECTION--- */

struct bt_conn *my_conn = NULL;



static void on_connected(struct bt_conn *conn, uint8_t err)
{
    if (err) {
        LOG_ERR("Connection error %d", err);
        return;
    }
    LOG_INF("Connected");
    my_conn = bt_conn_ref(conn);
    
    struct bt_conn_info info;
    err = bt_conn_get_info(conn, &info);
    if (err) {
        LOG_ERR("bt_conn_get_info() returned %d", err);
        return;
    }
    uint16_t connection_interval = (uint16_t)info.le.interval*1.25; // in ms
    uint16_t supervision_timeout = info.le.timeout*10; // in ms
    LOG_INF("Connection parameters: interval %.2d ms, latency %d intervals, timeout %d ms", connection_interval, info.le.latency, supervision_timeout);
}
static void on_disconnected(struct bt_conn *conn, uint8_t reason)
{
    LOG_INF("Disconnected. Reason %d", reason);
    bt_conn_unref(my_conn);
    
	api_callbacks.api_identify_off();
}

static struct bt_conn_cb connection_callbacks = {
    .connected              = on_connected,
    .disconnected           = on_disconnected,
};


/* ---BLE CONNECTION--- */


static int radio_init() {
	int err;

	err = bt_enable(NULL); 
    if (err) {
        LOG_ERR("Bluetooth enabled failed (err %d)\n", err);
        return err;
    }
    LOG_INF("Bluetooth enabled\n");

	bt_addr_le_t addr;
    err = bt_addr_le_from_str("FF:EE:DD:CC:BB:AA", "random", &addr);
    if (err) {
        LOG_ERR("Invalid BT address (err %d)\n", err);
    }
    err = bt_id_create(&addr, NULL);
    if (err < 0) {
        LOG_ERR("Creating new ID failed (err %d)\n", err);
    }   

    bt_conn_cb_register(&connection_callbacks);

    err = bt_le_adv_start(adv_param, ad, ARRAY_SIZE(ad),
			      sd, ARRAY_SIZE(sd));
	if (err) {
		LOG_ERR("Advertising failed to start (err %d)\n", err);
		return err;
	}

	return err;
}



int radio_api_init(struct api_water_dispenser *callbacks) {
	if(callbacks) {
		api_callbacks.api_get_count = callbacks->api_get_count;
		api_callbacks.api_set_count = callbacks->api_set_count;
		api_callbacks.api_identify_off = callbacks->api_identify_off;
		api_callbacks.api_identify_on = callbacks->api_identify_on;
		api_callbacks.api_run = callbacks->api_run;
		api_callbacks.api_run_stop = callbacks->api_run_stop;
		api_callbacks.api_pump_on = callbacks->api_pump_on;
		api_callbacks.api_pump_off = callbacks->api_pump_off;
	}

	return radio_init();
}
