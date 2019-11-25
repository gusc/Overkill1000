#include "ring_buffer.h"
#include <stdlib.h>
#include <string.h>

RingBuffer::RingBuffer(int itemCount, int itemSize){
	_itemSize = itemSize;
	_itemCount = itemCount;
	_start = (unsigned char*)malloc(itemCount * itemSize);
	_read = _start;
	_seek = _start;
	_write = _start;
	_end = _start + (itemCount * itemSize);
}
RingBuffer::~RingBuffer(){
	free(_start);
	_start = 0;
	_end = 0;
	_read = 0;
	_seek = 0;
	_write = 0;
}

bool RingBuffer::grow(int addCount){
	if (addCount > 0){
		int readOffset = _read - _start;
		int writeOffset = _write - _start;
		int seekOffset = _seek - _start;
		unsigned char *newBuffer = (unsigned char *)malloc((_itemCount + addCount) * _itemSize);
		memcpy(newBuffer, _start, _itemCount * _itemSize);
		free(_start);
		_start = 0;
		_start = newBuffer;
		_end = _start + ((_itemCount + addCount) * _itemSize);
		_read = _start + readOffset;
		_write = _start + writeOffset;
		_seek = _start + seekOffset;
		_itemCount += addCount;
		return true;
	}
	return false;
}
bool RingBuffer::shrink(int remCount){
	return false;
}

int RingBuffer::push(void *item){
	int idx = -1;
	if (!isFull()){ // Check if buffer not full
		memcpy(_write, item, _itemSize);
		idx = (_write - _start) / _itemSize;
		_write += _itemSize;
		if (_write >= _end){
			_write = _start;
		}
	}
	return idx;
}
bool RingBuffer::shift(void *item){
	if (!isEmpty()){
		memcpy(item, _read, _itemSize);
		_read += _itemSize;
		if (_read >= _end){
			_read = _start;
		}
		return true;
	}
	return false;
}
bool RingBuffer::pop(void *item){
	if (!isEmpty()){
		_write -= _itemSize;
		if (_write < _start){
			_write = _end - _itemSize;
		}
		memcpy(item, _write, _itemSize);
		return true;
	}
	return false;
}
bool RingBuffer::remove(int idx){
	unsigned char *i = (unsigned char *)((idx * _itemSize) + _start);
	if (_read <= _write){
		if (i >= _read && i < _write){
			memcpy((_read + _itemSize), _read, (i - _read));
			_read += _itemSize;
			if (_seek < _read){
				_seek = _read;
			}
			return true;
		}		
	} else if (_write < _read){
		if (i >= _read && i < _end){
			memcpy((_read + _itemSize), _read, (i - _read));
			_read += _itemSize;
			if (_seek < _read){
				_seek = _read;
			}
			return true;
		} else if (i >= _start && i < _write){
			memcpy(i, (i + _itemSize), (_write - (i + _itemSize)));
			_write -= _itemSize;
			if (_seek >= _write){
				_seek = _read;
			}
			return true;
		}
	}
	return false;
}
int RingBuffer::seek(void **item){
	int idx = -1;
	if (!isEmpty() && canSeek()){
		(*item) = _seek;
		idx = (_start - _seek) / _itemSize;
		_seek += _itemSize;
		if (_seek >= _end){
			_seek = _start;
		}
	}
	return idx;
}
void RingBuffer::restart(){
	_seek = _read;
}
void RingBuffer::empty(){
	_read = _start;
	_seek = _start;
	_write = _start;
}
int RingBuffer::count(){
	if (_read <= _write){
		return (int)(_write - _read) / _itemSize;
	} else {
		return (int)((_end - _read) + (_write - _itemSize)) / _itemSize;
	}
}
bool RingBuffer::isFull(){
	return (_read == _write + _itemSize) || (_read == _start && _write == _end - _itemSize);
}
bool RingBuffer::isEmpty(){
	return (_write == _read);
}
bool RingBuffer::canSeek(){
	if (_read <= _write){
		return (_seek >= _read && _seek < _write);
	} else {
		return (_seek >= _start && _seek < _write) || (_seek >= _read && _seek < _end);
	}
}