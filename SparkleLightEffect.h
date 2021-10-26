#ifndef SparkleLightEffect_h
#define SparkleLightEffect_h

#include "LightEffect.h"
#include "SparkleLight.h"
#include "TimeColorGenerator.h"

class SparkleLightEffect : public LightEffect, public ChainableLightListener {
	public:
		SparkleLightEffect();
		~SparkleLightEffect();
		void setup(LedStripContext &context);
		void loop(VolumeContext &context);
		virtual void onFinished(ChainableLight* finishedLight);
		virtual void onFirstElementChanged(ChainableLight* newFirstElement);

	private:
		ChainableLight* root = 0;
		ColorGenerator* colorGenerator = new TimeColorGenerator(7000);
		unsigned long timestampOfLastEffect = 0;
};

#endif