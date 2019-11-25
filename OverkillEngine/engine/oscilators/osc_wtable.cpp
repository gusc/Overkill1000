#include "osc_wtable.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

/// Create wavetable oscilator object
OscWTable::OscWTable() : OscSample(){

}
/// Destroy wavetable oscilator object
OscWTable::~OscWTable(){

}
/// Get oscilator type
/// @return int - value from OscType enum
int OscWTable::getType(){
	return kOscWTable;
}
/// Get sample value at given sample position
/// @param t_Note * - note data
/// @return float - sample value
float OscWTable::generateSample(t_Note *note){
	float volume = _getVol(note);
	float freq = _getFreq(note);
	float impulseLength = _sampleRate / freq;
	float samplePosition = fmod(note->sampleCounter, impulseLength);
	int position = 0; // Position in wave table
	float sample = 0.0f; // Sample amplitude
	float interval = (float)_bufferSize / impulseLength; // Table size impulse width sample ratio
	position = (int)(interval * samplePosition) % _bufferSize;
	if (interval > 1.0f){
		// Less samples needed than is (do some averaging from skipped samples)
		{
			int counter = 0;
			int position_prev;
			float samplePos_prev = samplePosition - 1.0f;
			if (samplePos_prev < 0.0f){
				samplePos_prev = impulseLength - (0.0f - samplePos_prev);
			}
			position_prev = (int)(interval * samplePos_prev) % _bufferSize;
			while (position_prev != position && counter < _bufferSize){
				sample += _buffer[position_prev];
				counter ++;
				position_prev = (position_prev + 1) % _bufferSize;
			}
			// Average
			sample = _buffer[position];
			sample = sample / (float)counter;
		}
	} else {
		// More samples needed than is (do some interpolation between samples)
		{
			int position_to = position;
			int position_from = position;
			float samplePos_prev = samplePosition;
			float samplePos_next = samplePosition;
			float count_up = 0.0f;
			float count_down = 0.0f;
			while (position_to == position){
				samplePos_next += 1.0;
				samplePos_next = fmod(samplePos_next, impulseLength);
				count_up += 1.0;
				position_to = (int)(interval * (samplePosition + samplePos_next)) % _bufferSize;
			}
			while (position_from == position){
				samplePos_prev -= 1.0f;
				if (samplePos_prev < 0.0f){
					samplePos_prev = impulseLength - (0.0f - samplePos_prev);
				}
				samplePos_prev = fmod(samplePos_prev, impulseLength);
				count_down += 1.0f;
				position_from = (int)(interval * (samplePosition - count_down)) % _bufferSize;
			}
			// Interpolation
			sample = _buffer[position_from] - (((_buffer[position_from] - _buffer[position_to]) * count_up) / (count_up + count_down));
		}
	}
	samplePosition += 1.0f;
	samplePosition = fmod(samplePosition, impulseLength);
	return (sample * volume);
}
