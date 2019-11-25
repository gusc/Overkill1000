#include "osc.h"
#include <math.h>

Osc::Osc(){
	_sampleRate = 44100.0;
}
Osc::~Osc(){

}

int Osc::type(){
	return kOscNone;
}
void Osc::setSampleRate(float sampleRate){
	_sampleRate = sampleRate;
}
float Osc::getSample(float samplePosition, float freq){
	// Generate empty sample
	return 0.0;
}