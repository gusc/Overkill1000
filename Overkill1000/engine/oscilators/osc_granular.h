#ifndef __osc_granular_h
#define __osc_granular_h

#include "osc.h"

class OscGranular : public Osc {
public:
	OscGranular(float *buffer, int bufferSize);
	~OscGranular();

	void setBuffer(float *buffer, int bufferSize);

	float getSample(float samplePosition, float freq, float grainSize, float bufferIndex);

private:
	float *_buffer;
	int _bufferSize;
	float _bufferIndex;
	float _grainSize;

};

#endif
