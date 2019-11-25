#include "audio_buffer.h"
#include <stdio.h>
#include <Windows.h>

/// Create sample buffer
/// @param int - maximum size
/// @param int - initial size
AudioBuffer::AudioBuffer(int maxSize, int initSize){
	_maxSize = maxSize;
	_size = initSize;
	if (_size > _maxSize){
		_size = _maxSize;
	}
	_buffer = (float *)calloc(_maxSize + 1, sizeof(float));
	_read = _buffer;
	_write = _start = _read + 1;
	_outer = false;
}
/// Create sample buffer from existing one
/// @param float * - buffer
/// @param int - size
AudioBuffer::AudioBuffer(float *buffer, int size){
	_maxSize = size - 1;
	_size = size - 1;
	_buffer = buffer;
	_read = _write = _buffer;
	_start = _buffer + 1;
	_outer = true;
}
/// Destroy sample buffer
	AudioBuffer::~AudioBuffer(){
	if (!_outer){
		free(_buffer);
	}
	_buffer = 0;
	_write = 0;
	_read = 0;
	_start = 0;
}
/// Resize buffer to new size
/// @param int - new buffer size
/// @return void
void AudioBuffer::resize(int newSize){
	if (newSize > _maxSize){
		newSize = _maxSize;
	}
	_size = newSize;
}
/// Fill buffer with empty samples
/// @return void
void AudioBuffer::fill(){
	int l = _size;
	float *b = _buffer;
	while (--l >= 0){
		(*b++) = 0.0f;
	}
	_start = _buffer + 1;
	_write = _buffer;
}
/// Write a sample to sample buffer
/// @param float *- pointer of sample from which to copy the value
/// @param void - write result (false - buffer full)
bool AudioBuffer::write(float *sample){
	if (isFull()){
		if (isStart()){
			_read ++;
			if (_read > _buffer + _size){
				_read = _buffer;
			}
		}
		_start ++;
		if (_start > _buffer + _size){
			_start = _buffer;
		}
	}
	memcpy(_write, sample, sizeof(float));
	_write ++;
	if (_write > _buffer + _size){
		_write = _buffer;
	}
	return true;
}
/// Read next sample
/// @param float **- pointer of pointer where to store sample pointer
/// @return bool - read result (false - end reached or buffer empty)
bool AudioBuffer::read(float **sample){
	if (!isEmpty() && !isEnd()){
		_read ++;
		if (_read > _buffer + _size){
			_read = _buffer;
		}
		*sample = _read;
		return true;
	}
	return false;
}
/// Get a value from buffer at a selected index
/// @param int - sample index
/// @param float * - pointer of pointer where to store sample pointer
/// @return void
bool AudioBuffer::get(int idx, float **sample){
	if (idx <= _size){
		if (!isEmpty()){
			int idx2 = (((_start + idx) - _buffer) % (_size));
			float *pidx = _buffer + idx2;
			if (!(pidx > _write && pidx < _start)){
				*sample = pidx;
				return true;
			}
		}
	}
	return false;
}
/// Delete first sample from sample buffer
/// @param float *- pointer sample where to store a copy of removed sample value 
/// @return bool - removed succesfully?
bool AudioBuffer::remove(float *sample){
	if (!isEmpty()){
		if (sample != 0){
			memcpy(sample, _start, sizeof(float));
		}
		_start ++;
		if (_start > _buffer + _size){
			_start = _buffer;
		}
		return true;
	}
	return false;
}
/// Clear buffer
/// @return void
void AudioBuffer::clear(){
	_read = _buffer;
	_write = _start = _read + 1;
}
/// Restart sample readout
/// @return void
void AudioBuffer::restart(){
	_read = _start - 1;
}
/// Is buffer full
/// @return bool
bool AudioBuffer::isFull(){
	if (_write == _start - 1){
		// |w|s|.|.|.|
		return true;
	} else if (_write == _buffer + _size && _start == _buffer){
		// |s|.|.|.|w|
		return true;
	}
	return false;
}
/// Is buffer empty
/// @return bool
bool AudioBuffer::isEmpty(){
	if (_start == _write){
		// |.|ws|.|.|.|
		return true;
	}
	return false;
}
/// Is the end of the read reached
/// @return bool
bool AudioBuffer::isEnd(){
	if (_read == _write - 1){
		// |.|r|w|.|.|
		return true;
	} else if (_read == _buffer + _size && _write == _buffer){
		// |w|.|.|.|r|
		return true;
	}
	return false;
}
/// Is the read at the starting position
/// @return bool
bool AudioBuffer::isStart(){
	if (_read == _start - 1){
		// |.|rs|.|.|.
		return true;
	}
	return false;
}
