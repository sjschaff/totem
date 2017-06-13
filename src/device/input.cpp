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

 static Input* inputGlobal = nullptr;

 #define DeclHandler(id) static volatile bool fPressed##id = false; void handle##id() { fPressed##id = true; }

 DeclHandler(A);
 DeclHandler(B);
 DeclHandler(C);
 DeclHandler(D);
 DeclHandler(E);
 DeclHandler(F);

 #define SetupBtn(pin, id) \
 	pinMode(pin, INPUT_PULLUP); \
 	attachInterrupt(digitalPinToInterrupt(pin), handle##id, RISING);

#define CheckBtn(id) if (fPressed##id) { btnHandler->OnPress##id(); fPressed##id = false; }

void Input::PollInput()
{
	CheckBtn(A);
	CheckBtn(B);
	CheckBtn(C);
	CheckBtn(D);
	CheckBtn(E);
	CheckBtn(F);
}

Input::Input(BtnHandler* btnHandler) :
	bitsPrecision(10),
	ratio(1.0 / ((1<<bitsPrecision) - 1)),
	btnHandler(btnHandler)
{
	inputGlobal = this;
	SetupBtn(22, A);
	SetupBtn(21, B);
	SetupBtn(20, E);
	SetupBtn(19, C);
	SetupBtn(18, F);
	SetupBtn(17, D);
}

uint Input::AnalogReadInt(uint pin, uint bitsPrecision)
{
	uint bitsOffset = this->bitsPrecision - bitsPrecision;
	return analogRead(pin) >> bitsOffset;
}

float Input::AnalogRead(uint pin, float min, float max)
{
	return dmap(
		analogRead(pin) * ratio,
		0, 1,
		min, max); // TODO condense into one range mapping
}

float Input::ReadA(float min, float max) { return AnalogRead(A20, min, max); }
float Input::ReadB(float min, float max) { return AnalogRead(A19, min, max); }
float Input::ReadC(float min, float max) { return AnalogRead(A18, min, max); }
