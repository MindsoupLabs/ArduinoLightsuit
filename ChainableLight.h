#ifndef ChainableLight_h
#define ChainableLight_h

#include "VolumeContext.h"

class ChainableLight;

class ChainableLightListener {
	public:
	 	virtual void onFinished(ChainableLight* finishedLight) = 0;
	 	virtual void onFirstElementChanged(ChainableLight* newFirstElement) = 0;
};

class ChainableLight {
	public:
		ChainableLight(ChainableLightListener* listener);
		virtual ~ChainableLight();
		void remove();
		void setNext(ChainableLight* effect);
		void setPrevious(ChainableLight* effect);
		ChainableLight* getNext();
		ChainableLight* getPrevious();
		void add(ChainableLight* effect);
		void runLoop(VolumeContext &context);

		virtual void loop(VolumeContext &context) = 0;

	protected:
		ChainableLightListener* getListener();

	private:
		ChainableLight* next = 0;
		ChainableLight* previous = 0;
		ChainableLightListener* listener = 0;
};

#endif