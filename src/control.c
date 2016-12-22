// control.c

#include "main.h"
#include "control.h"
#include "coil.h"

#include <stdlib.h>
#include <M451Series.h>
#include <Display.h>
#include <Font.h>
#include <Button.h>
#include <TimerUtils.h>

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
			controlFire();
			break;
		case 0x02: // right
			Atomizer_Control(0);
			controlRight();
			break;
		case 0x03: // fire + right
			Atomizer_Control(0);
			break;
		case 0x04: // left
			Atomizer_Control(0);
			controlLeft();
			break;
		case 0x05: // fire + left
			Atomizer_Control(0);
			break;
		case 0x06: // left + right
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
	switch(ctx.settings.mode) {
		case 0: // settings
			break;
		case 1: // vw
			fire();
			break;
		case 2: // bp
			fire();
			break;
		case 3: // tc
			break;
	}
}
