#include "ui.h"
#include "engine\engine.h"
#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Menu.H>
#include <FL/Fl_Menu_Item.H>

static void add_osc_cb(Fl_Widget *w, void *d){
	UI *ui = (UI *)w;
	ui->engine()->oscAdd((int)d);
	ui->rebuild();
}

UI::UI(Engine *engine):Fl_Group(0, 0, 800, 600, ""){
	_engine = engine;
	_oscCount = 0;
	_fxCount = 0;
	end();
}
UI::~UI(){
	_engine = NULL;
}
Engine *UI::engine(){
	return _engine;
}
void UI::rebuild(){
	int i;
	for (i = 0; i < _oscCount; i ++){
		remove(_osc[i]);
		delete _osc[i];
	}
	for (i = 0; i < _fxCount; i ++){
		remove(_fx[i]);
		delete _fx[i];
	}
	for (i = 0; i < _engine->oscCount(); i ++){
		Osc *o = _engine->oscGet(i);
		switch (o->getType()){
			case kOscGen:
				_osc[i] = new UIOscGen(o);
				break;
			default:
				_osc[i] = 0;
				break;
		}
		if (_osc[i] != 0){
			add(_osc[i]);
			_osc[i]->position(x() + 1, y() + ((_osc[i]->h() + 1) * i) + 1);
		}
	}
	for (i = 0; i < _engine->fxCount(); i ++){
		Effect *e = _engine->fxGet(i);
		switch (e->getType()){
		case kFxDelay:
				_fx[i] = new UIDelay(e);
				break;
			case kFxFilter:
				_fx[i] = new UIFilter(e);
				break;
			default:
				_fx[i] = 0;
				break;
		}
		if (_fx[i] != 0){
			add(_fx[i]);
			_fx[i]->position(x() + 400, y() + ((_fx[i]->h() + 1) * i) + 1);
		}
	}
	redraw();
}
int UI::handle(int e){
	switch (e){
	case FL_PUSH:
		if (Fl::event_button() == FL_RIGHT_MOUSE){
			Fl_Menu_Item rclick_menu[] = {
				{ "Add Generator",  0, add_osc_cb, (void*)kOscGen },
				{ "Add Sampler",  0, add_osc_cb, (void*)kOscSample },
				{ "Add Granular",  0, add_osc_cb, (void*)kOscGranular },
				{ "Add Wavetable",  0, add_osc_cb, (void*)kOscWTable },
        { 0 }
			};
			const Fl_Menu_Item *m = rclick_menu->popup(Fl::event_x(), Fl::event_y(), 0, 0, 0);
			if ( m ) m->do_callback(this, m->user_data());
		}
		break;
	}
	return Fl_Group::handle(e);
}
void UI::draw(){
	Fl_Widget *const*a = array();
  if (damage() == FL_DAMAGE_CHILD) { // only redraw some children
    for (int i = children(); i --; a ++) {
			update_child(**a);
		}
  } else { // total redraw
    Fl_Color bg = fl_rgb_color(64, 64, 64);
		Fl_Color sh = fl_rgb_color(216, 216, 216);
		Fl_Color hl = fl_rgb_color(32, 32, 32);
		fl_color(bg);
		fl_rectf(x(), y(), w(), h(), bg);
		fl_color(hl);
		fl_line_style(FL_SOLID | FL_CAP_SQUARE | FL_JOIN_MITER, 1, 0);
		fl_line(x(), y(), x() + w(), y());
		fl_line(x(), y(), x(), y() + h());
		fl_color(sh);
		fl_line(x() + w(), y(), x() + w(), y() + h());
		fl_line(x(), y() + h(), x() + w(), y() + h());
		for (int i = children(); i --; a ++) {
      draw_child(**a);
    }
  }
}