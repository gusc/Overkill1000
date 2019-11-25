#include "filter.h"
#include <math.h>

/// Create filter effect
/// @param int - filter type from FilterType enumeration
Filter::Filter(int type) : Effect() {
	_type = type;
	_q = 1.f;
	_freq = 22000.f;
	_steps = 5;
	_needsinterpolation = 0;
	_firsttime = 0;
	_cleanup();
	_computeCoef();
}
/// Destroy filter effect
Filter::~Filter(){
	
}
/// Get effect type
/// @return int - value from FxType enum
int Filter::getType(){
	return kFxFilter;
}
/// Set filter type
/// @param int - filter type from FilterType enumeration
/// @return void
void Filter::setFilterType(int type){
	_type = type;
}
/// Get filter type
/// @return int - filter type from FilterType enumeration
int Filter::getFilterType(){
	return _type;
}
/// Set frequency: center or cut-off
/// @param float - frequency
/// @return void
void Filter::setFreq(float freq){
	if(freq < 0.1f){
    freq = 0.1f;
	}
  
	float rap = _freq / freq;
  if(rap < 1.0f){
		rap = 1.0f / rap;
	}
  _oldabovenq = _abovenq;
  _abovenq = freq > (_sampleRate / 2.0f - 500.0f);
	int nyquistthresh = (_abovenq ^ _oldabovenq);
	if((rap > 3.0f) || (nyquistthresh != 0)) { //if the frequency is changed fast, it needs interpolation (now, filter and coeficients backup)
		if(_firsttime == 0){
			_needsinterpolation = 1;
			_intstep = 0;
		}
		_iparams = _params;
  }
  _freq = freq;
	_computeCoef();
  _firsttime = 0;
}
/// Get frequency: center or cut-off
/// @return float - frequency
float Filter::getFreq(){
	return _freq;
}
/// Set Q factor (resonance)
/// @param float
/// @return void
void Filter::setQ(float q){
	_q = q;
	_computeCoef();
}
/// Get Q factor (resonance)
/// @return float
float Filter::getQ(){
	return _q;
}
/// Set steps (or poles)
/// @param int - step/pole count
/// @return void
void Filter::setSteps(int steps){
	if(steps >= MAX_FILTER_STEPS){
		steps = MAX_FILTER_STEPS;
	}
  _steps = steps;
	_cleanup();
	_computeCoef();
}
/// Get steps (or poles)
/// @return int - step/pole count
int Filter::getSteps(){
	return _steps;
}
/// Process sample value 
/// @param float *- sample input/output
/// @return void
void Filter::processSample(float *sample){
	int i;
	float isample;
	fparams tmp = _params;
	_params = _iparams;
	if(_needsinterpolation != 0) {
    isample = *sample;
    for(i = 0; i < _steps + 1; i++){
			_filter(&isample, i);
		}
  }
	_params = tmp;
	for (int i = 0; i < _steps; i ++){
		_filter(sample, i);
	}
	if(_needsinterpolation != 0) {
		float x = (float)_intstep / 128.f;
		_intstep ++;
    *sample = isample * (1.0 - x) + (*sample) * x;
		if (_intstep > 128){
			_needsinterpolation = 0;
		}
  }
}

void Filter::_filter(float *sample, int step){
	float *out = 0;
	switch (_type){
		case kFltBP:
			out = &_stages[step].band;
			break;
		case kFltLP:
			out = &_stages[step].low;
			break;
		case kFltHP:
			out = &_stages[step].high;
			break;
		case kFltNotch:
			out = &_stages[step].notch;
			break;
	}
	_stages[step].low   = _stages[step].low + _params.f * _stages[step].band;
	_stages[step].high  = _params.q_sqrt * (*sample) - _stages[step].low - _params.q * _stages[step].band;
	_stages[step].band  = _params.f * _stages[step].high + _stages[step].band;
	_stages[step].notch = _stages[step].high + _stages[step].low;
	*sample = *out;
}
void Filter::_computeCoef(){
	_params.f = _freq / _sampleRate * 4.0f;
  if(_params.f > 0.99999){
      _params.f = 0.99999;
	}
  _params.q = 1.0 - atan(sqrt(_q)) * 2.0 / pi;
  _params.q = pow(_params.q, 1.0f / (float)(_steps + 1));
  _params.q_sqrt = sqrt(_params.q);
}
void Filter::_cleanup(){
	for(int i = 0; i < MAX_FILTER_STEPS + 1; i++) {
			_stages[i].low   = 0.0;
			_stages[i].high  = 0.0;
			_stages[i].band  = 0.0;
			_stages[i].notch = 0.0;
	}
	_oldabovenq = 0;
	_abovenq = 0;
}
