#ifndef __i_generator_h
#define __i_generator_h

#include "types.h"

/// Sample generator base class
class IGenerator {

public:
	/// Create sample generator base class
	IGenerator();
	/// Set sample rate
	/// @param float - sample rate
	/// @return void
	virtual void setSampleRate(float);
	/// Get sample rate
	/// @return float - sample rate
	float getSampleRate();
	/// Get sample value at given sample position
	/// @param t_Note * - note data
	/// @return float - sample value
	virtual float generateSample(t_Note *);

protected:
	/// Internal sample rate
	float _sampleRate;

};

#endif
