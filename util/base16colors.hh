#pragma once

#include "colors.hh"
#include <array>

namespace Base16
{
enum class Color {
	BLACK,
	GREY5,
	GREY4,
	GREY3,
	GREY2,
	GREY1,
	GREY0,
	WHITE,
	RED,
	ORANGE,
	YELLOW,
	GREEN,
	CYAN,
	BLUE,
	PURPLE,
	PINK,
};

inline constexpr Color index(const unsigned idx) {
	return static_cast<Color>(idx);
}

using Palette = std::array<::Color, 16>;

inline constexpr const ::Color &toRGB(const Palette &palette, const Color color) {
	return palette[static_cast<unsigned>(color)];
}

namespace Theme
{
inline constexpr Palette espresso{
	::Color{0x2d, 0x2d, 0x2d}, // black
	::Color{0x39, 0x39, 0x39}, // grey5
	::Color{0x51, 0x51, 0x51}, // grey4
	::Color{0x77, 0x77, 0x77}, // grey3
	::Color{0xb4, 0xb7, 0xb4}, // grey2
	::Color{0xcc, 0xcc, 0xcc}, // grey1
	::Color{0xe0, 0xe0, 0xe0}, // grey0
	::Color{0xff, 0xff, 0xff}, // white
	::Color{0xd2, 0x52, 0x52}, // red
	::Color{0xf9, 0xa9, 0x59}, // orange
	::Color{0xff, 0xc6, 0x6d}, // yellow
	::Color{0xa5, 0xc2, 0x61}, // green
	::Color{0xbe, 0xd6, 0xff}, // cyan
	::Color{0x6c, 0x99, 0xbb}, // blue
	::Color{0xd1, 0x97, 0xd9}, // purple
	::Color{0xf9, 0x73, 0x94}, // pink
};
}
} // namespace Base16
