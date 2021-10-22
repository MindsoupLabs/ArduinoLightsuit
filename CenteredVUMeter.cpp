#include "CenteredVUMeter.h"
#include "RegularVUMeter.h"

CenteredVUMeter::CenteredVUMeter(LedStripContext &context, unsigned int startPosition, float sizeRelativeToStrip, unsigned int fallOffTimeMs) {
	leftMeter = new RegularVUMeter(REGULAR, context, startPosition, sizeRelativeToStrip * 0.5, fallOffTimeMs);
	rightMeter = new RegularVUMeter(REVERSE, context, startPosition, sizeRelativeToStrip * 0.5, fallOffTimeMs);
}

CenteredVUMeter::~CenteredVUMeter() {
	delete leftMeter;
	delete rightMeter;
}

void CenteredVUMeter::loop(VolumeContext &context, unsigned int startPositionOffset) {
	leftMeter->loop(context, startPositionOffset);
	rightMeter->loop(context, startPositionOffset);
}
