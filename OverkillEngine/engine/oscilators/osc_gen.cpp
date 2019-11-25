#include "osc_gen.h"
#include <math.h>
#include <stdlib.h>
#include "engine\types.h"

/// Create generated oscilator object
OscGen::OscGen(int waveType) : Osc(){
	_waveType = waveType;
	_currentPhase = 0;
	_prevSample = 0.0;
	_pulseWidth = 1.0;
	_pwmOsc = 0;
}
/// Destroy generated oscilator object
OscGen::~OscGen(){

}
/// Get oscilator type
/// @return int - value from OscType enum
int OscGen::getType(){
	return kOscGen;
}
/// Set waveform type
/// @param int - value from OscWaveType enum
/// @return void
void OscGen::setWaveType(int waveType){
	_waveType = waveType;
}
/// Get waveform type
/// @return int - value from OscWaveType enum
int OscGen::getWaveType(){
	return _waveType;
}
/// Set pulse width
/// @param float - pulse width
/// @return void
void OscGen::setPulseWidth(float pulseWidth){
	_pulseWidth = pulseWidth;
}
/// Get pulse width
/// @return float
float OscGen::getPulseWidth(){
	return _pulseWidth;
}
/// Get a pointer to pulse width modulation oscilator
/// @return Osc 
Osc *OscGen::getPWMOsc(){
	return _pwmOsc;
}
/// Set a pointer to pulse width modulation oscilator
/// @param Osc *
/// @return void
void OscGen::setPWMOsc(Osc *pwmOsc){
	_pwmOsc = pwmOsc;
}
/// Get sample value at given sample position
/// @param t_Note * - note data
float OscGen::generateSample(t_Note * note){
	float volume = _getVol(note);
	float freq = _getFreq(note);
	float impulseLength = _sampleRate / freq;
	float samplePosition = fmod(note->sampleCounter, impulseLength);
	float impPerSample = freq / _sampleRate;
	float positiveWidth = (impulseLength / 2.0f) * _pulseWidth;
	float negativeWidth = (impulseLength / 2.0f) * (2.0f - _pulseWidth);
	float sample = 0.0f;
	switch (_waveType){
	case kOscTypeSine:
		if (_pulseWidth == 1.0f){
			sample = (float)sin(2.0f * pi * samplePosition * impPerSample);
		} else {
			if (samplePosition >= positiveWidth){
				sample = (float)sin(2.0f * pi * fmod(samplePosition - positiveWidth, impulseLength) * (impPerSample / (2.0f - _pulseWidth))) * -1.0f;
			} else {
				sample = (float)sin(2.0f * pi * samplePosition * (impPerSample / _pulseWidth));
			}
		}
		break;
	case kOscTypeSquare:
		if (samplePosition >= positiveWidth){
			sample = 0.999999999f;
		} else {
			sample = -0.999999999f;
		}
		break;
	case kOscTypeTri:
		if (samplePosition >= positiveWidth){
			sample =  1.0f - (2.0f * ((samplePosition - positiveWidth) / negativeWidth));
		} else {
			sample = (float)(2.0f * (samplePosition / positiveWidth)) - 1.0f;
		}
		break;
	case kOscTypeNoise:
		if (samplePosition >= positiveWidth){
			if (_currentPhase != -1){
				_prevSample = ((float)(rand() % SHRT_MAX) / (float)SHRT_MAX) * -1.0f;
			}
			_currentPhase = -1;
		} else {
			if (_currentPhase != 1){
				_prevSample = (float)(rand() % SHRT_MAX) / (float)SHRT_MAX;
			}
			_currentPhase = 1;
		}
		sample = _prevSample;
		break;
	}
	return (sample * volume);
}
