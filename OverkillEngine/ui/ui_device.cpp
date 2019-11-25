#include "ui_device.h"
#include <FL\Fl.H>
#include <FL\fl_draw.H>
#include <Windows.h>
#include "ui_resource.h"

UIDevice::UIDevice() : Fl_Group(0, 0, 398, 74, "") {

}
UIDevice::~UIDevice(){

}
void UIDevice::draw(){
	Fl_Widget *const*a = array();
  if (damage() == FL_DAMAGE_CHILD) { // only redraw some children
    for (int i = children(); i --; a ++) {
			update_child(**a);
		}
  } else { // total redraw
		Fl_Color hl = fl_rgb_color(216, 216, 216);
		Fl_Color sh = fl_rgb_color(32, 32, 32);
		Fl_Color bg = fl_rgb_color(124, 124, 124);
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
			draw_outside_label(**a);
    }

		//HBITMAP bmp = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_OSC_SCREEN));
		//fl_copy_offscreen(x() + 10, y() + 10, 68, 43, bmp, 0, 0);
		//DeleteObject(bmp);
	}
}