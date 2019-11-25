#ifndef __env_h
#define __env_h

#include "../types.h"
#include "../igenerator.h"

/// Envelope base class
class Env : public IGenerator {

public:
	/// Create envelope base object
	Env();
	/// Destroy envelope base object
	~Env();
	/// Get envelope type
	/// @return int - value from EnvType enum
	virtual int getType();
	/// Get sample value at given sample position
	/// @param t_Note * - note data
	/// @return float - sample value
	virtual float generateSample(t_Note *);

};


#endif
