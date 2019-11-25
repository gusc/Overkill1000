#ifndef __ui_editor_h
#define __ui_editor_h

#include "../vstgui/plugin-bindings/aeffguieditor.h"
#include "ui_parameters.h"

class UIEditor : public AEffGUIEditor, public CControlListener {
public:
	UIEditor(void *);
	~UIEditor();

	// from AEffGUIEditor
	bool open (void* ptr);
	void close ();
	void setParameter (VstInt32 index, float value);

	// from CControlListener
	void valueChanged (CControl* pControl);

protected:
	CControl* controls[kNumParameters];

};

#endif
