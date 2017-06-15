#include "anim.h"

AnimNoop::AnimNoop(bool fClear)
	: fClear(fClear) {}

void AnimNoop::Update(LedStrip& strip, Frame frame)
{
	if (fClear)
	{
		ForEachLed(iLed)
			strip.SetColor(iLed, Colr::Black);
	}
}
