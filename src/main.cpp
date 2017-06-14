#include "totem.h"

void initRand()
{
	long a = analogRead(A13); // disconnected pin
	delay(24);
	long b = analogRead(A13);
	randomSeed(a*b);
}

// Cause fuck that globals shit
//Main main;
void setup() {
	delay(100);
	initRand();
	Totem* totem = new Totem();
	delay(100);
	while (1) {
		totem->loop();
		yield();
	}
}

void loop() {}
