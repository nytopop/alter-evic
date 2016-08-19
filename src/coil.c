// coil.c

#include "main.h"
#include "coil.h"

extern volatile Context ctx;

int checkFire() {
	// check if atomizer reports no errors
	// check if v isn't too high for regulator
	// check if w isn't over limit
	// check if firetimer isn't too high
	// modulate power if battery is low, map so it ramps smoothly
	return 0;
}

int wattsToVolts(int watts) {
	return watts;
}

float readCoilTemp() {
	float result;
	result = ctx.atomizer.resistance - ctx.coil.baseRes;
	result = result / (ctx.coil.baseRes * ctx.settings.tcrValue);

	return result;
}

int calcPID(int temp) {
	return temp;
}

// actually fire the coil at current settings, use checkFire()
void fire() {
	switch(ctx.settings.mode) {
		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
		default:
			break;
	}
}
