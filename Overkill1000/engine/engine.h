#ifndef __engine_h
#define __engine_h

#include "midi_engine.h"
#include "oscilators\osc_folower.h"
#include "oscilators\osc_gen.h"
#include "oscilators\osc_wtable.h"
#include "envelope\env_dahdsr.h"

enum {
	kChanLeft = 0,
	kChanRight,
	kNumChan
};
enum {
	kMaxOsc = 1,
	kMaxEnv = 1
};

class Engine {
public:
	 Engine();
	 ~Engine();

	 void setSampleRate(float sampleRate);

	 void setBlockSize(int blockSize);

	 void noteOn(int noteNum, float velocity, int startOffset);
	 void noteOff(int noteNum);
	 void allOff();
	 void holdOn();
	 void holdOff();
	 void modWheel(float data);
	 void pitchWheel(float data);

	 bool generate(float *buffer_l, float * buffer_r, int length);

private:
	float _sampleRate;
	int _maxBlockSize;

	float _pitch;
	float _mod;
	float _volume;

	Env *_env[kMaxEnv];
	Osc *_osc[kMaxOsc];
	MidiEngine *_midi;
	
};

#endif
