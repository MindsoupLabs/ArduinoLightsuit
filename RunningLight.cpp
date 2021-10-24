#include "RunningLight.h"
#include <Arduino.h>

RunningLight::RunningLight(unsigned int startPosition, int ledsPerSecondSpeed, unsigned int size, unsigned int lifetime, ColorGenerator* colorGenerator, ChainableLightListener* listener) : ChainableLight(listener) {
	this->startPosition = startPosition;
	this->ledsPerSecondSpeed = ledsPerSecondSpeed;
	this->size = size;
	this->lifetime = lifetime;
	this->startTime = millis();
	this->colorGenerator = colorGenerator;
	this->directionIsForwards = ledsPerSecondSpeed > 0;
}

RunningLight::~RunningLight() {
	if(this->colorGenerator != 0) {
		delete this->colorGenerator;
	}
}

void RunningLight::loop(VolumeContext &context) {
	unsigned long elapsedTime = millis() - this->startTime;

	// if our lifetime expires
	if(elapsedTime > lifetime) {
		return this->getListener()->onFinished(this);
	}

	int pixelPosition = this->startPosition + ceil(((float)elapsedTime / 1000) * this->ledsPerSecondSpeed);

	Color color = this->colorGenerator->getColor((float)pixelPosition / context.ledStrip.numLeds);

	float tailColorFactor;
	for(unsigned int i = 0; i < size; i++) {
		tailColorFactor = (float)(size - i) / size;
		if(pixelPosition < context.ledStrip.numLeds && pixelPosition >= 0) {
			context.ledStrip.strip->setPixelColor(pixelPosition, context.ledStrip.strip->Color(floor(color.R * tailColorFactor), floor(color.G * tailColorFactor), floor(color.B * tailColorFactor)));
		}

		pixelPosition = pixelPosition - (this->ledsPerSecondSpeed / abs(this->ledsPerSecondSpeed));
	}

	// if we've moved off the strip entirely
	if((pixelPosition >= (int)context.ledStrip.numLeds && directionIsForwards) || (pixelPosition < 0 && !directionIsForwards)) {
		return this->getListener()->onFinished(this);
	}
}