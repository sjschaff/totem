#ifndef anim_h
#define anim_h

#include "util.h"

class Anim
{
public:
	ulong msStart;
	ulong msDuration;
};

class Mode
{
public:
	virtual void Update(ulong ms, float audio, bool isLoud) = 0;
};

#endif
