#ifndef __effect_h
#define __effect_h

#include "..\types.h"
#include "..\iprocessor.h"

/// Effect base object
class Effect : public IProcessor {

public:
	/// Create effect base object
	Effect();
	/// Destroy effect base object
	~Effect();
	/// Get effect type
	/// @return int - value from FxType enum
	virtual int getType();

};

#endif
