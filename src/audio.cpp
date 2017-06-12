
/*
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
*/

#include "audio.h"

static const uint strobePin = 14;
static const uint resetPin = 15;

Audio::Audio(Input& input) : input(input) //:
	//patchCord1(adc, fft1024)
//	patchCord2(lowpass, fft1024)
{
	pinMode(resetPin, OUTPUT);
	pinMode(strobePin, OUTPUT);

	//biquad.setLowpass(0, 250, 1);
	/*lowpass.frequency(200);
	lowpass.resonance(.707);
	lowpass.octaveControl(1);
	AudioMemory(10);*/
}

double Audio::ReadPeak()
{
/*	while (!peak.available());
	return peak.read();*/
	return 1;
}

double Audio::ReadPeakToPeak()
{/*
	while (!peak.available());
	return peak.readPeakToPeak();*/
	return 1;
}

// TODO: Return stuff
void Audio::ReadAudioValues()
{
	digitalWrite(strobePin, LOW);
	digitalWrite(resetPin, HIGH);
	delayMicroseconds(1);
	digitalWrite(resetPin, LOW);

	uint values[7];
	for (int i = 0; i < 7; ++i)
	{
		digitalWrite(strobePin, HIGH);
		delayMicroseconds(40);
		digitalWrite(strobePin, LOW);
		delayMicroseconds(20);
		values[i] = input.AnalogReadInt(A22, 10);
		delayMicroseconds(20);
	}

		/*	for (int i = 0; i < 7; ++i)
				log << values[i] << ", ";
			log << "\n";
		*///	delay(500);
}

double Audio::ReadEnergy(uint maxBin)
{
	/*while (!fft1024.available()); // Skip every other (1/2 overlap)
	// TODO: combine? despite 50% overlap?
	while (!fft1024.available());

	double energy = 0;
	for (uint i = 1; i < maxBin; ++i)
	{
		double bin = fft1024.read(i);// supposedly = sqrt(real^2 + imag^2),
		energy += bin;
		// TODO nead squaring?doubling for 512 -> 1024
		//*100;//bin;
	}*/

	return 1;//energy;
}
