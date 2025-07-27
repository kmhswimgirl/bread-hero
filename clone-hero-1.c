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

int keys[] = {GREEN_KEY, RED_KEY, YELLOW_KEY, BLUE_KEY, ORANGE_KEY};
int strum[] = {S_UP};
int function[] = {STARPOWER};

void init_fretboard(void){
  for(int i=0; i<5; i++){
    gpio_init(keys[i]);
    gpio_set_dir(keys[i], GPIO_IN);
    gpio_pull_up(keys[i]);
  }
}

void init_strumbar(void){
  for(int i=0; i<1; i++){
    gpio_init(strum[i]);
    gpio_set_dir(strum[i], GPIO_IN);
    gpio_pull_up(strum[i]);
  }
}

void init_function_keys(void){
  for(int i=0; i<1; i++){
    gpio_init(function[i]);
    gpio_set_dir(function[i], GPIO_IN);
    gpio_pull_up(function[i]);
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

void update_led_states(void){

}

int main() {
  board_init();
  tusb_init();
  init_fretboard();
  init_strumbar();
  init_function_keys();

  while (true) {
    tud_task(); // TinyUSB device task

    // get state
    bool green_pressed = !gpio_get(keys[0]);
    bool red_pressed = !gpio_get(keys[1]);
    bool yellow_pressed = !gpio_get(keys[2]);
    bool blue_pressed = !gpio_get(keys[3]);
    bool orange_pressed = !gpio_get(keys[4]);

    bool up_pressed = !gpio_get(strum[0]);

    bool starpower_pressed = !gpio_get(function[0]);

    if(tud_hid_ready()){ //make sure mc is ready 
      
      // gamepad report instead of keyboard one
      hid_gamepad_report_t gamepad_report = {
        .x   = 0,   .y   = 0,   .z   = 0,    //analog (whammy bar...)
        .rz  = 0,   .rx  = 0,   .ry  = 0,    //
        .hat = 0,                            //
        .buttons = 0                         // buttons
      };
      
      // convert button pressed bool to gamepad (bit positions)
      if (green_pressed)    gamepad_report.buttons |= (1 << 0);
      if (red_pressed)      gamepad_report.buttons |= (1 << 1);
      if (yellow_pressed)   gamepad_report.buttons |= (1 << 2);
      if (blue_pressed)     gamepad_report.buttons |= (1 << 3);
      if (orange_pressed)   gamepad_report.buttons |= (1 << 4);

      if (up_pressed)       gamepad_report.buttons |= (1 << 5);
      if (starpower_pressed) gamepad_report.buttons |= (1 << 6); 
      
      // gamepad report
      tud_hid_gamepad_report(0, gamepad_report.x, gamepad_report.y, gamepad_report.z, 
                            gamepad_report.rz, gamepad_report.rx, gamepad_report.ry, 
                            gamepad_report.hat, gamepad_report.buttons);
    }

    green_pressed_last = green_pressed;
    red_pressed_last = red_pressed;
    yellow_pressed_last = yellow_pressed;
    blue_pressed_last = blue_pressed;
    orange_pressed_last = orange_pressed;
    up_pressed_last = up_pressed;
    starpower_pressed_last = starpower_pressed;

    sleep_ms(1); // debouncing
  }
}