#ifndef audiosimple_h
#define audiosimple_h

#include <list>
#include "anim.h"
#include "leds.h"

class AudioSimple : public Mode
{
private:
	uint frame = 0;
	bool wasLoud = false;

	void LightStrip(float audio, bool isLoud);

public:
	AudioSimple(LedStrip& strip, Input& input);
	void Update(ulong ms, float audio, bool isBeat, bool isBeginBeat);
};

struct Beat
{
	ulong msStart;

	Beat(ulong msStart);
};

class AudioPulse : public Mode
{
private:
	uint iLed;
	std::list<Beat> beats;

	ulong msPrev;
	ulong msPhase;

	void Spin(float phase, float audio, bool isBeat);

public:
	AudioPulse(LedStrip& strip, Input& input);
	void Update(ulong ms, float audio, bool isBeat, bool isBeginBeat);
};

#endif
