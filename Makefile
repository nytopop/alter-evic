TARGET = alter-evic
OBJS = src/main.o src/coil.o src/anim.o src/control.o
EVICSDK:=/home/eric/src/projects/c/alter-evic/lib/evic-sdk
include $(EVICSDK)/make/Base.mk

build:
	./build.sh

upload: build
	./upload.sh
