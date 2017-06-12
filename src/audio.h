#ifndef audio_h
#define audio_h

#include <Audio.h>
#include <Wire.h>
#include "input.h"

class Audio
{//-mfpu=fpv4-sp-d16
private:
	Input& input;
	//AudioInputAnalog         adc;
//	AudioFilterStateVariable lowpass;
	//AudioAnalyzeFFT1024      fft1024;
//	AudioFilterBiquad        biquad;
//	AudioAnalyzePeak         peak;
	//AudioConnection          patchCord1;
//	AudioConnection          patchCord2;
	void ReadAudioValues();
	
public:
	Audio(Input& input);

	double ReadPeak();
	double ReadPeakToPeak();

	double ReadEnergy(uint maxBin); // Exclusive, to 512
};

#endif
