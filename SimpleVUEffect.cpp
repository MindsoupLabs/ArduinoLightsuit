#include "SimpleVUEffect.h"
#include "RegularVUMeter.h"
#include "RangeColorGenerator.h"
#include "LimitedRangeColorGenerator.h"

SimpleVUEffect::SimpleVUEffect() {
}

SimpleVUEffect::~SimpleVUEffect() {
	if(vumeter != 0) {
		delete vumeter;
	}
}

void SimpleVUEffect::setup(LedStripContext &context) {
	vumeter = new RegularVUMeter(REVERSE, context, context.numLeds - 1, 1.0, 5000, new LimitedRangeColorGenerator(0.09, 0.25), true);
}

void SimpleVUEffect::loop(VolumeContext &context) {
	vumeter->loop(context, 0);
}