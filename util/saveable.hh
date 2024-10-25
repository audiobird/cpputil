#pragma once

#include <cstdint>
#include <functional>
#include <type_traits>

class Saveable {
	using write_func = std::function<bool(uint8_t)>;
	using read_func = std::function<bool(uint8_t &, uint32_t)>;

public:
	using _m_inner_type = Saveable;

	template<typename T>
	requires(!std::is_same_v<T, Saveable>)
	bool serialize(this T &&self, write_func write_byte) {
		auto t = reinterpret_cast<const uint8_t *>(&self);
		for (auto i = 0u; i < self._m_size_of_self(); ++i) {
			if (!write_byte(*(t + i)))
				return false;
		}
		return true;
	}

	template<typename T>
	requires(!std::is_same_v<T, Saveable>)
	bool verify(this const T &self, read_func read_byte) {
		auto offset = 0u;
		uint8_t byte;
		auto w = reinterpret_cast<const uint8_t *>(&self);

		for (auto i = 0u; i < self._m_size_of_self(); ++i) {
			if (!read_byte(byte, offset++) || byte != *(w + i))
				return false;
		}
		return true;
	}

	template<typename T>
	requires(!std::is_same_v<T, Saveable>)
	bool deserialize(this T &&self, const uint32_t found_version, read_func read_byte) {

		if (found_version > self.get_version())
			return false;

		auto offset = 0u;
		auto s = reinterpret_cast<uint8_t *>(&self);
		for (auto i = 0u; i < self._m_get_found_version_size(found_version); ++i) {
			if (!read_byte(*(s + i), offset + i))
				return false;
		}

		self._m_make_compatible(found_version);

		return true;
	}

	template<typename T>
	requires(!std::is_same_v<T, Saveable>)
	constexpr uint32_t get_version(this T &&self) {
		if constexpr (std::same_as<typename std::remove_reference_t<decltype(self)>::_m_inner_type, Saveable>)
			return 0u;
		else
			return static_cast<const std::remove_reference_t<decltype(self)>::_m_inner_type &>(self).get_version() + 1u;
	}

private:
	constexpr uint32_t _m_get_found_version_size(this auto &&self, uint32_t found_version) {
		if (found_version == self.get_version())
			return self._m_size_of_self();

		return static_cast<std::remove_reference_t<decltype(self)>::_m_inner_type &>(self)._m_get_found_version_size(
			found_version);
	}
	constexpr uint32_t _m_size_of_self(this auto &&self) {
		return sizeof(decltype(self));
	}

	void _m_make_compatible(this auto &&self, uint32_t found_version) {
		if (found_version == self.get_version())
			return;

		static_cast<std::remove_reference_t<decltype(self)>::_m_inner_type &>(self)._m_make_compatible(found_version);

		self.make_compatible();
	}

	void make_compatible() {
	}
};

template<typename T>
concept IsSaveable = std::derived_from<T, Saveable>;

template<IsSaveable T>
struct Extension : T {
	using _m_inner_type = T;
};
