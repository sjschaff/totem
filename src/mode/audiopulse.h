#ifndef audiopulse_h
#define audiopulse_h

#include <list>
#include <vector>
#include "mode.h"

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
