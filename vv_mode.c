// alter-evic

#include "main.h"
#include "anim.h"
#include "vv_mode.h"
#include <math.h>
#include <M451Series.h>
#include <Button.h>
#include <Atomizer.h>
#include <TimerUtils.h>


// Globals
extern uint32_t wattage, boost_time, boost_wattage, boost, mode, idle_timer;
extern Atomizer_Info_t atomizer;


void vv_mode() {
	uint32_t atomizer_voltage, battery_load;
	uint8_t button_state, firing, timer;
	mode = 1;	

	firing = 0;
	
	Atomizer_ReadInfo(&atomizer);

	Timer_DelayMs(300);
	while(1) {
		button_state = Button_GetState();
		Atomizer_ReadInfo(&atomizer);

		atomizer_voltage = watts(wattage * 10, atomizer.resistance);
		//if(atomizer_voltage > 9000) {
		//	atomizer_voltage = 9000;
		//}

		if(ready_fire(button_state, atomizer.resistance)) {
			fire_volts(atomizer_voltage);
			firing = 1;
		} else if(!ready_fire(button_state, atomizer.resistance)) {
			Atomizer_Control(0);
			firing = 0;
		}

		if(firing) {
			battery_load = Battery_GetVoltage();
			anim_lines(lines_watts(), battery_load);
		} else {
			if(button_state) {
				if((button_state & BUTTON_MASK_LEFT) && (button_state & BUTTON_MASK_RIGHT)){
					return;
				} else if(button_state & BUTTON_MASK_LEFT) {
					if(wattage > 25){
						idle_timer = 0;
						wattage -= 25;
					}
				} else if(button_state & BUTTON_MASK_RIGHT){
					if(wattage < 8000) {
						idle_timer = 0;
						wattage += 25;
					}
				}
			}
			if(idle_timer < 5) {
				anim_home(atomizer_voltage, battery_load);
			} else {
				anim_png();
			}
		}
	}
}
