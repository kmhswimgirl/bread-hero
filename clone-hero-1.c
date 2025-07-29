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

int bread[] = {GREEN_BREAD, RED_BREAD, YELLOW_BREAD};
int led[] = {GREEN_LED, RED_LED, YELLOW_LED};

void init_bread(void){
  int i;
  for (i=0; i<3; i++){
    gpio_init(bread[i]);
  }   
}

void init_leds(void){
  int i;
  for (i=0;i<3;i++){
    gpio_init(led[i]);
    gpio_set_dir(led[i], GPIO_OUT);
    gpio_put(led[i], 0); 
  }
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

int main() {
  stdio_init_all();
  tusb_init();
  init_bread();
  init_leds();

  while (true) {
    tud_task();
    
    uint32_t green_value = read_touch_raw(bread[0]);
    bool green_pressed = green_value > 10;
    gpio_put(GREEN_LED, green_pressed); //turn on green led

    uint32_t red_value = read_touch_raw(bread[1]);
    bool red_pressed = red_value > 10;
    gpio_put(RED_LED, red_pressed); // turn on red led

    uint32_t yellow_value = read_touch_raw(bread[2]);
    bool yellow_pressed = yellow_value > 10;
    gpio_put(YELLOW_LED, yellow_pressed); // turn on yellow led

    if(tud_hid_ready()){ //make sure mc is ready
     // hid_gamepad_report_t report = {0};
     // gamepad report instead of keyboard one
      hid_gamepad_report_t gamepad_report = {
        .x   = 0,   .y   = 0,   .z   = 0,    //analog (whammy bar...)
        .rz  = 0,   .rx  = 0,   .ry  = 0,    //
        .hat = 0,                            //
        .buttons = 0                         // buttons
      };

      if (green_pressed)    gamepad_report.buttons |= (1 << 0);
      if (red_pressed)      gamepad_report.buttons |= (1 << 1);
      if (yellow_pressed)   gamepad_report.buttons |= (1 << 2);

      // send gamepad report
      tud_hid_gamepad_report(0, gamepad_report.x, gamepad_report.y, gamepad_report.z, 
                            gamepad_report.rz, gamepad_report.rx, gamepad_report.ry, 
                            gamepad_report.hat, gamepad_report.buttons);
    }

    green_pressed_last = green_pressed;
    red_pressed_last = red_pressed;
    yellow_pressed_last = yellow_pressed;
    
    sleep_ms(1); // debouncing

  }
}