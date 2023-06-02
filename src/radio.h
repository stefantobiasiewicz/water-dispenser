#ifndef RADIO_HH
#define RADIO_HH

#include <zephyr/types.h>
#include <stdbool.h>

// #define BT_UUID_LBS_VAL         BT_UUID_128_ENCODE(0x00001523, 0x1212, 0xefde, 0x1523, 0x785feabcd123)
// /** @brief Button Characteristic UUID. */
// #define BT_UUID_LBS_BUTTON_VAL  BT_UUID_128_ENCODE(0x00001524, 0x1212, 0xefde, 0x1523, 0x785feabcd123)
// /** @brief LED Characteristic UUID. */
// #define BT_UUID_LBS_LED_VAL     BT_UUID_128_ENCODE(0x00001525, 0x1212, 0xefde, 0x1523, 0x785feabcd123)

// #define BT_UUID_LBS           BT_UUID_DECLARE_128(BT_UUID_LBS_VAL)
// #define BT_UUID_LBS_BUTTON    BT_UUID_DECLARE_128(BT_UUID_LBS_BUTTON_VAL)
// #define BT_UUID_LBS_LED       BT_UUID_DECLARE_128(BT_UUID_LBS_LED_VAL)


#define BT_UUID_WATER_DISPENSER_ENCODED     BT_UUID_128_ENCODE(0x12345678, 0x1234, 0x5678, 0x1234, 0x56789abcdef1)
#define BT_UUID_WATER_DISPENSER     BT_UUID_DECLARE_128(BT_UUID_WATER_DISPENSER_ENCODED)

#define BT_UUID_IMPUSET             BT_UUID_DECLARE_128(BT_UUID_128_ENCODE(0x12345678, 0x1234, 0x5678, 0x1234, 0x56789abcdef2))
#define BT_UUID_RUN                 BT_UUID_DECLARE_128(BT_UUID_128_ENCODE(0x12345678, 0x1234, 0x5678, 0x1234, 0x56789abcdef3))
#define BT_UUID_IDENTIFY            BT_UUID_DECLARE_128(BT_UUID_128_ENCODE(0x12345678, 0x1234, 0x5678, 0x1234, 0x56789abcdef4))
#define BT_UUID_ON_OFF              BT_UUID_DECLARE_128(BT_UUID_128_ENCODE(0x12345678, 0x1234, 0x5678, 0x1234, 0x56789abcdef5))



// /** @brief Callback type for when an LED state change is received. */
// typedef void (*led_cb_t)(const bool led_state);

// /** @brief Callback type for when the button state is pulled. */
// typedef bool (*button_cb_t)(void);

// /** @brief Callback struct used by the LBS Service. */
// struct my_lbs_cb {
// 	/** LED state change callback. */
// 	led_cb_t    led_cb;
// 	/** Button read callback. */
// 	button_cb_t button_cb;
// };


// int my_lbs_init(struct my_lbs_cb *callbacks);



typedef void (*api_set_count_t)(const uint16_t count);
typedef uint16_t (*api_get_count_t)();
typedef void (*api_run_t)();
typedef void (*api_run_stop_t)();
typedef void (*api_identify_on_t)();
typedef void (*api_identify_off_t)();
typedef void (*api_pump_on_t)();
typedef void (*api_pump_off_t)();

struct api_water_dispenser {
	api_set_count_t api_set_count;
    api_get_count_t api_get_count;
    api_run_t api_run;
    api_run_stop_t api_run_stop;
    api_identify_on_t api_identify_on;
    api_identify_off_t api_identify_off;
    api_pump_on_t api_pump_on;
    api_pump_off_t api_pump_off;
};

int radio_api_init(struct api_water_dispenser *callbacks);

#endif