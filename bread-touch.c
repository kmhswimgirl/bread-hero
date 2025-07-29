#include "hardware/gpio.h"
#include "hardware/timer.h"
#include "pico/stdlib.h"

#define TOUCH_THRESHOLD 100  // test and adjust

uint32_t read_touch_raw(uint8_t pin) { //returns raw touch value / timing

    gpio_set_dir(pin, GPIO_OUT); // set pin is out, drive low
    gpio_put(pin, 0);
    sleep_ms(1);

    gpio_set_dir(pin, GPIO_IN);
    gpio_pull_up(pin); // Enable internal pull-up

    uint32_t count = 0;
    while (!gpio_get(pin) && count < 10000) {
        count++;
    }
    return count;
}

bool is_touched(uint8_t pin) { //logic for if it has been touched
    uint32_t touch_value = read_touch_raw(pin);
    return touch_value < TOUCH_THRESHOLD;
}