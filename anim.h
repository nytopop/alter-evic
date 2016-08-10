#ifndef ANIM_H
#define ANIM_H

void anim_png();
uint32_t lines_watts();
uint32_t lines_volts();
void anim_lines(uint32_t pixels, uint32_t battery_load);
void anim_matrix();
void anim_home(uint32_t volts, uint32_t battery_load);

#endif
