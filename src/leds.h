#ifndef leds_h
#define leds_h

#include <Adafruit_NeoPixel.h>
#include "colr.h"

class LedStrip
{
	private:
		Adafruit_NeoPixel strip;
		void setColorNoGamma(ushort px, Colr colr);

	public:
		LedStrip(int pin);

		void setColor(ushort px, Colr colr);
		void show();

		void setStripColor(Colr colr);
		void spinStrip(Colr colr, ulong time);

		// TODO Make useful
		static const ushort cLED;
		static const ushort outer[];
		static const ushort cOuter;
		static const ushort inner[];
		static const ushort cInner;
		static const ushort center;
		static const ushort cFace;
		static const ushort cTotal;
};

#endif
