#include "RunningLightEffect.h"
#include "RunningLight.h"
#include "RangeColorGenerator.h"
#include <Arduino.h>

RunningLightEffect::RunningLightEffect() {
}

RunningLightEffect::~RunningLightEffect() {
	if(this->root != 0) {
		delete this->root;
	}
}

void RunningLightEffect::setup(LedStripContext &context) {
}

void RunningLightEffect::loop(VolumeContext &context) {
	context.ledStrip.strip->fill(context.ledStrip.strip->Color(0,0,0), 0, context.ledStrip.numLeds);

	if(context.volume > 0.2 && (this->root == 0 || this->root->getChainLength() < 5) && timestampOfLastEffect + 300 < millis()) {
		RunningLight* light = new RunningLight(0, 80, 5, 1300, new RangeColorGenerator(), this);
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

void RunningLightEffect::onFinished(ChainableLight* finishedLight) {
	finishedLight->remove();
	delete finishedLight;
}

void RunningLightEffect::onFirstElementChanged(ChainableLight* newFirstElement) {
	this->root = newFirstElement;
}