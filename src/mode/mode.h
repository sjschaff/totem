#ifndef mode_h
#define mode_h

#include "anim/anim.h"
#include "device/input.h"

class Mode : BtnHandler
{
protected:
	LedStrip& strip;
	Input& input;

public:
	Mode(LedStrip& strip, Input& input);
	virtual void Update(Frame frame) = 0;

	virtual void OnPressA();
	virtual void OnPressB();
	virtual void OnPressC();
	virtual void OnPressD();
	virtual void OnPressE();
	virtual void OnPressF();
};

#endif
