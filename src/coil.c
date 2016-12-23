// coil.c

#include "main.h"
#include "coil.h"
#include <math.h>

extern volatile Context ctx;

// Set coil firing settings
void doFireSet() {
	int outV;
	int outW;
	switch(ctx.settings.mode) {
		case 0: // settings
			outV = 0;
			Atomizer_SetOutputVoltage(outV);
			break;
		case 1: // vw
			outW = ctx.settings.tW;
			if(outW > ctx.settings.maxWatts)
				outW = ctx.settings.maxWatts;
			outV = (int)sqrt(outW * ctx.atomizer.resistance);
			Atomizer_SetOutputVoltage(outV);
			break;
		case 2: // bp
			outV = ctx.battery.volts;
			Atomizer_SetOutputVoltage(outV);
			break;
		case 3: // tc
			// TODO : PID control loop
			outV = 0;
			Atomizer_SetOutputVoltage(outV);
			break;
	}
}

// check if safe to fire
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

int wattsToVolts(int watts) {
	return watts;
}

float readCoilTemp() {
	float result;

	result = ctx.atomizer.resistance - ctx.coil.baseRes;
	result = result / (ctx.coil.baseRes * ctx.settings.tcrValue);
	result += 20;

	return result;
}

// returns voltage from PID controller for target temp
int wattsPID(int temp) {
	return temp;
}

// actually fire the coil at current settings, use checkFire()
void fire() {
	if (checkFire()) {
		Atomizer_Control(1);
	} else {
		Atomizer_Control(0);
	}
}
