#include "window.h"

#include <src/engine/io/event.h>

#include <stb_image.h>

std::shared_ptr<Window> Window::CreateGLWindow(const std::string& name, int width, int height, std::shared_ptr<Window> share) {
	std::shared_ptr<Window> window = std::shared_ptr<Window>(new Window(name, width, height, share));

	GLFWimage image;
	image.pixels = stbi_load("./resources/icon_bg.png", &image.width, &image.height, NULL, 4);
	glfwSetWindowIcon(window->window, 1, &image);
	stbi_image_free(image.pixels);

	glfwSetWindowUserPointer(window->window, &window->data);

	glfwSetWindowCloseCallback(window->window,
		[](GLFWwindow* glfwWindow) {
			WindowData* data = (WindowData*)glfwGetWindowUserPointer(glfwWindow);
			data->isOpen = false;
		}
	);

	glfwSetKeyCallback(window->window,
		[](GLFWwindow* window, int key, int scancode, int action, int mods) {
			Key keyPair = { (uint32_t) key, (uint32_t) scancode };
			switch (action) {
			case GLFW_PRESS:
				Keyboard::KeyStates[keyPair] = true;
				Keyboard::TimeStamp[keyPair] = std::chrono::high_resolution_clock::now().time_since_epoch().count();
				Events::DispatchEvent(Event<Key>{ EventType::KEY_PRESSED, keyPair });
				break;
			case GLFW_RELEASE:
				Keyboard::KeyStates[keyPair] = false;
				Keyboard::TimeStamp[keyPair] = 0;
				Events::DispatchEvent(Event<Key>{ EventType::KEY_RELEASED, keyPair });
				break;
			case GLFW_REPEAT:
				Keyboard::TimeStamp[keyPair] = std::chrono::high_resolution_clock::now().time_since_epoch().count();
				Events::DispatchEvent(Event<Key>{ EventType::KEY_REPEATED, keyPair });
				break;
			}
		}
	);

	glfwSetCharCallback(window->window,
		[](GLFWwindow* windoow, unsigned int codepoint) {
			char chars[5];
			memcpy_s(chars, 4, &codepoint, 4);
			chars[4] = 0;
			Events::DispatchEvent(Event<char*>{EventType::KEY_CHARACTER, chars});
		}
	);

	glfwSetMouseButtonCallback(window->window,
		[](GLFWwindow* window, int button, int action, int mods) {
			switch (action) {
			case GLFW_PRESS:
			{
				uint64_t now = std::chrono::high_resolution_clock::now().time_since_epoch().count();
				Mouse::ButtonStates[button] = true;
				if (now - Mouse::TimeStamp[button] <= 250000000) {
					Events::DispatchEvent(Event<MouseButton>{ EventType::MOUSE_DOUBLE_CLICK, (uint32_t)button });
				} else {
					Events::DispatchEvent(Event<MouseButton>{ EventType::MOUSE_BUTTON_PRESSED, (uint32_t) button });
				}
				Mouse::TimeStamp[button] = now;
				break;
			}
			case GLFW_RELEASE:
				Mouse::ButtonStates[button] = false;
				Events::DispatchEvent(Event<MouseButton>{ EventType::MOUSE_BUTTON_RELEASED, (uint32_t) button });
				break;
			}
		}
	);

	glfwSetScrollCallback(window->window,
		[](GLFWwindow* window, double xoffset, double yoffset) {
			float vals[2] = { xoffset, yoffset };
			Events::DispatchEvent(Event<float*>{EventType::MOUSE_WHEEL_SCROLL, vals});
		}
	);

	glfwSetCursorPosCallback(window->window,
		[](GLFWwindow* window, double xpos, double ypos) {
			WindowData* data = (WindowData*)glfwGetWindowUserPointer(window);
			xpos -= data->size.x / 2.f;
			ypos = data->size.y / 2.f - ypos;
			float vals[4] = { xpos, ypos, xpos - Mouse::x, ypos - Mouse::y };
			Mouse::x = xpos;
			Mouse::y = ypos;
			Events::DispatchEvent(Event<float*>{ EventType::MOUSE_POSITION, vals });
		}
	);

	glfwSetWindowSizeCallback(window->window,
		[](GLFWwindow* glfwWindow, int width, int height) {
			WindowData* data = (WindowData*) glfwGetWindowUserPointer(glfwWindow);
			float vals[4] = { data->size.x, data->size.y, width, height };
			data->size.x = width;
			data->size.y = height;
			data->scale = data->size / data->initialSize;
			data->orthographicCamera = {width / -2.f, width / 2.f, height / -2.f, height / 2.f};
			glViewport(0, 0, width, height);
			Events::DispatchEvent(Event<float*>{ EventType::WINDOW_RESIZE, vals });
		}
	);

	return window;
}

void Window::updateTitle(const std::string& appendage) {
	glfwSetWindowTitle(this->window, (this->data.title + appendage).c_str());
}

void Window::flush() {
	glfwSwapBuffers(window);
	glFlush();
}

Window::Window(const std::string& title, int width, int height, std::shared_ptr<Window> share) 
	: data({ title, { width, height }}) {
	
	glfwSetErrorCallback([](int code, const char* msg) {
		throw msg;
	});
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	this->window = glfwCreateWindow(width, height, title.c_str(), NULL, share ? share->window : NULL);
	if (this->window == NULL) {
		fprintf(stderr, "Failed to create GLFW window\n");
		glfwTerminate();
		exit(1);
	} 
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		fprintf(stderr, "Failed to initialize GLAD\n");
		exit(1);
	}
	glfwSwapInterval(0);
	this->data.isOpen = true;
}

Window::~Window() {
	glfwDestroyWindow(this->window);
}
