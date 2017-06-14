#ifndef colrs_h
#define colrs_h

#include "util/colr.h"
#include "frame.h"
#include "phase.h"

class ColrAnim
{
public:
	virtual Colr GetColr(int iColr, float frColr, float frColrAnim) = 0;
	virtual void Update(Frame frame) = 0;
};

class ColrStatic : public ColrAnim
{
public:
	void Update(Frame frame);
	Colr GetColr(int iColr, float frColr, float frColrAnim);
	virtual Colr GetColr(int iColr, float frColr) = 0;
};

class ColrSolid : public ColrStatic
{
private:
	const Colr colr;

public:
	ColrSolid(Colr colr);

	Colr GetColr(int iColr, float frColr);
};

class ColrPair : public ColrStatic
{
protected:
	const Colr colrA;
	const Colr colrB;

public:
	ColrPair(float hue); // Complementary hue pair
	ColrPair(Colr colrA, Colr colrB);

	Colr GetColr(int iColr, float frColr);
};

class ColrGrad : public ColrPair
{
public:
	ColrGrad(float hue); // Complementary hue pair
	ColrGrad(Colr colrA, Colr colrB);

	Colr GetColr(int iColr, float frColr);
};

class ColrRainbow : public ColrStatic
{
public:
	Colr GetColr(int iColr, float frColr);
};

// TODO:Very similar to PhaseAnim
template<class TPhase, class TColr>
class ColrPhase : public ColrAnim
{
private:
public:
	TPhase phase;
	TColr colr;

public:
	ColrPhase(TPhase phase, TColr colr)
		: phase(phase), colr(colr) {}

	void Update(Frame frame) {
		phase.Accumulate(frame);
	}

	Colr GetColr(int iColr, float frColr, float frColrAnim) {
		return colr.GetColr(iColr, frac(frColrAnim + phase.Value()));
	}
};


class ColrRainbowPulse : public ColrPhase<PhasePulse, ColrRainbow>
{
public:
	ColrRainbowPulse(PhasePulse phase);
};

class ColrRainbowDual : public ColrStatic
{
public:
	Colr GetColr(int iColr, float frColr);
};

class ColrRainbowPulseDual : public ColrPhase<PhasePulse, ColrRainbowDual>
{
public:
	ColrRainbowPulseDual(PhasePulse phase);
};

class ColrGradPulse : public ColrPhase<PhaseBounce<PhasePulse>, ColrGrad>
{
public:
	ColrGradPulse(PhasePulse phase, ColrGrad grad);
};

#endif
