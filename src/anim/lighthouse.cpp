#include "anims.h"

template<class TPhase>
void LighthouseBase<TPhase>::Display(LedStrip& strip, Frame frame, float phase)
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
			float distA = modDelta(phase, led.frPolar) / width;
			float distB = modDelta(frac(phase + .5), led.frPolar) / width;
			float dist;
			uint iColr;
			if (distA <= .5)
			{
				iColr = 0;
				dist = distA;
			}
			else
			{
				iColr = 1;
				dist = distB;
			}

			float intens = smoothstepDual(dist);
			colr = frame.colr->GetColr(iColr, phaseColor, led.frY) * intens;
		}

		strip.SetColor(iLed, colr);
	}
}

AnimLighthouse::AnimLighthouse(float duration)
	: LighthouseBase(PhaseLinear(duration)) {}

AnimLighthousePulse::AnimLighthousePulse(PhasePulse phase)
	: LighthouseBase(phase) {}
