#include "engine.h"
#include <stdlib.h>
#include <time.h>
#include "types.h"

Engine::Engine(){
	int i = 0;
	srand((unsigned int)time(0));

	double a = 6.875;	// frequency of A -2
	a = a * k12th2 * k12th2 * k12th2; // frequency of C -1
	for (i = 0; i < 128; i ++){
		_freq[i] = (float)a;
		a *= k12th2; // Step up one semitone
	}

	// Set some defaults
	_sampleCounter = 0.0f;
	_beatLength = 4;
	_barLength = 4;
	_tempo = 120.0f;
	_sampleRate = 44100.0;
	_samplesPerBeat = (_sampleRate * 60.f * 60.f) / _tempo; 
	_maxBlockSize = 512;
	_mod = 0.0;
	_pitch = 0.0;
	_volume = (float)0.099999;

	_midi = new MidiBuffer(kMaxPoly);
	_vol = new EnvDAHDSR();
	_oscCount = 0;
	_envCount = 0;
	_fxCount = 0;
}
Engine::~Engine(){
	int i;
	for (i = 0; i < _oscCount; i ++){
		delete _osc[i];
	}
	for (i = 0; i < _envCount; i ++){
		delete _env[i];
	}
	delete _vol;
	delete _midi;
}

int Engine::oscCount(){
	return _oscCount;
}
int Engine::oscAdd(int type){
	if (_oscCount < kMaxOsc){
		Osc *osc;
		Env *vol;
		switch (type){
		case kOscGen:
			osc = new OscGen(kOscTypeSine);
			vol = new EnvDAHDSR();
			osc->setVolEnv(vol);
			break;
		case kOscGranular:
			osc = new OscGranular();
			vol = new EnvDAHDSR();
			osc->setVolEnv(vol);
			break;
		case kOscWTable:
			osc = new OscWTable();
			vol = new EnvDAHDSR();
			osc->setVolEnv(vol);
			break;
		default:
			return -1;
			break;
		}
		osc->setSampleRate(_sampleRate);
		_osc[_oscCount] = osc;
		return _oscCount++;
	}
	return -1;
}
Osc *Engine::oscGet(int id){
	if (id >= 0 && id < _oscCount){
		return _osc[id];
	}
	return NULL;
}
void Engine::oscDelete(int id){
	if (id >= 0 && id < _oscCount){
		delete _osc[id];
		for (;id < _oscCount; id ++){
			_osc[id] = _osc[id + 1];
		}
		_oscCount --;
	}
}

int Engine::envCount(){
	return _envCount;
}
int Engine::envAdd(int type){
	if (_oscCount < kMaxOsc){
		Env *env;
		switch (type){
		case kEnvDAHDSR:
			env = new EnvDAHDSR();
			break;
		default:
			return -1;
			break;
		}
		_env[_envCount] = env;
		return _envCount++;
	}
	return -1;
}
Env *Engine::envGet(int id){
	if (id >= 0 && id < _envCount){
		return _env[id];
	}
	return NULL;
}
void Engine::envDelete(int id){
	if (id >= 0 && id < _envCount){
		delete _env[id];
		for (;id < _envCount; id ++){
			_env[id] = _env[id + 1];
		}
		_envCount --;
	}
}

int Engine::fxCount(){
	return _fxCount;
}
int Engine::fxAdd(int type){
	if (_fxCount < kMaxFx){
		Effect *fx;
		switch (type){
		case kFxDelay:
			fx = new Delay(300.0f);
			((Delay *)fx)->setFeedback(0.8f);
			break;
		case kFxFilter:
			fx = new Filter(kFltLP);
			((Filter *)fx)->setFreq(22000.f);
			((Filter *)fx)->setQ(1.f);
			break;
		default:
			return -1;
			break;
		}
		fx->setSampleRate(_sampleRate);
		_fx[_fxCount] = fx;
		return _fxCount++;
	}
	return -1;
}
Effect *Engine::fxGet(int id){
	if (id >= 0 && id < _fxCount){
		return _fx[id];
	}
	return NULL;
}
void Engine::fxDelete(int id){
	if (id >= 0 && id < _fxCount){
		delete _fx[id];
		for (;id < _fxCount; id ++){
			_fx[id] = _fx[id + 1];
		}
		_fxCount --;
	}
}

void Engine::setTimeSignature(int barLength, int beatLength){
	_barLength = barLength;
	_beatLength = beatLength;
}
void Engine::setTempo(float tempo){
	_tempo = tempo;
	_samplesPerBeat = (_sampleRate * 60.f * 60.f) / _tempo;
}
float Engine::getTempo(){
	return _tempo;
}
void Engine::setSampleRate(float sampleRate){
	int i = 0;
	_sampleRate = sampleRate;
	for (i = 0; i < _oscCount; i ++){
		_osc[i]->setSampleRate(sampleRate);
	}
	for (i = 0; i < _envCount; i ++){
		_env[i]->setSampleRate(sampleRate);
	}
	for (i = 0; i < _fxCount; i ++){
		_fx[i]->setSampleRate(sampleRate);
	}
	_samplesPerBeat = (_sampleRate * 60.f * 60.f) / _tempo;
}
float Engine::getSampleRate(){
	return _sampleRate;
}
void Engine::setBlockSize(int blockSize){
	_maxBlockSize = blockSize;
}
void Engine::noteOn(int noteNum, float velocity, int startOffset){
	t_Note note;
	note.num = noteNum;
	note.freq = _freq[noteNum];
	note.velocity = velocity;
	note.sampleCounter = 0.0f;
	note.phase = kPhaseActive;
	if (_midi->isFull()){
		_midi->remove(0);
	}
	_midi->write(&note);
}
void Engine::noteOff(int noteNum){
	t_Note *rel;
	if (_midi->find(noteNum, &rel)){
		rel->releaseTime = rel->sampleCounter;
		rel->phase = kPhaseRelease;
	}
}
void Engine::allOff(){
	_midi->clear();
}
void Engine::holdOn(){
	//_midi->hold(true);
}
void Engine::holdOff(){
	//_midi->hold(false);
}
void Engine::modWheel(float data){
	_mod = data;
}
void Engine::pitchWheel(float data){
	_pitch = data;
}
