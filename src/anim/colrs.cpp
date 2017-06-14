#include "colrs.h"

Colr ColrStatic::GetColr(int iColr, float frColr, float frColrAnim)
{
	return GetColr(iColr, frColr);
}
// 1 - (a - 1)
// 1 - a + 1
// 2 - a

void ColrStatic::Update(Frame frame) {}

ColrSolid::ColrSolid(Colr colr) : colr(colr) {}

Colr ColrSolid::GetColr(int iColr, float frColr)
{
	return colr;
}

ColrPair::ColrPair(float hue)
	: colrA(Colr::Hue(hue)), colrB(Colr::Hue(hue+.5)) {}

ColrPair::ColrPair(Colr colrA, Colr colrB)
	: colrA(colrA), colrB(colrB) {}

Colr ColrPair::GetColr(int iColr, float frColr)
{
	return (iColr % 2) == 0 ? colrA : colrB;
}

ColrGrad::ColrGrad(float hue)
	: ColrPair(hue) {}

ColrGrad::ColrGrad(Colr colrA, Colr colrB)
	: ColrPair(colrA, colrB) {}

Colr ColrGrad::GetColr(int iColr, float frColr)
{
	return Colr::Lerp(colrA, colrB, frColr);
}

Colr ColrRainbow::GetColr(int iColr, float frColr)
{
	return Colr::Hue(frColr);
}

ColrRainbowPulse::ColrRainbowPulse(PhasePulse phase)
	: ColrPhase<PhasePulse, ColrRainbow>(phase, ColrRainbow()) {}


ColrGradPulse::ColrGradPulse(PhasePulse phase, ColrGrad grad)
	: ColrPhase<PhaseBounce<PhasePulse>, ColrGrad>(PhaseBounce<PhasePulse>(phase), grad) {}