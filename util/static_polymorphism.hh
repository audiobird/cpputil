#pragma once

#include "helpers.hh"
#include <array>
#include <cstdint>

// creates a data buffer that can hold the largest type passed in
template<typename Base, typename Derived, typename... Derived_>
class StaticPolymorphicContainer {
	std::array<uint8_t, sizeofLargestType<Derived, Derived_...>()> buffer;
	static_assert(are_base_of<Base, Derived, Derived_...>());

public:
	// Instantiates the given type into the static buffer
	// TODO: better name?
	template<typename T, typename... Ts>
	void transform(Ts &&...args) {
		static_assert(is_same_as<T, Derived, Derived_...>());
		static_cast<void>(new (buffer.data()) T{args...});
	}

	auto operator->() {
		return reinterpret_cast<Base *>(buffer.data());
	}
};
