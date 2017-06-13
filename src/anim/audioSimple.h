#ifndef audiosimple_h
#define audiosimple_h

#include <list>
#include <vector>
#include "anim.h"
#include "leds.h"

struct Beat
{
	ulong msStart;

	Beat(ulong msStart);
};

class AudioPulse : public Mode
{
private:
	std::list<Beat> beats;
	std::vector<Anim*> anims;
	uint iAnim;

	void Spin(float phase, float audio, bool isBeat);

public:
	AudioPulse(LedStrip& strip, Input& input);
	void Update(Frame frame);

	void OnPressE();
};

#endif
