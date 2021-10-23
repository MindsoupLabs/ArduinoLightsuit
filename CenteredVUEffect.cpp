#include "CenteredVUEffect.h"
#include "CenteredVUMeter.h"
#include "RangeColorGenerator.h"

CenteredVUEffect::CenteredVUEffect() {
}

CenteredVUEffect::~CenteredVUEffect() {
	if(vumeter != 0) {
		delete vumeter;
	}
}

void CenteredVUEffect::setup(LedStripContext &context) {
	vumeter = new CenteredVUMeter(context, 43, 1.0, 5000, new RangeColorGenerator());
}

void CenteredVUEffect::loop(VolumeContext &context) {
	vumeter->loop(context, 0);
}