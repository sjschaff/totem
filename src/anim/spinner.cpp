#include "leds.h"

const double revsPerSec = 2;

byte Tail(float head, float led, float len, float headLen)
{
	float dist = (head > led) ? head - led : 1 - (led - head);

	float fr = 1 - saturate(dist / len);
	fr += saturate((dist - (1-headLen)) / headLen);

	return (byte)(fr * 255);
}

void LedStrip::spinStrip(Colr col, ulong time)
{
	time = time % 6000;
	double head = frac(revsPerSec * time / 1000.0);

	for (uint i = 0; i < cLED; ++i)
	{
		Led led = leds[i];
		if (led.iRing == 0)
			setColor(i, Colr(0));
		else
		{
			float aa = (led.iRing == 2 ? 2 : 1)*led.polarDelta;
			byte tail = Tail(head, led.polarFace, .6, aa);
			setColor(i, col * tail);
		}
	}

	show();
}
