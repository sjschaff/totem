#include "util.h"

ubyte gamma(ubyte v)
{
  return (ubyte)(pow(v/255.0, 2.2) * 255);
}

double clamp(double v, double min, double max)
{
  if (v < min) return min;
  else if (v > max) return max;
  else return v;
}

// TODO Template
double saturate(double v) { return clamp(v, 0, 1); }

double frac(double v)
{
  double intpart;
  return modf(v, &intpart);
}

// TODO Template
double dmap(double val, double fMin, double fMax, double tMin, double tMax)
{
  return ((val - fMin) / (fMax - fMin)) * (tMax - tMin) + tMin;
}
