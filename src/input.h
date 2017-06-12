#ifndef input_h
#define input_h

#include "util.h"

class BtnHandler
{
public:
	virtual void OnPressA() = 0;
	virtual void OnPressB() = 0;
	virtual void OnPressC() = 0;
	virtual void OnPressD() = 0;
	virtual void OnPressE() = 0;
	virtual void OnPressF() = 0;
};

class Input
{
private:
	const uint bitsPrecision;
	const double ratio;

	BtnHandler* btnHandler;
	float AnalogRead(uint pin, float min, float max);

public:
	Input(BtnHandler* btnHandler);

	void PollInput();

	uint AnalogReadInt(uint pin, uint bitsPrecision);

	float ReadA(float min = 0, float max = 1);
	float ReadB(float min = 0, float max = 1);
	float ReadC(float min = 0, float max = 1);
};

#endif
