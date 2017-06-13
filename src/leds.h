#ifndef leds_h
#define leds_h

#include <Adafruit_NeoPixel.h>
#include "colr.h"
#include "lmath.h"

#define ForEachLed(var) for (uint var = 0; var < LedStrip::cLED; ++var)

struct Led
{
	uint iLed; // global
	struct Face {
		uint iLed; // per face

		uint iRing;
		float rad;
		float frRad; // 0 to ~3

		float polar;
		float frPolar;
		float polarDelta;
		float frPolarDelta;

		vec2 pt;
	} face;

	uint iFace; // 0-5
	uint iFacePolar; // 0-10
	vec3 zpt;
	float frPolar;
	float rad;
};

class LedStrip
{
private:
	static Led* GenerateLeds();

	Adafruit_NeoPixel strip;
	float frBrightness;

	void SetColorRaw(ushort px, Colr colr);
	Colr ColrRawForColr(Colr colr);

public:
	static const ushort cLED;

	LedStrip();
	const Led* const leds;

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
