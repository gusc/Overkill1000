#ifndef __osc_wtable_h
#define __osc_wtable_h

#include "osc.h"

class OscWTable : public Osc {
public:
	OscWTable(float *waveTable, int tableSize);
	~OscWTable();

	void setWaveTable(float *waveTable, int tableSize);

	float getSample(float samplePosition, float freq);

private:
	float *_waveTable;
	int _tableSize;

};

#endif
