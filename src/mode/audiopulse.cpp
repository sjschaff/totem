#include "mode.h"
#include "anim/anims.h"

// Main TODO:
// - mirror versions of all spinning type animations
// - auto anim switching + auto color switching
// 		- buttons to cycle
// - animation transitions
//		- random duration around ~15-30 secs?
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
// - tune color pulse


// rnd face pulse (color+compl on inner outer rings)
// TODO Shimmer type effect -> random leds on/off
// Colr offset takes phase and color and offsets frColr by phase
// Beat offset? takes frame and color and offsets iColr by beatCounter (+1, or +0)?

// Global frPolar rainbow looks sick, would make good lighthouse type effect

// Some day:
//  -automatic brightness adjustment based on power consumption to prevent overdraw
//	-randomize what value anims use for color phase
//  -randomize phase inputs to colors (also anims?) (i.e rainbow pulse vs rainbow linear)
//	-different anims for different audio freqs
//	-animation have prefered color schemes/types
//  -bias anims/colors to be used more often
//  -fancier combinitorial phase/param/anim system
//  -make flickering part of color for better combinitorial?
//  -try using beats to widen lighthouse instead of pulse it

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

class AnimNoop : public Anim
{
private:
	const bool fClear;

public:
	AnimNoop(bool fClear) : fClear(fClear) {}

	void Update(LedStrip& strip, Frame frame)
	{
		if (fClear)
		{
			ForEachLed(iLed)
				strip.SetColor(iLed, Colr::Black);
		}
	}
};

void AudioPulse::Update(Frame frame)
{
	//pulse->phase.phase.sfOffBeat = mapfr(frame.knobB, 0, .4); // .1
	//pulse->phase.phase.sfBeat = mapfr(frame.knobC, 0, 2); // .8
	if (!fLockAnim)
		msAnimCur += frame.ms;

	if (frame.audio.isBeginBeat && msAnimCur >= msAnimTarget)
	{
		NextRandom();
		StartRandomTimer();
	}

	LightshowMode::Update(frame);
}

void AudioPulse::StartRandomTimer()
{
	msAnimCur = 0;
	msAnimTarget = random(10000, 35000);

	*Log::log << "new anim: ["
		<< iAnim << ", " << iAnimTip << ", " << iColr << "] - "
		<< msAnimTarget << "\n";
}

void AudioPulse::OnPressB()
{
	fLockAnim = !fLockAnim;
	if (!fLockAnim)
		StartRandomTimer();
}

void AudioPulse::OnPressC() {
	NextRandom();
	StartRandomTimer();
}

void AudioPulse::OnPressD() { NextAnim(); }
void AudioPulse::OnPressE() { NextTip(); }
void AudioPulse::OnPressF() { NextColr(); }


#define AddColorPair(A, B) \
	colrs.push_back(new ColrPair(A, B)); \
	colrs.push_back(new ColrPair(B, A)); \
	colrs.push_back(new ColrGradPulse(ColrGrad(A, B)));

AudioPulse::AudioPulse(LedStrip& strip)
	: LightshowMode(strip)
{
	// THESE ARE DOPE
	anims.push_back(new AnimSpin(false));
	anims.push_back(new AnimSpin(true));
	anims.push_back(new AnimLighthousePulse(true));
	anims.push_back(new AnimLighthousePulse(false));

	anims.push_back(new AnimFlash());
	anims.push_back(new AnimRings(1000));
	anims.push_back(new AnimRings(1000));
	anims.push_back(new AnimVertPulse());
	anims.push_back(new AnimVertPulse());

	tipAnims.push_back(new AnimNoop(false));
	tipAnims.push_back(new TipPulse());
	tipAnims.push_back(new TipPulse());
	tipAnims.push_back(new TipFlash(FlashMode::Toggle));
	tipAnims.push_back(new TipFlash(FlashMode::Toggle));
	tipAnims.push_back(new TipFlash(FlashMode::Fade));
	tipAnims.push_back(new TipFlash(FlashMode::Fade));
	tipAnims.push_back(new TipFlash(FlashMode::Flicker));
	tipAnims.push_back(new TipFlash(FlashMode::Flicker));
	tipAnims.push_back(new TipAnimLoop(StepMode::Loop, false));
	tipAnims.push_back(new TipAnimLoop(StepMode::Loop, true));
	tipAnims.push_back(new TipAnimLoop(StepMode::Step, false));
	tipAnims.push_back(new TipAnimLoop(StepMode::Step, true));
	tipAnims.push_back(new TipAnimLoop(StepMode::FadeStep, false));
	tipAnims.push_back(new TipAnimLoop(StepMode::FadeStep, true));
	tipAnims.push_back(new TipAnimLoop(StepMode::FlickerStep, false));
	tipAnims.push_back(new TipAnimLoop(StepMode::FlickerStep, true));


	colrs.push_back(new ColrRainbow());
	colrs.push_back(new ColrRainbowPulseDual(PhasePulse(0, .1)));
	colrs.push_back(new ColrRainbowPulse(PhasePulse(0, .184)));

	AddColorPair(Colr::Blue, Colr::Red);
	AddColorPair(Colr::Blue, Colr::Green);
	AddColorPair(Colr(0, 0, .85), Colr::Hue(1.f/12.f));

	const uint cColr = 6;
	for (uint i = 0; i < cColr; ++i)
	{
		float hue = i / (float)cColr;
		colrs.push_back(new ColrPair(hue));

		if (i < cColr / 2)
			colrs.push_back(new ColrGradPulse(ColrGrad(hue)));
	}

	NextRandom();
	StartRandomTimer();
}
