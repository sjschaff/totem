#include "leds.h"

const ushort LedStrip::cLED = 195;
const ushort LedStrip::outer[] = { 0, 1, 2, 3, 4, 5, 6, 7, 15, 14 };
const ushort LedStrip::cOuter = 10;
const ushort LedStrip::inner[] = { 13, 11, 10, 9, 8 };
const ushort LedStrip::cInner = 5;
const ushort LedStrip::center = 12;
const ushort LedStrip::cFace = 11;
const ushort LedStrip::cTotal = 16;

LedStrip::LedStrip(int pin)
	: strip(cLED, pin, NEO_GRB + NEO_KHZ800)
{
	pinMode(pin, OUTPUT);
	strip.begin();
	strip.show();
}

void LedStrip::setColorNoGamma(ushort px, Colr colr)
{
	strip.setPixelColor(px, colr.r, colr.g, colr.b);
}

void LedStrip::setColor(ushort px, Colr colr)
{
	setColorNoGamma(px, colr.gammaCorrect());
}

void LedStrip::show() { strip.show(); }

void LedStrip::setStripColor(Colr colr)
{
	colr = colr.gammaCorrect();
	for (ushort i = 0; i < 2;++i)//cLED; ++i)
		setColorNoGamma(i, colr);
	strip.show();
}
