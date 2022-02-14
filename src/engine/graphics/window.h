#pragma once

#include <stdio.h>
#include <string>
#include <memory>
#include <chrono>

#include <src/engine/io/mouse.h>
#include <src/engine/io/keyboard.h>
#include <src/engine/io/event.h>

#include <glad/glad.h>
#include <glfw3/glfw3.h>

class Window {
public:
	bool isOpen;
	std::string baseTitle;
	int width, height;
	
	static std::shared_ptr<Window> CreateGLWindow(const std::string& name, int width, int height, std::shared_ptr<Window> share = nullptr);

	Window(const std::string&, int width, int height, std::shared_ptr<Window> share);
	~Window();

	void updateTitle(const std::string& appendage);

private:
	GLFWwindow* window;
};
