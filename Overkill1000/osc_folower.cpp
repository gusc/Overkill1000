#include "osc_folower.h"
#include <math.h>

OscFolower::OscFolower(){
	_samplePosition = 0.0;
}
OscFolower::~OscFolower(){

}

void OscFolower::setSampleRate(float sampleRate){
	_sampleRate = sampleRate;
}
void OscFolower::setSampleOffset(int sampleOffset){
	_sampleOffset = sampleOffset;
}
int OscFolower::getSampleOffset(){
	return _sampleOffset;
}
void OscFolower::start(){
	_samplePosition = 0.0;
}
float OscFolower::getNextSample(float freq){
	float samplePos = _samplePosition;
	float impulseLength = _sampleRate / freq;
	_samplePosition += 1.0;
	_samplePosition = fmod(_samplePosition, impulseLength);
	return samplePos;
}
