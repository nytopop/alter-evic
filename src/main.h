#include <Atomizer.h>
#include <stdbool.h>

#ifndef MAIN_H
#define MAIN_H

#define FPS 24

struct Settings {
	int mode; // 0:settings, 1:variable wattage, 2:bypass, 3:temp control
	int maxCtrlWatts;
	int maxWatts; // batPercent * 900 [100%:90w, 50%:45w]
	int minTemp;
	int maxTemp;
	float tcrValue; // TODO

	int tT; // target temperature
	int tW; // target wattage
	int bW; // bypass wattage

	int setPos; // menu position


	bool lockRes; // TODO
	bool flip; // TODO
	bool stealth; // TODO
	bool lock; // TODO
	int timeout;
};

struct State {
	bool firing;
	Atomizer_Error_t error;
	// something rtc?; unixtime!
	int fireTimer;
	int idleTimer;
	int fireTotal;
	int puffTotal;
};

struct Device {
	int temp;
};

struct Coil {
	int baseRes;
	int temp;
	float pTerm;
	float iTerm;
	float dTerm;
};

struct Battery {
	int amps;
	int volts;
	int loadVolts;
	int percent;
};

typedef struct {
	Atomizer_Info_t atomizer;
	struct Settings settings;
	struct State state;
	struct Device device;
	struct Coil coil;
	struct Battery battery;
} Context;

typedef struct {
	bool flipDisplay;
	bool lockResistance;
	bool stealthMode;
	bool lockButtons;
	float tcrValue;
} FlashSettings;

void collectData();
void readyLoop();
void incrementTime();
void sleep();

#endif
