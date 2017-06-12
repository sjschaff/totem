#ifndef util_h
#define util_h

#include <Arduino.h>

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef uint8 ubyte;
typedef uint16 ushort;
//typedef uint32 uint;
//typedef uint64 ulong;

#ifndef PI
#define PI 3.141592
#endif

constexpr float rads(float degs)
{
	return degs * PI / 180;
}

constexpr float dihedral = rads(116.5650512);


// TODO floats
// TODO we should prob just use a gamma table
float gamma(float v);

// TODO Template
float clamp(float v, float min, float max);
float saturate(float v);
float frac(float v);
float dmap(float val, float fMin, float fMax, float tMin, float tMax);
float mapfr(float val, float min, float max);

float lerp(float a, float b, float fr);

#endif
