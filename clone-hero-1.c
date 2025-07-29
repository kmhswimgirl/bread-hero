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

int bread[] = {GREEN_BREAD, RED_BREAD};

void init_bread_test(void){
  int i;
  for (i=0; i<2; i++){
    gpio_init(bread[i]);
  }    

  gpio_init(17);
  gpio_set_dir(17, GPIO_OUT);
  gpio_put(17, 0);  

  gpio_init(18);
  gpio_set_dir(18, GPIO_OUT);
  gpio_put(18, 0); 
}

int touch_pin_test(uint32_t raw_value, int led_pin){ //test function for gpio touch pins
  if (raw_value == 0) {
    // if pin is disconnected somehow
    gpio_put(led_pin, 1);
    sleep_ms(50);
    gpio_put(led_pin, 0);
    sleep_ms(50);
  } else if (raw_value >= 10000) {
    // if tapped to gnd
    gpio_put(led_pin, 1);
    sleep_ms(200);
  } else if (raw_value > 10) {
    // if touched
    gpio_put(led_pin, 1);
    sleep_ms(500);
    gpio_put(led_pin, 0);
    sleep_ms(500);
  } else {
    // if not touched
    gpio_put(led_pin, 1);
    sleep_ms(100);
    gpio_put(led_pin, 0);
    sleep_ms(100);
    gpio_put(led_pin, 1);
    sleep_ms(100);
    gpio_put(led_pin, 0);
    sleep_ms(300);
  }
}

// fret bools
bool green_pressed_last = false;
bool red_pressed_last = false;
bool yellow_pressed_last = false;
bool blue_pressed_last = false;
bool orange_pressed_last = false;

//strumbar bools
bool up_pressed_last = false;

//special key bools
bool starpower_pressed_last = false;

int main() {
  stdio_init_all();
  init_bread_test();

  while (true) {
    // uint32_t raw_value = read_touch_raw(bread[0]); // working threshold was 10...
    // uint32_t raw_val_r = read_touch_raw(bread[1]);
    // uint32_t raw_val_y = read_touch_raw(bread[2]);
    // uint32_t raw_val_star = read_touch_raw(bread[3]);

    // bool green_pressed = raw_value > 10;
    // bool red_pressed = raw_val_r > 10;
    // bool yellow_pressed = raw_val_y > 10;
    // bool star_pressed = raw_val_star > 10;

    // Green LED responds to green touch sensor
    uint32_t green_value = read_touch_raw(bread[0]);
    bool green_pressed = green_value > 10;
    gpio_put(17, green_pressed);
    
    // Red LED responds to red touch sensor
    uint32_t red_value = read_touch_raw(bread[1]);
    bool red_pressed = red_value > 10;
    gpio_put(18, red_pressed);

    // if(tud_hid_ready()){ //make sure mc is ready

    //  // gamepad report instead of keyboard one
    //   hid_gamepad_report_t gamepad_report = {
    //     .x   = 0,   .y   = 0,   .z   = 0,    //analog (whammy bar...)
    //     .rz  = 0,   .rx  = 0,   .ry  = 0,    //
    //     .hat = 0,                            //
    //     .buttons = 0                         // buttons
    //   };

    //   if (green_pressed)    gamepad_report.buttons |= (1 << 0);
    //   if (red_pressed)      gamepad_report.buttons |= (1 << 1);
    //   if (yellow_pressed)   gamepad_report.buttons |= (1 << 2);
    //   if (star_pressed)     gamepad_report.buttons |= (1 << 3);

    //   // send gamepad report
    //   tud_hid_gamepad_report(0, gamepad_report.x, gamepad_report.y, gamepad_report.z, 
    //                         gamepad_report.rz, gamepad_report.rx, gamepad_report.ry, 
    //                         gamepad_report.hat, gamepad_report.buttons);
    // }

    // green_pressed_last = green_pressed;
    // red_pressed_last = red_pressed;
    // yellow_pressed_last = yellow_pressed;
    // starpower_pressed_last = star_pressed;
    
    // sleep_ms(1); // debouncing

  }
}