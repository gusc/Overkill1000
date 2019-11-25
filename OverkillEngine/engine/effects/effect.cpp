#include "effect.h"

/// Create effect base object
Effect::Effect() : IProcessor() {

}
/// Destroy effect base object
Effect::~Effect(){

}
/// Get effect type
/// @return int - value from FxType enum
int Effect::getType(){
	return kFxNone;
}
