#include "overkillvst.h"

AudioEffect* createEffectInstance (audioMasterCallback audioMaster)
{
	return new OverkillVST(audioMaster);
}