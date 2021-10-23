#ifndef JitteryMultiMiniVUEffect_h
#define JitteryMultiMiniVUEffect_h

#include "LightEffect.h"
#include "VUMeter.h"

class JitteryMultiMiniVUEffect : public LightEffect {
	public:
		JitteryMultiMiniVUEffect();
		~JitteryMultiMiniVUEffect();
		void setup(LedStripContext &context);
		void loop(VolumeContext &context);

	private:
		static const unsigned int NUM_VUMETERS = 4;
		VUMeter* vumeters[NUM_VUMETERS];
		unsigned int positionOffset = 0;
};

#endif