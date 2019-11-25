#include "engine.h"
#include <stdlib.h>
#include <time.h>

Engine::Engine(){
	int i = 0;
	
	srand(time(0));

	// Set some defaults
	_sampleRate = 44100.0;
	_maxBlockSize = 512;
	_mod = 0.0;
	_pitch = 0.0;
	_volume = (float)0.099999;

	_midi = new MidiEngine(kMaxPoly);
	for (i = 0; i < kMaxOsc; i ++){
		_osc[i] = new OscGen(kOscTypeSquare);
	}
	for (i = 0; i < kMaxEnv; i ++){
		_env[i] = new EnvDAHDSR();
	}
}
Engine::~Engine(){
	int i;
	for (i = 0; i < kMaxOsc; i ++){
		delete _osc[i];
	}
	for (i = 0; i < kMaxEnv; i ++){
		delete _env[i];
	}
	delete _midi;
}

void Engine::setSampleRate(float sampleRate){
	int i = 0;
	_sampleRate = sampleRate;
	for (i = 0; i < kMaxOsc; i ++){
		_osc[i]->setSampleRate(sampleRate);
	}
	for (i = 0; i < kMaxEnv; i ++){
		_env[i]->setSampleRate(sampleRate);
	}
}
void Engine::setBlockSize(int blockSize){
	_maxBlockSize = blockSize;
}
void Engine::noteOn(int noteNum, float velocity, int startOffset){
	_midi->noteOn(noteNum, velocity, startOffset);
}
void Engine::noteOff(int noteNum){
	_midi->noteOff(noteNum);
}
void Engine::allOff(){
	_midi->allOff();
}
void Engine::holdOn(){
	_midi->hold(true);
}
void Engine::holdOff(){
	_midi->hold(false);
}
void Engine::modWheel(float data){
	_mod = data;
}
void Engine::pitchWheel(float data){
	_pitch = data;
}
