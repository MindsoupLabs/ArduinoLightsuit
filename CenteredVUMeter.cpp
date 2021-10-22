#include "CenteredVUMeter.h"

CenteredVUMeter::CenteredVUMeter(MeterType type, LedStripContext &context, unsigned int startPosition, float sizeRelativeToStrip, unsigned int fallOffTimeMs) : VUMeter(type, context, startPosition, sizeRelativeToStrip, fallOffTimeMs) {
	leftMeter = new VUMeter(REGULAR, context, startPosition, sizeRelativeToStrip * 0.5, fallOffTimeMs);
	rightMeter = new VUMeter(REVERSE, context, startPosition, sizeRelativeToStrip * 0.5, fallOffTimeMs);
}

CenteredVUMeter::~CenteredVUMeter() {
	delete leftMeter;
	delete rightMeter;
}

void CenteredVUMeter::loop(VolumeContext &context, unsigned int startPositionOffset) {
	leftMeter->loop(context, startPositionOffset);
	rightMeter->loop(context, startPositionOffset);
}
