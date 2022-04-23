#include "camera.h"

#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(float left, float right, float bottom, float top)
	: projection(glm::ortho(left, right, bottom, top)), view(glm::identity<glm::mat4>()) {}

Camera::Camera(float width, float height, float fov, float near, float far)
	: projection(glm::perspectiveFov(fov, width, height, near, far)), view(glm::identity<glm::mat4>()) {
}

void Camera::updateProjection(const float width, const float height, const float fov, const float near, const float far) {
	this->projection = glm::perspectiveFov(fov, width, height, near, far);
}

CameraObject::CameraObject(const glm::vec2& windowDimensions, const float fov, const float near, const float far, const glm::vec3& position, const glm::vec3& dimensions)
	: CameraObject(windowDimensions, fov, near, far, position, position, dimensions) {}

CameraObject::CameraObject(const glm::vec2& windowDimensions, const float fov, const float near, const float far, const glm::vec3& position, const glm::vec3& center, const glm::vec3& dimensions)
	: CameraObject({windowDimensions.x, windowDimensions.y, fov, near, far}, position, center, dimensions) {}

CameraObject::CameraObject(const Camera& camera, const glm::vec3& position, const glm::vec3& dimensions)
	: CameraObject(camera, position, position, dimensions) {}

CameraObject::CameraObject(const Camera& camera, const glm::vec3& position, const glm::vec3& center, const glm::vec3& dimensions)
	: camera(camera), position(position),
	  boundBottomLeftNear(center - (dimensions / 2.0f)),
	  boundTopRightDeep(center + (dimensions / 2.0f)) {
	setCameraView();
}

void CameraObject::setPosition(const glm::vec3& position) {
	this->position.x = std::clamp(position.x, boundBottomLeftNear.x, boundTopRightDeep.x);
	this->position.y = std::clamp(position.y, boundBottomLeftNear.y, boundTopRightDeep.y);
	this->position.z = std::clamp(position.z, boundBottomLeftNear.z, boundTopRightDeep.z);
	setCameraView();
}

void CameraObject::setYaw(const float yaw) {
	this->focusTarget = std::nullopt;
	this->yaw = yaw;
	while (this->yaw < YAW_MIN) this->yaw += 360.0f;
	while (this->yaw > YAW_MAX) this->yaw -= 360.0f;
	setCameraView();
}

void CameraObject::setPitch(const float pitch) {
	this->pitch = pitch;
	while (this->pitch < PITCH_MIN) this->pitch += 360.0f;
	while (this->pitch > PITCH_MAX) this->pitch -= 360.0f;
	setCameraView();
}

void CameraObject::focusOn(const glm::vec3& target) {
	this->focusTarget = target;
	setCameraView();
}

void CameraObject::moveForward(float amount) {
	this->position += this->front * amount;
	setCameraView();
}

void CameraObject::moveStrafe(float amount) {
	this->position += this->right * amount;
	setCameraView();
}

void CameraObject::moveVertical(float amount) {
	this->position += this->up * amount;
	setCameraView();
}

void CameraObject::move(float amount, MoveDirection direction) {
	switch (direction) {
		case MoveDirection::FORWARDS:
			moveForward(amount);
			break;
		case MoveDirection::BACKWARDS:
			moveForward(-amount);
			break;
		case MoveDirection::LEFT:
			moveStrafe(amount);
			break;
		case MoveDirection::RIGHT:
			moveStrafe(-amount);
			break;
		case MoveDirection::UP:
			moveVertical(amount);
			break;
		case MoveDirection::DOWN:
			moveVertical(-amount);
			break;
		default:
			throw "Unknown MoveDirection";
	}
}

void CameraObject::updateProjection(const glm::vec2& dimensions, const float fov, const float near, const float far) {
	this->camera.updateProjection(dimensions.x, dimensions.y, fov, near, far);
}

void CameraObject::setCameraView() {
	if (this->focusTarget.has_value()) {
		this->direction = this->focusTarget.value() - this->position;
		this->front = glm::normalize(this->direction);
		this->yaw = glm::degrees(std::atan2(this->front.x, this->front.z));
		this->pitch = glm::degrees(std::asin(this->front.y));
	} else {
		this->direction.x = std::cos(glm::radians(this->yaw)) * std::cos(glm::radians(this->pitch));
		this->direction.y = std::sin(glm::radians(this->pitch));
		this->direction.z = std::sin(glm::radians(this->yaw)) * std::cos(glm::radians(this->pitch));
		this->front = glm::normalize(this->direction);
	}
	this->right = glm::normalize(glm::cross(DEFAULT_UP_DIR, this->direction));
	this->up = glm::cross(this->direction, this->right);
	glm::mat4 lookAt = glm::lookAt(this->position, this->position + this->front, this->up);
	this->camera.view = lookAt;
}
