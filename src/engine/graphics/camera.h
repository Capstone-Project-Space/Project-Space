#pragma once

#include <cmath>
#include <optional>

#include <glm/glm.hpp>

class Camera {
public:
	Camera() = default;
	Camera(float left, float right, float bottom, float top);
	Camera(float width, float height, float fov, float near, float far);

	const inline glm::mat4& getProjection() const { return projection; }
	const inline void setProjection(glm::mat4 projection) { this->projection = projection; }
	const inline glm::mat4& getView() const { return view; }
	const inline void setView(glm::mat4 view) { this->view = view; }

	const inline glm::mat4& getProjectionView() const { return projection * view; }

private:
	friend class CameraObject;
	glm::mat4 projection;
	glm::mat4 view;

	void updateProjection(const float width, const float height, const float fov, const float near, const float far);
};

enum class MoveDirection { FORWARDS, BACKWARDS, LEFT, RIGHT, UP, DOWN };

constexpr float YAW_MIN = -180.0f, YAW_MAX = 180.0f;
constexpr float PITCH_MIN = -90.0f, PITCH_MAX = 90.0f;
constexpr glm::vec3 DEFAULT_UP_DIR{ 0.0f, 1.0f, 0.0f };
class CameraObject {
public:
	CameraObject(const glm::vec2& windowDimensions, const float fov, const float near, const float far, const glm::vec3& position, const glm::vec3& dimensions);
	CameraObject(const glm::vec2& windowDimensions, const float fov, const float near, const float far, const glm::vec3& position, const glm::vec3& center, const glm::vec3& dimensions);
	CameraObject(const Camera& camera, const glm::vec3& position, const glm::vec3& dimensions);
	CameraObject(const Camera& camera, const glm::vec3& position, const glm::vec3& center, const glm::vec3& dimensions);

	explicit inline operator Camera() { return this->camera; }

	const inline glm::vec3& getPosition() const { return this->position; }
	void setPosition(const glm::vec3& position);

	const inline glm::vec2& getPitchYaw() const { return { this->pitch, this->yaw }; }
	const inline float getYaw() const { return this->yaw; }
	const inline float getPitch() const { return this->pitch; }
	void setYaw(const float yaw);
	void setPitch(const float pitch);

	const inline glm::vec3& getDirection() const { return -this->front; }
	const inline glm::vec3& getRight() const { return this->right; }
	const inline glm::vec3& getUp() const { return this->up; }

	const inline Camera& getCamera() const { return this->camera; }

	std::optional<glm::vec3> getTarget() { return this->focusTarget; }
	void setTarget(std::optional<glm::vec3> focusTarget) { this->focusTarget = focusTarget; }
	bool hasTarget() { return (getTarget() == std::nullopt ? false : true); }

	void focusOn(const glm::vec3& target);
	void moveForward(float amount);
	void moveStrafe(float amount);
	void moveVertical(float amount);
	void move(float amount, MoveDirection direction);

	void updateProjection(const glm::vec2& dimensions, const float fov, const float near, const float far);

	float scale = 1.0f;


private:
	Camera camera;
	glm::vec3 position;
	const glm::vec3 boundBottomLeftNear, boundTopRightDeep;

	std::optional<glm::vec3> focusTarget{};

	float yaw = 0.0f;
	float pitch = 0.0f;
	glm::vec3 direction;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 front;


	void setCameraView();

};