#ifndef audiosimple_h
#define audiosimple_h

#include "anim.h"
#include "leds.h"

class AudioSimple : public Mode
{
private:
	LedStrip& strip;
	uint frame = 0;
	bool wasLoud = false;

	void LightStrip(float audio, bool isLoud);

public:
	AudioSimple(LedStrip& strip);
	void Update(ulong ms, float audio, bool isLoud);
};

#endif
