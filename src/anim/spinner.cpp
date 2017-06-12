#include "leds.h"
#include "util/stripbuilder.h"

float Tail(float head, float led, float len, float headLen)
{
	float dist = (head > led) ? head - led : 1 - (led - head);

	float fr = 1 - saturate(dist / len);
	fr += saturate((dist - (1-headLen)) / headLen);

	return fr;
}

void LedStrip::SpinStrip(Colr col, Colr colCenter, ulong time, float revsPerSec)
{
	float head = frac(revsPerSec * time / 1000.0);

	float tailLen = .6;

	for (uint i = 0; i < cLED; ++i)
	{
		Led led = leds[i];
		if (led.iRing == 0)
			SetColor(i, colCenter);//Colr(0));
		else
		{
			float aa = 1.5*(led.iRing == 2 ? 2 : 1)*led.polarDelta / (2*PI);
			float tail = Tail(head, led.polarFace / (2 * PI), tailLen, aa);
			SetColor(i, col*tail);
		}
	}

	Show();
}
