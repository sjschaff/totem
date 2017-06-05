#ifndef input_h
#define input_h

#include "util.h"

typedef void (*btnPressHandler)(void* data);

class Input
{
private:
	const uint bitsPrecision;
	const double ratio;

public:
	Input(uint bitsPrecision);

	uint AnalogReadInt(uint pin, uint bitsPrecision);
	double AnalogRead(uint pin);

	void attachBtnPressHandler(uint pin, btnPressHandler handler, void* data);
};

#endif
