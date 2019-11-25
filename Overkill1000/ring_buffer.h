#ifndef __ring_buffer_h
#define __ring_buffer_h

class RingBuffer {
public:
	RingBuffer(int itemCount, int itemSize);
	~RingBuffer();

	bool grow(int newCount);
	bool write(void *item);
	bool poke(void *item, int idx);
	bool read(void *item);
	bool peek(void *item, int idx);
	bool skip(int len);
	void clear();
	void restart();
	bool isFull();
	bool isEmpty();
	int realCount();

private:
	unsigned char *_start, *_end, *_write, *_read;
	int _itemSize;
	int _itemCount;
};

#endif
