#include "audioSimple.h"

Beat::Beat(ulong msStart) : msStart(msStart) {}

AudioPulse::AudioPulse(LedStrip& strip, Input& input) : Mode(strip, input) {}

void AudioPulse::Update(ulong ms, float audio, bool isBeat, bool isBeginBeat)
{
	if (isBeginBeat)
		beats.push_back(Beat(ms));

	//float phase = frPhase(ms, 1000);
	ulong del = ms - msPrev;
	if (isBeat)
		del *= 4;
	else
		del *= .5;

	msPhase = (msPhase + del) % 1000;

	float phase = msPhase / 1000.f;
	Spin(phase, audio, isBeat);

	strip.Show();
	msPrev = ms;
}

void AudioPulse::Spin(float phase, float audio, bool isBeat)
{
	ForEachLed(iLed)
	{
		Led led = strip.leds[iLed];

		if (led.face.iRing != 2 && led.face.iRing != 1)
			continue;

		Colr colr = Colr::Blue;
		float sf = 4 * input.ReadC();
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


AudioSimple::AudioSimple(LedStrip& strip, Input& input) : Mode(strip, input) {}

void AudioSimple::Update(ulong ms, float audio, bool isBeat, bool isBeginBeat)
{
	LightStrip(audio, isBeat);
}

void AudioSimple::LightStrip(float audio, bool isLoud)
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
	colr *= audio;
	//strip.spinStrip(colr, millis);
	strip.SetStripColor(colr);
}
