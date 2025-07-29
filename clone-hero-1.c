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

int bread_frets[] = {GREEN_BREAD, RED_BREAD, YELLOW_BREAD};

void init_bread_test(void){
  int i;
  for (i=0; i<4; i++){
    gpio_init(bread_frets[i]);
    gpio_set_dir(bread_frets[i], GPIO_OUT);  // Start as output for discharge
    gpio_put(bread_frets[i], 0);
  }            
}

int main() {
  board_init();
  init_bread_test();

  while (true) {
    uint32_t green_bread_value = read_touch_raw(bread_frets[0]);
    bool green_pressed = green_bread_value < 2000; 

    uint32_t red_bread_value = read_touch_raw(bread_frets[1]);
    bool red_pressed = red_bread_value < 2000; 

    uint32_t yellow_bread_value = read_touch_raw(bread_frets[2]);
    bool yellow_pressed = yellow_bread_value < 2000;

  
  }
}