#include "ui_editor.h"

AEffGUIEditor* createEditor (AudioEffectX* effect){
	return new UIEditor (effect);
}

UIEditor::UIEditor (void* ptr) : AEffGUIEditor (ptr){
	rect.left = 0;
	rect.top = 0;
	rect.right = 960;
	rect.bottom = 600;	
}
UIEditor::~UIEditor() {

}

bool UIEditor::open (void* ptr) {
	AEffGUIEditor::open(ptr);

	CBitmap* mainBg = new CBitmap ("MainBackground.png");

	//-- first we create the frame with a size of 300, 300 and set the background to white
	CRect frameSize (0.0, 0.0, mainBg->getWidth(), mainBg->getHeight());
	CFrame* newFrame = new CFrame (frameSize, ptr, this);
	newFrame->setBackgroundColor (kBlackCColor);
	newFrame->setBackground(mainBg);
	
	//-- load some bitmaps we need
	CBitmap* background = new CBitmap ("KnobBackground.png");
	CBitmap* handle = new CBitmap ("KnobHandle.png");

	//-- creating a knob and adding it to the frame
	CRect r (0, 0, background->getWidth (), background->getHeight ());
	r.offset(10, 41);
	CKnob* knob1 = new CKnob (r, this, kVolumeParameter, background, handle, CPoint (0, 0));
	knob1->setInsetValue(20.0);
	newFrame->addView (knob1);

	//-- creating another knob, we are offsetting the rect, so that the knob is next to the previous knob
	r.offset (background->getWidth(), 0);
	CKnob* knob2 = new CKnob (r, this, kPulseWidthParameter, background, handle, CPoint (0, 0));
	knob2->setInsetValue(20.0);
	newFrame->addView (knob2);

	//-- forget the bitmaps
	background->forget ();
	handle->forget ();

	//-- remember our controls so that we can sync them with the state of the effect
	controls[kVolumeParameter] = knob1;
	controls[kPulseWidthParameter] = knob2;

	//-- set the member frame to the newly created frame
	//-- we do it this way because it is possible that the setParameter method is called 
	//-- in between of constructing the frame and it's controls
	frame = newFrame;

	mainBg->forget();

	//-- sync parameters
	for (int i = 0; i < kNumParameters; i++)
		setParameter (i, effect->getParameter (i));

	return true;
}

//------------------------------------------------------------------------------------
void UIEditor::close ()
{
	//-- on close we need to delete the frame object.
	//-- once again we make sure that the member frame variable is set to zero before we delete it
	//-- so that calls to setParameter won't crash.
	CFrame* oldFrame = frame;
	frame = 0;
	oldFrame->forget ();
}

//------------------------------------------------------------------------------------
void UIEditor::valueChanged (CControl* pControl)
{
	//-- valueChanged is called whenever the user changes one of the controls in the User Interface (UI)
	effect->setParameterAutomated (pControl->getTag (), pControl->getValue ());
}

//------------------------------------------------------------------------------------
void UIEditor::setParameter (VstInt32 index, float value)
{
	//-- setParameter is called when the host automates one of the effects parameter.
	//-- The UI should reflect this state so we set the value of the control to the new value.
	//-- VSTGUI will automaticly redraw changed controls in the next idle (as this call happens to be in the process thread).
	if (frame && index < kNumParameters)
	{
		controls[index]->setValue (value);
	}
}