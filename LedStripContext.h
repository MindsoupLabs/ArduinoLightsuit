#ifndef LedStripContext_h
#define LedStripContext_h

#include <Adafruit_NeoPixel.h>

struct LedStripContext {
	Adafruit_NeoPixel* strip;
	unsigned int numLeds;
};

#endif