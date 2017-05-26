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
		for (uint i = 0; i < cSmpMax; ++i)
			samples[i] = samplesSmooth[i] = 0;

		iSmp = 0;
	}

	// Config
	static const uint cSmpMax = 1024;
	 uint cSmp = 42;
	static const uint cReadback = 13;
	 double threshold = 1.2;
	static constexpr double maxBrightness = .35;

	// State
	uint frame = 0;
	bool wasLoud = false;
	elapsedMillis millis;

	uint iSmp;
	double samples[cSmpMax];
	double samplesSmooth[cSmpMax];


	void loop()
	{
		//strip.spinStrip(Colr::Red * .3, millis);
		//return;

		double raw = audio.ReadPeakToPeak();

		//cReadback = plotA5+1; // 13
		double wt = 1 - 1.0/cReadback;

		samples[iSmp] = raw;

		double sound = 0;
		for (uint i = 0; i <= cReadback; ++i)
		{
			double smp = samples[iSmp - (cReadback - i)];
			sound = sound * wt + smp * (1- wt);
		}

		samplesSmooth[iSmp] = sound;

		cSmp = input.AnalogReadInt(A20, 6) + 32;
		//(analogRead(A20) >> 10) + 32;
		log.println(cSmp);

		Statistic stats;
		for (uint i = 0; i < cSmp; ++i)
		{
			int iSmpAvg = (iSmp - 1 - i + cSmpMax) % cSmpMax;
			stats.add(samplesSmooth[iSmpAvg]);
		}

		iSmp = (iSmp + 1) % cSmpMax;


		threshold = dmap(analogRead(A19) >> 8, 0, 256, 1, 3);
		double avg = stats.average();
		double std = stats.pop_stdev();
		double thresh = avg + std * threshold;

		plot.smoothed = sound;
		plot.smoothedRel = sound - avg;
		if (sound > thresh)
		{
		//	sound = 1;
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

		sound = 1;

		LightStrip(sound*maxBrightness);

		plot.val = raw;
		plot.avg = avg; // this is nan every once in a while
		plot.std = thresh - avg;
		plot.plot();
	}

	void LightStrip(double intensity)
	{
		Colr colr = frame == 0
			? Colr::Red
			: frame == 1
				? Colr::Green
				: Colr::Blue;

		colr *= intensity;
		strip.spinStrip(colr, millis);
		//strip.setStripColor(colr);
	}
};
