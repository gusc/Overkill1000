#include "env_folower.h"

EnvFolower::EnvFolower(Envelope *env){
	_env = env;

	_active = true;
	_end = false;

	_sampleRate = 44100.0;
	_sampleCounter = 0;
}
EnvFolower::~EnvFolower(){

}

void EnvFolower::setSampleRate(float sampleRate){
	_sampleRate = sampleRate;
}
void EnvFolower::setEnvelope(Envelope *env){
	_env = env;
}

bool EnvFolower::isEnd(){
	return _end;
}
void EnvFolower::start(){
	_active = true;
	_sampleCounter = 0;
}
void EnvFolower::release(){
	_active = false;
	_sampleCounter = 0;
}

float EnvFolower::getNextSample(){
	float time = ((float)_sampleCounter / _sampleRate) * 1000.0;
	float vol = 0.0;
	if (_active){
		if (time <= _env->delayTime){
			vol = _env->initialVol;
		} else if (time <= (_env->delayTime + _env->attackTime)){
			vol = _env->initialVol + (((_env->peakVol - _env->initialVol) / _env->attackTime) * (time - _env->delayTime));
		} else if (time <= (_env->delayTime + _env->attackTime + _env->holdTime)){
			vol = _env->peakVol;
		} else if (time <= (_env->delayTime + _env->attackTime + _env->holdTime + _env->decayTime)){
			vol = _env->peakVol + (((_env->sustainVol - _env->peakVol) / _env->decayTime) * (time - _env->delayTime - _env->attackTime - _env->holdTime));
		} else {
			vol = _env->sustainVol;
		}
	} else {
		if (time <= _env->releaseTime){
			vol = _env->sustainVol + (((_env->endVol - _env->sustainVol) / _env->releaseTime) * time);
		} else {
			vol = _env->endVol;
			_end = true;
		}
	}
	_sampleCounter ++;
	return vol;
}