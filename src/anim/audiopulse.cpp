#include "audiopulse.h"
#include "log.h"

Beat::Beat(ulong msStart) : msStart(msStart) {}

struct AnimSpin
{
	void Display(LedStrip& strip, Frame frame, float phase)
	{
		ForEachLed(iLed)
		{
			Led led = strip.leds[iLed];

			if (led.face.iRing != 2 && led.face.iRing != 1)
			{
				strip.SetColor(iLed, Colr::Black);
				continue;
			}

			Colr colr = Colr::Blue;
			float sf = 1.8;//4 * input.ReadC();
			float delta = led.face.frPolarDelta;
			float polar = led.face.frPolar;
			if (led.face.iRing == 1)
			{
					sf *= 1;
					polar = 1 - polar;
					colr = Colr::Red;
			}

			float width = sf*delta;
			float dist = modDelta(phase, polar) / width;
			float intens = smoothstepDual(dist);
			strip.SetColor(iLed, colr*intens);//*audio);
			continue;


			uint iCol = led.face.iRing % 3;
			switch (iCol)
			{
				case 0: colr = Colr::Green; break;
				case 1: colr = Colr::Black; break;//Blue; break;
				case 2: default: colr = Colr::Red; break;
			}

			strip.SetColor(iLed, colr);
		}
	}
};

struct AnimFlash : public Anim
{
private:
	bool wasLoud;
	uint iColr;

public:
	AnimFlash() : wasLoud(false), iColr(0) {}

	void Update(LedStrip& strip, Frame frame)
	{
		bool isLoud = frame.audio.isBeat;
		float audio = frame.audio.energy;
		if (isLoud)
		{
			//audio = 1;
			if (!wasLoud)
			{
				iColr = (iColr + 1) % 3;
				wasLoud = true;
			}
		}
		else
		{
			//audio = 0;
			wasLoud = false;
		}

	//	audio = 1;

		Colr colr = iColr == 0
			? Colr::Red
			: iColr == 1
				? Colr::Green
				: Colr::Blue;
	//colr = Colr::Red;
		colr *= audio;
		//strip.spinStrip(colr, millis);
		strip.SetStripColor(colr);
	}
};

struct AnimRing : public Anim
{
	void Update(LedStrip& strip, Frame frame)
	{
		ForEachLed(iLed)
		{
			Led led = strip.leds[iLed];
			if (led.face.iRing == 2)
			{
				strip.SetColor(iLed, Colr::Green * frame.audio.energy);
			}
			else
			{
				strip.SetColor(iLed, Colr::Black);
			}
		}
	}
};

struct AnimYRing
{
	void Display(LedStrip& strip, Frame frame, float phase)
	{
		float phaseColor = phase;
		float frHue = smoothstepDual(phaseColor*2 - 1);
		phase = frac(phase*4);
		ForEachLed(iLed)
		{
			Led led = strip.leds[iLed];
			Colr colr = Colr::Black;

			if (led.rad > 50)
			{
				float width = .2;
			//	phase = frame.knobB;
				float dist = modDelta(phase, led.frPolar) / width;
				float intens = smoothstepDual(dist);
				colr =
					Colr::Lerp(
						Colr::Hue(1.f/24.f),
						Colr::Hue(1.f/24.f +.5f),
						frHue) * intens;
			}

			strip.SetColor(iLed, colr);
		}
	}
};

struct AnimRings
{
	uint iPhase = 0;
	uint iPhaseTop = 5;
	bool wasLoud = false;

	void Display(LedStrip& strip, Frame frame, float phase)
	{
		//if (frame.audio.isBeginBeat)
			//iPhase = (iPhase + 1) % 4;

		bool isLoud = frame.audio.isBeat;
		if (isLoud)
		{
			//audio = 1;
			if (!wasLoud)
			{
				iPhase = (iPhase + 1) % 6;
				iPhaseTop = (iPhaseTop + 1) % 12;
				wasLoud = true;
			}
		}
		else
		{
			//audio = 0;
			wasLoud = false;
		}

		ForEachLed(iLed)
		{
			Led led = strip.leds[iLed];
			Colr colr = Colr::Black;

		/*	float width = mapfr(frame.knobC, .25, 2);
			float rad = mapfr(phase, -width, 3+width);
			float dist = abs(rad - led.face.frRad) / width;
			float intens = smoothstepDual(dist);
			intens = 1 - saturate(dist);
			strip.SetColor(iLed, Colr::Green * intens);*/
			//uint iPhase = phase * 3.999;
			uint iFaceTop = (uint)(phase * 4.999) % 5;
			float frIFaceTop = frac(phase*4.999);
			float hue = iPhaseTop / 6.f;
			if (led.face.iRing > 3)
			{
				uint iFaceOut = iFaceTop;
				uint iFaceIn = (iFaceTop + 1) % 5;

				if (led.face.iRing == 6)
					hue += .5;

				colr = Colr::Hue(hue);
				float smth = smoothstep(frIFaceTop); // move out of loop
				if (led.iFace == iFaceOut)
					colr *= 1 - smth;
				else if (led.iFace == iFaceIn)
					colr *= smth;
				else
					colr = Colr::Black;
			} else if (led.face.iRing == (iPhase % 3))
				colr = Colr::Hue(hue + (iPhase % 2)*.5) * frame.audio.energy;

			strip.SetColor(iLed, colr);
		}
	}
};


// vertical pulse (rings moving up)
// rnd face pulse (color+compl on inner outer rings)
// TODO Shimmer type effect -> random leds on/off
// TODO revive breathe, add to Chill Mode

// buttons:
// next mode
// rage mode?
// disable knobs
// per mode:
// 	next color
// 	next anim
// 	color mode -> random, fixed dual, fixed single, rainbow?

// knobs
// brightness
// color

AudioPulse::AudioPulse(LedStrip& strip, Input& input)
	: Mode(strip, input)
{
	anims.push_back(
		new AnimSmp<PhaseLinear, AnimYRing>(
			PhaseLinear(4000*4),
			AnimYRing()));

	anims.push_back(
		new AnimSmp<PhasePulse, AnimYRing>(
			PhasePulse(.125, .5), // TODO Slow speed for this is perfect for chill mode
			AnimYRing()));

	anims.push_back(
		new AnimSmp<PhasePulse, AnimYRing>(
			PhasePulse(.25, 2), // TODO Slow speed for this is perfect for chill mode (account for 4x tho)
			AnimYRing()));


	anims.push_back(
		new AnimSmp<PhaseLinear, AnimRings>(
			PhaseLinear(1000),
			AnimRings()));

	anims.push_back(new AnimRing());

	anims.push_back(
		new AnimSmp<PhasePulse, AnimSpin>(
			PhasePulse(.5, 4),
			AnimSpin()));

	anims.push_back(new AnimFlash());

	iAnim = 0;
}

void AudioPulse::Update(Frame frame)
{
	if (frame.audio.isBeginBeat)
		beats.push_back(Beat(frame.ms));

	Anim* anim = anims[iAnim];
	anim->Update(strip, frame);
	strip.Show();
}

void AudioPulse::OnPressE()
{
	iAnim = (iAnim + 1) % anims.size();
}
