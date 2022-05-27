#pragma once
#include <array>
#include <map>
#include <stdint.h>

#include <glfw3/glfw3.h>

struct Key {
	uint32_t keyCode;
	uint32_t scanCode;
	Key() : keyCode(0), scanCode(0) {}
	Key(uint32_t keyCode) : keyCode(keyCode), scanCode(0) {}
	Key(uint32_t keyCode, uint32_t scanCode) : keyCode(keyCode), scanCode(scanCode) {}
	bool operator<(const Key& key) const {
		if (scanCode == 0 || key.scanCode == 0) return this->keyCode < key.keyCode;
		return ((uint64_t)this->keyCode << 32 | (uint64_t)this->scanCode) < ((uint64_t)key.keyCode << 32 | (uint64_t)key.scanCode);
	}
	bool operator==(const Key& key) const {
		if (scanCode == 0 || key.scanCode == 0) return this->keyCode == key.keyCode;
		return this->scanCode == key.scanCode;
	}
};

struct Keyboard {
	static std::map<Key, bool> KeyStates;
	static std::map<Key, uint64_t> TimeStamp;

	static inline bool IsKeyDown(const Key key) {
		return Keyboard::KeyStates[key];
	}

	static inline bool IsKeyUp(const Key key) {
		return !Keyboard::IsKeyDown(key);
	}

	static inline bool IsCtrlDown() {
		return Keyboard::IsKeyDown({ GLFW_KEY_LEFT_CONTROL, 0 }) || Keyboard::IsKeyDown({ GLFW_KEY_RIGHT_CONTROL, 0 });
	}

	static inline bool IsShiftDown() {
		return Keyboard::IsKeyDown({ GLFW_KEY_LEFT_SHIFT, 0 }) || Keyboard::IsKeyDown({ GLFW_KEY_RIGHT_SHIFT, 0 });
	}

	static inline bool IsAltDown() {
		return Keyboard::IsKeyDown({ GLFW_KEY_LEFT_ALT, 0 }) || Keyboard::IsKeyDown({ GLFW_KEY_RIGHT_ALT, 0 });
	}

};
