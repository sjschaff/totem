#include "anims.h"

template<class TPhase>
void LighthouseBase<TPhase>::Display(LedStrip& strip, Frame frame, float phase)
{
	float phaseColor = phase;
	phase = frac(phase*4);
	if (dir)
		phase = 1 - phase;
	ForEachLed(iLed)
	{
		Led led = strip.leds[iLed];
		Colr colr = Colr::Black;

		//if (led.rad > 50)
		if (led.face.iRing < 6)
		{
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

AnimLighthouse::AnimLighthouse(bool dir)
	: LighthouseBase(dir, .185, PhaseLinear(8000)) {}

void AnimLighthouse::Update(LedStrip& strip, Frame frame)
{
	float duration = mapfr(frame.knobC, 4000, 32000);
	phase.invDuration = 1.f / duration;
	LighthouseBase::Update(strip, frame);
}

AnimLighthousePulse::AnimLighthousePulse(bool dir)
	: LighthouseBase(dir, .1325, PhasePulse(.064, .424)) {}
