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
	for(unsigned int i = startPosition; i < startPosition + lightUpPixels; i++) {
		// wrap around
		context.ledStrip.strip->setPixelColor(i < context.ledStrip.numLeds ? i : i - context.ledStrip.numLeds, 0, 0, 128);
	}

	context.ledStrip.strip->show();
}