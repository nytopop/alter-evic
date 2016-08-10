// alter-evic

#include "main.h"
#include "anim.h"
#include <math.h>
#include "tc_mode.h"
#include <M451Series.h>
#include <Atomizer.h>
#include <Battery.h>
#include <Button.h>
#include <TimerUtils.h>


// Globals
extern double tcr, coil_temp;
extern uint32_t wattage, tc_temp, mode, idle_timer;
extern Atomizer_Info_t atomizer;


void tc_mode() {
	uint8_t button_state, firing;
	uint32_t atomizer_voltage, battery_load;
	mode = 3;
	Timer_DelayMs(300);
	
	firing = 0;

	atomizer_voltage = 100;
	Atomizer_ReadInfo(&atomizer);

	while(1) {
		Atomizer_ReadInfo(&atomizer);
		button_state = Button_GetState();

		if(firing) {
			//Atomizer_ReadInfo(&atomizer);
			battery_load = Battery_GetVoltage();
			coil_temp = read_coil_temp();
			atomizer_voltage = pid_alg();
			anim_lines(lines_volts(), battery_load);
		} else {
			// handle mode and temp control
			if(button_state) {
				if((button_state & BUTTON_MASK_LEFT) && (button_state & BUTTON_MASK_RIGHT)){
					return;
				} else if(button_state & BUTTON_MASK_LEFT) {
					if(tc_temp > 20){
						idle_timer = 0;
						tc_temp--;
					}
				} else if(button_state & BUTTON_MASK_RIGHT){
					if(tc_temp < 300) {
						idle_timer = 0;
						tc_temp++;
					}
				}
			}
			if(idle_timer < 5) {
				anim_home(atomizer_voltage, battery_load);
			} else {
				anim_png();
			}
		}
		
		// Fire control
		if(ready_fire(button_state, atomizer.resistance)) {
			fire_volts(atomizer_voltage);
			firing = 1;
		} else if(!(button_state & BUTTON_MASK_FIRE)) {
			Atomizer_Control(0);
			firing = 0;
		}
	}
}


// get temp of coil
double read_coil_temp() {
	double result, baseres;
	// Using constant 0.314 resistance for the Krixus tank, need to re-hardcode for other coils
	//result = (double)atomizer.resistance - atomizer.baseResistance;
	//result = result / ((double)atomizer.baseResistance * tcr); // could this be a divide by zero case? manually allocate baseresistance?
	baseres = 300;
	result = (double)atomizer.resistance - baseres;
	result = result / ((double)baseres * tcr);
	result += 20;
	return result;
}


// revised pid algorithm
// Modify the algorithm so that it regulates voltage rather than wattage?
uint32_t pid_alg() {
	static double old_temp, iterm;
	double delta_error, error, output;
	int32_t min, max;
	int32_t kp, ki, kd;

	min = 1;
	//max = 9000;
	max = (uint32_t) sqrt(8000 * (atomizer.resistance * 10));
	kp = 64;
	ki = 12;
	kd = 16;

	// P term
	error = tc_temp - coil_temp;

	// I term
	iterm += (ki * error);
	if(iterm > max) {     // eliminate integrator windup
		iterm = max;
	} else if(iterm < min) {
		iterm = min;
	}

	// D term
	delta_error = coil_temp - old_temp;

	// PID term
	output = (kp * error) + (iterm) - (kd * delta_error);
	if(output > max) {    // limit maximum voltage to calculated maximum
		output = max;
	} else if(output < min) {
		output = min;
	}

	old_temp = coil_temp;
	return (uint32_t)output;
}
