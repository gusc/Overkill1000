#include "ui_osc.h"

UIOsc::UIOsc(Osc *osc) : UIDevice() {
	_osc = osc;
}
UIOsc::~UIOsc(){
	_osc = 0;
}
Osc *UIOsc::getOsc(){
	return _osc;
}
