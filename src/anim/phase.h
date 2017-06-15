#ifndef phase_h
#define phase_h

#include "frame.h"

struct Phase
{
private:
public:
	float phase;
	float invDuration;

protected:
	Phase(Phase* phaseOther);

	virtual float Delta(Frame frame) = 0;
	virtual void OnWrap();

public:
	Phase(float dur);
	virtual void Accumulate(Frame frame);
	virtual float Value();
};

struct PhasePulse : public Phase
{
private:
public:
	 float sfOffBeat;
	 float sfBeat;

public:
	PhasePulse(float sfOffBeat, float sfBeat);
	float Delta(Frame frame);
};

struct PhaseLinear : public Phase
{
	PhaseLinear(float dur);
	float Delta(Frame frame);
};

template<class TPhase>
struct PhaseBounce : public Phase
{
private:
public:
	TPhase phase;
	bool flipped;

protected:
	float Delta(Frame frame) {
		return phase.Delta(frame);
	}

	void OnWrap() {
		flipped = !flipped;
	}

public:
	PhaseBounce(TPhase phase) : Phase(&phase), phase(phase), flipped(false) {}

	float Value() {
		float val = Phase::Value();
		return flipped ? 1 - val : val;
	}
};

#endif
