#ifndef frame_h
#define frame_h

#include "device/audio.h"

class ColrAnim;

struct Frame
{
	ulong ms;
	float knobA;
	float knobB;
	float knobC;
	ColrAnim* colr;
	AudioData audio;
};

#endif
