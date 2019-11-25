#ifndef __i_processor_h
#define __i_processor_h

#include "buffers\audio_buffer.h"

/// Sample generator base class
class IProcessor {

public:
	/// Create effect base class
	IProcessor();
	/// Set sample rate
	/// @param float - sample rate
	/// @return void
	virtual void setSampleRate(float);
	/// Get sample rate
	/// @return float - sample rate
	float getSampleRate();
	/// Process sample value 
	/// @param float *- sample input/output
	/// @return void
	virtual void processSample(float *);
	/// Process sample buffer
	/// @param AudioBuffer *- sample buffer
	/// @return void
	virtual void processSampleBuffer(AudioBuffer *);

protected:
	/// Internal sample rate
	float _sampleRate;

};

#endif
