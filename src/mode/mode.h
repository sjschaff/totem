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


struct Beat
{
	ulong msStart;

	Beat(ulong msStart);
};

class LightshowMode : public Mode
{
protected:
	std::list<Beat> beats;
	std::vector<Anim*> anims;
	std::vector<ColrAnim*> colrs;
	uint iAnim;
	uint iColr;

	LightshowMode(LedStrip& strip);

public:
	void Update(Frame frame);
	void OnPressE();
	void OnPressF();
};

class AudioPulse : public LightshowMode
{
public:
	AudioPulse(LedStrip& strip);
};

class Chillax : public LightshowMode
{
public:
	Chillax(LedStrip& strip);
};

#endif
