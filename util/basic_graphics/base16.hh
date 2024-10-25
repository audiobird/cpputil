#pragma once

#include "../base16colors.hh"
#include "util/basic_graphics/crtp_driver.hh"

namespace Base16
{

template<GraphicsDriver D>
class GraphicsMiddleware : public D {
public:
	using ColorT = Color;
	void set_palette(const Palette &p) {
		palette = &p;
	}

	void draw_pixel(unsigned x, unsigned y, ColorT color) {
		D::draw_pixel(x, y, toRGB(*palette, color));
	}

	void fill(ColorT color) {
		D::fill(toRGB(*palette, color));
	}

protected:
private:
	const Palette *palette = &Theme::espresso;
};

} // namespace Base16
