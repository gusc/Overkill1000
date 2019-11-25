#include "types.h"
#include "iprocessor.h"

/// Create sample generator base class
IProcessor::IProcessor(){
	_sampleRate = 44100.0;
}
/// Set sample rate
/// @param float - sample rate
/// @return void
void IProcessor::setSampleRate(float sampleRate){
	_sampleRate = sampleRate;
}
/// Get sample rate
/// @return float - sample rate
float IProcessor::getSampleRate(){
	return _sampleRate;
}
/// Process sample value 
/// @param float *- sample input/output
/// @return void
void IProcessor::processSample(float *sample){

}
/// Process sample buffer
/// @param AudioBuffer *- sample buffer
/// @return void
void IProcessor::processSampleBuffer(AudioBuffer *buffer){
	float *sample;
	buffer->restart();
	while (buffer->read(&sample)){
		processSample(sample);
	}
}
