#include "ui_delay.h"
#include "engine\effects\delay.h"

static void time_cb(Fl_Widget *w, void*o){
	float v = ((Fl_Dial *)w)->value();
	((Delay *)o)->setDelay(v);
}
static void fb_cb(Fl_Widget *w, void*o){
	float v = ((Fl_Dial *)w)->value();
	((Delay *)o)->setFeedback(v);
}
UIDelay::UIDelay(Effect *fx) : UIEffect(fx) {
	_time = new Fl_Dial(10, 10, 32, 32, "Time");
	_time->box(FL_ROUND_UP_BOX);
	_time->callback(time_cb, (void *)_fx);
	_time->minimum(0.1f);
	_time->maximum(((Delay *)_fx)->getMaxDelay());
	_time->value(((Delay *)_fx)->getDelay());
	add(_time);
	_fb = new Fl_Dial(52, 10, 32, 32, "Fb");
	_fb->box(FL_ROUND_UP_BOX);
	_fb->callback(fb_cb, (void *)_fx);
	_fb->minimum(0.0f);
	_fb->maximum(1.0f);
	_fb->value(((Delay *)_fx)->getFeedback());
	add(_fb);
	end();
}
UIDelay::~UIDelay(){
	remove(_time);
	remove(_fb);
	delete _time;
	delete _fb;
}
