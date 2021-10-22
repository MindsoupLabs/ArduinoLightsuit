#include "RangeColorGenerator.h"

RangeColorGenerator::RangeColorGenerator() {
}

RangeColorGenerator::~RangeColorGenerator() {
}

Color RangeColorGenerator::getColor(float rangeValue) {
	return getColorForRange(rangeValue);
}