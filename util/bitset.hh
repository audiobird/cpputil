#pragma once

#include <bit>
#include <cstdint>
#include <type_traits>

template<uint8_t bit_cnt>
class bitset {
	static_assert(bit_cnt <= 32);
	using type = std::conditional_t<bit_cnt <= 8, uint8_t, std::conditional_t<bit_cnt <= 16, uint16_t, uint32_t>>;
	static constexpr type mask = static_cast<type>(-1);
	type state;
	static constexpr type getMask(const uint8_t pos) {
		return 1u << pos;
	}

public:
	class reference {
		friend class bitset;
		type &val;
		type mask;
		constexpr reference();

	public:
		constexpr reference(const reference &) = default;
		constexpr reference(type &val, const uint8_t pos)
			: val{val}
			, mask{getMask(pos)} {
		}
		constexpr ~reference() {
		}
		constexpr reference &operator=(const auto x) = delete;
		constexpr reference &operator=(const bool x) {
			val = x ? val | mask : val & ~mask;
			return *this;
		}
		constexpr reference &operator=(const reference &x) {
			val = x ? val | mask : val & ~mask;
			return *this;
		}
		constexpr operator bool() const {
			return val & mask;
		}
		constexpr reference &flip() {
			val ^= mask;
			return *this;
		}
		constexpr bool operator~() const {
			return !(val & mask);
		}
	};
	friend class reference;

	bitset() = default;

	constexpr bool operator[](const uint8_t pos) const {
		return reference{state, pos};
	}

	constexpr reference operator[](const uint8_t pos) {
		return reference{state, pos};
	}

	constexpr std::size_t count() const {
		return std::popcount(state);
	}

	constexpr bool all() const {
		return count() == bit_cnt;
	}

	constexpr bool any() const {
		return state;
	}

	constexpr bool none() const {
		return state == 0;
	}

	constexpr bool size() const {
		return bit_cnt;
	}

	constexpr bitset &set() {
		state = mask;
		return *this;
	}
	constexpr bitset &set(const uint8_t pos, const bool value = true) {
		reference{state, pos} = value;
		return *this;
	}
	constexpr bitset &reset() {
		state = 0;
		return *this;
	}
	constexpr bitset &reset(const uint8_t pos) {
		reference{state, pos} = false;
		return *this;
	}
	constexpr bitset &flip() {
		state ^= mask;
		return *this;
	}
	constexpr bitset &flip(const uint8_t pos) {
		reference{state, pos}.flip();
		return *this;
	}
	constexpr auto to_uintx() const {
		return state;
	}
};
