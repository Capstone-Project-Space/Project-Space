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
	/**
	 * @brief Create a Window.
	 * 
	 * @param title The title which appears at the top of a window.
	 * @param width The width in pixels of the window.
	 * @param height The height in pixels of the window.
	 * @param share The window to share resources with null if none.
	 * @return A std::shared_ptr to a Window.
	 */
	static std::shared_ptr<Window> CreateGLWindow(const std::string& title, int width, int height, std::shared_ptr<Window> share = nullptr);

	~Window();

	/**
	 * @brief Append something to the end of the windows title.
	 * 
	 * @param appendage The string to append to the end of the title.
	 */
	void updateTitle(const std::string& appendage);
	
	/**
	 * @brief Flush the rendering context and push the window's pixel buffer to the monitor.
	 */
	void flush();

	/**
	 * @brief Tell the window that it should close so the program can close cleanly.
	 */
	inline void close() {
		glfwSetWindowShouldClose(this->window, 1);
		this->data.isOpen = false;
	}
	
	inline const WindowData& getData() const { return data; }

private:
	GLFWwindow* window;
	WindowData data;
	Window(const std::string&, int width, int height, std::shared_ptr<Window> share);
};
