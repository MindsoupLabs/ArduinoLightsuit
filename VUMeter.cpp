#include "VUMeter.h"

VUMeter::VUMeter(MeterType type, LedStripContext &context, unsigned int startPosition, float sizeRelativeToStrip, unsigned int fallOffTimeMs) {
	this->type = type;
	this->fallOffTimeMs = fallOffTimeMs;
	this->startPosition = startPosition;
	this->sizeInLeds = (unsigned int)ceil(sizeRelativeToStrip * context.numLeds);
}

VUMeter::~VUMeter() {
}

void VUMeter::loop(VolumeContext &context, unsigned int startPositionOffset) {
	// reset all LEDs
	context.ledStrip.strip->fill(context.ledStrip.strip->Color(0, 0, 0), 0, context.ledStrip.numLeds);

	unsigned int lightUpPixels = ceil(context.volume * this->sizeInLeds);
	unsigned int startPosition = this->startPosition + startPositionOffset;
	for(unsigned int i = startPosition; i < startPosition + lightUpPixels; i++) {
		// wrap around
		context.ledStrip.strip->setPixelColor(i < context.ledStrip.numLeds ? i : i - context.ledStrip.numLeds, 0, 0, 128);
	}

	context.ledStrip.strip->show();
}