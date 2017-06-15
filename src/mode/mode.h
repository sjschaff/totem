#ifndef mode_h
#define mode_h

#include <list>
#include <vector>
#include "anim/anim.h"
#include "device/input.h"

class Mode : BtnHandler
{
protected:
	LedStrip& strip;
	Mode(LedStrip& strip);

public:
	virtual void Update(Frame frame) = 0;

	void OnPressA();

	virtual void OnPressB();
	virtual void OnPressC();
	virtual void OnPressD();
	virtual void OnPressE();
	virtual void OnPressF();
};

class LightshowMode : public Mode
{
protected:
	std::vector<Anim*> anims;
	std::vector<Anim*> tipAnims;
	std::vector<ColrAnim*> colrs;

	bool fManualMode = true;
	uint iAnim;
	uint iAnimTip;
	uint iColr;

	LightshowMode(LedStrip& strip);

public:
	void Update(Frame frame);
	void OnPressC();
	void OnPressD();
	void OnPressE();
	void OnPressF();
};

class AudioPulse : public LightshowMode
{
public:
	AudioPulse(LedStrip& strip);
	void Update(Frame frame);
};

class Chillax : public LightshowMode
{
public:
	Chillax(LedStrip& strip);
};

#endif
