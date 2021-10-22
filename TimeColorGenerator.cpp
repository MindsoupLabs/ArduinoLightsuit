#include "TimeColorGenerator.h"
#include <Arduino.h>

TimeColorGenerator::TimeColorGenerator(unsigned int period) {
	this->period = period;
}

TimeColorGenerator::~TimeColorGenerator() {
}

Color TimeColorGenerator::getColor(float rangeValue) {
	unsigned int remainder = millis() % this->period;
	return getColorForRange((float)remainder / this->period);
}