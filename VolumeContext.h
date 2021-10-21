#ifndef VolumeContext_h
#define VolumeContext_h

#include <Adafruit_NeoPixel.h>

struct VolumeContext {
	double volume;
	Adafruit_NeoPixel* strip;
	unsigned int numLeds;
};

#endif