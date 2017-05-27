#include <Statistic.h>
#include "util.h"
#include "audio.h"
#include "input.h"
#include "leds.h"
#include "plot.h"
#include "log.h"

class Totem
{
public:
	LedStrip strip;
	Audio audio;
	Input input;
	Plot plot;
	Log log;

	Totem() :
		strip(3),
		audio(),
		input(16),
		plot(false),
		log(true)
	{
		setup();
	}

	void setup()
	{
		for (uint i = 0; i < cSmp; ++i)
			samples[i] = 0;

		iSmp = 0;
	}

	// Config
	static const uint cSmp = 43;
	static constexpr double maxBrightness = .2;

	// State
	uint frame = 0;
	bool wasLoud = false;
	elapsedMillis millis;

	uint iSmp;
	double samples[cSmp];

	static const uint msPerBeat = 35;
	uint msLastBeat = 0;
	void loop()
	{
		float x = dmap(input.AnalogRead(A20), 0, 1, -120, 120);
		float y = dmap(input.AnalogRead(A19), 0, 1, 0, 240);
		float z = dmap(input.AnalogRead(A18), 0, 1, -120, 120);

		//strip.rainbowFace(0);

		strip.globalAxis(5, x, y, z, 23);
		//strip.rainbowFace(0);
/*
		strip.rainbowFaceLinear(5,
			input.AnalogRead(A20),
			input.AnalogRead(A19));*/

		strip.show();

		plot.x = x;
	//	plot.w = w;
		plot.plot();
		delay(2.5);
		return;



		double energy = audio.ReadEnergy(10);
		Statistic stats;
		for (uint i = 0; i < cSmp; ++i)
			stats.add(samples[i]);
		double avg = stats.average();

		double variance = 0;
		for (uint i = 0; i < cSmp; ++i)
		{
			double delta = samples[i] - avg;
			variance += delta*delta;
		}
		variance /= 43.0;

		double C = -.0025714 * variance + 1.5142857;
		double threshold = avg * C;

		samples[iSmp] = energy;
		iSmp = (iSmp + 1) % 43;

		bool isBeat = true;

		bool isLoud = energy > threshold;
		bool wasBeat = (millis - msLastBeat) < msPerBeat;

		if (!wasBeat)
		{
			if (isLoud)
				msLastBeat = millis;
			else
				isBeat = false;
		}

		plot.val = energy;
		plot.avg = avg;
		plot.std = variance;
		plot.smoothed = isBeat ? .1 : 0;
		plot.plot();

		double light = dmap(energy,
			avg - (plot.smoothed - avg),
			plot.smoothed,
			.2, 1);

		LightStrip(light, isLoud);
	}

	void LightStrip(double sound, bool isLoud)
	{
		if (isLoud)
		{
			//sound = 1;
			if (!wasLoud)
			{
				frame = (frame + 1) % 3;
				wasLoud = true;
			}
		}
		else
		{
			//sound = 0;
			wasLoud = false;
		}

	//	sound = 1;

		Colr colr = frame == 0
			? Colr::Red
			: frame == 1
				? Colr::Green
				: Colr::Blue;
//colr = Colr::Red;
		colr *= sound * maxBrightness;
		//strip.spinStrip(colr, millis);
		strip.setStripColor(colr);
	}
};
