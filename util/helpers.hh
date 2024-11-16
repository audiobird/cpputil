#pragma once

#include <algorithm>
#include <array>
#include <concepts>
#include <cstdint>
#include <cstdlib>
#include <limits>
#include <type_traits>

// checks if value is within range
template<typename T>
inline constexpr bool is_within(const T &val, const T &min, const T &max) {
	return std::clamp(val, min, max) == val;
}

// uses the smallest unsigned integer that can fit the given value
template<uint32_t size>
using smallest_uint_type =
	std::conditional_t<size <= std::numeric_limits<uint8_t>::max(),
					   uint8_t,
					   std::conditional_t<size <= std::numeric_limits<uint16_t>::max(), uint16_t, uint32_t>>;

// uses the smallest signed integer that can fit the given value
template<int32_t min, int32_t max>
class smallest_int {
	static constexpr auto get_max = []() {
		const auto m = min < 0 ? -static_cast<int64_t>(min) : min;
		return std::max<int64_t>(m, max);
	};

	static constexpr auto abs_min_is_max = get_max() != max;
	static constexpr auto max8 =
		abs_min_is_max ? -std::numeric_limits<int8_t>::min() : +std::numeric_limits<int8_t>::max();
	static constexpr auto max16 =
		abs_min_is_max ? -std::numeric_limits<int16_t>::min() : +std::numeric_limits<int16_t>::max();

	static constexpr auto m = get_max();

public:
	using type = std::conditional_t<m <= max8, int8_t, std::conditional_t<m <= max16, int16_t, int32_t>>;
};

template<int32_t min, int32_t max>
using smallest_int_type = smallest_int<min, max>::type;

// returns the size of the largest type
template<typename... Ts>
inline constexpr std::size_t sizeof_largest_type() {
	const auto arr = std::array<std::size_t, sizeof...(Ts)>{sizeof(Ts)...};
	return *std::max_element(arr.begin(), arr.end());
}

// checks if all given types are derived from T
template<typename T, typename... First>
concept are_base_of = (... && std::is_base_of_v<T, First>);

// checks if given type T is same as one of the other provided types
template<typename T, typename... First>
concept is_same_as = (... || std::same_as<T, First>);

// Tests
static_assert(is_same_as<bool, bool, int, float>);
static_assert(is_same_as<double, int, float, int, double>);
static_assert(!is_same_as<bool, int, float, double>);

static_assert(sizeof_largest_type<bool, uint8_t, int16_t, uint32_t, uint64_t>() == sizeof(uint64_t));
static_assert(sizeof_largest_type<bool, uint8_t, int16_t>() == sizeof(int16_t));
static_assert(sizeof_largest_type<bool *, uint8_t>() == sizeof(bool *));

static_assert(is_same_as<smallest_uint_type<1>, uint8_t>);
static_assert(is_same_as<smallest_uint_type<255>, uint8_t>);
static_assert(is_same_as<smallest_uint_type<256>, uint16_t>);
static_assert(is_same_as<smallest_uint_type<65535>, uint16_t>);
static_assert(is_same_as<smallest_uint_type<65536>, uint32_t>);

static_assert(is_same_as<smallest_int_type<-128, 127>, int8_t>);
static_assert(is_same_as<smallest_int_type<0, 127>, int8_t>);
static_assert(is_same_as<smallest_int_type<-129, 127>, int16_t>);
static_assert(is_same_as<smallest_int_type<0, 128>, int16_t>);
static_assert(is_same_as<smallest_int_type<-32768, 32767>, int16_t>);
static_assert(is_same_as<smallest_int_type<-32769, 32767>, int32_t>);
static_assert(is_same_as<smallest_int_type<std::numeric_limits<int32_t>::min(), 0>, int32_t>);

static_assert([] {
	class Base {};
	class D0 : Base {};
	class D1 : Base {};
	class D2 : D1 {};
	class T0 {};
	static_assert(are_base_of<Base, D0, D1, D2>);
	static_assert(!are_base_of<Base, D0, D1, D2, T0>);
	return true;
}());
