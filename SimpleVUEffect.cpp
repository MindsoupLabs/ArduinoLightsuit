#include "SimpleVUEffect.h"

SimpleVUEffect::SimpleVUEffect() {
}

SimpleVUEffect::~SimpleVUEffect() {
}

void SimpleVUEffect::setup(LedStripContext &context) {
}

void SimpleVUEffect::loop(VolumeContext &context) {
	unsigned int lightUpPixels = ceil(context.volume * context.ledStrip.numLeds);

	for(unsigned int i = 0; i < context.ledStrip.numLeds; i++) {
		context.ledStrip.strip->setPixelColor(i, 0, 0, i < lightUpPixels ? 128 : 0);
	}

	context.ledStrip.strip->show();
}