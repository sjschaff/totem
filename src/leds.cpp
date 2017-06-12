#include "leds.h"
#include "log.h"
#include "util/stripbuilder.h"

#define ForEachLed(var) for (uint var = 0; var < cLED; ++var)

const ushort LedStrip::cLED = 195;


static const uint ledPin = 23;

LedStrip::LedStrip() :
	strip(cLED, ledPin, NEO_GRB + NEO_KHZ800),
	leds(GenerateLeds())
{
	pinMode(ledPin, OUTPUT);
	strip.begin();
	strip.show();
}

void LedStrip::setColorNoGamma(ushort px, Colr colr)
{
	strip.setPixelColor(px, (ubyte)(colr.r*255), (ubyte)(colr.g*255), (ubyte)(colr.b*255));
}

void LedStrip::setColor(ushort px, Colr colr)
{
	setColorNoGamma(px, colr.gammaCorrect());
}

void LedStrip::show() { strip.show(); }

void LedStrip::setStripColor(Colr colr)
{
	colr = colr.gammaCorrect();
	ForEachLed(iLed)
		setColorNoGamma(iLed, colr);
	strip.show();
}

void LedStrip::rainbowFace(uint iFace)
{
	ForEachLed(iLed)
		setColor(iLed, Colr::Hue(leds[iLed].polarFace/(2*PI))*.4);
	strip.show();
}

void LedStrip::rainbowFaceLinear(uint iFace, float offs, float polarOffs)
{
	ForEachLed(iLed)
	{
		Led led = leds[iLed];
		float polar = led.polarFace + polarOffs*2*PI;
		float x = cos(polar) * led.radFace;

		float val = frac(dmap(x, -52, 52, 0, 1) + offs);
		setColor(iLed, Colr::Hue(val)*.5);
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

void LedStrip::globalAxis(uint iFace, float x, float y, float z, float width, Colr colr)
{
	ForEachLed(iLed) {
		Led led = leds[iLed];
		Colr colrr = colr *
			FrForAxis(led.zpt.y, y, width);

		setColor(iLed, colrr);
	}

	show();
}
