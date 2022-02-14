#pragma once
#include <src/engine/io/keyboard.h>
#include <src/engine/io/mouse.h>

class UI {
public:
	UI() = default;

	virtual void draw(float delta) = 0;

	virtual void keyPressed(const Key& key) = 0;
	virtual void keyReleased(const Key& key) = 0;

	virtual void mousePressed(const MouseButton& mouse) = 0;
	virtual void mouseReleased(const MouseButton& mouse) = 0;
	virtual void mouseDragged() = 0;

private:

};

class UIElement {
public:
	UIElement() = default;

	virtual void draw(float delta) = 0;


private:

};