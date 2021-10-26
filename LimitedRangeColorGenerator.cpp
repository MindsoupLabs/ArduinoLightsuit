#include "LimitedRangeColorGenerator.h"
#include <Arduino.h>

LimitedRangeColorGenerator::LimitedRangeColorGenerator(float rangeWidth, float start) {
	this->width = max(0.01, fmod(rangeWidth, 1.0));
	this->start = max(0.0, fmod(start, 1.0));
}

LimitedRangeColorGenerator::~LimitedRangeColorGenerator() {
}

Color LimitedRangeColorGenerator::getColor(float rangeValue) {
	return getColorForRange(this->start + (this->width * rangeValue));
}