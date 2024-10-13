#pragma once

#include "raw_fonts.hh"

namespace Font
{
struct Font {
	unsigned width;
	unsigned height;
	unsigned num_chars;
	constexpr Font(const unsigned char *raw, unsigned w, unsigned h, unsigned num_chars)
		: raw{raw}
		, width{w}
		, height{h}
		, num_chars{num_chars} {
	}
	constexpr const unsigned char &operator[](unsigned idx) const {
		return raw[idx];
	}

private:
	const unsigned char *raw;
};

inline constexpr auto TI5x7 = Font{Fonts::Raw::TI5x7raw.data(), 5u, 7u, 95u};
} // namespace Font
