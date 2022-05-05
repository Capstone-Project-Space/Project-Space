#pragma once

#include <cmath>
#include <optional>

#include <glm/glm.hpp>

class Camera {
public:
	Camera() = default;

	/**
	 * @brief Construct a new Orthographic style Camera. 
	 * 
	 * @param left The left most coordinate on the window.
	 * @param right The right most coordinate on the window.
	 * @param bottom The bottom most coordinate on the window.
	 * @param top The top most coordinate on the window.
	 */
	Camera(float left, float right, float bottom, float top);

	/**
	 * @brief Construct a new Perspective style Camera.
	 * 
	 * @param width The width of the window.
	 * @param height The height of the window.
	 * @param fov The field of view of the Camera.
	 * @param near The near plane of the camera.
	 * @param far The far plane of the camera.
	 */
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

	/**
	 * @brief Used to update only the projection of an orthographic projection.
	 */
	void updateProjection(const float left, const float right, const float bottom, const float top);

	/**
	 * @brief Used to update only the projection of a perspective projection.
	 */
	void updateProjection(const float width, const float height, const float fov, const float near, const float far);
};

enum class MoveDirection { FORWARDS, BACKWARDS, LEFT, RIGHT, UP, DOWN };

constexpr float YAW_MIN = -180.0f, YAW_MAX = 180.0f;
constexpr float PITCH_MIN = -90.0f, PITCH_MAX = 90.0f;
constexpr glm::vec3 DEFAULT_UP_DIR{ 0.0f, 1.0f, 0.0f };
class CameraObject {
public:
	/**
	 * @brief Construct a new Camera Object.
	 * 
	 * @param windowDimensions The width and height of the window.
	 * @param fov The field of view of the camera.
	 * @param near The near plane of the camera.
	 * @param far The far plane of the camera.
	 * @param position The initial position of the camera.
	 * @param dimensions The bounding box of width, height, and depth centered at position.
	 */
	CameraObject(const glm::vec2& windowDimensions, const float fov, const float near, const float far, const glm::vec3& position, const glm::vec3& dimensions);
	
	/**
	 * @brief Construct a new Camera Object.
	 * 
	 * @param windowDimensions The width and height of the window.
	 * @param fov The field of view of the camera.
	 * @param near The near plane of the camera.
	 * @param far The far plane of the camera.
	 * @param position The initial position of the camera.
	 * @param center The center of the bounding box area,
	 * @param dimensions The bounding box of width, height, and depth centered at position.
	 */
	CameraObject(const glm::vec2& windowDimensions, const float fov, const float near, const float far, const glm::vec3& position, const glm::vec3& center, const glm::vec3& dimensions);
	
	/**
	 * @brief Construct a new Camera Object.
	 * 
	 * @param camera The camera we will copy and use.
	 * @param position The initial position of the camera.
	 * @param dimensions The bounding box of width, height, and depth centered at position.
	 */
	CameraObject(const Camera& camera, const glm::vec3& position, const glm::vec3& dimensions);

	/**
	 * @brief Construct a new Camera Object.
	 * 
	 * @param camera The camera we will copy and use.
	 * @param position The initial position of the camera.
	 * @param center The center of the bounding box area,
	 * @param dimensions The bounding box of width, height, and depth centered at position.
	 */
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

	const std::optional<glm::vec3>& getTarget() { return this->focusTarget; }
	bool hasTarget() { return this->focusTarget.has_value(); }

	/**
	 * @brief Focus this camera on a specific position in space.
	 * 
	 * @param target The location to focus on.
	 */
	void focusOn(const glm::vec3& target);

	/**
	 * @brief Move the camera forward.
	 * 
	 * @param amount The distance to move forward.
	 */
	void moveForward(float amount);

	/**
	 * @brief Move the camera laterally(horizontally).
	 * 
	 * @param amount The distance to move laterally.
	 */
	void moveStrafe(float amount);

	/**
	 * @brief Move the camera vertically.
	 * 
	 * @param amount The distance to move vertically.
	 */
	void moveVertical(float amount);

	/**
	 * @brief Move the camera in a direction.
	 * 
	 * @param amount The distance to move.
	 * @param direction The direction to move the camera.
	 */
	void move(float amount, MoveDirection direction);

	/**
	 * @brief Update the projection matrix of the camera.
	 * 
	 * @param dimensions The dimensions of the window.
	 * @param fov The field of view.
	 * @param near The near plane of the camera.
	 * @param far The far plane of the camera.
	 */
	void updateProjection(const glm::vec2& dimensions, const float fov, const float near, const float far);

private:
	Camera camera;
	glm::vec3 position;
	const glm::vec3 boundBottomLeftNear, boundTopRightDeep;

	std::optional<glm::vec3> focusTarget{};

	float scale = 1.0f;
	float yaw = 0.0f;
	float pitch = 0.0f;
	glm::vec3 direction;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 front;


	void setCameraView();

};