#ifndef __oscilator_h
#define __oscilator_h

enum {
	kOscTypeSine = 0,
	kOscTypeSquare,
	kOscTypeTri,
	kOscTypeNoise,
	kOscTypeCount
};

class Oscilator {
public:
	Oscilator();
	~Oscilator();

	void setSampleRate(float sampleRate);
	void setOffset(float offset);
	void setType(int type);
	void setFrequency(float freq);
	void setWidth(float width);

	float generate(float *buffer, int length);

private:
	int _type;
	float _sampleRate;
	float _offset;
	float _frequency;
	float _width;
};

#endif
