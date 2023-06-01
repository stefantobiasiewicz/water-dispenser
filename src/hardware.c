#include "hardware.h"

#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/pwm.h>


#include <zephyr/logging/log.h>

LOG_MODULE_DECLARE(hardware, LOG_LEVEL_DBG);

struct hardware_cb hardware_callbacks ;

struct gpio_dt_spec led_y = GPIO_DT_SPEC_GET(DT_NODELABEL(led0), gpios);
struct gpio_dt_spec led_r = GPIO_DT_SPEC_GET(DT_NODELABEL(led1), gpios);

struct gpio_dt_spec flow_input = GPIO_DT_SPEC_GET(DT_NODELABEL(flow_meter), gpios);
static struct gpio_callback flow_input_cb_data;

void pin_isr(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
    if(hardware_callbacks.app_impuls_callback) {
        hardware_callbacks.app_impuls_callback();
    }
}

const struct pwm_dt_spec pump_pwm = PWM_DT_SPEC_GET(DT_NODELABEL(pump));



static int init_hardware() {
    int err = 0;
    
    /**
     * Checks all devices
    */
    if (!device_is_ready(led_y.port) || !device_is_ready(led_r.port) || !device_is_ready(flow_input.port) || !device_is_ready(pump_pwm.dev)) {
        return err;
    }

    /**
     * initailize outputs for leds
    */
    err = gpio_pin_configure_dt(&led_y, GPIO_OUTPUT_ACTIVE);
    if (err < 0) {
        return err;
    }

    err = gpio_pin_configure_dt(&led_r, GPIO_OUTPUT_ACTIVE);
    if (err < 0) {
        return err;
    }

    /**
     * initailize input
    */
    err = gpio_pin_configure_dt(&flow_input, GPIO_INPUT);
    if (err < 0) {
        return err;
    }
    err = gpio_pin_interrupt_configure_dt(&flow_input,GPIO_INT_EDGE_TO_ACTIVE);
    if (err < 0) {
        return err;
    }
    gpio_init_callback(&flow_input_cb_data, pin_isr, BIT(flow_input.pin));
    gpio_add_callback(flow_input.port, &flow_input_cb_data);

    /**
     * initailize pwm output
    */
    pwm_set_dt(&pump_pwm, 1000000, 0); 

    /**
     * set default values
    */
    hadware_api_led_red_on();
    hadware_api_led_yellow_on();

    k_msleep(10);

    hadware_api_led_red_off();
    hadware_api_led_yellow_off();
    hadware_api_pump_off();

    return err;
}



int hadware_api_init(struct hardware_cb *callbacks) {
    if(callbacks) {
        hardware_callbacks.app_impuls_callback = callbacks->app_impuls_callback;
    }

    return init_hardware();
}

void hadware_api_pump_on() {
    pwm_set_dt(&pump_pwm, 1000000, 1000000); 
}

void hadware_api_pump_off() {
    pwm_set_dt(&pump_pwm, 1000000, 0); 
}

void hadware_api_led_red_on() {
    gpio_pin_set_dt(&led_r, 1);
}

void hadware_api_led_red_off() {
    gpio_pin_set_dt(&led_r, 0);
}

void hadware_api_led_yellow_on() {
    gpio_pin_set_dt(&led_y, 1);
}

void hadware_api_led_yellow_off() {
    gpio_pin_set_dt(&led_y, 0);
}


void hardware_main_loop() {
   // gpio_pin_toggle_dt(&led_r);
}