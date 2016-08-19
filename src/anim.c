// display.c

#include "main.h"
#include "anim.h"

#include <stdlib.h>
#include <M451Series.h>
#include <Display.h>
#include <Font.h>

extern volatile Context ctx;

void displayIdle() {
	char mode_buf[12];
	char out_buf[12];
	char bat_buf[12];

	switch(ctx.settings.mode) {
		case 0: // set
			displaySettings(ctx);
			return;
		case 1: // vw
			snprintf(mode_buf, sizeof(mode_buf), "%s", "vw");
			snprintf(out_buf, sizeof(out_buf), "%dw", ctx.settings.tW);
			break;
		case 2: // bp
			snprintf(mode_buf, sizeof(mode_buf), "%s", "bp");
			snprintf(out_buf, sizeof(out_buf), "%dw", ctx.battery.loadVolts);
			break;
		case 3: // tc
			snprintf(mode_buf, sizeof(mode_buf), "%s", "tc");
			snprintf(out_buf, sizeof(out_buf), "%dc", ctx.settings.tT);
			break;
		default:
			break;
	}

	snprintf(bat_buf, sizeof(bat_buf), "%d%%", ctx.battery.percent);

	Display_Clear();
	Display_PutText(24, 0, mode_buf, FONT_DEJAVU_8PT);
	Display_PutText(0, 116, out_buf, FONT_DEJAVU_8PT);
	Display_PutText(32, 116, bat_buf, FONT_DEJAVU_8PT);
	Display_Update();
}

void displayFiring() {
	// do stuff
}

void displaySettings() {
	// do stuff
	// fire goes down in settings !
}
