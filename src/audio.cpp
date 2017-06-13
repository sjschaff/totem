#include <Statistic.h>
#include "audio.h"
#include "plot.h"
#include "log.h"

static const uint strobePin = 14;
static const uint resetPin = 15;

static const uint msPerBeat = 35;

static const uint cSmp = 43;
static volatile uint iSmp = 0;
static volatile float samples[cSmp];

static  uint decay = 2;//1;

static Audio* audio;
void sampleAudio()
{
	float sample = audio->SampleAudioValues();
  	noInterrupts();
	samples[iSmp] = sample;
	iSmp = (iSmp + 1) % cSmp;
  	interrupts();
}

void Audio::SetDecay(uint _decay)
{
	decay = _decay;
}

Audio::Audio(Input& input) : input(input)
{
	pinMode(resetPin, OUTPUT);
	pinMode(strobePin, OUTPUT);

	audio = this;

	msLastBeat = 0;
	for (uint i = 0; i < cSmp; ++i)
		samples[i] = 0;

	// This should probably be last
	sampleTimer.begin(sampleAudio, 1000.f*1000.f/cSmp);
}


float Audio::SampleAudioValues()
{
	return SampleAudioValues(0, 3);
}

float Audio::SampleAudioValues(uint binStart, uint binEnd)
{
	digitalWrite(strobePin, LOW);
	digitalWrite(resetPin, HIGH);
	delayMicroseconds(1);
	digitalWrite(resetPin, LOW);

	float values[7];
	float accum = 0;
	for (uint iDecay = 0; iDecay < decay; ++iDecay)
	{
		for (uint i = 0; i < 7; ++i)
		{
			digitalWrite(strobePin, HIGH);
			delayMicroseconds(40);
			digitalWrite(strobePin, LOW);
			delayMicroseconds(20);
			float value = input.AnalogRead(A22, 0, 1);
			if (iDecay == 0)
			{
				values[i] = value;
				if (i >= binStart && i < binEnd)
					accum += value;
			}

			delayMicroseconds(20);
		}
	}

/*
	for (int i = 0; i < 7; ++i)
		(*Log::log) << values[i] << ", ";
	(*Log::log) << "\n";
		delay(500);
*/
	return 1024.f * accum / (binEnd - binStart);
}

float Audio::GetEnergy(bool& isBeat, bool& isBeginBeat, Plot* _plot)
{
	Plot& plot = *_plot;
	float energy = samples[iSmp];
	Statistic stats;
	for (uint i = 0; i < cSmp; ++i)
		stats.add(samples[i]);
	float avg = stats.average();

	float variance = 0;
	for (uint i = 0; i < cSmp; ++i)
	{
		float delta = samples[i] - avg;
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

	isBeat = true;
	isBeginBeat = false;
	bool wasBeat = (millis - msLastBeat) < msPerBeat;

	if (isLoud)
		msLastBeat = millis;

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

	return saturate((millis - msLastBeat) / (4.f*msPerBeat));
	return dmap(energy,
		avg - (smoothed - avg),
		smoothed,
		.2, 1);
}
