
#include "util.h"
#include "audio.h"
#include "leds.h"
typedef LedStrip Led;

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


Colr Colors[] = {
	Colr(0, 0, 64),
	Colr(32, 0, 32),
	Colr(64, 0, 0),
	Colr(32, 0, 32)
};

int cColors = 4;

class Totem
{
public:
	LedStrip strip;
	Audio audio;

public:
	Totem() : strip(3), audio()
	{
		setup();
	}

void setup() {
	return;

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

	/*	plot.AddTimeGraph("Analog Input", 50,
		"A5", plotA5,
		"A4", plotA4,
		"max", plotMax,
		"min", anPlotMin
		);*/

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

int time2 = 0;
void loop()
{
	loopSpin();
}

bool loudBefore = false;
//double threshold = 3;//290;
double influence = .5;//


void loopSpin()
{
	const int cSmpMax = 1024;
	double samples[cSmpMax];
	double samplesSmooth[cSmpMax];
	for (int i = 0; i < cSmpMax; ++i) samples[i] = samplesSmooth[i] = 0;
	uint iSmp = 0;

	bool wasLoud = false;
	uint frame2 = 0;

	// config
	uint cSmp = 42;
	uint cReadback = 13;
	double threshold = 1.2;

	while(true)
	{
		double raw = audio.ReadPeakToPeak();

		double maxBrightness = .4;

		//cReadback = plotA5+1; // 13
		double wt = 1 - 1.0/cReadback;

		samples[iSmp] = raw;

		double sound = 0;
		for (uint i = 0; i <= cReadback; ++i)
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


		Colr colr = frame2 == 0
			? Colr::Red
			: frame2 == 1
				? Colr::Green
				: Colr::Blue;

		colr *= (maxBrightness * sound);

		for (ushort i = 0; i < Led::cLED; ++i)
			strip.setColor(i, colr);

		strip.show();



		plotVal = raw;
		//plotSmoothed = sound;
		//plotSmoothed2 = sound - avg;
		plotAvg = avg; // this is nan every once in a while
		plotStd = thresh - avg;

		stats.clear();
		plot.Plot();

		yield();
		continue;


		/*


			//_loopSpin(strip.Color(dmap(sound, -1, 1, 0, 255), 0, 0));
			 samples[iSmp] = raw;// * influence + samples[(iSmp+cSmp-1)%cSmp] * (1-influence);
			 iSmp = (iSmp + 1) % cSmp;*/
	}
}

void _loopSpin()
{
	Colr red = Colr(32,0,0);
	Colr green = Colr(0,32,0);
	Colr blue = Colr(0,0,32);

	Colr col;
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
			col = Colr(32, 32, 0); break;
		case 4:
			col = Colr(32, 0, 32); break;
		case 5:
			col = Colr(0, 32, 32); break;
		default:
			col = Colr(10 ,10,10);
	}
	_loopSpin(col);
}

void _loopSpin(Colr col)
{
	for (int iFace = 0; iFace < Led::cFace; ++iFace)
	{
		int offsFace = iOffset;// + iFace;
		int iLedFace = iFace*Led::cTotal;

		float headOuter = frac(revsPerSec * time / 1000.0);
		float headInner = frac(1.5*revsPerSec * time / 1000.0);

		Colr colOuter = Colors[offsFace % cColors];
		for (int i = 0; i < Led::cOuter; ++i)
		{
			int iLed = iLedFace + Led::outer[i];
			byte tail = Tail(headOuter, 1 - (i / (float)(Led::cOuter - 1)), .6, 2/(float)Led::cOuter);
			strip.setColor(iLed, col * tail);
		}

		Colr colInner = Colors[(offsFace + 1) % cColors];
		for (int i = 0; i < Led::cInner; ++i)
		{
			int iLed = iLedFace + Led::inner[i];
			byte tail = Tail(headOuter, 1 - (i / (float)(Led::cInner - 1)), .6, 1/(float)Led::cInner);
		//	setColor(iLed, col);//tail, 0, 0);
			strip.setColor(iLed, col * tail);
		}

		Colr colCenter = Colors[(offsFace + 2) % cColors];
		float intens = (time % 1000) / 1000.0;
		byte v = intens * 255;
		strip.setColor(iLedFace + Led::center, col * v);//0x80, 0x00, 0x40);
	}

	++iOffset;
	strip.show();
}

};
