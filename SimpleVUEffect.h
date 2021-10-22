#ifndef SimpleVUEffect_h
#define SimpleVUEffect_h

#include "LightEffect.h"
#include "VUMeter.h"

class SimpleVUEffect : public LightEffect {
	public:
		SimpleVUEffect();
		~SimpleVUEffect();
		void setup(LedStripContext &context);
		void loop(VolumeContext &context);

	private:
		VUMeter* vumeter = 0;
};

#endif