#ifndef __midi_note_h
#define __mide_note_h

#include "envelope\envelope.h"

class MidiNote {
public:
	MidiNote(int noteNum, float velocity);
	~MidiNote();

	void setEnvelope(Envelope *env);
	void setSampleRate(float sampleRate);

	void start();
	void tick();
	void release();
	bool isEnd();

	float getFreq();
	float getNextSamplePos();
	float getNextVelocity();

private:
	bool _active;
	bool _end;

	float _freq;
	float _sampleRate;
	float _sampleCounter;
	int _sampleOffset;
	int _noteNum;
	float _velocity;
	Envelope *_env;

};

#endif
