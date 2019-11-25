#include <engine\engine.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <Windows.h>

bool Engine::generate(float *buffer_l, float *buffer_r, int length){
	int j, l;
	float sample = 0.0;
	float *out_l, *out_r;
	t_Note *note;
	int noteEnd = 0;

	// Empty output buffer
	memset((void *)buffer_l, 0, sizeof(float) * length);
	memset((void *)buffer_r, 0, sizeof(float) * length);

	EnterCriticalSection(&_cs);

	if (length > 0){
		// Process MIDI -> Audio
		_midi->restart();
		while (_midi->read(&note)){
			if (note->velocity > 0.0 && note->phase != kPhaseEnd){
				l = length;
				out_l = buffer_l;
				out_r = buffer_r;
				
				while (--l >= 0){
					sample = 0.0f;
					noteEnd = 0;
					for (j = 0; j < _oscCount; j ++){
						sample += _osc[j]->generateSample(note);
					}
					//todo: move this to generators
					sample *= note->velocity;
					if (noteEnd >= _oscCount){
						note->phase = kPhaseEnd;
					} else {
						note->sampleCounter += 1.0f;
					}

					(*out_l++) += sample;
					(*out_r++) += sample;
				}
			}
		}
		
		// Process SFX
		/*
		AudioBuffer *bl = new AudioBuffer(buffer_l, length);
		AudioBuffer *br = new AudioBuffer(buffer_r, length);
		for (j = 0; j < _fxCount; j ++){
			_fx[j]->processSampleBuffer(bl);
			_fx[j]->processSampleBuffer(br);
		}
		delete bl;
		delete br;
		*/

		l = length;
		out_l = buffer_l;
		out_r = buffer_r;
		sample = 0.0f;
		while (--l >= 0){
			sample = *out_l;
			for (j = 0; j < _fxCount; j ++){
				_fx[j]->processSample(&sample);
			}
			if (sample > 0.999999999f){
				sample = 0.999999999f;
			} else if (sample < -0.999999999f){
				sample = -0.999999999f;
			}
			(*out_l++) = sample;
			(*out_r++) = sample;
		}

		//Todo: force limiter
	}

	LeaveCriticalSection(&_cs);

	return true;
}
