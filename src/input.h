#ifndef input_h
#define input_h

#include "util.h"

class Input
{
private:
	const uint bitsPrecision;
	const double ratio;

public:
	Input(uint bitsPrecision);

	uint AnalogReadInt(uint pin, uint bitsPrecision);
	double AnalogRead(uint pin);
};

#endif
