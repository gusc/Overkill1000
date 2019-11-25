#ifndef __audio_buffer_h
#define __audio_buffer_h

/// Audio buffer class
class AudioBuffer {
public:
	/// Create sample buffer
	/// @param int - maximum size
	/// @param int - initial size
	AudioBuffer(int, int);
	/// Create sample buffer from existing one
	/// @param float * - buffer
	/// @param int - size
	AudioBuffer(float *, int);
	/// Destroy sample buffer
	~AudioBuffer();
	/// Resize buffer to new size
	/// @param int - new buffer size
	/// @return void
	void resize(int);
	/// Fill buffer with empty samples
	/// @return void
	void fill();
	/// Write a sample to sample buffer
	/// @param float *- pointer of sample from which to copy the value
	/// @param void - write result (false - buffer full)
	bool write(float *);
	/// Read next sample
	/// @param float **- pointer of pointer where to store sample pointer
	/// @return bool - read result (false - end reached or buffer empty)
	bool read(float **);
	/// Get a value from buffer at a selected index
	/// @param int - sample index
	/// @param float * - pointer of pointer where to store sample pointer
	/// @return void
	bool get(int, float **);
	/// Delete first sample from sample buffer
	/// @param float *- pointer sample where to store a copy of removed sample value 
	/// @return bool - removed succesfully?
	bool remove(float *);
	/// Clear buffer
	/// @return void
	void clear();
	/// Restart sample readout
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
	/// Is the read at the starting position
	/// @return bool
	bool isStart();

private:
	int _size;
	int _maxSize;
	float *_buffer;
	float *_read;
	float *_write;
	float *_start;
	bool _outer;

};

#endif
