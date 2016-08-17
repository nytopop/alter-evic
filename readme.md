**** alter-evic

*** Bug
There is currently a bug which will cause 

An alternate firmware for Evic VTC-Mini devices.

Take care as the working state is still *experimental*!.

Currently working:

* VV mode
* Bypass mode
* TC mode, but TCR is hardcoded for now (0.0045).

Notes:

* wattage is limited to *80w*, and battery load amps are displayed as "bA", take care not to overdrain your batteries.

Issues / TODO

- No sleep mode

- Dirty code

- No fire time limit

- Max output does not scale down with battery level, leading to device reset as voltage dips too low.

- TC mode: coil temp will intermittently be stuck at -202.x degrees C, leading to no output. Mode switch fixes it.

- TC mode: no lock for base resistance of coil
