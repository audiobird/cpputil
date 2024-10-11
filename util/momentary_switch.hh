#pragma once

#include <cstdint>

class MomentarySwitch {
	uint8_t _state : 1;
	uint8_t _edge : 1;

public:
	bool edge() const {
		return _edge;
	}
	bool state() const {
		return _state;
	}
	void update(bool state) {
		_edge = state != _state;
		_state = state;
	}
};
