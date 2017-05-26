#include <Statistic.h>
#include "util.h"
#include "audio.h"
#include "input.h"
#include "leds.h"
#include "plot.h"
#include "log.h"

typedef LedStrip Led;

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
		plot(true),
		log(false)
	{
		setup();
	}

	void setup()
	{
		for (uint i = 0; i < cSmp; ++i)
			samples[i] =0;

		iSmp = 0;
	}

	// Config
	static const uint cSmp = 43;
	static constexpr double maxBrightness = .35;

	// State
	uint frame = 0;
	bool wasLoud = false;
	elapsedMillis millis;

	uint iSmp;
	double samples[cSmp];

	void loop()
	{
		double energy = audio.ReadEnergy(3);
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
		plot.val = energy;
		plot.avg = avg;
		plot.std = variance;
		double C = -.0025714 * variance + 1.5142857;
		plot.smoothed = avg * C;
		plot.plot();

		samples[iSmp] = energy;
		iSmp = (iSmp + 1) % 43;

		double light = dmap(energy,
			avg - (plot.smoothed - avg),
			plot.smoothed,
			.2, 1);

		LightStrip(light, plot.val > plot.smoothed);
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
