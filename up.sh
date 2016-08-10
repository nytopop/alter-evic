#!/bin/sh

export EVICSDK="`pwd`/lib/evic-sdk"
echo "SDK is at: $EVICSDK"

#cd lib/evic-sdk
#make clean
#make

#cd ../..
make clean
make && sudo evic-usb upload bin/alter-evic.bin
make clean
