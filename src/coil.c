// coil.c

#include "main.h"
#include "coil.h"
#include <Atomizer.h>

extern volatile Context ctx;

float readCoilTemp() {
	float result;

	result = ctx.atomizer.resistance - ctx.coil.baseRes;
	result = result / (ctx.coil.baseRes * ctx.settings.tcrValue);
	result += 20;

	return result;
}

int newPID() {
	float kp, ki, kd;
	float p, i, d;
	float err, dt, out;
	int min, max, outV;
	
	min = ATOMIZER_VOLTAGE_MIN;
	max = ATOMIZER_VOLTAGE_MAX;
	//max = ctx.settings.maxWatts / 100;
	//min = 0;
	//max = 750;
	kp = 32;
	ki = 0.4;
	kd = 24;

	// Proportional
	err = ctx.settings.tT - ctx.coil.temp;

	// Integral and Windup Eliminator
	ctx.coil.iTerm += err;
	if((int)ctx.coil.iTerm > max)
		ctx.coil.iTerm = max;
	if ((int)ctx.coil.iTerm < min)
		ctx.coil.iTerm = min;

	// Derivative and timing
	ctx.coil.dTerm = err - ctx.coil.pTerm;
	ctx.coil.pTerm = err;
	dt = 100 / FPS;

	// PID
	p = kp * err;
	i = ki * ctx.coil.iTerm * dt;
	d = kd * ctx.coil.dTerm / dt;
	ctx.coil.p = p;
	ctx.coil.i = i;
	ctx.coil.d = d;

	out = p + i + d;
	ctx.coil.out = out;
	if((int)out > max)
		out = max;
	if((int)out < min)
		out = min;

	outV = (int)out;
	return outV;
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
