/*
* cgame/rgb.hpp
*
* Copyright (c) 2016
* Marcin Koziuk <marcin.koziuk@gmail.com>
*/

#ifndef GRAVITY_CGAME_RGB_HPP
#define GRAVITY_CGAME_RGB_HPP

#include <stdint.h>

#include <glm/vec2.hpp>

#include <SDL2/SDL_pixels.h>
#include <nanovg.h>

namespace Gravity {

// Temp fix for windows.h ;(
#ifdef RGB
#undef RGB
#endif

struct RGB {
	uint8_t r, g, b;

	void CopyHex(uint32_t hex)
	{
		r = (hex) & 0xFF;
		g = (hex >> 8) & 0xFF;
		b = (hex >> 16) & 0xFF;
	}

	uint32_t ToHex()
	{
		return ((r & 0xff) << 16) + ((g & 0xff) << 8) + ((b) << 16);
	}

	RGB()
	{ r = 0, g = 0, b = 0; }

	RGB(uint32_t hex)
	{ CopyHex(hex); }

	RGB(uint8_t r, uint8_t g, uint8_t b)
		: r(r), g(g), b(b)
	{}
};

struct RGBA : public RGB {
	uint8_t a;

	void SetAlpha(double val)
	{ a = uint8_t(val * 255.); }

	double GetAlpha()
	{ return (double)a / 255.; }

	RGBA()
		: a(255)
	{}

	RGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
		: RGB(r, g, b), a(a)
	{}

	RGBA(uint8_t r, uint8_t g, uint8_t b, double a)
		: RGB(r, g, b), a((uint8_t) (a * 255.))
	{}

	RGBA(RGB rgb, uint8_t a)
		: RGB(rgb), a(a)
	{}

	RGBA(RGB rgb, double a)
		: RGB(rgb), a((uint8_t)(a * 255.))
	{}

	SDL_Color ToSdlColor()
	{ return { r, g, b, a }; }

	NVGcolor ToNVGColor()
	{ return{ r / 255.f, g / 255.f, b / 255.f, a / 255.f }; }
};

} // namespace Gravity

#endif /* GRAVITY_CGAME_RGB_HPP */