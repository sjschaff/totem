
/*
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
*/

#include "audio.h"

Audio::Audio() :
	patchCord1(adc, fft1024)
//	patchCord2(lowpass, fft1024)
{
	//biquad.setLowpass(0, 250, 1);
	/*lowpass.frequency(200);
	lowpass.resonance(.707);
	lowpass.octaveControl(1);*/
	AudioMemory(10);
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

double Audio::ReadEnergy(uint maxBin)
{
	while (!fft1024.available()); // Skip every other (1/2 overlap)
	while (!fft1024.available());

	double energy = 0;
	for (uint i = 1; i < maxBin; ++i)
	{
		double bin = fft1024.read(i);
		energy += bin;
		// TODO nead squaring?
		//*100;//bin;
	}

	return energy;
}
