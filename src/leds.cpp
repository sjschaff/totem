#include "leds.h"

const ushort LedStrip::cLED = 195;
const ushort LedStrip::outer[] = { 0, 1, 2, 3, 4, 5, 6, 7, 15, 14 };
const ushort LedStrip::cOuter = 10;
const ushort LedStrip::inner[] = { 13, 11, 10, 9, 8 };
const ushort LedStrip::cInner = 5;
const ushort LedStrip::center = 12;
const ushort LedStrip::cFace = 10;
const ushort LedStrip::cTotal = 16;

void InitLed(Led& led, float frPolar, float rad)
{
	float polar = frPolar * 2 * PI + PI/2;
	led.iFace = 0;
	led.polarFace = polar;
	led.radFace = rad;

	led.xFace = cos(polar)*rad;
	led.yFace = sin(polar)*rad;
}

Led* LedStrip::GenerateLeds()
{
	Led ledsFace[cTotal];
	InitLed(ledsFace[center], 0, 0);

	for (uint i = 0; i < cOuter; ++i)
	{
		uint iLed = outer[i];
		uint iPolar = (i + cOuter - 4) % cOuter;
		float polar = iPolar / (float)cOuter;
		InitLed(ledsFace[iLed], polar, 53);
	}

	for (uint i = 0; i < cInner; ++i)
	{
		uint iLed = inner[i];
		uint iPolar = (i + cInner - 2) % cInner;
		float polar = iPolar / (float)cInner;
		InitLed(ledsFace[iLed], polar, 27);
	}

	// Copy into full buffer
	Led* leds = new Led[cLED];
	for (uint i = 0; i < cFace; ++i)
	{
		uint iFace = i * cTotal;
		for (uint iLed = 0; iLed < cTotal; ++iLed)
			leds[iFace + iLed] = ledsFace[iLed];
	}

	return leds;
}

LedStrip::LedStrip(int pin) :
	strip(cLED, pin, NEO_GRB + NEO_KHZ800),
	leds(GenerateLeds())
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
	for (ushort i = 0; i < cLED; ++i)
		setColorNoGamma(i, colr);
	strip.show();
}

void LedStrip::rainbowFace(uint face, float offs, float polarOffs)
{
	uint iFace = face * cTotal;
	for (uint i = 0; i < cTotal; ++i)
	{
		Led led = leds[iFace+i];
		float polar = led.polarFace + polarOffs*2*PI;
		float x = cos(polar) * led.radFace;

		float val = frac(dmap(x, -52, 52, 0, 1) + offs);
		setColor(iFace+i, Colr::Hue(val)*.5);
	}
}

void LedStrip::verticalLineFace(uint face, float x, float width)
{
	uint iFace = face * cTotal;
	for (uint i = 0; i < cTotal; ++i)
	{
		Led led = leds[iFace+i];
		float delta = abs(led.xFace - x);
		float val = 1 - saturate(delta/width);
		Colr colr = Colr(0, 0, 0x80*val);
		setColor(iFace + i, colr);
	}
}
