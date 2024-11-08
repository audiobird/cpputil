#pragma once

#include <cstdint>
#include <functional>
#include <span>
#include <type_traits>

class serializer {
	using write_func = std::function<bool(const std::span<const char>)>;
	using verify_func = write_func;
	using read_func = std::function<bool(const std::span<char>)>;

public:
	using _m_inner_type = serializer;

	template<typename T>
	requires(!std::is_same_v<T, serializer>)
	bool serialize(this const T &self, write_func write_func) {
		return write_func(std::span{reinterpret_cast<const char *>(&self), self._m_size_of_self()});
	}

	template<typename T>
	requires(!std::is_same_v<T, serializer>)
	bool verify(this const T &self, verify_func verify_func) {
		return verify_func(std::span{reinterpret_cast<const char *>(&self), self._m_size_of_self()});
	}

	template<typename T>
	requires(!std::is_same_v<T, serializer>)
	bool deserialize(this T &self, const uint32_t found_version, read_func read_func) {
		if (found_version > self.get_version()) {
			return false;
		}

		if (!read_func(std::span{reinterpret_cast<char *>(&self), self._m_get_found_version_size(found_version)})) {
			return false;
		}

		self._m_make_compatible(found_version);

		return true;
	}

	template<typename T>
	requires(!std::is_same_v<T, serializer>)
	constexpr uint32_t get_version(this T &&self) {
		if constexpr (std::same_as<typename std::remove_reference_t<decltype(self)>::_m_inner_type, serializer>) {
			return 0u;
		} else {
			return static_cast<const std::remove_reference_t<decltype(self)>::_m_inner_type &>(self).get_version() + 1u;
		}
	}

private:
	constexpr uint32_t _m_get_found_version_size(this auto &&self, uint32_t found_version) {
		if (found_version == self.get_version()) {
			return self._m_size_of_self();
		}

		return static_cast<std::remove_reference_t<decltype(self)>::_m_inner_type &>(self)._m_get_found_version_size(
			found_version);
	}

	constexpr uint32_t _m_size_of_self(this auto &&self) {
		return sizeof(decltype(self));
	}

	void _m_make_compatible(this auto &&self, uint32_t found_version) {
		if (found_version == self.get_version()) {
			return;
		}

		static_cast<std::remove_reference_t<decltype(self)>::_m_inner_type &>(self)._m_make_compatible(found_version);

		self.make_compatible();
	}

	void make_compatible() {
	}
};

template<typename T>
concept is_serializer = std::derived_from<T, serializer>;

template<is_serializer T>
struct serializer_extension : T {
	using _m_inner_type = T;
};
