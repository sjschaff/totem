#ifndef anims_h
#define anims_h

#include "anim.h"

// Prefers dual color
class AnimSpin : public PhaseAnim<PhasePulse>
{
public:
	AnimSpin();
	void Display(LedStrip& strip, Frame frame, float phase);
};

class AnimFlash : public Anim
{
private:
	bool wasLoud;
	uint iColr;

public:
	AnimFlash();
	void Update(LedStrip& strip, Frame frame);
};

template<class TPhase>
class LighthouseBase : public PhaseAnim<TPhase>
{
private:
	public:
	float width;

protected:
	LighthouseBase(float width, TPhase phase)
		: PhaseAnim<TPhase>(phase), width(width) {}

public:
	void Display(LedStrip& strip, Frame frame, float phase);
};

class AnimLighthouse : public LighthouseBase<PhaseLinear>
{
public:
	AnimLighthouse(float width, float duration);
};

class AnimLighthousePulse : public LighthouseBase<PhasePulse>
{
public:
	AnimLighthousePulse(float width, PhasePulse phase);
};

template<class TPhase>
class AnimVertPulseBase : public PhaseAnim<TPhase>
{
protected:
	float width;
	AnimVertPulseBase(float width, TPhase phase)
		: PhaseAnim<TPhase>(phase), width(width) {}

public:
	void Display(LedStrip& strip, Frame frame, float phase);
};

class AnimBreathe : public AnimVertPulseBase<PhaseLinear>
{
public:
	AnimBreathe(float width, float duration);
};

class AnimVertPulse : public AnimVertPulseBase<PhasePulse>
{
public:
	AnimVertPulse(float width, PhasePulse phase);
};

class AnimRings : public PhaseAnim<PhaseLinear>
{
private:
	uint iPhase = 0;

public:
	AnimRings(float duration);

	void Display(LedStrip& strip, Frame frame, float phase);
};

enum StepMode
{
	Loop, Step, FadeStep, FlickerStep
};


// TODO tip anims should share colors
class TipAnimLoop : public PhaseAnim<PhaseLinear>
{
private:
	uint iPhase = 0;

	StepMode stepMode;

public:
	TipAnimLoop(StepMode stepMode, float duration);

	void Display(LedStrip& strip, Frame frame, float phase);
};

// TODO share with vert pulse (colrs work differently)
class TipPulse : public PhaseAnim<PhasePulse>
{
public:
	TipPulse();//PhasePulse phase);

	void Display(LedStrip& strip, Frame frame, float phase);
};

enum FlashMode
{
	Toggle,
	Fade,
	Flicker
};

class TipFlash : public Anim
{
private:
	const FlashMode flashMode;
	bool toggle = false;

public:
	TipFlash(FlashMode flashmode);
	void Update(LedStrip& strip, Frame frame);
};

#endif
