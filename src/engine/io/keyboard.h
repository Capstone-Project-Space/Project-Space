#include <glfw3/glfw3.h>

struct Key {
	int keycode;
};

struct Keyboard {
	bool keyStates[GLFW_KEY_LAST+1];
};