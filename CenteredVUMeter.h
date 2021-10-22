#ifndef CenteredVUMeter_h
#define CenteredVUMeter_h

#include "VUMeter.h"
#include "ColorGenerator.h"
#include "VolumeContext.h"
#include "LedStripContext.h"

class CenteredVUMeter : public VUMeter {
	public:
		CenteredVUMeter(LedStripContext &context, unsigned int startPosition, float sizeRelativeToStrip, unsigned int fallOffTimeMs, ColorGenerator* colorGenerator);
		~CenteredVUMeter();
		void loop(VolumeContext &context, unsigned int startPositionOffset);

	private:
		VUMeter* leftMeter;
		VUMeter* rightMeter;
};

#endif