// alter-evic

#include "main.h"
#include "anim.h"
#include "bitmap_line.h"
#include "bitmap_kanji.h"
#include <stdlib.h>
#include <M451Series.h>
#include <Display.h>
#include <Font.h>
#include <Button.h>
#include <Atomizer.h>
#include <Battery.h>


// Globals
extern double tcr, coil_temp;
extern uint32_t bp_wattage, wattage, boost_time, boost_wattage, boost, tc_temp, mode;
extern Atomizer_Info_t atomizer;


void anim_png() {
	uint32_t battery_voltage, battery_percent;
	battery_voltage = Battery_GetVoltage();
	battery_percent = Battery_VoltageToPercent(battery_voltage);

	char w_buf[12];
	char bp_buf[12];
	if(mode == 2) {
		snprintf(w_buf, sizeof(w_buf), "%dw", bp_wattage / 100);
	} else if(mode == 1) {
		snprintf(w_buf, sizeof(w_buf), "%dw", wattage / 100);
	} else if(mode == 3) {
		snprintf(w_buf, sizeof(w_buf), "%d°c", tc_temp);
	}
	if(battery_percent == 100) {
		snprintf(bp_buf, sizeof(bp_buf), "%d%%", battery_percent);
	} else if(battery_percent < 10) {
		snprintf(bp_buf, sizeof(bp_buf), "  %d%%", battery_percent);
	} else {
		snprintf(bp_buf, sizeof(bp_buf), " %d%%", battery_percent);
	}
	Display_Clear();
	Display_PutPixels(4, 0, bitmap_kanji, bitmap_kanji_width, bitmap_kanji_height);
	Display_PutText(0, 116, w_buf, FONT_DEJAVU_8PT);
	Display_PutText(32, 116, bp_buf, FONT_DEJAVU_8PT);
	Display_Update();
}


uint32_t lines_watts() {
	uint32_t result;
	double v, a, w, r;
	
	v = (double)atomizer.voltage / 1000;
	a = (double)atomizer.current / 1000;
	w = v * a;
	r = w / 80.0;
	result = (double)r * 104;
	return result;
}


uint32_t lines_volts() {
	double r, result;

	r = (double)atomizer.voltage / 9000;
	result = r * 104;
	return (uint32_t)result;
}


void anim_lines(uint32_t pixels, uint32_t battery_load) {
	Display_Clear();
	uint8_t i;
	for(i = 0; i < pixels; i += 2) {
		// populate 
		Display_PutPixels(0, i, Bitmap_line, Bitmap_line_width, Bitmap_line_height);
	}
	
	char w_buf[12];
	char ba_buf[12];
	double v, a, w, ba;
	v = (double)atomizer.voltage / 1000;
	a = (double)atomizer.current / 1000;
	w = v * a;
	ba = (double)battery_load / 1000;
	ba = w / ba;
	snprintf(w_buf, sizeof(w_buf), "aW %f", w);
	if(mode == 3) {
		snprintf(ba_buf, sizeof(ba_buf), "cT %f", coil_temp);
	} else {
		snprintf(ba_buf, sizeof(ba_buf), "bA %f", ba);
	}
	Display_PutPixels(0, 104, Bitmap_line, Bitmap_line_width, Bitmap_line_height);
	Display_PutText(0, 105, w_buf, FONT_DEJAVU_8PT);
	Display_PutText(0, 116, ba_buf, FONT_DEJAVU_8PT);

	Display_Update();
}


void anim_matrix() {
	// move plane detection to calling routine
	uint16_t truew = ((((atomizer.voltage * atomizer.current) / 10000) / 500) * 8);
	uint16_t setw = (wattage / 500) * 8;
	float ratio = (float) truew / setw;
	float limit = ratio * 128;

	Display_Clear();
	uint8_t i;
	// jiggle it
	for(i = (rand() % 3); i < limit; i += (rand() % (20 - 12 + 1) + 12)) {
		char matBuf[12];
		int r = rand();
		int rr = rand();
		snprintf(matBuf, sizeof(matBuf), "%d%d", r, rr);
		Display_PutText(0, i, matBuf, FONT_DEJAVU_8PT);
	}
	Display_Update();
}


void anim_home(uint32_t volts, uint32_t battery_load) {
	uint32_t battery_voltage, battery_percent, device_temp;
	
	battery_voltage = Battery_GetVoltage();
	battery_percent = Battery_VoltageToPercent(battery_voltage);

	device_temp = Atomizer_ReadBoardTemp();

	char m_buf[12];
	if(mode == 1) {
		snprintf(m_buf, sizeof(m_buf), "mode %s", "vv");
	} else if(mode == 2) {
		snprintf(m_buf, sizeof(m_buf), "mode %s", "bp");
	} else if(mode == 3) {
		snprintf(m_buf, sizeof(m_buf), "mode %s", "tc");
	}

	char w_buf[12];
	if(mode == 2) {
		snprintf(w_buf, sizeof(w_buf), "aW %f", (double)bp_wattage / 100);
	} else if(mode == 1) {
		snprintf(w_buf, sizeof(w_buf), "aW %f", (double)wattage / 100);
	} else if(mode == 3) {
		snprintf(w_buf, sizeof(w_buf), "aT %d", tc_temp);
	}
	char v_buf[12];
	snprintf(v_buf, sizeof(v_buf), "aV %f", (double)volts / 1000);
	// switch to atomizer.resistance/base
	char r_buf[12];
	snprintf(r_buf, sizeof(r_buf), "aR %f", (double)atomizer.baseResistance / 1000);
	
	char ba_buf[12];
	if(mode == 2) {
		snprintf(ba_buf, sizeof(ba_buf), "bA %f", (double)bp_wattage / ((battery_load - 40) / 10));
	} else {
		snprintf(ba_buf, sizeof(ba_buf), "bA %f", (double)wattage / (battery_load / 10));
	}
	
	char bv_buf[12];
	snprintf(bv_buf, sizeof(bv_buf), "bV %f", (double)battery_voltage / 1000);
	char bp_buf[12];
	snprintf(bp_buf, sizeof(bp_buf), "bP  %d", battery_percent);
	char da_buf[12];
	if(mode == 2) {
		snprintf(da_buf, sizeof(da_buf), "dA %f", (double)bp_wattage / (battery_load / 10));
	} else {
		snprintf(da_buf, sizeof(da_buf), "dA %f", (double)wattage / (volts / 10));
	}
	char dt_buf[12];
	snprintf(dt_buf, sizeof(dt_buf), "dT  %d", device_temp);

	Display_Clear();
	Display_PutText(0, 0, m_buf, FONT_DEJAVU_8PT);
	Display_PutText(0, 12, w_buf, FONT_DEJAVU_8PT);
	Display_PutText(0, 22, v_buf, FONT_DEJAVU_8PT);
	Display_PutText(0, 32, r_buf, FONT_DEJAVU_8PT);
	Display_PutText(0, 46, ba_buf, FONT_DEJAVU_8PT);
	Display_PutText(0, 56, bv_buf, FONT_DEJAVU_8PT);
	Display_PutText(0, 66, bp_buf, FONT_DEJAVU_8PT);
	Display_PutText(0, 80, da_buf, FONT_DEJAVU_8PT);
	Display_PutText(0, 90, dt_buf, FONT_DEJAVU_8PT);
	Display_Update();
}
