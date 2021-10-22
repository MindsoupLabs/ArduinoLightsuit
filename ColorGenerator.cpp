#include "ColorGenerator.h"

Color ColorGenerator::getColorForRange(float rangeValue) {
	Color color;
	float wheelValue = 255 * rangeValue;

	if(wheelValue < 85) {
		color.R = wheelValue * 3;
		color.G = 255 - wheelValue * 3;
		color.B = 0;
	} else if(wheelValue < 170) {
		wheelValue -= 85;
		color.R = 255 - wheelValue * 3;
		color.G = 0;
		color.B = wheelValue * 3;
	} else {
		wheelValue -= 170;
		color.R = 0;
		color.G = wheelValue * 3;
		color.B = 255 - wheelValue * 3;
	}

	return color;
}

