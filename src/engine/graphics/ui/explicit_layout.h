#pragma once

#include <variant>

#include <src/engine/graphics/ui/layout.h>

/**
 * @brief ExplicitLayout allows you to position a UIComponent to a specific x, y, z on the window or by percentage of the windows size. Also allows the width and height to be specified.
 * Format for a value is "###", "###.", "###.###" or for percentage add a '%' to the end.
 * Positions of x and y by percentage are 0% = left or top respectively.
 * x, y positions in 2D space have 0, 0 at the center of the window.
 * An ExplicitLayout must have an x, y, and z.
 */
class ExplicitLayout : public Layout {
public:
	using Literal = float;
	using Percentage = float;
	ExplicitLayout(Literal x, Literal y, Literal width, Literal height, Literal z = 0.0f);
	ExplicitLayout(const std::string_view& x, const std::string_view& y, const std::string_view& width, const std::string_view& height, const std::string_view& z = "0.0");

	operator Layout*() {
		return static_cast<Layout*>(this);
	}

	virtual glm::vec3 position(
		const std::shared_ptr<Window> window, const UIComponent& self,
		const std::unordered_map<std::string_view, std::shared_ptr<UIComponent>>& peers
	) override;

	virtual glm::vec2 size(
		const std::shared_ptr<Window> window, const UIComponent& self,
		const std::unordered_map<std::string_view, std::shared_ptr<UIComponent>>& peers
	) override;

private:
	std::variant<Literal, Percentage> x, y, z;
	std::optional<std::variant<Literal, Percentage>> width, height;

	static std::optional<std::variant<Literal, Percentage>> GetValue(const std::string_view& view);

};
