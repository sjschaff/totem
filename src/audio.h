#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
///*
AudioInputAnalog         adc1;           //xy=254,136
AudioFilterBiquad        biquad1;        //xy=473,155
AudioAnalyzePeak         peak1;          //xy=362,274
AudioConnection          patchCord1(adc1, biquad1);
AudioConnection          patchCord2(biquad1, peak1);
// GUItool: end automatically generated code
