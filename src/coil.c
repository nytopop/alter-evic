// coil.c

#include "main.h"
#include "coil.h"

extern volatile Context ctx;

float readCoilTemp() {
	float result;

	result = ctx.atomizer.resistance - ctx.coil.baseRes;
	result = result / (ctx.coil.baseRes * ctx.settings.tcrValue);
	result += 20;

	return result;
}

// returns wattage from PID controller for target temp
int wattsPID() {
	static float oldTemp, iTerm;
	float deltaError, error, output;
	int min, max;
	int kp, ki, kd;

	// constants
	min = 1;
	max = ctx.settings.maxWatts;
	kp = 64;
	ki = 16;
	kd = 1;

	// P term
	error = ctx.settings.tT - ctx.coil.temp;

	// I term
	iTerm += ki * error;
	// integrator windup eliminator
	if(iTerm > max) {
		iTerm = max;
	} else if(iTerm < min) {
		iTerm = min;
	}

	// D term
	deltaError = ctx.coil.temp - oldTemp;

	//           P            I           D
	output = (kp * error) + iTerm - (kd * deltaError);

	// clamp output to min/max
	if(output > max) {
		output = max;
	} else if(output < min) {
		output = min;
	}

	oldTemp = ctx.coil.temp;

	return (int)output;
}

// actually fire the coil at current settings, use checkFire()
void fire() {
	if (checkFire()) {
		Atomizer_Control(1);
	} else {
		Atomizer_Control(0);
	}
}

// check if safe to fire
// TODO
int checkFire() {
	// check if atomizer reports no errors
	// check if v isn't too high for regulator
	// check if w isn't over limit

	// check if firetimer isn't too high TODO : this needs a reset switch
	if(ctx.state.fireTimer > 10)
		return 0;
	// modulate power if battery is low, map so it ramps smoothly
	return 1;
}
