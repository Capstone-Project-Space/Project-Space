#include <glfw3/glfw3.h>

struct Key {
	int keycode;
};

struct Keyboard {
	bool keyStates[GLFW_KEY_LAST + 1];
	int timeStamp[GLFW_KEY_LAST + 1];

	inline bool isKeyDown

};