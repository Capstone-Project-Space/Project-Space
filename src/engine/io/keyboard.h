#pragma once
#include <array>
#include <stdint.h>

#include <glfw3/glfw3.h>

using Key = uint32_t;

struct Keyboard {
	static std::array<bool, GLFW_KEY_LAST + 1> KeyStates;
	static std::array<uint64_t, GLFW_KEY_LAST + 1> TimeStamp;

	inline bool Keyboard::isKeyDown(const Key key) {
		return Keyboard::KeyStates[key];
	}

	inline bool Keyboard::isKeyUp(const Key key) {
		return !Keyboard::isKeyDown(key);
	}

	inline bool Keyboard::isCtrlDown() {
		return Keyboard::isKeyDown({ GLFW_KEY_LEFT_CONTROL }) || Keyboard::isKeyDown({ GLFW_KEY_LEFT_CONTROL });
	}

	inline bool Keyboard::isShiftDown() {
		return Keyboard::isKeyDown({ GLFW_KEY_LEFT_SHIFT }) || Keyboard::isKeyDown({ GLFW_KEY_RIGHT_SHIFT });
	}


};
