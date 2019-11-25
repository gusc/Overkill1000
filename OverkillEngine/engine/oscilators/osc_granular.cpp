#include "osc_granular.h"

/// Create granular oscilator
OscGranular::OscGranular() : OscSample(){
	_grainSize = 128.0;
}
/// Destroy granular oscilator
OscGranular::~OscGranular(){

}
/// Get oscilator type
/// @return int - value from OscType enum
int OscGranular::getType(){
	return kOscGranular;
}
/// Get grain size
/// @return float
float OscGranular::getGrainSize(){
	return _grainSize;
}
/// Set grain size
/// @param float
/// @return void
void OscGranular::setGrainSize(float grainSize){
	_grainSize = grainSize;
}
/// Get sample value at given sample position
/// @param t_Note * - note data
/// @return float - sample value
float OscGranular::generateSample(t_Note *note){
	// TODO: create grain synth here
	return 0.0f;
}
