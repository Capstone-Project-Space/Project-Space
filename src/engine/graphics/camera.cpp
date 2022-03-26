#include "camera.h"

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera() : projection(glm::perspectiveFov(70.0f, 1280.0f, 720.0f, .01f, 1000.0f)), view(glm::identity<glm::mat4>()) {
	view = glm::scale(view, { .03f, .03f, .03f });
	view = glm::translate(view, {0.0f, 0.0f, -2.0f});
	view = glm::rotate(view, glm::radians(45.0f), { 1.0f, 0.0f, 0.0f });
}
