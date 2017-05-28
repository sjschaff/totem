#ifndef util_h
#define util_h

#include <Arduino.h>

//#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

using namespace glm;

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

// TODO doubles
// TODO we should prob just use a gamma table
ubyte gamma(ubyte v);

// TODO Template
double clamp(double v, double min, double max);
double saturate(double v);
double frac(double v);
double dmap(double val, double fMin, double fMax, double tMin, double tMax);

ubyte lerp(ubyte a, ubyte b, float fr);

#endif
