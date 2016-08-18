#include <Atomizer.h>

#ifndef MAIN_H
#define MAIN_H

struct Settings {
	int mode;
	int maxWatts;
	float tcrValue;

	int targetTemp;
	int targetVolts;
	int targetWatts;

	int lockRes;
	int flip;
	int stealth;
	int lock;
	int timeout;
};

struct State {
	int firing;
	// something rtc?
	int fireTimer;
	int idleTimer;
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

struct Context {
	struct Settings settings;
	struct State state;
	Atomizer_Info_t atomizer;
	struct Device device;
	struct Coil coil;
	struct Battery battery;
};

struct Context collectData(struct Context ctx);

#endif
