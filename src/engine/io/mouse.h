#pragma once
#include <array>
#include <stdint.h>

#include <glfw3/glfw3.h>

struct MouseButton {
	uint32_t button;
	MouseButton() : button(0) {}
	MouseButton(uint32_t button) : button(button) {}
	bool operator<(const uint32_t& button) const {
		return this->button < button;
	}
	bool operator<(const MouseButton& button) const {
		return this->button < button.button;
	}
	bool operator==(const uint32_t& button) const {
		return this->button == button;
	}
	bool operator==(const MouseButton& button) const {
		return this->button == button.button;
	}
	bool operator!=(const uint32_t& button) const {
		return this->button != button;
	}
	bool operator!=(const MouseButton& button) const {
		return this->button != button.button;
	}
};;

struct Mouse {
	static std::array<bool, GLFW_MOUSE_BUTTON_LAST + 1> ButtonStates;
	static std::array<uint64_t, GLFW_MOUSE_BUTTON_LAST + 1> TimeStamp;
	static double x, y;
	
	inline static bool IsButtonDown(const MouseButton& button) { return Mouse::ButtonStates[button.button]; }
};
