#include "log.h"

Log* Log::log = nullptr;

Log::Log(bool enable) : enable(enable)
{
	log = this;
	if (enable)
		Serial.begin(9600);
}
