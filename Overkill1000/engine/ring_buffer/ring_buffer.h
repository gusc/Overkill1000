#ifndef __ring_buffer_h
#define __ring_buffer_h

class RingBuffer {
public:
	RingBuffer(int itemCount, int itemSize);
	~RingBuffer();

	bool grow(int addCount); // Increase the size of the buffer
	bool shrink(int remCount); // Decrease the size of the buffer
	
	int push(void *item); // Add an item to the end of the buffer
	bool shift(void *item); // Remove item from the begining
	bool pop(void *item); // Remove item from the end
	bool remove(int idx); // Remove item by it's index and defragment the buffer

	void empty(); // Empty buffer
	int count(); // Return the real count of items in the buffer
	
	int seek(void **item); // Get next element between read and write pointers
	void restart(); // Restart the seek
	
	bool isFull(); // Is buffer full
	bool isEmpty(); // Is buffer empty
	bool canSeek(); // Can we perform a seek operation

private:
	unsigned char *_start, *_end, *_write, *_read, *_seek;
	int _itemSize;
	int _itemCount;
};

#endif
