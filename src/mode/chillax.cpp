#include "mode.h"
#include "anim/anims.h"


class ColrRainbowAlways : public ColrAnim
{
public:
	Colr GetColr(int iColr, float frColr, float frColrAnim)
	{
		return ColrRainbow().GetColr(iColr, frColrAnim);
	}

	void Update(Frame frame) {}
};

template<class TColr>
class ColrRainbowChill : public ColrPhase<PhaseLinear, TColr>
{
public:
	ColrRainbowChill()
		: ColrPhase<PhaseLinear, TColr>(PhaseLinear(1000), TColr()) {}

	void Update(Frame frame)
	{
		float duration = mapfr(frame.knobB, 4000, 32000);
		ColrPhase<PhaseLinear, TColr>::phase.invDuration = 1.f / duration;
		ColrPhase<PhaseLinear, TColr>::Update(frame);
	}

	Colr GetColr(int iColr, float frColr, float frColrAnim) {
		return ColrPhase<PhaseLinear, TColr>::colr.GetColr(
			iColr,
			ColrPhase<PhaseLinear, TColr>::phase.Value());
	}
};

Chillax::Chillax(LedStrip& strip)
	: LightshowMode(strip)
{
	tipAnims.push_back(new AnimNoop(false));

	anims.push_back(new AnimBreathe(true));
	anims.push_back(new AnimBreathe(false));
	anims.push_back(new AnimLighthouse(false));
	anims.push_back(new AnimLighthouse(true));

	colrs.push_back(new ColrRainbowAlways());
	colrs.push_back(new ColrRainbowChill<ColrRainbow>());
	colrs.push_back(new ColrRainbowChill<ColrRainbowDual>());

	// - color modes chill:
	//		- fixed (rainbow knob)
	//		- fixed (rainbow dual knob)
}

void Chillax::OnPressC()
{
	NextAnim();
}

void Chillax::OnPressD()
{
	NextColr();
}
