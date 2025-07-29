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

// Required TinyUSB HID callbacks

int bread_frets[] = {GREEN_BREAD, RED_BREAD, YELLOW_BREAD};

void init_bread_test(void){
  int i;
  for (i=0; i<4; i++){
    gpio_init(bread_frets[i]);
    gpio_set_dir(bread_frets[i], GPIO_OUT);  // Start as output for discharge
    gpio_put(bread_frets[i], 0);
  } 
    gpio_init(STARPOWER_BREAD);
    gpio_set_dir(STARPOWER_BREAD, GPIO_OUT);  // Start as output for discharge
    gpio_put(STARPOWER_BREAD, 0);      
}

int main() {
  board_init();
  tusb_init();
  init_bread_test();

  bool green_pressed_last = false;
  bool red_pressed_last = false;
  bool yellow_pressed_last = false;

  bool starpower_pressed_last = false;

  while (true) {
    tud_task(); // TinyUSB device task

    // Read your touch sensors
    uint32_t green_bread_value = read_touch_raw(bread_frets[0]);
    bool green_pressed = green_bread_value < 2000; 

    uint32_t red_bread_value = read_touch_raw(bread_frets[1]);
    bool red_pressed = red_bread_value < 2000; 

    uint32_t yellow_bread_value = read_touch_raw(bread_frets[2]);
    bool yellow_pressed = yellow_bread_value < 2000;

    uint32_t starpower_bread_value = read_touch_raw(STARPOWER_BREAD);
    bool starpower_pressed = starpower_bread_value < 2000;

    if (tud_hid_ready()) {
        // Standard gamepad report format for Clone Hero
        // Map to standard gamepad buttons (A, B, X, Y, etc.)
        uint16_t buttons = 0;
        
        // Map your frets to standard gamepad buttons
        if (green_pressed)    buttons |= (1 << 4);  // Left Shoulder
        if (red_pressed)      buttons |= (1 << 5);  // Right Shoulder  
        if (yellow_pressed)   buttons |= (1 << 0);  // A button
        if (starpower_pressed) buttons |= (1 << 6); // Back button

        // Send standard gamepad report
        tud_hid_gamepad_report(0, 0, 0, 0, 0, 0, 0, 0, buttons);
    }

    sleep_ms(1);
  }
}