#ifndef leds_h
#define leds_h

#include <Adafruit_NeoPixel.h>
#include "colr.h"

struct Led
{
	uint iFace;

	float polarFace;
	float radFace;

	float xFace, yFace;
};

class LedStrip
{
public:
	// TODO Make useful
	static const ushort cLED;
	static const ushort outer[];
	static const ushort cOuter;
	static const ushort inner[];
	static const ushort cInner;
	static const ushort center;
	static const ushort cFace;
	static const ushort cTotal;

private:
	static Led* GenerateLeds();
	Adafruit_NeoPixel strip;
	const Led* const leds;
	void setColorNoGamma(ushort px, Colr colr);

public:
	LedStrip(int pin);

	void setColor(ushort px, Colr colr);
	void show();

	void spinStrip(Colr colr, ulong time);

	// color space tests
	void setStripColor(Colr colr);
	void verticalLineFace(uint face, float x, float w);
	void rainbowFace(uint face, float offs, float polarOffs);
};

#endif
