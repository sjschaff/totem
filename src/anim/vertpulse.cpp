#include "anims.h"

template<class TPhase>
void AnimVertPulseBase<TPhase>::Display(LedStrip& strip, Frame frame, float phase)
{
	const float frGap = 0;//.2; // TODO: param
	const float frPulse = 1 - frGap;

	uint iPhase = phase > .5 ? 1 : 0;
	phase = frac(phase * 2);
	//float maxY = 233; => 1

	// .116 seems to be good min for smoothness
	//float width = mapfr(frame.knobC, .1, 2);//200;

	if (phase <= frPulse)
	{
		phase = phase / frPulse;

		// TODO: make sin phase (how to combine dif phases with multi phase?)
		float theta = mapfr(phase, -PI/2, PI/2);

		// TODO make smoothstep dual
		float height = dmap(sin(theta), -1, 1, -width, 1+width);

		ForEachLed(iLed) {
			Led led = strip.leds[iLed];
			Colr colr = frame.colr->GetColr(iPhase, phase, led.frPolar);

			// TODO:make helper function for this very common case
			float intens = smoothstepDual((led.frY - height)/width);
			strip.SetColor(iLed, colr*intens);
		}
	}
	else
	{
		//phase = (phase - frPulse) / frGap;
		ForEachLed(iLed)
			strip.SetColor(iLed, Colr::Black);
	}
}

AnimBreathe::AnimBreathe(float width, float duration)
	: AnimVertPulseBase<PhaseLinear>(width, PhaseLinear(duration)) {}

AnimVertPulse::AnimVertPulse()
	: AnimVertPulseBase<PhasePulse>(.126, PhasePulse(.1, 2.7)) {}
