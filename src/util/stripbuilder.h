#ifndef stripbuilder_h
#define stripbuilder_h

#include "util.h"
#include "lmath.h"

struct Led
{
	uint iFace;
	uint iRing;
	float polarFace;

	// computed from iRad
	float polarDelta; // ex. duPixel
	float radFace;

	vec2 ptFace;
	vec3 zpt;
};

#endif
