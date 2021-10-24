#ifndef RunningLightEffect_h
#define RunningLightEffect_h

#include "LightEffect.h"
#include "RunningLight.h"

class RunningLightEffect : public LightEffect, public ChainableLightListener {
	public:
		RunningLightEffect();
		~RunningLightEffect();
		void setup(LedStripContext &context);
		void loop(VolumeContext &context);
		virtual void onFinished(ChainableLight* finishedLight);
		virtual void onFirstElementChanged(ChainableLight* newFirstElement);

	private:
		ChainableLight* root = 0;
};

#endif