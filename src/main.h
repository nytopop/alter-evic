#include <Atomizer.h>

#ifndef MAIN_H
#define MAIN_H

struct Settings {
	int mode;
	int maxWatts;
	// mW is equal to batPercent * 1000
	// at 100%, 100W
	// at 70%, 70w
	// at 60%, 60w
	float tcrValue;

	uint16_t tT;
	uint8_t tW;

	int lockRes;
	int flip;
	int stealth;
	int lock;
	int timeout;
};

struct State {
	int firing;
	// something rtc?
	uint16_t fireTimer;
	uint16_t idleTimer;
	uint32_t fireTotal;
	uint32_t puffTotal;
};

struct Controls {
	// button combos?
};

struct Device {
	int amps;
	int volts;
	int watts;
	int temp;
	struct Controls controls;
};

struct Coil {
	int baseRes;	
	int temp;
};

struct Battery {
	int amps;
	int volts;
	int loadVolts;
	int percent;
};

typedef struct {
	struct Settings settings;
	struct State state;
	Atomizer_Info_t atomizer;
	struct Device device;
	struct Coil coil;
	struct Battery battery;
} Context;

void collectData();
void incrementTime();
void sleep();

#endif
