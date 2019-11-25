#include "osc_wtable.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

OscWTable::OscWTable(float *waveTable, int tableSize) : Osc(){
	_tableSize = tableSize;
	_waveTable = (float *)malloc(sizeof(float) * tableSize);
	memcpy(_waveTable, waveTable, sizeof(float) * tableSize);
}
OscWTable::~OscWTable(){
	_tableSize = 0;
	free(_waveTable);
	_waveTable = 0;
}

void OscWTable::setWaveTable(float *waveTable, int tableSize){
	_tableSize = 0;
	free(_waveTable);
	_waveTable = 0;
	
	_tableSize = tableSize;
	_waveTable = (float *)malloc(sizeof(float) * tableSize);
	memcpy(_waveTable, waveTable, sizeof(float) * tableSize);
}

float OscWTable::getSample(float samplePosition, float freq){
	float impulseLength = _sampleRate / freq;
	int position = 0; // Position in wave table
	float sample = 0.0; // Sample amplitude
	float interval = (float)_tableSize / impulseLength; // Table size impulse width sample ratio
	position = (int)(interval * samplePosition) % _tableSize;
	if (interval > 1.0){
		// Less samples needed than is (do some averaging from skipped samples)
		{
			int counter = 0;
			int position_prev;
			float samplePos_prev = samplePosition - (float)1.0;
			if (samplePos_prev < 0.0){
				samplePos_prev = impulseLength - ((float)0.0 - samplePos_prev);
			}
			position_prev = (int)(interval * samplePos_prev) % _tableSize;
			while (position_prev != position && counter < _tableSize){
				sample += _waveTable[position_prev];
				counter ++;
				position_prev = (position_prev + 1) % _tableSize;
			}
			// Average
			sample = _waveTable[position];
			sample = sample / (float)counter;
		}
	} else {
		// More samples needed than is (do some interpolation between samples)
		{
			int position_to = position;
			int position_from = position;
			float samplePos_prev = samplePosition;
			float samplePos_next = samplePosition;
			float count_up = 0.0;
			float count_down = 0.0;
			while (position_to == position){
				samplePos_next += 1.0;
				samplePos_next = fmod(samplePos_next, impulseLength);
				count_up += 1.0;
				position_to = (int)(interval * (samplePosition + samplePos_next)) % _tableSize;
			}
			while (position_from == position){
				samplePos_prev -= 1.0;
				if (samplePos_prev < 0.0){
					samplePos_prev = impulseLength - ((float)0.0 - samplePos_prev);
				}
				samplePos_prev = fmod(samplePos_prev, impulseLength);
				count_down += 1.0;
				position_from = (int)(interval * (samplePosition - count_down)) % _tableSize;
			}
			// Interpolation
			sample = _waveTable[position_from] - (((_waveTable[position_from] - _waveTable[position_to]) * count_up) / (count_up + count_down));
		}
	}
	samplePosition += (float)1.0;
	samplePosition = fmod(samplePosition, impulseLength);
	return sample;
}
