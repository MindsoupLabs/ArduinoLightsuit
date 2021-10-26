#ifndef LimitedRangeColorGenerator_h
#define LimitedRangeColorGenerator_h

#include "ColorGenerator.h"

class LimitedRangeColorGenerator : public ColorGenerator {
	public:
		LimitedRangeColorGenerator(float rangeWidth, float start);
			// examples:
			// new LimitedRangeColorGenerator(0.09, 0.25) for yellow/red (fire)
		virtual ~LimitedRangeColorGenerator();
		virtual Color getColor(float rangeValue);

	private:
		float width;
		float start;
};

#endif