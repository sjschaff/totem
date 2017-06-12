#include "audioSimple.h"


AudioSimple::AudioSimple(LedStrip& strip) : strip(strip) {}

void AudioSimple::Update(ulong ms, float audio, bool isLoud)
{
	LightStrip(audio, isLoud);
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
