#ifndef anims_h
#define anims_h

#include "anim.h"

struct AnimSpin
{
	void Display(LedStrip& strip, Frame frame, float phase);
};

struct AnimFlash : public Anim
{
private:
	bool wasLoud;
	uint iColr;

public:
	AnimFlash();
	void Update(LedStrip& strip, Frame frame);
};

struct AnimLighthouse
{
	void Display(LedStrip& strip, Frame frame, float phase);
};

#endif
