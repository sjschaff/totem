#include "input.h"
#include "log.h"

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

uint _pin;
btnPressHandler _handler;
void* _data;
// TODO: Multi pin blech
void handlerProxy()
{
	_handler(_data);
}

void handleBtn(uint pin)
{
	*Log::log << "pin " << pin << "\n";
}

#define DeclHandler(pin) void handle##pin() { handleBtn(pin); }

//void handle21() { handleBtn(21); }
DeclHandler(22);
DeclHandler(21);
DeclHandler(20);
DeclHandler(19);
DeclHandler(18);
DeclHandler(17);

void Input::attachBtnPressHandler(uint pin, btnPressHandler handler, void* data)
{
	pinMode(22, INPUT_PULLUP);
	pinMode(21, INPUT_PULLUP);
	pinMode(20, INPUT_PULLUP);
	pinMode(19, INPUT_PULLUP);
	pinMode(18, INPUT_PULLUP);
	pinMode(17, INPUT_PULLUP);
	attachInterrupt(digitalPinToInterrupt(22), handle22, RISING);
	attachInterrupt(digitalPinToInterrupt(21), handle21, RISING);
	attachInterrupt(digitalPinToInterrupt(20), handle20, RISING);
	attachInterrupt(digitalPinToInterrupt(19), handle19, RISING);
	attachInterrupt(digitalPinToInterrupt(18), handle18, RISING);
	attachInterrupt(digitalPinToInterrupt(17), handle17, RISING);

	/*pinMode(pin, INPUT_PULLUP);
	_pin = pin;
	_handler = handler;
	_data = data;
	attachInterrupt(digitalPinToInterrupt(pin), handlerProxy, RISING);*/
}
