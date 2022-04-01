#pragma once

#include <glm/glm.hpp>

struct PerformanceData {
	float frames;
};

struct WindowData {
	bool isOpen;
	const std::string title;
	glm::vec2 size;
	
	PerformanceData perf;
};