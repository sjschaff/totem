#include <Statistic.h>
#include "audio.h"

static const uint strobePin = 14;
static const uint resetPin = 15;
static const uint cSmp = 43;
static const uint cChannel = 7;

static volatile uint iSmp = 0;
static volatile float samples[cSmp][cChannel];
static volatile bool fSampleReady = false;

static const uint msPerBeat = 35;
static const uint decay = 2;//1;

static Audio* audio;
void sampleAudio()
{
	float samplesNew[cChannel];
	audio->SampleAudioValues(samplesNew);

  	noInterrupts();
	for (uint i = 0; i < cChannel; ++i)
		samples[iSmp][i] = samplesNew[i];
	iSmp = (iSmp + 1) % cSmp;
	fSampleReady = true;
  	interrupts();
}

float getSample(uint smp, uint binMin, uint binMax)
{
	float accum = 0;
	for (uint i = binMin; i < binMax; ++i)
		accum += samples[smp][i];

	return 1024.f * accum / (binMax - binMin);
}
/*
void Audio::SetDecay(uint _decay)
{
	decay = _decay;
}*/

Audio::Audio(Input& input) : input(input)
{
	pinMode(resetPin, OUTPUT);
	pinMode(strobePin, OUTPUT);

	audio = this;

	msLastBeat = 0;
	for (uint i = 0; i < cSmp; ++i)
		for (uint iCh = 0; iCh < cChannel; ++iCh)
			samples[i][iCh] = 0;

	// This should probably be last
	sampleTimer.begin(sampleAudio, 1000.f*1000.f/cSmp);
}

AudioData Audio::ReadAudio(Plot* _plot)
{
	/*
	for (int i = 0; i < 7; ++i)
		(*Log::log) << values[i] << ", ";
	(*Log::log) << "\n";
		delay(500);
	*/

	while (!fSampleReady)
		delayMicroseconds(100);
	fSampleReady = false;

	// TODO: per channel data?
	float smps[cSmp];
	for (uint i = 0; i < cSmp; ++i)
		smps[i] = getSample(i, 0, 3);

	Plot& plot = *_plot;
	float energy = smps[iSmp];
	Statistic stats;
	for (uint i = 0; i < cSmp; ++i)
		stats.add(smps[i]);
	float avg = stats.average();

	float variance = 0;
	for (uint i = 0; i < cSmp; ++i)
	{
		float delta = smps[i] - avg;
		variance += delta*delta;
	}
	variance = variance / (float)cSmp;

	float C = -.0025714 * sqrt(variance) + 1.5142857;
	float threshold = avg * C;
	/*energy =
		energy * .7 +
		samples[(iSmp + cSmp - 1) % cSmp] * .3 ;*/
//		samples[(iSmp + cSmp - 2) % cSmp] * .1;
	bool isLoud = energy > threshold;

	bool isBeat = true;
	bool isBeginBeat = false;
	bool wasBeat = (millis - msLastBeat) < msPerBeat;

	if (isLoud)
		msLastBeat = millis;

	// TODO consider replacing with the simpler
	/*{
		if (isLoud)
		{
			if (!wasLoud)
			{
				isNewBeat
				wasLoud = true;
			}
		}
		else
		{
			wasLoud = false;
		}
	}*/

	if (!wasBeat)
	{
		if (isLoud)
		{
			//msLastBeat = millis;
			isBeginBeat = true;
		}
		else
			isBeat = false;
	}

	float smoothed = isBeat ? .1 : 0; // ?

	plot.val = energy;
	plot.avg = avg;
	plot.std = sqrt(variance);
	plot.smoothed = threshold;
	plot.plot();

	AudioData audio;
	audio.energy = energy;
	audio.isBeat = isBeat;
	audio.isBeginBeat = isBeginBeat;
	audio.energy = dmap(energy,
		avg - (smoothed - avg),
		smoothed,
		.2, 1); // TODO dafuq??

	audio.beatFalloff = 1 - saturate((millis - msLastBeat) / (4.f*msPerBeat));
	return audio;
}

void Audio::SampleAudioValues(float* samplesOut)
{
	digitalWrite(strobePin, LOW);
	digitalWrite(resetPin, HIGH);
	delayMicroseconds(1);
	digitalWrite(resetPin, LOW);

	for (uint iDecay = 0; iDecay < decay; ++iDecay)
	{
		for (uint i = 0; i < cChannel; ++i)
		{
			digitalWrite(strobePin, HIGH);
			delayMicroseconds(40);
			digitalWrite(strobePin, LOW);
			delayMicroseconds(20);
			float value = input.AnalogRead(A22);
			if (iDecay == 0)
				samplesOut[i] = value;

			delayMicroseconds(20);
		}
	}
}
