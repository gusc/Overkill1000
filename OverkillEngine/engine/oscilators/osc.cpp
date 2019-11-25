#include "osc.h"
#include <math.h>

/// Create oscilator base object
Osc::Osc() : IGenerator(){
	_volEnv = 0;
	_fmOsc = 0;
	_pitchEnv = 0;

	_volume = 1.0f;
	_fmAmt = 220.0f;
	_pitch = 0.0f;
}
/// Destroy oscilator base object
Osc::~Osc(){

}
/// Get oscilator type
/// @return int - value from OscType enum
int Osc::getType(){
	return kOscNone;
}
/// Get a pointer to frequency modulation oscilator
/// @return Osc *
Osc *Osc::getFMOsc(){
	return _fmOsc;
}
/// Set a pointer to frequency modulation oscilator
/// @param Osc *
/// @return void
void Osc::setFMOsc(Osc *fmOsc){
	_fmOsc = fmOsc;
}
/// Get a frequency modulation ammount
/// @return float
float Osc::getFMAmt(){
	return _fmAmt;
}
/// Set a frequency modulation ammount
/// @param float
/// @return void
void Osc::setFMAmt(float amt){
	_fmAmt = amt;
}
/// Set volume
/// @param float - volume
/// @return void
void Osc::setVolume(float vol){
	_volume = vol;
}
/// Get volume
/// @return float - volume
float Osc::getVolume(){
	return _volume;
}
/// Get volume envelope object
/// @return Env * - volume envelope object
Env *Osc::getVolEnv(){
	return _volEnv;
}
/// Set volume envelope object
/// @param Env * - volume envelope object
/// @return void
void Osc::setVolEnv(Env *env){
	_volEnv = env;
}
/// Set pitch transposition
/// @param float - pitch
/// @return void
void Osc::setPitch(float pitch){
	_pitch = pitch;
}
/// Get pitch transposition
/// @return float - pitch
float Osc::getPitch(){
	return _pitch;
}
/// Get pitch envelope object
/// @return Env * - pitch envelope object
Env *Osc::getPitchEnv(){
	return _pitchEnv;
}
/// Set pitch envelope object
/// @param Env * - pitch envelope object
/// @return void
void Osc::setPitchEnv(Env *env){
	_pitchEnv = env;
}


/// Get modulated volume
/// @param t_Note * - note data 
/// @return float
float Osc::_getVol(t_Note *note){
	float vol = _volume;
	if (_volEnv != 0){
		vol *= _volEnv->generateSample(note);
	}
	return vol;
}
/// Get modulated frequence
/// @param t_Note * - note data 
/// @return float
float Osc::_getFreq(t_Note *note){
	float pitch = _pitch + (float)note->num;
	if (_pitchEnv != 0){
		pitch *= _pitchEnv->generateSample(note);
	}
	float freq = 440.f * pow(2.f, (pitch - 49.f) / 12);
	if (_fmOsc != 0){
		freq += abs(_fmOsc->generateSample(note) * _fmAmt);
	}
	return freq;
}