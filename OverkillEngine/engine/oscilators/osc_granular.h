#ifndef __osc_granular_h
#define __osc_granular_h

#include "osc_sample.h"

/// Granular oscilator class
class OscGranular : public OscSample {
public:
	/// Create granular oscilator
	OscGranular();
	/// Destroy granular oscilator
	~OscGranular();
	/// Get oscilator type
	/// @return int - value from OscType enum
	int getType();
	/// Get grain size
	/// @return float
	float getGrainSize();
	/// Set grain size
	/// @param float
	/// @return void
	void setGrainSize(float);
	/// Get sample value at given sample position
	/// @param t_Note * - note data
	/// @return float - sample value
	float generateSample(t_Note *);

private:
	/// Grain size
	float _grainSize;

};

#endif
