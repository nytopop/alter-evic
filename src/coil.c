// coil.c

#include "main.h"
#include "coil.h"
#include <Atomizer.h>
#include <math.h>

extern volatile Context ctx;

float readCoilTemp() {
	float result;

	result = ctx.atomizer.resistance - ctx.coil.baseRes;
	result = result / (ctx.coil.baseRes * ctx.settings.tcrValue);
	result += 20;

	return result;
}

int voltsPID() {
	//float kp, ki, kd;
	float p, i, d;
	float err, out;
	int min, max, outV;

	// compute min and max from maxwatts
	// V = sqrt(p*r)
	max = (int)sqrt(ctx.settings.maxWatts * ctx.atomizer.resistance);
	min = ATOMIZER_VOLTAGE_MIN;
	//max = ATOMIZER_VOLTAGE_MAX;
	
	// Gain constants
	const float kp = 64;
	const float ki = 0.4;
	const float kd = 32;
	const float dt = 100 / FPS;

	// Proportional term
	err = ctx.settings.tT - ctx.coil.temp;

	// Integral term and Windup Eliminator
	ctx.coil.iTerm += err;
	if((int)ctx.coil.iTerm > max)
		ctx.coil.iTerm = max;
	if ((int)ctx.coil.iTerm < min)
		ctx.coil.iTerm = min;

	// Derivative term
	ctx.coil.dTerm = err - ctx.coil.pTerm;
	ctx.coil.pTerm = err;
	
	// Calculate as [p = kp * (setTemp - currentTemp)]
	p = kp * err;
	// Calculate as [i = ki * (∑ error) * interval]
	i = ki * ctx.coil.iTerm * dt;
	// Calculate as [d = kd * (currentError - lastError) / interval]
	d = kd * ctx.coil.dTerm / dt;

	// Output term and clamping
	out = p + i + d;
	if((int)out > max)
		out = max;
	if((int)out < min)
		out = min;

	// Dry fire protect, if temp is 8 deg or more over setpoint, disable output
	if (err < -8)
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
