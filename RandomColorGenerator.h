#ifndef RandomColorGenerator_h
#define RandomColorGenerator_h

#include "ColorGenerator.h"

class RandomColorGenerator : public ColorGenerator {
	public:
		RandomColorGenerator();
		virtual ~RandomColorGenerator();
		virtual Color getColor(float rangeValue);
};

#endif