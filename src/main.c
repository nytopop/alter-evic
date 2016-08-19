// alter-evic v2

#include "main.h"
#include "coil.h"
#include "anim.h"
#include "control.h"

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

// all amps, watts, volts, and ohms are in /1 form.
// ie: 4 = 400mV, 400mA, 400mO, 400mW

// 400 == 0.4V, 0.4A, 0.4O, 0.4W

// mode: 0-settings, 1-vw, 2-bp, 3-tc

// load tcr curves

volatile Context ctx;

// Main
int main() {
	// initial settings, will change to user interactive / eeprom
	ctx.settings.mode = 1;
	ctx.settings.maxWatts = 75000;
	ctx.settings.tcrValue = 0.0045;

	ctx.settings.flip = 0;
	ctx.settings.stealth = 0;
	ctx.settings.lock = 0;
	ctx.settings.timeout = 60;

	// init timers
	uint8_t timer;
	timer = Timer_CreateTimer(1, 1, incrementTime, 1);

	// main loop, rest is event handling
	while(1) {
		// collect runtime data
		collectData();

		// do controls
		doControls();

		if(!ctx.state.firing) {
			displayIdle();
		}
	}

	return 0;
}

void collectData() {
	// state 
	ctx.state.firing = 0;

	// update firing

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
	if(ctx.state.firing) {
		ctx.battery.loadVolts = ctx.battery.volts;
	}

	// settings
	ctx.settings.maxWatts = (ctx.battery.percent * 1000);
}

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

void sleep() {
	// engage sleep mode
}
