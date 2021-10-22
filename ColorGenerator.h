#ifndef ColorGenerator_h
#define ColorGenerator_h

struct Color {
	unsigned char R;
	unsigned char G;
	unsigned char B;
};

class ColorGenerator {
	public:
		virtual ~ColorGenerator() {}
		virtual Color getColor(float rangeValue) = 0;

	protected:
		Color getColorForRange(float rangeValue);
};

#endif