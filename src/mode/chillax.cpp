#include "mode.h"
#include "anim/anims.h"


struct PhaseKnob : public Phase
{
public:
	PhaseKnob() : Phase(1000) {}

	void Accumulate(Frame frame)
	{
		phase = frame.knobB;
	}

	float Delta(Frame frame) { return 0; }
};

class ColrRainbowAlways : public ColrAnim
{
public:
	Colr GetColr(int iColr, float frColr, float frColrAnim)
	{
		return ColrRainbow().GetColr(iColr, frColrAnim);
	}

	void Update(Frame frame) {}
};

template<class TPhase, class TColr>
class ColrChill : public ColrPhase<TPhase, TColr>
{
protected:
	ColrChill(TPhase phase)
		: ColrPhase<TPhase, TColr>(phase, TColr()) {}

	Colr GetColr(int iColr, float frColr, float frColrAnim) {
		return ColrPhase<TPhase, TColr>::colr.GetColr(
			iColr,
			ColrPhase<TPhase, TColr>::phase.Value());
	}
};

template<class TColr>
class ColrRainbowChill : public ColrChill<PhaseLinear, TColr>
{
public:
	ColrRainbowChill()
		: ColrChill<PhaseLinear, TColr>(PhaseLinear(16000)) {}

	void Update(Frame frame)
	{
		float duration = mapfr(frame.knobB, 4000, 32000);
		ColrPhase<PhaseLinear, TColr>::phase.invDuration = 1.f / duration;
		ColrPhase<PhaseLinear, TColr>::Update(frame);
	}
};

template<class TColr>
class ColrRainbowKnob : public ColrChill<PhaseKnob, TColr>
{
public:
	ColrRainbowKnob()
		: ColrChill<PhaseKnob, TColr>(PhaseKnob()) {}
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
	colrs.push_back(new ColrRainbowKnob<ColrRainbow>());
	colrs.push_back(new ColrRainbowKnob<ColrRainbowDual>());
}

void Chillax::OnPressC()
{
	NextAnim();
}

void Chillax::OnPressD()
{
	NextColr();
}
