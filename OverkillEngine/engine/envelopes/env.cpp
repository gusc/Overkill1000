#include "env.h"

/// Create envelope base object
Env::Env() : IGenerator(){
}
/// Destroy envelope base object
Env::~Env(){

}
/// Get envelope type
int Env::getType(){
	return kEnvNone;
}
/// Get sample value at given sample position
/// @param t_Note * - note data
/// @return float - sample value
float Env::generateSample(t_Note *note){
	// Return loud sample
	return 1.0f;
}
