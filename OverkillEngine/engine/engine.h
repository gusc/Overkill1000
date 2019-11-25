#ifndef __engine_h
#define __engine_h

#include <engine\types.h>
#include <engine\buffers\midi_buffer.h>
#include <engine\oscilators\osc.h>
#include <engine\oscilators\osc_gen.h>
#include <engine\oscilators\osc_sample.h>
#include <engine\oscilators\osc_wtable.h>
#include <engine\oscilators\osc_granular.h>
#include <engine\envelopes\env.h>
#include <engine\envelopes\env_dahdsr.h>
#include <engine\effects\delay.h>
#include <engine\effects\filter.h>
#include <Windows.h>

enum {
	kMaxOsc = 8,
	kMaxEnv = 8,
	kMaxLfo = 8,
	kMaxFx = 8
};

class Engine {
public:
	Engine();
	~Engine();

	int oscCount();
	int oscAdd(int);
	Osc *oscGet(int);
	void oscDelete(int);

	int envCount();
	int envAdd(int);
	Env *envGet(int);
	void envDelete(int);

	int fxCount();
	int fxAdd(int);
	Effect *fxGet(int);
	void fxDelete(int);

	void setTempo(float);
	float getTempo();
	void setTimeSignature(int, int);
	void setSampleRate(float);
	float getSampleRate();
	void setBlockSize(int);

	void noteOn(int, float, int);
	void noteOff(int);
	void allOff();
	void holdOn();
	void holdOff();
	void modWheel(float);
	void pitchWheel(float);

	bool generate(float *, float *, int);

private:

	CRITICAL_SECTION _cs;

	float _sampleCounter;

	float _freq[128];
	float _tempo;
	int _beatLength;
	int _barLength;
	float _samplesPerBeat;
	float _sampleRate;
	int _maxBlockSize;

	float _pitch;
	float _mod;
	float _volume;

	Env *_vol;

	int _envCount;
	Env *_env[kMaxEnv];
	int _oscCount;
	Osc *_osc[kMaxOsc];
	int _fxCount;
	Effect *_fx[kMaxFx];

	MidiBuffer *_midi;
	
};

#endif
