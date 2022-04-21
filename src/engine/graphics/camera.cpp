#include "camera.h"

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(float left, float right, float bottom, float top)
	: projection(glm::ortho(left, right, bottom, top)), view(glm::identity<glm::mat4>()) {}

Camera::Camera(float width, float height, float fov, float near, float far)
	: projection(glm::perspectiveFov(fov, width, height, near, far)), view(glm::identity<glm::mat4>()) {
	view = glm::scale(view, { .03f, .03f, .03f });
	view = glm::translate(view, { 0.0f, 0.0f, -2.0f });
	view = glm::rotate(view, glm::radians(45.0f), { 1.0f, 0.0f, 0.0f });
}

CameraObject::CameraObject(glm::vec3 position, glm::vec3 target) {
	this->position = position;
	this->target = target;
	direction = glm::normalize(position - target);
	cameraRight = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), direction));
	cameraUp = glm::cross(direction, cameraRight);
	cameraFront = glm::vec3(-1.0f, 0.0f, -1.0f);
	cameraSpeed = 0.5f;
	cameraFOV = 70.0f;
	cameraYaw = 0.0f;
	cameraLock = true;
}

void CameraObject::moveCamera(float cameraSpeed, glm::vec3 moveDirection) {
	position += cameraSpeed * moveDirection;
	if (!cameraLock) {
		target += cameraSpeed * moveDirection;
	}
}

void CameraObject::moveCameraMouse(float dx, float dy, float speed) {  
	moveCamera(speed, -dx * glm::normalize(glm::cross(getCameraFront(), getCameraUp())));
	moveCamera(speed, dy * getCameraFront());
}