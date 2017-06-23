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
#include <System.h>
#include <Display.h>

volatile Context ctx;
volatile uint8_t loop;

// Main
int main() {
	// initial settings, will change to user interactive / eeprom
	ctx.settings.mode = 3;
	ctx.settings.maxCtrlWatts = 75000;
	ctx.settings.maxWatts = 75000;
	ctx.settings.minTemp = 60;
	ctx.settings.maxTemp = 300;
	ctx.settings.tcrValue = 0.00092;

	ctx.settings.tT = 248;
	ctx.settings.tW = 56000;

	ctx.settings.lockRes = false;
	ctx.settings.flip = false;
	ctx.settings.stealth = false;
	ctx.settings.lock = false;
	ctx.settings.timeout = 10;

	// init timers
	uint8_t timeTimer;
	timeTimer = Timer_CreateTimer(1, 1, incrementTime, 1);
	uint8_t loopTimer;
	loopTimer = Timer_CreateTimer(FPS, 1, readyLoop, 1);

	// init atomizer
	Atomizer_SetErrorLock(true);

	// main loop, rest is event handling
	while(1) {
		loop = 0;
		// collect runtime data
		collectData();

		// do controls
		doControls();

		if(ctx.state.firing) {
			displayFiring();
		} else {
			if(ctx.state.idleTimer < 20) {
				switch(ctx.settings.mode) {
					case 0: // settings
						displaySettings();
						break;
					case 1: // vw
						displayVW();
						break;
					case 2: // bp
						displayBP();
						break;
					case 3: // tc
						displayTC();
						break;
				}
			} else {
				// switch to low power mode
				Display_Clear();
				Display_Update();
				sleep();
			}
		}

		// waste time until loop refresh timer is ready
		while(loop == 0) {}
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
	ctx.coil.temp = readCoilTemp();
	// TODO lock resistance setting here
	if(!ctx.settings.lockRes)
		ctx.coil.baseRes = ctx.atomizer.baseResistance;
		//ctx.coil.baseRes = 377;

	// battery
	ctx.battery.volts = Battery_GetVoltage();
	if(!ctx.state.firing)
		ctx.battery.percent = Battery_VoltageToPercent(ctx.battery.volts);
	if(ctx.state.firing)
		ctx.battery.loadVolts = ctx.battery.volts;

	// calculate max watts as [w = ((bPct / 100) * 50 + 30) * 1000)]
	// At 50 - 100%, 75W
	// At 30 - 49%   59W
	// At 0  - 29%   45W
	/*if(ctx.battery.percent >= 50) {
		ctx.settings.maxWatts = 75000;
	} else if(ctx.battery.percent >= 30) {
		ctx.settings.maxWatts = 59000;
	} else {
		ctx.settings.maxWatts = 45000;
	}*/
	float r = (float)ctx.battery.percent / 100;
	r *= 50;
	r += 30;
	ctx.settings.maxWatts = (int)r * 1000;

	// calculate bypass watts as [w = e^2 / r]
	if(ctx.battery.loadVolts != 0) {
		ctx.settings.bW = ctx.battery.loadVolts * ctx.battery.loadVolts;
		ctx.settings.bW /= ctx.atomizer.resistance;
	} else {
		ctx.settings.bW = ctx.battery.volts * ctx.battery.volts;
		ctx.settings.bW /= ctx.atomizer.resistance;
	}
}

// Callback; 
void readyLoop() {
	loop = 1;
}

// Callback; increments timers every second
void incrementTime() {
	if(ctx.state.firing) {
		ctx.state.idleTimer = 0;
		ctx.state.fireTimer += 1;
		ctx.state.fireTotal += 1;
	} else {
		ctx.state.fireTimer = 0;
		ctx.state.idleTimer += 1;
	}
}

// Engage sleep mode
void sleep() {
	Sys_SetWakeupSource(0x0f);
	Sys_Sleep();
}
