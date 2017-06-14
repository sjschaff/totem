#include "anims.h"

AnimRings::AnimRings(float duration)
	: PhaseAnim(PhaseLinear(duration)) {}

void AnimRings::Display(LedStrip& strip, Frame frame, float phase)
{
	if (frame.audio.isBeginBeat)
		iPhase = (iPhase + 1) % 6;

	ForEachLed(iLed)
	{
		Led led = strip.leds[iLed];
		Colr colr = Colr::Black;

		if (led.face.iRing == (iPhase % 3))
			colr = frame.colr->GetColr(iPhase % 2, phase, 0) * frame.audio.energy;

		if (led.face.iRing == 2)
			colr *= .8;

		strip.SetColor(iLed, colr);
	}
}

TipAnimLoop::TipAnimLoop(StepMode stepMode, float duration)
	: PhaseAnim(PhaseLinear(duration)), stepMode(stepMode) {}

void TipAnimLoop::Display(LedStrip& strip, Frame frame, float phase)
{
	if (frame.audio.isBeginBeat)
		iPhase = (iPhase + 1) % 5;

	// Spinner
	uint iFace = iPhase;
	float fadePre = 0;
	float fadePost;

	switch (stepMode)
	{
		case StepMode::Loop: {
			iFace = (uint)(phase * 4.999) % 5;
			float frIFace = frac(phase*4.999);
			float fade = smoothstep(frIFace); // move out of loop
			fadePre = 1 - fade;
			fadePost = fade;

			break;
		}

		case StepMode::Step: {
			fadePost = 1;
			break;
		}

		case StepMode::FadeStep: {
			fadePost = frame.audio.beatFalloff;
			break;
		}

		case StepMode::FlickerStep: {
			fadePost = frame.audio.beatFalloff;
			fadePre = 1 - fadePost;
			break;
		}
	}

	uint iFaceOut = iFace;
	uint iFaceIn = (iFace + 1) % 5;

	ForEachLed(iLed)
	{
		Led led = strip.leds[iLed];
		if (led.face.iRing > 3)
		{
			uint iColr = led.face.iRing == 6 ? 1 : 0;
			Colr colr = frame.colr->GetColr(iColr, phase, 0);

			if (led.iFace == iFaceOut)
				colr *= fadePre;
			else if (led.iFace == iFaceIn)
				colr *= fadePost;
			else
				continue;

			strip.SetColor(iLed, colr);
		}
	}
}

TipPulse::TipPulse()//PhasePulse phase)
	: PhaseAnim(PhasePulse(.1, 6)) {}

void TipPulse::Display(LedStrip& strip, Frame frame, float phase)
{
	ForEachLed(iLed)
	{
		Led led = strip.leds[iLed];
		if (led.face.iRing > 3)
		{
			uint iColr = led.face.iRing == 6 ? 1 : 0;
			Colr colr = frame.colr->GetColr(iColr, phase, 0);

			// TODO: share math from vert pulse
			float width = .54; // .44 is a good thinner one
			float min = 1;
			float max = 3;
			float height = dmap(
				//sin(mapfr(phase, -PI/2, PI/2)), -1, 1,
				phase, 0, 1,
				min - width, max + width);

			float intens = smoothstepDual((led.face.frRad - height)/width);
			colr *= intens;

			strip.SetColor(iLed, colr);
		}
	}
}

TipFlash::TipFlash(FlashMode flashMode) : flashMode(flashMode) {}

void TipFlash::Update(LedStrip& strip, Frame frame)
{
	float intens = 0;
	switch (flashMode)
	{
		case Toggle:
			if (frame.audio.isBeginBeat)
				toggle = !toggle;
			intens = toggle ? 1 : 0;
			break;

		case Fade:
			intens = frame.audio.beatFalloff;
			break;

		case Flicker:
			intens = frame.audio.energy;
			break;
	}

	ForEachLed(iLed)
	{
		Led led = strip.leds[iLed];
		if (led.face.iRing > 3)
		{
			uint iColr = led.face.iRing == 6 ? 1 : 0;
			Colr colr = frame.colr->GetColr(iColr, led.frPolar, 0);
			strip.SetColor(iLed, colr*intens);
		}
	}
}
