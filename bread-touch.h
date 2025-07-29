#ifndef BREAD_TOUCH_H
#define BREAD_TOUCH_H

#include <stdint.h>
#include <stdbool.h>

#define TOUCH_THRESHOLD 1000

uint32_t read_touch_raw(uint8_t pin);
bool is_touched(uint8_t pin);

#endif