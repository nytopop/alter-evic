// alter-evic v2

#include "main.h"
#include "coil.h"

#include <stdlib.h>
#include <M451Series.h>
#include <Atomizer.h>
#include <Button.h>
#include <TimerUtils.h>
#include <Battery.h>

#define FIRE 0
#define RIGHT 1
#define LEFT 2
#define FPS 30

// all amps, watts, volts, and ohms are in /1000 form.
// ie: 400 = 400mV, 400mA, 400mO, 400mW

// mode: 0-settings, 1-vv, 2-vw, 3-bp, 4-tc

// load tcr curves

// Main
int main() {
	struct Context ctx;

	// initial settings, will change to user interactive / eeprom
	ctx.settings.mode = 0;
	ctx.settings.maxWatts = 75000;
	ctx.settings.tcrValue = 0.0045;

	ctx.settings.flip = 0;
	ctx.settings.stealth = 0;
	ctx.settings.lock = 0;
	ctx.settings.timeout = 60;

	// main loop, rest is event handling
	while(1) {
		// collect runtime data
		ctx = collectData(ctx);

		// run based on ctx
		switch(ctx.settings.mode) {
			case 0:
				// settings
				break;
			case 1:
				// vv
				break;
			case 2:
				// vw
				break;
			case 3:
				// bp
				break;
			case 4:
				// tc
				break;
			default:
				// shouldn't ever run
				ctx.settings.mode = 0;
				break;
		}
	}
}

struct Context collectData(struct Context ctx) {
	// settings
	// state
	if(ctx.state.firing) {
		// 
	}

	// atomizer
	Atomizer_ReadInfo(&ctx.atomizer);

	// device
	ctx.device.temp = Atomizer_ReadBoardTemp();

	// coil
	ctx.coil.temp = readCoilTemp(ctx);
	if(!ctx.settings.lockRes) {
		ctx.coil.baseRes = ctx.atomizer.baseResistance;
	}

	// battery
	ctx.battery.volts = Battery_GetVoltage();
	ctx.battery.percent = Battery_VoltageToPercent(ctx.battery.volts);

	return ctx;
}

void incrementSleep() {
	// increment sleep timer
}

void sleep() {
	// engage sleep mode
}
