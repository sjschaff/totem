#ifndef leds_h
#define leds_h

#include <Adafruit_NeoPixel.h>
#include "colr.h"

struct Led
{
	uint iFace;
	uint iRing;
	float polarFace;

	// computed from iRad
	float polarDelta; // ex. duPixel
	float radFace;

	vec2 ptFace;
	vec3 zpt;
};

class LedStrip
{
public:
	// TODO Make useful
	static const ushort cLED;

private:
	Adafruit_NeoPixel strip;
	const Led* const leds;
	void setColorNoGamma(ushort px, Colr colr);

public:
	LedStrip(ushort pin);

	void setColor(ushort px, Colr colr);
	void show();

	void spinStrip(Colr colr, ulong time);

	// color space tests
	void setStripColor(Colr colr);
	void globalAxis(uint face, float x, float y, float z, float w);
	void rainbowFace(uint face);
	void rainbowFaceLinear(uint face, float offs, float polarOffs);
};

#endif
