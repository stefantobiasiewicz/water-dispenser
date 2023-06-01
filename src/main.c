#include <zephyr/kernel.h>

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/gap.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/addr.h>

#include <zephyr/bluetooth/conn.h>

#include <zephyr/logging/log.h>
#include <zephyr/types.h>

#include "hardware.h"
#include "radio.h"
#include "app.h"

LOG_MODULE_REGISTER(main, LOG_LEVEL_DBG);



static struct hardware_cb hadware_callback = {
    .app_impuls_callback = app_one_impuls_callback
};

static struct api_water_dispenser radio_api = {
    .api_set_count = app_set_count,
    .api_get_count = app_get_count,
    .api_run = app_run,
    .api_run_stop = app_run_stop,
    .api_identify_on = app_identify_on,
    .api_identify_off = app_identify_off,
    .api_pump_on = app_pump_on,
    .api_pump_off = app_pump_off
};

static struct hardware_api hardware_api_impl = {
    .led_red_off = hadware_api_led_red_off,
    .led_red_on = hadware_api_led_red_on,
    .led_yellow_off = hadware_api_led_yellow_off,
    .led_yellow_on = hadware_api_led_yellow_on,
    .pump_off = hadware_api_pump_off,
    .pump_on = hadware_api_pump_on
};


void main(void)
{
    int err;
    LOG_INF("ST water dispenser app starts");

    
    err = hadware_api_init(&hadware_callback);
    if (err) {
        LOG_ERR("Hardware init failed (err %d)\n", err);
        return;
    }
    LOG_INF("hardware initalized");

    err = radio_api_init(&radio_api); 
    if (err) {
        LOG_ERR("Bluetooth init failed (err %d)\n", err);
        return;
    }
    LOG_INF("Bluetooth initialized\n");

    err = app_api_init(&hardware_api_impl); 
    if (err) {
        LOG_ERR("app init failed (err %d)\n", err);
        return;
    }
    LOG_INF("app initialized\n");


    LOG_INF("init finish device started!");
    while (1)
    {
        hardware_main_loop();
        app_main_loop_function();

        k_msleep(10);
    }
}


