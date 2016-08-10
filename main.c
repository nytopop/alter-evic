// alter-evic

#include "main.h"
#include "anim.h"
#include "vv_mode.h"
#include "bp_mode.h"
#include "tc_mode.h"
#include <math.h>
#include <M451Series.h>
#include <Display.h>
#include <Font.h>
#include <Button.h>
#include <Atomizer.h>
#include <TimerUtils.h>
#include <Battery.h>


// Globals
double tcr, coil_temp;
uint32_t bp_wattage, wattage, boost_time, boost_wattage, boost, tc_temp, mode, idle_timer;

Atomizer_Info_t atomizer;


int main() {
	int16_t timer;
	// boost code disabled until settings is ready
	boost_wattage = 8000;
	boost_time = 0;
	boost = 1;
	wattage = 4600;

	tcr = 0.0045;
	tc_temp = 140;
	
	timer = Timer_CreateTimer(1, 1, increment_idle, 1);

	while(1){
		vv_mode();
		bp_mode();
		tc_mode();
	}
	return 0;
}

void increment_idle(uint8_t arg) {
	if(idle_timer > 300) {
		//do sleep
	} else {
		idle_timer++;
	}
	return;
}


// return mV from mW
uint32_t watts(uint32_t watts, uint32_t res) {
  // V = sqrt(P * R)
  uint32_t volts = (uint32_t)(double)(sqrt(watts * res) + 5) / 10;
  return volts * 10;
}


// Set output voltage and fire, if not already on
void fire_volts(uint32_t volts) {
	Atomizer_SetOutputVoltage(volts);
	if(!Atomizer_IsOn()) {
		Atomizer_Control(1);
	}
}


// Check if fire button pressed, healthy resistance, and no error
uint8_t ready_fire(uint8_t button_state, uint32_t resistance) {
	if((button_state & BUTTON_MASK_FIRE) && (resistance != 0) && (Atomizer_GetError() == OK)) {
		return 1;
	} else {
		return 0;
	}
}
