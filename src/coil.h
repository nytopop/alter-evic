#ifndef COIL_H
#define COIL_H

void checkFire(struct Context ctx);
int wattsToVolts(int watts);
int readCoilTemp(struct Context ctx);
int calcPID(int temp);
void fireVolts(int volts);

#endif
