#include "mouse.h"
#include "keyboard.h"

std::map<Key, bool> Keyboard::KeyStates;
std::map<Key, uint64_t> Keyboard::TimeStamp;

std::array<bool, GLFW_MOUSE_BUTTON_LAST + 1> Mouse::ButtonStates;
std::array<uint64_t, GLFW_MOUSE_BUTTON_LAST + 1> Mouse::TimeStamp;
double Mouse::x = 0.0, Mouse::y = 0.0;
