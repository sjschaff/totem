#include "colr.h"

// nice orange Colr::Hue(1.f/12.f)
const Colr Colr::Red = Colr(1, 0, 0);
const Colr Colr::Yellow = Colr(1, 1, 0);
const Colr Colr::Green = Colr(0, 1, 0);
const Colr Colr::Cyan = Colr(0, 1, 1);
const Colr Colr::Blue = Colr(0, 0, 1);
const Colr Colr::Purple = Colr(1, 0, 1);
const Colr Colr::Black = Colr(0);
const Colr Colr::White = Colr(1);

Colr::Colr() : r(0), g(0), b(0) {}
Colr::Colr(float grey) : r(grey), g(grey), b(grey) {}
Colr::Colr(float r, float g, float b) : r(r), g(g), b(b) {}

Colr Colr::operator*(float v) const
{
	return Colr(r*v, g*v, b*v);
}

void Colr::operator*=(float v)
{
	*this = *this * v;
}

Colr Colr::gammaCorrect()
{
	return Colr( gamma(r), gamma(g)*.9, gamma(b)*.8);
}

Colr Colr::Lerp(Colr a, Colr b, float fr)
{
	return Colr(
		lerp(a.r, b.r, fr),
		lerp(a.g, b.g, fr),
		lerp(a.b, b.b, fr));
}

Colr Colr::Hue(float fr)
{
	fr = frac(fr);
	int step = fr * 6;
	fr = frac(fr*6);
	switch (step)
	{
		case 0: return Lerp(Red, Yellow, fr);
		case 1: return Lerp(Yellow, Green, fr);
		case 2: return Lerp(Green, Cyan, fr);
		case 3: return Lerp(Cyan, Blue, fr);
		case 4: return Lerp(Blue, Purple, fr);
		default: return Lerp(Purple, Red, fr);
	}
}
