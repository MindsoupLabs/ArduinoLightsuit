#include "RandomColorGenerator.h"
#include <Arduino.h>

RandomColorGenerator::RandomColorGenerator() {
}

RandomColorGenerator::~RandomColorGenerator() {
}

Color RandomColorGenerator::getColor(float rangeValue) {
	return getColorForRange(1.0 * esp_random() / UINT32_MAX);
}