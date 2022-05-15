#pragma once
#include <array>
#include <stdint.h>

#include <glfw3/glfw3.h>

using MouseButton = uint32_t;

struct Mouse {
	static std::array<bool, GLFW_MOUSE_BUTTON_LAST + 1> ButtonStates;
	static std::array<uint64_t, GLFW_MOUSE_BUTTON_LAST + 1> TimeStamp;
	static double x, y;
	
	inline static bool IsButtonDown(MouseButton button) { return Mouse::ButtonStates[button]; }
};
