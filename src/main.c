// alter-evic v2

#include "main.h"
#include "coil.h"
#include "anim.h"
#include "control.h"

#include <stdlib.h>
#include <stdbool.h>
#include <M451Series.h>
#include <Atomizer.h>
#include <Button.h>
#include <TimerUtils.h>
#include <Battery.h>

#define FIRE 0
#define RIGHT 1
#define LEFT 2
#define FPS 30

volatile Context ctx;

// Main
int main() {
	// initial settings, will change to user interactive / eeprom
	ctx.settings.mode = 2;
	ctx.settings.maxCtrlWatts = 75000;
	ctx.settings.maxWatts = 75000;
	ctx.settings.minTemp = 60;
	ctx.settings.maxTemp = 300;
	ctx.settings.tcrValue = 0.00098;

	ctx.settings.tT = 120;
	ctx.settings.tW = 56000;

	ctx.settings.lockRes = false;
	ctx.settings.flip = false;
	ctx.settings.stealth = false;
	ctx.settings.lock = false;
	ctx.settings.timeout = 10;

	// init timers
	uint8_t timer;
	timer = Timer_CreateTimer(1, 1, incrementTime, 1);

	// init atomizer
	Atomizer_SetErrorLock(true);

	// main loop, rest is event handling
	while(1) {
		// collect runtime data
		collectData();

		// set operation settings
		doFireSet();

		// do controls
		doControls();

		if(ctx.state.firing) {
			displayFiring();
		} else {
			displayIdle();
		}
	}

	return 0;
}

// Collect runtime data.
void collectData() {
	// state
	ctx.state.firing = Atomizer_IsOn();
	ctx.state.error = Atomizer_GetError();

	// atomizer
	Atomizer_ReadInfo(&ctx.atomizer);

	// device
	ctx.device.temp = Atomizer_ReadBoardTemp();

	// coil
	ctx.coil.temp = readCoilTemp(ctx);
	if(!ctx.settings.lockRes)
		ctx.coil.baseRes = ctx.atomizer.baseResistance;

	// battery
	ctx.battery.volts = Battery_GetVoltage();

	if(!ctx.state.firing)
		ctx.battery.percent = Battery_VoltageToPercent(ctx.battery.volts);
	
	if(ctx.state.firing)
		ctx.battery.loadVolts = ctx.battery.volts;

	// settings
	ctx.settings.maxWatts = (ctx.battery.percent * 900);

	// calculate bypass watts with : p = (e^2 / r)
	if(ctx.battery.loadVolts != 0) {
		ctx.settings.bW = ctx.battery.loadVolts * ctx.battery.loadVolts;
		ctx.settings.bW /= ctx.atomizer.resistance;
	} else {
		ctx.settings.bW = ctx.battery.volts * ctx.battery.volts;
		ctx.settings.bW /= ctx.atomizer.resistance;
	}
}

// Callback; increments timers every second
void incrementTime() {
	if(!ctx.state.firing) {
		ctx.state.fireTimer = 0;
		ctx.state.idleTimer += 1;
	} else {
		ctx.state.idleTimer = 0;
		ctx.state.fireTimer += 1;
		ctx.state.fireTotal += 1;
	}
}

// Engage sleep mode
void sleep() {
	// engage sleep mode
}
