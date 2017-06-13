#include "audiopulse.h"
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
			float hue = iPhaseTop / 6.f;
			if (led.face.iRing > 3)
			{
				uint iFaceOut = iFaceTop;
				uint iFaceIn = (iFaceTop + 1) % 5;

				if (led.face.iRing == 6)
				{
					hue += .5;
					*Log::log << "y max: " << led.zpt.y << "\n";
				}

				colr = Colr::Hue(hue);
				float smth = smoothstep(frIFaceTop); // move out of loop
				if (led.iFace == iFaceOut)
					colr *= 1 - smth;
				else if (led.iFace == iFaceIn)
					colr *= smth;
				else
					colr = Colr::Black;
			} else if (led.face.iRing == (iPhase % 3))
				colr = Colr::Hue(hue + (iPhase % 2)*.5) * frame.audio.energy;

			strip.SetColor(iLed, colr);
		}
	}
};


// vertical pulse (rings moving up)
// rnd face pulse (color+compl on inner outer rings)
// TODO Shimmer type effect -> random leds on/off
// TODO revive breathe, add to Chill Mode

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

AudioPulse::AudioPulse(LedStrip& strip, Input& input)
	: Mode(strip, input)
{
	anims.push_back(new AnimBreathe(.2, 2000));

	// was .1, 4.5.  good but flickery
	anims.push_back(new AnimVertPulse(.126, PhasePulse(.15, 3.5)));

	anims.push_back(new AnimLighthouse(4000*4));
	anims.push_back(new AnimLighthousePulse(PhasePulse(.125, .5)));
	anims.push_back(new AnimLighthousePulse(PhasePulse(.25, 2))); // TODO Slow speed for this is perfect for chill mode (account for 4x tho)
	anims.push_back(new AnimRings(1000));
	anims.push_back(new AnimSpin());
	anims.push_back(new AnimFlash());

	iAnim = 0;
}

void AudioPulse::Update(Frame frame)
{
	if (frame.audio.isBeginBeat)
		beats.push_back(Beat(frame.ms));

	Anim* anim = anims[iAnim];
	anim->Update(strip, frame);
	strip.Show();
}

void AudioPulse::OnPressE()
{
	iAnim = (iAnim + 1) % anims.size();
}
