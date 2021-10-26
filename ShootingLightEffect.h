#ifndef ShootingLightEffect_h
#define ShootingLightEffect_h

#include "LightEffect.h"
#include "RunningLight.h"

class ShootingLightEffect : public LightEffect, public ChainableLightListener {
	public:
		ShootingLightEffect();
		~ShootingLightEffect();
		void setup(LedStripContext &context);
		void loop(VolumeContext &context);
		virtual void onFinished(ChainableLight* finishedLight);
		virtual void onFirstElementChanged(ChainableLight* newFirstElement);

	private:
		ChainableLight* root = 0;
		unsigned long timestampOfLastEffect = 0;
};

#endif