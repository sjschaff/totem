#include "anims.h"

void AnimSpin::Display(LedStrip& strip, Frame frame, float phase)
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
