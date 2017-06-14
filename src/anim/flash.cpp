#include "anims.h"

AnimFlash::AnimFlash() : wasLoud(false), iColr(0) {}

void AnimFlash::Update(LedStrip& strip, Frame frame)
{
	bool isLoud = frame.audio.isBeat;

	// TODO: move to audio
	if (isLoud)
	{
		if (!wasLoud)
		{
			iColr = (iColr + 1) % 2;
			wasLoud = true;
		}
	}
	else
	{
		wasLoud = false;
	}

	ForEachLed(iLed)
	{
		Led led = strip.leds[iLed];
		float frColStatic = led.face.frRad <= 1 ? led.face.frRad : (3 - led.face.frRad) / 2.f;
		Colr colr = frame.colr->GetColr(iColr, frColStatic, 0);
		colr *= frame.audio.energy * .75;
		strip.SetColor(iLed, colr);
	}
	/*Colr colr = iColr == 0
		? Colr::Red
		: iColr == 1
			? Colr::Green
			: Colr::Blue;*/
//colr = Colr::Red;
	//strip.spinStrip(colr, millis);
	//strip.SetStripColor(colr);
}
