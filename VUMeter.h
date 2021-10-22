#ifndef VUMeter_h
#define VUMeter_h

#include <Adafruit_NeoPixel.h>
#include "VolumeContext.h"
#include "LedStripContext.h"

enum MeterType { REGULAR, REVERSE, CENTERED };

class VUMeter {
	public:
		VUMeter(MeterType type, LedStripContext &context, unsigned int startPosition, float sizeRelativeToStrip, unsigned int fallOffTimeMs);
		~VUMeter();
		void loop(VolumeContext &context, unsigned int startPositionOffset);

	private:
		unsigned int previousMax;
		unsigned long previousMaxTime;
};

#endif