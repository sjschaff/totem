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
	// TODO make anims more sparing with simultaneous lit leds
	strip.SetBrightness(mapfr(frame.knobA, .2, .85));

	if (frame.audio.isBeginBeat)
		beats.push_back(Beat(frame.ms));

	frame.colr = colrs[iColr];
	frame.colr->Update(frame);
	Anim* anim = anims[iAnim];
	anim->Update(strip, frame);
	strip.Show();
}

void LightshowMode::OnPressE()
{
	iAnim = (iAnim + 1) % anims.size();
}

void LightshowMode::OnPressF()
{
	iColr = (iColr + 1) % colrs.size();
}
