#ifndef __envelope_h
#define __envelope_h

typedef struct {
	float initialVol;
	float delayTime;
	float attackTime;
	float peakVol;
	float holdTime;
	float decayTime;
	float sustainVol;
	float releaseTime;
	float endVol;
} t_Envelope;

#endif