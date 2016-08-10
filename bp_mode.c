// alter-evic

#include "main.h"
#include "anim.h"
#include "bp_mode.h"
#include <M451Series.h>
#include <Display.h>
#include <Font.h>
#include <Button.h>
#include <Atomizer.h>
#include <TimerUtils.h>
#include <Battery.h>


// Globals
extern uint32_t bp_wattage, wattage, mode, idle_timer;
extern Atomizer_Info_t atomizer;


void bp_mode() {
	uint32_t atomizer_voltage, battery_load;
	double amperage, f_wattage;
	uint8_t button_state, firing;
	mode = 2;
	Timer_DelayMs(300);

	atomizer_voltage = Battery_GetVoltage() - 40;

	firing = 0;
	battery_load = atomizer_voltage + 40;
	Atomizer_ReadInfo(&atomizer);
	while(1) {
		button_state = Button_GetState();
		Atomizer_ReadInfo(&atomizer);

		if(ready_fire(button_state, atomizer.resistance)) {
			fire_volts(atomizer_voltage);
			firing = 1;
		} else if(!ready_fire(button_state, atomizer.resistance)) {
			Atomizer_Control(0);
			firing = 0;
		}

		if(firing) {
			battery_load = Battery_GetVoltage();
			atomizer_voltage = battery_load - 40;
			//anim_matrix();
			anim_lines(lines_watts(), battery_load);
		} else {
			if(button_state && (button_state & BUTTON_MASK_LEFT) && (button_state & BUTTON_MASK_RIGHT)) {
				return;
			} else if((button_state & BUTTON_MASK_LEFT) || (button_state & BUTTON_MASK_RIGHT)) {
				idle_timer = 0;
			}
			// use atomizer constansts!!!
			amperage = (double)atomizer_voltage / atomizer.baseResistance;
			f_wattage = (amperage * (atomizer_voltage / 10));
			bp_wattage = (uint32_t)f_wattage;
			if(idle_timer < 5) {
				anim_home(atomizer_voltage, battery_load);
			} else {
				anim_png();
			}
		}
	}
}
