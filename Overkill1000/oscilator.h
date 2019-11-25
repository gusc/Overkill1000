#ifndef __oscilator_h
#define __oscilator_h

#ifdef __cplusplus
extern "C" {
#endif

float osc_sine(float *buffer, int bufferSize, float samplePos, float impulseLength, float pulseWidth);
float osc_square(float *buffer, int bufferSize, float samplePos, float impulseLength, float pulseWidth);
float osc_tri(float *buffer, int bufferSize, float samplePos, float impulseLength, float pulseWidth);
float osc_noise(float *buffer, int bufferSize, float samplePos, float impulseLength, float pulseWidth);
float osc_wavetable(float *buffer, int bufferSize, float samplePos, float impulseLength, float *table, int tableSize); 

#ifdef __cplusplus
}
#endif

#endif
