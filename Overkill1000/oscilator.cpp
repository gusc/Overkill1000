#include "oscilator.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>

const static double _pi = 3.1415926535897932384626433832795;

static float _sampleRate = 44100.0;

void osc_sampleRate(float sampleRate){
	_sampleRate = sampleRate;
}

float osc_sine(float *buffer, int sampleCount, float samplePos, float freq, float width){
	float smp_int = freq / _sampleRate;
	float smp_pw = _sampleRate / freq; // Pulse width in samples
	float smp_pw_pos = (smp_pw / (float)2.0) * width; // Width of positive phase
	
	while (--sampleCount >= 0){
		if (width == 1.0){
			(*buffer++) = (float)sin((float)2.0 * _pi * samplePos * smp_int);
		} else {
			if (samplePos >= smp_pw_pos){
				(*buffer++) = (float)sin((float)2.0 * _pi * fmod(samplePos - smp_pw_pos, smp_pw) * (smp_int / ((float)2.0 - width))) * (float)(-1.0);
			} else {
				(*buffer++) = (float)sin((float)2.0 * _pi * samplePos * (smp_int / width));
			}
		}
		samplePos += (float)1.0;
		samplePos = fmod(samplePos, smp_pw);
	}
	return samplePos;
}

float osc_square(float *buffer, int sampleCount, float samplePos, float freq, float width){
	float smp_pw = _sampleRate / freq; // Pulse width in samples
	float smp_pw_pos = (smp_pw / (float)2.0) * width; // Width of positive phase
	
	while (--sampleCount >= 0){
		if (samplePos >= smp_pw_pos){
			(*buffer++) = (float)0.999999;
		} else {
			(*buffer++) = (float)-0.999999;
		}
		samplePos += (float)1.0;
		samplePos = fmod(samplePos, smp_pw);
	}
	return samplePos;
}

float osc_tri(float *buffer, int sampleCount, float samplePos, float freq, float width){
	float smp_pw = _sampleRate / freq; // Pulse width in samples
	float smp_pw_pos = (smp_pw / (float)2.0) * width; // Width of positive phase
	float smp_pw_neg = (smp_pw / (float)2.0) * ((float)2.0 - width); // Width of negative phase
	
	while (--sampleCount >= 0){
		if (samplePos >= smp_pw_pos){
			(*buffer++) = (float)1.0 - ((float)2.0 * ((samplePos - smp_pw_pos) / smp_pw_neg));
		} else {
			(*buffer++) = (float)((float)2.0 * (samplePos / smp_pw_pos)) - (float)1.0;
		}
		samplePos += (float)1.0;
		samplePos = fmod(samplePos, smp_pw);
	}
	return samplePos;
}

float osc_noise(float *buffer, int sampleCount, float samplePos, float freq, float width){
	float sample = 0.0; // Sampled value for sample & hold
	short phase = 0; // Phase counter for sample & hold
	float smp_pw = _sampleRate / freq; // Pulse width in samples
	float smp_pw_pos = (smp_pw / (float)2.0) * width; // Width of positive phase
	
	while (--sampleCount >= 0){
		if (samplePos >= smp_pw_pos){
			if (phase != 2){
				sample = ((float)(rand() % SHRT_MAX) / (float)SHRT_MAX) * (float)(-1.0);
			}
			phase = 2;
			(*buffer++) = sample;
		} else {
			if (phase != 1){
				sample = (float)(rand() % SHRT_MAX) / (float)SHRT_MAX;
			}
			phase = 1;
			(*buffer++) = sample;
		} 
		samplePos += (float)1.0;
		samplePos = fmod(samplePos, smp_pw);
	}
	return samplePos;
}

float osc_wavetable(float *buffer, int sampleCount, float samplePos, float freq, float *table, int tableSize){
	float sample = 0.0;
	float sample2;
	int position = 0;
	float smp_pw = _sampleRate / freq; // Pulse width in samples
	float interval = (float)tableSize / smp_pw; // Table size pulse width sample ratio
	while (--sampleCount >= 0){
		position = (int)(interval * samplePos);
		sample = table[position];
		if (interval > 1.0){
			// Less samples needed
			
		} else {
			// Interpolation needed
			if (position == 0 && position < tableSize){
				sample2 = table[tableSize - 1];
			} else {
				sample2 = table[position - 1];
			}
			sample = (sample + sample2) / 2.0;
		}
		(*buffer++) = sample;
		samplePos += (float)1.0;
		samplePos = fmod(samplePos, smp_pw);
	}
	return samplePos;
}
