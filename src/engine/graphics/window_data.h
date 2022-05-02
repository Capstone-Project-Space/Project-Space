#pragma once

#include <src/engine/graphics/camera.h>

#include <glm/glm.hpp>

struct PerformanceData {
	float frames;
};

struct WindowData {
	bool isOpen;
	const std::string title;
	glm::vec2 size;

	Camera orthographicCamera;
	
	PerformanceData perf;

	WindowData(const std::string& title, const glm::vec2& size)
		: isOpen(false), title(title), size(size),
		orthographicCamera({size.x / -2.f, size.x / 2.f, size.y / -2.f, size.y / 2.f}),
		perf({0.0f}) {}
};
