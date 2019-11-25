#include "midi_buffer.h"
#include "engine\types.h"
#include <stdio.h>
#include <Windows.h>

MidiBuffer::MidiBuffer(int poliphony){
	_size = poliphony;
	_buffer = (t_Note *)calloc(_size + 1, sizeof(t_Note));
	_read = _buffer;
	_write = _start = _read + 1;
	for (int i = 0; i < 128; i ++){
		_pt[i] = 0;
	}
}
MidiBuffer::~MidiBuffer(){
	for (int i = 0; i < 128; i ++){
		_pt[i] = 0;
	}
	free(_buffer);
	_buffer = 0;
	_write = 0;
	_read = 0;
	_start = 0;
}
bool MidiBuffer::write(t_Note *note){
	if (!isFull()){
		EnterCriticalSection(&_cs);
		//_pt[note->num] = 0;
		memcpy(_write, note, sizeof(t_Note));
		_pt[note->num] = _write;
		_write ++;
		if (_write > _buffer + _size){
			_write = _buffer;
		}
		LeaveCriticalSection(&_cs);
		return true;
	}
	return false;
}
bool MidiBuffer::read(t_Note **note){
	if (!isEmpty() && !isEnd()){
		_read ++;
		if (_read > _buffer + _size){
			_read = _buffer;
		}
		if (_pt[_read->num] == 0){
			_read->phase = kPhaseEnd;
		}
		*note = _read;
		return true;
	}
	return false;
}
bool MidiBuffer::remove(t_Note *note){
	if (!isEmpty()){
		EnterCriticalSection(&_cs);
		_pt[_start->num] = 0;
		if (note != 0){
			memcpy(note, _start, sizeof(t_Note));
		}		
		_start ++;
		if (_start > _buffer + _size){
			_start = _buffer;
		}
		LeaveCriticalSection(&_cs);
		return true;
	}
	return false;
}
bool MidiBuffer::find(int noteNum, t_Note **note){
	if (!isEmpty()){
		if (_pt[noteNum] != 0){
			*note = _pt[noteNum];
			return true;
		}
	}
	return false;
}
/*bool MidiBuffer::removeByNum(int noteNum, t_Note *note){
	if (!isEmpty()){
		t_Note *b = _buffer;
		t_Note *p = _start;
		int key = _start - _buffer;
		while(p != _write){
			if (p->num == noteNum){
				if (note != 0){
					memcpy(note, p, sizeof(t_Note));
				}
				key += _size;
				while (p != _start){
					key --;
					memcpy(p, (b + (key % (_size))), sizeof(t_Note));
					p = b + (key % (_size));
				}
				_start ++;
				if (_start > _buffer + _size){
					_start = _buffer;
				}
				return true;
			}
			key ++;
			p = b + (key % (_size));
		}
	}
	return false;
}*/
void MidiBuffer::clear(){
	_read = _buffer;
	_write = _start = _read + 1;
	for (int i = 0; i < 128; i ++){
		_pt[i] = 0;
	}
}
void MidiBuffer::restart(){
	_read = _start - 1;
}
bool MidiBuffer::isFull(){
	if (_write == _start - 1){
		return true;
	} else if (_write == _buffer + _size && _start == _buffer){
		return true;
	}
	return false;
}
bool MidiBuffer::isEmpty(){
	if (_start == _write){
		return true;
	}
	return false;
}
bool MidiBuffer::isEnd(){
	if (_read == _write - 1){
		return true;
	} else if (_read == _buffer + _size && _write == _buffer){
		return true;
	}
	return false;
}
