#ifndef LightEffect_h
#define LightEffect_h

#include "VolumeContext.h"
#include "LedStripContext.h"

class LightEffect {
	public:
		virtual ~LightEffect() {}
		virtual void setup(LedStripContext &context) = 0;
		virtual void loop(VolumeContext &context) = 0;
};

#endif