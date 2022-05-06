#pragma once

#include <src/engine/graphics/camera.h>

#include <glm/glm.hpp>

/**
 * @brief This struct should contain performance metrics data for a Window.
 */
struct PerformanceData {
	float frames;
};

/**
 * @brief This struct should contain all data related to a Window.
 */
struct WindowData {
	bool isOpen;
	const std::string title;
	const glm::vec2 initialSize;
	glm::vec2 size;
	glm::vec2 scale;

	Camera orthographicCamera;
	
	PerformanceData perf;

	WindowData(const std::string& title, const glm::vec2& size)
		: isOpen(false), title(title), initialSize(size), size(size), scale({1.f}),
		orthographicCamera({size.x / -2.f, size.x / 2.f, size.y / -2.f, size.y / 2.f}),
		perf({0.0f}) {}
};
