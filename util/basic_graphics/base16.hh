#pragma once

#include "../base16colors.hh"
#include "basic_graphics.hh"

namespace Base16
{

class Graphics : public SimpleGraphics<const Color &> {
public:
	void set_palette(const Palette &p) {
		palette = &p;
	}

protected:
	const Palette *palette = &Theme::espresso;
};
} // namespace Base16
