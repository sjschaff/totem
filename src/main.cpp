#include "totem.h"

// Cause fuck that globals shit
//Main main;
void setup() {
	Totem totem;
	while (1) {
		totem.loop();
		yield();
	}
}

void loop() {}
