#ifndef colr_h
#define colr_h

#include "util/util.h"

struct Colr
{
	static const Colr Red;
	static const Colr Yellow;
	static const Colr Green;
	static const Colr Cyan;
	static const Colr Blue;
	static const Colr Purple;

	static const Colr Black;
	static const Colr White;

	float r;
	float g;
	float b;

	Colr();
	Colr(float grey);
	Colr(float r, float g, float b);

	Colr operator*(float v) const;
	void operator*=(float v);

	Colr gammaCorrect();

	static Colr Lerp(Colr a, Colr b, float fr);
	static Colr Hue(float frHue);
};
/*


Colr Lerp(Colr a, Colr b, float fr)
{
  return ColrNew(
     lerp(a.r, b.r, fr),
     lerp(a.g, b.g, fr),
     lerp(a.b, b.b, fr)
   );
}

Colr ColrForHue(float hue)
{
    hue *= 2;
    if (hue > 1)
      hue = 2 - hue;

    return Lerp(ColrNew(0x00, 0x00, 0x00), ColrNew(0xFF, 0x00, 0xFF), hue);

    hue *= 6;
         if (hue < 1) return Lerp(ColrNew(0xFF, 0x00, 0x00), ColrNew(0xFF, 0xFF, 0x00), hue);
    else if (hue < 2) return Lerp(ColrNew(0xFF, 0xFF, 0x00), ColrNew(0x00, 0xFF, 0x00), hue - 1);
    else if (hue < 3) return Lerp(ColrNew(0x00, 0xFF, 0x00), ColrNew(0x00, 0xFF, 0xFF), hue - 2);
    else if (hue < 4) return Lerp(ColrNew(0x00, 0xFF, 0xFF), ColrNew(0x00, 0x00, 0xFF), hue - 3);
    else if (hue < 5) return Lerp(ColrNew(0x00, 0x00, 0xFF), ColrNew(0xFF, 0x00, 0xFF), hue - 4);
    else              return Lerp(ColrNew(0xFF, 0x00, 0xFF), ColrNew(0xFF, 0x00, 0x00), hue - 5);
}
*/
#endif
