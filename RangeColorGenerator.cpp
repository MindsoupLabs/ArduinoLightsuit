#include "RangeColorGenerator.h"

#include <Arduino.h>

RangeColorGenerator::RangeColorGenerator() {
}

RangeColorGenerator::~RangeColorGenerator() {
}

Color RangeColorGenerator::getColor(float rangeValue) {
	return getColorForRange(rangeValue);
}