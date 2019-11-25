#ifndef __osc_gen_h
#define __osc_gen_h

#include "osc.h"

enum {
	kOscTypeSine = 0,
	kOscTypeSquare,
	kOscTypeTri,
	kOscTypeNoise,
	kMaxOscType
};

class OscGen : public Osc {
public:
	OscGen(int oscType);
	
	int type();
	void setOscType(int oscType);
	int getOscType();
	
	float getSample(float samplePosition, float freq, float pulseWidth);

protected:
	int _oscType;

	float _prevSample;
	int _currentPhase;
};


#endif
