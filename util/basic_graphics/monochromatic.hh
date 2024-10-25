#pragma once

#include "util/basic_graphics/crtp_driver.hh"

namespace Monochrome
{

template<GraphicsDriver D>
class GraphicsMiddleware : public D {
public:
	using ColorT = bool;
	void draw_pixel(unsigned x, unsigned y, ColorT color) {
		D::draw_pixel(x, y, color ? Colors::white : Colors::black);
	}

	void fill(ColorT color) {
		D::fill(color ? Colors::white : Colors::black);
	}
};

} // namespace Monochrome
