#include "mode.h"
#include "anim/anims.h"

Beat::Beat(ulong msStart) : msStart(msStart) {}

// Random test stuff
class AnimRings : public PhaseAnim<PhaseLinear>
{
private:
	uint iPhase = 0;
	uint iPhaseTop = 5;
	bool wasLoud = false;

public:
	AnimRings(float duration) : PhaseAnim(PhaseLinear(duration)) {}

	void Display(LedStrip& strip, Frame frame, float phase)
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
};

// Main TODO:
// - auto anim switching + auto color switching
// 	- buttons to cycle
// - Separate Tip Anims
// 		- needs anim overlay
// - animation tip transitions
// 		- needs anim overlay
// - color modes:
//		- auto
//		- fixed (rainbow knob)
//		- fixed (hue pair knob)
//		- rainbow
// - chill mode:
//		- knobs for speed
// - disable knobs?
// - Manual anim+color selection mode for audiopulse?
// - fix battery pack
// - tune vert pulse, tune lighthouse (try using beats to widen lighthouse instead of pulse it)


// rnd face pulse (color+compl on inner outer rings)
// TODO Shimmer type effect -> random leds on/off
// Colr offset takes phase and color and offsets frColr by phase
// Beat offset? takes frame and color and offsets iColr by beatCounter (+1, or +0)?
// Tip Anims:
	// Tip loop (loop wit beat?)
	// Tip Flicker
	// Tip Flash wit beat

// Global frPolar rainbow looks sick, would make good lighthouse type effect

// Some day: randomize what value anims use for color phase

// buttons:
// next mode
// rage mode?
// disable knobs
// per mode:
// 	next color
// 	next anim
// 	color mode -> random, fixed dual, fixed single, rainbow?

// knobs
// brightness
// color

AudioPulse::AudioPulse(LedStrip& strip)
	: LightshowMode(strip)
{
	//anims.push_back(new AnimSpin());
	anims.push_back(new AnimFlash());

	// (pre phasex2) was .1, 4.5.  good but flickery
	anims.push_back(new AnimVertPulse(.126, PhasePulse(.3, 7)));

	anims.push_back(new AnimLighthousePulse(PhasePulse(.125, .5)));
	anims.push_back(new AnimLighthousePulse(PhasePulse(.25, 2))); // TODO Slow speed for this is perfect for chill mode (account for 4x tho)
	anims.push_back(new AnimRings(1000));

	colrs.push_back(new ColrPair(Colr::Blue, Colr::Red));
	colrs.push_back(new ColrPair(Colr::Purple, Colr::Green));
	colrs.push_back(new ColrRainbow());
	colrs.push_back(new ColrRainbowPulse(PhasePulse(.1, .8)));
	colrs.push_back(new ColrGradPulse(
		PhasePulse(.1, .8),
		ColrGrad(Colr::Blue, Colr::Red)));


	colrs.push_back(new ColrPair(Colr::Blue, Colr::Green));
	// Blue orange
	// 1/12 hue is nice orange

}
