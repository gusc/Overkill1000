#ifndef __AUDIO_ASIO_H
#define __AUDIO_ASIO_H

#include "asio\asioaudio.h"

enum AudioDriverType {
	kAudioNone = 0,
	kAudioASIO = 1
};

class AudioSystem {	
	int audioDriverId;
	int midiDriverId;
	AudioDriverType audioDriverType;

public:
	AudioSystem();
	~AudioSystem();

	void setAudioType(AudioDriverType);
	bool loadAudio(int);
	void unloadAudio();
	bool loadMidi(int);
	void unloadMidi();
	void dialogOpen();

};

#endif
