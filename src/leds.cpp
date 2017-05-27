#include <glm/gtc/matrix_transform.hpp>

#include "leds.h"

#include "log.h"

//#undef round

const ushort LedStrip::cLED = 195;
const ushort LedStrip::outer[] = { 0, 1, 2, 3, 4, 5, 6, 7, 15, 14 };
const ushort LedStrip::cOuter = 10;
const ushort LedStrip::inner[] = { 13, 11, 10, 9, 8 };
const ushort LedStrip::cInner = 5;
const ushort LedStrip::center = 12;
const ushort LedStrip::cFace = 10;
const ushort LedStrip::cTotal = 16;

void InitLed(Led& led, float frPolar, float rad)
{
	float polar = frPolar * 2 * PI + PI/2;
	led.iFace = 0;
	led.polarFace = polar;
	led.radFace = rad;

	led.ptFace = vec2(
		cos(polar)*rad,
		sin(polar)*rad);
}

constexpr float rads(float degs)
{
	return degs * PI / 180;
}

constexpr float dihedral = rads(116.5650512);
constexpr float radOuter = 53;
constexpr float radInner = 27;

mat4 rotate_axis(vec3 pt, vec3 dir, float angle)
{
	return
		glm::translate(mat4(), pt) *
		glm::rotate(mat4(), angle, dir) *
		glm::translate(mat4(), -pt);
}

vec3 mul(mat4 m, vec3 v)
{
	vec4 vRes = m * vec4(v, 1);
	return vec3(vRes.x, vRes.y, vRes.z) / vRes.w;
}

mat4 XfmFace(uint face)
{
	vec3 zptUpper = vec3(0, 0, -56);//-adOuter);
	vec3 zptLowerLeft = mul(glm::rotate(mat4(), rads(144.f), vec3(0, 1, 0)), zptUpper);
	vec3 zptUpperRight = mul(glm::rotate(mat4(), rads(-72.f), vec3(0, 1, 0)), zptUpper);

	mat4 mat0 =
		rotate_axis(zptLowerLeft, vec3(1,0,0), dihedral) *
		translate(mat4(), vec3(0, -4.5, 0));

	if (face < 5)
	{
		return glm::rotate(mat4(), rads(-72.f) * face, vec3(0, 1, 0)) * mat0;
	}
	else
	{
		return
			glm::rotate(mat4(),
				rads(72.f) * (face - 5),
				vec3(0, 1, 0)) *
			mat0 *
			rotate_axis(
				zptUpper,
				normalize(zptUpperRight - zptUpper),
				dihedral) *
			glm::rotate(mat4(),
				rads(-72.f),
				vec3(0, 1, 0)) *
			glm::rotate(mat4(),
				rads(180),
				vec3(0, 0, -1));
	}
}

Led ComputeLedGlobal(Led led, uint iFace, mat4 xfm)
{
	led.iFace = iFace;
	led.zpt = xfm * vec4(led.ptFace.x, 0, -led.ptFace.y, 1);
	return led;
}

Led* LedStrip::GenerateLeds()
{
	Led ledsFace[cTotal];
	InitLed(ledsFace[center], 0, 0);

	for (uint i = 0; i < cOuter; ++i)
	{
		uint iLed = outer[i];
		uint iPolar = (i + cOuter - 4) % cOuter;
		float polar = iPolar / (float)cOuter;
		InitLed(ledsFace[iLed], polar, radOuter);
	}

	for (uint i = 0; i < cInner; ++i)
	{
		uint iLed = inner[i];
		uint iPolar = (i + cInner - 2) % cInner;
		float polar = iPolar / (float)cInner;
		InitLed(ledsFace[iLed], polar, radInner);
	}

	// Copy into full buffer
	Led* leds = new Led[cLED];
	for (uint i = 0; i < cFace; ++i)
	{
		uint iFace = i * cTotal;
		mat4 xfmFace = XfmFace(i);
		for (uint iLed = 0; iLed < cTotal; ++iLed)
		{
			leds[iFace + iLed] = ComputeLedGlobal(ledsFace[iLed], i, xfmFace);

			if (i == 0 && iLed == 4)
			{
				Led led = leds[iFace + iLed];
				//ptLog = led.ptFace;
				//zptLog = led.zpt;
			}
		}
	}

	return leds;
}

LedStrip::LedStrip(int pin) :
	strip(cLED, pin, NEO_GRB + NEO_KHZ800),
	leds(GenerateLeds())
{
	pinMode(pin, OUTPUT);
	strip.begin();
	strip.show();
}

void LedStrip::setColorNoGamma(ushort px, Colr colr)
{
	strip.setPixelColor(px, colr.r, colr.g, colr.b);
}

void LedStrip::setColor(ushort px, Colr colr)
{
	setColorNoGamma(px, colr.gammaCorrect());
}

void LedStrip::show() { strip.show(); }

void LedStrip::setStripColor(Colr colr)
{
	colr = colr.gammaCorrect();
	for (ushort i = 0; i < cLED; ++i)
		setColorNoGamma(i, colr);
	strip.show();
}

void LedStrip::rainbowFace(uint face)
{
	uint iFace = face * cTotal;
	for (uint i = 0; i < cTotal; ++i)
	{
		Led led = leds[iFace+i];
		float polar = led.polarFace;
		setColor(iFace+i, Colr::Hue(polar/(2*PI))*.4);
	}
}

void LedStrip::rainbowFaceLinear(uint face, float offs, float polarOffs)
{
	uint iFace = face * cTotal;
	for (uint i = 0; i < cTotal; ++i)
	{
		Led led = leds[iFace+i];
		float polar = led.polarFace + polarOffs*2*PI;
		float x = cos(polar) * led.radFace;

		float val = frac(dmap(x, -52, 52, 0, 1) + offs);
		setColor(iFace+i, Colr::Hue(val)*.5);
	}
}

ubyte FrForAxis(float val, float offs, float width)
{
	float delta = abs(val - offs);
	float fr = 1 - saturate(delta/width);
	return fr*0x80;
}

void print(vec3 v)
{
	Log::log->print("<");
	Log::log->print(v.x);
	Log::log->print(", ");
	Log::log->print(v.y);
	Log::log->print(", ");
	Log::log->print(v.z);
	Log::log->print(">, ");
}

void LedStrip::globalAxis(uint _face, float x, float y, float z, float width)
{
	for (uint face = 0; face < 10; ++face)
	{
		uint iFace = face * cTotal;
		for (uint i = 0; i < cTotal; ++i)
		{
			Led led = leds[iFace+i];
			Colr colr = Colr(
				//dmap(led.zpt.x, 0, 140, 0, 255),0,0);
				FrForAxis(led.zpt.x, x, width),
				FrForAxis(led.zpt.y, y, width),
				FrForAxis(led.zpt.z, z, width));

			setColor(iFace + i, colr);
		}
	}
}
