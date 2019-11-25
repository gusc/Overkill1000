#ifndef __osc_h
#define __osc_h

#include "..\types.h"
#include "..\igenerator.h"
#include "engine\envelopes\env.h"

/// Oscilator base object
class Osc : public IGenerator {

public:
	/// Create oscilator base object
	Osc();
	/// Destroy oscilator base object
	~Osc();
	/// Get oscilator type
	/// @return int - value from OscType enum
	virtual int getType();
	/// Get a pointer to frequency modulation oscilator
	/// @return Osc *
	virtual Osc *getFMOsc();
	/// Set a pointer to frequency modulation oscilator
	/// @param Osc *
	/// @return void
	virtual void setFMOsc(Osc *);
	/// Get a frequency modulation ammount
	/// @return float
	virtual float getFMAmt();
	/// Set a frequency modulation ammount
	/// @param float
	/// @return void
	virtual void setFMAmt(float);
	/// Get volume
	/// @return float - volume
	virtual float getVolume();
	/// Set volume
	/// @param float - volume
	/// @return void
	virtual void setVolume(float);
	/// Get volume envelope object
	/// @return Env * - volume envelope object
	virtual Env *getVolEnv();
	/// Set volume envelope object
	/// @param Env * - volume envelope object
	/// @return void
	virtual void setVolEnv(Env *);
	/// Get pitch transposition
	/// @return float - pitch
	virtual float getPitch();
	/// Set pitch transposition
	/// @param float - pitch
	/// @return void
	virtual void setPitch(float);
	/// Get pitch envelope object
	/// @return Env * - pitch envelope object
	virtual Env *getPitchEnv();
	/// Set pitch envelope object
	/// @param Env * - pitch envelope object
	/// @return void
	virtual void setPitchEnv(Env *);
	
	
protected:
	/// Internal pointer to FM ocilator
	Osc *_fmOsc;
	/// FM ammount
	float _fmAmt;
	/// Current volume
	float _volume;
	/// Internal pointer to volume envelope
	Env *_volEnv;
	/// Pitch correction
	float _pitch;
	/// Internal pointer to volume envelope
	Env *_pitchEnv;

	/// Get modulated volume
	/// @param t_Note * - note data 
	/// @return float
	float _getVol(t_Note *);
	/// Get modulated frequence
	/// @param t_Note * - note data 
	/// @return float
	float _getFreq(t_Note *);

};

#endif
