#include "ui_filter.h"
#include "engine\effects\filter.h"

static void freq_cb(Fl_Widget *w, void*o){
	float v = ((Fl_Dial *)w)->value();
	((Filter *)o)->setFreq(v);
}
static void q_cb(Fl_Widget *w, void*o){
	float v = ((Fl_Dial *)w)->value();
	((Filter *)o)->setQ(v);
}
static void type_cb(Fl_Widget *w, void*o){
	int v = ((Fl_Choice *)w)->value();
	((Filter *)o)->setFilterType(v);
}

UIFilter::UIFilter(Effect *fx) : UIEffect(fx) {
	_type = new Fl_Choice(24, 10, 56, 24, "Type");
	_type->add("LP");
	_type->add("BP");
	_type->add("HP");
	_type->add("Notch");
	_type->callback(type_cb, (void *)_fx);
	_type->value(((Filter *)_fx)->getFilterType());
	add(_type);
	_freq = new Fl_Dial(90, 10, 32, 32, "Freq");
	_freq->box(FL_ROUND_UP_BOX);
	_freq->callback(freq_cb, (void *)_fx);
	_freq->minimum(0.1f);
	_freq->maximum(22000.0f);
	_freq->value(((Filter *)_fx)->getFreq());
	add(_freq);
	_q = new Fl_Dial(132, 10, 32, 32, "Q");
	_q->box(FL_ROUND_UP_BOX);
	_q->callback(q_cb, (void *)_fx);
	_q->minimum(0.1f);
	_q->maximum(10.0f);
	_q->value(((Filter *)_fx)->getQ());
	add(_q);
	end();
}
UIFilter::~UIFilter(){
	remove(_type);
	remove(_freq);
	remove(_q);
	delete _type;
	delete _freq;
	delete _q;
}
