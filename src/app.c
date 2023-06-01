#include "app.h"


#include <zephyr/logging/log.h>

LOG_MODULE_DECLARE(app, LOG_LEVEL_DBG);

static struct hardware_api hardware;

static uint16_t impuls_count_all = 0;
static uint16_t impuls_count = 0;
static volatile uint16_t impuls_count_readed = 0;

static bool is_running = false;

static bool is_blinking = false;
static const uint16_t blinking_period = 10;
static uint16_t blinking_value = 0;
static bool blinking_on = false;


void app_set_count(const uint16_t count) {
    impuls_count = count;
}

uint16_t app_get_count() {
    return impuls_count_readed;
}

void app_run() {
    if (impuls_count != 0) {
        // error
        LOG_ERR("error");
    }

    impuls_count_all += impuls_count;

    impuls_count_readed = 0;

    // run method
    is_running = true;
    hardware.pump_on();
    hardware.led_red_on();
}

//todo initalize in hardware
void app_one_impuls_callback() {
    // shardware made one impuls
    impuls_count_readed ++;
}

uint16_t app_get_all_count() {
    return impuls_count_all;
}

void app_identify_on() {
    // blinking on
    is_blinking = true;
    hardware.led_yellow_on();
}

void app_identify_off() {
    // stop blinking
    is_blinking = false;
    hardware.led_yellow_off();
}


void app_main_loop_function() {
    if(is_running) {
        if(impuls_count_readed >= impuls_count) {
            is_running = false;
            hardware.pump_off();
            hardware.led_red_off();
        }
    }
    if(is_blinking) {
        if(blinking_value >= blinking_period) {
            if(blinking_on) {
                hardware.led_yellow_on();
                blinking_on = false;
            }
            else {
                hardware.led_yellow_off();
                blinking_on = true;
            }
            blinking_value = 0;
        } else {
            blinking_value++;
        }
    }
}


int app_api_init(struct hardware_api * api) {
    if(api) {
        hardware.led_red_off = api->led_red_off;
        hardware.led_red_on = api->led_red_on;
        hardware.led_yellow_off = api->led_yellow_off;
        hardware.led_yellow_on = api->led_yellow_on;
        hardware.pump_off = api->pump_off;
        hardware.pump_on = api->pump_on;

        return 0;
    }

    return 1;   
}