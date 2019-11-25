#include "editor.h"
#include "FL\Fl.H"
#include "FL\fl_draw.H"
#include "FL\Fl_Menu_Item.H"
#include <stdlib.h>
#include <memory>

enum WaveForm {
	kSine = 1,
	kSquare = 2,
	kTri = 3,
	kAscSaw = 4,
	kDescSaw = 5
};

static const double _pi = 3.1415926535897932384626433832795;

static void generate_cb(Fl_Widget *w, void *d){
	((Editor *)w)->generateWave((int)d);
}
static void buffsize_cb(Fl_Widget *w, void *d){
	((Editor *)w)->setTableSize((int)d);
}

Editor::Editor() : Fl_Box(0, 0, 512, 256, 0) {
	_tableSize = 512;
	_table = (float *)calloc(_tableSize, sizeof(float));
	_changed = false;
	_x_prev = _y_prev = -1;
}

Editor::~Editor(){

}

int Editor::getTableSize(){
	return _tableSize;
}
void Editor::setTableSize(int ts){
	if (ts > _tableSize){
		float *t;
		_table = (float *)realloc(_table, sizeof(float) * ts);
		t = _table;
		t += _tableSize;
		memset(t, 0, sizeof(float) * (ts - _tableSize));
	}
	_tableSize = ts;
	redraw();
}
void Editor::getTable(float *table, int ts){
	memcpy(table, _table, sizeof(float) * ts);
}
void Editor::setTable(float *table, int ts){
	setTableSize(ts);
	memcpy(_table, table, sizeof(float) * _tableSize);
	redraw();
}
void Editor::clearTable(){
	memset(_table, 0, sizeof(float) * _tableSize);
	redraw();
}
bool Editor::isChanged(){
	return _changed;
}
void Editor::generateWave(int type){
	float step = 2.0f / (float)_tableSize;
	for (int i = 0; i < _tableSize; i ++){
		switch (type){
		case kSine:
			_table[i] = (float)sin(_pi * (float)i * step);
			break;
		case kSquare:
			if (i < _tableSize / 2){
				_table[i] = (float)0.999999;
			} else {
				_table[i] = (float)-0.999999;
			}
			break;
		case kTri:
			if (i < _tableSize / 2){
				_table[i] =  1.0f - (2.0f * ((float)i / (float)(_tableSize / 2)));
			} else {
				_table[i] = (2.0f * ((float)(i - (_tableSize / 2)) / (float)(_tableSize / 2))) - 1.0f;
			}
			break;
		case kAscSaw:
			_table[i] = -1.0f + (step * (float)i);
			break;
		case kDescSaw:
			_table[i] = 1.0f - (step * (float)i);
			break;
		}
	}
	redraw();
}

void Editor::draw(){
	Fl_Color bg = fl_rgb_color(0, 32, 0);
	Fl_Color grid = fl_rgb_color(0, 128, 0);
	Fl_Color wave = fl_rgb_color(255, 255, 255);
	fl_draw_box(FL_FLAT_BOX, x(), y(), 512, 256, bg);
	fl_color(grid);
	fl_line_style(FL_SOLID | FL_CAP_SQUARE | FL_JOIN_MITER, 1, 0);
	fl_line(x(), y() + 127, x() + 512, y() + 127);
	fl_line(x() + 255, y(), x() + 255, y() + 256);
	fl_color(wave);
	float spp = 512.0f / (float)_tableSize;
	int x_from, x_to, y_from, y_to;
	for (int i = 0; i < _tableSize - 1; i ++){
		x_from = x_to = x();
		x_from += (int)((float)i * spp);
		x_to += (int)((float)(i + 1) * spp);
		y_from = y_to = y();
		y_from += (0 - (int)(_table[i] * 128.0f)) + 127;
		y_to += (0 - (int)(_table[i + 1] * 128.0f)) + 127;
		fl_line(x_from, y_from, x_to, y_from);
		fl_line(x_to, y_from, x_to, y_to);
	}
	if (_tableSize > 0){
		x_to = x();
		x_to += (int)((float)(_tableSize - 1) * spp);
		y_to = y();
		y_to += (0 - (int)(_table[_tableSize - 1] * 128.0f)) + 127;
		fl_line(x_to, y_to, x() + 512, y_to);
	}
}
int Editor::handle(int e){
	switch (e){
		case FL_PUSH:
			if (Fl::event_button() == FL_RIGHT_MOUSE){
				Fl_Menu_Item rclick_menu[] = {
					{ "Generate",  0, 0, 0, FL_SUBMENU },
						{ "Sine", 0, generate_cb, (void*)kSine },
						{ "Square",  0, generate_cb, (void*)kSquare },
						{ "Triangle",  0, generate_cb, (void*)kTri },
						{ "Ascending Saw",  0, generate_cb, (void*)kAscSaw },
						{ "Descending Saw",  0, generate_cb, (void*)kDescSaw },
						{ 0 },
					{ "Size", 0, 0, 0, FL_SUBMENU },
						{ "32", 0, buffsize_cb, (void *)32 },
						{ "64", 0, buffsize_cb, (void *)64 },
						{ "128", 0, buffsize_cb, (void *)128 },
						{ "256", 0, buffsize_cb, (void *)256 },
						{ "512", 0, buffsize_cb, (void *)512 },
						{ "1024", 0, buffsize_cb, (void *)1024 },
						{ "2048", 0, buffsize_cb, (void *)2048 },
						{ "4096", 0, buffsize_cb, (void *)4096 },
						{ 0 },
					{ 0 }
				};
				const Fl_Menu_Item *m = rclick_menu->popup(Fl::event_x(), Fl::event_y(), 0, 0, 0);
				if ( m && m->user_data() != 0 ) m->do_callback(this, m->user_data());
			} else if (Fl::event_button() == FL_LEFT_MOUSE){
				return 1;
			}
			break;
		case FL_RELEASE:
			if (Fl::event_button() == FL_LEFT_MOUSE){
				_x_prev = _y_prev = -1;
			}
			break;
		case FL_DRAG:
			if (Fl::event_button() == FL_LEFT_MOUSE){
				int mx = Fl::event_x() - x();
				int my = Fl::event_y() - y();
				float xidx = 512.0f / (float)_tableSize;
				float amp = ((256.0f - (float)my) / 128.0f) - 1.0f;
				int idx = (int)((float)mx / xidx);
				if (_x_prev > 0){
					float amp_prev = ((256.0f - (float)_y_prev) / 128.0f) - 1.0f;
					int dx = mx - _x_prev;
					float dy = (amp - amp_prev) / (float)abs(dx);
					if (dx >= 0){
						for (int i = 0; i < dx; i ++){
							_table[(int)((float)(i + _x_prev) / xidx)] = (amp_prev + (dy * (float)i)); 
						}
					} else {
						for (int i = dx; i < 0; i ++){
							_table[(int)((float)(i + _x_prev) / xidx)] = (amp_prev - (dy * (float)i)); 
						}
					}
				}
				_table[idx] = amp;
				_x_prev = mx;
				_y_prev = my;
				redraw();
			}
			break;
	}
	return Fl_Box::handle(e);
}
