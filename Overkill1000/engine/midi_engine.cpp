#include "midi_engine.h"
#include <stdio.h>
#include <Windows.h>

MidiEngine::MidiEngine(int poliphony){
	int i;
	double k = 1.0594630943592952645618252949463;	// 12th root of 2
	double a = 6.875;	// frequency of A -2
	a = a * k * k * k; // frequency of C -1
	for (i = 0; i < 128; i ++){
		_notes[i].freq = (float)a;
		_notes[i].velocity = 0.0;
		_notes[i].sampleCounter = 0.0;
		_notes[i].releaseCounter = 0.0;
		_notes[i].sampleOffset = 0;
		_notes[i].active = false;
		_notes[i].released = false;
		a *= k; // Step up one semitone
	}

	_poliphony = poliphony;
	_hold = false;
	_active = 0;
	_readActive = 0;
	_readReleased = 0;
}
MidiEngine::~MidiEngine(){

}
void MidiEngine::noteOn(int noteNum, float velocity, int sampleOffset){
	_notes[noteNum].active = true;
	_notes[noteNum].released = false;
	_notes[noteNum].velocity = velocity;
	_notes[noteNum].sampleCounter = 0.0;
	_notes[noteNum].releaseCounter = 0.0;
	_notes[noteNum].sampleOffset = sampleOffset;
}
void MidiEngine::noteOff(int noteNum){
	if (_notes[noteNum].active && !_notes[noteNum].released){
		_notes[noteNum].active = false;
		_notes[noteNum].released = true;
	}
}
void MidiEngine::allOff(){
	int i;
	for (i = 0; i < 128; i ++){
		_notes[i].active = false;
		_notes[i].released = true;
	}
}
void MidiEngine::hold(bool state){
	_hold = state;
}
void MidiEngine::restartReadActive(){
	_readActive = 0;
}
bool MidiEngine::readNextActive(t_Note **note){
	while (!_notes[_readActive].active && _readActive < 128){
		_readActive ++;
	}
	if (_readActive < 128){
		*note = &(_notes[_readActive]);
		_readActive ++;
		return true;
	}
	return false;
}
void MidiEngine::restartReadReleased(){
	_readReleased = 0;
}
bool MidiEngine::readNextReleased(t_Note **note){
	while (!_notes[_readReleased].released && _readReleased < 128){
		_readReleased ++;
	}
	if (_readReleased < 128){
		*note = &(_notes[_readReleased]);
		_readReleased ++;
		return true;
	}
	return false;
}