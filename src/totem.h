#include <Statistic.h>
#include "util.h"
#include "audio.h"
#include "input.h"
#include "leds.h"
#include "plot.h"
#include "log.h"

volatile bool fRed = false;

void toggleRed(void* data)
{
	fRed = !fRed;
}

class Totem
{
public:
	LedStrip strip;
	Audio audio;
	Input input;
	Plot plot;
	Log log;

	Totem() :
		strip(23),
		audio(),
		input(16),
		plot(false),
		log(false)
	{
		setup();
	}

	void setup()
	{
		for (uint i = 0; i < cSmp; ++i)
			samples[i] = 0;

		iSmp = 0;

		pinMode(21, INPUT_PULLUP);
		input.attachBtnPressHandler(21, toggleRed, nullptr);
	}

	void loop()
	{
		breathe();
	}

	// Config
	static const uint cSmp = 43;
	static constexpr double maxBrightness = .3;

	// State
	uint frame = 0;
	bool wasLoud = false;
	elapsedMillis millis;

	uint iSmp;
	double samples[cSmp];

	static const uint msPerBeat = 35;
	uint msLastBeat = 0;

	void debugXYZ()
	{
		float x = dmap(input.AnalogRead(A20), 0, 1, -120, 120);
		float y = dmap(input.AnalogRead(A19), 0, 1, 0, 240);
		float z = dmap(input.AnalogRead(A18), 0, 1, -120, 120);
		strip.globalAxis(5, x, y, z, 23, Colr::Red);
	}

	bool fUp = true;
	bool fWasGap = false;
	void breathe()
	{
	//	const uint totalTime = 8174; // 6th octave below schumann
	//	const uint duration = 6539;
		const uint totalTime = input.AnalogReadInt(A20, 6) << 5; // 6th octave below schumann
		const uint duration = totalTime;

	//	float hue = input.AnalogRead(A20);
		float brightness = .5;//input.AnalogRead(A18);
		uint hueDuration = input.AnalogReadInt(A19, 6) << 5;

		float maxY = 233;
		float width = 200;//dmap(input.AnalogRead(A18), 0, 1, 100, 400);

	//	log << "20:" << hueDuration << ", 19: " << brightness << "\n";

		uint millisHue = millis % hueDuration;
		float hue = millisHue / (float)hueDuration;

		uint phase = millis % totalTime;

		uint modShared = hueDuration * totalTime;
		if (millis > modShared)
			millis -= modShared;

		log
			<< "dur: " << totalTime
			<< ", hueDur: " << hueDuration
			<< ", time: " << millis
			<< ", phase: " << phase
			<< ", hue: " << millisHue
			<< ", shared: " << modShared
			<< "\n";

		float height = 10000;
		if (phase < duration)
		{
			if (fWasGap)
			{
				//fUp = !fUp;
				fWasGap = false;
			}
			float fr = phase / (double)duration;
			if (!fUp)
				fr = 1 - fr;
			float theta = mapfr(fr, -PI/2, PI/2);
			height = dmap(sin(theta), -1, 1, -width, maxY+width);
		}
		else
			fWasGap = true;

bool isLoud;
		Colr colr;
		if (!fRed)//digitalRead(21))
			//colr = Colr::Hue(hue);
			colr = Colr::Blue;
		else
			colr = Colr::Red;

		//brightness *= readAudio(isLoud);
		strip.globalAxis(0, 500,
			100,//height,
			500, width, colr*brightness);
		delay(10);


		/*
		Math.easeOutCubic =

		function (t, b, c, d) {
			/*t /= d;
			t--;
			return c*(t*t*t + 1) + b;
		};*/
	}

	float readAudio(bool& isLoud)
	{
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

		isLoud = energy > threshold;
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

		return  dmap(energy,
			avg - (plot.smoothed - avg),
			plot.smoothed,
			.2, 1);
	}

	void displayAudio()
	{
		bool isLoud;
		float light = readAudio(isLoud);
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
