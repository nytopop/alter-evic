#include <M451Series.h>

#ifndef MAIN_H
#define MAIN_H

void increment_idle(uint8_t arg);
uint32_t watts(uint32_t watts, uint32_t res);
void fire_volts(uint32_t volts);
uint8_t ready_fire(uint8_t button_state, uint32_t resistance);

#endif
