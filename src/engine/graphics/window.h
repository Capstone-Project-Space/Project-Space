#pragma once

#include <stdio.h>
#include <string>
#include <memory>
#include <chrono>

#include <src/engine/io/mouse.h>
#include <src/engine/io/keyboard.h>
#include <src/engine/graphics/window_data.h>

#include <glad/glad.h>
#include <glfw3/glfw3.h>

class Window {
public:
	static std::shared_ptr<Window> CreateGLWindow(const std::string& name, int width, int height, std::shared_ptr<Window> share = nullptr);

	Window(const std::string&, int width, int height, std::shared_ptr<Window> share);
	~Window();

	void updateTitle(const std::string& appendage);
	void flush();

	inline void close() {
		glfwSetWindowShouldClose(this->window, 1);
		this->data.isOpen = false;
	}
	inline const WindowData& getData() const { return data; }

private:
	GLFWwindow* window;
	WindowData data;
};
