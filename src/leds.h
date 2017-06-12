#ifndef leds_h
#define leds_h

#include <Adafruit_NeoPixel.h>
#include "colr.h"

struct Led;

class LedStrip
{
private:
	static const ushort cLED;

	Adafruit_NeoPixel strip;
	const Led* const leds;
	static Led* GenerateLeds();
	void setColorNoGamma(ushort px, Colr colr);

public:
	LedStrip();

	void setColor(ushort px, Colr colr);
	void show();

	void spinStrip(Colr colr, Colr colrCenter, ulong time, float revsPerSec);

	// color space tests
	void setStripColor(Colr colr);
	void globalAxis(uint face, float x, float y, float z, float w, Colr colr);
	void rainbowFace(uint face);
	void rainbowFaceLinear(uint face, float offs, float polarOffs);
};

#endif
