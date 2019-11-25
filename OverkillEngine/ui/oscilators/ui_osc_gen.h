#ifndef __ui_osc_gen_h
#define __ui_osc_gen_h

#include "ui_osc.h"
#include <FL\Fl_Dial.H>
#include <FL\Fl_Choice.H>

class UIOscGen : public UIOsc {

	Fl_Dial *_pw;
	Fl_Dial *_vol;
	Fl_Dial *_pitch;
	Fl_Choice *_wf;

public:
	
	UIOscGen(Osc *);
	~UIOscGen();

};

#endif
