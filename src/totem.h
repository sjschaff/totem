#include "util.h"
#include "audio.h"
#include "input.h"
#include "leds.h"
#include "plot.h"
#include "log.h"

// TODO
/*
	-weird pulsing on spinner (see at .5 rps, probably error in polar coords?)
	-polar delta (for AA) for tip leds
	-incorporate mseq ic
	-animation overlay system (sep. color from animation)
	-figure out coil whine amps
	-better time mod system
*/

class Totem : public BtnHandler
{
public:
	Log log;
	Plot plot;

	Input input;
	LedStrip strip;
	Audio audio;

	void OnPressA() {OnPress('A');}
	void OnPressB() {OnPress('B');}
	void OnPressC() {OnPress('C');}
	void OnPressD() {OnPress('D');}
	void OnPressE() {OnPress('E');}
	void OnPressF() {OnPress('F');}

	Totem() :
		log(true), plot(true),
		input(this), audio(input)
	{ }

	void OnPress(char c)
	{
		log << "Button " << c << " Pressed. ["
			<< input.ReadA() << ", "
			<< input.ReadB() << ", "
			<< input.ReadC() << "].\n";
	}

	void loop()
	{
		input.PollInput();
		strip.SetBrightness(input.ReadA(.2, .85)); // TODO make anims more sparing with simultaneous lit leds
		//spin();
		displayAudio();
		//breathe();
	}

	// State
	uint frame = 0;
	bool wasLoud = false;
	elapsedMillis millis;

	void spin()
	{
		const float colPerSec = .05;
		const uint loopMod = 30000;
		if (millis > loopMod)
			millis -= loopMod;
		float frCol = millis * .001 * colPerSec;
		frCol = frac(frCol);
		Colr colr = Colr::Hue(frCol);
			//Colr(1,1,1);
		Colr colCenter = Colr::Hue(frac(frCol+5));
		strip.SpinStrip(colr, colCenter, millis, 1.5);
		delay(16);
	}

	void debugXYZ()
	{
		float x = dmap(input.ReadA(), 0, 1, -120, 120);
		float y = dmap(input.ReadB(), 0, 1, 0, 240);
		float z = dmap(input.ReadC(), 0, 1, -120, 120);
		strip.GlobalAxis(5, x, y, z, 23, Colr::Red);
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

		Colr colr;//digitalRead(21))
			//colr = Colr::Hue(hue);
			colr = Colr::Blue;

		strip.GlobalAxis(0, 500,
			100,//height,
			500, width, colr);
		delay(10);


		/*
		Math.easeOutCubic =

		function (t, b, c, d) {
			/*t /= d;
			t--;
			return c*(t*t*t + 1) + b;
		};*/
	}

	void displayAudio()
	{
		uint dec = (uint)input.ReadC(1, 10);
		plot.knobC = dec;
	//	audio.SetDecay(dec);
		bool isLoud;
		float light = audio.GetEnergy(isLoud, &plot);
		LightStrip(light, isLoud);
		delay(23);
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
		colr *= sound;
		//strip.spinStrip(colr, millis);
		strip.SetStripColor(colr);
	}
};
