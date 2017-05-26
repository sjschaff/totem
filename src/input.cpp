#include "input.h"

Input::Input(uint bitsPrecision) :
	bitsPrecision(bitsPrecision),
	ratio(1.0 / ((1<<bitsPrecision) - 1))
{}

uint Input::AnalogReadInt(uint pin, uint bitsPrecision)
{
	uint bitsOffset = this->bitsPrecision - bitsPrecision;
	return analogRead(pin) >> bitsOffset;
}

double Input::AnalogRead(uint pin)
{
	return analogRead(pin) * ratio;
}
