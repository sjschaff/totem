#include "common.h"
#include "phase.h"

Phase::Phase(float dur) : phase(0), invDuration(1.f/dur) {}
Phase::Phase(Phase* phaseOther) : phase(phaseOther->phase), invDuration(phaseOther->invDuration) {}

void Phase::OnWrap() {}

void Phase::Accumulate(Frame frame)
{
	float delta = Delta(frame) * invDuration;
	phase += delta;
	if (phase > 1)
	{
		OnWrap();
		phase = frac(phase);
	}
}

float Phase::Value() { return phase; }

PhasePulse::PhasePulse(float sfOffBeat, float sfBeat)
	: Phase(1000), sfOffBeat(sfOffBeat), sfBeat(sfBeat) {}

float PhasePulse::Delta(Frame frame) {
	if (frame.audio.isBeat)
		return frame.ms * sfBeat;
	else
		return frame.ms * sfOffBeat;
}

PhaseLinear::PhaseLinear(float dur) : Phase(dur) {}

float PhaseLinear::Delta(Frame frame) { return frame.ms; }
