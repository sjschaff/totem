#include "leds.h"

const ushort LedStrip::cLED = 195;

static const uint ledPin = 23;

LedStrip::LedStrip() :
	strip(cLED, ledPin, NEO_GRB + NEO_KHZ800),
	leds(GenerateLeds())
{
	pinMode(ledPin, OUTPUT);
	strip.begin();
	strip.show();
	frBrightness = .5;
}

void LedStrip::SetBrightness(float fr)
{
	frBrightness = fr;
}

void LedStrip::SetColorRaw(ushort px, Colr colr)
{
	strip.setPixelColor(px, (ubyte)(colr.r*255), (ubyte)(colr.g*255), (ubyte)(colr.b*255));
}

Colr LedStrip::ColrRawForColr(Colr colr)
{
	return (colr * frBrightness).gammaCorrect();
}

void LedStrip::SetColor(ushort px, Colr colr)
{
	SetColorRaw(px, ColrRawForColr(colr));
}

void LedStrip::Show() { strip.show(); }

void LedStrip::SetStripColor(Colr colr)
{
	colr = ColrRawForColr(colr);
	ForEachLed(iLed)
		SetColorRaw(iLed, colr);
	Show();
}

void LedStrip::RainbowFace(uint iFace)
{
	ForEachLed(iLed)
		SetColor(iLed, Colr::Hue(leds[iLed].face.frPolar));
	Show();
}

void LedStrip::RainbowFaceLinear(uint iFace, float offs, float polarOffs)
{
	ForEachLed(iLed)
	{
		Led led = leds[iLed];
		float polar = led.face.polar + polarOffs*2*PI;
		float x = cos(polar) * led.face.rad;

		float val = frac(dmap(x, -52, 52, 0, 1) + offs);
		SetColor(iLed, Colr::Hue(val)*.5);
	}
}

float FrForAxis(float val, float offs, float width)
{
	float delta = abs(val - offs);
	float fr = saturate(delta/width);
	fr = dmap(cos(mapfr(fr, 0, PI)), -1, 1, 0, 1);
	return fr;
}

void print(vec3 v)
{
	Log::log->print("<");
	Log::log->print(v.x);
	Log::log->print(", ");
	Log::log->print(v.y);
	Log::log->print(", ");
	Log::log->print(v.z);
	Log::log->print(">, ");
}

void LedStrip::GlobalAxis(uint iFace, float x, float y, float z, float width, Colr colr)
{
	ForEachLed(iLed) {
		Led led = leds[iLed];
		Colr colrr = colr *
			FrForAxis(led.zpt.y, y, width);

		SetColor(iLed, colrr);
	}

	Show();
}
