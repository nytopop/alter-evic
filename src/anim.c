// display.c

#include "main.h"
#include "anim.h"
#include "bitmap.h"

#include <stdlib.h>
#include <stdio.h>
#include <M451Series.h>
#include <Display.h>
#include <Font.h>

extern volatile Context ctx;

void displayFiring() {
	char temp_buf[12];
	char out_buf[5];
	snprintf(temp_buf, sizeof(temp_buf), "%dc", ctx.coil.temp);
	snprintf(out_buf, sizeof(out_buf), "%dw",
			(ctx.atomizer.voltage * ctx.atomizer.current) / 1000000);


	// PID
	char p_buf[32];
	char i_buf[32];
	char d_buf[32];
	char o_buf[32];
	snprintf(p_buf, sizeof(p_buf), "%f", ctx.coil.p);
	snprintf(i_buf, sizeof(i_buf), "%f", ctx.coil.i);
	snprintf(d_buf, sizeof(d_buf), "%f", ctx.coil.d);
	snprintf(o_buf, sizeof(o_buf), "%f", ctx.coil.out);

	Display_Clear();

	int lines;
	lines = numLines();
	for(int i = 0; i < lines; i += 2) {
		Display_PutPixels(0, i, bitmapLine, bitmapLineWidth, bitmapLineHeight);
	}

	Display_PutText(0, 70, p_buf, FONT_DEJAVU_8PT);
	Display_PutText(0, 80, i_buf, FONT_DEJAVU_8PT);
	Display_PutText(0, 90, d_buf, FONT_DEJAVU_8PT);
	Display_PutText(0, 100, o_buf, FONT_DEJAVU_8PT);

	Display_PutPixels(0, 112, bitmapLine, bitmapLineWidth, bitmapLineHeight);
	Display_PutText(0, 116, temp_buf, FONT_DEJAVU_8PT);
	Display_PutText(32, 116, out_buf, FONT_DEJAVU_8PT);
	Display_Update();
}

int numLines() {
	int w;
	float r;

	w = (ctx.atomizer.voltage * ctx.atomizer.current) / 1000;
	r = ((float)w / (float)ctx.settings.maxCtrlWatts) * 104;

	return (int)r;
}

void displaySettings() {
	char mode_buf[32];
	char res_buf[32];
	char err_buf[32];
	
	snprintf(mode_buf, sizeof(mode_buf), "%s", "settings");
	snprintf(res_buf, sizeof(res_buf), "%d %d", 
			ctx.atomizer.baseResistance, ctx.atomizer.resistance);
	snprintf(err_buf, sizeof(err_buf), "%s", ctx.state.error);

	Display_Clear();
	Display_PutText(0, 0, mode_buf, FONT_DEJAVU_8PT);
	Display_PutText(0, 10, res_buf, FONT_DEJAVU_8PT);
	Display_PutText(0, 20, err_buf, FONT_DEJAVU_8PT);
	Display_Update();
}

void displayVW() {
	char mode_buf[3];
	char out_buf[5];
	char bat_buf[5];
	
	snprintf(mode_buf, sizeof(mode_buf), "%s", "vw");
	snprintf(out_buf, sizeof(out_buf), "%dw", ctx.settings.tW/1000);
	snprintf(bat_buf, sizeof(bat_buf), "%d%%", ctx.battery.percent);
	
	Display_Clear();
	Display_PutPixels(4, 2, bitmapBottle, bitmapBottleWidth, bitmapBottleHeight);
	Display_PutText(24, 0, mode_buf, FONT_DEJAVU_8PT);
	Display_PutPixels(0, 112, bitmapLine, bitmapLineWidth, bitmapLineHeight);
	Display_PutText(0, 116, out_buf, FONT_DEJAVU_8PT);
	Display_PutText(32, 116, bat_buf, FONT_DEJAVU_8PT);
	Display_Update();
}
void displayBP() {
	char mode_buf[3];
	char out_buf[5];
	char bat_buf[5];
	
	snprintf(mode_buf, sizeof(mode_buf), "%s", "bp");
	snprintf(out_buf, sizeof(out_buf), "%dw", ctx.settings.bW/1000);
	snprintf(bat_buf, sizeof(bat_buf), "%d%%", ctx.battery.percent);
	
	Display_Clear();
	Display_PutPixels(4, 2, bitmapBottle, bitmapBottleWidth, bitmapBottleHeight);

	Display_PutText(24, 0, mode_buf, FONT_DEJAVU_8PT);
	Display_PutPixels(0, 112, bitmapLine, bitmapLineWidth, bitmapLineHeight);
	Display_PutText(0, 116, out_buf, FONT_DEJAVU_8PT);
	Display_PutText(32, 116, bat_buf, FONT_DEJAVU_8PT);
	Display_Update();
}
void displayTC() {
	char mode_buf[3];
	char out_buf[5];
	char bat_buf[5];
	
	snprintf(mode_buf, sizeof(mode_buf), "%s", "tc");
	snprintf(out_buf, sizeof(out_buf), "%dc", ctx.settings.tT);
	snprintf(bat_buf, sizeof(bat_buf), "%d%%", ctx.battery.percent);
	
	Display_Clear();
	Display_PutPixels(4, 2, bitmapBottle, bitmapBottleWidth, bitmapBottleHeight);
	Display_PutText(24, 0, mode_buf, FONT_DEJAVU_8PT);
	Display_PutPixels(0, 112, bitmapLine, bitmapLineWidth, bitmapLineHeight);
	Display_PutText(0, 116, out_buf, FONT_DEJAVU_8PT);
	Display_PutText(32, 116, bat_buf, FONT_DEJAVU_8PT);
	Display_Update();
}
