#include "mode.h"
#include "anim/anims.h"

// Main TODO:
// - add blue orange
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

template<class TAnim, class TTip>
class AnimWithTip : public Anim
{
private:
	TAnim animBody;
	TTip animTip;

public:
	AnimWithTip(TAnim animBody, TTip animTip) : animBody(animBody), animTip(animTip) {}

	void Update(LedStrip& strip, Frame frame)
	{
		animBody.Update(strip, frame);
		animTip.Update(strip, frame);
	}
};

class AnimWithTip2 : public Anim
{
private:
	Anim* animBody;
	Anim* animTip;

public:
	AnimWithTip2(Anim* animBody, Anim* animTip) : animBody(animBody), animTip(animTip) {}

	void Update(LedStrip& strip, Frame frame)
	{
		animBody->Update(strip, frame);
		animTip->Update(strip, frame);
	}
};

void AudioPulse::Update(Frame frame)
{
	//pulse->phase.phase.sfOffBeat = mapfr(frame.knobB, 0, .4); // .1
	//pulse->phase.phase.sfBeat = mapfr(frame.knobC, 0, 2); // .8
	LightshowMode::Update(frame);
}

#define AddColorPair(A, B) \
	colrs.push_back(new ColrPair(A, B)); \
	colrs.push_back(new ColrPair(B, A)); \
	colrs.push_back(new ColrGradPulse(ColrGrad(A, B)));

AudioPulse::AudioPulse(LedStrip& strip)
	: LightshowMode(strip)
{
	// THESE ARE DOPE
	anims.push_back(new AnimSpin());
	anims.push_back(new AnimFlash());
	anims.push_back(new AnimRings(1000));
	anims.push_back(new AnimVertPulse(.126, PhasePulse(.1, 2.7)));
	anims.push_back(new AnimLighthousePulse(.1325, PhasePulse(.064, .424)));

	tipAnims.push_back(new AnimNoop(false));
	tipAnims.push_back(new TipPulse());
	tipAnims.push_back(new TipFlash(FlashMode::Toggle));
	tipAnims.push_back(new TipFlash(FlashMode::Fade));
	tipAnims.push_back(new TipFlash(FlashMode::Flicker));
	tipAnims.push_back(new TipAnimLoop(StepMode::Loop, 2000));
	tipAnims.push_back(new TipAnimLoop(StepMode::Step, 2000));
	tipAnims.push_back(new TipAnimLoop(StepMode::FadeStep, 2000));
	tipAnims.push_back(new TipAnimLoop(StepMode::FlickerStep, 2000));


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
}
