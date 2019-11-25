#include "engine.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <Windows.h>

bool Engine::generate(float *buffer_l, float *buffer_r, int length){
	int j, l;
	float sample = 0.0;
	float volume;
	float *out_l, *out_r;
	t_Note *note;

	// Empty output buffer
	memset((void *)buffer_l, 0, sizeof(float) * length);
	memset((void *)buffer_r, 0, sizeof(float) * length);

	if (length > 0){
		// Generate Delay, Attack, Hold, Decay & Sustain phases (Active notes)
		_midi->restartReadActive();
		while (_midi->readNextActive(&note)){
			if (note->velocity > 0.0){
				l = length;
				out_l = buffer_l;
				out_r = buffer_r;
				
				while (--l >= 0){
					sample = 0.0;
					volume = _volume * note->velocity;
					
					if (note->sampleOffset > 0){
						// Note starts slightly off the begining of the buffer
						note->sampleOffset --;
					} else {
						for (j = 0; j < kMaxEnv; j ++){
							switch (_env[j]->type()){
							case kEnvDAHDSR:
								{
									EnvDAHDSR *e = (EnvDAHDSR *)_env[j];
									volume *= e->getAmplitude(note->sampleCounter, kEnvPhaseActive);
								}
								break;
							}
							
						}
						for (j = 0; j < kMaxOsc; j ++){
							switch (_osc[j]->type()){
							case kOscGen:
								{
									OscGen *o = (OscGen *)_osc[j];
									sample += o->getSample(note->sampleCounter, note->freq, 1.0);
									sample *= volume;
								}
								break;
							case kOscWTable:
								break;
							case kOscGranular:
								break;
							}
						}

						note->sampleCounter += 1.0;
					}
					(*out_l++) += sample;
					(*out_r++) += sample;
				}
			} else {

			}
		}
		// Generate Release (and maybe stuck notes)
		_midi->restartReadReleased();
		while (_midi->readNextReleased(&note)){
			if (note->velocity > 0.0){
				l = length;
				out_l = buffer_l;
				out_r = buffer_r;
				
				while (--l >= 0){
					sample = 0.0;
					volume = _volume * note->velocity;
					
					for (j = 0; j < kMaxEnv; j ++){
						switch (_env[j]->type()){
						case kEnvDAHDSR:
							{
								EnvDAHDSR *e = (EnvDAHDSR *)_env[j];
								volume *= e->getAmplitude(note->releaseCounter, kEnvPhaseRelease);
							}
							break;
						}
							
					}
					for (j = 0; j < kMaxOsc; j ++){
						switch (_osc[j]->type()){
						case kOscGen:
							{
								OscGen *o = (OscGen *)_osc[j];
								sample += o->getSample(note->sampleCounter, note->freq, 1.0);
								sample *= volume;
							}
							break;
						case kOscWTable:
							break;
						case kOscGranular:
							break;
						}
					}
					(*out_l++) += sample;
					(*out_r++) += sample;

					note->sampleCounter += 1.0;
					note->releaseCounter += 1.0;
				}
			}
		}
	}

	return true;
}
