#include "osc_gen.h"
#include <math.h>
#include <stdlib.h>

static const double _pi = 3.1415926535897932384626433832795;

OscGen::OscGen(int oscType) : Osc(){
	_oscType = oscType;
	_currentPhase = 0;
	_prevSample = 0.0;
}
int OscGen::type(){
	return kOscGen;
}
void OscGen::setOscType(int oscType){
	_oscType = oscType;
}
int OscGen::getOscType(){
	return _oscType;
}

float OscGen::getSample(float samplePosition, float freq, float pulseWidth){
	float impulseLength = _sampleRate / freq;
	samplePosition = fmod(samplePosition, impulseLength);
	float impPerSample = freq / _sampleRate;
	float positiveWidth = (impulseLength / (float)2.0) * pulseWidth;
	float negativeWidth = (impulseLength / (float)2.0) * ((float)2.0 - pulseWidth);
	switch (_oscType){
	case kOscTypeSine:
		if (pulseWidth == 1.0){
			_prevSample = (float)sin((float)2.0 * _pi * samplePosition * impPerSample);
		} else {
			if (samplePosition >= positiveWidth){
				_prevSample = (float)sin((float)2.0 * _pi * fmod(samplePosition - positiveWidth, impulseLength) * (impPerSample / ((float)2.0 - pulseWidth))) * (float)(-1.0);
			} else {
				_prevSample = (float)sin((float)2.0 * _pi * samplePosition * (impPerSample / pulseWidth));
			}
		}
		break;
	case kOscTypeSquare:
		if (samplePosition >= positiveWidth){
			_prevSample = (float)0.999999;
		} else {
			_prevSample = (float)-0.999999;
		}
		break;
	case kOscTypeTri:
		if (samplePosition >= positiveWidth){
			_prevSample =  (float)1.0 - ((float)2.0 * ((samplePosition - positiveWidth) / negativeWidth));
		} else {
			_prevSample = (float)((float)2.0 * (samplePosition / positiveWidth)) - (float)1.0;
		}
		break;
	case kOscTypeNoise:
		if (samplePosition >= positiveWidth){
			if (_currentPhase != -1){
				_prevSample = ((float)(rand() % SHRT_MAX) / (float)SHRT_MAX) * (float)(-1.0);
			}
			_currentPhase = -1;
		} else {
			if (_currentPhase != 1){
				_prevSample = (float)(rand() % SHRT_MAX) / (float)SHRT_MAX;
			}
			_currentPhase = 1;
		}
		break;
	}
	return _prevSample;
}
