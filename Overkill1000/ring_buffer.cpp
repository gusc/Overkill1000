#include "ring_buffer.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

RingBuffer::RingBuffer(int itemCount, int itemSize){
	_itemSize = itemSize;
	_itemCount = itemCount;
	_start = (unsigned char*)malloc(itemCount * itemSize);
	_end = _start + (itemCount * itemSize);
	_read = _start;
	_write = _start + 1;
}
RingBuffer::~RingBuffer(){
	free(_start);
	_start = 0;
	_end = 0;
	_read = 0;
	_write = 0;
}

bool RingBuffer::grow(int newCount){
	if (newCount > _itemCount){
		unsigned char *newBuffer = (unsigned char *)malloc(newCount * _itemSize);
		memcpy(newBuffer, _start, _itemCount * _itemSize);
		free(_start);
		_start = 0;
		_start = newBuffer;
		_end = _start + (newCount * _itemSize);
		_read = _start;
		_write = _start + 1;
		_itemCount = newCount;
		return true;
	}
	return false;
}
bool RingBuffer::write(void *item){
	if (_write != _read + 1){
		memcpy(_write, item, _itemSize);
		_write += _itemSize;
		return true;
	}
	return false;
}
bool RingBuffer::poke(void *item, int idx){
	if (_write + idx != _read + 1){
		memcpy(_write + idx, item, _itemSize);
		return true;
	}
	return false;
}
bool RingBuffer::read(void *item){
	if (_read != _write + 1){
		memcpy(item, _read, _itemSize);
		_read += _itemSize;
		return true;
	}
	return false;
}
bool RingBuffer::peek(void *item, int idx){
	if (_read + idx != _write + 1){
		memcpy(item, _read + idx, _itemSize);
		return true;
	}
	return false;
}
bool RingBuffer::skip(int len){
	if (_read + len != _write + 1){
		_read += len * _itemSize;
		return true;
	}
	return false;
}
void RingBuffer::clear(){
	_read = _start;
	_write = _start + 1;
}
void RingBuffer::restart(){
	_read = _write + 1;
}
bool RingBuffer::isFull(){
	return (_read == _write + 1);
}
bool RingBuffer::isEmpty(){
	return (_write == _read + 1);
}
int RingBuffer::realCount(){
	if (_write < _read){
		return (int)((_end - _read) + (_write - 1));
	} else {
		return (int)(_write - _read);
	}
}