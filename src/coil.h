#ifndef COIL_H
#define COIL_H

int checkFire();
int wattsToVolts(int watts);
float readCoilTemp();
int calcPID(int temp);
void fire();

#endif
