#include "SimpleVUEffect.h"
#include "RegularVUMeter.h"
#include "CenteredVUMeter.h"
#include "TimeColorGenerator.h"
#include "RandomColorGenerator.h"
#include "RangeColorGenerator.h"

SimpleVUEffect::SimpleVUEffect() {
}

SimpleVUEffect::~SimpleVUEffect() {
	if(vumeter != 0) {
		delete vumeter;
	}
}

void SimpleVUEffect::setup(LedStripContext &context) {
	//vumeter = new RegularVUMeter(REGULAR, context, 0, 1.0, 5000, new TimeColorGenerator(10000));
	//vumeter = new RegularVUMeter(REGULAR, context, 0, 1.0, 5000, new RangeColorGenerator());
	vumeter = new CenteredVUMeter(context, 43, 0.2, 5000, new RangeColorGenerator());
}

void SimpleVUEffect::loop(VolumeContext &context) {
	vumeter->loop(context, 0);
}