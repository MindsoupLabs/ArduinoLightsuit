#ifndef VUMeter_h
#define VUMeter_h

#include "VolumeContext.h"

class VUMeter {
	public:
		virtual ~VUMeter() {}
		virtual void loop(VolumeContext &context, unsigned int startPositionOffset) = 0;
};

#endif