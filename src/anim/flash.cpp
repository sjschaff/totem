#include "anims.h"

AnimFlash::AnimFlash() : wasLoud(false), iColr(0) {}

void AnimFlash::Update(LedStrip& strip, Frame frame)
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
