#include "VUMeter.h"

VUMeter::VUMeter(MeterType type, LedStripContext &context, unsigned int startPosition, float sizeRelativeToStrip, unsigned int fallOffTimeMs) {
}

VUMeter::~VUMeter() {
}

void VUMeter::loop(VolumeContext &context, unsigned int startPositionOffset) {
	unsigned int lightUpPixels = ceil(context.volume * context.ledStrip.numLeds);

	for(unsigned int i = 0; i < context.ledStrip.numLeds; i++) {
		context.ledStrip.strip->setPixelColor(i, 0, 0, i < lightUpPixels ? 128 : 0);
	}

	context.ledStrip.strip->show();
}