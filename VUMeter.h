#ifndef VUMeter_h
#define VUMeter_h

#include <Adafruit_NeoPixel.h>
#include "VolumeContext.h"
#include "LedStripContext.h"

enum MeterType { REGLAR, REVERSE, CENTERED };

class VUMeter {
	public:
		VUMeter(MeterType type, LedStripContext &context, unsigned int startPosition, float sizeRelativeToStrip);
		~VUMeter();
		void loop(VolumeContext &context, unsigned int startPositionOffset) = 0;
};

##endif