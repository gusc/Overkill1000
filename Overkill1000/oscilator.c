#include "oscilator.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>

const static double _pi = 3.1415926535897932384626433832795;

float osc_sine(float *buffer, int bufferSize, float samplePos, float impulseLength, float pulseWidth){
	float impPerSmp = (float)1.0 / impulseLength;
	float phasePosW = (impulseLength / (float)2.0) * pulseWidth; // Width of positive phase
	
	while (--bufferSize >= 0){
		if (pulseWidth == 1.0){
			(*buffer++) = (float)sin((float)2.0 * _pi * samplePos * impPerSmp);
		} else {
			if (samplePos >= phasePosW){
				(*buffer++) = (float)sin((float)2.0 * _pi * fmod(samplePos - phasePosW, impulseLength) * (impPerSmp / ((float)2.0 - pulseWidth))) * (float)(-1.0);
			} else {
				(*buffer++) = (float)sin((float)2.0 * _pi * samplePos * (impPerSmp / pulseWidth));
			}
		}
		samplePos += (float)1.0;
		samplePos = fmod(samplePos, impulseLength);
	}
	return samplePos;
}

float osc_square(float *buffer, int bufferSize, float samplePos, float impulseLength, float pulseWidth){
	float phasePosW = (impulseLength / (float)2.0) * pulseWidth; // Width of positive phase
	
	while (--bufferSize >= 0){
		if (samplePos >= phasePosW){
			(*buffer++) = (float)0.999999;
		} else {
			(*buffer++) = (float)-0.999999;
		}
		samplePos += (float)1.0;
		samplePos = fmod(samplePos, impulseLength);
	}
	return samplePos;
}

float osc_tri(float *buffer, int bufferSize, float samplePos, float impulseLength, float pulseWidth){
	float phasePosW = (impulseLength / (float)2.0) * pulseWidth; // Width of positive phase
	float phaseNegW = (impulseLength / (float)2.0) * ((float)2.0 - pulseWidth); // Width of negative phase
	
	while (--bufferSize >= 0){
		if (samplePos >= phasePosW){
			(*buffer++) = (float)1.0 - ((float)2.0 * ((samplePos - phasePosW) / phaseNegW));
		} else {
			(*buffer++) = (float)((float)2.0 * (samplePos / phasePosW)) - (float)1.0;
		}
		samplePos += (float)1.0;
		samplePos = fmod(samplePos, impulseLength);
	}
	return samplePos;
}

float osc_noise(float *buffer, int bufferSize, float samplePos, float impulseLength, float pulseWidth){
	float sample = 0.0; // Sampled value for sample & hold
	short phase = 0; // Phase counter for sample & hold
	float phasePosW = (impulseLength / (float)2.0) * pulseWidth; // Width of positive phase
	
	while (--bufferSize >= 0){
		if (samplePos >= phasePosW){
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
		samplePos = fmod(samplePos, impulseLength);
	}
	return samplePos;
}

float osc_wavetable(float *buffer, int bufferSize, float samplePos, float impulseLength, float *table, int tableSize){
	int position = 0; // Position in wave table
	float sample = 0.0; // Sample amplitude
	float interval = (float)tableSize / impulseLength; // Table size impulse width sample ratio
	while (--bufferSize >= 0){
		position = (int)(interval * samplePos) % tableSize;
		if (interval > 1.0){
			// Less samples needed than is (do some averaging from skipped samples)
			{
				int counter = 0;
				int position_prev;
				float samplePos_prev = samplePos - 1.0;
				if (samplePos_prev < 0.0){
					samplePos_prev = impulseLength - (0.0 - samplePos_prev);
				}
				position_prev = (int)(interval * samplePos_prev) % tableSize;
				while (position_prev != position && counter < tableSize){
					sample += table[position_prev];
					counter ++;
					position_prev = (position_prev + 1) % tableSize;
				}
				// Average
				sample = table[position];
				sample = sample / (float)counter;
			}
		} else {
			// More samples needed than is (do some interpolation between samples)
			{
				int position_to = position;
				int position_from = position;
				float samplePos_prev = samplePos;
				float samplePos_next = samplePos;
				float count_up = 0.0;
				float count_down = 0.0;
				while (position_to == position){
					samplePos_next += 1.0;
					samplePos_next = fmod(samplePos_next, impulseLength);
					count_up += 1.0;
					position_to = (int)(interval * (samplePos + samplePos_next)) % tableSize;
				}
				while (position_from == position){
					samplePos_prev -= 1.0;
					if (samplePos_prev < 0.0){
						samplePos_prev = impulseLength - (0.0 - samplePos_prev);
					}
					samplePos_prev = fmod(samplePos_prev, impulseLength);
					count_down += 1.0;
					position_from = (int)(interval * (samplePos - count_down)) % tableSize;
				}
				// Interpolation
				sample = table[position_from] - (((table[position_from] - table[position_to]) * count_up) / (count_up + count_down));
			}
		}
		(*buffer++) = sample;
		samplePos += (float)1.0;
		samplePos = fmod(samplePos, impulseLength);
	}
	return samplePos;
}
