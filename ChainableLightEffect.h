#ifndef ChainableLightEffect_h
#define ChainableLightEffect_h

#include "LightEffect.h"

class ChainableLightEffect;

class ChainableLightEffectListener {
	public:
	 	virtual void onFinished(ChainableLightEffect* finishedEffect) = 0;
	 	virtual void onFirstElementChanged(ChainableLightEffect* finishedEffect) = 0;
};

class ChainableLightEffect : public LightEffect {
	public:
		ChainableLightEffect(ChainableLightEffectListener* listener);
		virtual ~ChainableLightEffect();
		virtual void setup(LedStripContext &context) = 0;
		virtual void loop(VolumeContext &context) = 0;
		virtual void setNext(ChainableLightEffect* effect);
		virtual void setPrevious(ChainableLightEffect* effect);
		virtual ChainableLightEffect* getNext();
		virtual ChainableLightEffect* getPrevious();
		virtual void add(ChainableLightEffect* effect);
		virtual void runLoop(VolumeContext &context);

	protected:
		ChainableLightEffectListener* getListener();

	private:
		ChainableLightEffect* next = 0;
		ChainableLightEffect* previous = 0;
		ChainableLightEffectListener* listener = 0;
};

#endif