#include "mode.h"
#include "anim/anims.h"

Chillax::Chillax(LedStrip& strip)
	: LightshowMode(strip)
{
	tipAnims.push_back(new AnimNoop(false));

	anims.push_back(new AnimBreathe(true));
	anims.push_back(new AnimBreathe(false));
	//anims.push_back(new AnimLighthouse(false, .2, 4000*4));

	colrs.push_back(new ColrSolid(Colr::Green));
}

void Chillax::OnPressC()
{
	NextAnim();
}
