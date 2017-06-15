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

	uint iAnim = 0;
	uint iAnimTip = 0;
	uint iColr = 0;

	LightshowMode(LedStrip& strip);
	void NextRandom();
	void NextAnim();
	void NextTip();
	void NextColr();

public:
	void Update(Frame frame);
};

class AudioPulse : public LightshowMode
{
private:
	bool fLockAnim = false;
	ulong msAnimCur = 0;
	ulong msAnimTarget = 0;

	void StartRandomTimer();

public:
	AudioPulse(LedStrip& strip);
	void Update(Frame frame);

	void OnPressB();
	void OnPressC();
	void OnPressD();
	void OnPressE();
	void OnPressF();
};

class Chillax : public LightshowMode
{
public:
	Chillax(LedStrip& strip);

	void OnPressC();
};

#endif
