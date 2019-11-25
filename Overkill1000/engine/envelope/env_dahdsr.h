#ifndef __env_dahdsr_h
#define __env_dahdsr_h

#include "env.h"

enum {
	kEnvPhaseActive = 0,
	kEnvPhaseRelease = 1,
	kEnvPhaseEnd = 2
};

class EnvDAHDSR : public Env {
public:
	EnvDAHDSR();
	~EnvDAHDSR();

	int type();

	void setInitialAmp(float initialAmp);
	void setDelayTime(float delayTime);
	void setAttackTime(float attackTime);
	void setPeakAmp(float peakAmp);
	void setHoldTime(float holdTime);
	void setDecayTime(float decayTime);
	void setSustainAmp(float sustainAmp);
	void setReleaseTime(float releaseTime);
	void setEndAmp(float endAmp);

	bool isEnd();
	float getAmplitude(float samplePosition, int phase);

private:
	bool _end;

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