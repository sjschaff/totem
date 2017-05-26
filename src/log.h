#ifndef log_h
#define log_h

#include <Arduino.h>

class Log
{
private:
	const bool enable;

public:
	Log(bool enable);

	template<class T>
	void print(const T& t)
	{
		if (enable)
			Serial.print(t);
	}

	template<class T>
	void println(const T& t)
	{
		if (enable)
			Serial.println(t);
	}
};

#endif
