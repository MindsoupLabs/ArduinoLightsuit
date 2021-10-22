#ifndef TimeColorGenerator_h
#define TimeColorGenerator_h

#include "ColorGenerator.h"

class TimeColorGenerator : public ColorGenerator {
	public:
		TimeColorGenerator(unsigned int period);
		virtual ~TimeColorGenerator();
		virtual Color getColor(float rangeValue);

	private:
		unsigned int period;
};

#endif