#ifndef audio_h
#define audio_h

#include "input.h"

class Plot;

class Audio
{
private:
	Input& input;

	IntervalTimer sampleTimer;
	uint msLastBeat = 0;
	elapsedMillis millis;

public:
	Audio(Input& input);

	float GetEnergy(bool& isBeat, bool& isBeginBeat, Plot* plot);

// private:
	void SetDecay(uint decay);
	float SampleAudioValues(uint binStart, uint binEnd);
	float SampleAudioValues();
};

#endif
