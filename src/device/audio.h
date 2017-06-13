#ifndef audio_h
#define audio_h

#include "input.h"

class Plot;

struct AudioData
{
	float energy;
	bool isBeat;
	bool isBeginBeat;
	float beatFalloff; // TODO: extract to parameterizable?
};

class Audio
{
private:
	Input& input;

	IntervalTimer sampleTimer;
	uint msLastBeat = 0;
	elapsedMillis millis;

public:
	Audio(Input& input);

	AudioData ReadAudio(Plot* plot);

// private:
	void SetDecay(uint decay);
	float SampleAudioValues(uint binStart, uint binEnd);
	float SampleAudioValues();
};

#endif
