#ifndef VUMeter_h
#define VUMeter_h

#include <Adafruit_NeoPixel.h>
#include "VolumeContext.h"
#include "LedStripContext.h"

enum MeterType { REGULAR, REVERSE };

class VUMeter {
	public:
		VUMeter(MeterType type, LedStripContext &context, unsigned int startPosition, float sizeRelativeToStrip, unsigned int fallOffTimeMs);
		~VUMeter();
		virtual void loop(VolumeContext &context, unsigned int startPositionOffset);

	private:
		unsigned int previousMax = 0;
		unsigned long previousMaxTime = millis();
		unsigned int sizeInLeds;
		unsigned int startPosition;
		unsigned int fallOffTimeMs;
		MeterType type;
};

#endif