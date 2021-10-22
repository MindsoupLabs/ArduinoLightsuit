#include "SimpleVUEffect.h"

SimpleVUEffect::SimpleVUEffect() {
}

SimpleVUEffect::~SimpleVUEffect() {
	if(vumeter != 0) {
		delete vumeter;
	}
}

void SimpleVUEffect::setup(LedStripContext &context) {
	vumeter = new VUMeter(REGULAR, context, 0, 1.0, 200);
}

void SimpleVUEffect::loop(VolumeContext &context) {
	vumeter->loop(context, 0);
}