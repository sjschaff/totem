
/*
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
*/

#include "audio.h"
/*
// GUItool: begin automatically generated code
///*
AudioInputAnalog         adc1;           //xy=254,136
AudioFilterBiquad        biquad1;        //xy=473,155
AudioAnalyzePeak         peak1;          //xy=362,274
AudioConnection          patchCord1(adc1, biquad1);
AudioConnection          patchCord2(biquad1, peak1);
// GUItool: end automatically generated code
*/
Audio::Audio() :
	adc1(),
	biquad1(),
	peak1(),
	patchCord1(adc1, biquad1),
	patchCord2(biquad1, peak1)
{
	biquad1.setLowpass(0, 250, 1);
	AudioMemory(10);
}

double Audio::ReadPeak()
{
	while (!peak1.available());
	return peak1.read();
}

double Audio::ReadPeakToPeak()
{
	while (!peak1.available());
	return peak1.readPeakToPeak();
}
