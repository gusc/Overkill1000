#ifndef __midi_buffer_h
#define __midi_buffer_h

#include "engine\types.h"
#include <Windows.h>

enum {
	kMaxPoly = 16
};

/// MIDI buffer class
class MidiBuffer {

public:
	/// Create MIDI buffer
	/// @param int - maximum poliphony
	MidiBuffer(int);
	/// Destroy MIDI buffer
	~MidiBuffer();
	/// Read next note
	/// @param t_Note **- pointer to pointer of note structure where to store buffer pointer
	/// @return bool - read result (false - end reached or buffer empty)
	bool read(t_Note **);
	/// Write a note to note buffer
	/// @param t_Note *- pointer to note structure from which to read data for buffer
	/// @param void - write result (false - buffer full)
	bool write(t_Note *);
	/// Delete first element from note buffer
	/// @param t_Note *- pointer to note structure where to store a copy of removed note 
	/// @return bool - removed succesfully?
	bool remove(t_Note *);
	/// Find note by it's note number
	/// @param int - note number
	/// @param t_Note *- pointer to note structure where to store a copy of removed note 
	/// @return bool - have we found it?
	bool find(int, t_Note **);
	/// Clear buffer
	/// @return void
	void clear();
	/// Restart note readout
	/// @return void
	void restart();
	/// Is buffer full
	/// @return bool
	bool isFull();
	/// Is buffer empty
	/// @return bool
	bool isEmpty();
	/// Is the end of the read reached
	/// @return bool
	bool isEnd();

private:

	CRITICAL_SECTION _cs;

	int _size;
	t_Note *_buffer;
	t_Note *_pt[128];
	t_Note *_read;
	t_Note *_write;
	t_Note *_start;

};

#endif
