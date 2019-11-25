#ifndef __env_h
#define __env_h

enum {
	kEnvNone = 0,
	kEnvDAHDSR = 1,
	kEnvStepSeq = 2
};

class Env {
public:
	Env();
	~Env();

	virtual int type();
	
	void setSampleRate(float sampleRate);

	virtual float getAmplitude(float samplePosition);

protected:
	float _sampleRate;

};


#endif
