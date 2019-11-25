#include "mididriverlist.h"
#include "engine\engine.h"

extern Engine *engine;

HMIDIIN midiDriver = NULL;

static void CALLBACK midiInProcess(HMIDIIN hMidiIn, UINT wMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2){
	switch (wMsg){
		case MIM_DATA:
			{
				UCHAR channel, cc, note, velocity;
				cc = *(((UCHAR*)&dwParam1));
				//other = *(((UCHAR*)&dwParam1) + 3);
				if ((cc & 0xF0) == 0x90){
					velocity = *(((UCHAR*)&dwParam1) + 2);
					channel = cc & 0x0F;
					note = *(((UCHAR*)&dwParam1) + 1);
					if (velocity > 0){
						// Note on
						float offset = 0.f;//(float)&dwParam2 / engine->getSampleRate();
						engine->noteOn((int)note, ((float)velocity) / 256.0, (int)offset);
					} else {
						engine->noteOff((int)note);
					}
				} else if ((cc & 0xF0) == 0x80) {
					// Note off or velocity=0
					channel = cc & 0x0F;
					note = *(((UCHAR*)&dwParam1) + 1);
					engine->noteOff((int)note);
				}
			}
			break;
		case MIM_OPEN:
			break;
		case MIM_CLOSE:
			break;
		default:
			break;
	}
}

MIDIDriverList::MIDIDriverList(){
	numdrv = midiInGetNumDevs();
	lpdrvlist = new MIDIDriver[numdrv];
	for (int i = 0; i < numdrv; i ++){
		lpdrvlist[i].drvID = i;
		MIDIINCAPS mi;
		if (!midiInGetDevCaps(i, &mi, sizeof(MIDIINCAPS))){
			memcpy(lpdrvlist[i].drvname, mi.szPname, sizeof(char) * MAXDRVNAMELEN);
		}
	}
}
MIDIDriverList::~MIDIDriverList(){
	delete[] lpdrvlist;
}
long MIDIDriverList::openDriver (int drvID){
	if (drvID >= 0 && drvID < numdrv){
		if (midiInOpen(&midiDriver, drvID, (DWORD_PTR)(&midiInProcess), 0, CALLBACK_FUNCTION) == MMSYSERR_NOERROR){
			if (midiInStart(midiDriver) == MMSYSERR_NOERROR){
				return 0;
			}
		}
	}
	return DRVERR_DEVICE_NOT_FOUND;
}
long MIDIDriverList::closeDriver (int drvID){
	if (midiDriver != NULL){
		midiInStop(midiDriver);
		midiInClose(midiDriver);
	}
	return 0;
}
long MIDIDriverList::getNumDev(){
	return numdrv;
}
long MIDIDriverList::getDriverName (int drvID,char *drvname,int drvnamesize){
	MIDIDriver *lpdrv;

	if (!drvname) return DRVERR_INVALID_PARAM;

	if (drvID >= 0 && drvID < numdrv){
		lpdrv = &(lpdrvlist[drvID]);
		if (strlen(lpdrv->drvname) < (unsigned int)drvnamesize) {
			strcpy(drvname,lpdrv->drvname);
		} else {
			memcpy(drvname,lpdrv->drvname,drvnamesize-4);
			drvname[drvnamesize-4] = '.';
			drvname[drvnamesize-3] = '.';
			drvname[drvnamesize-2] = '.';
			drvname[drvnamesize-1] = 0;
		}
		return 0;
	}
	return DRVERR_DEVICE_NOT_FOUND;
}
