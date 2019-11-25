#ifndef __env_dahdsr_h
#define __env_dahdsr_h

#include "env.h"

/// Delay, attack, hold, decay, sustain, release envelope class
class EnvDAHDSR : public Env {
public:
	/// Create DAHDSR envelope object
	EnvDAHDSR();
	/// Destroy DAHDSR envelope object
	~EnvDAHDSR();
	/// Get envelope type
	/// @return int - value from EnvType enum
	int getType();
	/// Set start amplitude
	/// @param float - start amplitude
	/// @return void
	void setInitialAmp(float);
	/// Set delay time in milliseconds
	/// @param float - delay time
	/// @return void
	void setDelayTime(float);
	/// Set attack time in milliseconds
	/// @param float - attack time
	/// @return void
	void setAttackTime(float);
	/// Set peak amplitude
	/// @param float - peak amplitude
	/// @return void
	void setPeakAmp(float);
	/// Set hold time in milliseconds
	/// @param float - hold time
	/// @return void
	void setHoldTime(float);
	/// Set decay time in milliseconds
	/// @param float - decay time
	/// @return void
	void setDecayTime(float);
	/// Set sustain amplitude
	/// @param float - sustain amplitude
	/// @return void
	void setSustainAmp(float);
	/// Set release time in milliseconds
	/// @param float - release time
	/// @return void
	void setReleaseTime(float);
	/// Set end amplitude
	/// @param float - end amplitude
	/// @return void
	void setEndAmp(float);
	/// Get sample value at given sample position
	/// @param t_Note * - note data
	/// @return float - sample value
	float generateSample(t_Note *);

private:

	float _initialAmp;
	float _delayTime;
	float _attackTime;
	float _peakAmp;
	float _holdTime;
	float _decayTime;
	float _sustainAmp;
	float _releaseTime;
	float _endAmp;

};

#endif
