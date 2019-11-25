#include "env_dahdsr.h"

/// Create DAHDSR envelope object
EnvDAHDSR::EnvDAHDSR() : Env(){
	_initialAmp = 0.0f;
	_delayTime = 0.0f;
	_attackTime = 5.0f;
	_peakAmp = 1.0f;
	_holdTime = 0.0f;
	_decayTime = 50.0f;
	_sustainAmp = 0.5f;
	_releaseTime = 300.0f;
	_endAmp = 0.0f;
}
/// Destroy DAHDSR envelope object
EnvDAHDSR::~EnvDAHDSR(){

}
/// Get envelope type
/// @return int - value from EnvType enum
int EnvDAHDSR::getType(){
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
/// Get sample value at given sample position
/// @param t_Note * - note
/// @return float - sample value
float EnvDAHDSR::generateSample(t_Note *note){
	float time = 0.0f;
	float amp = 0.0f;
	if (note->phase == kPhaseActive){
		time = (note->sampleCounter / _sampleRate) * 1000.0f;
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
	} else if (note->phase == kPhaseRelease) {
		time = ((note->sampleCounter - note->releaseTime) / _sampleRate) * 1000.0f;
		if (time <= _releaseTime){
			amp = _sustainAmp + (((_endAmp - _sustainAmp) / _releaseTime) * time);
		} else {
			amp = _endAmp;
			note->phase = kPhaseEnd;
		}
	}
	return amp;
}