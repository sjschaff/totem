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
	float AnalogRead(uint pin);

public:
	Input(BtnHandler* btnHandler);

	void PollInput();

	uint AnalogReadInt(uint pin, uint bitsPrecision);

	float ReadA();
	float ReadB();
	float ReadC();
};

#endif
