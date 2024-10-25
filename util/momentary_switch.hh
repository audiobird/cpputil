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
	bool was_just_pressed() const {
		return _edge && _state;
	}
	bool was_just_released() const {
		return _edge && !_state;
	}
	void update(bool state) {
		_edge = state != _state;
		_state = state;
	}
};
