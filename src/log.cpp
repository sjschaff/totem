#include "log.h"

Log::Log(bool enable) : enable(enable)
{
	if (enable)
		Serial.begin(9600);
}
