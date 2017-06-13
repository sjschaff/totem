#ifndef anim_h
#define anim_h

#include "util.h"
#include "input.h"
#include "leds.h"

class Anim
{
public:
	ulong msStart;
	ulong msDuration;
};

class Mode : BtnHandler
{
protected:
	LedStrip& strip;
	Input& input;

public:
	Mode(LedStrip& strip, Input& input);
	virtual void Update(ulong ms, float audio, bool isBeat, bool isBeginBeat) = 0;

	virtual void OnPressA();
	virtual void OnPressB();
	virtual void OnPressC();
	virtual void OnPressD();
	virtual void OnPressE();
	virtual void OnPressF();
};

#endif
