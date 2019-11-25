#ifndef __osc_gen_h
#define __osc_gen_h

#include "osc.h"

/// Oscilator waveform types
enum OscWaveType {
	kOscTypeSine = 0,
	kOscTypeSquare,
	kOscTypeTri,
	kOscTypeNoise,
	kMaxOscType
};

/// Generated oscilator class
class OscGen : public Osc {
public:
	/// Create generated oscilator object
	OscGen(int oscWave);
	/// Destroy generated oscilator object
	~OscGen();
	/// Get oscilator type
	/// @return int - value from OscType enum
	int getType();
	/// Set waveform type
	/// @param int - value from OscWaveType enum
	/// @return void
	void setWaveType(int);
	/// Get waveform type
	/// @return int - value from OscWaveType enum
	int getWaveType();
	/// Set pulse width
	/// @param float - pulse width
	/// @return void
	void setPulseWidth(float);
	/// Get pulse width
	/// @return float
	float getPulseWidth();
	/// Get a pointer to pulse width modulation oscilator
	/// @return Osc *
	Osc *getPWMOsc();
	/// Set a pointer to pulse width modulation oscilator
	/// @param Osc *
	/// @return void
	void setPWMOsc(Osc *);
	/// Get sample value at given sample position
	/// @param t_Note * - note data
	/// @return float - sample value
	float generateSample(t_Note *);

protected:
	/// Internal pointer to PWM oscilator
	Osc *_pwmOsc;
	/// Current waveform type
	int _waveType;
	/// Current pulse width
	float _pulseWidth;
	/// Previous sample amplitude
	float _prevSample;
	/// Current wave phase (positive/negative)
	int _currentPhase;

};

#endif
