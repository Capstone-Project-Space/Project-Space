#pragma once

#include "mouse.h"
#include "keyboard.h"

/**
 * @brief A class that inherits from EventReceiver can be registered to recieve events.
 */
class EventReceiver {
public:
	virtual bool onKeyPressed(const Key& key) { return false; }
	virtual bool onKeyReleased(const Key& key) { return false; }
	virtual bool onKeyRepeated(const Key& key) { return false; }

	virtual bool onMouseButtonPressed(const MouseButton& button) { return false; }
	virtual bool onMouseButtonReleased(const MouseButton& button) { return false; }
	virtual bool onMouseDoubleClick(const MouseButton& button) { return false; }
	virtual bool onMouseWheelScroll(const float xoffset, const float yoffset) { return false; }
	virtual bool onMouseMoved(const float x, const float y, const float dx, const float dy) { return false; }

	virtual void onWindowResize(const float oldWidth, const float oldHeight, const float newWidth, const float newHeight) {}
};
