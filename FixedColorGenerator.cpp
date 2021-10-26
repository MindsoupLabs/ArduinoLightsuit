#include "FixedColorGenerator.h"

FixedColorGenerator::FixedColorGenerator(Color color) {
	this->color.R = color.R;
	this->color.G = color.G;
	this->color.B = color.B;
}

FixedColorGenerator::~FixedColorGenerator() {
}

Color FixedColorGenerator::getColor(float rangeValue) {
	return this->color;
}