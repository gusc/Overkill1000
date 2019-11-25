#include "audiosystem.h"
#include "asio\asioaudio.h"
#include "asio\asiodriverlist.h"
#include "midi\mididriverlist.h"
#include "engine\engine.h"
#include <FL/Fl_Widget.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Preferences.H>
#include <Windows.h>

extern Engine *engine;
extern AudioSystem *audio;
extern ASIODriverList *asioList;
MIDIDriverList *midiList = new MIDIDriverList();

static void close_cb(Fl_Widget *w, void *d) {
	w->parent()->hide();
}
static void asio_cb(Fl_Widget *w, void *d){
	audio->loadAudio((int)d);
}
static void midi_cb(Fl_Widget *w, void *d){
	audio->loadMidi((int)d);
}

AudioSystem::AudioSystem(){
	int i;
	char *drvPref = (char *)calloc(128, sizeof(char));
	char *drvName = (char *)calloc(128, sizeof(char));
	audioDriverId = -1;
	midiDriverId = -1;

	Fl_Preferences *pref = new Fl_Preferences(Fl_Preferences::USER, "gusC", "Overkill1000");
	if (pref->entryExists("asioDriver")){
		pref->get("asioDriver", drvPref, "", 0, 127);
		for (i = 0; i < asioList->getNumDev(); i ++){
			if (asioList->getDriverName(i, drvName, 127) == 0){
				if (strcmp(drvName, drvPref) == 0){
					loadAudio(i);
					break;
				}
			}
		}
	}
	if (pref->entryExists("midiDriver")){
		pref->get("midiDriver", drvPref, "", 0, 127);
		for (i = 0; i < midiList->getNumDev(); i ++){
			if (midiList->getDriverName(i, drvName, 127) == 0){
				if (strcmp(drvName, drvPref) == 0){
					loadMidi(i);
					break;
				}
			}
		}
	}
	free(drvPref);
	free(drvName);
	delete pref;
}

AudioSystem::~AudioSystem(){
	unloadAudio();
	audioDriverId = -1;
	delete asioList;
}

void AudioSystem::unloadAudio(){
	unloadAsio();
	audioDriverId = -1;
}
bool AudioSystem::loadAudio(int id){
	if (id != audioDriverId){
		unloadAudio();
		if (loadAsio(id)){
			audioDriverId = id;
			char *drvName = (char *)calloc(128, sizeof(char));
			if (asioList->getDriverName(id, drvName, 127) == 0){
				Fl_Preferences *pref = new Fl_Preferences(Fl_Preferences::USER, "gusC", "Overkill1000");
				pref->set("asioDriver", drvName, 128);
				delete pref;
			}
			free(drvName);
		} else {
			return false;
		}
	}
	return true;
}
void AudioSystem::setAudioType(AudioDriverType type){
	audioDriverType = type;
}
void AudioSystem::unloadMidi(){
	midiList->closeDriver(midiDriverId);
	midiDriverId = -1;
}
bool AudioSystem::loadMidi(int id){
	if (id != midiDriverId){
		unloadMidi();
		if (midiList->openDriver(id) == 0){
			midiDriverId = id;
			char *drvName = (char *)calloc(128, sizeof(char));
			if (midiList->getDriverName(id, drvName, 127) == 0){
				Fl_Preferences *pref = new Fl_Preferences(Fl_Preferences::USER, "gusC", "Overkill1000");
				pref->set("midiDriver", drvName, 128);
				delete pref;
			}
			free(drvName);
		} else {
			return false;
		}
	}
	return true;
}

void AudioSystem::dialogOpen(){
	int i = 0;
	char name[128];
	Fl_Window *pref_win = new Fl_Window(260, 90, "Preferences");
	pref_win->set_modal();

	Fl_Choice *asio = new Fl_Choice(60, 10, 180, 22, "ASIO:");
	asio->add("- NONE -", 0, asio_cb, (void *)-1, 0);
	for (i = 0; i < asioList->getNumDev(); i ++){
		asioList->getDriverName(i, name, 128);
		asio->add(name, 0, asio_cb, (void *)i, 0);
	}
	asio->value(audioDriverId + 1);

	Fl_Choice *midi = new Fl_Choice(60, 35, 180, 22, "MIDI:");
	midi->add("- NONE -", 0, midi_cb, (void *)-1, 0);
	for (i = 0; i < midiList->getNumDev(); i ++){
		midiList->getDriverName(i, name, 128);
		midi->add(name, 0, midi_cb, (void *)i, 0);
	}
	midi->value(midiDriverId + 1);

	Fl_Button *ok = new Fl_Button(90, 60, 80, 22, "Close");
	ok->callback(close_cb);
	pref_win->end();
	pref_win->show();
}
