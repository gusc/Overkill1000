#ifndef __env_folower_h
#define __env_folower_h

#include "envelope.h"

class EnvFolower {
public:
	EnvFolower(Envelope *env);
	~EnvFolower();

	void setSampleRate(float sampleRate);
	void setEnvelope(Envelope *env);

	void start();
	void release();
	bool isEnd();
	float getNextSample();

private:
	bool _active;
	bool _end;

	float _sampleRate;
	int _sampleCounter;
	Envelope *_env;
};


#endif
