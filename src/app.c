#include "app.h"


#include <zephyr/logging/log.h>

LOG_MODULE_DECLARE(app, LOG_LEVEL_DBG);

static struct hardware_api hardware;

static uint16_t impuls_count = 0;
static volatile uint16_t impuls_count_readed = 0;

static bool is_running = false;

static bool is_blinking = false;
static const uint16_t blinking_period = 10;
static uint16_t blinking_value = 0;
static bool blinking_on = false;


void app_set_count(const uint16_t count) {
    LOG_INF("setting impuls count: %d", impuls_count);
    impuls_count = count;
}

uint16_t app_get_count() {
    LOG_INF("reading impuls count: %d", impuls_count);
    return impuls_count_readed;
}

void app_run() {
    if (impuls_count == 0) {
        // error
        LOG_ERR("error - impuls_count == 0");
        return;
    }

    impuls_count_readed = 0;

    // run method

    LOG_INF("run method started, impuls: %d", impuls_count);

    is_running = true;
    hardware.pump_on();
    hardware.led_red_on();
}

void app_run_stop() {
    LOG_INF("run method stopped!");
    impuls_count = 0;
    is_running = false;
    hardware.pump_off();
    hardware.led_red_off();
}

//todo initalize in hardware
void app_one_impuls_callback() {
    // shardware made one impuls
    impuls_count_readed ++;
}

void app_identify_on() {
    // blinking on
    LOG_INF("identyfy start!");
    is_blinking = true;
    hardware.led_yellow_on();
}

void app_identify_off() {
    // stop blinking
    LOG_INF("identyfy stopped!");
    is_blinking = false;
    hardware.led_yellow_off();
}

void app_pump_on() {
    LOG_INF("pump on!");
    hardware.pump_on();
    hardware.led_red_on();
}

void app_pump_off() {
    LOG_INF("pump off!");
    hardware.pump_off();
    hardware.led_red_off();
}


void app_main_loop_function() {
    if(is_running) {
        if(impuls_count_readed >= impuls_count) {
            is_running = false;
            hardware.pump_off();
            hardware.led_red_off();

            LOG_INF("run method finished!");
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