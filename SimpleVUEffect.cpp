#include "SimpleVUEffect.h"
#include "RegularVUMeter.h"
#include "RangeColorGenerator.h"

SimpleVUEffect::SimpleVUEffect() {
}

SimpleVUEffect::~SimpleVUEffect() {
	if(vumeter != 0) {
		delete vumeter;
	}
}

void SimpleVUEffect::setup(LedStripContext &context) {
	vumeter = new RegularVUMeter(REVERSE, context, context.numLeds - 1, 1.0, 5000, new RangeColorGenerator(), true);
}

void SimpleVUEffect::loop(VolumeContext &context) {
	vumeter->loop(context, 0);
}