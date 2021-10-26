#include "SparkleLight.h"
#include <Arduino.h>

SparkleLight::SparkleLight(unsigned int startPosition, int size, bool isOutwards, ColorGenerator* colorGenerator, ChainableLightListener* listener) : ChainableLight(listener) {
	this->startPosition = startPosition;
	this->size = max(1, size);
	this->isOutwards = isOutwards;
	this->startTime = millis();
	this->colorGenerator = colorGenerator;
}

SparkleLight::~SparkleLight() {
	if(this->colorGenerator != 0) {
		delete this->colorGenerator;
	}
}

void SparkleLight::loop(VolumeContext &context) {
	int lifetime = millis() - this->startTime;

	if(lifetime > this->lifeTimeMS) {
		return this->getListener()->onFinished(this);
	}

	float lifetimeFactor = (float)min(lifetime, this->lifeTimeMS) / this->lifeTimeMS;
	Color color = this->colorGenerator->getColor(lifetimeFactor);
	lifetimeFactor = 1.0 - lifetimeFactor;

	int ledPosition;
	int distanceFromCenter;
	for(int i = 0; i < size; i++) {
		distanceFromCenter = i - floor(size / 2);
		ledPosition = this->startPosition + distanceFromCenter;
		distanceFromCenter = abs(distanceFromCenter) + 1; // so that center point has value 1 and every LED from there 1 + distance

		if(ledPosition >= context.ledStrip.numLeds) {
			ledPosition = ledPosition % context.ledStrip.numLeds;
		} else if(ledPosition < 0) {
			ledPosition = ledPosition + context.ledStrip.numLeds;
		}

		context.ledStrip.strip->setPixelColor(ledPosition, context.ledStrip.strip->Color(floor(color.R * pow(lifetimeFactor, distanceFromCenter)), floor(color.G * pow(lifetimeFactor, distanceFromCenter)), floor(color.B * pow(lifetimeFactor, distanceFromCenter))));
	}
}