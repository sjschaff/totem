/**
 * Blink
 *
 * Turns on an LED on for one second,
 * then off for one second, repeatedly.
 */
#include "Arduino.h"
#include "audio.h"
#include "leds.h"

#include <Adafruit_NeoPixel.h>
  #ifdef __AVR__
    #include <avr/power.h>
  #endif
  Adafruit_NeoPixel strip = Adafruit_NeoPixel(cLED, 3, NEO_GRB + NEO_KHZ800);

#define COLOR Color
#define SETPX setPixelColor


#include <Plotter.h>
Plotter plot;

#include <Statistic.h>
Statistic stats;


double plotVal = 0;
double plotSmoothed = 0;
double plotSmoothed2 = 0;
double plotAvg = 0;
double plotStd = 0;
double plotThresh = 0;

double plotA5 = 50;
double plotA4 = 0;
double plotMax = 1;
double plotMin = -1;
double anPlotMin = 0;

void setup() {
    biquad1.setLowpass(0, 250, 1);
  pinMode(3, OUTPUT);
  AudioMemory(10);
  Serial.begin(9600);
  plot.Begin();
  plot.AddTimeGraph("Audio Sensor", 500,
    //"val", plotVal,
    "smooth", plotSmoothed,
    "smooth rel", plotSmoothed2,
    "avg", plotAvg,
    "std", plotStd,
   // "thresh", plotThresh,
    //"max", plotMax,
    "min", anPlotMin
    );

  /*  plot.AddTimeGraph("Analog Input", 50,
    "A5", plotA5,
    "A4", plotA4,
    "max", plotMax,
    "min", anPlotMin
    );*/

}


uint32_t colors[] = {
  strip.COLOR(0, 0, 64),
  strip.COLOR(32, 0, 32),
  strip.COLOR(64, 0, 0),
  strip.COLOR(32, 0, 32)
};

int cColors = 4;


byte gamma(byte v)
{
  return (byte)(pow((v*.5)/255.0, 2.2) * 255);
}

void setColor(int i, byte r, byte g, byte b)
{
  strip.SETPX(i, gamma(r), gamma(g), gamma(b));
}

void setColor(int i, byte r, byte g, byte b, byte brightness)
{
   uint32_t v = brightness;
  strip.SETPX(i,
    (r * v) >> 8,
    (g * v) >> 8,
    (b * v) >> 8);
}

void setColor(int i, uint32_t col, byte brightness)
{
  setColor(i, col >> 16, col >> 8, col, brightness);
}

void setColor(int i, uint32_t col)
{
  strip.SETPX(i, col);
}

float clamp(float v, float min, float max)
{
  if (v < min)
    return min;
  else if (v > max)
    return max;
  else
    return v;
}

float saturate(float v)
{
  return clamp(v, 0, 1);
}

float frac(float v)
{
  double intpart;
  return (float)modf(v, &intpart);
}

byte Tail(float head, float led, float len, float headLen)
{
  float dist = (head > led) ? head - led : 1 - (led - head);

  float fr = 1 - saturate(dist / len);
  fr += saturate((dist - (1-headLen)) / headLen);

  return (byte)(fr * 255);
}

int iOffset = 0;
int time = 0;
int frame = 0;

void delayCount(int amt)
{
  delay(amt);
  time += amt;
  time = time % 6000;
}

float revsPerSec = 2;

typedef unsigned long ulong;
typedef unsigned int uint;

void loopSpin();
void _loopSpin();
void _loopSpin(uint32_t col);
double dmap(double val, double, double, double, double);


int time2 = 0;
void loop()
{
  loopSpin();
}

bool loudBefore = false;
//double threshold = 3;//290;
double influence = .5;//

double dmap(double val, double fMin, double fMax, double tMin, double tMax)
{
  return ((val - fMin) / (fMax - fMin)) * (tMax - tMin) + tMin;
}

