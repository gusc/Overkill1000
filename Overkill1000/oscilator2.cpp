#include "oscilator.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>

const static double _pi = 3.1415926535897932384626433832795;

Oscilator::Oscilator(){
	srand(time((time_t)0));
	_sampleRate = 44100.0;
	_type = kOscTypeSine;

	_offset = 0.0;
	_frequency = (float)440.0;
	_width = (float)1.0;
}
Oscilator::~Oscilator(){

}
void Oscilator::setType(int type){
	_type = type;
}
void Oscilator::setSampleRate(float sampleRate){
	_sampleRate = sampleRate;
}
void Oscilator::setOffset(float offset){
	_offset = offset;
}
void Oscilator::setFrequency(float freq){
	_frequency = freq;
}
void Oscilator::setWidth(float width){
	_width = width;
}
float Oscilator::generate(float *buffer, int length){
	float sample = 0.0; // Sampled value for sample & hold
	short pulse_phase = 0; // Phase counter for sample & hold

	float smp_pos; // Current sample position
	float smp_rise; // Angle of triangle wave
	float smp_int = _frequency / _sampleRate;
	float smp_pw = _sampleRate / _frequency; // Pulse width in samples
	float smp_pw_pos = (smp_pw / (float)2.0) * _width; // Width of positive phase
	float smp_pw_neg = (smp_pw / (float)2.0) * ((float)2.0 - _width); // Width of negative phase

	while (--length >= 0){
		smp_pos = _offset / smp_pw;
		switch(_type){
			case kOscTypeSine:
				// Generate sine wave
				if (_width == 1.0){
					(*buffer++) = (float)sin((float)2.0 * _pi * smp_pos * smp_int);
				} else {
					if (smp_pos >= smp_pw_pos){
						(*buffer++) = (float)sin((float)2.0 * _pi * fmod(_offset - smp_pw_pos, smp_pw) * (smp_int / ((float)2.0 - _width))) * (float)(-1.0);
					} else {
						(*buffer++) = (float)sin((float)2.0 * _pi * smp_pos * (smp_int / _width));
					}
				}
				break;
			case kOscTypeSquare:
				// Generate square wave
				if (smp_pos >= smp_pw_pos){
					(*buffer++) = (float)0.999999;
				} else {
					(*buffer++) = (float)-0.999999;
				}
				break;
			case kOscTypeTri:
				// Generate triangular wave
				smp_rise = smp_pos - smp_pw_pos;
				if (smp_pos >= smp_pw_pos){
					(*buffer++) = (float)1.0 - ((float)2.0 * (smp_rise / smp_pw_neg));
				} else {
					(*buffer++) = (float)((float)2.0 * smp_rise) - (float)1.0;
				}
				break;
			case kOscTypeNoise:
				// Generate noise
				// Do the sample & hold
				if (smp_pos >= smp_pw_pos){
					if (pulse_phase != 2){
						sample = ((float)(rand() % SHRT_MAX) / (float)SHRT_MAX) * (float)(-1.0);
					}
					pulse_phase = 2;
					(*buffer++) = sample;
				} else {
					if (pulse_phase != 1){
						sample = (float)(rand() % SHRT_MAX) / (float)SHRT_MAX;
					}
					pulse_phase = 1;
					(*buffer++) = sample;
				} 
				break;
		}
		_offset += (float)1.0;
		_offset = fmod(_offset, smp_pw);
	}
	return _offset;
}
