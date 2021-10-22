#ifndef CenteredVUEffect_h
#define CenteredVUEffect_h

#include "LightEffect.h"
#include "VUMeter.h"

class CenteredVUEffect : public LightEffect {
	public:
		CenteredVUEffect();
		~CenteredVUEffect();
		void setup(LedStripContext &context);
		void loop(VolumeContext &context);

	private:
		VUMeter* vumeter = 0;
};

#endif