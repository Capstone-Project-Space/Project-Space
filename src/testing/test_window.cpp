#include "test_window.h"

#include <src/engine/graphics/window.h>

TestResult WN_Test_Creation() {
	TestResult result{"Create a Window"};
	std::shared_ptr<Window> window = Window::CreateGLWindow("Project Space", 1920, 1080);

	if (window == nullptr) {
		result.msg = "Window was still nullptr after creation";
		return result;
	}

	window.~shared_ptr();
	glfwTerminate();
	result.succeeded = true;
	return result;
}
