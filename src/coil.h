#ifndef COIL_H
#define COIL_H

void doFireSet();
int checkFire();
int wattsToVolts(int watts);
float readCoilTemp();
int wattsPID(int temp);
void fire();

#endif
