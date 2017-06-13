#include "common.h"
#include "device/audio.h"
#include "device/input.h"
#include "device/leds.h"
#include "mode/audiopulse.h"

// TODO
/*
	-weird pulsing on spinner (see at .5 rps, probably error in polar coords?)
	-polar delta (for AA) for tip leds
	-incorporate mseq ic
	-animation overlay system (sep. color from animation)
	-figure out coil whine amps
	-better time mod system
*/

class Totem : public BtnHandler
{
public:
	Log log;
	Plot plot;

	Input input;
	LedStrip strip;
	Audio audio;

	Mode* mode;

	void OnPressA() {OnPress('A'); mode->OnPressA();}
	void OnPressB() {OnPress('B'); mode->OnPressB();}
	void OnPressC() {OnPress('C'); mode->OnPressC();}
	void OnPressD() {OnPress('D'); mode->OnPressD();}
	void OnPressE() {OnPress('E'); mode->OnPressE();}
	void OnPressF() {OnPress('F'); mode->OnPressF();}

	Totem() :
		log(true), plot(false),
		input(this), audio(input)
	{
		mode = new AudioPulse(strip, input);
	}

	void OnPress(char c)
	{
		log << "Button " << c << " Pressed. ["
			<< input.ReadA() << ", "
			<< input.ReadB() << ", "
			<< input.ReadC() << "].\n";
	}

	// State
	elapsedMillis millis;

	void loop()
	{
			//uint dec = (uint)input.ReadC(1, 10);
			//plot.knobC = dec;
			//	audio.SetDecay(dec);
		Frame frame;
		frame.audio = audio.ReadAudio(&plot); // Block until new audio data
		frame.ms = millis;
		millis = 0;

		input.PollInput();
		frame.knobB = input.ReadB();
		frame.knobC = input.ReadC();

		// TODO: move to modes?
		strip.SetBrightness(input.ReadA(.2, .85)); // TODO make anims more sparing with simultaneous lit leds

		mode->Update(frame);

		//spin();
		//displayAudio();
		//breathe();
	}

	void spin()
	{
		const float colPerSec = .05;
		const uint loopMod = 30000;
		if (millis > loopMod)
			millis -= loopMod;
		float frCol = millis * .001 * colPerSec;
		frCol = frac(frCol);
		Colr colr = Colr::Hue(frCol);
			//Colr(1,1,1);
		Colr colCenter = Colr::Hue(frac(frCol+5));
		strip.SpinStrip(colr, colCenter, millis, 1.5);
		delay(16);
	}

	void debugXYZ()
	{
		float x = dmap(input.ReadA(), 0, 1, -120, 120);
		float y = dmap(input.ReadB(), 0, 1, 0, 240);
		float z = dmap(input.ReadC(), 0, 1, -120, 120);
		strip.GlobalAxis(5, x, y, z, 23, Colr::Red);
	}
};
