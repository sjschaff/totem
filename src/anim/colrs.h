#ifndef colrs_h
#define colrs_h

#include "util/colr.h"

class ColrAnim
{
public:
	virtual Colr GetColr(int iColr, float frColr) = 0;
};

class ColrSolid : public ColrAnim
{
private:
	const Colr colr;

public:
	ColrSolid(Colr colr);

	Colr GetColr(int iColr, float frColr);
};

class ColrPair : public ColrAnim
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

class ColrRainbow : public ColrAnim
{
public:
	Colr GetColr(int iColr, float frColr);
};

#endif
