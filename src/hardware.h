#ifndef HARDWARE_HH
#define HARDWARE_HH

#include <stdbool.h>

/* API */

void hadware_api_pump_on();
void hadware_api_pump_off();
void hadware_api_led_red_on();
void hadware_api_led_red_off();
void hadware_api_led_yellow_on();
void hadware_api_led_yellow_off();


/**
 * function run in main loop every 10 ms
*/
void hardware_main_loop(void);


typedef void (*api_impuls_callback_t)();

struct hardware_cb {
	api_impuls_callback_t    app_impuls_callback;
};


int hadware_api_init(struct hardware_cb *callbacks);

#endif