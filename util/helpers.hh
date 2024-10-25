#pragma once

#include <algorithm>
#include <array>
#include <cstdint>
#include <cstdlib>
#include <type_traits>

// checks if value is within range
template<typename T>
inline constexpr bool is_within(const T &val, const T &min, const T &max) {
	return std::clamp(val, min, max) == val;
}

// uses the smallest unsigned integer that can fit the given value
template<uint32_t size>
using smallest_uint = std::conditional_t < size < 256,
	  uint8_t, std::conditional_t<size<65536, uint16_t, uint32_t>>;

// returns the size of the largest type
template<typename... Ts>
inline constexpr std::size_t sizeof_largest_type() {
	const auto arr = std::array<std::size_t, sizeof...(Ts)>{sizeof(Ts)...};
	return *std::max_element(arr.begin(), arr.end());
}

// checks if all given types are derived from T
template<typename T, typename First, typename... Rest>
inline constexpr bool are_base_of() {
	if constexpr (sizeof...(Rest)) {
		return std::is_base_of<T, First>() && are_base_of<T, Rest...>();
	} else {
		return std::is_base_of<T, First>();
	}
}

// checks if given type T is same as one of the other provided types
template<typename T, typename First, typename... Rest>
inline constexpr bool is_same_as() {
	if constexpr (sizeof...(Rest)) {
		return std::is_same<T, First>() || is_same_as<T, Rest...>();
	} else {
		return std::is_same<T, First>();
	}
}

// Tests
static_assert(is_same_as<bool, bool, int, float>());
static_assert(is_same_as<double, int, float, int, double>());
static_assert(!is_same_as<bool, int, float, double>());

static_assert(sizeof_largest_type<bool, uint8_t, int16_t, uint32_t, uint64_t>() == sizeof(uint64_t));
static_assert(sizeof_largest_type<bool, uint8_t, int16_t>() == sizeof(int16_t));
static_assert(sizeof_largest_type<bool *, uint8_t>() == sizeof(bool *));

static_assert(is_same_as<smallest_uint<1>, uint8_t>());
static_assert(is_same_as<smallest_uint<255>, uint8_t>());
static_assert(is_same_as<smallest_uint<256>, uint16_t>());
static_assert(is_same_as<smallest_uint<65535>, uint16_t>());
static_assert(is_same_as<smallest_uint<65536>, uint32_t>());

static_assert([] {
	class Base {};
	class D0 : Base {};
	class D1 : Base {};
	class D2 : D1 {};
	class T0 {};
	static_assert(are_base_of<Base, D0, D1, D2>());
	static_assert(!are_base_of<Base, D0, D1, D2, T0>());
	return true;
}());
