#!/bin/sh

export EVICSDK="`pwd`/lib/evic-sdk"
echo "SDK is at: $EVICSDK"

cd lib/evic-sdk
make clean
make evic-rel
#make

cd ../..
make clean
make evic-rel
