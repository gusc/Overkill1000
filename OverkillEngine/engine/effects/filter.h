#ifndef __filter_h
#define __filter_h

#include "..\types.h"
#include "effect.h"
#include "engine\buffers\audio_buffer.h"


/// Filter types
enum FilterType {
	kFltLP = 0, // Low-pass
	//kFltLS, // Low-shelf
	kFltBP, // Band-pass
	//kFltHS, // High-shelf
	kFltHP, // High-pass
	kFltNotch, // Notch
	//kFltFormant, // Formant
	//kFltComb // Comb
};

#define MAX_FILTER_STEPS 5

/// Filter effect class
class Filter : public Effect {

public:
	/// Create filter effect
	/// @param int - filter type from FilterType enumeration
	Filter(int);
	/// Destroy filter effect
	~Filter();
	/// Get effect type
	/// @return int - value from FxType enum
	int getType();
	/// Set filter type
	/// @param int - filter type from FilterType enumeration
	/// @return void
	void setFilterType(int);
	/// Get filter type
	/// @return int - filter type from FilterType enumeration
	int getFilterType();
	/// Set frequency: center or cut-off
	/// @param float - frequency
	/// @return void
	void setFreq(float);
	/// Get frequency: center or cut-off
	/// @return float - frequency
	float getFreq();
	/// Set Q factor (resonance)
	/// @param float
	/// @return void
	void setQ(float);
	/// Get Q factor (resonance)
	/// @return float
	float getQ();
	/// Set steps (or poles)
	/// @param int - step/pole count
	/// @return void
	void setSteps(int);
	/// Get steps (or poles)
	/// @return int - step/pole count
	int getSteps();
	/// Process sample value 
	/// @param float *- sample input/output
	/// @return void
	void processSample(float *);

private:

	int _type;
	struct fsteps {
      float low, high, band, notch;
  } _stages[MAX_FILTER_STEPS + 1];
	struct fparams {
      float f, q, q_sqrt;
  } _params, _iparams;
	float _freq;
	float _q;
	int _steps;

	// some garbage
	int _abovenq; //this is 1 if the frequency is above the nyquist
  int _oldabovenq;
  int _needsinterpolation, _firsttime;
	int _intstep;

	void _computeCoef();
	void _cleanup();
	void _filter(float *, int);
};

#endif
