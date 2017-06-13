#include "util.h"

float gamma(float v)
{
  return pow(v, 2.2);
}

float clamp(float v, float min, float max)
{
  if (v < min) return min;
  else if (v > max) return max;
  else return v;
}

// TODO Template
float saturate(float v) { return clamp(v, 0, 1); }

float frac(float v)
{
  double intpart;
  return modf(v, &intpart);
}

// TODO Template
float dmap(float val, float fMin, float fMax, float tMin, float tMax)
{
  return clamp(
	  ((val - fMin) / (fMax - fMin)) * (tMax - tMin) + tMin,
	  tMin, tMax);
}

float mapfr(float val, float min, float max)
{
	return dmap(val, 0, 1, min, max);
}

float lerp(float a, float b, float fr)
{
	  return (b - a) * fr + a;
}

float frPhase(uint ms, uint phase)
{
	return (ms % phase) / (float)phase;
}

float smoothstep(float t)
{
	t = saturate(t);
	return t * t * (3.0 - 2.0 * t);
}

float smoothstepDual(float t)
{
	return 1 - smoothstep(abs(t));
}

float modDelta(float x, float y)
{
	float delta = abs(y - x);
	if (delta > .5)
		delta = 1 - delta;
	return delta;
}
