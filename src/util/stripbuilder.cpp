#include "lmath_full.h"
#include "stripbuilder.h"
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

	Led InitLedGlobal(Led ledFace, uint iFace, mat4 xfm)
	{
		ledFace.iFace = iFace;
		ledFace.zpt = xfm * vec4(ledFace.ptFace.x, 0, -ledFace.ptFace.y, 1);
		return ledFace;
	}

	const float radii[] = { 0, 27, 53, 83, 110, 132, 156 }; // TODO Compute
	const float polarDeltas[] = { 0, 1 / (float)cInner, 1 / (float)cOuter, 0.0467, 0.0218, 0, 0};

	Led InitLedFace(uint iRing, int iPolar)
	{
		Led led;
		led.iFace = 0;
		led.iRing = iRing;
		led.polarDelta = polarDeltas[iRing] * 2 * PI;
		led.polarFace = iPolar * led.polarDelta + PI/2;
		if (iRing > 2)
			led.polarFace += PI;

		if (iRing == 3 && iPolar == 0)
			led.radFace = 86;
		else
			led.radFace = radii[iRing];

		led.ptFace = vec2(
			cos(led.polarFace)*led.radFace,
			sin(led.polarFace)*led.radFace);

		return led;
	}

	Led* GenerateLeds(ushort cLED)
	{
		uint cLedFace = cInner+cOuter+1;
		Led ledsFace[cLedFace];

		const uint cLedTip = 7;
		Led ledsTip[cLedTip];

		ledsFace[center] = InitLedFace(0, 0);

		for (uint i = 0; i < cInner; ++i)
			ledsFace[inner[i]] = InitLedFace(1, i);
		for (uint i = 0; i < cOuter; ++i)
			ledsFace[outer[i]] = InitLedFace(2, i);

		for (uint i = 0; i < cTipA; ++i)
			ledsTip[tipA[i]] = InitLedFace(3, i - 1);
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
				Led led = ledsFace[iLed];
				leds[iLedHalf] = InitLedGlobal(led, iFace, xfLower);
				leds[iLedUpper + iLedHalf] = InitLedGlobal(led, iFace, xfUpper);
			}

			uint iLedFaceTip = iLedTip + iFace * cLedTip;
			for (uint iLed = 0; iLed < cLedTip; ++iLed)
				leds[iLedFaceTip + iLed] = InitLedGlobal(ledsTip[iLed], iFace, xfUpper);
		}

		return leds;
	}
}

Led* LedStrip::GenerateLeds() { return StripBuilder::GenerateLeds(cLED); }
