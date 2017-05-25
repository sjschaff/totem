
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif
Adafruit_NeoPixel strip = Adafruit_NeoPixel(195, 6, NEO_GRB + NEO_KHZ800);

typedef struct _Colr
{
  int r;
  int g;
  int b;

  uint32_t ToColor()
  {
    return strip.Color(r,g,b);
  }

} Colr;

Colr ColrNew(int r, int g, int b)
{
    Colr colr;
    colr.r = r;
    colr.g = g;
    colr.b = b;
    return colr;
}

int lerp(int a, int b, float fr)
{
  return ((b - a) * fr + a);
}

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
