#include "JitteryMultiMiniVUEffect.h"
#include "CenteredVUMeter.h"
#include "TimeColorGenerator.h"

JitteryMultiMiniVUEffect::JitteryMultiMiniVUEffect() {
}

JitteryMultiMiniVUEffect::~JitteryMultiMiniVUEffect() {
	for(int i = 0; i < NUM_VUMETERS; i++) {
		delete vumeters[i];
	}
}

void JitteryMultiMiniVUEffect::setup(LedStripContext &context) {
	float size = 1.0 / (NUM_VUMETERS * 2);
	for(int i = 0; i < NUM_VUMETERS; i++) {
		vumeters[i] = new CenteredVUMeter(context, (i * 2 + 1) * size * context.numLeds, size, 7000, new TimeColorGenerator(5000));
	}
}

void JitteryMultiMiniVUEffect::loop(VolumeContext &context) {
	if(context.volume > 0.3) {
		positionOffset = positionOffset + ((esp_random() % 5) - 2);
	}
	for(int i = 0; i < NUM_VUMETERS; i++) {
		vumeters[i]->loop(context, positionOffset);
	}
}