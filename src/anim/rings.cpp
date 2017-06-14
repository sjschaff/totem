#include "anims.h"

AnimRings::AnimRings(float duration)
	: PhaseAnim(PhaseLinear(duration)) {}

void AnimRings::Display(LedStrip& strip, Frame frame, float phase)
{
	//if (frame.audio.isBeginBeat)
		//iPhase = (iPhase + 1) % 4;

	bool isLoud = frame.audio.isBeat;
	if (isLoud)
	{
		//audio = 1;
		if (!wasLoud)
		{
			iPhase = (iPhase + 1) % 6;
			iPhaseTop = (iPhaseTop + 1) % 12;
			wasLoud = true;
		}
	}
	else
	{
		//audio = 0;
		wasLoud = false;
	}

	ForEachLed(iLed)
	{
		Led led = strip.leds[iLed];
		Colr colr = Colr::Black;

	/*	float width = mapfr(frame.knobC, .25, 2);
		float rad = mapfr(phase, -width, 3+width);
		float dist = abs(rad - led.face.frRad) / width;
		float intens = smoothstepDual(dist);
		intens = 1 - saturate(dist);
		strip.SetColor(iLed, Colr::Green * intens);*/
		//uint iPhase = phase * 3.999;
		uint iFaceTop = (uint)(phase * 4.999) % 5;
		float frIFaceTop = frac(phase*4.999);
		float hue = iPhaseTop / 6.f; // TODO add to colors
		if (led.face.iRing > 3)
		{
			uint iFaceOut = iFaceTop;
			uint iFaceIn = (iFaceTop + 1) % 5;

			uint iColr = 0;

			if (led.face.iRing == 6)
			{
				iColr = 1;
				hue += .5;
				*Log::log << "y max: " << led.zpt.y << "\n";
			}

			colr = //Colr::Hue(hue);
				frame.colr->GetColr(iColr, phase, 0);

			float smth = smoothstep(frIFaceTop); // move out of loop
			if (led.iFace == iFaceOut)
				colr *= 1 - smth;
			else if (led.iFace == iFaceIn)
				colr *= smth;
			else
				colr = Colr::Black;
		} else if (led.face.iRing == (iPhase % 3))
			colr =
				frame.colr->GetColr(iPhase % 2, phase, 0)
				//Colr::Hue(hue + (iPhase % 2)*.5)
				* frame.audio.energy;

		strip.SetColor(iLed, colr);
	}
}
