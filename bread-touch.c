#include "hardware/gpio.h"
#include "hardware/timer.h"

#define TOUCH_THRESHOLD 1000  // test and adjust

uint32_t read_touch_raw(uint8_t pin) { //returns raw touch value / timing

    gpio_set_dir(pin, GPIO_OUT); // set pin is out, drive low
    gpio_put(pin, 0);
    sleep_us(10);  //discharge time
    
    gpio_set_dir(pin, GPIO_IN); //no pull up
    gpio_disable_pulls(pin);
    
    uint32_t start_time = time_us_32(); // time pin to go high
    uint32_t timeout = 5000;  // 5ms timeout
    
    while (!gpio_get(pin) && (time_us_32() - start_time) < timeout) {
        // either pin goes high, or timeout
    }
    
    return time_us_32() - start_time; //return diff
}

bool is_touched(uint8_t pin) { //logic for if it has been touched
    uint32_t touch_value = read_touch_raw(pin);
    return touch_value < TOUCH_THRESHOLD;
}