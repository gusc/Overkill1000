#ifndef __ui_osc_h
#define __ui_osc_h

#include "ui\ui_device.h"
#include "engine\oscilators\osc.h"

class UIOsc : public UIDevice {

public:

	UIOsc(Osc *);
	~UIOsc();

	Osc *getOsc();

protected:

	Osc *_osc;

};

#endif
