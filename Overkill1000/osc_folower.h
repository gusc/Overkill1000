#ifndef __osc_folower_h
#define __osc_folower_h

class OscFolower {
public:
	OscFolower();
	~OscFolower();

	void setSampleRate(float sampleRate);
	void setSampleOffset(int sampleOffset);
	int getSampleOffset();
	void start();

	float getNextSample(float freq);

private:
	float _sampleRate;
	float _samplePosition;
	int _sampleOffset;
};

#endif
