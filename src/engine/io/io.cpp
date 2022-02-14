#include "mouse.h"
#include "keyboard.h"

std::array<bool, GLFW_KEY_LAST + 1> Keyboard::KeyStates;
std::array<uint64_t, GLFW_KEY_LAST + 1> Keyboard::TimeStamp;

std::array<bool, GLFW_MOUSE_BUTTON_LAST + 1> Mouse::ButtonStates;
double Mouse::x = 0.0, Mouse::y = 0.0;