void loopSpin()
{
  const int cSmpMax = 1024;
  double samples[cSmpMax];
  double samplesSmooth[cSmpMax];
  for (int i = 0; i < cSmpMax; ++i) samples[i] = samplesSmooth[i] = 0;
  uint iSmp = 0;

  bool wasLoud = false;
  int frame2 = 0;

  // config
  uint cSmp = 42;
  uint cReadback = 13;
  double threshold = 1.2;

  while(true)
  {
    while (!peak1.available());
    double raw =
      //peak1.read();
      peak1.readPeakToPeak();

    double maxBrightness = .4;

    //cReadback = plotA5+1; // 13
    double wt = 1 - 1.0/cReadback;

    samples[iSmp] = raw;

    double sound = 0;
    for (int i = 0; i <= cReadback; ++i)
    {
      double smp = samples[iSmp - (cReadback - i)];
      sound = sound * wt + smp * (1- wt);
    }

    samplesSmooth[iSmp] = sound;


    cSmp = (analogRead(A20) >> 10) + 32;

    for (uint i = 0; i < cSmp; ++i)
    {
      int iSmpAvg = (iSmp - 1 - i + cSmpMax) % cSmpMax;
      stats.add(samplesSmooth[iSmpAvg]);
    }

    iSmp = (iSmp + 1) % cSmpMax;


    threshold = dmap(analogRead(A19) >> 8, 0, 256, 1, 3);
    double avg = stats.average();
    double std = stats.pop_stdev();
    double thresh = avg + std * threshold;

    plotSmoothed = sound;
    plotSmoothed2 = sound - avg;
    if (sound > thresh)
    {
      sound = 1;
      if (!wasLoud)
      {
        frame2 = (frame2 + 1) % 3;
        wasLoud = true;
      }
    }
    else
    {
      sound = 0;
      wasLoud = false;
    }



    byte value = 0xFF*maxBrightness*sound;
    for (int i = 0; i < 195; ++i)
    {
     setColor(i,
        frame2 == 0 ? value : 0,
        frame2 == 1 ? value : 0,
        frame2 == 2 ? value : 0);
    }
    strip.show();



    plotVal = raw;
    //plotSmoothed = sound;
    //plotSmoothed2 = sound - avg;
    plotAvg = avg; // this is nan every once in a while
    plotStd = thresh - avg;

    stats.clear();
    plot.Plot();

    continue;


    /*


      //_loopSpin(strip.COLOR(dmap(sound, -1, 1, 0, 255), 0, 0));
       samples[iSmp] = raw;// * influence + samples[(iSmp+cSmp-1)%cSmp] * (1-influence);
       iSmp = (iSmp + 1) % cSmp;*/
  }
}

void _loopSpin()
{
  uint32_t red = strip.COLOR(32,0,0);
  uint32_t green = strip.COLOR(0,32,0);
  uint32_t blue = strip.COLOR(0,0,32);

  uint32_t col = 0;
  int colSwitch = (int)(time / 1000);
  colSwitch = frame;
  switch ((colSwitch) % 6)
  {
    case 0:
      col = red; break;
    case 1:
      col = green; break;
    case 2:
      col = blue; break;
    case 3:
      col = strip.COLOR(32, 32, 0); break;
    case 4:
      col = strip.COLOR(32, 0, 32); break;
    case 5:
      col = strip.COLOR(0, 32, 32); break;
    default:
      col = strip.COLOR(10 ,10,10);
  }
  _loopSpin(col);
}

void _loopSpin(uint32_t col)
{

  unsigned long m = millis();
  for (int iFace = 0; iFace < cFace; ++iFace)
  {
    int offsFace = iOffset;// + iFace;
    int iLedFace = iFace*cTotal;

    float headOuter = frac(revsPerSec * time / 1000.0);
    float headInner = frac(1.5*revsPerSec * time / 1000.0);

    uint32_t colOuter = colors[offsFace % cColors];
    for (int i = 0; i < cOuter; ++i)
    {
      int iLed = iLedFace + outer[i];
      byte tail = Tail(headOuter, 1 - (i / (float)(cOuter - 1)), .6, 2/(float)cOuter);
      setColor(iLed, col, tail);
    }

    uint32_t colInner = colors[(offsFace + 1) % cColors];
    for (int i = 0; i < cInner; ++i)
    {
      int iLed = iLedFace + inner[i];
      byte tail = Tail(headOuter, 1 - (i / (float)(cInner - 1)), .6, 1/(float)cInner);
    //  setColor(iLed, col);//tail, 0, 0);
      setColor(iLed, col, tail);
    }

    uint32_t colCenter = colors[(offsFace + 2) % cColors];
    float intens = (time % 1000) / 1000.0;
    byte v = intens * 255;
    setColor(iLedFace + center, col, v);//0x80, 0x00, 0x40);
  }

  ++iOffset;
  strip.show();
}



/*
int led = 149;
int numLeds = 150;




const float wavelen = 5;//10;
const float tailLen = 4;
const float speed = .01;

const float litMax = 1;

 int time = 0;
const int step = 16;
const int timeMod = wavelen / speed;

float saturate(float v)
{
  return constrain(v, 0, 1);
}

void loop() {
  float litIntensity = analogRead(0) / 1023.0;

  for (int i = 0; i < numLeds; ++i)
  {
   // float v  = sin(time*speed + i * 2 * 3.141592 / wavelen) * .5 + .5;
    float v = fmod(i+time*speed, wavelen) / tailLen;
    v = saturate(1 - v);
    v*= litIntensity;
    int c = v * 255;
    strip.SETPX(i, strip.COLOR(.3*c,0,c));
  }
  //Colr colr = ColrForHue(time / 255.0);
  //time = (time + 1) % 255;
  //strip.SETPX(led, colr.ToColor());
  strip.show();

  time += step;
  if (time > timeMod)
    time -= timeMod;

  /*if ((int)(time / 1000) % 2 == 1)
  {
    digitalWrite(7, LOW);
  }
  else* /
  {
   // digitalWrite(7, HIGH);
  }

  delay(step);
}*/
