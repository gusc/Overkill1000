#ifndef __MIDI_DRIVER_LIST_H
#define __MIDI_DRIVER_LIST_H

#include <Windows.h>

#define DRVERR			-5000
#define DRVERR_INVALID_PARAM		DRVERR-1
#define DRVERR_DEVICE_ALREADY_OPEN	DRVERR-2
#define DRVERR_DEVICE_NOT_FOUND		DRVERR-3

#define MAXDRVNAMELEN		128

struct MIDIDriver {
	int						drvID;
	char					drvname[MAXDRVNAMELEN];
};

class MIDIDriverList {
	MIDIDriver *lpdrvlist;
	int numdrv;

public:
	/// Create MIDI driver list
	MIDIDriverList();
	/// Destroy MIDI driver list
	~MIDIDriverList();
	/// Open MIDI driver
	/// @param int - driver index
	/// @return long
	long openDriver (int);
	/// Close MIDI driver
	/// @param int - driver index
	/// @return long
	long closeDriver (int);
	/// Get number of MIDI drivers
	/// @return long
	long getNumDev (void);
	/// Get MIDI driver name
	/// @param int - driver index
	/// @param char * - driver name buffer
	/// @param int - max length of driver name to copy
	/// @return long
	long getDriverName (int,char *,int);
	
};

#endif
