#include "SimpleVUEffect.h"
#include "CenteredVUMeter.h"

SimpleVUEffect::SimpleVUEffect() {
}

SimpleVUEffect::~SimpleVUEffect() {
	if(vumeter != 0) {
		delete vumeter;
	}
}

void SimpleVUEffect::setup(LedStripContext &context) {
	vumeter = new CenteredVUMeter(REVERSE, context, 40, 1.0, 5000);
}

void SimpleVUEffect::loop(VolumeContext &context) {
	vumeter->loop(context, 0);
}