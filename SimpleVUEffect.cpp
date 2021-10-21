#include "SimpleVUEffect.h"

SimpleVUEffect::SimpleVUEffect() {
}

SimpleVUEffect::~SimpleVUEffect() {
}

void SimpleVUEffect::setup() {
}

void SimpleVUEffect::loop(VolumeContext &context) {
	unsigned int lightUpPixels = ceil(context.volume * context.numLeds);

	for(unsigned int i = 0; i < context.numLeds; i++) {
		context.strip->setPixelColor(i, 0, 0, i < lightUpPixels ? 128 : 0);
	}

	context.strip->show();
}