#include "ui_osc_gen.h"
#include "engine\oscilators\osc_gen.h"

static void pw_cb(Fl_Widget *w, void*o){
	float v = ((Fl_Dial *)w)->value();
	((OscGen *)o)->setPulseWidth(v);
}
static void vol_cb(Fl_Widget *w, void*o){
	float v = ((Fl_Dial *)w)->value();
	((OscGen *)o)->setVolume(v);
}
static void wf_cb(Fl_Widget *w, void*o){
	int v = ((Fl_Choice *)w)->value();
	((OscGen *)o)->setWaveType(v);
}
static void pitch_cb(Fl_Widget *w, void*o){
	float v = ((Fl_Dial *)w)->value();
	((OscGen *)o)->setPitch(v);
}

UIOscGen::UIOscGen(Osc *osc) : UIOsc(osc) {
	_wf = new Fl_Choice(24, 10, 56, 24, "WF");
	_wf->add("Sine");
	_wf->add("Square");
	_wf->add("Triangle");
	_wf->add("Noise");
	_wf->callback(wf_cb, (void *)_osc);
	_wf->value(((OscGen *)_osc)->getWaveType());
	add(_wf);
	_pw = new Fl_Dial(90, 10, 32, 32, "PW");
	_pw->box(FL_ROUND_UP_BOX);
	_pw->callback(pw_cb, (void *)_osc);
	_pw->minimum(0.0f);
	_pw->maximum(2.0f);
	_pw->value(((OscGen *)_osc)->getPulseWidth());
	add(_pw);
	_vol = new Fl_Dial(132, 10, 32, 32, "VOL");
	_vol->box(FL_ROUND_UP_BOX);
	_vol->callback(vol_cb, (void *)_osc);
	_vol->minimum(0.0f);
	_vol->maximum(1.0f);
	_vol->value(((OscGen *)_osc)->getVolume());
	add(_pw);
	_pitch = new Fl_Dial(174, 10, 32, 32, "Pitch");
	_pitch->box(FL_ROUND_UP_BOX);
	_pitch->callback(pitch_cb, (void *)_osc);
	_pitch->minimum(-24.0f);
	_pitch->maximum(24.0f);
	_pitch->step(1.0f);
	_pitch->value(((OscGen *)_osc)->getPitch());
	add(_pitch);
	end();
}
UIOscGen::~UIOscGen(){
	remove(_pw);
	remove(_wf);
	remove(_vol);
	delete _pw;
	delete _wf;
	delete _vol;
}
