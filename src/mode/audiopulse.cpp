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

AnimLighthousePulse* pulse;
void AudioPulse::Update(Frame frame)
{
	//pulse->phase.sfOffBeat = mapfr(frame.knobB, 0, .4); // .136, .064
	//pulse->phase.sfBeat = mapfr(frame.knobC, .1, .7); // .5
	LightshowMode::Update(frame);
}

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









// TODO this could prob go slower
	colrs.push_back(new ColrRainbowPulseDual(PhasePulse(.1, .8)));

	colrs.push_back(new ColrPair(Colr::Blue, Colr::Red));
	colrs.push_back(new ColrPair(Colr::Purple, Colr::Green));
	colrs.push_back(new ColrRainbow());
	colrs.push_back(new ColrRainbowPulse(PhasePulse(.1, .8)));

	// TODO this could prob go quicker
	colrs.push_back(new ColrGradPulse(
		PhasePulse(.1, .8),
		ColrGrad(Colr::Blue, Colr::Red)));


	colrs.push_back(new ColrPair(Colr::Blue, Colr::Green));
	// Blue orange
	// 1/12 hue is nice orange

}
