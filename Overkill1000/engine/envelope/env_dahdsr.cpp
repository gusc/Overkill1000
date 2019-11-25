#include "env_dahdsr.h"

EnvDAHDSR::EnvDAHDSR() : Env(){
	_end = false;

	_initialAmp = 0.0;
	_delayTime = 0.0;
	_attackTime = 5.0;
	_peakAmp = 1.0;
	_holdTime = 0.0;
	_decayTime = 50.0;
	_sustainAmp = 0.5;
	_releaseTime = 50.0;
	_endAmp = 0.0;
}
EnvDAHDSR::~EnvDAHDSR(){

}

int EnvDAHDSR::type(){
	return kEnvDAHDSR;
}

void EnvDAHDSR::setInitialAmp(float initialAmp){
	_initialAmp = initialAmp;
}
void EnvDAHDSR::setDelayTime(float delayTime){
	_delayTime = delayTime;
}
void EnvDAHDSR::setAttackTime(float attackTime){
	_attackTime = attackTime;
}
void EnvDAHDSR::setPeakAmp(float peakAmp){
	_peakAmp = peakAmp;
}
void EnvDAHDSR::setHoldTime(float holdTime){
	_holdTime = holdTime;
}
void EnvDAHDSR::setDecayTime(float decayTime){
	_decayTime = decayTime;
}
void EnvDAHDSR::setSustainAmp(float sustainAmp){
	_sustainAmp = sustainAmp;
}
void EnvDAHDSR::setReleaseTime(float releaseTime){
	_releaseTime = releaseTime;
}
void EnvDAHDSR::setEndAmp(float endAmp){
	_endAmp = endAmp;
}

bool EnvDAHDSR::isEnd(){
	return _end;
}
float EnvDAHDSR::getAmplitude(float samplePosition, int phase){
	_end = false;

	float time = (samplePosition / _sampleRate) * (float)1000.0;
	float amp = 0.0;
	if (phase == kEnvPhaseActive){
		if (time <= _delayTime){
			amp = _initialAmp;
		} else if (time <= (_delayTime + _attackTime)){
			amp = _initialAmp + (((_peakAmp - _initialAmp) / _attackTime) * (time - _delayTime));
		} else if (time <= (_delayTime + _attackTime + _holdTime)){
			amp = _peakAmp;
		} else if (time <= (_delayTime + _attackTime + _holdTime + _decayTime)){
			amp = _peakAmp + (((_sustainAmp - _peakAmp) / _decayTime) * (time - _delayTime - _attackTime - _holdTime));
		} else {
			amp = _sustainAmp;
		}
	} else {
		if (time <= _releaseTime){
			amp = _sustainAmp + (((_endAmp - _sustainAmp) / _releaseTime) * time);
		} else {
			amp = _endAmp;
			_end = true;
		}
	}
	return amp;
}