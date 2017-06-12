#ifndef audio_h
#define audio_h

#include "input.h"

class Audio
{
private:
	Input& input;

	IntervalTimer sampleTimer;
	uint msLastBeat = 0;
	elapsedMillis millis;

public:
	Audio(Input& input);

	float GetEnergy(bool& isLoud);

// private:
	uint ReadAudioValues(uint binStart, uint binEnd);
};

#endif
