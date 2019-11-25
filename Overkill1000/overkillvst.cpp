#include "vstsdk2.4\pluginterfaces\vst2.x\aeffectx.h"
#include "vstsdk2.4\public.sdk\source\vst2.x\audioeffectx.h"
#include "ui\ui_editor.h"
#include "overkillvst.h"

OverkillVST::OverkillVST(audioMasterCallback audioMaster):AudioEffectX(audioMaster, kNumPrograms, kNumParams) {
	_engine = new Engine();

	setProgram (0);

	if (audioMaster){
		setNumInputs (kNumInputs);
		setNumOutputs (kNumOutputs);
		canProcessReplacing ();
		isSynth ();
		setUniqueID ('OVKL');

		extern AEffGUIEditor* createEditor (AudioEffectX*);
		setEditor (createEditor (this));
	}

	suspend ();
}
OverkillVST::~OverkillVST() {
	delete _engine;
}

void OverkillVST::processReplacing(float** inputs, float** outputs, VstInt32 sampleFrames) {
	_engine->generate(outputs[0], outputs[1], sampleFrames);
}

VstInt32 OverkillVST::processEvents(VstEvents* events) {
	for (VstInt32 i = 0; i < events->numEvents; i++){
		if ((events->events[i])->type != kVstMidiType){
			continue;
		}
		char *msg = (char *)malloc(sizeof(char) * 64);
		char title[] = "off";

		VstMidiEvent* event = (VstMidiEvent*)events->events[i];
		char* midiData = event->midiData;
		VstInt32 status = midiData[0] & 0xf0;	// ignoring channel
		if (status == 0x90 || status == 0x80) {
			// we only look at notes
			VstInt32 note = midiData[1] & 0x7f;
			VstInt32 velocity = midiData[2] & 0x7f;
			if (status == 0x80){
				// note off by velocity 0
				velocity = 0;
			}
			if (!velocity){
				_engine->noteOff(note);
			} else {
				_engine->noteOn(note, ((float)velocity / (float)128.0), event->deltaFrames);
			}
		} else if (status == 0xb0){
			if (midiData[1] == 0x7e || midiData[1] == 0x7b){
				// all notes off
				_engine->allOff();
			}
		}
		event++;
	}
	return 1;
}

void OverkillVST::setProgram(VstInt32 program) {

}
void OverkillVST::setProgramName(char* name) {

}
void OverkillVST::getProgramName(char* name) {
	vst_strncpy (name, "Program", kVstMaxProgNameLen);
}
bool OverkillVST::getProgramNameIndexed(VstInt32 category, VstInt32 index, char* text) {
	return false;
}

void OverkillVST::setParameter(VstInt32 index, float value) {

}
float OverkillVST::getParameter(VstInt32 index) {
	return 0.0;
}
void OverkillVST::getParameterLabel(VstInt32 index, char* label) {
	vst_strncpy (label, "x1x", kVstMaxParamStrLen);
}
void OverkillVST::getParameterDisplay(VstInt32 index, char* text) {
	vst_strncpy (text, "0", kVstMaxParamStrLen);
}
void OverkillVST::getParameterName(VstInt32 index, char* text) {
	vst_strncpy (text, "Param", kVstMaxParamStrLen);
}

void OverkillVST::setSampleRate(float sampleRate) {
	AudioEffectX::setSampleRate(sampleRate);
	_engine->setSampleRate(sampleRate);
}
void OverkillVST::setBlockSize (VstInt32 blockSize) {
	AudioEffectX::setBlockSize(blockSize);
	_engine->setBlockSize(blockSize);
}

bool OverkillVST::getInputProperties(VstInt32 index, VstPinProperties* properties) {
	if (index < kNumOutputs) {
		vst_strncpy (properties->label, "Overkill1000 ", 63);
		char temp[11] = {0};
		int2string (index + 1, temp, 10);
		vst_strncat (properties->label, temp, 63);

		properties->flags = kVstPinIsActive;
		if (index < 2){
			// make channel 1+2 stereo
			properties->flags |= kVstPinIsStereo;	
		}
		return true;
	}
	return false;
}
bool OverkillVST::getOutputProperties(VstInt32 index, VstPinProperties* properties) {
	if (index < kNumOutputs)
	{
		vst_strncpy (properties->label, "Overkill1000 ", 63);
		char temp[11] = {0};
		int2string (index + 1, temp, 10);
		vst_strncat (properties->label, temp, 63);

		properties->flags = kVstPinIsActive;
		if (index < 2){
			// make channel 1+2 stereo
			properties->flags |= kVstPinIsStereo;
		}
		return true;
	}
	return false;
}

bool OverkillVST::getEffectName(char* name) {
	vst_strncpy (name, "Overkill1000", kVstMaxEffectNameLen);
	return true;
}
bool OverkillVST::getVendorString(char* text) {
	vst_strncpy (text, "Gusts 'gusC' Kaksis", kVstMaxVendorStrLen);
	return true;
}
bool OverkillVST::getProductString(char* text) {
	vst_strncpy (text, "Modular VST Synth", kVstMaxProductStrLen);
	return true;
}
VstInt32 OverkillVST::getVendorVersion() {
	return 1000;
}
VstInt32 OverkillVST::canDo(char* text) {
	if (!strcmp (text, "receiveVstEvents"))
		return 1;
	if (!strcmp (text, "receiveVstMidiEvent"))
		return 1;
	if (!strcmp (text, "receiveVstMidiEvent"))
		return 1;
	if (!strcmp (text, "midiProgramNames"))
		return -1;
	return -1;	// explicitly can't do; 0 => don't know
}

VstInt32 OverkillVST::getNumMidiInputChannels() {
	return 1;
}
VstInt32 OverkillVST::getNumMidiOutputChannels() {
	return 0;
}

VstInt32 OverkillVST::getMidiProgramName(VstInt32 channel, MidiProgramName* midiProgramName) {
	return 0;
}
VstInt32 OverkillVST::getCurrentMidiProgram(VstInt32 channel, MidiProgramName* currentProgram) {
	return 0;
}
VstInt32 OverkillVST::getMidiProgramCategory(VstInt32 channel, MidiProgramCategory* category) {
	return 0;
}
bool OverkillVST::hasMidiProgramsChanged(VstInt32 channel) {
	return false;
}
bool OverkillVST::getMidiKeyName(VstInt32 channel, MidiKeyName* keyName) {
	keyName->keyName[0] = 0;
	keyName->reserved = 0;				// zero
	keyName->flags = 0;					// reserved, none defined yet, zero.
	return false;
}
