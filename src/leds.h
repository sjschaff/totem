#ifndef leds_h
#define leds_h

#include <Adafruit_NeoPixel.h>
#include "colr.h"

struct Led;

class LedStrip
{
private:
	static const ushort cLED;
	static Led* GenerateLeds();

	Adafruit_NeoPixel strip;
	const Led* const leds;
	float frBrightness;

	void SetColorRaw(ushort px, Colr colr);
	Colr ColrRawForColr(Colr colr);

public:
	LedStrip();

	void SetBrightness(float fr);

	void SetColor(ushort px, Colr colr);
	void Show();

	void SpinStrip(Colr colr, Colr colrCenter, ulong time, float revsPerSec);

	// color space tests
	void SetStripColor(Colr colr);
	void GlobalAxis(uint face, float x, float y, float z, float w, Colr colr);
	void RainbowFace(uint face);
	void RainbowFaceLinear(uint face, float offs, float polarOffs);
};

#endif
