#include <Statistic.h>
#include "audio.h"

static const uint strobePin = 14;
static const uint resetPin = 15;

static const uint msPerBeat = 35;

static const uint cSmp = 43;
static volatile uint iSmp = 0;
static volatile float samples[cSmp];

static Audio* audio;
void sampleAudio()
{
	float sample = audio->ReadAudioValues(0, 2);
  	noInterrupts();
	samples[iSmp] = sample;
	iSmp = (iSmp + 1) % cSmp;
  	interrupts();
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

uint Audio::ReadAudioValues(uint binStart, uint binEnd)
{
	digitalWrite(strobePin, LOW);
	digitalWrite(resetPin, HIGH);
	delayMicroseconds(1);
	digitalWrite(resetPin, LOW);

	uint values[7];
	uint value = 0;
	for (uint i = 0; i < binEnd; ++i)
	{
		digitalWrite(strobePin, HIGH);
		delayMicroseconds(40);
		digitalWrite(strobePin, LOW);
		delayMicroseconds(20);
		values[i] = input.AnalogReadInt(A22, 10);
		if (i >= binStart)
			value += values[i];

		delayMicroseconds(20);
	}

	return value;

	/*	for (int i = 0; i < 7; ++i)
			log << values[i] << ", ";
		log << "\n";
	*///	delay(500);
}

float Audio::GetEnergy(bool& isLoud)
{
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
	variance /= (float)cSmp;

	float C = -.0025714 * variance + 1.5142857;
	float threshold = avg * C;
	isLoud = energy > threshold;

	bool isBeat = true;
	bool wasBeat = (millis - msLastBeat) < msPerBeat;

	if (!wasBeat)
	{
		if (isLoud)
			msLastBeat = millis;
		else
			isBeat = false;
	}

	float smoothed = isBeat ? .1 : 0; // ?

/*	plot.val = energy;
	plot.avg = avg;
	plot.std = variance;
	plot.smoothed = smoothed;
	plot.plot();*/

	return dmap(energy,
		avg - (smoothed - avg),
		smoothed,
		.2, 1);
}
