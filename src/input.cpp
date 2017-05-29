#include "input.h"

// TODO Static assert don't use ADC0
// TODO Verify if dual adc ports can be used
/**
 *	ADC's:

 	A0-A1: ADC0
	A2-A3: Dual
	A4-A9: ADC0
	A10-A11: Unknown / Differential
	A12-A13: ADC1
	A14-A15: ADC0
	A16-A20: ADC1
	A21: ADC0
	A22: ADC1
 */

Input::Input(uint bitsPrecision) :
	bitsPrecision(bitsPrecision),
	ratio(1.0 / ((1<<bitsPrecision) - 1))
{
	//analogReference(0);//EXTERNAL);
}

uint Input::AnalogReadInt(uint pin, uint bitsPrecision)
{
	uint bitsOffset = this->bitsPrecision - bitsPrecision;
	return analogRead(pin) >> bitsOffset;
}

double Input::AnalogRead(uint pin)
{
	return analogRead(pin) * ratio;
}
