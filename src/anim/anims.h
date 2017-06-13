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

#endif
