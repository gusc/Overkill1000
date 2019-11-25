#include "midi_note.h"
#include <math.h>

MidiNote::MidiNote(int noteNum, float velocity){

}

void MidiNote::setSampleRate(float sampleRate){
	_sampleRate = sampleRate;
}
void MidiNote::setEnvelope(Envelope *env){
	_env = env;
}

bool MidiNote::isEnd(){
	return _end;
}
void MidiNote::start(){
	_active = true;
	_sampleCounter = 0;
}
void MidiNote::tick(){
	_sampleCounter += 1.0;
}
void MidiNote::release(){
	_active = false;
	_sampleCounter = 0;
}

float MidiNote::getFreq(){

}
float MidiNote::getNextSamplePos(){
	float impulseLength = _sampleRate / _freq;
	return fmod(_sampleCounter, impulseLength);
}
float MidiNote::getNextVelocity(){
	float time = (_sampleCounter / _sampleRate) * 1000.0;
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
	return vol;
}