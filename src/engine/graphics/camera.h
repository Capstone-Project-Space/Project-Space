#pragma once

#define _USE_MATH_DEFINES
#include <math.h>

#include <glm/glm.hpp>

class Camera {
public:
	Camera(float left, float right, float bottom, float top);
	Camera(float width, float height, float fov, float near, float far);

	const inline glm::mat4& getProjection() const { return projection; }
	const inline void setProjection(glm::mat4 projection) { this->projection = projection; }
	const inline glm::mat4& getView() const { return view; }
	const inline void setView(glm::mat4 view) { this->view = view; }

	const inline glm::mat4& getProjectionView() const { return projection * view; }

private:
	glm::mat4 projection;
	glm::mat4 view;
};

class CameraObject {
public:
	CameraObject(glm::vec3 position, glm::vec3 target);

	const inline glm::vec3& getPosition() const { return position; }
	const inline void setPosition(glm::vec3 position) {

		if((position.x < 25.0f && position.x > -25.0f) && (position.y < 25.0f && position.y > -25.0f))this->position = position;

		if (this->position.x < -25.0f) this->position.x = -25.0f;
		if (this->position.x >  25.0f) this->position.x =  25.0f;
		if (this->position.y < -25.0f) this->position.y = -25.0f;
		if (this->position.y >  25.0f) this->position.y =  25.0f;
		if (this->position.z < -25.0f) this->position.z = -25.0f;
		if (this->position.z >  25.0f) this->position.z =  25.0f;
	}
	const inline glm::vec3& getTarget() const { return target; }
	const inline void setTarget(glm::vec3 target) { this->target = target; }
	const inline glm::vec3& getDirection() const { return direction; }
	const inline void setDirection(glm::vec3 direction) { this->direction = direction; }
	const inline glm::vec3& getCameraUp() const { return cameraUp; }
	const inline void setCameraUp(glm::vec3 cameraUp) { this->cameraUp = cameraUp; }
	const inline glm::vec3& getCameraRight() const { return cameraRight; }
	const inline void setCameraRight(glm::vec3 cameraRight) { this->cameraRight = cameraRight; }
	const inline glm::vec3& getCameraFront() const { return cameraFront; }
	const inline void setCameraFront(glm::vec3 cameraFront) { this->cameraFront = cameraFront; }
	const inline float getCameraSpeed() { return cameraSpeed; }
	const inline void setCameraSpeed(float cameraSpeed) {
		this->cameraSpeed = cameraSpeed;
		if (this->cameraSpeed <= 0.1f) this->cameraSpeed = 0.1f;
		if (this->cameraSpeed >= 5.0f) this->cameraSpeed = 5.0f;
	}
	const inline float getCameraFOV() { return cameraFOV; }
	const inline void setCameraFOV(float cameraFOV) {
		this->cameraFOV = cameraFOV;
		if (this->cameraFOV <= 15.0f) this->cameraFOV = 15.0f;
		if (this->cameraFOV >= 110.0f) this->cameraFOV = 110.0f;
	}
	const inline float getCameraYaw() { return cameraYaw; }
	const inline void setCameraYaw(float cameraYaw) {
		this->cameraYaw = cameraYaw;
		if (this->cameraYaw <= 0.0f) this->cameraYaw += (2 * M_PI);
		if (this->cameraYaw >= (2 * M_PI)) this->cameraYaw -= (2 * M_PI);
	}
	const inline bool getCameraLock() { return cameraLock; }
	const inline void setCameraLock(bool cameraLock) { this->cameraLock = cameraLock; }

	void moveCamera(float, glm::vec3);
	void moveCameraMouse(float, float, float);

private:
	glm::vec3 position;
	glm::vec3 target;
	glm::vec3 direction;
	glm::vec3 cameraUp;
	glm::vec3 cameraRight;
	glm::vec3 cameraFront;

	float cameraSpeed;
	float cameraFOV;
	float cameraYaw;

	bool cameraLock;
};