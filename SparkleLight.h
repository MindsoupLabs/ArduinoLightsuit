#ifndef SparkleLight_h
#define SparkleLight_h

#include "ChainableLight.h"
#include "ColorGenerator.h"

class SparkleLight : public ChainableLight {
	public:
		SparkleLight(unsigned int startPosition, int size, bool isOutwards, ColorGenerator* colorGenerator, ChainableLightListener* listener);
		virtual ~SparkleLight();
		virtual void loop(VolumeContext &context);

	private:
		const int lifeTimeMS = 400;
		unsigned int startPosition;
		unsigned int size;
		unsigned long startTime;
		bool isOutwards;
		ColorGenerator* colorGenerator = 0;
};

#endif
