#ifndef RegularVUMeter_h
#define RegularVUMeter_h

#include "VUMeter.h"
#include "ColorGenerator.h"
#include "VolumeContext.h"
#include "LedStripContext.h"

enum MeterType { REGULAR, REVERSE };

class RegularVUMeter : public VUMeter {
	public:
		RegularVUMeter(MeterType type, LedStripContext &context, unsigned int startPosition, float sizeRelativeToStrip, unsigned int fallOffTimeMs, ColorGenerator* colorGenerator, bool deleteColorGeneratorOnDestruct);
		~RegularVUMeter();
		virtual void loop(VolumeContext &context, unsigned int startPositionOffset);

	private:
		unsigned int previousMax = 0;
		unsigned long previousMaxTime = millis();
		unsigned int sizeInLeds;
		unsigned int startPosition;
		unsigned int fallOffTimeMs;
		MeterType type;
		ColorGenerator* colorGenerator;
		bool deleteColorGeneratorOnDestruct = true;
};

#endif