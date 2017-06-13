#ifndef anims_h
#define anims_h

#include "anim.h"

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
protected:
	LighthouseBase(TPhase phase) : PhaseAnim<TPhase>(phase) {}

public:
	void Display(LedStrip& strip, Frame frame, float phase);
};

class AnimLighthouse : public LighthouseBase<PhaseLinear>
{
public:
	AnimLighthouse(float duration);
};

class AnimLighthousePulse : public LighthouseBase<PhasePulse>
{
public:
	AnimLighthousePulse(PhasePulse phase);
};

template<class TPhase>
class AnimVertPulseBase : public PhaseAnim<TPhase>
{
protected:
	const float width;
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

#endif
