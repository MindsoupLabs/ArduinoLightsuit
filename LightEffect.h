#ifndef LightEffect_h
#define LightEffect_h

#include "VolumeContext.h"

class LightEffect {
	public:
		virtual ~LightEffect() {}
		virtual void setup() = 0;
		virtual void loop(VolumeContext &context) = 0;
};

#endif