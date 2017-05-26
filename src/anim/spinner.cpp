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
	for (int iFace = 0; iFace < cFace; ++iFace)
	{
		int iLedFace = iFace*cTotal;

		double head = frac(revsPerSec * time / 1000.0);

		for (int i = 0; i < cOuter; ++i)
		{
			int iLed = iLedFace + outer[i];
			byte tail = Tail(head, 1 - (i / (float)(cOuter - 1)), .6, 2/(float)cOuter);
			setColor(iLed, col * tail);
		}

		for (int i = 0; i < cInner; ++i)
		{
			int iLed = iLedFace + inner[i];
			byte tail = Tail(head, 1 - (i / (float)(cInner - 1)), .6, 1/(float)cInner);
		//	setColor(iLed, col);//tail, 0, 0);
			setColor(iLed, col * tail);
		}

		float intens = (time % 1000) / 1000.0;
		byte v = intens * 255;
		setColor(iLedFace + center, col * v);//0x80, 0x00, 0x40);
	}

	show();
}
