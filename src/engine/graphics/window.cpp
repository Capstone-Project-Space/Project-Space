#include "window.h"

std::shared_ptr<Window> Window::CreateGLWindow(const std::string& name, int width, int height, std::shared_ptr<Window> share) {
	std::shared_ptr<Window> window = std::shared_ptr<Window>(new Window(name, width, height, share));

	glfwSetWindowUserPointer(window->window, window.get());

	glfwSetWindowCloseCallback(window->window,
		[](GLFWwindow* glfwWindow) {
			Window* window = (Window*)glfwGetWindowUserPointer(glfwWindow);
			window->isOpen = false;
		}
	);

	glfwSetKeyCallback(window->window,
		[](GLFWwindow* window, int key, int scancode, int action, int mods) {
			switch (action) {
			case GLFW_PRESS:
				Keyboard::KeyStates[key] = true;
				Keyboard::TimeStamp[key] = std::chrono::high_resolution_clock::now().time_since_epoch().count();
				Events::DispatchEvent(Event<Key>{ EventType::KEY_PRESSED, (uint32_t) key });
				break;
			case GLFW_RELEASE:
				Keyboard::KeyStates[key] = false;
				Keyboard::TimeStamp[key] = 0;
				Events::DispatchEvent(Event<Key>{ EventType::KEY_RELEASED, (uint32_t) key });
				break;
			case GLFW_REPEAT:
				Keyboard::TimeStamp[key] = std::chrono::high_resolution_clock::now().time_since_epoch().count();
				Events::DispatchEvent(Event<Key>{ EventType::KEY_REPEATED, (uint32_t) key });
				break;
			}
		}
	);

	glfwSetMouseButtonCallback(window->window,
		[](GLFWwindow* window, int button, int action, int mods) {
			switch (action) {
			case GLFW_PRESS:
				Mouse::ButtonStates[button] = true;
				Events::DispatchEvent(Event<MouseButton>{ EventType::MOUSE_BUTTON_PRESSED, (uint32_t) button });
				break;
			case GLFW_RELEASE:
				Mouse::ButtonStates[button] = false;
				Events::DispatchEvent(Event<MouseButton>{ EventType::MOUSE_BUTTON_RELEASED, (uint32_t) button });
				break;
			}
		}
	);

	glfwSetCursorPosCallback(window->window,
		[](GLFWwindow* window, double xpos, double ypos) {
			float vals[4] = { xpos, ypos, xpos - Mouse::x, ypos - Mouse::y };
			Mouse::x = xpos;
			Mouse::y = ypos;
			Events::DispatchEvent(Event<float[4]>{ EventType::MOUSE_POSITION, vals });
		}
	);

	glfwSetWindowSizeCallback(window->window,
		[](GLFWwindow* glfwWindow, int width, int height) {
			Window* window = (Window*)glfwGetWindowUserPointer(glfwWindow);
			window->width = width;
			window->height = height;
			glViewport(0, 0, width, height);
		}
	);

	return window;
}

void Window::updateTitle(const std::string& appendage) {
	glfwSetWindowTitle(this->window, (this->baseTitle + appendage).c_str());
}

void Window::flush() {
	glfwSwapBuffers(window);
	glFlush();
}

Window::Window(const std::string& title, int width, int height, std::shared_ptr<Window> share) 
	: baseTitle(title), width(width), height(height) {
	static bool glfwInited = false;
	if (!glfwInited) {
		glfwSetErrorCallback([](int code, const char* msg) {
			throw msg;
		});
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwInited = true;
	}

	this->window = glfwCreateWindow(width, height, title.c_str(), NULL, share ? share->window : NULL);
	if (this->window == NULL) {
		fprintf(stderr, "Failed to create GLFW window\n");
		glfwTerminate();
		exit(1);
	} 
	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		fprintf(stderr, "Failed to initialize GLAD\n");
		exit(1);
	}
	glfwSwapInterval(0);
	this->isOpen = true;
}

GLFWwindow* Window::getWindowPtr() {
	return this->window;
}

Window::~Window() {
	glfwDestroyWindow(this->window);
}
