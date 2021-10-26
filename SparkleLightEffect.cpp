#include "SparkleLightEffect.h"
#include "SparkleLight.h"
#include "FixedColorGenerator.h"
#include <Arduino.h>

SparkleLightEffect::SparkleLightEffect() {
}

SparkleLightEffect::~SparkleLightEffect() {
	if(this->root != 0) {
		delete this->root;
	}

	delete this->colorGenerator;
}

void SparkleLightEffect::setup(LedStripContext &context) {
}

void SparkleLightEffect::loop(VolumeContext &context) {
	context.ledStrip.strip->fill(context.ledStrip.strip->Color(0,0,0), 0, context.ledStrip.numLeds);

	if(context.volume > 0.35 && (this->root == 0 || this->root->getChainLength() < 3) && timestampOfLastEffect + 300 < millis()) {
		unsigned int startPosition = floor((1.0 * esp_random() / UINT32_MAX) * context.ledStrip.numLeds);

		SparkleLight* light = new SparkleLight(startPosition, ceil(11 * context.volume), false, new FixedColorGenerator(this->colorGenerator->getColor(1.0)), this);

		if(this->root == 0) {
			this->root = light;
		} else {
			this->root->add(light);
		}

		timestampOfLastEffect = millis();
	}

	if(this->root != 0) {
		this->root->runLoop(context);
	}
}

void SparkleLightEffect::onFinished(ChainableLight* finishedLight) {
	finishedLight->remove();
	delete finishedLight;
}

void SparkleLightEffect::onFirstElementChanged(ChainableLight* newFirstElement) {
	this->root = newFirstElement;
}