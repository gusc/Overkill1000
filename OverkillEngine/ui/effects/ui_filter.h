#ifndef __ui_filter_h
#define __ui_filter_h

#include "ui_effect.h"
#include <FL\Fl_Dial.H>
#include <FL\Fl_Choice.H>

class UIFilter : public UIEffect {

	Fl_Dial *_freq;
	Fl_Dial *_q;
	Fl_Choice *_type;

public:

	UIFilter(Effect *);
	~UIFilter();

};

#endif
