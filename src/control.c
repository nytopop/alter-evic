// control.c

#include "main.h"
#include "control.h"

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

	// register both outside switch	
	if((state & BUTTON_MASK_LEFT) && (state & BUTTON_MASK_RIGHT)) {
		if(ctx.settings.mode == 3) {
			ctx.settings.mode = 0;
		} else {
			ctx.settings.mode += 1;
		}
		Timer_DelayMs(300);
	}

	switch(ctx.settings.mode) {
		case 0:	// settings
			break;
		case 1: // vw
			if(state & BUTTON_MASK_LEFT) {
				ctx.settings.tW -= 1;
			} else if(state & BUTTON_MASK_RIGHT) {
				ctx.settings.tW += 1;
			}	
			break;
		case 2: // bp
			break;
		case 3: // tc
			if(state & BUTTON_MASK_LEFT) {
				ctx.settings.tT -= 1;
			} else if(state & BUTTON_MASK_RIGHT) {
				ctx.settings.tT += 1;
			}
			break;
		default:
			break;
	}
}



void fireCallback(uint8_t state) {
	// do nothing yet
	// call fire() from coil.h
}
