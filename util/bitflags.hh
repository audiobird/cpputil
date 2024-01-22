#pragma once

#include <cstdint>
#include <type_traits>

template<uint32_t bit_cnt>
class BitFlags {
	static_assert(bit_cnt <= 32);
	using type = std::conditional_t<bit_cnt <= 8, uint8_t, std::conditional_t<bit_cnt <= 16, uint16_t, uint32_t>>;
	static constexpr type mask = static_cast<type>(-1);
	type state;

public:
	type Read() {
		return state;
	}
	bool Read(uint8_t bit) {
		return state & GetMask(bit);
	}
	void Set() {
		state = mask;
	}
	void Set(uint8_t bit) {
		state |= GetMask(bit);
	}
	void Flip() {
		state ^= mask;
	}
	void Flip(uint8_t bit) {
		state ^= GetMask(bit);
	}
	void Clear() {
		state = 0;
	}
	void Clear(uint8_t bit) {
		state &= ~GetMask(bit);
	}

private:
	type GetMask(uint8_t bit) {
		return type{1} << bit;
	}
};
