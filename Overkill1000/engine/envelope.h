#ifndef __envelope_h
#define __envelope_h

typedef struct {
	float start = 0; // Level
	float delay = 0; // Time
	float attack = 0.1; // Time
	float peak = 1.0; // Level
	float hold = 0; // Time
	float decay = 0.5; // Time
	float sustain = 0.7; // Level
	float release = 0.01; // Time
	float end = 0; // Level
} t_Envelope;

#endif
