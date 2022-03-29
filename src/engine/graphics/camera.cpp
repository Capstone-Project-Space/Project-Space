#include "camera.h"

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(float width, float height)
	: projection(glm::ortho(-width / height, width / height, -1.0f, 1.0f)), view(glm::identity<glm::mat4>()) {}

Camera::Camera(float width, float height, float fov, float near, float far)
	: projection(glm::perspectiveFov(fov, width, height, near, far)), view(glm::identity<glm::mat4>()) {
	view = glm::scale(view, { .03f, .03f, .03f });
	view = glm::translate(view, { 0.0f, 0.0f, -2.0f });
	view = glm::rotate(view, glm::radians(45.0f), { 1.0f, 0.0f, 0.0f });
}
