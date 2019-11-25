#ifndef __editor_h
#define __editor_h

#include <FL/Fl_Box.H>

class Editor : public Fl_Box {

	float *_table;
	int _tableSize;
	bool _changed;

public:
	Editor();
	~Editor();

	int getTableSize();
	void setTableSize(int);
	void getTable(float *, int);
	void setTable(float *, int);
	void generateWave(int);
	void clearTable();
	bool isChanged();

	int handle(int);

protected:

	int _x_prev, _y_prev;
	void draw();

};

#endif
