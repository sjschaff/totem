#include "audioSimple.h"
#include "log.h"

Beat::Beat(ulong msStart) : msStart(msStart) {}

struct AnimSpin
{
	void Display(LedStrip& strip, float phase)
	{
		ForEachLed(iLed)
		{
			Led led = strip.leds[iLed];

			if (led.face.iRing != 2 && led.face.iRing != 1)
				continue;

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

AudioPulse::AudioPulse(LedStrip& strip, Input& input)
	: Mode(strip, input)
{
	anims.push_back(
		new AnimSmp<PhasePulse, AnimSpin>(
			PhasePulse(.5, .4),
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
