#ifndef SimpleVUEffect_h
#define SimpleVUEffect_h

#include "LightEffect.h"

class SimpleVUEffect : public LightEffect {
	public:
		SimpleVUEffect();
		~SimpleVUEffect();
		void setup();
		void loop(VolumeContext &context);

};

#endif