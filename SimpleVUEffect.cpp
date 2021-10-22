#include "SimpleVUEffect.h"

SimpleVUEffect::SimpleVUEffect() {
}

SimpleVUEffect::~SimpleVUEffect() {
	if(vumeter != 0) {
		delete vumeter;
	}
}

void SimpleVUEffect::setup(LedStripContext &context) {
	vumeter = new VUMeter(REVERSE, context, 0, 1.0, 3000);
}

void SimpleVUEffect::loop(VolumeContext &context) {
	vumeter->loop(context, 0);
}