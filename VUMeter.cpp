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
	// handle falloff
	// first reduce the previousMax value according to the falloff speed, but never below 0
	unsigned long timeElapsedSinceLast = millis() - this->previousMaxTime;
	this->previousMax = max(this->previousMax - (context.ledStrip.numLeds * ((double)timeElapsedSinceLast / fallOffTimeMs)), 0.0);
	// light up pixels is either relative to the volume or the previous max, whichever higher
	unsigned int lightUpPixels = ceil(context.volume * this->sizeInLeds);
	lightUpPixels = max(lightUpPixels, this->previousMax);
	if(lightUpPixels > this->previousMax) {
		this->previousMax = lightUpPixels;
		this->previousMaxTime = millis();
	}

	// set lit up pixels
	unsigned int startPosition = this->startPosition + startPositionOffset;
	int ledIndex = startPosition;
	for(unsigned int i = 0; i < this->sizeInLeds; i++) {
		// regular or reverse order
		ledIndex = this->type == REGULAR ? i + startPosition : startPosition - i;

		// wrap around
		ledIndex = ledIndex < 0 ? context.ledStrip.numLeds + ledIndex : ledIndex;
		ledIndex = ledIndex < context.ledStrip.numLeds ? ledIndex : ledIndex - context.ledStrip.numLeds;

		if(i < lightUpPixels) {
			context.ledStrip.strip->setPixelColor(ledIndex, 0, 0, 128);
		} else {
			// turn unlit pixels off
			context.ledStrip.strip->setPixelColor(ledIndex, 0, 0, 0);
		}

	}

	context.ledStrip.strip->show();
}