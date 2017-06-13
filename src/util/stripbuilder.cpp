#include "lmath_full.h"
#include "leds.h"

namespace StripBuilder
{
	const ushort outer[] = { 4, 5, 6, 7, 15, 14, 0, 1, 2, 3 };
	const ushort cOuter = 10;

	const ushort inner[] = { 10, 9, 8, 13, 11 };
	const ushort cInner = 5;

	const ushort center = 12;

	const ushort tipA[] = { 3, 2, 0 };
	const ushort cTipA = 3;
	const ushort tipB[] = { 4, 1 };
	const ushort cTipB = 2;
	const ushort tipC = 5;
	const ushort tipD = 6;

	const vec3 zptUpper = vec3(0, 0, -56);
	const vec3 zptLowerLeft = mul(glm::rotate(mat4(), rads(144.f), vec3(0, 1, 0)), zptUpper);
	const vec3 zptUpperRight = mul(glm::rotate(mat4(), rads(-72.f), vec3(0, 1, 0)), zptUpper);
	const mat4 xfFrontLower =
		rotate_axis(zptLowerLeft, vec3(1,0,0), dihedral) *
		translate(mat4(), vec3(0, -4.5, 0));

	const mat4 xfFrontUpperRight =
		xfFrontLower *
		rotate_axis(
			zptUpper,
			normalize(zptUpperRight - zptUpper),
			dihedral) *
		rotate(mat4(),
			rads(-72.f),
			vec3(0, 1, 0)) *
		rotate(mat4(),
			rads(180),
			vec3(0, 0, -1));

	mat4 XfFaceLower(uint iFace)
	{
		return
			rotate(mat4(),
				rads(-72.f) * iFace,
				vec3(0, 1, 0)) *
			xfFrontLower;
	}

	mat4 XfFaceUpper(uint iFace)
	{
		return
			rotate(mat4(),
				rads(72.f) * iFace,
				vec3(0, 1, 0)) *
			xfFrontUpperRight;
	}

	Led InitLedGlobal(Led::Face ledFace, uint iFace, mat4 xfm)
	{
		Led led;
		led.face = ledFace;
		led.iFace = iFace;
		led.zpt = xfm * vec4(ledFace.pt.x, 0, -ledFace.pt.y, 1);
		return led;
	}

	const float radii[] = { 0, 27, 53, 83, 110, 132, 156 }; // TODO Compute
	const float polarDeltas[] = { 0, 1 / (float)cInner, 1 / (float)cOuter, 0.0467, 0.0218, 0, 0};

	Led::Face InitLedFace(uint iRing, int iPolar)
	{
		Led::Face led;
		led.iRing = iRing;
		led.frPolarDelta = polarDeltas[iRing];
		led.polarDelta = led.frPolarDelta * (2*PI);

		led.polar = iPolar * led.polarDelta + PI/2;
		if (iRing > 2)
			led.polar += PI;

		led.frPolar = led.polar / (2*PI);

		if (iRing == 3 && iPolar == 0)
			led.rad = 86;
		else
			led.rad = radii[iRing];

		led.pt = vec2(
			cos(led.polar)*led.rad,
			sin(led.polar)*led.rad);

		return led;
	}

	Led* GenerateLeds(ushort cLED)
	{
		uint cLedFace = cInner+cOuter+1;
		Led::Face ledsFace[cLedFace];

		const uint cLedTip = 7;
		Led::Face ledsTip[cLedTip];

		ledsFace[center] = InitLedFace(0, 0);

		for (uint i = 0; i < cInner; ++i)
			ledsFace[inner[i]] = InitLedFace(1, i);
		for (uint i = 0; i < cOuter; ++i)
			ledsFace[outer[i]] = InitLedFace(2, i);

		for (uint i = 0; i < cTipA; ++i)
			ledsTip[tipA[i]] = InitLedFace(3, i-1);
		for (uint i = 0; i < cTipB; ++i)
			ledsTip[tipB[i]] = InitLedFace(4, i*2-1);

		ledsTip[tipC] = InitLedFace(5, 0);
		ledsTip[tipD] = InitLedFace(6, 0);

		// Copy into full buffer
		Led* leds = new Led[cLED];
		const uint iLedUpper = 5 * cLedFace;
		const uint iLedTip = 2 * iLedUpper;

		for (uint iFace = 0; iFace < 5; ++iFace)
		{
			uint iLedFace = iFace * cLedFace;
			mat4 xfLower = XfFaceLower(iFace);
			mat4 xfUpper = XfFaceUpper(iFace);
			for (uint iLed = 0; iLed < cLedFace; ++iLed)
			{
				uint iLedHalf = iLedFace + iLed;
				Led::Face face = ledsFace[iLed];
				leds[iLedHalf] = InitLedGlobal(face, iFace, xfLower);
				leds[iLedUpper + iLedHalf] = InitLedGlobal(face, iFace, xfUpper);
			}

			uint iLedFaceTip = iLedTip + iFace * cLedTip;
			for (uint iLed = 0; iLed < cLedTip; ++iLed)
				leds[iLedFaceTip + iLed] = InitLedGlobal(ledsTip[iLed], iFace, xfUpper);
		}

		return leds;
	}
}

Led* LedStrip::GenerateLeds() { return StripBuilder::GenerateLeds(cLED); }
