#pragma once

#include "mouse.h"
#include "keyboard.h"

class EventReceiver {
public:
	virtual bool onKeyPressed(const Key& key) = 0;
	virtual bool onKeyReleased(const Key& key) = 0;
	virtual bool onKeyRepeated(const Key& key) = 0;

	virtual bool onMouseButtonPressed(const MouseButton& button) = 0;
	virtual bool onMouseButtonReleased(const MouseButton& button) = 0;
	virtual bool onMouseMoved(const float x, const float y, const float dx, const float dy) = 0;
};
