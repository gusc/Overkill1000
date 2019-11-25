#ifndef __osc_wtable_h
#define __osc_wtable_h

#include "osc_sample.h"

/// Wavetable oscilator class
class OscWTable : public OscSample {
public:
	/// Create wavetable oscilator object
	OscWTable();
	/// Destroy wavetable oscilator object
	~OscWTable();
	/// Get oscilator type
	/// @return int - value from OscType enum
	int getType();
	/// Get sample value at given sample position
	/// @param t_Note * - note data
	/// @return float - sample value
	float generateSample(t_Note *);

};

#endif
