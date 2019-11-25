#ifndef __midi_engine_h
#define __midi_engine_h

typedef struct {
	float freq;
	float velocity;
	int sampleOffset;
	float sampleCounter;
	float releaseCounter;
	bool active;
	bool released;
} t_Note;

enum {
	kMaxPoly = 16
};

class MidiEngine {
public:
	MidiEngine(int polyphony);
	~MidiEngine();

	void noteOn(int noteNum, float velocity, int sampleOffset);
	void noteOff(int noteNum);
	void allOff();
	void hold(bool state);

	void restartReadActive();
	bool readNextActive(t_Note **note);
	void restartReadReleased();
	bool readNextReleased(t_Note **note);

private:
	bool _hold;
	int _poliphony;
	int _active;
	int _readActive;
	int _readReleased;

	t_Note _notes[128];
};

#endif
