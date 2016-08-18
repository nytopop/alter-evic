// firing.c

#include "main.h"

void checkFire(struct Context ctx) {
	// check if atomizer reports no errors
	// check if v isn't too high for regulator
	// check if w isn't over limit
	// check if firetimer isn't too high
	// modulate power if battery is low, map so it ramps smoothly
}

int wattsToVolts(int watts) {
	return watts;
}

float readCoilTemp(struct Context ctx) {
	float result;
	result = ctx.atomizer.resistance - ctx.coil.baseRes;
	result = result / (ctx.coil.baseRes * ctx.settings.tcrValue);

	return result;
}

int calcPID(int temp) {
	return temp;
}

void fireVolts(int volts) {

}
