#include "main.h"

#define MAX_LOADSTRING 100

#include <FL/Fl.H>
#include <FL/Fl_Widget.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Menu_Item.H>
#include <FL/Fl_Native_File_Chooser.H>
#include <Windows.h>
#include "editor.h"

Editor *ed;

void quit_cb(Fl_Widget *w, void *d) {
	exit(0);
}
void new_cb(Fl_Widget *w, void *d) {
	ed->clearTable();
}
void load_cb(Fl_Widget *w, void *d) {
	Fl_Native_File_Chooser *fc = new Fl_Native_File_Chooser(Fl_Native_File_Chooser::BROWSE_FILE);
	fc->filter("*.wtf");
	if (fc->show() == 0){
		FILE *fh = fopen(fc->filename(), "rb");
		char head[5] = "";
		int ts = 512;
		fread(head, sizeof(char), 4, fh);
		if (strcmp(head, "OKWF") == 0){
			fread(&ts, sizeof(int), 1, fh);
			float *t = (float *)malloc(sizeof(float) * ts);
			fread(t, sizeof(float), ts, fh);
			ed->setTable(t, ts);
		}
		fclose(fh);
	}
	delete fc;
}
void save_cb(Fl_Widget *w, void *d) {
	Fl_Native_File_Chooser *fc = new Fl_Native_File_Chooser(Fl_Native_File_Chooser::BROWSE_SAVE_FILE);
	fc->filter("*.wtf");
	if (fc->show() == 0){
		bool add = false;
		char *fname = (char *)fc->filename();
		if (strlen(fname) > 4){
			char *p = fname;
			p += strlen(fname) - 4;
			if (strcmp(p, ".wtf") != 0){
				add = true;
			}
		} else {
			add = true;
		}
		if (add){
			char *fname2 = (char *)malloc(sizeof(char) * (strlen(fname) + 5));
			memcpy(fname2, fname, sizeof(char) * strlen(fname));
			char *p = fname2;
			p += strlen(fname);
			memcpy(p, ".wtf\0", sizeof(char) * 5);
			fname = fname2;
		}
		FILE *fh = fopen(fname, "wb");
		int ts = ed->getTableSize();
		float *t = (float *)malloc(sizeof(float) * ts);
		ed->getTable(t, ts);
		fwrite("OKWF", sizeof(char), 4, fh);
		fwrite(&ts, sizeof(int), 1, fh);
		fwrite(t, sizeof(float), ts, fh);
		fclose(fh);
	}
	delete fc;
}
Fl_Menu_Item menutable[] = {
  {"&File", 0, 0, 0, FL_SUBMENU},
		{"&New...", FL_CTRL+'n', new_cb},
		{"&Load...", FL_CTRL+'o', load_cb},
		{"&Save...", FL_CTRL+'s', save_cb},
    {"&Quit", FL_ALT+'q', quit_cb},
		{0},
	{0}
};

int main(int argc, char **argv) {
	Fl_Double_Window *window = new Fl_Double_Window(512, 305, "WaveTable editor");

	Fl_Menu_Bar *menu = new Fl_Menu_Bar(0, 0, 512, 22);
	menu->box(FL_FLAT_BOX);
	menu->menu(menutable);

	ed = new Editor();
	ed->position(0, 24);
	
	window->end();
  window->show(argc, argv);

	return Fl::run();
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
	main(0, NULL);
}
