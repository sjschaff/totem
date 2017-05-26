#include "colr.h"

const Colr Colr::Red = Colr(0xFF, 0, 0);
const Colr Colr::Green = Colr(0, 0xFF, 0);
const Colr Colr::Blue = Colr(0, 0, 0xFF);
const Colr Colr::Black = Colr(0);
const Colr Colr::White = Colr(0xFF);

Colr::Colr() : r(0), g(0), b(0) {}
Colr::Colr(ubyte grey) : r(grey), g(grey), b(grey) {}
Colr::Colr(ubyte r, ubyte g, ubyte b) : r(r), g(g), b(b) {}

Colr Colr::operator*(double v) const
{
	return Colr(r*v, g*v, b*v);
}

Colr Colr::operator*(byte v) const
{
	return (*this)*(v/255.0);
}

void Colr::operator*=(double v)
{
	*this = *this * v;
}

Colr Colr::gammaCorrect()
{
	return Colr( gamma(r), gamma(g), gamma(b));
}
