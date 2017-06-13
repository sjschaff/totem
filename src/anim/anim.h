#ifndef anim_h
#define anim_h

#include "util.h"
#include "input.h"
#include "leds.h"
/*
template<class Src, class Op>
class Aval
{
private:
	Src src;
	Op op;

public:
	Aval(Src src, Op op) : src(src), op(op) {}

	template<class OpNext>
	Aval<Aval<Src, Op>, OpNext> to(OpNext opnext)
	{
		return Aval(*this, opnext);
	}

	float Value() {
		return op.Value(src.Value());
	}
};

struct NoOp
{
	inline float Value(float v) { return v; }
};

struct PhaseAval
{

};

class Smoothstep
{

};


class Phase;

template<class TPhase>
class Aval<PhaseAval, TPhase>
{
private:
	TPhase* phase;

public:
	Aval(TPhase* phase) : phase(phase) {}

	float Value();// { return phase->Value(); }

	template<class OpNext>
	Aval<Aval<TPhase>, OpNext> to(OpNext opnext)
	{
		return Aval(*this, opnext);
	}
};
*/
#include "audio.h"

struct Frame
{
	ulong ms;
	float knobB;
	float knobC;
	AudioData audio;
};

struct Phase
{
private:
	float phase;
	float invDuration;

protected:
	virtual float Delta(Frame frame) = 0;

public:
	Phase(float dur = 1000) : phase(0), invDuration(1.f/dur) { }

	void Accumulate(Frame frame)
	{
		float delta = Delta(frame) * invDuration;
		phase = frac(phase + delta);
	}

	float Value() { return phase; }
};

struct PhasePulse : public Phase
{
private:
	const float sfOffBeat;
	const float sfBeat;

public:
	PhasePulse(float sfOffBeat, float sfBeat) : sfOffBeat(sfOffBeat), sfBeat(sfBeat) {}

	float Delta(Frame frame) {
		if (frame.audio.isBeat)
			return frame.ms * sfBeat;
		else
			return frame.ms * sfOffBeat;
	}
};

struct PhaseLinear : public Phase
{
	PhaseLinear(float dur) : Phase(dur) {}
	float Delta(Frame frame) { return frame.ms; }
};

//float Aval<TPhase>::Value() { return phase->Value(); }

class Anim
{
public:
	virtual void Update(LedStrip& strip, Frame frame) = 0;
};

template<class TPhase, class TAnim>
class AnimSmp : public Anim
{
private:
	TPhase phase;
	TAnim anim;

public:
	AnimSmp(TPhase phase, TAnim anim) : phase(phase), anim(anim) {}

	void Update(LedStrip& strip, Frame frame) {
		phase.Accumulate(frame);
		anim.Display(strip, phase.Value());
	}
};

class Mode : BtnHandler
{
protected:
	LedStrip& strip;
	Input& input;

public:
	Mode(LedStrip& strip, Input& input);
	virtual void Update(Frame frame) = 0;

	virtual void OnPressA();
	virtual void OnPressB();
	virtual void OnPressC();
	virtual void OnPressD();
	virtual void OnPressE();
	virtual void OnPressF();
};

#endif