#include "delay.h"

Delay::Delay(float time):Effect() {
	_delayTime = time;
	_buffer = new AudioBuffer((int)(_sampleRate * 3.f), (int)(_sampleRate * (_delayTime / 1000.f)));
	_feedback = 0.0f;
}
Delay::~Delay(){
	delete _buffer;
}
/// Get effect type
/// @return int - value from FxType enum
int Delay::getType(){
	return kFxDelay;
}
void Delay::setDelay(float time){
	_delayTime = time;
	_buffer->resize((int)(_sampleRate * (_delayTime / 1000.f)));
}
float Delay::getDelay(){
	return _delayTime;
}
float Delay::getMaxDelay(){
	return 3000.f;
}
void Delay::setFeedback(float fb){
	_feedback = fb;
}
float Delay::getFeedback(){
	return _feedback;
}
/// Process sample value 
/// @param float *- sample input/output
/// @return void
void Delay::processSample(float *sample){
	float prev = 0.0f;
	if (_buffer->isFull()){
		_buffer->remove(&prev);
	}
	*sample += prev * _feedback;
	_buffer->write(sample);
}
