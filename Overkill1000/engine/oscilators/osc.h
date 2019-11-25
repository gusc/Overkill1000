#ifndef __osc_h
#define __osc_h

enum {
	kOscNone = 0,
	kOscGen = 1,
	kOscWTable = 2,
	kOscGranular = 3
};

class Osc {
public:
	Osc();
	~Osc();
	
	virtual int type();
	void setSampleRate(float sampleRate);
	void enableState(bool enabled);
	bool isEnabled();

	virtual float getSample(float samplePosition, float freq);

protected:
	float _sampleRate;
	bool _enabled;

};

#endif
