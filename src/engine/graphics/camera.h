#pragma once

#include <glm/glm.hpp>

class Camera {
public:
	Camera(float width, float height);
	Camera(float width, float height, float fov, float near, float far);

	const inline glm::mat4& getProjection() const { return projection; }
	const inline glm::mat4& getView() const { return view; }

	const inline glm::mat4& getProjectionView() const { return projection * view; }

private:
	glm::mat4 projection;
	glm::mat4 view;
};
