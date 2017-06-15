#include "totem.h"

void initRand()
{
	long a = analogRead(A13); // disconnected pin
	if (a == 0)
		a = 1;

	delay(24);

	long b = analogRead(A13);
	if (b == 0)
		b = 2;

	long seed = a*b;
	if (seed == 0)
		seed = 3;
		
	randomSeed(seed);
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
