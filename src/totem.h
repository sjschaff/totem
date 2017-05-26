
#include "util.h"
#include "audio.h"
#include "leds.h"
typedef LedStrip Led;

#include <Plotter.h>
Plotter plot;

#include <Statistic.h>
Statistic stats;


double plotVal = 0;
double plotSmoothed = 0;
double plotSmoothed2 = 0;
double plotAvg = 0;
double plotStd = 0;
double plotThresh = 0;

double plotA5 = 50;
double plotA4 = 0;
double plotMax = 1;
double plotMin = -1;
double anPlotMin = 0;


class Totem
{
public:
	LedStrip strip;
	Audio audio;

public:
	Totem() : strip(3), audio()
	{
		setup();
	}

	void setup() {
		return;

		Serial.begin(9600);
		plot.Begin();
		plot.AddTimeGraph("Audio Sensor", 500,
			//"val", plotVal,
			"smooth", plotSmoothed,
			"smooth rel", plotSmoothed2,
			"avg", plotAvg,
			"std", plotStd,
		 // "thresh", plotThresh,
			//"max", plotMax,
			"min", anPlotMin
			);

		/*	plot.AddTimeGraph("Analog Input", 50,
			"A5", plotA5,
			"A4", plotA4,
			"max", plotMax,
			"min", anPlotMin
			);*/

	}

	bool loudBefore = false;
	//double threshold = 3;//290;
	double influence = .5;//

	elapsedMillis millis;


	void loop()
	{
		strip.spinStrip(Colr::Red * .3, millis);
		return;
		const int cSmpMax = 1024;
		double samples[cSmpMax];
		double samplesSmooth[cSmpMax];
		for (int i = 0; i < cSmpMax; ++i) samples[i] = samplesSmooth[i] = 0;
		uint iSmp = 0;

		bool wasLoud = false;
		uint frame = 0;

		// config
		uint cSmp = 42;
		uint cReadback = 13;
		double threshold = 1.2;

		while(true)
		{
			double raw = audio.ReadPeakToPeak();

			double maxBrightness = .4;

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


			cSmp = (analogRead(A20) >> 10) + 32;

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

			plotSmoothed = sound;
			plotSmoothed2 = sound - avg;
			if (sound > thresh)
			{
				sound = 1;
				if (!wasLoud)
				{
					frame = (frame + 1) % 3;
					wasLoud = true;
				}
			}
			else
			{
				sound = 0;
				wasLoud = false;
			}


			Colr colr = frame == 0
				? Colr::Red
				: frame == 1
					? Colr::Green
					: Colr::Blue;

			colr *= (maxBrightness * sound);

			for (ushort i = 0; i < Led::cLED; ++i)
				strip.setColor(i, colr);

			strip.show();



			plotVal = raw;
			//plotSmoothed = sound;
			//plotSmoothed2 = sound - avg;
			plotAvg = avg; // this is nan every once in a while
			plotStd = thresh - avg;

			stats.clear();
			plot.Plot();

			yield();
		}
	}


};
