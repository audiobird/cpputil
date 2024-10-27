#pragma once

#include "raw_fonts.hh"
#include <cstdint>
#include <type_traits>

namespace Font
{
template<typename T>
concept is_array = std::is_array_v<T>;

struct Font {
	unsigned width;
	unsigned height;
	unsigned offset;
	uint8_t num_chars;

	constexpr Font(auto &arr, unsigned w, unsigned h, uint8_t num_chars)
		: raw{arr.data()}
		, width{w}
		, height{h}
		, offset{static_cast<unsigned>(arr.size() / num_chars)}
		, num_chars{num_chars} {
	}
	constexpr const unsigned char &operator[](unsigned idx) const {
		return raw[idx];
	}

private:
	const unsigned char *raw;
};

inline constexpr auto Font8x16x95 = Font{Fonts::Raw::Font8x16x95, 8u, 16u, 95u};
inline constexpr auto Font6x7x95 = Font{Fonts::Raw::Font6x7x95, 6u, 7u, 95u};
inline constexpr auto Font8x8x95 = Font{Fonts::Raw::Font8x8x95, 8u, 8u, 95u};
inline constexpr auto Font8x8x95xSquare = Font{Fonts::Raw::Font8x8x95xSquare, 8u, 8u, 95u};
} // namespace Font
