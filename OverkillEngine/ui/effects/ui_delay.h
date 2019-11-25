#ifndef __ui_delay_h
#define __ui_delay_h

#include "ui_effect.h"
#include <FL\Fl_Dial.H>

class UIDelay : public UIEffect {

	Fl_Dial *_time;
	Fl_Dial *_fb;

public:

	UIDelay(Effect *);
	~UIDelay();

};

#endif
