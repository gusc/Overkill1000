#ifndef __delay_h
#define __delay_h

#include "effect.h"
#include "engine\buffers\audio_buffer.h"

/// Delay effect
class Delay : public Effect {

public:
	/// Create delay effect
	Delay(float);
	/// Destroy delay effect
	~Delay();
	/// Get effect type
	/// @return int - value from FxType enum
	int getType();
	/// Set delay time
	/// @param float - delay time in milliseconds
	/// @return void
	void setDelay(float);
	/// Get delay time
	/// @return float - delay time in milliseconds
	float getDelay();
	/// Get maximum delay value
	/// @return float - maximum delay time in milliseconds
	float getMaxDelay();
	/// Set feedback ammount
	/// @param float - feedback amount
	/// @return void
	void setFeedback(float);
	/// Get feedback ammount
	/// @return float - feedback ammount
	float getFeedback();
	/// Process sample value 
	/// @param float *- sample input/output
	/// @return void
	void processSample(float *);

private:
	// Feedback ammount
	float _feedback;
	// Delay time in miliseconds
	float _delayTime;
	// Internal audio buffer
	AudioBuffer *_buffer;

};

#endif
