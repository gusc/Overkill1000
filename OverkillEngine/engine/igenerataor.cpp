#include "types.h"
#include "igenerator.h"

/// Create sample generator base class
IGenerator::IGenerator(){
	_sampleRate = 44100.0;
}
/// Set sample rate
/// @param float - sample rate
/// @return void
void IGenerator::setSampleRate(float sampleRate){
	_sampleRate = sampleRate;
}
/// Get sample rate
/// @return float - sample rate
float IGenerator::getSampleRate(){
	return _sampleRate;
}
/// Get sample value at given sample position
/// @param t_Note *- note data
/// @return float - sample value
float IGenerator::generateSample(t_Note * note){
	// return flat sample
	return 0.0f;
}
