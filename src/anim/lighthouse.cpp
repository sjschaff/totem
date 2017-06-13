#include "anims.h"

void AnimLighthouse::Display(LedStrip& strip, Frame frame, float phase)
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
