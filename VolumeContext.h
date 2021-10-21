#ifndef VolumeContext_h
#define VolumeContext_h

#include <Adafruit_NeoPixel.h>
#include "LedStripContext.h"

struct VolumeContext {
	double volume;
	LedStripContext ledStrip;
};

#endif