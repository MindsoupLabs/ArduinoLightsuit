#ifndef FixedColorGenerator_h
#define FixedColorGenerator_h

#include "ColorGenerator.h"

class  FixedColorGenerator : public ColorGenerator {
	public:
		FixedColorGenerator(Color color);
		virtual ~FixedColorGenerator();
		virtual Color getColor(float rangeValue);

	private:
		Color color;
};

#endif