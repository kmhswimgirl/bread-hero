#include <stdio.h>
#include <string.h> 

#include "pico/stdlib.h"
#include "bsp/board.h"
#include "tusb.h"
#include "class/hid/hid_device.h"
#include "class/hid/hid.h"
#include "device/usbd.h"   
#include "tusb_config.h"
#include "controller_gpio.h"
#include "bread-touch.h"

void init_bread_test(void){
  gpio_init(BREAD_TEST);
  gpio_set_dir(BREAD_TEST, GPIO_OUT);  // Start as output for discharge
  gpio_put(BREAD_TEST, 0);             // Initial discharge
  
  gpio_init(LED);
  gpio_set_dir(LED, GPIO_OUT);
  gpio_put(LED, 0);               // LED off initially
}

uint32_t simple_touch_test(uint8_t pin) {
  // Discharge
  gpio_set_dir(pin, GPIO_OUT);
  gpio_put(pin, 0);
  sleep_ms(1);
  
  // Switch to input
  gpio_set_dir(pin, GPIO_IN);
  gpio_disable_pulls(pin);
  
  // Count how long until pin goes high
  uint32_t count = 0;
  while (!gpio_get(pin) && count < 10000) {
      count++;
  }
  
  return count;
}

int main() {
  board_init();
  init_bread_test();

  while (true) {
    uint32_t touch_value = read_touch_raw(BREAD_TEST);
    bool touch_pressed = touch_value < 2000; 

    // Debug: Show touch_value with LED blinks
    // Blink LED based on touch_value range
    if (touch_value < 10) {
      gpio_put(LED, 0);
    } else {
      gpio_put(LED, 1);
    }
  }
}