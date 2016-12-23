// control.c

#include "main.h"
#include "control.h"
#include "coil.h"

#include <stdlib.h>
#include <math.h>

#include <M451Series.h>
#include <Display.h>
#include <Font.h>
#include <Button.h>
#include <TimerUtils.h>
#include <Atomizer.h>

extern volatile Context ctx;

// read all control inputs and do an action
void doControls() {
	uint8_t state;
	state = Button_GetState();

	// switch on button state
	switch(state) {
		case 0x00: // none
			Atomizer_Control(0);
			break;
		case 0x01: // fire
			ctx.state.idleTimer = 0;
			controlFire();
			break;
		case 0x02: // right
			ctx.state.idleTimer = 0;
			Atomizer_Control(0);
			controlRight();
			break;
		case 0x03: // fire + right
			ctx.state.idleTimer = 0;
			Atomizer_Control(0);
			break;
		case 0x04: // left
			ctx.state.idleTimer = 0;
			Atomizer_Control(0);
			controlLeft();
			break;
		case 0x05: // fire + left
			ctx.state.idleTimer = 0;
			Atomizer_Control(0);
			break;
		case 0x06: // left + right
			ctx.state.idleTimer = 0;
			Atomizer_Control(0);
			// switch modes
			if(ctx.settings.mode == 3) {
				ctx.settings.mode = 0;
			} else {
				ctx.settings.mode += 1;
			}
			Timer_DelayMs(300);
			break;
		case 0x07: // left + right + fire
			ctx.state.idleTimer = 0;
			Atomizer_Control(0);
			break;
	}

}

void controlLeft() {
	switch(ctx.settings.mode) {
		case 0: // settings
			break;
		case 1: // vw
			if(ctx.settings.tW > 0)
				ctx.settings.tW -= 1000;
			break;
		case 2: // bp
			break;
		case 3: // tc
			if(ctx.settings.tT > ctx.settings.minTemp)
				ctx.settings.tT -= 1;
			break;
	}
}

void controlRight() {
	switch(ctx.settings.mode) {
		case 0: // settings
			break;
		case 1: // vw
			if(ctx.settings.tW < ctx.settings.maxCtrlWatts)
				ctx.settings.tW += 1000;
			break;
		case 2: // bp
			break;
		case 3: // tc
			if(ctx.settings.tT < ctx.settings.maxTemp)
				ctx.settings.tT += 1;
			break;
	}
}

void controlFire() {
	int outV, outW;

	switch(ctx.settings.mode) {
		case 0: // settings
			//outV = 0;
			//Atomizer_SetOutputVoltage(outV);
			break;
		case 1: // vw
			outW = ctx.settings.tW;
			if(outW > ctx.settings.maxWatts)
				outW = ctx.settings.maxWatts;
			outV = (int)sqrt(outW * ctx.atomizer.resistance);
			Atomizer_SetOutputVoltage(outV);
			fire();
			break;
		case 2: // bp
			outV = ctx.battery.volts;
			Atomizer_SetOutputVoltage(outV);
			fire();
			break;
		case 3: // tc
			outW = wattsPID();
			outV = (int)sqrt(outW * ctx.atomizer.resistance);
			Atomizer_SetOutputVoltage(outV);
			fire();
			break;
	}
}
