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

int bread_frets[] = {GREEN_BREAD, RED_BREAD};

void init_bread_test(void){
  int i;
  for (i=0; i<2; i++){
    gpio_init(bread_frets[i]);
  }    

  gpio_init(17);
  gpio_set_dir(17, GPIO_OUT);
  gpio_put(17, 0);  
}

// TEST MAIN LOOP FOR DEBUG
int main() {
  stdio_init_all();
  init_bread_test();

  while (true) {
    uint32_t raw_value = read_touch_raw(bread_frets[0]);
    
    if (raw_value == 0) {
        // if pin is disconnected somehow
        gpio_put(17, 1);
        sleep_ms(50);
        gpio_put(17, 0);
        sleep_ms(50);
    } else if (raw_value >= 10000) {
        // if tapped to gnd
        gpio_put(17, 1);
        sleep_ms(200);
    } else if (raw_value > 10) {
        // if touched
        gpio_put(17, 1);
        sleep_ms(500);
        gpio_put(17, 0);
        sleep_ms(500);
    } else {
        // if not touched
        gpio_put(17, 1);
        sleep_ms(100);
        gpio_put(17, 0);
        sleep_ms(100);
        gpio_put(17, 1);
        sleep_ms(100);
        gpio_put(17, 0);
        sleep_ms(300);
    }
  }
}