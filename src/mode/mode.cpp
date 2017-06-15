#include "mode.h"

Mode::Mode(LedStrip& strip)
	: strip(strip) {}

void Mode::OnPressA() {}
void Mode::OnPressB() {}
void Mode::OnPressC() {}
void Mode::OnPressD() {}
void Mode::OnPressE() {}
void Mode::OnPressF() {}

LightshowMode::LightshowMode(LedStrip& strip)
	: Mode(strip), iAnim(0), iColr(0) {}

void LightshowMode::Update(Frame frame)
{
	strip.SetBrightness(mapfr(frame.knobA, .2, .9));

	frame.colr = colrs[iColr];
	frame.colr->Update(frame);
	Anim* anim = anims[iAnim];
	anim->Update(strip, frame);

	Anim* tipAnim = tipAnims[iAnimTip];
	tipAnim->Update(strip, frame);
	strip.Show();
}

void LightshowMode::NextRandom()
{
	iAnim = random(anims.size());
	iAnimTip = random(tipAnims.size());
	iColr = random(colrs.size());
}

void LightshowMode::NextAnim()
{
	iAnim = (iAnim + 1) % anims.size();
}

void LightshowMode::NextTip()
{
	iAnimTip = (iAnimTip + 1) % tipAnims.size();
}

void LightshowMode::NextColr()
{
	iColr = (iColr + 1) % colrs.size();
}
