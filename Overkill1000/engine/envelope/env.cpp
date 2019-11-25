#include "env.h"

Env::Env(){
	_sampleRate = 44100.0;
}
Env::~Env(){

}
int Env::type(){
	return kEnvNone;
}
void Env::setSampleRate(float sampleRate){
	_sampleRate = sampleRate;
}

float Env::getAmplitude(float samplePosition){
	return 1.0;
}