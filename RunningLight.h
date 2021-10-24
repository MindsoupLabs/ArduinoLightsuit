#ifndef RunningLight_h
#define RunningLight_h

#include "ChainableLight.h"
#include "ColorGenerator.h"

class RunningLight : public ChainableLight {
	public:
		RunningLight(unsigned int startPosition, int ledsPerSecondSpeed, unsigned int size, unsigned int lifetime, ColorGenerator* colorGenerator, ChainableLightListener* listener);
		virtual ~RunningLight();
		virtual void loop(VolumeContext &context);

	private:
		unsigned int startPosition;
		int ledsPerSecondSpeed;
		unsigned int size;
		unsigned int lifetime;
		unsigned long startTime;
		bool directionIsForwards;
		ColorGenerator* colorGenerator = 0;
};

#endif
