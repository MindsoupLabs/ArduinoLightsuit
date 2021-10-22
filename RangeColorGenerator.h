#ifndef RangeColorGenerator_h
#define RangeColorGenerator_h

#include "ColorGenerator.h"

class RangeColorGenerator : public ColorGenerator {
	public:
		RangeColorGenerator();
		virtual ~RangeColorGenerator();
		virtual Color getColor(float rangeValue);
};

#endif