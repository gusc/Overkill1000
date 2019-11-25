#ifndef __ui_h
#define __ui_h

#include <FL/Fl_Group.H>
#include "engine\engine.h"
#include "ui\effects\ui_effect.h"
#include "ui\effects\ui_filter.h"
#include "ui\effects\ui_delay.h"
#include "ui\oscilators\ui_osc.h"
#include "ui\oscilators\ui_osc_gen.h"

class UI : public Fl_Group {
	Engine * _engine;

	int _oscCount;
	int _fxCount;
	UIOsc *_osc[kMaxOsc];
	UIEffect *_fx[kMaxFx];

public:
	UI(Engine *);
	~UI();

	Engine *engine();
	void rebuild();
	int handle(int);

protected:

	void draw();
};

#endif
