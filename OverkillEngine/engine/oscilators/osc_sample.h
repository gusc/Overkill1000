#ifndef __osc_sample_h
#define __osc_sample_h

#include "osc.h"

/// Sample playback oscilator class
class OscSample : public Osc {
public:
	/// Create sample playback oscilator object
	OscSample();
	/// Destroy sample playback oscilator object
	~OscSample();
	/// Get oscilator type
	/// @return int - value from OscType enum
	int getType();
	/// Set wave table buffer
	/// @param float * - wave table sample buffer
	/// @param int - wave table sample buffer size
	/// @return void
	void setBuffer(float *, int);
	/// Get playback start index
	/// @return float
	float getStartIndex();
	/// Set playback start index
	/// @param float
	/// @return void
	void setStartIndex(float);
	/// Get sample value at given sample position
	/// @param t_Note * - note data
	/// @return float - sample value
	float generateSample(t_Note *);

protected:
	/// Internal pointer to sample buffer
	float *_buffer;
	/// Buffer size
	int _bufferSize;
	/// Playback start index
	float _startIndex;

};

#endif
