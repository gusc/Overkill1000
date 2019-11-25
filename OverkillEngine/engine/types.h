#ifndef __types_h
#define __types_h

const double k12th2 = 1.0594630943592952645618252949463; // 12th root of 2
const double pi = 3.1415926535897932384626433832795; // PI

/// Channel counter
enum Channels {
	kChanLeft = 0,
	kChanRight,
	kNumChan
};
/// Envelope types
enum EnvType {
	kEnvNone = 0,
	kEnvDAHDSR = 1,
	kEnvStepSeq = 2
};
/// Oscilator types
enum OscType {
	kOscNone = 0,
	kOscGen = 1,
	kOscSample = 2,
	kOscWTable = 3,
	kOscGranular = 4
};
/// Effect types
enum FxType {
	kFxNone = 0,
	kFxDelay = 1,
	kFxFilter = 2
};
/// Playback phase
enum PlaybackPhase {
	/// Active phase
	kPhaseActive = 0,
	/// Release phase
	kPhaseRelease = 1,
	/// End phase
	kPhaseEnd = 2
};
/// Note structure
typedef struct {
	int num;
	float freq;
	float velocity;
	float sampleCounter;
	float releaseTime;
	int phase;
} t_Note;

#endif
