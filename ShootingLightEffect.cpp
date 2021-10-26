#include "ShootingLightEffect.h"
#include "RunningLight.h"
#include "TimeColorGenerator.h"

ShootingLightEffect::ShootingLightEffect() {
}

ShootingLightEffect::~ShootingLightEffect() {
	if(this->root != 0) {
		delete this->root;
	}
}

void ShootingLightEffect::setup(LedStripContext &context) {
}

void ShootingLightEffect::loop(VolumeContext &context) {
	context.ledStrip.strip->fill(context.ledStrip.strip->Color(0,0,0), 0, context.ledStrip.numLeds);

	if(context.volume > 0.2 && (this->root == 0 || this->root->getChainLength() < 4)  && timestampOfLastEffect + 300 < millis()) {
		RunningLight* light = new RunningLight(context.ledStrip.numLeds - 1, -80, 4, floor(1200 * context.volume), new TimeColorGenerator(10000), this);
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

void ShootingLightEffect::onFinished(ChainableLight* finishedLight) {
	finishedLight->remove();
	delete finishedLight;
}

void ShootingLightEffect::onFirstElementChanged(ChainableLight* newFirstElement) {
	this->root = newFirstElement;
}