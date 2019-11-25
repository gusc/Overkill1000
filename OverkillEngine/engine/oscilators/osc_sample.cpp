#include "osc_sample.h"
#include <stdlib.h>
#include <memory.h>

/// Create sample playback oscilator object
OscSample::OscSample(){
	_startIndex = 0.0;
	_bufferSize = 128;
	_buffer = (float *)calloc(128, sizeof(float));
}
/// Destroy sample playback oscilator object
OscSample::~OscSample(){
	_bufferSize = 0;
	free(_buffer);
}
/// Get oscilator type
/// @return int - value from OscType enum
int OscSample::getType(){
	return kOscSample;
}
/// Set wave table buffer
/// @param float * - wave table sample buffer
/// @param int - wave table sample buffer size
/// @return void
void OscSample::setBuffer(float *buffer, int size){
	_bufferSize = 0;
	free(_buffer);
	_buffer = (float *)malloc(sizeof(float) * size);
	memcpy(_buffer, buffer, size * sizeof(float));
	_bufferSize = size;
}
/// Get playback start index
/// @return float
float OscSample::getStartIndex(){
	return _startIndex;
}
/// Set playback start index
/// @param float
/// @return void
void OscSample::setStartIndex(float startIndex){
	_startIndex = startIndex;
}
/// Get sample value at given sample position
/// @param t_Note * - note data
/// @return float - sample value
float OscSample::generateSample(t_Note *note){
	if (_startIndex + note->sampleCounter < _bufferSize){
		return _buffer[((int)(_startIndex + note->sampleCounter))];
	}
	return 0.0;
}
