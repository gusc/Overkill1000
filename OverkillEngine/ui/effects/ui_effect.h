#ifndef __ui_effect_h
#define __ui_effect_h

#include "ui\ui_device.h"
#include "engine\effects\effect.h"

class UIEffect : public UIDevice {

public:

	UIEffect(Effect *);
	~UIEffect();

	Effect *getFx();

protected:

	Effect *_fx;

};

#endif
