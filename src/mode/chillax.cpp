#include "mode.h"
#include "anim/anims.h"

Chillax::Chillax(LedStrip& strip)
	: LightshowMode(strip)
{
	anims.push_back(new AnimBreathe(.21, 2000*2));
	anims.push_back(new AnimLighthouse(.2, 4000*4));
}
