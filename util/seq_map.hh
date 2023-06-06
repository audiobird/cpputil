#pragma once
#include <array>
#include <cstddef>

template<typename KeyT, typename ValT, size_t Size>
struct SeqMap {
	std::array<KeyT, Size> keys;
	std::array<ValT, Size> vals;
	size_t idx = 0;

	bool insert(KeyT key, const ValT &val) {
		if (idx >= Size)
			return false;
		keys[idx] = key;
		vals[idx] = val;
		idx++;
		return true;
	}

	bool insert(KeyT key, ValT &&val) {
		if (idx >= Size)
			return false;
		keys[idx] = key;
		vals[idx] = std::move(val);
		idx++;
		return true;
	}

	//TODO: allow move for key on insert?

	ValT *get(KeyT key) {
		for (size_t i = 0; auto k : keys) {
			if (k == key)
				return &vals[i];
			i++;
		}
		return nullptr;
	}

	bool key_exists(KeyT key) {
		for (auto &k : keys) {
			if (k == key)
				return true;
		}
		return false;
	}
};
