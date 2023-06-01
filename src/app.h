#ifndef APP_HH
#define APP_HH

#include <zephyr/types.h>
#include "hardware.h"

/* App content code */

/**
 * set count of impuls 
 * read count of impuls
 * 
 * start work
 * 
 * identify on
 * identify off
 * 
 * read all of impuls
*/

/* API */

void app_set_count(const uint16_t count);
uint16_t app_get_count();
void app_run();
void app_identify_on();
void app_identify_off();
uint16_t app_get_all_count();

void app_one_impuls_callback(); 

void app_main_loop_function();


/* Hardware api */
typedef void (*api_pump_on)();
typedef void (*api_pump_off)();
typedef void (*api_led_red_on)();
typedef void (*api_led_red_off)();
typedef void (*api_led_yellow_on)();
typedef void (*api_led_yellow_off)();

struct hardware_api
{
    api_pump_on pump_on;
    api_pump_off pump_off;
    api_led_red_on led_red_on;
    api_led_red_off led_red_off;
    api_led_yellow_on led_yellow_on;
    api_led_yellow_off led_yellow_off;
};

int app_api_init(struct hardware_api * api);

#endif