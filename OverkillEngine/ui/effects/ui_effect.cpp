#include "ui_effect.h"

UIEffect::UIEffect(Effect *fx) : UIDevice() {
	_fx = fx;
}
UIEffect::~UIEffect(){
	_fx = 0;
}
Effect *UIEffect::getFx(){
	return _fx;
}
