// DrawTest.cpp : Defines the entry point for the application.
//

#include "main.h"

#define MAX_LOADSTRING 100
//#define FL_INTERNALS
//#include <FL/x.h>
#include <FL/Fl.H>
#include <FL/Fl_Widget.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Menu_Item.H>
#include <FL/Fl_Preferences.H>
#include <ui\ui.h>
#include <engine\engine.h>
#include <Windows.h>
#include "audiosystem.h"
//#include <asiosdk2.2\host\pc\asiolist.h>

Engine *engine;
AudioSystem *audio;
UI *ui;

void quit_cb(Fl_Widget *w, void *d) {
	delete audio;
	delete ui;
	delete engine;
	exit(0);
}
void pref_cb(Fl_Widget *w, void *d) {
	audio->dialogOpen();
}
Fl_Menu_Item menutable[] = {
  {"&File", 0, 0, 0, FL_SUBMENU},
    {"&Quit", FL_ALT+'q', quit_cb},
		{0},
  {"&Edit", FL_F+2, 0, 0, FL_SUBMENU},
    {"Undo", FL_ALT+'z', 0},
    {"Redo", FL_ALT+'r', 0, 0, FL_MENU_DIVIDER},
    {"Cut", FL_ALT+'x', 0},
    {"Copy", FL_ALT+'c', 0},
    {"Paste", FL_ALT+'v', 0},
    {"Inactive", FL_ALT+'d', 0, 0, FL_MENU_INACTIVE},
    {"Clear", 0, 0, 0, FL_MENU_DIVIDER},
    {"Invisible", FL_ALT+'e', 0, 0, FL_MENU_INVISIBLE},
    {"Preferences", 0, pref_cb},
    {0},
	{0}
};

int main(int argc, char **argv) {
	int res;
	engine = new Engine();
	audio = new AudioSystem();

	Fl_Double_Window *window = new Fl_Double_Window(800, 630, "Overkill1000");

	Fl_Menu_Bar *menu = new Fl_Menu_Bar(0, 0, 800, 22);
	menu->box(FL_FLAT_BOX);
	menu->menu(menutable);

	ui = new UI(engine);
	ui->position(0, 24);
	ui->show();

	// START TEST!
	engine->oscAdd(kOscGen);
	engine->fxAdd(kFxDelay);
	engine->fxAdd(kFxFilter);
	ui->rebuild();
	// END TEST
	
	window->end();
  window->show(argc, argv);
	res = Fl::run();

	delete audio;
	delete ui;
	delete engine;
	return res;
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
	main(0, NULL);
}
