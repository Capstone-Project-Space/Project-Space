#pragma once
#include <array>
#include <stdint.h>

#include <glfw3/glfw3.h>

using Key = uint32_t;

struct Keyboard {
	static std::array<bool, GLFW_KEY_LAST + 1> KeyStates;
	static std::array<uint64_t, GLFW_KEY_LAST + 1> TimeStamp;

	static inline bool IsKeyDown(const Key key) {
		return Keyboard::KeyStates[key];
	}

	static inline bool IsKeyUp(const Key key) {
		return !Keyboard::IsKeyDown(key);
	}

	static inline bool IsCtrlDown() {
		return Keyboard::IsKeyDown(GLFW_KEY_LEFT_CONTROL) || Keyboard::IsKeyDown(GLFW_KEY_LEFT_CONTROL);
	}

	static inline bool IsShiftDown() {
		return Keyboard::IsKeyDown(GLFW_KEY_LEFT_SHIFT) || Keyboard::IsKeyDown(GLFW_KEY_RIGHT_SHIFT);
	}

	static inline bool IsAltDown() {
		return Keyboard::IsKeyDown(GLFW_KEY_LEFT_ALT) || Keyboard::IsKeyDown(GLFW_KEY_RIGHT_ALT);
	}

};
